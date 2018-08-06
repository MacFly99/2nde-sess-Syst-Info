#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
/*
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
void main() 
{ 
	int err = 0;
	printf("Creation des matrices \n");
	struct matrix *mat1 = matrix_init(4, 4);
	struct sp_matrix *sp_mat1 = sp_matrix_init(0.01, 4, 4);

	err = sp_matrix_grap(sp_mat1);

	err = sp_matrix_set(sp_mat1, 1, 1, 1);
	printf("01 err:%d 1 1 1\n", err);
	err = sp_matrix_grap(sp_mat1);

	err = sp_matrix_set(sp_mat1, 1, 3, 2);
	printf("02 err:%d 1 3 2\n", err);
	err = sp_matrix_grap(sp_mat1);

	err = sp_matrix_set(sp_mat1, 1, 2, 3);
	printf("03 err:%d 1 2 3\n", err);
	err = sp_matrix_grap(sp_mat1);

	err = sp_matrix_set(sp_mat1, 2, 2, 4);
	printf("04 err:%d 2 2 4\n", err);
	err = sp_matrix_grap(sp_mat1);

	err = sp_matrix_set(sp_mat1, 0, 1, 5);
	printf("05 err:%d 0 1 5\n", err);
	err = sp_matrix_grap(sp_mat1);

	err = sp_matrix_set(sp_mat1, 0, 3, 6);
	printf("06 err:%d 0 3 6\n", err);
	err = sp_matrix_grap(sp_mat1);

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
	if (matrix == NULL) //Matrice non-definie, on retourne -1.
	{
		return -1;
	}
	if (i >= matrix->nlines || j >= matrix->ncols) //Postion depassant la taille de la matrice. Cela est compris dans les pre-conditions de la fonction, mais cela rend quand meme le code plus solide
	{
		return -1;
	}
	if (val < matrix->precision && val > -(matrix->precision)) // On remplace la valeur par 0 si elle est comprise en dessous de la precision de la matrice.
	{
		val = 0;
	}
	if (matrix->lines == NULL) //Cas ou la matrice est vide
	{
		if (val == 0)//La matrice reste vide, on ne change rien.
		{
			return 0;
		}
		else //On cree le premier element de la matrice, donc une nouvelle ligne et une nouvelle colonne
		{
			struct elem *element = (struct elem *)malloc(sizeof(struct elem));
			element->j = j;
			element->next = NULL;
			element->value = val;

			struct line *ligne = (struct line *)malloc(sizeof(struct line));
			ligne->elems = element;
			ligne->i = i;
			ligne->next = NULL;

			matrix->lines = ligne;

			return 0;
		}
	}
	else //Cas si la matrice n'est pas vide
	{
		if (matrix->lines->i > i) //Si la premiere ligne existante est plus grande que celle du nouveau nombre.
		{
			if (val == 0) //Si la ligne valait 0, et que la nouvelle valeur aussi, on ne change rien
			{
				return 0;
			}
			else //On cree un nouvel element et une nouvelle ligne, celle ci etant la premiere ligne
			{
				struct elem *element = (struct elem *)malloc(sizeof(struct elem));
				element->j = j;
				element->next = NULL;
				element->value = val;

				struct line *ligne = (struct line *)malloc(sizeof(struct line));
				ligne->elems = element;
				ligne->i = i;
				ligne->next = matrix->lines;

				matrix->lines = ligne;

				return 0;
			}
		}
		else if (matrix->lines->i == i) //Si la premiere ligne existante est celle du nouvel element.
		{
			if (matrix->lines->elems->j > j) //Si la colonne du premier element existant est plus grande que celle du nouveau nombre.
			{
				if (val == 0) //Si la valeur vaut 0, on ne rajoute pas l'element.
				{
					return 0;
				}
				else //On cree un nouveau element, le premier de la premiere ligne.
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = matrix->lines->elems;
					element->value = val;

					matrix->lines->elems = element;

					return 0;
				}
			}
			else if (matrix->lines->elems->j == j) //Si on doit modifier la valeur du premier element de la premiere ligne.
			{
				if (val == 0) //Si la nouvelle valeure est 0, on doit liberer la memoire de l'element present avant.
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
				else //Si la valeur est differente de 0, on remplace la valeur du premier element de la premiere ligne par la nouvelle valeur.
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

		else if (matrix->lines->next == NULL)//Si il n'y a qu'une ligne et que le nouvel element n'en fait pas partie, on cree une nouvelle ligne+nouvel elem
		{
			if (matrix->lines->i > i)//L'element a creer est dans une ligne plus petite que la ligne deja existante
			{
				if (val == 0)//La matrice garde une seule ligne non nulle
				{
					return 0;
				}
				else //On cree un element de la matrice dans une nouvelle ligne, donc une nouvelle ligne et une nouvelle colonne
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = NULL;
					element->value = val;

					struct line *ligne = (struct line *)malloc(sizeof(struct line));
					ligne->elems = element;
					ligne->i = i;
					ligne->next = matrix->lines;

					matrix->lines = ligne;

					return 0;
				}
			}
			else if (matrix->lines->i == i)//L'element a creer se place dans la seule ligne deja existente, on cherche ou le placer dans la ligne
			{
			//struct line *prePointeurLi = pointeurli;
			//pointeurli = pointeurli->next;  pointeurli => matrix->lines
			if (matrix->lines->elems->j > j)//L'element a creer se place avant le premier element de la seule ligne 
			{
				if (val == 0)
				{
					return 0;
				}
				else
				{

					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = matrix->lines->elems;
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
						return -3;
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
			else //L'element a creer est dans une ligne plus grande que la ligne deja existante
			{
				if (val == 0)//La matrice garde une seule ligne non nulle
				{
					return 0;
				}
				else //On cree un element de la matrice dans une nouvelle ligne, donc une nouvelle ligne et une nouvelle colonne
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = NULL;
					element->value = val;

					struct line *ligne = (struct line *)malloc(sizeof(struct line));
					ligne->elems = element;
					ligne->i = i;
					ligne->next = NULL;

					matrix->lines->next = ligne;

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
			struct line *prePointeurLi = pointeurli;
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
					element->next = pointeurli->elems;
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
							prePointeurLi->next = NULL;
							return 0;
						}
						else
						{
							prePointeurLi->next = pointeurli->next;
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
		return -1;
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

int matrix_grap(const struct matrix *matrix)
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
	return 0;
}

int sp_matrix_grap(const struct sp_matrix *mat)
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