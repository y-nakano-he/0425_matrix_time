#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef MATRIX_SIZE
#define MATRIX_SIZE 1000
#endif

typedef struct {
	char* name;
	void (*func)(double*, double*, double*, int);
} MethodEntry;

/*****************************************
*malloc_matrix関数：行列のメモリを確保する	*
*mat1[N * N]分のメモリ領域を確保	 		*
******************************************/

double* malloc_1dmatrix(int N){
	//double* mat1はmat1[0]を指す  
	double* mat1 = malloc(sizeof(double) * N * N);
	if(mat1 == NULL){
		printf("Memory not allocated.\n");
		exit(8);
    }
	return mat1;
}
//行列をrand() / RAND_MAXで初期化
void rand_1dmatrix(double *mat1, int N){
	for(int i = 0; i < N; i++){
			mat1[i] = (double)rand() / RAND_MAX;
	}
}

//結果が格納される行列を初期化
void zero_1dmatrix(double *mat1, int N){
	for(int i = 0; i < N; i++){
	    mat1[i] = 0.0;
	}
}
//matrix_ijk：N*Nの行列の乗算を行う i->j->k
void matrix_ijk(double *m1, double *m2, double *m3, int N){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N;j++){
			for(int k = 0; k < N; k++){
				m3[i * N + j] += m1[i * N + k] * m2[k * N + j];
			}
		}
	}
}

//matrix_ikj：N*Nの行列の乗算を行う i->k->j
void matrix_ikj(double *m1, double *m2, double *m3, int N){
	//i->j->kでループ
	for(int i = 0; i < N; i++){
		for(int k = 0; k < N;k++){
			double  m1ik = m1[i * N + k];
			for(int j = 0; j < N; j++){
				m3[i * N + j] +=  m1ik * m2[k * N + j];
			}
		}		
	}
}

//matrix_jik：N*Nの行列の乗算を行う j->i->k
void matrix_jik(double *m1, double *m2, double *m3, int N){
	//j->i->kでループ
	for(int j = 0; j < N; j++){
		for(int i = 0; i < N;i++){
			for(int k = 0; k < N; k++){
				m3[i * N + j] += m1[i * N + k] * m2[k * N + j];
			}
		}		
	}
}

//matrix_jki：N*Nの行列の乗算を行う j->k->i
void matrix_jki(double *m1, double *m2, double *m3, int N){
	//j->k->iでループ
	for(int j = 0; j < N; j++){
		for(int k = 0; k < N;k++){
			double m2kj = m2[k * N + j];
			for(int i = 0; i < N; i++){
				m3[i * N + j] += m1[i * N + k] * m2kj;
			}
		}		
	}
}
//matrix_kij：N*Nの行列の乗算を行う k->i->j
void matrix_kij(double *m1, double *m2, double *m3, int N){
	//k→i→jでループ
	for(int k = 0; k < N; k++){
		for(int i = 0; i < N; i++){
			double  m1ik = m1[i * N + k];
			for(int j = 0; j < N; j++){
				m3[i * N + j] +=  m1ik * m2[k * N + j];
			}
		}		
	}
}

//matrix_kji：N*Nの行列の乗算を行う k->j->i
void matrix_kji(double *m1, double *m2, double *m3, int N){
	//k->j->iでループ
	for(int k = 0; k < N; k++){
		for(int j = 0; j < N;j++){
			double m2kj = m2[k * N + j];
			for(int i = 0; i < N; i++){
				m3[i * N + j] += m1[i * N + k] * m2kj;
			}
		}		
	}
}
/*
//確保したmat1を解放(オミット) 
void free_1dmatrix(double *mat, int N){
	for(int i = 0; i < N; i++){
		free(mat);
	}
	free(mat);
}
*/

int main(){
	
	/*->シェルスクリプト側で処理するため不要
 	ループ出力するために関数とラベルの構造体配列を作成
	MethodEntry methods[] = {
		{"matrix_ijk", matrix_ijk},
		{"matrix_ikj", matrix_ikj},
		{"matrix_jik", matrix_jik},
		{"matrix_jki", matrix_jki},
		{"matrix_kij", matrix_kij},
		{"matrix_kji", matrix_kji}
	};
	*/
	#ifdef MATRIX_SIZE
		int N = MATRIX_SIZE;
	#else
		#error "MATRIX_SIZE not defined."
	#endif
	/*
	FILE *fp = fopen("1dmatrix_product_time.csv", "a");
	if(fp == NULL) {
		printf("fopen failed");
		exit(8);
	}
	*/

	//mallocで行列を確保して作成
	double *A = malloc_1dmatrix(N);
	double *B = malloc_1dmatrix(N);
	double *C = malloc_1dmatrix(N);

	rand_1dmatrix(A, N);
	rand_1dmatrix(B, N);

	zero_1dmatrix(C, N);
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
		
	#ifdef METHOD
		METHOD(A, B, C, N);
	#else
		#error "METHOD not difined."
	#endif
	clock_gettime(CLOCK_MONOTONIC, &end);
	double matrix_time = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
	//gflops：1要素の計算量は乗算と加算を行うため[2],要素数は[N^2]
	//  	-> よって 要素数 * 計算量 = 2N * N^2 = 2N^3 
	double flop = (double)(2.0 * N * N * N);
	double gflops = (flop / 1e9) / matrix_time;
	printf("%.6f %.4f\n", matrix_time, gflops);
	//fprintf(fp, "%s, %d, %.6f, %.4f\n", #METHOD, N, matrix_time, gflops);

	free(A);
	free(B);
	free(C);	
		
	//fclose(fp);
	return 0;
}
