#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*****************************************
*malloc_matrix関数：行列のメモリを確保する	*
*mat1[N]行をN列並べた分のメモリ領域を確保	 *
*→配列mat1[N]1行分のメモリ領域を確保		*
******************************************/

double **malloc_matrix(int N){
	//double* mat1はmat1[0]を指す  
	double **mat1 = malloc(N * sizeof(*mat1));
	if(mat1 == NULL){
		printf("Memory not allocated.\n");
		exit(8);
	}
	for(int i = 0; i < N; i++){
		mat1[i] = malloc(N * sizeof(*mat1[i]));
		if(mat1 == NULL){
			printf("Memory not allocated.\n");
			exit(8);
		}
	}
	return mat1;
}
//mat1要素をrand() % 10で初期化
void rand_matrix(double **mat, int N){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			mat[i][j] = (double)(rand() % 10);
		}
	}
}
//結果を格納する行列Cの初期化
void zero_matrix(double **mat, int N){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			mat[i][j] = 0.0;
		}
	}
}
//matrix_jik：N*Nの行列の乗算を行う j->i->k
void matrix_jik(double **m1, double **m2, double **m3, int N){
	//j->i->kでループ
	for(int j = 0; j < N; j++){
		for(int i = 0; i < N;i++){
			for(int k = 0; k < N; k++){
				m3[i][j] += m1[i][k] * m2[k][j];
			}
		}		
	}
}

//確保したmat1を解放
void free_matrix1(double **mat, int N){
	for(int i = 0; i < N; i++){
		free(mat[i]);
	}
	free(mat);
}

int main(){
	FILE *fp = fopen("2dmatrix_product_time.csv", "w");
	if(fp == NULL) {
		printf("fopen failed");
		exit(8);
	}
	fprintf(fp, "N,method,time_sec\n");

	for(int N = 1000; N <= 10000; N += 1000){
		printf("\nMatrix size: %d\n", N);
		//mallocで行列を確保して作成
		double **A = malloc_matrix(N);
		double **B = malloc_matrix(N);
		double **C = malloc_matrix(N);

		rand_matrix(A, N);
		rand_matrix(B, N);
		zero_matrix(C, N);

		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC, &start);
		matrix_jik(A, B, C, N);
		clock_gettime(CLOCK_MONOTONIC, &end);
		double matrix_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

		fprintf(fp, "%d,%s,%.6f\n",N, "matrix_jik", matrix_time);
		
		free_matrix1(A, N);
		free_matrix1(B, N);
		free_matrix1(C, N);	
	}
	
	fclose(fp);
	return 0;
}
	
