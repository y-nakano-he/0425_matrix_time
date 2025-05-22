#!/bin/bash

methods=("matrix_ijk" "matrix_ikj" "matrix_jik" "matrix_jki" "matrix_kij" "matrix_kji")
echo "method,N,time,gflops" > "1dmatrix_2000.csv"
for method in "${methods[@]}"
do
	for ((N=1000;N <= 2000; N+=1000))
	do
		#進捗を出力
		echo "Runnning $method, N =$N"
		#マクロでサイズと手法を設定してコンパイル
		gcc -O3 ./Cmatmul/1dmatrix_product.c -DMATRIX_SIZE=$N -DMETHOD=$method -o matrix_product
		#Cの標準出力結果を変数として受け取る
		read matrix_time gflops < <(./matrix_product)
		#CSVへの出力	
		echo "$method,$N,$matrix_time,$gflops" >> "1dmatrix_2000.csv"
	done
done


