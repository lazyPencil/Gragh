#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Prob3_Weight_gragh_adjacency_list.h"

int main(void) {
	int menu, idx = 0, weight;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]¥���� 10��
	gragh* g1 = creategragh();
	if (!g1) return -1;

	while (1) {
		printf("InsertVertex(0),AddEdge(1),DeleteVertex(2),DeleteEdge(3),Exit(4)\n");
		scanf("%d", &menu);
		getchar();
		if (menu == 4)
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
			printf("���� Edge�� %%c %%c %%d(weight) �������� �Է�: ");
			scanf("%c %c %d", &source2[0], &source2[1], &weight);
			g1 = addedge(g1, &source2[0], &source2[1], weight);
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
		}
	}
	deletegragh(g1);
	return 0;
}