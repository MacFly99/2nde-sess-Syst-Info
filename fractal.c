#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

void main() 
{ 
	printf("Creation des matrices \n");
	struct matrix *mat1 = matrix_init(4, 3);

	struct sp_matrix *sp_mat1 = sp_matrix_init(0.001, 4, 3);
	
	printf("Assignation de valeurs\n");
	int err=0;
	err = matrix_set(mat1, 1, 2, 17);
	err = matrix_set(mat1, 2, 0, 12);
	err = matrix_set(mat1, 1, 1, 10);
	err = matrix_set(mat1, 0, 0, 28);
	err = matrix_set(mat1, 1, 2, 54);
	matrix_grap(mat1);
	
	err = 0;
	err = sp_matrix_set(sp_mat1, 1, 2, 17.12);
	printf("0");
	printf("%f", sp_mat1->lines->next->elems->next->next->value);
	err = sp_matrix_set(sp_mat1, 2, 0, 12.13);
	printf("1");
	printf("%f", sp_mat1->lines->next->next->elems->value);
	err = sp_matrix_set(sp_mat1, 1, 1, 10.11);
	printf("2");
	printf("%f", sp_mat1->lines->next->elems->next->value);
	err = sp_matrix_set(sp_mat1, 0, 0, 28.22);
	printf("3");
	printf("%f", sp_mat1->lines->elems->value);
	err = sp_matrix_set(sp_mat1, 1, 2, 54.01);
	printf("4");
	printf("%f", sp_mat1->lines->next->elems->next->next->value);
	sp_matrix_grap(sp_mat1);

	
	printf("Liberation des matrices\n");
	matrix_free(mat1);
	sp_matrix_free(sp_mat1);
	return;
}

// CREATION DES MATRICES // INIT // // //

struct matrix *matrix_init(unsigned int nlines, unsigned int ncols)
{
	//Allocation de la memoire et initialisation des variables selon les arguments.
	struct matrix *mat = (struct matrix *)malloc(sizeof(struct matrix));
	mat->nlines = nlines;
	mat->ncols = ncols;
	double ** ele = (double **)malloc(nlines * sizeof(double*));
	//Initialisation du tableau pour que toutes les valeurs valent 0.
	int i;
	for (i = 0; i < nlines; i++)
	{
		*(ele + i) = (double*)malloc(ncols * sizeof(double));
		int j;
		for (j = 0; j < ncols; j++)
		{
			ele[i][j] = 0;
		}
	}
	mat->elems = ele;
	return mat;
}

struct sp_matrix *sp_matrix_init(double precision, unsigned int nlines, unsigned int ncols)
{
	//Allocation de la memoire et initialisation des variables selon les arguments.
	struct sp_matrix *mat = (struct sp_matrix *)malloc(sizeof(struct sp_matrix));
	mat->precision = precision;
	mat->nlines = nlines;
	mat->ncols = ncols;
	mat->lines = NULL;

	return mat;
}

// DESTRUCTION DES MATRICES // FREE // // //

void matrix_free(struct matrix *matrix)
{
	double **ele = matrix->elems;
	int i;
	for (i = 0; i < matrix->nlines; i++)
	{
		free( *(ele + i) );
	}
	free(matrix);
	return;
}

void sp_matrix_free(struct sp_matrix *matrix)
{
	if (matrix == NULL)
	{
		return;
	}
	if (matrix->lines == NULL)
	{
		free(matrix);
		return;
	}
	struct line *pointeurLi = matrix->lines;
	if (pointeurLi->next == NULL)
	{
		struct elem *pointeurCo = pointeurLi->elems;
		if (pointeurCo->next == NULL)
		{
			free(pointeurCo);
		}
		else
		{
			struct elem *nextPointeurCo = pointeurCo->next;
			while (nextPointeurCo != NULL)
			{
				free(pointeurCo);
				pointeurCo = nextPointeurCo;
				nextPointeurCo = nextPointeurCo->next;
			}
			free(nextPointeurCo);
		}
		free(pointeurLi);
	}
	else
	{
		struct line *nextPointeurLi = pointeurLi->next;
		while (nextPointeurLi != NULL)
		{
			struct elem *pointeurCo = pointeurLi->elems;
			if (pointeurCo->next == NULL)
			{
				free(pointeurCo);
			}
			else
			{
				struct elem *nextPointeurCo = pointeurCo->next;
				while (nextPointeurCo != NULL)
				{
					free(pointeurCo);
					pointeurCo = nextPointeurCo;
					nextPointeurCo = nextPointeurCo->next;
				}
				free(nextPointeurCo);
			}
			free(pointeurLi);
			pointeurLi = nextPointeurLi;
			nextPointeurLi = nextPointeurLi->next;
		}
		free(nextPointeurLi);
	}
	free(matrix);
	return;
}

