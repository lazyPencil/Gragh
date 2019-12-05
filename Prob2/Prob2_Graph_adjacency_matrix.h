// 2019.11.25~ Adjacency Matrix of Directed Graph
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct edge {
	struct edge* next;
	struct vertex* dst; // 유의할 부분 
}edge;

typedef struct vertex {
	struct vertex* next;
	void* nameptr;
	edge* edgeptr;
	bool visited;
	int indeg;
	int outdeg;
}vertex;

typedef struct gragh {
	vertex* first;
	int cnt;
	int (*compare)(void*, void*);
}gragh;

typedef struct {
	vertex** arrV;
	int size;
	int front, rear;
}queue;

int compareint(void* dataptr1, void* dataptr2) {
	return *(int*)dataptr1 - *(int*)dataptr2;
}

gragh* creategragh(void) {
	gragh* pNew = (gragh*)malloc(sizeof(gragh));
	if (pNew == NULL) return NULL;
	pNew->cnt = 0;
	pNew->compare = compareint;
	pNew->first = NULL;
	return pNew;
}

vertex* searchvertex(vertex* v1, void* find) { // 찾아서 그 vertex를 리턴 // 11.26에러: A만들고 다시 A만드는데 왜?->코드순서 잘못짬->처리함
	if (!v1);
		//printf("searchvertex) v1이 NULL이라 %c를 찾을 수 없어요\n", *(char*)find);
	else {
		while (v1) {
			if (*(char*)v1->nameptr == *(char*)find)
				break;
			v1 = v1->next;
		}
		if (v1 == NULL);
			//printf("searchvertex) gragh를 모두 확인해보니 %c가 안 들어있네요\n", *(char*)find);
	}
	return v1;
}

vertex* searchvertex2(vertex* v1, void* find, vertex** ppver) {
	if (!v1)
		printf("searchvertex) v1이 NULL이라 %c를 찾을 수 없어요\n", *(char*)find);
	else {
		while (v1) {
			if (*(char*)v1->nameptr == *(char*)find)
				break;
			*ppver = v1;
			v1 = v1->next;
		}
		if (v1 == NULL)
			printf("searchvertex) gragh를 모두 확인해보니 %c가 안 들어있네요\n", *(char*)find);
	}
	return v1;
}

edge* searchedge(vertex* srcV, vertex* findV) { // srcV->edgeptr->dst 중에 findV vertex가 있습니까?
	edge* eTemp = srcV->edgeptr;//11.26에러: A,B,C 추가 A->B,A->C,B->A,C->A 한 뒤 A를 지우니까 eTemp 가 0xdddddddd 만남
	while (eTemp) {
		if (eTemp->dst == findV) // 찾음
			break;
		eTemp = eTemp->next;
	}
	return eTemp;
}

edge* searchedge2(vertex* srcV, vertex* findV, edge** ppedge) {
	edge* eTemp = srcV->edgeptr;
	while (eTemp) {
		if (eTemp->dst == findV) // 찾음
			break;
		*ppedge = eTemp;
		eTemp = eTemp->next;
	}
	return eTemp;
}

void addvertex(gragh* g1, void* addname) {
	vertex* pNewV = NULL;
	vertex* pTemp = NULL;
	vertex* pSear = NULL;
	if ((pSear = searchvertex(g1->first, addname)) == NULL) {
		pNewV = (vertex*)malloc(sizeof(vertex));
		if (!pNewV)
			printf("addvertex)memory allocation problem\n");
		else { // vertex initialization
			pNewV->edgeptr = NULL;
			pNewV->nameptr = addname;
			pNewV->visited = false;
			pNewV->next = NULL;
			pNewV->indeg = 0;
			pNewV->outdeg = 0;
			if (g1->cnt == 0) // (g1->first == NULL)
				g1->first = pNewV;
			else { // 중간or끝에 매달기 (g1->first != NULL)
				pTemp = g1->first;
				while (pTemp->next != NULL)
					pTemp = pTemp->next;
				pTemp->next = pNewV;
			}
			g1->cnt++;
		}
	}
	else
		printf("addvertex) %c vertex가 이미 있으므로 수행되지 않습니다.\n",*(char*)pSear->nameptr);
	return;
}

