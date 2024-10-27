#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DESCR 55 

/*
Decisões de projeto:
    Decidimos armazenar os processos em duas listas:
     - p_prior: guarda os processos ordenadamente com base na prioridade
     - p_tempo: guarda os processos ordenadamente com base no tempo
    Escolhemos essa forma de organização dos dados para otimizar a busca 
    de cada processo com base em um dos dois campos (prioridade e tempo)
    através do algoritmo de busca binária (O(logn))
*/

/*struct que guarda cada campo do horario*/
typedef struct {
    int hh; //hora
    int mm; //minuto
    int ss; //segundo
} horario;


/*struct que guarda informações de um processo*/
typedef struct {
    int prior; //prioridade (1 à 99)
    horario chegada; //horario de chegada
    char descricao[MAX_DESCR]; //descrição do processo
} celula;

/*
Função que compara dois elementos com base no horario e verifica
se o horário a é menor que o horario b.
Retorno:
    horario a < horario b: return true
    horario a > horario b: return false 
*/
bool eh_menor(horario a, horario b) {
    if (a.hh != b.hh) return a.hh < b.hh;
    if (a.mm != b.mm) return a.mm < b.mm;
    if (a.ss != b.ss) return a.ss < b.ss;
    return false;
}

/*
Realiza o algoritmo de busca binária com base na prioridade 
a fim de encontrar o local de inserção de um elemento no array
Parâmetros: 
    *proc - array que contem as celulas ordenadas pela prioridade
    sz - tamanho do array
    p - prioridade que terá sua inserção encontrada

Retorno:
    Retorna o id que a prioridade "p" deve ser inserida
    no vetor "*proc"
*/
int busca_binaria_prior(celula* proc, int sz, int p) {
    //l = id para o inicio da busca no array
    //r = id para o fim da busca no array
    int l = 0, r = sz-1; 

    while (l <= r) { 
        int m = (l + r)/2; //m = id para o meio do array
        if (proc[m].prior < p) { 
            r = m - 1; //ajusta a busca binaria para a primeira metade do array válido
        } else {
            l = m + 1; //ajusta a busca binaria para a segunda metade do array válido
        }
    }

    return l; //retorna o primeiro valor válido
}