// DEFINITION DE LA VALEUR D'UN ELEMENT D'UNE MATRICE // SET // // //

int matrix_set(struct matrix *matrix, unsigned int i, unsigned int j, double val)
{
	if (matrix == NULL)
	{
		return -1;
	}
	if (i >= matrix->nlines || j >= matrix->ncols) //Cela est deja compris dans les pre-conditions de la fonction, mais cela rend quand meme le code plus solide
	{
		return -1;
	}
	matrix->elems[i][j] = val;
	return 0;
}

int sp_matrix_set(struct sp_matrix *matrix, unsigned int i, unsigned int j, double val)
{
	if (matrix == NULL)
	{
		return -1;
	}
	if (i >= matrix->nlines || j >= matrix->ncols) //Cela est deja compris dans les pre-conditions de la fonction, mais cela rend quand meme le code plus solide
	{
		return -1;
	}
	if (val < matrix->precision && val > -(matrix->precision))
	{
		val = 0;
	}
	if (matrix->lines == NULL)
	{
		if (val == 0)
		{
			return 0;
		}
		else
		{
			struct elem *element = (struct elem *)malloc(sizeof(struct elem));
			element->j = j;
			element->next = NULL;
			element->value = val;

			struct line *ligne = (struct line *)malloc(sizeof(struct line));
			ligne->elems = element;
			ligne->i = i;
			ligne->next = NULL;

			return 0;
		}
	}
	else
	{
		if (matrix->lines->i > i)
		{
			if (val == 0)
			{
				return 0;
			}
			else
			{
				struct elem *element = (struct elem *)malloc(sizeof(struct elem));
				element->j = j;
				element->next = NULL;
				element->value = val;

				struct line *ligne = (struct line *)malloc(sizeof(struct line));
				ligne->elems = element;
				ligne->i = i;
				ligne->next = matrix->lines->next;

				matrix->lines = ligne;

				return 0;
			}
		}
		else if (matrix->lines->i == i)
		{
			if (matrix->lines->elems->j > j)
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = matrix->lines->elems->next;
					element->value = val;

					matrix->lines->elems = element;

					return 0;
				}
			}
			else if (matrix->lines->elems->j == j)
			{
				if (val == 0)
				{
					if (matrix->lines->elems->next == NULL)
					{
						free(matrix->lines->elems);

						if (matrix->lines->next == NULL)
						{
							free(matrix->lines);
							matrix->lines = NULL;
							return 0;
						}
						else
						{
							struct line *att = matrix->lines;
							matrix->lines = matrix->lines->next;
							free(att);

							return 0;
						}
						return -2;
					}
					else
					{
						struct elem *att = matrix->lines->elems;
						matrix->lines->elems = matrix->lines->elems->next;
						free(att);

						return 0;
					}
				}
				else
				{
					matrix->lines->elems->value = val;
					return 0;
				}
			}

			int flag = 1;
			struct elem *pointeurco = matrix->lines->elems;
			while (pointeurco->next->j < j && flag)
			{
				if (pointeurco->next->next == NULL)
				{
					flag = 0;
				}
				else
				{
					pointeurco = pointeurco->next;
				}
			}

			if (pointeurco->next->j == j)
			{
				if (val == 0)
				{
					struct elem *att = pointeurco->next;
					pointeurco->next = pointeurco->next->next;
					free(att);

					return 0;
				}
				else
				{
					pointeurco->next->value = val;
					return 0;
				}
			}
			else if (!flag)
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = NULL;
					element->value = val;

					pointeurco->next->next = element;

					return 0;
				}
			}
			else
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = pointeurco->next;
					element->value = val;

					pointeurco->next = element;

					return 0;
				}
			}

		}

		//On parcourt les lignes pour trouver celle ou s'arreter, meme systeme que pour trouver le bon element dans la 1ere ligne
		int flag = 1;
		struct line *pointeurli = matrix->lines;
		while (pointeurli->next->i < i && flag)
		{
			if (pointeurli->next->next == NULL)
			{
				flag = 0;
			}
			else
			{
				pointeurli = pointeurli->next;
			}
		}

		if (pointeurli->next->i == i) //La ligne existe deja, il ne faut pas en creer, juste rajouter (ou pas) un nouvel elem
		{
			struct line *prepointeurli = pointeurli;
			pointeurli = pointeurli->next;
			if (pointeurli->elems->j > j)
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{

					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = pointeurli->elems->next;
					element->value = val;

					pointeurli->elems = element;

					return 0;
				}
			}
			else if (pointeurli->elems->j == j)
			{
				if (val == 0)
				{
					if (pointeurli->elems->next == NULL)
					{
						free(pointeurli->elems);

						if (pointeurli->next == NULL)
						{
							free(pointeurli);
							prepointeurli = NULL;
							return 0;
						}
						else
						{
							prepointeurli->next = pointeurli->next;
							free(pointeurli);

							return 0;
						}
						return -3;
					}
					else
					{
						struct elem *att = pointeurli->elems;
						pointeurli->elems = pointeurli->elems->next;
						free(att);

						return 0;
					}
				}
				else
				{
					pointeurli->elems->value = val;
					return 0;
				}
			}

			int flag = 1;
			struct elem *pointeurco = pointeurli->elems;
			while (pointeurco->next->j < j && flag)
			{
				if (pointeurco->next->next == NULL)
				{
					flag = 0;
				}
				else
				{
					pointeurco = pointeurco->next;
				}
			}

			if (pointeurco->next->j == j)
			{
				if (val == 0)
				{
					struct elem *att = pointeurco->next;
					pointeurco->next = pointeurco->next->next;
					free(att);

					return 0;
				}
				else
				{
					pointeurco->next->value = val;
					return 0;
				}
			}
			else if (!flag)
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = NULL;
					element->value = val;

					pointeurco->next->next = element;

					return 0;
				}
			}
			else
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = pointeurco->next;
					element->value = val;

					pointeurco->next = element;

					return 0;
				}
			}
		}
		else if (!flag) //La ligne du nouvel elem n'existe pas encore, et ce sera la nouvelle derniere ligne, on cree une nouvelle ligne et un nouvel elem
		{
			if (val == 0)
			{
				return 0;
			}
			else
			{
				struct elem *element = (struct elem *)malloc(sizeof(struct elem));
				element->j = j;
				element->next = NULL;
				element->value = val;

				struct line *ligne = (struct line *)malloc(sizeof(struct line));
				ligne->elems = element;
				ligne->i = i;
				ligne->next = NULL;

				pointeurli->next->next = ligne;

				return 0;
			}
		}
		else //La ligne du nouvel elem n'existe pas encore, on cree une nouvelle ligne et un nouvel elem
		{
			if (val == 0)
			{
				return 0;
			}
			else
			{
				struct elem *element = (struct elem *)malloc(sizeof(struct elem));
				element->j = j;
				element->next = NULL;
				element->value = val;

				struct line *ligne = (struct line *)malloc(sizeof(struct line));
				ligne->elems = element;
				ligne->i = i;
				ligne->next = pointeurli->next;

				pointeurli->next = ligne;

				return 0;
			}
		}

	}
	return -4;
}