void addedge(gragh* g1, void* from, void* to) {
	vertex* start = searchvertex(g1->first, from);
	vertex* end = searchvertex(g1->first, to);
	edge* pNewE = NULL;
	edge* eTemp = NULL;
	if (start == NULL || end == NULL)
		printf("addedge)%c 또는 %c Vertex가 없어서 수행할 수 없습니다\n", *(char*)from, *(char*)to);
	else {
		if (searchedge(start, end) != NULL)
			printf("addedge)Edge(%c->%c)가 이미 있어요\n", *(char*)start->nameptr, *(char*)end->nameptr);
		else {
			pNewE = (edge*)malloc(sizeof(edge));
			if (!pNewE)
				printf("addedge)memory allocation problem\n");
			else {
				pNewE->dst = end;
				pNewE->next = NULL;
				if (start->edgeptr != NULL) {
					eTemp = start->edgeptr;
					while (eTemp->next != NULL) // eTemp->next = NULL 이 되면 탈출
						eTemp = eTemp->next;
					eTemp->next = pNewE;
				}
				else
					start->edgeptr = pNewE;
				start->outdeg++;
				end->indeg++;
			}
		}
	}
}

edge* traverse_subE(edge* e1) {
	if (!e1);
		//printf("traverse_subE) e1=NULL을 만났습니다\n");
	else {
		traverse_subE(e1->next);
		e1->dst->indeg--; // indeg가 초기화가 안돼있다!->이유:A,B를 만들고 A->B이어준다음 B를 free하니 A->edgeptr->dst는 접근하면 안됨. 어짜피 끝낼건데 indeg건들 필요도 사실 없음.
		free(e1);
		e1 = NULL;
	}
	return e1;
}

void traverse_subV(gragh* g1, vertex* v1) {
	if (!v1);
		//printf("traverse_subV) v1=NULL을 만났습니다\n");
	else {
		traverse_subV(g1, v1->next);
		v1->edgeptr = traverse_subE(v1->edgeptr); // 11.26에러:수행 뒤에도 NULL이 아니라 그대로->처리함
		if (v1->edgeptr == NULL)
			v1->outdeg = 0; // subE수행하면서 v1->edgeptr == NULL이 되었을 것
		else { // 잘 작동하면 지울 것
			printf("traverse_subV) traverse_subE의 작동이 이상한 것 같습니다(현재위치:Vertex = %c)\n", *(char*)v1->nameptr);
			return;
		}
		free(v1);
		g1->cnt--;
	}
	return;
}

