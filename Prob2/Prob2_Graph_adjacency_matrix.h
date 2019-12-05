// 2019.11.25~ Adjacency Matrix of Directed Graph
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct edge {
	struct edge* next;
	struct vertex* dst; // ������ �κ� 
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

vertex* searchvertex(vertex* v1, void* find) { // ã�Ƽ� �� vertex�� ���� // 11.26����: A����� �ٽ� A����µ� ��?->�ڵ���� �߸�«->ó����
	if (!v1);
		//printf("searchvertex) v1�� NULL�̶� %c�� ã�� �� �����\n", *(char*)find);
	else {
		while (v1) {
			if (*(char*)v1->nameptr == *(char*)find)
				break;
			v1 = v1->next;
		}
		if (v1 == NULL);
			//printf("searchvertex) gragh�� ��� Ȯ���غ��� %c�� �� ����ֳ׿�\n", *(char*)find);
	}
	return v1;
}

vertex* searchvertex2(vertex* v1, void* find, vertex** ppver) {
	if (!v1)
		printf("searchvertex) v1�� NULL�̶� %c�� ã�� �� �����\n", *(char*)find);
	else {
		while (v1) {
			if (*(char*)v1->nameptr == *(char*)find)
				break;
			*ppver = v1;
			v1 = v1->next;
		}
		if (v1 == NULL)
			printf("searchvertex) gragh�� ��� Ȯ���غ��� %c�� �� ����ֳ׿�\n", *(char*)find);
	}
	return v1;
}

edge* searchedge(vertex* srcV, vertex* findV) { // srcV->edgeptr->dst �߿� findV vertex�� �ֽ��ϱ�?
	edge* eTemp = srcV->edgeptr;//11.26����: A,B,C �߰� A->B,A->C,B->A,C->A �� �� A�� ����ϱ� eTemp �� 0xdddddddd ����
	while (eTemp) {
		if (eTemp->dst == findV) // ã��
			break;
		eTemp = eTemp->next;
	}
	return eTemp;
}

edge* searchedge2(vertex* srcV, vertex* findV, edge** ppedge) {
	edge* eTemp = srcV->edgeptr;
	while (eTemp) {
		if (eTemp->dst == findV) // ã��
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
			else { // �߰�or���� �Ŵޱ� (g1->first != NULL)
				pTemp = g1->first;
				while (pTemp->next != NULL)
					pTemp = pTemp->next;
				pTemp->next = pNewV;
			}
			g1->cnt++;
		}
	}
	else
		printf("addvertex) %c vertex�� �̹� �����Ƿ� ������� �ʽ��ϴ�.\n",*(char*)pSear->nameptr);
	return;
}

