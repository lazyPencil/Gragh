#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Prob4_Weight_gragh_adjacency_list.h"

void BFS(vertex* v1, queue* pQ);
void DFS(gragh* g1, vertex* v1);
void RestoreGragh(gragh* g1);
void MST(gragh* g1, gragh* Result);
edge* searchedge(vertex* srcV, vertex* findV);
void printMST(gragh* Result, vertex* v1);
void shortpath(gragh* g1, vertex* ver);
int allvisited(gragh* g1);
vertex* searchvertexindex(vertex* v1, int index);

int main(void) {
	int idx = 0, error,weight;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]짜리가 10개
	gragh* g1 = NULL;
	gragh* Result = NULL;
	queue* pQ = NULL;
	FILE* fp = fopen("Prob4_Input.txt", "r");
	if (fp == NULL) {
		printf("fopen Error\n");
		return -1;
	}
	g1 = creategragh();
	pQ = createQ(10);
	Result = creategragh();
	if (!g1)
		printf("g1 memory allocation problem\n");
	else if (!pQ) {
		printf("pQ memory allocation problem\n");
		free(g1);
	}
	else if (!Result) {
		printf("Result memory allocation problem\n");
		free(pQ);
		free(g1);
	}
	else {
		fscanf(fp, "%c", &source[idx]);
		g1 = insertvertex(g1, &source[idx++]);
		fgetc(fp); // 공백 한 줄에 잘 작동하는지 체크->11.27에러:공백 한 줄 더 받아라
		fgetc(fp);
		while (!feof(fp)) { // 11.27에러: vertex를 안 만들어놓아서 만들기 실패 -> addedge 전에 vertex 있는지 확인해보자 -> 구현했음
			error = fscanf(fp, "%c %c %d", &source2[0], &source2[1], &weight);
			fgetc(fp);
			if (error != 3)
				printf("fscanf) 인자 갯수에 문제가 있네요?\n");
			else {
				printf("fscanf) 결과 : %c %c %d\n", source2[0], source2[1], weight);
				if (searchvertex(g1->first, &source2[0]) == NULL) {
					source[idx] = source2[0];
					g1 = insertvertex(g1, &source[idx++]);
				}
				if (searchvertex(g1->first, &source2[1]) == NULL) {
					source[idx] = source2[1];
					g1 = insertvertex(g1, &source[idx++]);
				}
				g1 = addedge(g1, &source2[0], &source2[1], weight);
			}
		}
		printarc(g1->first); //내가 확인하려고
		puts("");
		printf("An example of print out :\n\nMinnimum Spanning Tree : \n");
		MST(g1, Result);
		printMST(Result,Result->first);
		RestoreGragh(g1);
		printf("\nShortest paths from %c : \n",*(char*)g1->first->nameptr);
		shortpath(g1, g1->first);
		printf("\nThank You\n");
		free(pQ);
		deletegragh(g1);
		deletegragh(Result);
	}
	fclose(fp);
	return 0;
}

