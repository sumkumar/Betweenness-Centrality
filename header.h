typedef struct edge{
    int dest;
    struct edge *next;
}edge;
typedef struct node{
    edge *head;
}node;
typedef struct graph{
    int n;
    node **arr;
}graph;

double* get_BC_BFS(graph *g);

double* get_BC_FW(graph *g);