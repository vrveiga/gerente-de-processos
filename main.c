#include <stdio.h>
#include <string.h>

#define MAX_DESCR 55

typedef struct {
    int hh;
    int mm;
    int ss;
} horario;

typedef struct {
    int prior;
    horario chegada;
    char descricao[MAX_DESCR];
} celula;

int busca_binaria_prior(celula* proc, int sz, int p) {
    int l = 0, r = sz-1;
    int ret = 0;

    while (l <= r) {
        int m = (l + r) / 2;
        if (proc[m].prior < p) {
            ret = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return l;
}

void add(celula* p_prior, celula* p_tempo, int* sz) {
    int prior, hh, mm, ss;
    char desc[MAX_DESCR];

    scanf(" %d %d:%d:%d %s", &prior, &hh, &mm, &ss, desc);
    horario tempo = {hh, mm, ss};

    int id = busca_binaria_prior(p_prior, *sz, prior);

    for (int i = *sz; i > id; i--)
        p_prior[i] = p_prior[i-1];

    p_prior[id].prior = prior;
    p_prior[id].chegada = tempo;
    strncpy(p_prior[id].descricao, desc, MAX_DESCR);

    (*sz)++;
}

void print(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %c%c", &op, &op);

    if (op == 'p') {
        for (int i = 0; i < sz; i++) {
            printf("%d %02d:%02d:%02d %s\n", p_prior[i].prior, p_prior[i].chegada.hh,
                    p_prior[i].chegada.mm, p_prior[i].chegada.ss, p_prior[i].descricao);
        }
    }
}

int main() {
    char comando[50]; //aaaaa

    celula p_prior[150], p_tempo[150];
    int sz = 0;

    while (scanf(" %s", comando) && strcmp(comando, "quit")) {
        if (!strcmp(comando, "add"))
            add(p_prior, p_tempo, &sz);
        else if (!strcmp(comando, "print"))
            print(p_prior, p_tempo, sz);

        
    }
}