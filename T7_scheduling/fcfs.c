/*  CENTRO FEDERAL DE EDUCACAO TECNOLOGICA DE MINAS GERAIS		*
*   LABORATORIO DE SISTEMAS OPERACIONAIS						*
*	DANIELE DE SOUZA LIMA										*
*																*
*	ALGORITMO DE ESCALONAMENTO FCFS (FIRST-COME, FIRST SERVED)	*
*	ATENDER AS TAREFAS EM SEQUENCIA, A MEDIDA EM QUE ELAS SE	* 
*	TORNAM PRONTAS (OU SEJA, CONFORME SUA ORDEM DE CHEGADA NA 	*
*   FILA DE TAREFAS PRONTAS)									*
*/						

#include <stdio.h>
#include <stdlib.h>

/* ESTRUTURA DE UM PROCESSO */
struct process{
    int id;                   /* IDENTIFICADOR DO PROCESSO */              
    int turnaroundTime;       /* SURTO - TEMPO DE DURACAO DO PROCESSO */
    int priority;             /* PRIORIDADE DO PROCESSO */
    int execution;            /* TEMPO DE EXECUCAO DO PROCESSO */
    int wait;                 /* TEMPO DE ESPERA DO PROCESSO */
    struct process *prox;
};

/* INICIALIZACAO DE ENTRADA DA LISTA DE PROCESSOS */
struct process *init_process (int id, int turnaroundTime, int priority){
    struct process *proc;
    proc = (struct process*)malloc(sizeof(struct process)); 
    if (proc == NULL){
        printf("Erro: Falha de Alocacao de memoria.\n");
        exit(1);
    };
    proc->id = id;
    proc->turnaroundTime = turnaroundTime;
    proc->priority = priority;
    proc->execution = 0;
    proc->wait = 0;
    proc->prox = NULL;
    return(proc);
};

/* LISTAR PROCESSOS */
void listproc(struct process *proc){
  struct process *tmp = proc;
  printf("\tListagem de Processos\n");
  printf("\n");
  while (tmp != NULL){
	printf("Tarefa: %d\tPrioridade: %d\tDuracao: %d\n", tmp->id, tmp->priority, 
            tmp->turnaroundTime);
	tmp = tmp->prox;
  };
  printf("\n\n");
};


/* ESCALONAMENTO FCFS - O PRIMEIRO QUE CHEGA E O PRIMEIRO A SAIR */
void fcfs (struct process *proc){
    int duration = 0, wait;
    struct process *tmp = proc;
    printf("\tEscalonamento FCFS\n");
    printf("\n");
    while (tmp != NULL) { /* CONSIDERANDO TODOS OS PROCESSOS */
		wait = duration;
		duration += tmp->turnaroundTime;
		printf("Tarefa: %d\tEspera: %d\tDuracao: %d\n", tmp->id, 
                wait, duration);
		tmp = tmp->prox;
    };
    printf("\n\n");
};


int main (void){
    
    struct process *list, *aux;
    list       = init_process(1, 5, 0);
    list->prox = init_process(2, 2, 0); aux = list->prox;
    aux->prox  = init_process(3, 4, 1); aux = aux->prox;
    aux->prox  = init_process(4, 3, 3); 
       
    listproc(list);
    fcfs(list);    
    while (list != NULL){
        aux  = list;
        list = list->prox;
        free(aux);
    };
    return(0);
};