// RECUPERATION DE LA VALEUR D'UN ELEMENT D'UNE MATRICE // GET // // //

double matrix_get(const struct matrix *matrix, unsigned int i, unsigned int j)
{
	if (matrix == NULL)
	{
		return -10 * 10 ^ 4 + 387;
	}
	else
	{
		return matrix->elems[i][j];
	}
}

double sp_matrix_get(const struct sp_matrix *matrix, unsigned int i, unsigned int j)
{
	if (matrix == NULL)
	{
		return -10 * 10 ^ 4 + 387;
	}
	else if (matrix->lines == NULL)
	{
		return 0;
	}
	else
	{
		struct line *pointeurli = matrix->lines;
		while (pointeurli->i <= i)
		{
			if (pointeurli->i == i)
			{
				if (pointeurli->elems == NULL)
				{
					return 0;
				}
				else
				{
					struct elem *pointeurco = pointeurli->elems;
					while (pointeurco->j <= j)
					{
						if (pointeurco->j == j)
						{
							return pointeurco->value;
						}
						else
						{
							pointeurco = pointeurco->next;
						}
					}
					return 0;
				}
			}
			else
			{
				pointeurli = pointeurli->next;
			}
		}
		return 0;
	}
	return -10 * 10 ^ 4 + 387;
}

// ADDITION DE DEUX MATRICES // ADD // // //

