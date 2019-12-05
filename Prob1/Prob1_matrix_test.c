#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Prob1_Graph_adjacency_matrix.h"

int main(void) {
	int menu, idx = 0, error;
	char source[10] = { 0, };
	char source2[2] = { 0, }; // char[2]짜리가 10개
	gragh* g1 = creategragh();
	if (!g1) return -1;

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
			//error = subedge(g1, &source2[0], &source2[1]);
			error = subedge2(g1, &source2[0], &source2[1]);
			if (error != 1)
				printf("main) subeedge의 반환값이 비정상\n");
			printmatrix(g1);
			break;
		case 4:

			break;
		case 5:
			break;
		}
	}
	subgragh(g1);
	return 0;
}