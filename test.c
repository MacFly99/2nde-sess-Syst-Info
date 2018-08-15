#include <stdlib.h>
#include <stdio.h>
#include "matrix.c"

// Fonctions de test qui affiche sur stdOut le contenu de la matrice entree en argument

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
	printf("Tests OK\n");

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

	sp_matrix_free(sp_mat1);
	sp_matrix_free(sp_mat2);
	sp_matrix_free(sp_mat3);
	sp_matrix_free(sp_mat4);
	return 1;
}

/* TEST GET SP
err = sp_matrix_set(sp_mat1, 1, 1, 1);
err = sp_matrix_set(sp_mat1, 1, 3, 2);
err = sp_matrix_set(sp_mat1, 1, 2, 3);
err = sp_matrix_set(sp_mat1, 2, 2, 4);
err = sp_matrix_set(sp_mat1, 0, 1, 5);
err = sp_matrix_set(sp_mat1, 0, 3, 6);
err = sp_matrix_grap(sp_mat1);
printf("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
sp_matrix_get(sp_mat1, 0, 0), sp_matrix_get(sp_mat1, 0, 1), sp_matrix_get(sp_mat1, 0, 2), sp_matrix_get(sp_mat1, 0, 3),
sp_matrix_get(sp_mat1, 1, 0), sp_matrix_get(sp_mat1, 1, 1), sp_matrix_get(sp_mat1, 1, 2), sp_matrix_get(sp_mat1, 1, 3),
sp_matrix_get(sp_mat1, 2, 0), sp_matrix_get(sp_mat1, 2, 1), sp_matrix_get(sp_mat1, 2, 2), sp_matrix_get(sp_mat1, 2, 3),
sp_matrix_get(sp_mat1, 3, 0), sp_matrix_get(sp_mat1, 3, 1), sp_matrix_get(sp_mat1, 3, 2), sp_matrix_get(sp_mat1, 3, 3));
printf("Liberation des matrices\n");
*/

/* TEST ADD
err = matrix_set(mat1, 1, 2, 5);
err = matrix_set(mat1, 3, 2, 2);
err = matrix_set(mat1, 2, 1, 3);
err = matrix_set(mat1, 3, 1, 4);
err = matrix_set(mat1, 0, 0, 6);
err = matrix_set(mat1, 0, 3, 9);
err = matrix_set(mat1, 1, 3, 7);

err = matrix_set(mat2, 2, 3, 1);
err = matrix_set(mat2, 3, 0, 2);
err = matrix_set(mat2, 1, 2, 3);
err = matrix_set(mat2, 0, 3, 4);
err = matrix_set(mat2, 0, 2, 5);
err = matrix_set(mat2, 3, 1, 6);
err = matrix_set(mat2, 2, 0, 7);

err = matrix_grap(mat1);
printf("\n");
err = matrix_grap(mat2);
struct matrix *result = matrix_add(mat1, mat2);
err = matrix_grap(result);
*/

/* TEST ADD SP
err = sp_matrix_set(sp_mat1, 1, 2, 5);
err = sp_matrix_set(sp_mat1, 3, 2, 2);
err = sp_matrix_set(sp_mat1, 2, 1, 3);
err = sp_matrix_set(sp_mat1, 3, 1, 4);
err = sp_matrix_set(sp_mat1, 0, 0, 6);
err = sp_matrix_set(sp_mat1, 0, 3, 9);
err = sp_matrix_set(sp_mat1, 1, 3, 7);

err = sp_matrix_set(sp_mat2, 2, 3, 1);
err = sp_matrix_set(sp_mat2, 3, 0, 2);
err = sp_matrix_set(sp_mat2, 1, 2, 3);
err = sp_matrix_set(sp_mat2, 0, 3, 4);
err = sp_matrix_set(sp_mat2, 0, 2, 5);
err = sp_matrix_set(sp_mat2, 3, 1, 6);
err = sp_matrix_set(sp_mat2, 2, 0, 7);

err = sp_matrix_grap(sp_mat1);
printf("\n");
err = sp_matrix_grap(sp_mat2);
struct sp_matrix *result = sp_matrix_add(sp_mat1, sp_mat2);
err = sp_matrix_grap(result);
*/

