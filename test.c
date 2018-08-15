#include <stdlib.h>
#include <stdio.h>
//#include "matrix.h"
#include "matrix.c"

//FONCTIONS DE TEST

int matrix_grap(const struct matrix *matrix)
{
	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)matrix->ncols; j++)
		{
			printf("%f ", matrix->elems[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int sp_matrix_grap(const struct sp_matrix *mat)
{
	struct matrix *matrix = sp_matrix_to_matrix(mat);
	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)matrix->ncols; j++)
		{
			printf("%f ", matrix->elems[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

//MAIN

int main()
{
	int err = 0;
	printf("Creation des matrices \n");
	//struct matrix *mat1 = matrix_init(3, 4);
	//struct matrix *mat2 = matrix_init(2, 4);
	struct sp_matrix *sp_mat1 = sp_matrix_init(0.01, 4, 4);
	struct sp_matrix *sp_mat2 = sp_matrix_init(0.01, 3, 4);

	err = sp_matrix_set(sp_mat1, 1, 2, 5);
	err = sp_matrix_set(sp_mat1, 3, 2, 2);
	err = sp_matrix_set(sp_mat1, 2, 1, 3);
	err = sp_matrix_set(sp_mat1, 3, 1, -4);
	err = sp_matrix_set(sp_mat1, 0, 0, 6);
	err = sp_matrix_set(sp_mat1, 0, 3, -9);
	err = sp_matrix_set(sp_mat1, 1, 3, 7);

	err = sp_matrix_set(sp_mat2, 1, 3, 1);
	err = sp_matrix_set(sp_mat2, 0, 0, 2);
	err = sp_matrix_set(sp_mat2, 2, 2, -3);
	err = sp_matrix_set(sp_mat2, 2, 3, 4);
	err = sp_matrix_set(sp_mat2, 0, 2, -5);
	err = sp_matrix_set(sp_mat2, 0, 1, 6);
	err = sp_matrix_set(sp_mat2, 1, 0, -7);

	err = sp_matrix_save(sp_mat1, "mat1");
	err = sp_matrix_save(sp_mat2, "mat2");
	struct sp_matrix *sp_mat3 = sp_matrix_load("mat1");
	struct sp_matrix *sp_mat4 = sp_matrix_load("mat2");
	err = sp_matrix_grap(sp_mat1);
	err = sp_matrix_grap(sp_mat3);
	err = sp_matrix_grap(sp_mat2);
	err = sp_matrix_grap(sp_mat4);

	//matrix_free(mat1);
	//matrix_free(mat2);
	sp_matrix_free(sp_mat1);
	sp_matrix_free(sp_mat2);
	return err;
}