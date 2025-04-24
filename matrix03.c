#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

/*****************************************
*malloc_matrix関数：行列のメモリを確保する	*
*配列mat1[N]分のメモリ領域を確保		    *
*→ mat1をN個並べた分のメモリ領域を確保    	*
******************************************/

double **malloc_matrix(){
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
//行列要素をrand() % 10で初期化
void rand_matrix(double **D){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			D[i][j] = (double)(rand() % 10);
		}
	}
}
//結果を格納する行列Cの初期化
void zero_matrix(double **E){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			E[i][j] = 0.0;
		}
	}
}
//matrix_product1：N*Nの行列の乗算を行う i->j->k
void matrix_product1(double **A, double **B, double **C){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N;j++){
			for(int k = 0; k < N; k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}		
	}
}
//matrix_product2：N*Nの行列の乗算を行う k->i->j
void matrix_product2(double **A, double **B, double **C){
	//k→i→jでループ
	for(int k = 0; k < N; k++){
		for(int i = 0; i < N; i++){
			double aik = A[i][k];
			for(int j = 0; j < N; j++){
				C[i][j] += aik * B[k][j];
			}
		}		
	}
}
//確保した領域を解放
void free_matrix(double **mat){
	for(int i = 0; i < N; i++){
		free(mat[i]);
	}
	free(mat);
}


int main(){
	//mallocで行列を確保して作成
	double **A = malloc_matrix();
	double **B = malloc_matrix();
	double **C = malloc_matrix();

	//確保した型と領域に要素を挿入していく
	rand_matrix(A);
	rand_matrix(B);

	//Cを初期化
	zero_matrix(C);

	//行列演算のCPU時間を測定
	clock_t begin1 = clock();
	matrix_product1(A, B, C);
	clock_t end1 = clock();

	//Cを初期化
	zero_matrix(C);

	//行列演算のCPU時間を測定
	clock_t begin2 = clock();
	matrix_product2(A, B, C);
	clock_t end2 = clock();


	//確保した領域を解放
	free_matrix(A);
	free_matrix(B);
	free_matrix(C);

	//計測時間の表示
	printf("ijk time = %lf sec\n", (double)(end1 - begin1) / CLOCKS_PER_SEC);
	printf("kij time = %lf sec\n", (double)(end2 - begin2) / CLOCKS_PER_SEC);

	return 0;
}	
	