struct matrix *matrix_add(const struct matrix *m1, const struct matrix *m2)
{
	if (m1->ncols != m2->ncols || m1->nlines != m2->nlines)
	{
		return NULL;
	}
	struct matrix *mat = matrix_init(m1->nlines, m2->ncols);
	int i;
	for (i = 0; i < m1->nlines; i++)
	{
		int j;
		for (j = 0; j < m1->nlines; j++)
		{
			double val1 = matrix_get(m1, i, j);
			double val2 = matrix_get(m2, i, j);
			double res = val1 + val2;
			int err = matrix_set(mat, i, j, res);
			if (err)
			{
				return NULL;
			}
		}
	}
	return mat;
}

struct sp_matrix *sp_matrix_add(const struct sp_matrix *m1, const struct sp_matrix *m2)
{
	if (m1->ncols != m2->ncols || m1->nlines != m2->nlines)
	{
		return NULL;
	}

	double prec;
	if (m1->precision < m2->precision)
	{
		prec = m1->precision;
	}
	else
	{
		prec = m2->precision;
	}

	struct sp_matrix *mat = sp_matrix_init(prec, m1->nlines, m2->ncols);
	int i;
	for (i = 0; i < m1->nlines; i++)
	{
		int j;
		for (j = 0; j < m1->nlines; j++)
		{
			double val1 = sp_matrix_get(m1, i, j);
			double val2 = sp_matrix_get(m2, i, j);
			double res = val1 + val2;
			int err = sp_matrix_set(mat, i, j, res);
			if (err)
			{
				return NULL;
			}
		}
	}
	return mat;

}

// TRANSPOSEE D'UNE MATRICE // TRANSPOSE // // //

struct matrix *matrix_transpose(const struct matrix *matrix)
{
	struct matrix *mat = matrix_init(matrix->ncols, matrix->nlines);
	int i;
	for (i = 0; i < matrix->nlines ; i++)
	{
		int j;
		for (j = 0; j < matrix->ncols;j++)
		{
			double val = matrix_get(matrix, i, j);

			int err = matrix_set(mat, j, i, val);
			if (err)
			{
				return NULL;
			}
		}
	}
	return mat;
}

struct sp_matrix *sp_matrix_transpose(const struct sp_matrix *matrix)
{
	struct sp_matrix *mat = sp_matrix_init(matrix->precision,matrix->ncols, matrix->nlines);
	int i;
	for (i = 0; i < matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < matrix->ncols; j++)
		{
			double val = sp_matrix_get(matrix, i, j);

			int err = sp_matrix_set(mat, j, i, val);
			if (err)
			{
				return NULL;
			}
		}
	}
	return mat;
}

// MULTIPLICATION DE DEUX MATRICES // MULT // // //

struct matrix *matrix_mult(const struct matrix *m1, const struct matrix *m2)
{
	if (m1->ncols != m2->nlines)
	{
		return NULL;
	}
	struct matrix *mat = matrix_init(m1->nlines, m2->ncols);
		
	int i;
	for (i = 0; i < m1->nlines; i++)
	{
		int j;
		for (j = 0; j < m2->ncols; j++)
		{
			int k;
			double res = 0;
			for (k = 0; k < m1->ncols; k++)
			{
				double prod1 = matrix_get(m1, i, k);
				double prod2 = matrix_get(m2, k, j);
				res += (prod1*prod2);

				int err = matrix_set(mat, i, j, res);
				if (err)
				{
					return NULL;
				}
			}

		}
	}
	return mat;
}

struct sp_matrix *sp_matrix_mult(const struct sp_matrix *m1, const struct sp_matrix *m2)
{
	if (m1->ncols != m2->nlines)
	{
		return NULL;
	}

	double prec;
	if (m1->precision < m2->precision)
	{
		prec = m1->precision;
	}
	else
	{
		prec = m2->precision;
	}

	struct sp_matrix *mat = sp_matrix_init(prec,m1->nlines, m2->ncols);

	int i;
	for (i = 0; i < m1->nlines; i++)
	{
		int j;
		for (j = 0; j < m2->ncols; j++)
		{
			int k;
			double res = 0;
			for (k = 0; k < m1->ncols; k++)
			{
				double prod1 = sp_matrix_get(m1, i, k);
				double prod2 = sp_matrix_get(m2, k, j);
				res += (prod1*prod2);

				int err = sp_matrix_set(mat, i, j, res);
				if (err)
				{
					return NULL;
				}
			}

		}
	}
	return mat;
}

// CONVERSION D'UNE MATRICE D'UN TYPE VERS UN AUTRE // TO // // //

struct sp_matrix *matrix_to_sp_matrix(const struct matrix *matrix, double precision)
{
	struct sp_matrix *mat = sp_matrix_init(precision, matrix->nlines, matrix->ncols);