/* TEST TRANSPOSE
err = matrix_set(mat1, 1, 2, 5);
err = matrix_set(mat1, 3, 2, 2);
err = matrix_set(mat1, 2, 1, 3);
err = matrix_set(mat1, 3, 1, -4);
err = matrix_set(mat1, 0, 0, 6);
err = matrix_set(mat1, 0, 3, -9);
err = matrix_set(mat1, 1, 3, 7);

err = matrix_set(mat2, 1, 3, 1);
err = matrix_set(mat2, 0, 0, 2);
err = matrix_set(mat2, 1, 2, -3);
err = matrix_set(mat2, 0, 3, 4);
err = matrix_set(mat2, 0, 2, -5);
err = matrix_set(mat2, 0, 1, 6);
err = matrix_set(mat2, 1, 0, -7);

err = matrix_grap(mat1);
struct matrix *transp = matrix_transpose(mat1);
err = matrix_grap(transp);
err = matrix_grap(mat2);
transp = matrix_transpose(mat2);
err = matrix_grap(transp);

err = sp_matrix_set(sp_mat1, 1, 2, 5);
err = sp_matrix_set(sp_mat1, 3, 2, 2);
err = sp_matrix_set(sp_mat1, 2, 1, 3);
err = sp_matrix_set(sp_mat1, 3, 1, -4);
err = sp_matrix_set(sp_mat1, 0, 0, 6);
err = sp_matrix_set(sp_mat1, 0, 3, -9);
err = sp_matrix_set(sp_mat1, 1, 3, 7);

err = sp_matrix_set(sp_mat2, 1, 3, 1);
err = sp_matrix_set(sp_mat2, 0, 0, 2);
err = sp_matrix_set(sp_mat2, 1, 2, -3);
err = sp_matrix_set(sp_mat2, 0, 3, 4);
err = sp_matrix_set(sp_mat2, 0, 2, -5);
err = sp_matrix_set(sp_mat2, 0, 1, 6);
err = sp_matrix_set(sp_mat2, 1, 0, -7);

err = sp_matrix_grap(sp_mat1);
struct sp_matrix *sp_transp = sp_matrix_transpose(sp_mat1);
err = sp_matrix_grap(sp_transp);
err = sp_matrix_grap(sp_mat2);
sp_transp = sp_matrix_transpose(sp_mat2);
err = sp_matrix_grap(sp_transp);
*/

/* TEST MULT
struct matrix *mat3 = matrix_init(3, 3);
struct matrix *mat4 = matrix_init(2, 4);
struct matrix *res = matrix_mult(mat3, mat4);
if (res == NULL)
{
printf("Ok \n");
}
struct sp_matrix *sp_mat3 = sp_matrix_init(0.01, 4, 3);
struct sp_matrix *sp_mat4 = sp_matrix_init(0.01, 2, 4);
struct sp_matrix *sp_res = sp_matrix_mult(sp_mat3, sp_mat4);
if (sp_res == NULL)
{
printf("Ok2 \n");
}

err = matrix_set(mat1, 1, 0, 5);
err = matrix_set(mat1, 1, 1, 2);
err = matrix_set(mat1, 2, 1, 3);
err = matrix_set(mat1, 2, 1, -4);
err = matrix_set(mat1, 0, 0, 6);
err = matrix_set(mat1, 0, 1, -9);
err = matrix_set(mat1, 1, 0, 7);

err = matrix_set(mat2, 1, 3, 1);
err = matrix_set(mat2, 0, 0, 2);
err = matrix_set(mat2, 1, 2, -3);
err = matrix_set(mat2, 0, 3, 4);
err = matrix_set(mat2, 0, 2, -5);
err = matrix_set(mat2, 0, 1, 6);
err = matrix_set(mat2, 1, 0, -7);

err = matrix_grap(mat1);
err = matrix_grap(mat2);
struct matrix *mult = matrix_mult(mat1,mat2);
err = matrix_grap(mult);

err = sp_matrix_set(sp_mat1, 1, 0, 5);
err = sp_matrix_set(sp_mat1, 2, 1, 2);
err = sp_matrix_set(sp_mat1, 2, 1, 3);
err = sp_matrix_set(sp_mat1, 1, 1, -4);
err = sp_matrix_set(sp_mat1, 0, 0, 6);
err = sp_matrix_set(sp_mat1, 0, 0, -9);
err = sp_matrix_set(sp_mat1, 1, 1, 7);

err = sp_matrix_set(sp_mat2, 1, 3, 1);
err = sp_matrix_set(sp_mat2, 0, 0, 2);
err = sp_matrix_set(sp_mat2, 1, 2, -3);
err = sp_matrix_set(sp_mat2, 0, 3, 4);
err = sp_matrix_set(sp_mat2, 0, 2, -5);
err = sp_matrix_set(sp_mat2, 0, 1, 6);
err = sp_matrix_set(sp_mat2, 1, 0, -7);

err = sp_matrix_grap(sp_mat1);
err = sp_matrix_grap(sp_mat2);
struct sp_matrix *sp_mult = sp_matrix_mult(sp_mat1,sp_mat2);
err = sp_matrix_grap(sp_mult);
*/

/* TEST SAVE + LOAD NORMAL
err = matrix_set(mat1, 1, 3, 5);
err = matrix_set(mat1, 1, 1, 2);
err = matrix_set(mat1, 2, 2, 3);
err = matrix_set(mat1, 2, 1, -4);
err = matrix_set(mat1, 0, 0, 6);
err = matrix_set(mat1, 0, 2, -9);
err = matrix_set(mat1, 1, 0, 7);

err = matrix_set(mat2, 1, 3, 1);
err = matrix_set(mat2, 0, 0, 2);
err = matrix_set(mat2, 1, 2, -3);
err = matrix_set(mat2, 0, 3, 4);
err = matrix_set(mat2, 0, 2, -5);
err = matrix_set(mat2, 0, 1, 6);
err = matrix_set(mat2, 1, 0, -7);

err = matrix_save(mat1, "mat1");
err = matrix_save(mat2, "mat2");
struct matrix *mat3 = matrix_load("mat1");
err = matrix_grap(mat1);
err = matrix_grap(mat3);
struct matrix *mat4 = matrix_load("mat2");
err = matrix_grap(mat2);
err = matrix_grap(mat4);
*/

/* TEST SAVE+LOAD SP
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
*/
