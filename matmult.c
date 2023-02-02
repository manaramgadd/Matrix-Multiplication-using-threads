#include <stdlib.h>
#include <pthread.h>

#define Item(X, I, J, NCOL) X[((J) + (I) * (NCOL))]
struct nums
{
    int *a;
    int *b;
    int *c;
    int r1; // l
    int c1; // m
    int c2; // n
    int indexi;
    int indexj;
};

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int *A, int *B, int *C, int l, int m, int n)
{
    for (int i = 0; i < l; i++)
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            for (int k = 0; k < m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}

// CALCULATIONS OF VERSION1 
void *elemnts(void *arg)
{
    struct nums *out = (struct nums *)arg;
    int *A = out->a;
    int *B = out->b;
    int *C = out->c;//result
    int l = out->r1;
    int m = out->c1;
    int n = out->c2;
    int i = out->indexi;
    int j = out->indexj;
    int sum = 0;

    for (int k = 0; k < m; k++)
    {
        sum += Item(A, i, k, m) * Item(B, k, j, n);
    }

    Item(C, i, j, n) = sum;
}
// creating threads for v1
void matmult_v1(int *A, int *B, int *C, int l, int m, int n)
{
    pthread_t threads_elements[l][n];
    struct nums no[l][n];

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            no[i][j].a = A;
            no[i][j].b = B;
            no[i][j].c = C;
            no[i][j].r1 = l;
            no[i][j].c1 = m;
            no[i][j].c2 = n;
            no[i][j].indexi = i;
            no[i][j].indexj = j;
            pthread_create(&threads_elements[i][j], NULL, elemnts, (void *)&no[i][j]);
        }
    }
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            pthread_join(threads_elements[i][j], NULL);
        }
    }
}

// CALCULATIONS OF V2
void *rows(void *arg)
{
    struct nums *no = (struct nums *)arg;
    int *A = no->a;
    int *B = no->b;
    int *C = no->c;
    int l = no->r1;
    int m = no->c1;
    int n = no->c2;
    int i = no->indexi;

    for (int k = 0; k < n; k++)
    {
        int val = 0;
        for (int q = 0; q < m; q++)
        {
            val += Item(A, i, q, m) * Item(B, q, k, n);
        }
        Item(C, i, k, n) = val;
    }
}

// creating threads for v2
void matmult_v2(int *A, int *B, int *C, int l, int m, int n)
{
    pthread_t thread_rows[l];
    struct nums r[l];

    for (int i = 0; i < l; i++)
    {
        r[i].a = A;
        r[i].b = B;
        r[i].c = C;
        r[i].r1 = l;
        r[i].c1 = m;
        r[i].c2 = n;
        r[i].indexi = i;

        pthread_create(&thread_rows[i], NULL, rows, (void *)&r[i]);
    }

    for (int i = 0; i < l; i++)
    {
        pthread_join(thread_rows[i], NULL);
    }
}
