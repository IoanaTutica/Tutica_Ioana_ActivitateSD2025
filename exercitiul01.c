#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Magazin {
    int cod;
    char* denumire;
    int nrProduse;
    float* preturi; 
};
typedef struct Magazin Magazin;

Magazin citireMagazin() {
    Magazin m;
    char buffer[100];
    
    printf("Introduceti codul magazinului: ");
    scanf("%d", &m.cod);

    printf("Introduceti denumirea magazinului: ");
    scanf("%s", buffer);
    m.denumire = malloc(strlen(buffer) + 1);
    strcpy(m.denumire, buffer);

    printf("Introduceti numarul de produse: ");
    scanf("%d", &m.nrProduse);

    m.preturi = malloc(m.nrProduse * sizeof(float));
    printf("Introduceti preturile pentru fiecare produs:\n");
    for (int i = 0; i < m.nrProduse; i++) {
        printf("Pretul produsului %d: ", i + 1);
        scanf("%f", &m.preturi[i]);
    }

    return m;
}

float calculeazaMediaPreturilor(Magazin m) {
    float suma = 0;
    for (int i = 0; i < m.nrProduse; i++) {
        suma += m.preturi[i];
    }
    return suma / m.nrProduse;
}

void modificaPret(Magazin* m, int indexProdus, float noulPret) {
    if (indexProdus >= 0 && indexProdus < m->nrProduse) {
        m->preturi[indexProdus] = noulPret;
    } else {
        printf("Indexul produsului este invalid.\n");
    }
}

void afiseazaMagazin(Magazin m) {
    printf("Cod magazin: %d\n", m.cod);
    printf("Denumire magazin: %s\n", m.denumire);
    printf("Numar produse: %d\n", m.nrProduse);
    printf("Preturi produse:\n");
    for (int i = 0; i < m.nrProduse; i++) {
        printf("Produs %d: %.2f\n", i + 1, m.preturi[i]);
    }
}

int main() {
    Magazin m = citireMagazin();
    
    float media = calculeazaMediaPreturilor(m);
    printf("Media preturilor produselor: %.2f\n", media);
    int produsDeModificat;
    float nouPret;
    printf("Introduceti indexul produsului de modificat (0-%d) si noul pret: ", m.nrProduse - 1);
    scanf("%d %f", &produsDeModificat, &nouPret);
    modificaPret(&m, produsDeModificat, nouPret);
    afiseazaMagazin(m);
    free(m.denumire);
    free(m.preturi);
    
    return 0;
}


