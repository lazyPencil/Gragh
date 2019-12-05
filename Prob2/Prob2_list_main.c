#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Prob2_gragh_adjacency_list.h"

void BFS(vertex* v1, queue* pQ);
void DFS(gragh* g1, vertex* v1);
void RestoreGragh(gragh* g1);

int main(void) {
	int idx = 0, error;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]¥���� 10��
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
		g1 = insertvertex(g1, &source[idx++]);
		fgetc(fp); // ���� �� �ٿ� �� �۵��ϴ��� üũ->11.27����:���� �� �� �� �޾ƶ�
		fgetc(fp);
		while (!feof(fp)) { // 11.27����: vertex�� �� �������Ƽ� ����� ���� -> addedge ���� vertex �ִ��� Ȯ���غ��� -> ��������
			error = fscanf(fp, "%c %c", &source2[0], &source2[1]);
			fgetc(fp);
			if (error != 2)
				printf("fscanf) ���� ������ ������ �ֳ׿�?\n");
			else {
				printf("fscanf) ��� : %c %c\n", source2[0], source2[1]);
				if (searchvertex(g1->first, &source2[0]) == NULL) {
					source[idx] = source2[0];
					g1 = insertvertex(g1, &source[idx++]);
				}
				if (searchvertex(g1->first, &source2[1]) == NULL) {
					source[idx] = source2[1];
					g1 = insertvertex(g1, &source[idx++]);
				}
				g1 = addedge(g1, &source2[0], &source2[1]);
			}
		}

		printf("An example of print out :\nDFS : ");
		DFS(g1, g1->first);
		RestoreGragh(g1);
		printf("\nBFS : ");
		BFS(g1->first, pQ);
		printf("\nThank You\n");
		free(pQ);
		deletegragh(g1);
	}
	fclose(fp);
	return 0;
}
	/*
	int menu, idx = 0;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]¥���� 10��
	gragh* g1 = NULL;
	queue* pQ = NULL;
	g1 = creategragh();
	pQ = createQ(10);
	if (!g1)
		printf("g1 memory allocation problem\n");
	else if(!pQ){
		printf("pQ memory allocation problem\n");
		free(g1);
	}
	else{
		while (1) {
			printf("InsertVertex(0),AddEdge(1),DeleteVertex(2),DeleteEdge(3),DFS(4),BFS(5),Exit(6)\n");
			scanf("%d", &menu);
			getchar();
			if (menu == 6)
				break;
			switch (menu) {
			case 0:
				printf("���� Vertex �Է�: ");
				scanf("%c", &source[idx]);
				getchar();
				g1 = insertvertex(g1, &source[idx]);
				idx++;
				printarc(g1->first);
				break;
			case 1:
				printf("���� Edge�� %%c %%c �������� �Է�: ");
				scanf("%c %c", &source2[0], &source2[1]);
				g1 = addedge(g1, &source2[0], &source2[1]);
				printarc(g1->first);
				break;
			case 2:
				printf("���� Vertex �Է�: ");
				scanf("%c", &source2[0]);
				g1 = deletevertex(g1, &source2[0]); //11.25����: A,B�� �ְ� A�� �������� B�� first���� �ϴµ� ������ A�� first->�̾��ֱ� ������->ó����
				printarc(g1->first);
				break;
			case 3:
				printf("���� Edge�� %%c %%c �������� �Է�: ");
				scanf("%c %c", &source2[0], &source2[1]);
				g1 = deleteedge(g1, &source2[0], &source2[1]);
				printarc(g1->first);
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
	deletegragh(g1);
	free(pQ);
	}
	return 0
	;
}
*/

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