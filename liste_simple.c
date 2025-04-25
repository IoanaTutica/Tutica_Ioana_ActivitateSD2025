#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);
	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	} else {
		*cap = nou;
	}
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			adaugaMasinaInLista(&cap, citireMasinaDinFisier(f));
		}
		fclose(f);
	}
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	while (*cap) {
		Nod* p = *cap;
		*cap = (*cap)->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

void stergeNodPozitie(Nod** cap, int index) {
	if (*cap == NULL || index < 0) return;
	if (index == 0) {
		Nod* aux = *cap;
		*cap = (*cap)->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
		return;
	}
	Nod* p = *cap;
	int poz = 0;
	while (p->next && poz < index - 1) {
		p = p->next;
		poz++;
	}
	if (p->next) {
		Nod* deSters = p->next;
		p->next = deSters->next;
		free(deSters->info.model);
		free(deSters->info.numeSofer);
		free(deSters);
	}
}

void inserareSortataDupaPret(Nod** cap, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;
	if (*cap == NULL || (*cap)->info.pret > m.pret) {
		nou->next = *cap;
		*cap = nou;
	} else {
		Nod* p = *cap;
		while (p->next && p->next->info.pret < m.pret) {
			p = p->next;
		}
		nou->next = p->next;
		p->next = nou;
	}
}

Masina* masiniCuNrUsiPeste4(Nod* cap, int* nr) {
	*nr = 0;
	Nod* p = cap;
	while (p) {
		if (p->info.nrUsi > 4) {
			(*nr)++;
		}
		p = p->next;
	}
	Masina* vector = (Masina*)malloc(sizeof(Masina) * (*nr));
	int k = 0;
	p = cap;
	while (p) {
		if (p->info.nrUsi > 4) {
			vector[k].id = p->info.id;
			vector[k].nrUsi = p->info.nrUsi;
			vector[k].pret = p->info.pret;
			vector[k].model = (char*)malloc(strlen(p->info.model) + 1);
			strcpy(vector[k].model, p->info.model);
			vector[k].numeSofer = (char*)malloc(strlen(p->info.numeSofer) + 1);
			strcpy(vector[k].numeSofer, p->info.numeSofer);
			vector[k].serie = p->info.serie;
			k++;
		}
		p = p->next;
	}
	return vector;
}

void interschimbaPozitii(Nod* cap, int poz1, int poz2) {
	if (poz1 == poz2 || cap == NULL) return;
	Nod* nod1 = NULL;
	Nod* nod2 = NULL;
	Nod* p = cap;
	int index = 0;
	while (p) {
		if (index == poz1) nod1 = p;
		if (index == poz2) nod2 = p;
		index++;
		p = p->next;
	}
	if (nod1 && nod2) {
		Masina temp = nod1->info;
		nod1->info = nod2->info;
		nod2->info = temp;
	}
}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	printf("--- Lista initiala ---\n");
	afisareListaMasini(cap);

	printf("\n--- Stergere de pe pozitia 2 ---\n");
	stergeNodPozitie(&cap, 2);
	afisareListaMasini(cap);

	printf("\n--- Inserare sortata dupa pret ---\n");
	Masina mNou = { 99, 4, 11000, strdup("Golf"), strdup("Ioana"), 'C' };
	inserareSortataDupaPret(&cap, mNou);
	afisareListaMasini(cap);

	printf("\n--- Salvare in vector masini cu nrUsi > 4 ---\n");
	int nr = 0;
	Masina* v = masiniCuNrUsiPeste4(cap, &nr);
	for (int i = 0; i < nr; i++) {
		afisareMasina(v[i]);
		free(v[i].model);
		free(v[i].numeSofer);
	}
	free(v);

	printf("\n--- Interschimbare pozitii 0 si 2 ---\n");
	interschimbaPozitii(cap, 0, 2);
	afisareListaMasini(cap);

	dezalocareListaMasini(&cap);
	return 0;
}
