#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

bool eh_menor(horario a, horario b) {
    if (a.hh != b.hh) return a.hh < b.hh;
    if (a.mm != b.mm) return a.mm < b.mm;
    if (a.ss != b.ss) return a.ss < b.ss;
    return false;
}

int busca_binaria_prior(celula* proc, int sz, int p) {
    int l = 0, r = sz-1;
    int ret = 0;

    while (l <= r) {
        int m = (l + r)/2;
        if (proc[m].prior < p) {
            ret = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return l;
}

int busca_binaria_tempo(celula* proc, int sz, horario t) {
    int l = 0, r = sz - 1;
    int ret = sz;

    while (l <= r) {
        int m = (l + r)/2;
        if (eh_menor(t, proc[m].chegada)) {
            ret = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return ret;
}

void swap(celula* a, celula* b) {
    celula c = *a;
    *a = *b;
    *b = c;
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

    id = busca_binaria_tempo(p_tempo, *sz, tempo);
    for (int i = *sz; i > id; i--)
        p_tempo[i] = p_tempo[i-1];

    p_tempo[id].prior = prior;
    p_tempo[id].chegada = tempo;
    strncpy(p_tempo[id].descricao, desc, MAX_DESCR);

    (*sz)++;
}

void exec(celula* p_prior, celula* p_tempo, int* sz) {
    char op;
    scanf(" %c%c", &op, &op);

    int id_p, id_t;

    if (op == 'p') {
        id_p = 0;
        for (int i = 0; i < *sz; i++) {
            if (p_tempo[i].prior == p_prior[0].prior) {
                id_t = i;
                break;
            }
        }
    } else if (op == 't') {
        id_t = 0;
        for (int i = 0; i < *sz; i++) {
            if (p_prior[i].prior == p_tempo[0].prior) {
                id_p = i;
                break;
            }
        }
    }

    for (int i = id_p; i < *sz; i++)
        p_prior[i] = p_prior[i+1];

    for (int i = id_t; i < *sz; i++)
        p_tempo[i] = p_tempo[i+1];

    (*sz)--;
}

void next(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %c%c", &op, &op);
    if (sz == 0) return;
    
    if (op == 'p')
        printf("%d %02d:%02d:%02d %s\n", p_prior[0].prior, p_prior[0].chegada.hh,
                p_prior[0].chegada.mm, p_prior[0].chegada.ss, p_prior[0].descricao);
    else if (op == 't')
        printf("%d %02d:%02d:%02d %s\n", p_tempo[0].prior, p_tempo[0].chegada.hh,
                p_tempo[0].chegada.mm, p_tempo[0].chegada.ss, p_tempo[0].descricao);

    printf("\n");
}

void change(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %c%c", &op, &op);

    if (op == 'p') {
        int prior_from, prior_to;
        scanf(" %d|%d", &prior_from, &prior_to);

        int id;
        for (int i = 0; i < sz; i++) {
            if (p_tempo[i].prior == prior_from) {
                p_tempo[i].prior = prior_to;
            }
            if (p_prior[i].prior == prior_from) {
                id = i;
                p_prior[i].prior = prior_to;
            }
        }

        while (id > 0 && p_prior[id].prior > p_prior[id-1].prior) {
            swap(p_prior + id, p_prior + (id-1));
            id--;
        }

        while (id < sz-1 && p_prior[id].prior < p_prior[id+1].prior) {
            swap(p_prior + id, p_prior + (id+1));
            id++;
        }
    }
    else if (op == 't') {
        int h1, m1, s1, h2, m2, s2;
        scanf(" %d:%d:%d|%d:%d:%d", &h1, &m1, &s1, &h2, &m2, &s2);

        horario hora_from = {h1, m1, s1};
        horario hora_to = {h2, m2, s2};

        int id;
        for (int i = 0; i < sz; i++) {
            if (p_tempo[i].chegada.hh == h1 && p_tempo[i].chegada.mm == m1 && p_tempo[i].chegada.ss == s1) {
                p_tempo[i].chegada = hora_to;
                id = i;
            }
            if (p_prior[i].chegada.hh == h1 && p_prior[i].chegada.mm == m1 && p_prior[i].chegada.ss == s1) {
                p_prior[i].chegada = hora_to;
            }
        }

        while (id > 0 && eh_menor(p_tempo[id].chegada, p_tempo[id-1].chegada)) {
            swap(p_tempo + id, p_tempo + (id-1));
            id--;
        }

        while (id < sz-1 && eh_menor(p_tempo[id+1].chegada, p_tempo[id].chegada)) {
            swap(p_tempo + id, p_tempo + (id+1));
            id++;
        }
    }
}

void print(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %c%c", &op, &op);
    if (sz == 0) return;

    if (op == 'p') {
        for (int i = 0; i < sz; i++) {
            printf("%d %02d:%02d:%02d %s\n", p_prior[i].prior, p_prior[i].chegada.hh,
                    p_prior[i].chegada.mm, p_prior[i].chegada.ss, p_prior[i].descricao);
        }
    } else if (op == 't') {
        for (int i = 0; i < sz; i++) {
            printf("%d %02d:%02d:%02d %s\n", p_tempo[i].prior, p_tempo[i].chegada.hh,
                    p_tempo[i].chegada.mm, p_tempo[i].chegada.ss, p_tempo[i].descricao);
        }
    }
    printf("\n");
}

int main() {
    char comando[10];

    celula p_prior[150], p_tempo[150];
    int sz = 0;

    while (scanf(" %s", comando) && strcmp(comando, "quit")) {
        if (!strcmp(comando, "add"))
            add(p_prior, p_tempo, &sz);
        else if (!strcmp(comando, "next"))
            next(p_prior, p_tempo, sz);
        else if (!strcmp(comando, "exec"))
            exec(p_prior, p_tempo, &sz);
        else if (!strcmp(comando, "change"))
            change(p_prior, p_tempo, sz);
        else if (!strcmp(comando, "print"))
            print(p_prior, p_tempo, sz);
    }

    return 0;
}