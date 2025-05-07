#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000

/*****************************************
*malloc_matrix関数：行列のメモリを確保する	*
*mat1[N]行をN列並べた分のメモリ領域を確保	 *
*→配列mat1[N]1行分のメモリ領域を確保		*
******************************************/

double **malloc_matrix1(){
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
double *malloc_matrix2(){
	//double* mat2はmat2[0]を指す
	double *mat2 = malloc(sizeof(*mat2[i] * N * N));
	if(mat2 == NULL){
		printf("Memory not allocated.\n");
		exit(8);
	}
	return mat2;
}
//mat1要素をrand() % 10で初期化
void rand_matrix(double **mat){
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			mat[i][j] = (double)(rand() % 10);
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
//matrix_ijk：N*Nの行列の乗算を行う i->j->k
void matrix_ijk(double **A, double **B, double **C){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N;j++){
			for(int k = 0; k < N; k++){
				double cij += A[i][k] * B[k][j];
			}
			C[i][j] = cij;
		}
	}
}
//matrix_kij：N*Nの行列の乗算を行う k->i->j
void matrix_kij(double **A, double **B, double **C){
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
//matrix_ikj：N*Nの行列の乗算を行う i->k->j
void matrix_ikj(double **A, double **B, double **C){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int k = 0; k < N;k++){
			double aik = A[i][k];
			for(int j = 0; j < N; j++){
				C[i][j] += aik * B[k][j];
			}
		}		
	}
}
//matrix_jik：N*Nの行列の乗算を行う j->i->k
void matrix_jik(double **A, double **B, double **C){
	//j->i->kでループ
	for(int j = 0; j < N; j++){
		for(int i = 0; i < N;i++){
			for(int k = 0; k < N; k++){
				double cij += A[i][k] * B[k][j];
			}
			C[i][j] = cij;
		}		
	}
}
//matrix_jki：N*Nの行列の乗算を行う j->k->i
void matrix_jki(double **A, double **B, double **C){
	//j->k->iでループ
	for(int j = 0; j < N; j++){
		for(int k = 0; k < N;k++){
			double bkj = B[k][j];
			for(int i = 0; i < N; i++){
				C[i][j] += A[i][k] * bkj;
			}
		}		
	}
}
//matrix_kji：N*Nの行列の乗算を行う k->j->i
void matrix_kji(double **A, double **B, double **C){
	//k->j->iでループ
	for(int k = 0; k < N; k++){
		for(int j = 0; j < N;j++){
			double bkj = B[k][j];
			for(int i = 0; i < N; i++){
				C[i][j] += A[i][k] * bkj;
			}
		}		
	}
}

//確保したmat1を解放
void free_matrix1(double **mat){
	for(int i = 0; i < N; i++){
		free(mat[i]);
	}
	free(mat1);
}
//確保したmat2を解放
void free_matrix2(double *mat){
	for(int i = 0; i < N; i++){
		free(mat[i]);
	}
	free(mat2);
}

int main(){
	//mallocで行列を確保して作成
	double **A = malloc_matrix1();
	double **B = malloc_matrix1();
	double **C = malloc_matrix1();

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
	free_matrix1(A);
	free_matrix1(B);
	free_matrix1(C);

	//計測時間の表示
	printf("ijk time = %lf sec\n", (double)(end1 - begin1) / CLOCKS_PER_SEC);
	printf("kij time = %lf sec\n", (double)(end2 - begin2) / CLOCKS_PER_SEC);

	return 0;
}	
	
