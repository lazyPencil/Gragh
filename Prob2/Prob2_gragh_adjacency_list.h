// Graph using adjacency list // Directed
#pragma once
#include <stdbool.h> // bool
#include <stdlib.h> // malloc,free
#include <string.h>

typedef struct edge {
	struct edge* next;
	struct vertex* src; // ������ �κ� 
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

vertex* searchvertex(vertex* first, void* name) { // *(char*)name == *(char*)����vertex->nameptr �� vertex�� ã�´� (first vertex���� ���ʴ��)
	if (!first)
		printf("searchvertex) g1->first addr is %p\n", first);
	else {
		while (first) { // first == NULL�� �Ǹ� ( ���� �����ϸ� ) Ż��
			//printf("searchvertex) %c�� %c�� ���մϴ�\n", *(char*)first->nameptr, *(char*)name);
			if (*(char*)first->nameptr == *(char*)name)
				return first;
			first = first->next;
		}
	}
	return NULL; // �� ã��
}

// from�� ���� vertex�� outdeg �� to�� ���� vertex�� indeg�� ���� (2 arcs)
gragh* deleteedge(gragh* g1, void* from, void* to) { // 11.25����: B->outdeg�� D->indeg�� NULL�� �ƴϱ⸸ �ϸ� B->D�� ��� �ȴ�->for�� ���۵�->ó����
	vertex* start = NULL, * end = NULL;
	edge* pDelE = NULL, * pPreE = NULL;
	start = searchvertex(g1->first, from);   // *(char*)from == *(char*)start->nameptr �� ���� vertex
	end = searchvertex(g1->first, to);     
	if (start == NULL || end == NULL)      
		printf("Edge(%c->%c)�� ������� �ϴµ� vertices(%c,%c)�� �� ����ֳ׿�..?\n", *(char*)from, *(char*)to, *(char*)from, *(char*)to);
	else if (start->outdeg==NULL || end->indeg==NULL) {
		printf("%c->outdeg �Ǵ� %c->indeg�� NULL�̹Ƿ� deleteedge�� �Ұ��մϴ�\n", *(char*)start->nameptr, *(char*)end->nameptr);
	}
	else {
		for (pDelE = start->outdeg; pDelE->next != NULL; pPreE = pDelE , pDelE = pDelE->next) { // ����:start->outdeg==NULL�� pDelE->next ���ٿ���-> else if �߰�
			if (pDelE->src == end) // end vertex�� indeg arc�� ã�� ��
				break;
		}
		if (pDelE->next == NULL && pDelE->src != end) // for�� ���ܶ����� ������ �񱳸� �� �� �� �ϵ��� ����
			printf("deleteedge)%c�� outdeg(������ ����)���� %c�� �����\n", *(char*)start->nameptr, *(char*)end->nameptr);
		else {
			printf("deleteedge)%c�� outdeg�� �ִ� %c edge�� ����ڽ��ϴ�\n", *(char*)start->nameptr, *(char*)end->nameptr);
			// @@@pDelE�� ����� ���� pDelE �հ� �ڸ� �̾��־�� ��
			if (pPreE == NULL) // case 1 : ù ��° start->outdeg�� ������ ��
				start->outdeg = pDelE->next;
			else 			  // case2,3: �߰��� �ִ� or ���� �ִ� outdeg�� ������ ��. ���� pPreE->next = pDelE �̾����� next�� �̾��ش�.
				pPreE->next = pDelE->next;
			free(pDelE);
			pDelE = NULL;
		}
		pPreE = NULL; // �� for���� ���鼭 ���������� �ǵ���
		for (pDelE = end->indeg; pDelE->next != NULL; pPreE = pDelE, pDelE = pDelE->next) {
			if (pDelE->src == start)
				break;
		}
		if (pDelE->next == NULL && pDelE->src != start) // for�� ���ܶ����� ����
			printf("deleteedge)%c�� indeg(������ ����)���� %c�� �����\n", *(char*)start->nameptr, *(char*)end->nameptr);
		else {
			printf("deleteedge)%c�� indeg�� �ִ� %c edge�� ����ڽ��ϴ�\n", *(char*)end->nameptr, *(char*)start->nameptr);
			if (pPreE == NULL) // ù��°�� ����� case
				end->indeg = pDelE->next;
			else 			  // �߰��� ����� or ���� ����� case
				pPreE->next = pDelE->next;
			free(pDelE);
			pDelE = NULL;
		}
	}
	return g1;
}

gragh* deletevertex(gragh* g1, void* del) { //1.pDelV outdeg �� �ݴ� indeg ���� 2.pDelV indeg �� �ݴ� outdeg ���� 3.pDelV ���� �� �̾��ֱ� 4.pDelV ����
	vertex* pDelV = searchvertex(g1->first, del); // pDelV : ���� target vertex
	vertex* pPreV = NULL;
	edge* eTemp = NULL;
	void* STemp[10] = { 0, };
	int idx = 0;
	if (!pDelV)
		printf("deletevertex) %c��� vertex�� �����\n", *(char*)del);
	else {
		if (pDelV->outdeg == NULL && pDelV->indeg == NULL);
		else {
			eTemp = pDelV->outdeg; // 1.���� pDelV outdeg ó��
			while (eTemp) {
				STemp[idx++] = eTemp;
				eTemp = eTemp->next;
			}
			for (idx--; idx > -1; idx--) // ���Լ����� // idx=2�� 1 �ѹ� 0 �ѹ� ����
				g1 = deleteedge(g1, pDelV->nameptr, ((edge*)STemp[idx])->src->nameptr);
			eTemp = pDelV->indeg; // 2.pDelV indeg ó��
			idx = 0;
			while (eTemp) {
				STemp[idx++] = eTemp;
				eTemp = eTemp->next;
			}
			for (idx--; idx > -1; idx--) // ���Լ����� // idx=5�� 4,3,2,1,0 ����
				g1 = deleteedge(g1, ((edge*)STemp[idx])->src->nameptr, pDelV->nameptr);
			//4. V ����
		}
		//3. ���� �� �̾��ֱ�
		if (g1->first == pDelV) // case 1 : ù ��° V�� ���� : ���� : �� ���� �Ʒ� for���� ���������� ����
			g1->first = pDelV->next;
		else {
			for (pPreV = g1->first; pPreV->next != pDelV; pPreV = pPreV->next); // pPreV->next == pDelV�� ���ߴ� ����: Del �հ� �ڸ� �̾��ַ���
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
	for (idx--; idx > -1; idx--) // idx = 3��(vertex3�����ٴ¸�) 2,1,0 ������ ����
		g1 = deletevertex(g1, STemp[idx]);
	free(g1);
}

gragh* insertvertex(gragh* g1, void* d1) { // 11.25����:A�� �̹� �ִµ� A�� �߰������� ����-> ó����
	vertex* pNew = NULL;
	vertex* pTemp = NULL;

	pTemp = searchvertex(g1->first, d1);
	if (pTemp)
		printf("�̹� Vertex %c�� �����մϴ�.\n",*(char*)pTemp->nameptr);
	else {
		pNew = (vertex*)malloc(sizeof(vertex));
		if (pNew == NULL) return NULL;
		pNew->nameptr = d1; // overwriting����
		pNew->indeg = pNew->outdeg = NULL;
		pNew->next = NULL;
		pNew->visited = false;
		if (g1->cnt < 1)
			g1->first = pNew;
		else {
			pTemp = g1->first;
			while (pTemp->next != NULL) //11.25����: g1->first == NULL�� ���� cnt=0�̶� ���⸻�� if������ �����ϴµ� else������ �͹���->ó����
				pTemp = pTemp->next;
			pTemp->next = pNew;
		}
		g1->cnt++;
	}
	return g1;
}


gragh* addedge(gragh* g1, void* from, void* to) { // 11.25����: A->B�� �̹� �ִµ� A->B���ϸ� �߰��Ǵ� ���� -> ó����
	vertex* start = NULL;
	vertex* end = NULL;
	edge* etemp = NULL;
	edge* eNewstart = NULL;
	edge* eNewend = NULL;
	start = searchvertex(g1->first, from);
	end = searchvertex(g1->first, to);
	if (start == NULL || end == NULL)
		printf("addedge)Vertex(%c, %c)�� �־�� ���� �� �ִµ� �����ϴ�.\n",*(char*)from,*(char*)to);
	else {
		etemp = start->outdeg;
		while (etemp) {
			if (*(char*)etemp->src->nameptr == *(char*)end->nameptr) {
				printf("%c->outdeg �߿� �̹� %c�� �ֽ��ϴ�.\n", *(char*)start->nameptr, *(char*)end->nameptr);
				return g1;
			}
			etemp = etemp->next;
		}
		//11.25 start->outdeg�� end->indeg�� addedge���� �׻� ���ÿ� ��������ϱ� start->outdeg case�� Ȯ���غ��� �� �� ����.
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

void printarc(vertex* ver) { // 11.25����: g1->first�� NULL�� �� �ɷ����ϴµ� recursion�̶�..-> if(ver==NULL)�� �߰��ߴ�
	edge* eTemp = NULL; // 11.25����:A,B vertex�� ����� A�� ����ϱ� ver->nameptr=0xddddddd�� ���ٿ���//����:g1->first�� B�� �ٲ��� �ʾ���->������ ó��->ó����
	if (ver == NULL)
		printf("printarc) vertex addr = %p (Base case)\n", ver);
	else {
		printf("\n%c�� indeg : ", *(char*)ver->nameptr);
		if (ver->indeg == NULL)
			printf("����");
		else {
			eTemp = ver->indeg;
			while (eTemp) {
				printf("%c ", *(char*)eTemp->src->nameptr);
				eTemp = eTemp->next;
			}
		}
		printf("\n%c�� outdeg : ", *(char*)ver->nameptr);
		if (ver->outdeg == NULL)
			printf("����");
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

queue* createQ(int size) { // 10������ ������
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
	if ((pQ->front - pQ->rear == 1) || (pQ->rear - pQ->front == pQ->size - 1)) // r,f ���� �� ���� / f,r ���� �� ����
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