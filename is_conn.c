#include <stdio.h>
#include <stdlib.h>

int main(){
    int n,i, x, y, f=0, dum;
    long j,e;
    scanf("%d %ld", &n, &e);
    /*int *a = (int *)malloc(sizeof(int) * (n+1));*/
    int a[n+1];
    for(i=1;i<n+1;i++)
        a[i] = -1;
    for(j=0;j<e;j++){
        scanf("%d %d", &x, &y);
        a[x] = 1;
        a[y] = 1;
    }
    for(i=1;i<n+1;i++)
        if(a[i] == -1) {
            f=1;
            printf("2 %d\n", i);
        }
    printf("%d", f);
}

