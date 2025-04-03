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

struct Tir citireTir() {
	struct Tir t;
	char buffer[100];

	printf("Introduceti codul TIR-ului: ");
	scanf("%d", &t.cod);

	printf("Introduceti denumirea TIR-ului: ");
	scanf("%s", buffer);
	t.denumire = (char*)malloc(strlen(buffer) + 1);
	strcpy_s(t.denumire, strlen(buffer) + 1, buffer);

	printf("Introduceti numarul de marfuri: ");
	scanf("%d", &t.nrMarfuri);

	t.greutati = (float*)malloc(t.nrMarfuri * sizeof(float));
	for (int i = 0; i < t.nrMarfuri; i++) {
		printf("Introduceti greutatea marfii %d: ", i + 1);
		scanf("%f", &t.greutati[i]);
	}

	return t;
}

void dezalocare(struct Tir* t) {
	free(t->denumire);
	free(t->greutati);
	t->denumire = NULL;
	t->greutati = NULL;
	t->nrMarfuri = 0;
}

int main() {
	struct Tir tir = citireTir();

	printf("\nAfisare TIR:\n");
	afisare(tir);

	float total = calculeazaGreutateTotala(tir);
	printf("Greutate totala: %.2f\n", total);

	int index;
	float nouaGreutate;
	printf("Introduceti indexul marfii de modificat (0 - %d) si noua greutate: ", tir.nrMarfuri - 1);
	scanf("%d %f", &index, &nouaGreutate);
	modificaGreutate(&tir, index, nouaGreutate);

	printf("\nDupa modificare:\n");
	afisare(tir);

	dezalocare(&tir);

	return 0;
}
