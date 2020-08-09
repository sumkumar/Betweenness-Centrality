#include<stdio.h>
#include<stdlib.h>
#include"header.h"

extern short valid[20005];
extern int no_spaths[20005][20005];
extern int dis[20005][20005];

void add_edge(graph *g, int src, int dest){
    if(g == NULL || src>(g->n) || dest>(g->n) || src<=0 || dest<=0){
        printf("Invalid input");
        exit(0);
    }
    edge *list = g->arr[src]->head;
    if(valid[src] == 0){
        edge *new_edge = (edge*) malloc(sizeof(edge));
        if(new_edge == NULL){
            printf("Memory Full");
            exit(0);
        }
        new_edge->dest = dest;
        new_edge->next = NULL;
        g->arr[src]->head = new_edge;
        valid[src] = 1;
        return ;
    }
    edge *new_edge = (edge*) malloc(sizeof(edge));
    if(new_edge == NULL){
        printf("Memory Full");
        exit(0);
    }
    new_edge->dest = dest;
    new_edge->next = list;
    g->arr[src]->head = new_edge;
    valid[src] = 1;
}

graph* create_graph(int n){
    graph *g = (graph*) malloc(sizeof(graph));
    if(g == NULL){
        printf("Memory Full");
        exit(0);
    }
    g->n=n;
    int i;
    node **arr = (node **)malloc(n * sizeof(node *));
    for(i=0;i<n;i++){
        edge *edg = NULL;
        arr[i]= (node*) malloc(sizeof(node));
        if(arr[i] == NULL){
            printf("Memory Full");
            exit(0);
        }
        arr[i]->head = edg;
    }
    g->arr = arr;
    return g;
}

graph* get_graph(){
    int i, src, dest, dummy, v;
    long e, j;
    scanf("%d %ld", &v, &e);
    v++;
    graph *g= create_graph(v);
    for(j=0;j<e;j++){
        scanf("%d %d", &src, &dest);
        if(src >= v || dest >= v || src < 1 || dest < 1){
            printf("%d %d\n", src, dest);
            printf("Invalid Input\n");
            exit(0);
        }
        add_edge(g, src, dest);
        add_edge(g, dest, src);
        no_spaths[src][dest] = 1;
        no_spaths[dest][src] = 1;
        dis[src][dest] = 1;
        dis[dest][src] = 1;
    }
    return g;
}

void main(){
    graph *g = get_graph();
    int v = g->n, i;
    double *bcs_bfs = get_BC_BFS(g);
    printf("Betweenness Centrality using BFS\n");
    for(i=1;i<v;i++){
        printf("%d: %lf\n", i, bcs_bfs[i]);
    }
    double *bcs_fw = get_BC_FW(g);
    printf("Betweenness Centrality using FW\n");
    for(i=1;i<v;i++){
        printf("%d: %lf\n", i, bcs_fw[i]);
    }
}

