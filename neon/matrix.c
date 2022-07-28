#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arm_neon.h>
#include <stdbool.h>
#include <math.h>

#define BLOCK_SIZE 4

void print_matrix(float32_t *M, uint32_t cols, uint32_t rows) {
	for (int i=0; i<rows; i++) {
			for (int j=0; j<cols; j++) {
					printf("%f ", M[i * cols + j]);
			}
			printf("\n");
	}
	printf("\n");
}

void matrix_init_rand(float32_t *M, uint32_t numvals) {
        for (int i=0; i<numvals; i++) {
                M[i] = (float)rand()/(float)(RAND_MAX);
        }
}

void matrix_init(float32_t *M, uint32_t cols, uint32_t rows, float32_t val) {
        for (int i=0; i<rows; i++) {
                for (int j=0; j<cols; j++) {
                        M[j*rows + i] = val;
                }
        }
}

void gemm_c(const float32_t* A, const float32_t* B, float32_t* C, const int M, const int N, const int K)
{
	for(int i = 0; i < M; ++i)
	{
		for(int k = 0; k < K; ++k)
		{
			for(int j = 0; j < N; ++j)
			{
				*(C + i * N + j) += *(A + i * K + k) * *(B + k * N + j);
			}
		}
	}
}

void gemm_c_block(const float32_t* A, const float32_t* B, float32_t* C, const int M, const int N, const int K)
{
	int row, col;
	for(int m = 0; m < M; m+=BLOCK_SIZE)
	{
		for(int n = 0; n < N; n+=BLOCK_SIZE)
		{
			for(int k = 0; k < K; ++k)
			{
				for(int i = 0; i < BLOCK_SIZE; ++i)
				{
					for(int j = 0; j < BLOCK_SIZE; ++j)
					{
						row = m + i;
						col = n + j;
						*(C + row * N + col) += *(A + row * K + k) * *(B + k * N + col);
					}
				}
			}
		}
	}
}

void gemm_neon(const float32_t* A, const float32_t* B, float32_t* C, const int M, const int N, const int K)
{
	int row, col;

	float32x4_t A_0;
	float32x4_t A_1;
	float32x4_t A_2;
	float32x4_t A_3;

	float32x4_t B_0;
	float32x4_t B_1;
	float32x4_t B_2;
	float32x4_t B_3;

	float32x4_t C_0;
	float32x4_t C_1;
	float32x4_t C_2;
	float32x4_t C_3;


	for(int m = 0; m < M; m+=4)
	{
		for(int n = 0; n < N; n+=4)
		{
			C_0 = vmovq_n_f32(0);
			C_1 = vmovq_n_f32(0);
			C_2 = vmovq_n_f32(0);
			C_3 = vmovq_n_f32(0);
			for(int k = 0; k < K; k+=4)
			{
				B_0 = vld1q_f32(B + k * N + n);
				B_1 = vld1q_f32(B + k * N + n + N);
				B_2 = vld1q_f32(B + k * N + n + N * 2);
				B_3 = vld1q_f32(B + k * N + n + N * 3);
				
				A_0 = vld1q_f32(A + m * K + k);
				C_0 = vfmaq_laneq_f32(C_0, B_0, A_0, 0);
				C_0 = vfmaq_laneq_f32(C_0, B_1, A_0, 1);
				C_0 = vfmaq_laneq_f32(C_0, B_2, A_0, 2);
				C_0 = vfmaq_laneq_f32(C_0, B_3, A_0, 3);
				
				A_1 = vld1q_f32(A + m * K + k + K);
				C_1 = vfmaq_laneq_f32(C_1, B_0, A_1, 0);
				C_1 = vfmaq_laneq_f32(C_1, B_1, A_1, 1);
				C_1 = vfmaq_laneq_f32(C_1, B_2, A_1, 2);
				C_1 = vfmaq_laneq_f32(C_1, B_3, A_1, 3);
				
				A_2 = vld1q_f32(A + m * K + k + K * 2);
				C_2 = vfmaq_laneq_f32(C_2, B_0, A_2, 0);
				C_2 = vfmaq_laneq_f32(C_2, B_1, A_2, 1);
				C_2 = vfmaq_laneq_f32(C_2, B_2, A_2, 2);
				C_2 = vfmaq_laneq_f32(C_2, B_3, A_2, 3);

				A_3 = vld1q_f32(A + m * K + k + K * 3);
				C_3 = vfmaq_laneq_f32(C_3, B_0, A_3, 0);
				C_3 = vfmaq_laneq_f32(C_3, B_1, A_3, 1);
				C_3 = vfmaq_laneq_f32(C_3, B_2, A_3, 2);
				C_3 = vfmaq_laneq_f32(C_3, B_3, A_3, 3);

			}
			vst1q_f32(C + m * N + n, C_0);
			vst1q_f32(C + m * N + n + N, C_1);
			vst1q_f32(C + m * N + n + N * 2, C_2);
			vst1q_f32(C + m * N + n + N * 3, C_3);
		}
	}
}

bool check(const float32_t* A, const float32_t* B, const int len)
{
	float err = 0;
	float tmp = 0;
	for(int i = 0; i < len; ++i)
	{
		tmp = fabs(*(A + i) - *(B + i));
		err = err > tmp ? err : tmp;
	}
	printf("Max err is %f\n", err);
	return err < 0.0001;
}

int main(int argc, char** argv)
{
	const int M = 16 * BLOCK_SIZE;
	const int N = 32 * BLOCK_SIZE;
	const int K = 64 * BLOCK_SIZE;
	
	int i,j;	
	double t;
	clock_t start, end;

	// Allocate memory for matrix A, B, C
	float32_t* A = (float32_t*) malloc(M * K * sizeof(float32_t));
	float32_t* B = (float32_t*) malloc(K * N * sizeof(float32_t));
	float32_t* C = (float32_t*) malloc(M * N * sizeof(float32_t));
	float32_t* D = (float32_t*) malloc(M * N * sizeof(float32_t));

	// Initialize matrix A, B, C
	matrix_init_rand(A, M * K);
	matrix_init_rand(B, K * N);
	matrix_init(C, M, N, 0);
	matrix_init(D, M, N, 0);
	
	start = clock();
	gemm_c(A, B, C, M, N, K);
	end = clock();
	t = (double)(end-start)/CLOCKS_PER_SEC;
	printf("C cost %f \n",t);

	start = clock();
	gemm_c_block(A, B, D, M, N, K);
	end = clock();
	t = (double)(end-start)/CLOCKS_PER_SEC;
	printf("C Block cost %f \n",t);
	printf("Equal ? %d \n", check(C, D, M * N));

	start = clock();
	gemm_neon(A, B, D, M, N, K);
	end = clock();
	t = (double)(end-start)/CLOCKS_PER_SEC;
	printf("C Neon cost %f \n",t);
	printf("Equal ? %d \n", check(C, D, M * N));

	// Release memory	
	free(A);
	free(B);
	free(C);
	free(D);
	
	return 0;
}