//만드는중->더 간단한 버젼으로 완성
int subedge2(gragh* g1, void* from, void* to) {//1.vertex는 있나? 2.edge도 있나? 3.삭제 전 이어주기 4.삭제(outdeg--)
	vertex* start = searchvertex(g1->first, from);
	vertex* end = searchvertex(g1->first, to);
	edge* ePre = NULL;
	edge* eDel = NULL;
	if (start == NULL || end == NULL)// 1
		printf("subedge)%c 또는 %c Vertex가 없어서 수행할 수 없습니다\n", *(char*)from, *(char*)to);
	else if ((eDel = searchedge2(start, end, &ePre)) == NULL) // 2
		printf("subedge)Edge(%c->%c)가 없어서 삭제할 수 없어요\n", *(char*)start->nameptr, *(char*)end->nameptr);
	else { // 3,4
		if (ePre == NULL) // 첫번째 edge를 떼는 case
			start->edgeptr = eDel->next; // start->edgeptr = eDel 였다
		else
			ePre->next = eDel->next; // ePre->next = eDel 였다
		free(eDel);
		start->outdeg--;
		end->indeg--;
		return 1;
	}
	return 0;
}
/*int subedge(gragh* g1, void* from, void* to) {//1.vertex는 있나? 2.edge도 있나? 3.삭제 전 이어주기 4.삭제(outdeg--)
	vertex* start = searchvertex(g1->first, from);
	vertex* end = searchvertex(g1->first, to);
	edge* ePre = NULL;
	edge* eDel = NULL;
	if (start == NULL || end == NULL)// 1
		printf("subedge)%c 또는 %c Vertex가 없어서 수행할 수 없습니다\n", *(char*)from, *(char*)to);
	else if (start->edgeptr == NULL) // 2
		printf("subedge)%c->edgeptr이 NULL이라 수행할 수 없습니다\n", *(char*)start->nameptr);
	else { // 3,4
		for (eDel = start->edgeptr; eDel->next != NULL; ePre = eDel, eDel = eDel->next) {
			if (eDel->dst == end) {
				if (ePre == NULL) // 첫번째 edge를 떼는 case
					start->edgeptr = eDel->next; // start->edgeptr = eDel 였다
				else
					ePre->next = eDel->next; // ePre->next = eDel 였다
				free(eDel);
				start->outdeg--;
				end->indeg--;
				return 1;
			}
		}
		if (eDel->next == NULL && eDel->dst == end) {// for문에서 확인하지 않는 예외 Case
			if (ePre == NULL) // 첫번째 edge를 떼는 case
				start->edgeptr = eDel->next; // start->edgeptr = eDel 였다
			else
				ePre->next = eDel->next; // ePre->next = eDel 였다
			free(eDel);
			start->outdeg--;
			end->indeg--;
			return 1;
		}
		printf("subedge)Edge(%c->%c)가 없어서 삭제할 수 없어요\n", *(char*)start->nameptr, *(char*)end->nameptr);
	}
	return 0;
}*/
//만드는중->완성
int subvertex(gragh* g1, void* del) {//1.vDel 있나? 2.vDel에서 나가는 edge삭제==subedge 반복 3.vDel로 들어오는 edge삭제 == searchedge(변수,vDel) 반복 4.이어주기 5.vDel 삭제
	vertex* vDel = NULL;
	vertex* vPre = NULL;
	vertex* vTemp = NULL;
	edge* eTemp = NULL;
	edge* ePre = NULL;
	edge** sTemp = NULL;
	int idx=0,error;
	vDel = searchvertex2(g1->first, del, &vPre);
	if (vDel == NULL) {//1
		printf("subvertex)Vertex %c는 없습니다\n", *(char*)del);
		return 0;
	}
	else {
		//2
		if (vDel->edgeptr == NULL)
			printf("subvertex)Vertex %c의 edgeptr이 NULL이라 지울 outdeg edge가 없네요\n", *(char*)vDel->nameptr);
		else {
			sTemp = (edge * *)malloc(sizeof(edge*) * g1->cnt);
			if (!sTemp)
				printf("subvertex)memory allocation problem\n");
			else {
				for (eTemp = vDel->edgeptr; eTemp != NULL; eTemp = eTemp->next)
					sTemp[idx++] = eTemp; // 후입선출특성 쓰려고 저장 // 3개 달려있었으면 최종 idx = 4일 것이고 3,2,1,0 순으로 실행을 구현하자
				for (idx--; idx > -1; idx--) {
					error = subedge2(g1, vDel->nameptr, sTemp[idx]->dst->nameptr);
					if (error != 1) {
						printf("subvertex)subedge2의 반환값이 이상합니다\n");
					}
				}
			}
		}
		//3,4 전체 vertex를 돌면서 vTemp의 outdeg edge 중에 vDel의 edge가 있으면 앞 뒤를 이어주면서 없애자 (vDel은 제거될 거니까)
		vTemp = g1->first;
		while (vTemp) {
			eTemp = searchedge2(vTemp, vDel, &ePre);
			if (eTemp == NULL) // Not found
				printf("subvertex)Edge(%c->%c)가 없으므로 삭제할 Edge가 없습니다\n", *(char*)vTemp->nameptr, *(char*)vDel->nameptr);
			else { // 지울 edge가 있다
				if (ePre == NULL) // 지울 edge(여기서는 eTemp)가 vTemp->edgeptr일 때
					vTemp->edgeptr = eTemp->next;
				else //지울 edge가 중간or끝에 있을 때
					ePre->next = eTemp->next;
				free(eTemp);
				vTemp->outdeg--;
				vDel->indeg--;
			}
			vTemp = vTemp->next;
		}
		//5 마지막으로 vDel을 삭제하기 전에 앞 뒤를 이어주고 없애자(vPre)
		if (vPre == NULL)
			g1->first = vDel->next;
		else
			vPre->next = vDel->next;
		free(vDel);
		g1->cnt--;
		return 1;
	}
}