void BFS(vertex* v1, queue* pQ) { // Recursion Model
	vertex* vTemp = NULL;
	edge* eTemp = NULL;
	if (!v1 || v1->visited == true) return;
	printf("%c ", *(char*)v1->nameptr);
	v1->visited = true;
	eTemp = v1->outdeg;
	while (eTemp) {
		enqueue(pQ, eTemp->src);
		eTemp = eTemp->next;
	}
	while (isemptyqueue(pQ) != true) {
		vTemp = dequeue(pQ);
		BFS(vTemp, pQ);
	}
	return;
}
void DFS(gragh* g1, vertex* v1) {
	edge* eTemp = NULL;
	if (!v1 || v1->visited == true) {
		return;
	}
	else {
		printf("%c ", *(char*)v1->nameptr);
		v1->visited = true;
		if ((eTemp = v1->outdeg) != NULL) { // General case
			while (eTemp) {
				DFS(g1, eTemp->src);
				eTemp = eTemp->next;
			}
			return;
		}
		else// Base case
			return;
	}
}
void RestoreGragh(gragh* g1) {
	vertex* vTemp = g1->first;
	if (vTemp == NULL)
		;
	else {
		while (vTemp) {
			vTemp->visited = false;
			vTemp = vTemp->next;
		}
	}
}
void MST(gragh* g1, gragh* Result) { // 비어있던 Result는 MST 결과 체워진다. // g1는 Undirect 지만 Result는 direct // main에서 Result 삭제 시 유의
	vertex* vTemp = NULL;
	vertex* orivTemp = NULL;
	vertex* minvertex = NULL;
	edge* eTemp = NULL;
	edge* minedge = NULL;
	int idx = 0, jdx = 0, min = 200000000;
	Result = insertvertex(Result, g1->first->nameptr);
	while (g1->cnt != Result->cnt) { // 이외에도 동등한 조건은 많음
		vTemp = Result->first;
		while (vTemp) { // !?
			orivTemp = searchvertex(g1->first, vTemp->nameptr);
			orivTemp->visited = true;
			if ((eTemp = orivTemp->outdeg) == NULL);
			else {
				while (eTemp) {
					if (searchedge(vTemp, eTemp->src) != NULL); // 연결되어있는건(트리안에있는건) 확인하면 안됨(search에 구현)
					else {
						if (eTemp->weight < min && eTemp->src->visited == false) {
							minedge = eTemp; // eTemp 는 g1에 있는 녀석임 !!
							minvertex = orivTemp; // orivTemp 는 g1에 있는 녀석임 !!
							min = minedge->weight;
						}
					}
					eTemp = eTemp->next;
				}
			}
			vTemp = vTemp->next;
		}// 만약 minedge , minvertex 가 NULL이면 그건 g1을 잘못 입력한 것
		//printf("Vertex %c 에서 Vertex %c가 min이므로 추가해서 이어주겠습니다\n", *(char*)minvertex->nameptr, *(char*)minedge->src->nameptr);
		Result = insertvertex(Result, minedge->src->nameptr);
		Result = addedge(Result, minvertex->nameptr, minedge->src->nameptr, minedge->weight);
		min = 200000000;
		minedge = NULL;
		minvertex = NULL;
	}
		/*//이제 vertex 생성, minvertex->(minedge->src)) 방향의 edge를 생성 , minedge->src->visited = true.
		if (minedge != NULL && minvertex != NULL) {// 필요없을것같은데 밑줄떠서..
			Result = insertvertex(Result, minedge->src->nameptr);
			Result = addedge(Result, minvertex->nameptr, minedge->src->nameptr, min);
			minedge->src->visited = true;
			min = 200000000;
			minedge = NULL;
			minvertex = NULL;
		}*/
}
edge* searchedge(vertex* srcV, vertex* findV) { // srcV->edgeptr->dst 중에 findV vertex가 있습니까?
	edge* eTemp = srcV->outdeg;//11.26에러: A,B,C 추가 A->B,A->C,B->A,C->A 한 뒤 A를 지우니까 eTemp 가 0xdddddddd 만남
	if (*(char*)srcV->nameptr == *(char*)findV->nameptr)
		return srcV->nameptr;
	while (eTemp) {
		if (*(char*)eTemp->src->nameptr == *(char*)findV->nameptr) // 찾음
			break;
		eTemp = eTemp->next;
	}
	return eTemp;
}
void printMST(gragh* Result, vertex* v1) {
	edge* eTemp = NULL;
	if (v1 == NULL)
		return;
	eTemp = v1->outdeg;
	while (eTemp) {
		printf("%c %c\n", *(char*)v1->nameptr, *(char*)eTemp->src->nameptr);
		eTemp = eTemp->next;
	}
	printMST(Result, v1->next);
}
void shortpath(gragh* g1, vertex* ver) {
	edge* eTemp = NULL;
	vertex* startV = ver;
	int i,minD,startidx,minidx,NewarrD;
	int* arrD = (int*)calloc(g1->cnt, sizeof(int));
	if (!arrD) return;
	for (i = 0; i < g1->cnt; i++) {
		if (i == startV->index) {
			arrD[i] = 0; // 시작지점이니까
			startidx = startV->index;
		}
		else
			arrD[i] = 200000000;
	}
	while (allvisited(g1) != 1) {
		if (ver == NULL) {
			printf("여기를 방문한다면 당신은 코드를 잘못 짰습니다\n");
			break;
		}
		ver->visited = true;
		eTemp = ver->outdeg; // 11.27오류:C에서 A가 보이면 안되는데 보인다->처리함
		minD = 200000000;
		while (eTemp) {
			if (eTemp->src->visited == false) {
				NewarrD = arrD[ver->index] + eTemp->weight;
				if (NewarrD < arrD[eTemp->src->index])
					arrD[eTemp->src->index] = NewarrD;
			}
			eTemp = eTemp->next;
		}
		for (i = 0; i < g1->cnt; i++) {
			ver = searchvertexindex(g1->first, i);
			if (arrD[i] != 0 && arrD[i] < minD && ver->visited==false) {
				minD = arrD[i];
				minidx = i;
			}
		}// 다음 방문할 최소거리 = minD , 인덱스 = minidx
		ver = searchvertexindex(g1->first, minidx);
	}
	for (i = 0; i < g1->cnt; i++) {
		if (i!=startidx) {
			ver = searchvertexindex(g1->first, i);
			printf("%c 에서 %c 최단거리 : %d\n", *(char*)startV->nameptr, *(char*)ver->nameptr, arrD[i]);
		}
	}
	free(arrD);
}

int allvisited(gragh* g1) {
	vertex* vTemp = NULL;
	if (g1 == NULL)
		return 0;
	if ((vTemp = g1->first) == NULL)
		return 0;
	while (vTemp) {
		if (vTemp->visited == false)
			return 0;
		vTemp = vTemp->next;
	}
	return 1; // 모두 visited
}

vertex* searchvertexindex(vertex* v1, int findidx) {
	while (v1) {
		if (v1->index == findidx)
			return v1;
		v1 = v1->next;
	}
	return NULL;
}