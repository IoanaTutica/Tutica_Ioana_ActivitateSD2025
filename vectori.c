#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct Tir {
	int cod;
	char* denumire;
	int nrMarfuri;
	float* greutati;
};

struct Tir initializare(int cod, const char* denumire, int nrMarfuri, float* greutati) {
	struct Tir t;
	t.cod = cod;
	t.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy_s(t.denumire, strlen(denumire) + 1, denumire);
	t.nrMarfuri = nrMarfuri;
	t.greutati = (float*)malloc(sizeof(float) * nrMarfuri);
	for (int i = 0; i < nrMarfuri; i++) {
		t.greutati[i] = greutati[i];
	}
	return t;
}

void afisare(struct Tir t) {
	printf("Cod TIR: %d\n", t.cod);
	printf("Denumire: %s\n", t.denumire);
	printf("Numar marfuri: %d\n", t.nrMarfuri);
	for (int i = 0; i < t.nrMarfuri; i++) {
		printf("Greutate marfa %d: %.2f\n", i + 1, t.greutati[i]);
	}
}

float calculeazaGreutateTotala(struct Tir t) {
	float suma = 0;
	for (int i = 0; i < t.nrMarfuri; i++) {
		suma += t.greutati[i];
	}
	return suma;
}

void modificaGreutate(struct Tir* t, int index, float nouaGreutate) {
	if (index >= 0 && index < t->nrMarfuri) {
		t->greutati[index] = nouaGreutate;
	}
	else {
		printf("Index invalid!\n");
	}
}

void afisareVector(struct Tir* vector, int dim) {
	for (int i = 0; i < dim; i++) {
		printf("-----------\n");
		afisare(vector[i]);
	}
}

struct Tir* copiazaTiruriGrele(struct Tir* vector, int dim, int* dimNou) {
	*dimNou = 0;
	for (int i = 0; i < dim; i++) {
		if (calculeazaGreutateTotala(vector[i]) > 5000) {
			(*dimNou)++;
		}
	}
	struct Tir* nou = (struct Tir*)malloc(sizeof(struct Tir) * (*dimNou));
	int k = 0;
	for (int i = 0; i < dim; i++) {
		if (calculeazaGreutateTotala(vector[i]) > 5000) {
			nou[k] = initializare(vector[i].cod, vector[i].denumire, vector[i].nrMarfuri, vector[i].greutati);
			k++;
		}
	}
	return nou;
}

struct Tir* mutaTiruriUsoare(struct Tir* vector, int dim, int* dimNou) {
	*dimNou = 0;
	for (int i = 0; i < dim; i++) {
		if (vector[i].nrMarfuri < 3) {
			(*dimNou)++;
		}
	}
	struct Tir* nou = (struct Tir*)malloc(sizeof(struct Tir) * (*dimNou));
	int k = 0;
	for (int i = 0; i < dim; i++) {
		if (vector[i].nrMarfuri < 3) {
			nou[k] = initializare(vector[i].cod, vector[i].denumire, vector[i].nrMarfuri, vector[i].greutati);
			k++;
		}
	}
	return nou;
}

struct Tir* concateneazaVectori(struct Tir* v1, int dim1, struct Tir* v2, int dim2, int* dimRezultat) {
	*dimRezultat = dim1 + dim2;
	struct Tir* rezultat = (struct Tir*)malloc(sizeof(struct Tir) * (*dimRezultat));
	for (int i = 0; i < dim1; i++) {
		rezultat[i] = initializare(v1[i].cod, v1[i].denumire, v1[i].nrMarfuri, v1[i].greutati);
	}
	for (int i = 0; i < dim2; i++) {
		rezultat[dim1 + i] = initializare(v2[i].cod, v2[i].denumire, v2[i].nrMarfuri, v2[i].greutati);
	}
	return rezultat;
}

void dezalocare(struct Tir* t) {
	free(t->denumire);
	free(t->greutati);
	t->denumire = NULL;
	t->greutati = NULL;
	t->nrMarfuri = 0;
}

void dezalocareVector(struct Tir** vector, int* dim) {
	for (int i = 0; i < *dim; i++) {
		dezalocare(&(*vector)[i]);
	}
	free(*vector);
	*vector = NULL;
	*dim = 0;
}

int main() {
	int dim = 5;
	struct Tir* vector = (struct Tir*)malloc(sizeof(struct Tir) * dim);

	float g1[] = { 2000, 1800, 1500 };
	float g2[] = { 1000, 1000, 500 };
	float g3[] = { 800 };
	float g4[] = { 4000, 1300 };
	float g5[] = { 2500, 3000, 1000, 800 };

	vector[0] = initializare(1, "TirA", 3, g1);
	vector[1] = initializare(2, "TirB", 3, g2);
	vector[2] = initializare(3, "TirC", 1, g3);
	vector[3] = initializare(4, "TirD", 2, g4);
	vector[4] = initializare(5, "TirE", 4, g5);

	printf("\n=== Vector initial ===\n");
	afisareVector(vector, dim);

	int dimGrele = 0;
	struct Tir* tiruriGrele = copiazaTiruriGrele(vector, dim, &dimGrele);
	printf("\n=== Tiruri cu greutate totala > 5000 ===\n");
	afisareVector(tiruriGrele, dimGrele);

	int dimUsoare = 0;
	struct Tir* tiruriUsoare = mutaTiruriUsoare(vector, dim, &dimUsoare);
	printf("\n=== Tiruri cu mai putin de 3 marfuri ===\n");
	afisareVector(tiruriUsoare, dimUsoare);

	int dimConcat = 0;
	struct Tir* tiruriConcat = concateneazaVectori(tiruriGrele, dimGrele, tiruriUsoare, dimUsoare, &dimConcat);
	printf("\n=== Vector concatenat ===\n");
	afisareVector(tiruriConcat, dimConcat);

	dezalocareVector(&vector, &dim);
	dezalocareVector(&tiruriGrele, &dimGrele);
	dezalocareVector(&tiruriUsoare, &dimUsoare);
	dezalocareVector(&tiruriConcat, &dimConcat);

	return 0;
}
