#include<stdio.h>
#include<stdlib.h>
#include"header.h"
#include<sys/time.h>

short valid[20005];
int no_spaths[20005][20005];
int dis[20005][20005];

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
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
                            /*d = i_to_k;
                            d/=tot;
                            d *= j_to_k;
                            bcs_fw[k] += d;*/
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

double* get_BC_FW(graph *g){
    double *bcs_fw;
    long long start_time, stop_time, total_time;
    start_time = timeInMilliseconds();
    bcs_fw = BC_FloydWarshall(g, bcs_fw);
    stop_time = timeInMilliseconds();
    total_time = stop_time - start_time;
    printf("time in miliseconds: %lld\n", total_time);
    return bcs_fw;
}

double* get_BC_BFS(graph *g){
    double *bcs_bfs;
    long long start_time, stop_time, total_time;
    start_time = timeInMilliseconds();
    get_sspath(g);
    bcs_bfs = bc_bfs(g, bcs_bfs);
    stop_time = timeInMilliseconds();
    total_time = stop_time - start_time;
    printf("time in miliseconds using BFS: %lld\n", total_time);
    return bcs_bfs;
}