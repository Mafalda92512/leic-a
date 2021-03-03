#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX 999

void initializegraph(int vertices, int M);
void addtarget(int x, int y, int M, int vertices);
void addsource(int x, int y, int M, int vertices);
int EdmondsKarp(int start, int end, int vertices);
void addReverseEdge(int current, int previous);
void deleteEdge(int previous, int current);

typedef struct V{
    int index;
    struct V *next;
}V;

typedef struct{
    V *head;
    int parents;
    int visited;
}Graph;

Graph *graph;

struct Node {
	int data;
	struct Node *next;
};

typedef struct Queue{
	struct Node *front;
	struct Node *last;
	unsigned int size;
}Queue;

void initQueue(Queue *q);
int pop(Queue *q);
void push(Queue *q, int data);
int BFS(int start, int end, int vertices);
int not_in_list(int index, int v);

int main() {
    int M, N, S, C, x, y, i, z;             /*avenidas, ruas, supermercados, cidadaos*/
    z=scanf("%d %d", &M, &N);
    z=scanf("%d %d", &S, &C);
    int nvertices= M*N;
    graph = malloc(sizeof(Graph) *((nvertices*2)+2));   /*vertices divididos em vin e vout mais source e target*/

    int total= nvertices*2 + 2;         /*numero total de vertices*/

    initializegraph(nvertices*2, M);

    for(i=0; i<S; i++){
        z=scanf("%d %d", &x, &y);
        addtarget(x,y, M, nvertices*2);             /*adicionar supermercados ao target*/
    }

    for(i=0; i<C; i++){
        z=scanf("%d %d", &x, &y);
        addsource(x, y, M, nvertices*2);      /*adicionar cidadaos a source*/
    }

    int maxflow=EdmondsKarp(nvertices*2, nvertices*2+1, total);
    printf("%d\n", maxflow);

    for (i=0; i<(nvertices*2)+2; i++){
        V *iter;
        while(graph[i].head != NULL){
            iter = graph[i].head;
            graph[i].head = graph[i].head->next;
            free(iter);
        }


    }
    free(graph);

    if (z==-1) return -1;


return 0;
}

void initializegraph(int vertices, int M){
    int i=0, j;
    for (i=0; i<(vertices +2 ) ; i++){
        graph[i].head=NULL;
        graph[i].parents = -1;
        graph[i].visited = 0;
    }

    /*ligacao a direita*/
    j=0;
    while(j<vertices){
        for (i=0; i<(M*2-1); i++){      /*o da ultima coluna ja nao se liga a direita*/
            V *adj = malloc(sizeof(V));
            adj->index= j+i+1;
            adj->next=graph[j+i].head;
            graph[j+i].head= adj;
        }
        j=j+(M*2);
    }
    /*ligacao vout a esquerda*/
    j=2*M -1 ;
    while(j<vertices){
        for (i=0; i<(M-1)*2; i=i+2){             /*o da primeira coluna ja nao se liga a esquerda*/
            V *adj = malloc(sizeof(V));
            adj->index= j-i-3;
            adj->next=graph[j-i].head;
            graph[j-i].head= adj;
        }
        j=j+(M*2);
    }
    /*ligacao vout a cima*/
    j=M*2;                            /*primeira linha nao se liga a cima*/
    while(j<vertices){
        for (i=1; i<(M*2); i=i+2){
            V *adj = malloc(sizeof(V));
            adj->index= j+i-(M*2)-1;
            adj->next=graph[j+i].head;
            graph[j+i].head= adj;
        }
        j=j+(M*2);
    }
    /*ligacao vout a baixo*/
    j=0;                               /*ultima linha nao se liga a baixo*/
    while(j<(vertices-(M*2))){
        for (i=1; i<(M*2); i=i+2){
            V *adj = malloc(sizeof(V));
            adj->index= j+i+(M*2)-1;
            adj->next=graph[j+i].head;
            graph[j+i].head= adj;
        }
        j=j+(M*2);
    }
}

