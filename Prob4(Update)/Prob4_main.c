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
	char source2[2] = { 0, }; // char[2]¥���� 10��
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
		fgetc(fp); // ���� �� �ٿ� �� �۵��ϴ��� üũ->11.27����:���� �� �� �� �޾ƶ�
		fgetc(fp);
		while (!feof(fp)) { // 11.27����: vertex�� �� �������Ƽ� ����� ���� -> addedge ���� vertex �ִ��� Ȯ���غ��� -> ��������
			error = fscanf(fp, "%c %c %d", &source2[0], &source2[1], &weight);
			fgetc(fp);
			if (error != 3)
				printf("fscanf) ���� ������ ������ �ֳ׿�?\n");
			else {
				printf("fscanf) ��� : %c %c %d\n", source2[0], source2[1], weight);
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
		printarc(g1->first); //���� Ȯ���Ϸ���
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
void MST(gragh* g1, gragh* Result) { // ����ִ� Result�� MST ��� ü������. // g1�� Undirect ���� Result�� direct // main���� Result ���� �� ����
	vertex* vTemp = NULL;
	vertex* orivTemp = NULL;
	vertex* minvertex = NULL;
	edge* eTemp = NULL;
	edge* minedge = NULL;
	int idx = 0, jdx = 0, min = 200000000;
	Result = insertvertex(Result, g1->first->nameptr);
	while (g1->cnt != Result->cnt) { // �̿ܿ��� ������ ������ ����
		vTemp = Result->first;
		while (vTemp) { // !?
			orivTemp = searchvertex(g1->first, vTemp->nameptr);
			orivTemp->visited = true;
			if ((eTemp = orivTemp->outdeg) == NULL);
			else {
				while (eTemp) {
					if (searchedge(vTemp, eTemp->src) != NULL); // ����Ǿ��ִ°�(Ʈ���ȿ��ִ°�) Ȯ���ϸ� �ȵ�(search�� ����)
					else {
						if (eTemp->weight < min && eTemp->src->visited == false) {
							minedge = eTemp; // eTemp �� g1�� �ִ� �༮�� !!
							minvertex = orivTemp; // orivTemp �� g1�� �ִ� �༮�� !!
							min = minedge->weight;
						}
					}
					eTemp = eTemp->next;
				}
			}
			vTemp = vTemp->next;
		}// ���� minedge , minvertex �� NULL�̸� �װ� g1�� �߸� �Է��� ��
		//printf("Vertex %c ���� Vertex %c�� min�̹Ƿ� �߰��ؼ� �̾��ְڽ��ϴ�\n", *(char*)minvertex->nameptr, *(char*)minedge->src->nameptr);
		Result = insertvertex(Result, minedge->src->nameptr);
		Result = addedge(Result, minvertex->nameptr, minedge->src->nameptr, minedge->weight);
		min = 200000000;
		minedge = NULL;
		minvertex = NULL;
	}
		/*//���� vertex ����, minvertex->(minedge->src)) ������ edge�� ���� , minedge->src->visited = true.
		if (minedge != NULL && minvertex != NULL) {// �ʿ�����Ͱ����� ���ٶ���..
			Result = insertvertex(Result, minedge->src->nameptr);
			Result = addedge(Result, minvertex->nameptr, minedge->src->nameptr, min);
			minedge->src->visited = true;
			min = 200000000;
			minedge = NULL;
			minvertex = NULL;
		}*/
}
edge* searchedge(vertex* srcV, vertex* findV) { // srcV->edgeptr->dst �߿� findV vertex�� �ֽ��ϱ�?
	edge* eTemp = srcV->outdeg;//11.26����: A,B,C �߰� A->B,A->C,B->A,C->A �� �� A�� ����ϱ� eTemp �� 0xdddddddd ����
	if (*(char*)srcV->nameptr == *(char*)findV->nameptr)
		return srcV->nameptr;
	while (eTemp) {
		if (*(char*)eTemp->src->nameptr == *(char*)findV->nameptr) // ã��
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
			arrD[i] = 0; // ���������̴ϱ�
			startidx = startV->index;
		}
		else
			arrD[i] = 200000000;
	}
	while (allvisited(g1) != 1) {
		if (ver == NULL) {
			printf("���⸦ �湮�Ѵٸ� ����� �ڵ带 �߸� ®���ϴ�\n");
			break;
		}
		ver->visited = true;
		eTemp = ver->outdeg; // 11.27����:C���� A�� ���̸� �ȵǴµ� ���δ�->ó����
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
		}// ���� �湮�� �ּҰŸ� = minD , �ε��� = minidx
		ver = searchvertexindex(g1->first, minidx);
	}
	for (i = 0; i < g1->cnt; i++) {
		if (i!=startidx) {
			ver = searchvertexindex(g1->first, i);
			printf("%c ���� %c �ִܰŸ� : %d\n", *(char*)startV->nameptr, *(char*)ver->nameptr, arrD[i]);
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
	return 1; // ��� visited
}

vertex* searchvertexindex(vertex* v1, int findidx) {
	while (v1) {
		if (v1->index == findidx)
			return v1;
		v1 = v1->next;
	}
	return NULL;
}