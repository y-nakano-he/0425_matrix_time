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
//matrix_ijk：N*Nの行列の乗算を行う i->j->k
void matrix_ijk(double **m1, double **m2, double **m3, int N){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N;j++){
			for(int k = 0; k < N; k++){
				m3[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
}

//matrix_ikj：N*Nの行列の乗算を行う i->k->j
void matrix_ikj(double **m1, double **m2, double **m3, int N){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int k = 0; k < N;k++){
			double 1ik = m1[i][k];
			for(int j = 0; j < N; j++){
				m3[i][j] += 1ik * m2[k][j];
			}
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

//matrix_jki：N*Nの行列の乗算を行う j->k->i
void matrix_jki(double **m1, double **m2, double **m3, int N){
	//j->k->iでループ
	for(int j = 0; j < N; j++){
		for(int k = 0; k < N;k++){
			double 2kj = m2[k][j];
			for(int i = 0; i < N; i++){
				m3[i][j] += m1[i][k] * 2kj;
			}
		}		
	}
}
//matrix_kij：N*Nの行列の乗算を行う k->i->j
void matrix_kij(double **m1, double **m2, double **m3, int N){
	//k→i→jでループ
	for(int k = 0; k < N; k++){
		for(int i = 0; i < N; i++){
			double 1ik = m1[i][k];
			for(int j = 0; j < N; j++){
				m3[i][j] += 1ik * m2[k][j];
			}
		}		
	}
}

//matrix_kji：N*Nの行列の乗算を行う k->j->i
void matrix_kji(double **m1, double **m2, double **m3, int N){
	//k->j->iでループ
	for(int k = 0; k < N; k++){
		for(int j = 0; j < N;j++){
			double 2kj = m2[k][j];
			for(int i = 0; i < N; i++){
				m3[i][j] += m1[i][k] * 2kj;
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
	//ループ用に関数ポインタ配列を作成
	void (*funcarray[])(double**, double**, double**, int) = {matrix_ijk, matrix_ikj, matrix_jik, matrix_jki, matrix_kij, matrix_kji};
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

		for(int i = 0; i < 6; i++){
			zero_matrix(C, N);
			struct timespec start, end;
			clock_gettime(CLOCK_MONOTONIC, &start);
			funcarray[i](A, B, C, N);
			clock_gettime(CLOCK_MONOTONIC, &end);
			double matrix_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

			fprintf(fp, "%d,%s,%.6f\n",N, funcarray[i], matrix_time);
		}
		for(int i = 0; i < N; i++){ free(A[i]); free(C[i]); }
		free(A); free(B); free(C);
	}
	
	fclose(fp);
	return 0;
}
	
