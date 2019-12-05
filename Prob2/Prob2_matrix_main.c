#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Prob2_Graph_adjacency_matrix.h"

void BFS(vertex* v1, queue* pQ);
void DFS(gragh* g1, vertex* v1);
void RestoreGragh(gragh* g1);

int main(void) {
	int idx = 0, error;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]짜리가 10개
	gragh* g1 = NULL;
	queue* pQ = NULL;
	FILE* fp = fopen("Prob2_Input.txt", "r");
	if (fp == NULL) {
		printf("fopen Error\n");
		return -1;
	}
	g1 = creategragh();
	pQ = createQ(10);
	if (!g1)
		printf("g1 memory allocation problem\n");
	else if (!pQ) {
		printf("pQ memory allocation problem\n");
		free(g1);
	}
	else {
		fscanf(fp, "%c", &source[idx]);
		addvertex(g1, &source[idx++]);
		fgetc(fp); // 공백 한 줄에 잘 작동하는지 체크->11.27에러:공백 한 줄 더 받아라
		fgetc(fp);
		while (!feof(fp)) { // 11.27에러: vertex를 안 만들어놓아서 만들기 실패 -> addedge 전에 vertex 있는지 확인해보자 -> 구현했음
			error = fscanf(fp, "%c %c", &source2[0], &source2[1]);
			fgetc(fp);
			if (error != 2)
				printf("fscanf) 인자 갯수에 문제가 있네요?\n");
			else {
				printf("fscanf) 결과 : %c %c\n", source2[0], source2[1]);
				if (searchvertex(g1->first, &source2[0]) == NULL) {
					source[idx] = source2[0];
					addvertex(g1, &source[idx++]);
				}
				if (searchvertex(g1->first, &source2[1]) == NULL) {
					source[idx] = source2[1];
					addvertex(g1, &source[idx++]);
				}
				addedge(g1, &source2[0], &source2[1]);
			}
		}
		printf("An example of print out :\nDFS : ");
		DFS(g1, g1->first);
		RestoreGragh(g1);
		printf("\nBFS : ");
		BFS(g1->first, pQ);
		printf("\nThank You\n");
		free(pQ);
		subgragh(g1);
	}
	fclose(fp);
	return 0;
	/*int menu, idx = 0, error;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]짜리가 10개
	gragh* g1 = creategragh();
	queue* pQ = createQ(10);
	if (!g1) return -1;
	if (!pQ) {
		free(g1);
		return -1;
	}
	while (1) {
		printf("addvertex(0),addedge(1),subvertex(2),subedge(3),DFS(4),BFS(5),Exit(6)\n");
		scanf("%d", &menu);
		getchar();
		if (menu == 6)
			break;
		switch (menu) {
		case 0:
			printf("만들 Vertex 입력: ");
			scanf("%c", &source[idx]);
			getchar();
			addvertex(g1, &source[idx]);
			idx++;
			printmatrix(g1);
			break;
		case 1:
			printf("만들 Edge을 %%c %%c 형식으로 입력: ");
			scanf("%c %c", &source2[0], &source2[1]);
			addedge(g1, &source2[0], &source2[1]);
			printmatrix(g1);
			break;
		case 2:
			printf("지울 Vertex 입력: ");
			scanf("%c", &source2[0]);
			error = subvertex(g1, &source2[0]);
			if (error != 1)
				printf("main) subvertex의 반환값이 비정상\n");
			printmatrix(g1);
			break;
		case 3:
			printf("지울 Edge를 %%c %%c 형식으로 입력: ");
			scanf("%c %c", &source2[0], &source2[1]);
			error = subedge2(g1, &source2[0], &source2[1]);
			if (error != 1)
				printf("main) subeedge의 반환값이 비정상\n");
			printmatrix(g1);
			break;
		case 4:
			DFS(g1, g1->first);
			puts("");
			RestoreGragh(g1);
			break;
		case 5:
			BFS(g1->first, pQ);
			puts("");
			RestoreGragh(g1);
			break;
		}
	}
	subgragh(g1);
	free(pQ);
	return 0;*/
}

void BFS(vertex* v1, queue* pQ) { // Recursion Model
	vertex* vTemp = NULL;
	edge* eTemp = NULL;
	if (!v1 || v1->visited == true) return;
	printf("%c ", *(char*)v1->nameptr);
	v1->visited = true;
	eTemp = v1->edgeptr;
	while (eTemp) {
		enqueue(pQ, eTemp->dst);
		eTemp = eTemp->next;
	}
	while (isemptyqueue(pQ) != true) {
		vTemp = dequeue(pQ);
		BFS(vTemp, pQ);
	}
	return;
}
void DFS(gragh* g1, vertex* v1){
	edge* eTemp = NULL;
	if (!v1 || v1->visited == true) {
		//printf("\nDFS)v1가 NULL을 만났거나 v1->visited 가 true입니다\n");
		return;
	}
	else {
		//printf("DFS)여기는 Vertex %c이고 %c->visited=%d입니다 print하고 true로 바꾸겠습니다.\n", *(char*)v1->nameptr, *(char*)v1->nameptr,v1->visited);
		printf("%c ", *(char*)v1->nameptr);
		v1->visited = true;
		if ((eTemp = v1->edgeptr) != NULL) { // General case
			while (eTemp) {
				DFS(g1, eTemp->dst);
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
		printf("RestoreGragh)g1->first가 NULL이라 수행하지 않습니다\n");
	else {
		while (vTemp) {
			vTemp->visited = false;
			vTemp = vTemp->next;
		}
	}
}