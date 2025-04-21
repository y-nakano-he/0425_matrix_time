#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

void matrix_product(double A[N][N], double B[N][N], double C[N][N]){
	//インデックスを作成
	int i,j,k;
	//C[i][j]の初期化 
	for(i = 0; i < N; i++)
		for(j = 0; j < N;j++)
			C[i][j] = 0.0;
	//k→i→jでループ
	for(k = 0; k < N; k++){
		for(i = 0; i < N; i++){
			double aik = A[i][k];
			for(j = 0; j < N; j++){
				C[i][j] += aik * B[k][j];
			}
		}		
	}
}

void rand_matrix(double C[N][N]){
	int i,j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			C[i][j] = rand() % 10;
		}
	}
}


double main(){
	double A[N][N];
	double B[N][N];
	double C[N][N];

	rand_matrix(A);
	rand_matrix(B);
	rand_matrix(C);
	
	clock_t begin = clock();
	matrix_product(A, B, C);
	clock_t end = clock();

	printf("CPU time = %lf sec\n", (double)(end - begin) / CLOCKS_PER_SEC);

	return 0;
}	
	