void addedge(gragh* g1, void* from, void* to) {
	vertex* start = searchvertex(g1->first, from);
	vertex* end = searchvertex(g1->first, to);
	edge* pNewE = NULL;
	edge* eTemp = NULL;
	if (start == NULL || end == NULL)
		printf("addedge)%c �Ǵ� %c Vertex�� ��� ������ �� �����ϴ�\n", *(char*)from, *(char*)to);
	else {
		if (searchedge(start, end) != NULL)
			printf("addedge)Edge(%c->%c)�� �̹� �־��\n", *(char*)start->nameptr, *(char*)end->nameptr);
		else {
			pNewE = (edge*)malloc(sizeof(edge));
			if (!pNewE)
				printf("addedge)memory allocation problem\n");
			else {
				pNewE->dst = end;
				pNewE->next = NULL;
				if (start->edgeptr != NULL) {
					eTemp = start->edgeptr;
					while (eTemp->next != NULL) // eTemp->next = NULL �� �Ǹ� Ż��
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
		//printf("traverse_subE) e1=NULL�� �������ϴ�\n");
	else {
		traverse_subE(e1->next);
		e1->dst->indeg--; // indeg�� �ʱ�ȭ�� �ȵ��ִ�!->����:A,B�� ����� A->B�̾��ش��� B�� free�ϴ� A->edgeptr->dst�� �����ϸ� �ȵ�. ��¥�� �����ǵ� indeg�ǵ� �ʿ䵵 ��� ����.
		free(e1);
		e1 = NULL;
	}
	return e1;
}

void traverse_subV(gragh* g1, vertex* v1) {
	if (!v1);
		//printf("traverse_subV) v1=NULL�� �������ϴ�\n");
	else {
		traverse_subV(g1, v1->next);
		v1->edgeptr = traverse_subE(v1->edgeptr); // 11.26����:���� �ڿ��� NULL�� �ƴ϶� �״��->ó����
		if (v1->edgeptr == NULL)
			v1->outdeg = 0; // subE�����ϸ鼭 v1->edgeptr == NULL�� �Ǿ��� ��
		else { // �� �۵��ϸ� ���� ��
			printf("traverse_subV) traverse_subE�� �۵��� �̻��� �� �����ϴ�(������ġ:Vertex = %c)\n", *(char*)v1->nameptr);
			return;
		}
		free(v1);
		g1->cnt--;
	}
	return;
}

//�������->�� ������ �������� �ϼ�
int subedge2(gragh* g1, void* from, void* to) {//1.vertex�� �ֳ�? 2.edge�� �ֳ�? 3.���� �� �̾��ֱ� 4.����(outdeg--)
	vertex* start = searchvertex(g1->first, from);
	vertex* end = searchvertex(g1->first, to);
	edge* ePre = NULL;
	edge* eDel = NULL;
	if (start == NULL || end == NULL)// 1
		printf("subedge)%c �Ǵ� %c Vertex�� ��� ������ �� �����ϴ�\n", *(char*)from, *(char*)to);
	else if ((eDel = searchedge2(start, end, &ePre)) == NULL) // 2
		printf("subedge)Edge(%c->%c)�� ��� ������ �� �����\n", *(char*)start->nameptr, *(char*)end->nameptr);
	else { // 3,4
		if (ePre == NULL) // ù��° edge�� ���� case
			start->edgeptr = eDel->next; // start->edgeptr = eDel ����
		else
			ePre->next = eDel->next; // ePre->next = eDel ����
		free(eDel);
		start->outdeg--;
		end->indeg--;
		return 1;
	}
	return 0;
}
/*int subedge(gragh* g1, void* from, void* to) {//1.vertex�� �ֳ�? 2.edge�� �ֳ�? 3.���� �� �̾��ֱ� 4.����(outdeg--)
	vertex* start = searchvertex(g1->first, from);
	vertex* end = searchvertex(g1->first, to);
	edge* ePre = NULL;
	edge* eDel = NULL;
	if (start == NULL || end == NULL)// 1
		printf("subedge)%c �Ǵ� %c Vertex�� ��� ������ �� �����ϴ�\n", *(char*)from, *(char*)to);
	else if (start->edgeptr == NULL) // 2
		printf("subedge)%c->edgeptr�� NULL�̶� ������ �� �����ϴ�\n", *(char*)start->nameptr);
	else { // 3,4
		for (eDel = start->edgeptr; eDel->next != NULL; ePre = eDel, eDel = eDel->next) {
			if (eDel->dst == end) {
				if (ePre == NULL) // ù��° edge�� ���� case
					start->edgeptr = eDel->next; // start->edgeptr = eDel ����
				else
					ePre->next = eDel->next; // ePre->next = eDel ����
				free(eDel);
				start->outdeg--;
				end->indeg--;
				return 1;
			}
		}
		if (eDel->next == NULL && eDel->dst == end) {// for������ Ȯ������ �ʴ� ���� Case
			if (ePre == NULL) // ù��° edge�� ���� case
				start->edgeptr = eDel->next; // start->edgeptr = eDel ����
			else
				ePre->next = eDel->next; // ePre->next = eDel ����
			free(eDel);
			start->outdeg--;
			end->indeg--;
			return 1;
		}
		printf("subedge)Edge(%c->%c)�� ��� ������ �� �����\n", *(char*)start->nameptr, *(char*)end->nameptr);
	}
	return 0;
}*/
//�������->�ϼ�
int subvertex(gragh* g1, void* del) {//1.vDel �ֳ�? 2.vDel���� ������ edge����==subedge �ݺ� 3.vDel�� ������ edge���� == searchedge(����,vDel) �ݺ� 4.�̾��ֱ� 5.vDel ����
	vertex* vDel = NULL;
	vertex* vPre = NULL;
	vertex* vTemp = NULL;
	edge* eTemp = NULL;
	edge* ePre = NULL;
	edge** sTemp = NULL;
	int idx=0,error;
	vDel = searchvertex2(g1->first, del, &vPre);
	if (vDel == NULL) {//1
		printf("subvertex)Vertex %c�� �����ϴ�\n", *(char*)del);
		return 0;
	}
	else {
		//2
		if (vDel->edgeptr == NULL)
			printf("subvertex)Vertex %c�� edgeptr�� NULL�̶� ���� outdeg edge�� ���׿�\n", *(char*)vDel->nameptr);
		else {
			sTemp = (edge * *)malloc(sizeof(edge*) * g1->cnt);
			if (!sTemp)
				printf("subvertex)memory allocation problem\n");
			else {
				for (eTemp = vDel->edgeptr; eTemp != NULL; eTemp = eTemp->next)
					sTemp[idx++] = eTemp; // ���Լ���Ư�� ������ ���� // 3�� �޷��־����� ���� idx = 4�� ���̰� 3,2,1,0 ������ ������ ��������
				for (idx--; idx > -1; idx--) {
					error = subedge2(g1, vDel->nameptr, sTemp[idx]->dst->nameptr);
					if (error != 1) {
						printf("subvertex)subedge2�� ��ȯ���� �̻��մϴ�\n");
					}
				}
			}
		}
		//3,4 ��ü vertex�� ���鼭 vTemp�� outdeg edge �߿� vDel�� edge�� ������ �� �ڸ� �̾��ָ鼭 ������ (vDel�� ���ŵ� �Ŵϱ�)
		vTemp = g1->first;
		while (vTemp) {
			eTemp = searchedge2(vTemp, vDel, &ePre);
			if (eTemp == NULL) // Not found
				printf("subvertex)Edge(%c->%c)�� �����Ƿ� ������ Edge�� �����ϴ�\n", *(char*)vTemp->nameptr, *(char*)vDel->nameptr);
			else { // ���� edge�� �ִ�
				if (ePre == NULL) // ���� edge(���⼭�� eTemp)�� vTemp->edgeptr�� ��
					vTemp->edgeptr = eTemp->next;
				else //���� edge�� �߰�or���� ���� ��
					ePre->next = eTemp->next;
				free(eTemp);
				vTemp->outdeg--;
				vDel->indeg--;
			}
			vTemp = vTemp->next;
		}
		//5 ���������� vDel�� �����ϱ� ���� �� �ڸ� �̾��ְ� ������(vPre)
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
		printf("printmatrix) gragh�� �� �������� �ʽ��ϴ�\n");
	else {
		TempV = g1->first;
		arrV = (vertex * *)malloc(sizeof(vertex*) * (g1->cnt + 1)); // arrV[0]�� ��� ���� �ʴ´�
		if (arrV == NULL)
			printf("printmatrix)memory allocation problem\n");
		else {
			puts("");
			for (i = 1; i < g1->cnt + 1; i++) {
				printf("%c indeg = %d\toutdeg = %d\n", *(char*)TempV->nameptr, TempV->indeg, TempV->outdeg);
				arrV[i] = TempV;
				TempV = TempV->next;
			}
			for (i = 0; i < g1->cnt + 1; i++) { // cnt+1�� ���� vertex�� ����Ϸ���
				for (j = 0; j < g1->cnt + 1; j++) {
					if (i == 0 && j == 0) // �Ѵ� 0�϶�
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