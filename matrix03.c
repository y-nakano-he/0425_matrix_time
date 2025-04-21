#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

//matrix_product：N*Nの行列の乗算を行う
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

void rand_matrix(double **C, int N){
	int i,j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			C[i][j] = rand();
		}
	}
}
/*****************************************
*malloc_matrix関数：行列のメモリを確保する	*
*配列mat1[N]分のメモリ領域を確保		    *
*→ mat1をN個並べた分のメモリ領域を確保    	*
******************************************/

double **malloc_matrix(int N){
	//mat1：double型のポインタをN個持った「行ポインタの配列」
	double **mat1 = malloc(N * sizeof(*mat1));
	if(mat1 == NULL){
		printf("Memory not allocated.\n");
		exit(8);
	}
	for(int i = 0; i < N; i++){
		mat1[i] = malloc(N * sizeof(mat1));
		if(mat1 == NULL){
			printf("Memory not allocated.\n");
			exit(8);
		}
	}
	return mat1;
}

int main(){
	//mallocで行列を確保して作成
	double **A = malloc_matrix(N);
	double **B = malloc_matrix(N);
	double **C = malloc_matrix(N);

	//確保した型と領域に要素を挿入していく
	rand_matrix(A);
	rand_matrix(B);
	rand_matrix(C);
	
	//行列演算のCPU時間を測定
	clock_t begin = clock();
	matrix_product(A, B, C);
	clock_t end = clock();

	//確保した領域を解放
	free_matrix(A, N);
	free_matrix(B, N);
	free_matrix(C, N);

	printf("CPU time = %lf sec\n", (double)(end - begin) / CLOCKS_PER_SEC);

	return 0;
}	
	