void subgragh(gragh* g1) {
	traverse_subV(g1, g1->first);
	free(g1);
}

void printmatrix(gragh* g1) {
	int i,j;
	vertex** arrV = NULL;
	vertex* TempV = NULL;
	if (g1->first == NULL)
		printf("printmatrix) gragh가 비어서 수행하지 않습니다\n");
	else {
		TempV = g1->first;
		arrV = (vertex * *)malloc(sizeof(vertex*) * (g1->cnt + 1)); // arrV[0]은 사실 쓰지 않는다
		if (arrV == NULL)
			printf("printmatrix)memory allocation problem\n");
		else {
			puts("");
			for (i = 1; i < g1->cnt + 1; i++) {
				printf("%c indeg = %d\toutdeg = %d\n", *(char*)TempV->nameptr, TempV->indeg, TempV->outdeg);
				arrV[i] = TempV;
				TempV = TempV->next;
			}
			for (i = 0; i < g1->cnt + 1; i++) { // cnt+1인 이유 vertex도 출력하려고
				for (j = 0; j < g1->cnt + 1; j++) {
					if (i == 0 && j == 0) // 둘다 0일때
						printf("\n  ");
					else {
						if (i == 0) { // j!=0 
							printf("%c ", *(char*)arrV[j]->nameptr);
						}
						else {//i!=0
							if (j == 0) {//i!=0 , j==0
								printf("%c ", *(char*)arrV[i]->nameptr);
							}
							else {//i!=0 , j!=0 (General case)
								if (searchedge(arrV[i], arrV[j]))
									printf("1 ");
								else
									printf("0 ");
							}
						}
					}
				}
				puts("");
			}
		}
		free(arrV);
	}
}

queue* createQ(int size) { // 10개정도 만들자
	queue* pQ = (queue*)malloc(sizeof(queue));
	if (pQ == NULL) return NULL;
	pQ->arrV = (vertex * *)malloc(sizeof(vertex*) * size);
	if (pQ->arrV == NULL) {
		free(pQ);
		return NULL;
	}
	pQ->front = pQ->rear = 0;
	pQ->size = size;
	return pQ;
}

bool isfullqueue(queue* pQ) {
	if ((pQ->front - pQ->rear == 1) || (pQ->rear - pQ->front == pQ->size - 1)) // r,f 순일 땐 전자 / f,r 순일 땐 후자
		return true;
	else
		return false;
}
bool isemptyqueue(queue * pQ) {
	if (pQ->front == pQ->rear)
		return true;
	else
		return false;
}

void enqueue(queue* pQ, vertex* item) {
	if (isfullqueue(pQ)) {
		printf("queue is full...\n");
		return;
	}
	else
	{
		pQ->rear = (pQ->rear + 1) % pQ->size;
		pQ->arrV[pQ->rear] = item;
	}
}
vertex* dequeue(queue * pQ) {
	vertex* x;
	if (isemptyqueue(pQ)) {
		printf("queue is empty...\n");
		return NULL;
	}
	else {
		pQ->front = (pQ->front + 1) % pQ->size;
		x = pQ->arrV[pQ->front];
		return x;
	}
}