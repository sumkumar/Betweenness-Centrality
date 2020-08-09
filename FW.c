#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

typedef struct egde{
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

void print_dist_matrix(int** dist, int n){
    int i,j;
    for(i=1;i<n;i++){
        for(j=1;j<n;j++){
            printf("%d\t", dist[i][j]);
        }
        printf("\n");
    }
}

double* BC_FloydWarshall(graph* g, double *bcs_fw){
    int i, j, k, n=g->n;
    long tot, i_to_k, j_to_k;
    int **dist_fw = (int **)malloc(n * sizeof(int *));
    for (i=0; i<n; i++)
        dist_fw[i] = (int *)malloc(n * sizeof(int));
    int **spaths = (int **)malloc(n * sizeof(int *));
    for (i=0; i<n; i++)
        spaths[i] = (int *)malloc(n * sizeof(int));
    for (i = 1; i < n; i++)
        for (j = 1; j <n; j++){
            dist_fw[i][j] = -1;
            spaths[i][j] = 0;
        }
    for (i = 1; i < n; i++){
        dist_fw[i][i]=0;
    }
    for(i=1;i<n;i++){
        if(valid[i] == 1){
            edge *list = g->arr[i]->head;
            while(list!=NULL){
                dist_fw[i][list->dest] = 1;
                spaths[i][list->dest] = 1;
                dist_fw[list->dest][i] = 1;
                spaths[list->dest][i] = 1;
                list = list->next;
            }
        }
    }
    for (k = 1; k < n; k++){
        if(valid[k] == 1)
            for (i = 1; i < n; i++){
                if(valid[i] == 1)
                    for (j = 1; j < n; j++){
                        if (valid[j] == 1 && dist_fw[i][k] != -1 && dist_fw[k][j] != -1 && (dist_fw[i][j] == -1 || dist_fw[i][k] + dist_fw[k][j] < dist_fw[i][j])){
                            dist_fw[i][j] = dist_fw[i][k] + dist_fw[k][j];
                            spaths[i][j] = spaths[i][k] * spaths[k][j];
                        }
                        else if (valid[j] == 1 && dist_fw[i][k] != -1 && dist_fw[k][j] != -1 && (dist_fw[i][k] + dist_fw[k][j] == dist_fw[i][j])){
                            spaths[i][j] += spaths[i][k] * spaths[k][j];
                        }
                    }
            }
    }
    bcs_fw = (double *)malloc(n * sizeof(double));
    double d, e;
    for (k = 1; k < n; k++){
        bcs_fw[k] = 0;
        if(valid[k] == 1){
            for (i = 1; i < n; i++){
                d=0;e=0;
                if(i!=k && valid[i] == 1 && dist_fw[i][k] != -1)
                    for (j = 1; j < n; j++){
                        if (j!=k && i!=j && valid[j] == 1 && dist_fw[k][j] != -1 && dist_fw[i][j] != -1 && (dist_fw[i][j] == (dist_fw[i][k] + dist_fw[k][j]))){
                            tot = spaths[i][j];
                            i_to_k = spaths[i][k];
                            j_to_k = spaths[k][j];
                            d = i_to_k;
                            d/=tot;
                            e= j_to_k;
                            bcs_fw[k] += (d*e);
                        }
                    }
            }
        }
    }
	/*for (j = 1; j <n; j++){
		free(dist_fw[i]);
		free(spaths[i][j]);
		dist_fw[i] = NULL;
		spaths[i] = NULL;
	}
	free(dist_fw);
	dist_fw = NULL;
	free(spaths);
	spaths = NULL;*/
    return bcs_fw;
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
    double *bcs_fw;
    long long start_time = timeInMilliseconds(), stop_time, total_time;
    bcs_fw = BC_FloydWarshall(g, bcs_fw);
    stop_time = timeInMilliseconds();
    total_time = stop_time - start_time;
    printf("time in miliseconds: %lld\n", total_time);
    printf("Betweenness Centrality using FW\n");
    for(i=1;i<v;i++){
        printf("%d: %lf\n", i, bcs_fw[i]);
    }
}

