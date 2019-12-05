// Graph using adjacency list // Directed
#pragma once
#include <stdbool.h> // bool
#include <stdlib.h> // malloc,free
#include <string.h>

typedef struct edge {
	struct edge* next;
	struct vertex* src; // 유의할 부분 
}edge;

typedef struct vertex {
	struct vertex* next;
	void* nameptr;
	edge* indeg;
	edge* outdeg;
	bool visited;
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

vertex* searchvertex(vertex* first, void* name) { // *(char*)name == *(char*)리턴vertex->nameptr 인 vertex를 찾는다 (first vertex부터 차례대로)
	if (!first)
		printf("searchvertex) g1->first addr is %p\n", first);
	else {
		while (first) { // first == NULL이 되면 ( 끝에 도착하면 ) 탈출
			//printf("searchvertex) %c와 %c를 비교합니다\n", *(char*)first->nameptr, *(char*)name);
			if (*(char*)first->nameptr == *(char*)name)
				return first;
			first = first->next;
		}
	}
	return NULL; // 못 찾음
}

// from을 가진 vertex의 outdeg 및 to를 가진 vertex의 indeg를 삭제 (2 arcs)
gragh* deleteedge(gragh* g1, void* from, void* to) { // 11.25에러: B->outdeg와 D->indeg가 NULL이 아니기만 하면 B->D가 없어도 된다->for문 오작동->처리함
	vertex* start = NULL, * end = NULL;
	edge* pDelE = NULL, * pPreE = NULL;
	start = searchvertex(g1->first, from);   // *(char*)from == *(char*)start->nameptr 가 참인 vertex
	end = searchvertex(g1->first, to);     
	if (start == NULL || end == NULL)      
		printf("Edge(%c->%c)를 지우려고 하는데 vertices(%c,%c)가 안 들어있네요..?\n", *(char*)from, *(char*)to, *(char*)from, *(char*)to);
	else if (start->outdeg==NULL || end->indeg==NULL) {
		printf("%c->outdeg 또는 %c->indeg가 NULL이므로 deleteedge가 불가합니다\n", *(char*)start->nameptr, *(char*)end->nameptr);
	}
	else {
		for (pDelE = start->outdeg; pDelE->next != NULL; pPreE = pDelE , pDelE = pDelE->next) { // 예외:start->outdeg==NULL면 pDelE->next 접근에러-> else if 추가
			if (pDelE->src == end) // end vertex의 indeg arc를 찾는 중
				break;
		}
		if (pDelE->next == NULL && pDelE->src != end) // for문 예외때문에 마지막 비교를 한 번 더 하도록 만듦
			printf("deleteedge)%c의 outdeg(나가는 방향)에는 %c가 없어요\n", *(char*)start->nameptr, *(char*)end->nameptr);
		else {
			printf("deleteedge)%c의 outdeg에 있는 %c edge를 지우겠습니다\n", *(char*)start->nameptr, *(char*)end->nameptr);
			// @@@pDelE을 지우기 전에 pDelE 앞과 뒤를 이어주어야 함
			if (pPreE == NULL) // case 1 : 첫 번째 start->outdeg가 지워질 때
				start->outdeg = pDelE->next;
			else 			  // case2,3: 중간에 있는 or 끝에 있는 outdeg가 지워질 때. 원래 pPreE->next = pDelE 이었으나 next로 이어준다.
				pPreE->next = pDelE->next;
			free(pDelE);
			pDelE = NULL;
		}
		pPreE = NULL; // 위 for문을 돌면서 더럽혀진걸 되돌림
		for (pDelE = end->indeg; pDelE->next != NULL; pPreE = pDelE, pDelE = pDelE->next) {
			if (pDelE->src == start)
				break;
		}
		if (pDelE->next == NULL && pDelE->src != start) // for문 예외때문에 만듦
			printf("deleteedge)%c의 indeg(들어오는 방향)에는 %c가 없어요\n", *(char*)start->nameptr, *(char*)end->nameptr);
		else {
			printf("deleteedge)%c의 indeg에 있는 %c edge를 지우겠습니다\n", *(char*)end->nameptr, *(char*)start->nameptr);
			if (pPreE == NULL) // 첫번째를 지우는 case
				end->indeg = pDelE->next;
			else 			  // 중간을 지우는 or 끝을 지우는 case
				pPreE->next = pDelE->next;
			free(pDelE);
			pDelE = NULL;
		}
	}
	return g1;
}

gragh* deletevertex(gragh* g1, void* del) { //1.pDelV outdeg 및 반대 indeg 삭제 2.pDelV indeg 및 반대 outdeg 삭제 3.pDelV 삭제 전 이어주기 4.pDelV 삭제
	vertex* pDelV = searchvertex(g1->first, del); // pDelV : 지울 target vertex
	vertex* pPreV = NULL;
	edge* eTemp = NULL;
	void* STemp[10] = { 0, };
	int idx = 0;
	if (!pDelV)
		printf("deletevertex) %c라는 vertex는 없어요\n", *(char*)del);
	else {
		if (pDelV->outdeg == NULL && pDelV->indeg == NULL);
		else {
			eTemp = pDelV->outdeg; // 1.먼저 pDelV outdeg 처리
			while (eTemp) {
				STemp[idx++] = eTemp;
				eTemp = eTemp->next;
			}
			for (idx--; idx > -1; idx--) // 후입선출방식 // idx=2면 1 한번 0 한번 수행
				g1 = deleteedge(g1, pDelV->nameptr, ((edge*)STemp[idx])->src->nameptr);
			eTemp = pDelV->indeg; // 2.pDelV indeg 처리
			idx = 0;
			while (eTemp) {
				STemp[idx++] = eTemp;
				eTemp = eTemp->next;
			}
			for (idx--; idx > -1; idx--) // 후입선출방식 // idx=5면 4,3,2,1,0 수행
				g1 = deleteedge(g1, ((edge*)STemp[idx])->src->nameptr, pDelV->nameptr);
			//4. V 삭제
		}
		//3. 삭제 전 이어주기
		if (g1->first == pDelV) // case 1 : 첫 번째 V가 삭제 : 이유 : 이 경우는 아래 for문을 빠져나오지 못함
			g1->first = pDelV->next;
		else {
			for (pPreV = g1->first; pPreV->next != pDelV; pPreV = pPreV->next); // pPreV->next == pDelV면 멈추는 이유: Del 앞과 뒤를 이어주려고
			pPreV->next = pDelV->next;
		}
		free(pDelV);
		g1->cnt--;
	}
	return g1;
}

void deletegragh(gragh* g1) {
	vertex* Temp = g1->first;
	void* STemp[10] = { 0, };
	int idx = 0;
	while (Temp) {
		STemp[idx++] = Temp->nameptr;
		Temp = Temp->next;
	}
	for (idx--; idx > -1; idx--) // idx = 3면(vertex3개였다는말) 2,1,0 순으로 실행
		g1 = deletevertex(g1, STemp[idx]);
	free(g1);
}

gragh* insertvertex(gragh* g1, void* d1) { // 11.25에러:A가 이미 있는데 A가 추가가능한 현상-> 처리함
	vertex* pNew = NULL;
	vertex* pTemp = NULL;

	pTemp = searchvertex(g1->first, d1);
	if (pTemp)
		printf("이미 Vertex %c가 존재합니다.\n",*(char*)pTemp->nameptr);
	else {
		pNew = (vertex*)malloc(sizeof(vertex));
		if (pNew == NULL) return NULL;
		pNew->nameptr = d1; // overwriting에러
		pNew->indeg = pNew->outdeg = NULL;
		pNew->next = NULL;
		pNew->visited = false;
		if (g1->cnt < 1)
			g1->first = pNew;
		else {
			pTemp = g1->first;
			while (pTemp->next != NULL) //11.25에러: g1->first == NULL일 때는 cnt=0이라 여기말고 if문으로 가야하는데 else문으로 와버림->처리함
				pTemp = pTemp->next;
			pTemp->next = pNew;
		}
		g1->cnt++;
	}
	return g1;
}


gragh* addedge(gragh* g1, void* from, void* to) { // 11.25에러: A->B가 이미 있는데 A->B를하면 추가되는 현상 -> 처리함
	vertex* start = NULL;
	vertex* end = NULL;
	edge* etemp = NULL;
	edge* eNewstart = NULL;
	edge* eNewend = NULL;
	start = searchvertex(g1->first, from);
	end = searchvertex(g1->first, to);
	if (start == NULL || end == NULL)
		printf("addedge)Vertex(%c, %c)가 있어야 이을 수 있는데 없습니다.\n",*(char*)from,*(char*)to);
	else {
		etemp = start->outdeg;
		while (etemp) {
			if (*(char*)etemp->src->nameptr == *(char*)end->nameptr) {
				printf("%c->outdeg 중에 이미 %c가 있습니다.\n", *(char*)start->nameptr, *(char*)end->nameptr);
				return g1;
			}
			etemp = etemp->next;
		}
		//11.25 start->outdeg와 end->indeg는 addedge에서 항상 동시에 만들어지니까 start->outdeg case만 확인해봐도 될 것 같다.
		eNewstart = (edge*)malloc(sizeof(edge));
		eNewend = (edge*)malloc(sizeof(edge));
		if (!eNewstart || !eNewend)
			printf("edge memory allocation failed\n");
		else {
			eNewstart->next = NULL;
			eNewstart->src = end;
			if (start->outdeg == NULL)
				start->outdeg = eNewstart;
			else {
				for (etemp = start->outdeg; etemp->next != NULL; etemp = etemp->next);
				etemp->next = eNewstart;
			}
			eNewend->next = NULL;
			eNewend->src = start;
			if (end->indeg == NULL)
				end->indeg = eNewend;
			else {
				for (etemp = end->indeg; etemp->next != NULL; etemp = etemp->next);
				etemp->next = eNewend;
			}
		}
	}
	return g1;
}

void printarc(vertex* ver) { // 11.25에러: g1->first가 NULL일 때 걸러야하는데 recursion이라..-> if(ver==NULL)을 추가했다
	edge* eTemp = NULL; // 11.25에러:A,B vertex를 만들고 A를 지우니까 ver->nameptr=0xddddddd에 접근에러//이유:g1->first가 B로 바뀌지 않았음->반정도 처리->처리됨
	if (ver == NULL)
		printf("printarc) vertex addr = %p (Base case)\n", ver);
	else {
		printf("\n%c의 indeg : ", *(char*)ver->nameptr);
		if (ver->indeg == NULL)
			printf("없음");
		else {
			eTemp = ver->indeg;
			while (eTemp) {
				printf("%c ", *(char*)eTemp->src->nameptr);
				eTemp = eTemp->next;
			}
		}
		printf("\n%c의 outdeg : ", *(char*)ver->nameptr);
		if (ver->outdeg == NULL)
			printf("없음");
		else {
			eTemp = ver->outdeg;
			while (eTemp) {
				printf("%c ", *(char*)eTemp->src->nameptr);
				eTemp = eTemp->next;
			}
		}
		puts("");
		printarc(ver->next);
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

void enqueue(queue * pQ, vertex * item) {
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