	int i;
	for (i = 0; i < matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < matrix->ncols; j++)
		{
			if (matrix->elems[i][j] != 0)
			{
				int err = sp_matrix_set(mat, i, j, matrix->elems[i][j]);
				if (err)
				{
					return NULL;
				}
			}
		}
	}
	return mat;
}

struct matrix *sp_matrix_to_matrix(const struct sp_matrix *matrix)
{
	struct matrix *mat = matrix_init(matrix->nlines, matrix->ncols);
	
	if (matrix->lines == NULL)
	{
		return mat;
	}
	
	struct line *pointeurLi = matrix->lines;
	while (pointeurLi != NULL)
	{
		struct elem *pointeurCo = pointeurLi->elems;
		while (pointeurCo != NULL)
		{
			int err = matrix_set(mat, pointeurLi->i, pointeurCo->j, pointeurCo->value);
			if (err)
			{
				return NULL;
			}
			pointeurCo = pointeurCo->next;
		}
		pointeurLi = pointeurLi->next;
	}
	return mat;
}

// SAUVERGARDE D'UNE MATRICE DANS UN FICHIER // SAVE // // //

int matrix_save(const struct matrix *matrix, char *path)
{
	FILE *fi = fopen(path, "w");
	if (fi == NULL)
	{
		return -1;
	}

	fprintf(fi, "%d %d", matrix->nlines, matrix->ncols);

	int i;
	for (i = 0; i < matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < matrix->ncols; j++)
		{
			fprintf(fi, "%f", matrix->elems[i][j]);
		}
	}
	
	fclose(fi);
	return 0;
}

int sp_matrix_save(const struct sp_matrix *matrix, char *path)
{
	FILE *fi = fopen(path, "w");
	if (fi == NULL)
	{
		return -1;
	}

	fprintf(fi, "%d %d %f", matrix->nlines, matrix->ncols, matrix->precision);
	
	if (matrix->lines == NULL)
	{
		fclose(fi);
		return 0;
	}

	struct line *pointeurLi = matrix->lines;
	while (pointeurLi != NULL)
	{
		struct elem *pointeurCo = pointeurLi->elems;
		while (pointeurCo != NULL)
		{
			fprintf(fi, "%d %d %f", pointeurLi->i, pointeurCo->j, pointeurCo->value);
			pointeurCo = pointeurCo->next;
		}
		pointeurLi = pointeurLi->next;
	}
	return 0;
}

// CHARGEMENT D'UNE MATRICE A PARTIR D'UN FICHIER // LOAD // // //

struct matrix *matrix_load(char *path)
{
	FILE *fi = fopen(path, "r");
	if (fi == NULL)
	{
		return NULL;
	}
	int res, nlines, ncols;
	res = fscanf(fi, "%d %d", &nlines, &ncols);
	if (res < 2)
	{
		return NULL;
	}
	struct matrix *mat = matrix_init(nlines,ncols);

	int i;
	for (i = 0; i < nlines; i++)
	{
		int j;
		for (j = 0; j < ncols; j++)
		{
			int res2;
			double val;
			res2 = fscanf(fi, "%f", &val);
			if (res2 != 1)
			{
				return NULL;
			}
			int err= matrix_set(mat, i, j, val);
			if (err)
			{
				return NULL;
			}
		}
	}
	return mat;
}

struct sp_matrix *sp_matrix_load(char *path)
{
	FILE *fi = fopen(path, "r");
	if (fi == NULL)
	{
		return NULL;
	}
	int res, nlines, ncols;
	double prec;
	res = fscanf(fi, "%d %d %f", &nlines, &ncols, &prec);
	if (res < 3)
	{
		return NULL;
	}
	struct sp_matrix *mat = sp_matrix_init(prec, nlines, ncols);
	int flag = 1;
	while (flag)
	{
		int i, j;
		double val;
		flag = fscanf(fi, "%d %d %f", i, j, val);
		int err = sp_matrix_set(mat, i, j, val);
		if (err)
		{
			return NULL;
		}
	}
	return mat;
}

//FONCTIONS DE TEST

void matrix_grap(const struct matrix *matrix)
{
	int i;
	for (i = 0; i < matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < matrix->ncols; j++)
		{
			printf("%f ", matrix->elems[i][j] );
		}
		printf("\n");
	}
	return;
}

void sp_matrix_grap(const struct sp_matrix *mat)
{
	struct matrix *matrix = sp_matrix_to_matrix(mat);
	int i;
	for (i = 0; i < matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < matrix->ncols; j++)
		{
			printf("%f ", matrix->elems[i][j]);
		}
		printf("\n");
	}
	return;
}