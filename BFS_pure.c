#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

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

short valid[20005];

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

void print_adj_list(graph *g){
    int i, v= g->n;
    for(i=1;i<v;i++){
            if(valid[i] == 1){
                edge *list = g->arr[i]->head;
                printf("%d: ", i);
                while(list!=NULL){
                    printf("%d ", list->dest);
                    list = list->next;
                }
                printf("\n");
            }
    }
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

int* BFS(graph *g, int src, int *ret){
    int V = g->n, i;
    short vis[V];
    for(i = 1; i < V; i++)
        vis[i] = 0;
    int q[V], front=0, rear = 0, *lev = (int*) malloc(V*sizeof(int));
    for(i=1;i<V;i++)
        lev[i] = -1;
    vis[src] = 1;
    q[rear++]=src;
    lev[src] = 0;
    node *curr_node = NULL;
    edge *list = NULL;
    while(front != rear){
        curr_node = g->arr[q[front]];
        list = curr_node->head;
        while(list != NULL){
            if (vis[list->dest] == 0){
                vis[list->dest] = 1;
                q[rear++]=list->dest;
                lev[list->dest] = lev[q[front]]+1;
            }
            list = list->next;
        }
        front++;
    }
    ret = lev;
    return ret;
}

int is_edge(graph *g, int src, int dest){
    int i,j,n=g->n;
    edge *list = g->arr[src]->head;
    while(list != NULL){
        if(list->dest == dest)
            return 1;
        list = list->next;
    }
    list = g->arr[dest]->head;
    while(list != NULL){
        if(list->dest == src)
            return 1;
        list = list->next;
    }
    return 0;
}

int bfs_count_shortest_path(graph *g, int src, int dest, int* src_bfs, int* dest_bfs, int lev){
    int i,j,k,n=g->n, len = dest_bfs[src], cnt=0, d;
    if(lev == len-1 && src_bfs[dest] == 1 && is_edge(g, src, dest) == 1){
        return 1;
    }
    for(i=1;i<n;i++){
        if(dest_bfs[i] == lev+1 && src_bfs[i] == len-lev-1 && is_edge(g, dest, i) == 1){
            d = bfs_count_shortest_path(g, src, i, src_bfs, dest_bfs, lev+1);
            cnt += d;
        }
    }
    return cnt;
}

double bc_bfs_node(graph* g, int v){
    int i,j,k, n=g->n;
    long tot, i_to_v, j_to_v;
    double bc=0;
    double d;
    if(valid[v] == 0){
        return 0;
    }
    int *v_bfs;
    v_bfs = BFS(g, v, v_bfs);
    for(i=1;i<n;i++){
        if(i!=v && valid[i]==1 && v_bfs[i] != -1){
            int *i_bfs;
            i_bfs = BFS(g, i, i_bfs);
            for(j=1;j<n;j++){
                if(i!=j && j!=v && valid[j] == 1 && i_bfs[j] != -1){
                    int *j_bfs;
                    j_bfs = BFS(g, j, j_bfs);
                    if(j_bfs[i] !=-1 && j_bfs[v] !=-1 && j_bfs[i] == i_bfs[v] + v_bfs[j] && i_bfs[j] == j_bfs[v] + v_bfs[i]){
                        tot = bfs_count_shortest_path(g, i, j, i_bfs, j_bfs, 0);
                        if(tot != 0 ){
                            i_to_v = bfs_count_shortest_path(g, i, v, i_bfs, v_bfs, 0);
                            if(i_to_v != 0){
                                j_to_v = bfs_count_shortest_path(g, v, j, v_bfs, j_bfs, 0);
                                d = i_to_v * j_to_v;
                                d/=tot;
                                bc+=d;
                            }
                        }
                    }
                    else{
                    }
                    free(j_bfs);
                    j_bfs = NULL;
                }
            }
            free(i_bfs);
            i_bfs = NULL;
        }
    }
    free(v_bfs);
    v_bfs = NULL;
    return bc;
}

void print_bfs(int *bfs, int N){
    int i,j;
    for(i=1;i<N;i++){
        if(bfs[i]!=-1){
            printf("i: %d, lev: %d\n", i, bfs[i]);
        }
    }
}

double* bc_bfs(graph* g, double *bcs_bfs){
    int i,j,n=g->n;
    double *bc = (double *)malloc(n * sizeof(double));
    for(i=1;i<n;i++){
        bc[i] = 0;
        if(valid[i] == 1){
            bc[i] = bc_bfs_node(g, i);
        }
    }
    bcs_bfs = bc;
    return bcs_bfs;
}

int main(){
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
    }
    double *bcs_bfs;
    long long start_time = timeInMilliseconds(), stop_time, total_time;
    bcs_bfs = bc_bfs(g, bcs_bfs);
    stop_time = timeInMilliseconds();
    total_time = stop_time - start_time;
    printf("time in miliseconds using BFS: %lld\n", total_time);
    printf("Betweenness Centrality using BFS\n");
    for(i=1;i<v;i++){
        printf("%d: %lf\n", i, bcs_bfs[i]);
    }
}
