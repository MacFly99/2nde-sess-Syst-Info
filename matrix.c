#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

// CREATION DES MATRICES // INIT // // //

struct matrix *matrix_init(unsigned int nlines, unsigned int ncols)
{
	//Allocation de la memoire et initialisation des variables selon les arguments.
	struct matrix *mat = (struct matrix *)malloc(sizeof(struct matrix));
	mat->nlines = nlines;
	mat->ncols = ncols;
	//Le tableau de double est alloue avec des mallocs pour bien correspondre a la structure matrix.
	double ** ele = (double **)malloc(nlines * sizeof(double*));
	int i;
	for (i = 0; i < (int)nlines; i++)
	{
		//A chaque nouvelle ligne, on alloue une nouvelle chaine de doubles. 
		*(ele + i) = (double*)malloc(ncols * sizeof(double));
		int j;
		for (j = 0; j < (int)ncols; j++)
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
	//Liberation de la matrice, on libere d'abord la memoire de chaque ligne avant de liberer le reste de la matrice.
	double **ele = matrix->elems;
	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		free(*(ele + i));
	}

	free(matrix->elems);
	free(matrix);
	return;
}

void sp_matrix_free(struct sp_matrix *matrix)
{
	//Si la matrice vaut NULL, il n'y a rien a faire.
	if (matrix == NULL)
	{
		return;
	}
	//Si la matrice est vide, il n'y a aucun element ni ligne a liberer, on libere donc uniquement la matrice.
	if (matrix->lines == NULL)
	{
		free(matrix);
		return;
	}
	//On cree un pointeur vers une ligne qui va parcourire toutes les lignes de la matrices pour les liberer une a une. Pour ensuite pouvoir liberer la matrice.
	struct line *pointeurLi = matrix->lines;
	if (pointeurLi->next == NULL) //Si la matrice ne contient qu'une seule ligne non-nulle.
	{
		//On cree un pointeur vers un element qui va parcourire tous les elements de la ligne pour les liberer un a un.
		struct elem *pointeurCo = pointeurLi->elems;
		if (pointeurCo->next == NULL) //Si il n'y a qu'un seul element dans la seule ligne. On doit seulement liberer l'element, puis ensuite la matrice
		{
			free(pointeurCo);
		}
		else //S'il y a plus d'un element dans la seule ligne. On va parcourir tous les elements
		{
			//Tant que le dernier element de la ligne n'est pas atteint, on pointe l'element i+1 et on libere l'element i.
			struct elem *nextPointeurCo = pointeurCo->next;
			while (nextPointeurCo != NULL)
			{
				free(pointeurCo);
				pointeurCo = nextPointeurCo;
				nextPointeurCo = nextPointeurCo->next;
			}
			//Une fois la ligne terminee, on libere le dernier element de la ligne.
			free(nextPointeurCo);
		}
		//On peut finalement liberer la seule ligne de notre matrice. 
		free(pointeurLi);
	}
	// S'il y a plusieurs lignes non-nulles dans la matrice.
	else
	{
		//On parcoure les lignes en gardant le pointeur nextPointeurLi sur la ligne i+1 pour pouvoir liberer la ligne i et ses elements sans perdre la ligne i+1.
		struct line *nextPointeurLi = pointeurLi->next;
		while (nextPointeurLi != NULL)
		{
			//On applique la meme methode pour liberer les elements de la ligne i que dans le cas d'une ligne non-nulle unique (ligne 281).
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
		//Quand la derniere ligne est atteinte, on libere ses elements par la meme methode que precedement (ligne 281).
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
		//On libere finalement la derniere ligne, liberee de ses elements
		free(nextPointeurLi);
	}
	//Une fois tous les lignes et elements liberer, on peut liberer la matrice.
	free(matrix);
	return;
}

// DEFINITION DE LA VALEUR D'UN ELEMENT D'UNE MATRICE // SET // // //

int matrix_set(struct matrix *matrix, unsigned int i, unsigned int j, double val)
{
	if (matrix == NULL) //Si la matrice est NULL, on retourne -1.
	{
		return -1;
	}
	if (i >= matrix->nlines || j >= matrix->ncols) //Si la position qu'on veut changer n'est pas comprise dans le tableau. Cela est deja compris dans les pre-conditions de la fonction, mais cela rend quand meme le code plus solide
	{
		return -1;
	}
	//Si les conditions precedentes sont remplies, on n'a plus qu'a modifier la valeur de l'element.
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
			else if (matrix->lines->elems->next == NULL) //Si il n'exite qu'un seul element dans la ligne, et qu'on doit creer un element apres celui-ci.
			{
				struct elem *element = (struct elem *)malloc(sizeof(struct elem));
				element->j = j;
				element->next = NULL;
				element->value = val;

				matrix->lines->elems->next = element;

				return 0;
			}
			//Si aucune des conditions precedente n'est remplie, on parcourt toute la ligne jusqu'a trouver l'emplacement du nouvel element.
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

			if (pointeurco->next->j == j) //Si l'element a ajouter ne valait pas 0 avant.
			{
				if (val == 0) //Si la nouvelle valeur est 0, on doit liberer l'element.
				{
					if (pointeurco->next != NULL) //Si notre element a retirer n'etait pas le dernier, on lie l'element precedent a l'element suivant avant de liberer l'element.
					{
						struct elem *att = pointeurco->next;
						pointeurco->next = pointeurco->next->next;
						free(att);

						return 0;
					}
					else //Si l'element a supprimer etait le dernier de la ligne, on libere juste l'element
					{
						free(pointeurco);
						return 0;
					}
				}
				else //Si on doit seulement modifier la valeur de l'element.
				{
					pointeurco->next->value = val;
					return 0;
				}
			}
			else if (!flag) //Si notre element est au dela du dernier element actuel.
			{
				if (val == 0) //Si la valeur vaut 0, on ne fait rien.
				{
					return 0;
				}
				else //On rajoute un element au bout de la liste
				{
					struct elem *element = (struct elem *)malloc(sizeof(struct elem));
					element->j = j;
					element->next = NULL;
					element->value = val;

					pointeurco->next->next = element;

					return 0;
				}
			}
			else //Si notre nouvel element remplace un 0 dans la matrice.
			{
				if (val == 0) //Si la valeur vaut 0, on ne doit rien faire.
				{
					return 0;
				}
				else //On cree un nouvel element qu'on rajoute au milieu de la liste, attention aux ->next des elements.
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
		else if (matrix->lines->next == NULL && matrix->lines->i < i)//Si il n'y a qu'une ligne et qu'on doit rajouter une ligne apres celle-ci
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

		//On parcourt les lignes pour trouver celle ou s'arreter, meme systeme que pour trouver le bon element dans la 1ere ligne.
		// Une fois qu'on a trouve la bonne ligne, la demarche est exactement la meme que quand il n'y a qu'une seule ligne, voir ci-dessus.
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
			else if (pointeurli->elems->next == NULL)
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

					pointeurli->elems->next = element;

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
	if (matrix == NULL) //Si la matrice vaut NULL, on retourne -1.
	{
		return -1;
	}
	if (i >= matrix->nlines || j >= matrix->ncols) //Si la position qu'on veut recuperer n'est pas comprise dans le tableau. Cela est deja compris dans les pre-conditions de la fonction, mais cela rend quand meme le code plus solide
	{
		return -1;
	}
	else //On retourne la valeure indiquee dans le tableau de double a la position demandee.
	{
		return matrix->elems[i][j];
	}
}

double sp_matrix_get(const struct sp_matrix *matrix, unsigned int i, unsigned int j)
{
	if (matrix == NULL) //Si la matrice est NULL, on retourne -1.
	{
		return -1;
	}
	if (i >= matrix->nlines || j >= matrix->ncols) //Si la position qu'on veut changer n'est pas comprise dans le tableau. Cela est deja compris dans les pre-conditions de la fonction, mais cela rend quand meme le code plus solide
	{
		return -1;
	}
	else if (matrix->lines == NULL) //Si la matrice est vide, toutes les valeurs valent 0, on peut donc retourner 0.
	{
		return 0;
	}
	else
	{
		//On cree un pointeur vers une ligne qui va parcourir toutes les lignes jusqu'a trouver la ligne de notre valeur, ou la fin de la matrice.
		struct line *pointeurli = matrix->lines;
		int flag = 1;
		while (pointeurli->next != NULL && flag)
		{
			if (pointeurli->i < i) //Notre pointeur n'a pas atteint la ligne demandee, on passe a la suivante et on recommence la boucle.
			{
				pointeurli = pointeurli->next;
			}
			else if (pointeurli->i == i)//On a trouve la bonne ligne, on arrete la boucle.
			{
				flag = 0;
			}
			else //Le dernier cas restant est quand on a depasse la ligne demandee sans la rencontrer, cela veut dire que la ligne est vide, et que l'element demande vaut forcement 0, on retourne donc 0.
			{
				return 0;
			}
		}
		if (pointeurli->next == NULL && pointeurli->i != i) //Si on est arrive a la fin des lignes, et que la derniere ligne n'est pas celle de l'element demande, on retourne 0 car la ligne demandee est au dela de la derniere ligne non-vide.
		{
			return 0;
		}

		//On cree un pointeur vers un element qui va parcourir la ligne trouvee pour pouvoir trouver le bon element. Le fonctionnement est exactement le meme que le pointeur vers la ligne (ligne 849)
		struct elem *pointeurco = pointeurli->elems;
		while (pointeurco->next != NULL)
		{
			if (pointeurco->j < j)
			{
				pointeurco = pointeurco->next;
			}
			else if (pointeurco->j == j) //Si on a trouve le bon element, on retourne la valeur de l'element.
			{
				return pointeurco->value;
			}
			else
			{
				return 0;
			}
		}
		if (pointeurco->j == j) //Si on est arrive ici, c'est qu'on est a la fin de la ligne, si le dernier element est le bon, on retourne sa valeur, sinon on retourne 0.
		{
			return pointeurco->value;
		}
	}
	return 0;
}

// ADDITION DE DEUX MATRICES // ADD // // //

struct matrix *matrix_add(const struct matrix *m1, const struct matrix *m2)
{
	if (m1->ncols != m2->ncols || m1->nlines != m2->nlines) //Si les matrices ne font pas la meme taille, la somme est impossible, on retourne NULL.
	{
		return NULL;
	}
	struct matrix *mat = matrix_init(m1->nlines, m2->ncols); //On cree la nouvelle matrice, le resultat de l'addition.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)m1->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)m1->nlines; j++)
		{
			//On fait une double boucle qui parcourt tous les element, on utilise la fonction get pour chaque element, et on le set dans la nouvelle matrice avec la fonction set.
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
	//Une fois les matrices completement parcourues, on renvoie le resultat.
	return mat;
}

struct sp_matrix *sp_matrix_add(const struct sp_matrix *m1, const struct sp_matrix *m2)
{
	if (m1->ncols != m2->ncols || m1->nlines != m2->nlines)//Si les matrices ne font pas la meme taille, la somme est impossible, on retourne NULL.
	{
		return NULL;
	}

	double prec;//Condition pour recuperer la precision la plus petite des deux matrices a sommer, elle nous servira comme precision dans la matrice resultat
	if (m1->precision < m2->precision)
	{
		prec = m1->precision;
	}
	else
	{
		prec = m2->precision;
	}

	struct sp_matrix *mat = sp_matrix_init(prec, m1->nlines, m2->ncols);//On cree la nouvelle matrice, le resultat de l'addition.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)m1->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)m1->nlines; j++)
		{
			//On fait une double boucle qui parcourt tous les element, on utilise la fonction get pour chaque element, et on le set dans la nouvelle matrice avec la fonction set.
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
	struct matrix *mat = matrix_init(matrix->ncols, matrix->nlines); //On cree la nouvelle matrice, avec les valeurs de nlines et ncols inversees de la matrice entree en argument.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)matrix->ncols; j++)
		{
			//On fait une double boucle qui parcourt tous les element, on utilise la fonction get pour chaque element, et on le set dans la nouvelle matrice avec la fonction set aux coordonnees inverses.
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
	struct sp_matrix *mat = sp_matrix_init(matrix->precision, matrix->ncols, matrix->nlines);//On cree la nouvelle matrice, avec les valeurs de nlines et ncols inversees de la matrice entree en argument.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)matrix->ncols; j++)
		{
			//On fait une double boucle qui parcourt tous les element, on utilise la fonction get pour chaque element, et on le set dans la nouvelle matrice avec la fonction set aux coordonnees inverses.
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
	if (m1->ncols != m2->nlines) //Si la premiere matrice n'a pas autant de colonnes que la deuxieme a de lignes, la multiplication n'est pas possible, on retourne NULL.
	{
		return NULL;
	}
	struct matrix *mat = matrix_init(m1->nlines, m2->ncols); // On cree la nouvelle matrice avec le nombre de ligne de la premiere matrice pour les lignes, et le nombre de colonnes de la seconde matrice pour les colonnes.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)m1->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)m2->ncols; j++)
		{
			//On fait une double boucle pour calculer la valeur de chaque valeur de la nouvelle matrice.
			int k;
			double res = 0;
			for (k = 0; k < (int)m1->ncols; k++)
			{
				//Pour chaque element de la nouvelle matrice, on fait une boucle qui parcourt la ligne (i,*) de la premiere matrice et la colonne (*,j) de la deuxieme matrice, et additionne au resultat le resultat de la multiplication mat1(i,k)*mat2(k,j).
				double prod1 = matrix_get(m1, i, k);
				double prod2 = matrix_get(m2, k, j);
				res += (prod1*prod2);

				//Une fois la valeure du nouvel element calcule, on le set a sa place dans la nouvelle matrice.
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
	if (m1->ncols != m2->nlines)//Si la premiere matrice n'a pas autant de colonnes que la deuxieme a de lignes, la multiplication n'est pas possible, on retourne NULL.
	{
		return NULL;
	}

	double prec;//Condition pour recuperer la precision la plus petite des deux matrices a multiplier, elle nous servira comme precision dans la matrice resultat
	if (m1->precision < m2->precision)
	{
		prec = m1->precision;
	}
	else
	{
		prec = m2->precision;
	}

	struct sp_matrix *mat = sp_matrix_init(prec, m1->nlines, m2->ncols);// On cree la nouvelle matrice avec le nombre de ligne de la premiere matrice pour les lignes, et le nombre de colonnes de la seconde matrice pour les colonnes.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)m1->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)m2->ncols; j++)
		{
			//On fait une double boucle pour calculer la valeur de chaque valeur de la nouvelle matrice.
			int k;
			double res = 0;
			for (k = 0; k < (int)m1->ncols; k++)
			{
				double prod1 = sp_matrix_get(m1, i, k);
				double prod2 = sp_matrix_get(m2, k, j);
				res += (prod1*prod2);

				//Une fois la valeure du nouvel element calcule, on le set a sa place dans la nouvelle matrice.
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
	struct sp_matrix *mat = sp_matrix_init(precision, matrix->nlines, matrix->ncols); //On cree la matrice creuse avec le meme nombre de lignes et de colonnes que la matrice a convertir.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)matrix->ncols; j++)
		{
			//On fait une double boucle pour parcourir tous les elements de la matrice a convertir.
			if (matrix->elems[i][j] != 0) //On ne set que les elements non-nuls car la matrice etait de toute facon initialement nulle, cela rend la fonction plus rapide.
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
	struct matrix *mat = matrix_init(matrix->nlines, matrix->ncols);//On cree la matrice avec le meme nombre de lignes et de colonnes que la matrice a convertir.
	if (mat == NULL)
	{
		return NULL;
	}
	if (matrix->lines == NULL) //Si la matrice a convertir est nulle, on retourne directement la matrice.
	{
		return mat;
	}

	struct line *pointeurLi = matrix->lines;
	while (pointeurLi != NULL)
	{
		struct elem *pointeurCo = pointeurLi->elems;
		while (pointeurCo != NULL)
		{
			//On fait une double boucle qui parcourt tous les elements non-nuls pour les set dans la nouvelle matrice.
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
	//On commence par ouvrir le fichier pour pouvoir y sauvegarder la matrice. L'option "w" indique qu'on va y erire, si le fichier existait deja, il est efface pour avoir un document vierge.
	FILE *fi = fopen(path, "w");
	if (fi == NULL)
	{
		return -1;
	}
	//On commence par inscrire le nombre de lignes et de colonnes de notre matrice, cela permettra d'etre plus facilement lue.
	fprintf(fi, "%d %d ", matrix->nlines, matrix->ncols);

	int i;
	for (i = 0; i < (int)matrix->nlines; i++)
	{
		int j;
		for (j = 0; j < (int)matrix->ncols; j++)
		{
			//On finit par parcourir toute notre matrice et ecrire la valeur de tous les elements de la matrice.
			fprintf(fi, "%f ", matrix->elems[i][j]);
		}
	}
	//Une fois tous les elements parcourus, on ferme le fichier.
	fclose(fi);
	return 0;
}

int sp_matrix_save(const struct sp_matrix *matrix, char *path)
{
	//On commence par ouvrir le fichier pour pouvoir y sauvegarder la matrice. L'option "w" indique qu'on va y erire, si le fichier existait deja, il est efface pour avoir un document vierge.
	FILE *fi = fopen(path, "w");
	if (fi == NULL)
	{
		return -1;
	}
	//On commence par inscrire le nombre de lignes et de colonnes de notre matrice et la precision de celle-ci, cela permettra d'etre plus facilement lue.
	fprintf(fi, "%d %d %f ", matrix->nlines, matrix->ncols, matrix->precision);

	//Si la matrice est vide, on ferme notre fichier.
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
			//On parcourt toute la structure de la matrice, et pour chaque element, on inscrit sa position (i et j) et ensuite la valeur de la matrice a cet emplacement.
			fprintf(fi, "%d %d %f ", pointeurLi->i, pointeurCo->j, pointeurCo->value);
			pointeurCo = pointeurCo->next;
		}
		pointeurLi = pointeurLi->next;
	}
	//Une fois tous les elements parcourus, on ferme le fichier.
	fclose(fi);
	return 0;
}

// CHARGEMENT D'UNE MATRICE A PARTIR D'UN FICHIER // LOAD // // //

struct matrix *matrix_load(char *path)
{
	//On commence par ouvrir le fichier indique par path, si celui ci est vide, on retourne NULL. L'ption "r" indique qu'on va uniquement lire dans le fichier.
	FILE *fi = fopen(path, "r");
	if (fi == NULL)
	{
		return NULL;
	}

	// Ensuite, on lit les 2 premiers entiers du fichier, qui correspondent au nombre de ligne et de colonne de la matrice a creer.
	int res, nlines, ncols;
	res = fscanf(fi, "%d %d ", &nlines, &ncols);
	if (res < 2)
	{
		return NULL;
	}
	struct matrix *mat = matrix_init(nlines, ncols);//On cree ensuite la matrice avec les valeures precedement lues.
	if (mat == NULL)
	{
		return NULL;
	}
	int i;
	for (i = 0; i < nlines; i++)
	{
		int j;
		for (j = 0; j < ncols; j++)
		{
			//On fait ensuite une double boucle qui va parcourir toute la matrice.
			int res2;
			double val;
			// Pour chaque element, on lit dans le fichier la valeur correspondante a la place actuelle dans les boucles, et on set la valeur lue dans la matrice.
			res2 = fscanf(fi, "%lf ", &val);
			if (res2 != 1)
			{
				return NULL;
			}
			int err = matrix_set(mat, i, j, val);
			if (err)
			{
				return NULL;
			}
		}
	}
	//Une fois tous les elements lus, on retourne la matrce cree.
	return mat;
}

struct sp_matrix *sp_matrix_load(char *path)
{
	//On commence par ouvrir le fichier indique par path, si celui ci est vide, on retourne NULL. L'ption "r" indique qu'on va uniquement lire dans le fichier.
	FILE *fi = fopen(path, "r");
	if (fi == NULL)
	{
		return NULL;
	}

	// Ensuite, on lit les 2 premiers entiers et le premier double du fichier, qui correspondent au nombre de ligne et de colonne et la precision de la matrice a creer.
	int res, nlines, ncols;
	double prec;
	res = fscanf(fi, "%d %d %lf", &nlines, &ncols, &prec);
	if (res < 3)
	{
		return NULL;
	}
	struct sp_matrix *mat = sp_matrix_init(prec, nlines, ncols);//On cree ensuite la matrice avec les valeures precedement lues.
	if (mat == NULL)
	{
		return NULL;
	}
	int derI = -1;
	int derJ = -1;
	int flag = 1;
	while (flag)
	{
		//On fait ensuite une boucle qui lit a chaque fois deux entiers et un double. Les deux entiers sont l'emplacement (i,j) d'un element, et le double, la valeure associee a cet element.
		int i, j;
		double val;
		flag = fscanf(fi, "%d %d %lf", &i, &j, &val);
		
		//On verifie que le nouvel element lu n'est pas le meme que celui precedement lu en comparant les valeurs de i et j pour eviter une boucle infinie une fois tous les elements lus.
		if ((derI == i && derJ == j) || !flag) //Si les elements lus ont les meme i et j que les precedent, on interrompt la boucle.
		{
			flag = 0;
		}
		else //Si l'element est nouveau, on remplace les precedents i et j par les nouveaux, lu dans la boucle actuelle.
		{
			derI = i;
			derJ = j;
		}
		int err = sp_matrix_set(mat, i, j, val); //On set l'element lu de la boucle.
		if (err)
		{
			return NULL;
		}
	}
	//Une fois tous les elements lus, et la matrice remplie, on retourne la matrice lue dans le fichier.
	return mat;
}