/*
Realiza o algoritmo de busca binária com base no tempo a 
fim de encontrar o local de inserção de um elemento no array
Parâmetros: 
    *proc - array que contem as celulas ordenadas pelo horario
    sz - tamanho do array
    t - horário que terá sua inserção encontrada

Retorno:
    Retorna o id que o horário "t" deve ser inserido
    no vetor "*proc"
*/
int busca_binaria_tempo(celula* proc, int sz, horario t) {
    //l = id para o inicio da busca no array
    //r = id para o fim da busca no array
    int l = 0, r = sz - 1;
    int ret = sz;

    while (l <= r) {
        int m = (l + r)/2; //m = id para o meio do array
        if (eh_menor(t, proc[m].chegada)) { //verifica se o horário t é menor que
            ret = m;                          //o horario do elemento de posição m na lista 
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    return ret;
}

/*Troca o valor de duas células*/
void swap(celula* a, celula* b) {
    celula c = *a;
    *a = *b;
    *b = c;
}

/*
Realiza a leitura de adição de um processo nas duas listas:
a que possui os elementos ordenados por tempo e a que possui
os elementos ordenados pelo horário
Parâmetros:
    *p_prior: lista que ordena os processos por prioridade 
    *p_tempo: lista que ordena os processos por tempo 
    *sz: ponteiro para a variável que armazena a quantidade de processos
*/
void add(celula* p_prior, celula* p_tempo, int* sz) {
    int prior, hh, mm, ss;
    char desc[MAX_DESCR];

    //realiza leitura dos dados do processo
    scanf(" %d %d:%d:%d %s", &prior, &hh, &mm, &ss, desc);
    horario tempo = {hh, mm, ss};

    //determina o id de inserção do processo no vetor p_prior com base na prioridade
    int id = busca_binaria_prior(p_prior, *sz, prior);
    for (int i = *sz; i > id; i--) //desloca os elementos da lista a partir da inserção
        p_prior[i] = p_prior[i-1];

    //insere as informações do processo na celula na posição correta no array p_prior
    p_prior[id].prior = prior;
    p_prior[id].chegada = tempo;
    strncpy(p_prior[id].descricao, desc, MAX_DESCR);

    //determina o id de inserção do processo no vetor p_tempo com base no tempo
    id = busca_binaria_tempo(p_tempo, *sz, tempo);
    for (int i = *sz; i > id; i--) //desloca os elementos da lista a partir da inserção
        p_tempo[i] = p_tempo[i-1];

    //insere as informações do processo na celula na posição correta no array p_prior
    p_tempo[id].prior = prior;
    p_tempo[id].chegada = tempo;
    strncpy(p_tempo[id].descricao, desc, MAX_DESCR);

    (*sz)++; //incrementa o número de processos
}

/*
Executa um processo com base na opção fornecida, em que:
    -p: executa o processo com maior prior
    -t: executa o processo com menor tempo

Parâmetros:
    *p_prior: lista que ordena os processos por prioridade 
    *p_tempo: lista que ordena os processos por tempo 
    *sz: ponteiro para a variável que armazena a quantidade de processos
*/
void exec(celula* p_prior, celula* p_tempo, int* sz) {
    char op; //faz a leitura da opção de execução
    scanf(" %*c%c", &op);

    int id_p, id_t;

    if (op == 'p') { //caso a opção seja com base na prioridade
        id_p = 0; //detemina id do processo no vetor de prioridade
        for (int i = 0; i < *sz; i++) { //encontra o id do processo no vetor de tempo
            if (p_tempo[i].prior == p_prior[0].prior) {
                id_t = i; //detemina id do processo no vetor de tempo
                break;
            }
        }
    } else if (op == 't') { //caso a opção seja com base no tempo
        id_t = 0; //detemina id do processo no vetor de tempo
        for (int i = 0; i < *sz; i++) { //encontra o id do processo no vetor de prioridade
            if (p_prior[i].prior == p_tempo[0].prior) {
                id_p = i; //detemina id do processo no vetor de prioridade
                break;
            }
        }
    }

    //realiza deslocamento dos elementos na lista de prioridade
    for (int i = id_p; i < (*sz) - 1; i++)
        p_prior[i] = p_prior[i+1];

    //realiza deslocamento dos elementos na lista de tempo
    for (int i = id_t; i < (*sz) - 1; i++)
        p_tempo[i] = p_tempo[i+1];

    (*sz)--; //decrementa o número de processos
}

/*
Mostra um processo com base na opção fornecida, em que:
    -p: mostra em uma linha todas as informações do processo com maior prioridade
    -t: mostra em uma linha todas as informações do processo com menor horário

Parâmetros:
    *p_prior: lista que ordena os processos por prioridade 
    *p_tempo: lista que ordena os processos por tempo 
    sz: variável que armazena a quantidade de processos
*/
void next(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %*c%c", &op); //faz a leitura da opção de mostrar o processo
    if (sz == 0) return; //garante que existam processos para serem mostrados
    
    if (op == 'p') //caso a opção seja com base na prioridade
        //printa as informações com base nas especificações do projeto
        printf("%02d %02d:%02d:%02d %s\n", p_prior[0].prior, p_prior[0].chegada.hh,
                p_prior[0].chegada.mm, p_prior[0].chegada.ss, p_prior[0].descricao);

    else if (op == 't') //caso a opção seja com base no tempo
        //printa as informações com base nas especificações do projeto
        printf("%02d %02d:%02d:%02d %s\n", p_tempo[0].prior, p_tempo[0].chegada.hh,
                p_tempo[0].chegada.mm, p_tempo[0].chegada.ss, p_tempo[0].descricao);

    printf("\n");
}


/*
Modifica as informações de um processo com base na opção fornecida, em que:
    -p: altera o campo prior com valor anterior para o valor novo
    -t: altera o campo tempo com valor anterior para o valor novo
Parâmetros:
    *p_prior: lista que ordena os processos por prioridade 
    *p_tempo: lista que ordena os processos por tempo 
    sz: variável que armazena a quantidade de processos
*/
void change(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %*c%c", &op); //faz a leitura da opção de modificação

    if (op == 'p') { //caso a opção seja com base na prioridade
        int prior_from; //prioridade do processo que será alterado (anterior)
        int prior_to; //nova prioridade do processo (novo)
        scanf(" %d|%d", &prior_from, &prior_to); //realiza leitura das informações

        int id; //id da posição do processo no vetor de tempo
        for (int i = 0; i < sz; i++) {
            if (p_tempo[i].prior == prior_from) {
                p_tempo[i].prior = prior_to; //atualiza a prioridade do processo no vetor de prioridade
            }
            if (p_prior[i].prior == prior_from) {
                id = i; //guarda o id do processo no vetor de prioridade
                p_prior[i].prior = prior_to; //atualiza a prioridade do processo no vetor de tempo
            }
        }

        /*caso a nova prioridade seja MAIOR do que o vizinho anterior 
          desloca o processo para o local com a prioridade ordenada na lista de prioridade */
        while (id > 0 && p_prior[id].prior > p_prior[id-1].prior) {
            swap(p_prior + id, p_prior + (id-1)); 
            id--;
        }

        /*caso a nova prioridade seja MENOR do que o vizinho anterior 
          desloca o processo para o local com a prioridade ordenada na lista de prioridade*/
        while (id < sz-1 && p_prior[id].prior < p_prior[id+1].prior) {
            swap(p_prior + id, p_prior + (id+1)); //desloca o 
            id++;
        }
    }
    else if (op == 't') { //caso a opção seja com base no tempo
        int h1, m1, s1, h2, m2, s2;
        scanf(" %d:%d:%d|%d:%d:%d", &h1, &m1, &s1, &h2, &m2, &s2);

        horario hora_from = {h1, m1, s1}; //horario do processo que será alterado
        horario hora_to = {h2, m2, s2}; //novo horário do processo

        int id; //id da posição do processo no vetor de prioridade
        for (int i = 0; i < sz; i++) {
            if (p_tempo[i].chegada.hh == h1 && p_tempo[i].chegada.mm == m1 && p_tempo[i].chegada.ss == s1) {
                p_tempo[i].chegada = hora_to; //atualiza o tempo do processo no vetor de tempo
                id = i; //guarda o id do processo no vetor de prioridade
            }
            if (p_prior[i].chegada.hh == h1 && p_prior[i].chegada.mm == m1 && p_prior[i].chegada.ss == s1) {
                p_prior[i].chegada = hora_to; //atualiza o tempo do processo no vetor de prioridade
            }
        }

        /*caso o novo tempo seja MENOR do que o vizinho anterior 
        desloca o processo para o local com o tempo ordenado no vetor de tempo*/
        while (id > 0 && eh_menor(p_tempo[id].chegada, p_tempo[id-1].chegada)) {
            swap(p_tempo + id, p_tempo + (id-1));
            id--;
        }

        /*caso o novo tempo seja MAIOR do que o vizinho anterior 
        desloca o processo para o local com o tempo ordenado no vetor de tempo*/
        while (id < sz-1 && eh_menor(p_tempo[id+1].chegada, p_tempo[id].chegada)) {
            swap(p_tempo + id, p_tempo + (id+1));
            id++;
        }
    }
}

/*
Imprime todos os processos a serem executados com base na opção fornecida, em que:
    -p: imprime os processos em ordem decrescente de prioridade
    -t: imprime os processos em ordem crescente de horários
Parâmetros:
    *p_prior: lista que ordena os processos por prioridade 
    *p_tempo: lista que ordena os processos por tempo 
    sz: variável que armazena a quantidade de processos
*/
void print(celula* p_prior, celula* p_tempo, int sz) {
    char op;
    scanf(" %*c%c", &op); //lê a operação fornecida
    if (sz == 0) return; //garante que existam elementos nos vetores

    if (op == 'p') { //caso a impressão seja com base na prioridade
        for (int i = 0; i < sz; i++) { //imprime todas as informações de cada processo em cada linha
            printf("%02d %02d:%02d:%02d %s\n", p_prior[i].prior, p_prior[i].chegada.hh,
                    p_prior[i].chegada.mm, p_prior[i].chegada.ss, p_prior[i].descricao);
        }
    } else if (op == 't') { //caso a impressão seja com base no tempo
        for (int i = 0; i < sz; i++) { //imprime todas as informações de cada processo em cada linha
            printf("%02d %02d:%02d:%02d %s\n", p_tempo[i].prior, p_tempo[i].chegada.hh,
                    p_tempo[i].chegada.mm, p_tempo[i].chegada.ss, p_tempo[i].descricao);
        }
    }
    printf("\n");
}

int main() {
    char comando[10];

    celula p_prior[150], p_tempo[150];
    int sz = 0;

    //determina qual operação será realizada com base na primeira palavra de cada linha da entrada 
    while (scanf(" %s", comando) && strcmp(comando, "quit")) { //termina a execução com a entrada "quit"
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