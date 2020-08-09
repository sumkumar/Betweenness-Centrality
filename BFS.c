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
int no_spaths[20005][20005];
int dis[20005][20005];

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

double bc_bfs_node(graph* g, int v){
    int i,j,k, n=g->n;
    long tot, i_to_v, j_to_v;
    double bc=0;
    double d;
    for(i=1;i<n;i++){
        if(i!=v){
            for(j=i+1;j<n;j++){
                if(i!=j && j!=v){
					tot = no_spaths[i][j];
					if(tot != 0 ){
						i_to_v = no_spaths[i][v];
						if(dis[i][j] == dis[i][v]+dis[j][v]){
							j_to_v = no_spaths[j][v];
							d = i_to_v;
							d/=tot;
							d *= j_to_v;
							bc+=(d*2);
						}
					}
                }
            }
            
        }
    }
    
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
            //printf("%d: %lf\n", i, bc[i]);
        }
    }
    bcs_bfs = bc;
    return bcs_bfs;
}

void get_sspath(graph *g){
	int V = g->n, i,j, src;
	for(j=1;j<V;j++){
		src = j;
		short vis[V];
		for(i = 1; i < V; i++)
			vis[i] = 0;
		int q[V], front=0, rear = 0, lev[V];
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
			edge *head = g->arr[q[front]]->head;
			while(head!=NULL){
				if(lev[head->dest] == lev[q[front]] -1)
					no_spaths[src][q[front]] += no_spaths[src][head->dest];
				head = head->next;
			}
			dis[src][q[front]] = lev[q[front]];
			front++;
		}
	}
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
		no_spaths[src][dest] = 1;
		no_spaths[dest][src] = 1;
		dis[src][dest] = 1;
		dis[dest][src] = 1;
    }
    double *bcs_bfs;
    long long start_time, stop_time, total_time;
    start_time = timeInMilliseconds();
    get_sspath(g);
    bcs_bfs = bc_bfs(g, bcs_bfs);
    stop_time = timeInMilliseconds();
    total_time = stop_time - start_time;
    printf("time in miliseconds using BFS: %lld\n", total_time);
    printf("Betweenness Centrality using BFS\n");
    for(i=1;i<v;i++){
        printf("%d: %lf\n", i, bcs_bfs[i]);
    }
}