void addtarget(int x, int y, int M, int vertices){
    int index = ((y-1) * M) + (x-1);        /*calcular o index para saber o numero do vertice*/
    index= (index*2)+1;                     /*traducao para numeracao do vout*/
    if (not_in_list(index, vertices+1)) {
      V *target= malloc(sizeof(V));
      target->index = vertices+1;             /*ultimo indice do grafo*/
      target->next=graph[index].head;         /*target e vizinho do supermercado*/
      graph[index].head=target;
    }
}

int not_in_list(int index, int v) {
  V *iter;
  iter = graph[index].head;
  while (iter!=NULL && iter->index != v) {
    iter = iter->next;
  }
  if (iter == NULL) return 1;
  return 0;
}

void addsource(int x, int y, int M, int vertices){
    int index = ((y-1) * M) + (x-1);        /*calcular o index para saber o numero do vertice*/
    if (not_in_list(vertices, index*2)) {
      V *adj = malloc(sizeof(V));
      adj->index=index*2;                     /*traducao para numeracao do vin*/
      adj->next=graph[vertices].head;        /*graph[vertices] e a posicao da source*/
      graph[vertices].head=adj;
    }
}


void initQueue(Queue *q){
	q->front = NULL;
	q->last = NULL;
	q->size = 0;
}

int pop(Queue *q){
    int value=-1;
    if (q->size > 0){
        q->size--;
	    struct Node *tmp = q->front;
	    q->front = q->front->next;
        value=tmp->data;
	    free(tmp);
    }
    return value;
}

void push(Queue *q, int data){
	q->size++;

	if (q->front == NULL) {
		q->front = (struct Node *) malloc(sizeof(struct Node));
		q->front->data = data;
		q->front->next = NULL;
		q->last = q->front;
	} else {
		q->last->next = (struct Node *) malloc(sizeof(struct Node));
		q->last->next->data = data;
		q->last->next->next = NULL;
		q->last = q->last->next;
	}
}

int BFS(int start, int end, int vertices){
    int i;
    for (i=0; i<vertices;i++) {
      graph[i].visited = 0;
    }

    Queue *q = malloc(sizeof(Queue));
    initQueue(q);

    push(q, start);
    graph[start].parents=-1;
    graph[start].visited=1;


    while (q->size > 0){
        int currentNode = pop(q);

        V *node= graph[currentNode].head;

        while(node!=NULL){
            int to = node->index; /*primeiro vizinho a visitar*/

            if (graph[to].visited!=1){
                graph[to].parents = currentNode;
                graph[to].visited=1;

                if (to == end) {
                    graph[to].visited=1;
                    while (q->size>0) pop(q);
                    free(q);
                    return 1;
                }
                push(q, to);
            }
            node=node->next;
        }

    }
    free(q);
 return 0;
}


int EdmondsKarp(int start, int end, int vertices){
    int maxflow=0, i;

    for (i=0;i<vertices;i++) {
      graph[i].parents=-1;
    }

    while(BFS(start,end, vertices)){

        int flow= MAX;
        int previousNode;
        int currentNode = end;

        while(currentNode != start){
            previousNode= graph[currentNode].parents;
            flow=MIN(flow, 1);

            currentNode=previousNode;
        }
        currentNode = end;
        while (currentNode != start){

            previousNode= graph[currentNode].parents;

            addReverseEdge(currentNode, previousNode);
            deleteEdge(previousNode, currentNode);
            currentNode=previousNode;
        }
        maxflow+=flow;
      }
    return maxflow;
}

void addReverseEdge(int current, int previous){
    V *new=malloc(sizeof(V));
    new->index=previous;
    new->next=graph[current].head;
    graph[current].head=new;
}

void deleteEdge(int previous, int current){
    V *iter, *prev;
    iter=graph[previous].head;
    prev=graph[previous].head;
    if (iter!=NULL && iter->index == current){
        graph[previous].head=graph[previous].head->next;
        free(iter);
        return;
    }
    while (iter!=NULL && iter->index != current){
        prev=iter;
        iter=iter->next;
    }
    if(iter==NULL) return;
    prev->next=iter->next;
    free(iter);
}
