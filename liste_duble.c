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
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

struct NodSimplu {
	Masina info;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy(m.model, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m.numeSofer, aux);
	m.serie = *strtok(NULL, sep);
	return m;
}

void afisareMasina(Masina m) {
	printf("ID: %d | Nr usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
		m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina m) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = m;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last) lista->last->next = nou;
	else lista->first = nou;
	lista->last = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista = { NULL, NULL, 0 };
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			adaugaMasinaInLista(&lista, m);
		}
		fclose(f);
	}
	return lista;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.nrNoduri == 0) return 0;
	float suma = 0;
	Nod* p = lista.first;
	while (p) {
		suma += p->masina.pret;
		p = p->next;
	}
	return suma / lista.nrNoduri;
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	Nod* p = lista.first;
	if (!p) return NULL;
	Nod* max = p;
	while (p) {
		if (p->masina.pret > max->masina.pret)
			max = p;
		p = p->next;
	}
	char* rezultat = malloc(strlen(max->masina.numeSofer) + 1);
	strcpy(rezultat, max->masina.numeSofer);
	return rezultat;
}

void stergeNodPozitie(ListaDubla* lista, int poz) {
	if (poz < 0 || poz >= lista->nrNoduri) return;
	Nod* p = lista->first;
	for (int i = 0; i < poz; i++) p = p->next;
	if (p->prev) p->prev->next = p->next;
	else lista->first = p->next;
	if (p->next) p->next->prev = p->prev;
	else lista->last = p->prev;
	free(p->masina.model);
	free(p->masina.numeSofer);
	free(p);
	lista->nrNoduri--;
}

void interschimbaPozitii(ListaDubla lista, int poz1, int poz2) {
	if (poz1 == poz2) return;
	Nod* p1 = lista.first;
	Nod* p2 = lista.first;
	for (int i = 0; i < poz1 && p1; i++) p1 = p1->next;
	for (int i = 0; i < poz2 && p2; i++) p2 = p2->next;
	if (p1 && p2) {
		Masina aux = p1->masina;
		p1->masina = p2->masina;
		p2->masina = aux;
	}
}

void afisareListaSortataCrescator(ListaDubla lista) {
	afisareListaMasiniDeLaInceput(lista);
}

void afisareListaSortataDescrescator(ListaDubla lista) {
	afisareListaMasiniDeLaSfarsit(lista);
}

void adaugaSimpluFinal(NodSimplu** cap, Masina m) {
	NodSimplu* nou = malloc(sizeof(NodSimplu));
	nou->info = m;
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	} else {
		NodSimplu* p = *cap;
		while (p->next) p = p->next;
		p->next = nou;
	}
}

NodSimplu* extrageMasiniNrUsiPeste4(ListaDubla lista) {
	NodSimplu* cap = NULL;
	Nod* p = lista.first;
	while (p) {
		if (p->masina.nrUsi > 4) {
			Masina copie;
			copie.id = p->masina.id;
			copie.nrUsi = p->masina.nrUsi;
			copie.pret = p->masina.pret;
			copie.serie = p->masina.serie;
			copie.model = malloc(strlen(p->masina.model) + 1);
			strcpy(copie.model, p->masina.model);
			copie.numeSofer = malloc(strlen(p->masina.numeSofer) + 1);
			strcpy(copie.numeSofer, p->masina.numeSofer);
			adaugaSimpluFinal(&cap, copie);
		}
		p = p->next;
	}
	return cap;
}

void afisareListaSimpla(NodSimplu* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void dezalocareListaSimpla(NodSimplu** cap) {
	while (*cap) {
		NodSimplu* aux = *cap;
		*cap = (*cap)->next;
		free(aux->info.model);
		free(aux->info.numeSofer);
		free(aux);
	}
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		free(aux->masina.model);
		free(aux->masina.numeSofer);
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	printf("Lista initiala:\n");
	afisareListaMasiniDeLaInceput(lista);

	printf("\nPret mediu: %.2f\n", calculeazaPretMediu(lista));

	char* nume = getNumeSoferMasinaScumpa(lista);
	printf("Sofer masina scumpa: %s\n", nume);
	free(nume);

	printf("\nDupa stergere pozitia 2:\n");
	stergeNodPozitie(&lista, 2);
	afisareListaSortataCrescator(lista);

	printf("\nDupa interschimbare 0 <-> 1:\n");
	interschimbaPozitii(lista, 0, 1);
	afisareListaSortataCrescator(lista);

	printf("\nAfisare invers:\n");
	afisareListaSortataDescrescator(lista);

	printf("\nLista simpla cu masini nr usi > 4:\n");
	NodSimplu* simpla = extrageMasiniNrUsiPeste4(lista);
	afisareListaSimpla(simpla);
	dezalocareListaSimpla(&simpla);

	dezalocareLDMasini(&lista);
	return 0;
}
