/*  CENTRO FEDERAL DE EDUCACAO TECNOLOGICA DE MINAS GERAIS		*
*   LABORATORIO DE SISTEMAS OPERACIONAIS						*
*	DANIELE DE SOUZA LIMA										*
*																*
*	ALGORITMO DE ESCALONAMENTO RR (ROUND-ROBIN)					*
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
    if (proc == NULL) {
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

/* Escalonamento Round-Robin */
void rr (struct process *proc, int quantum){
	int jobsremain, passes;
	struct process *copia, *tmpsrc, *tmp, *slot;
	printf("\tEscalonamento Round-Robin - Quantum: %d)\n", quantum);
	printf("\n");
	tmpsrc = proc;
	copia = tmp = NULL;
	while (tmpsrc != NULL){     printf("oioioi");
		if (copia == NULL){ /*inicializacao*/
			copia = init_process(tmpsrc->id, tmpsrc->turnaroundTime, 
								   tmpsrc->priority);
			tmp = copia;
		} 
		else{
			tmp->prox = init_process(tmpsrc->id, tmpsrc->turnaroundTime, 
                                       tmpsrc->priority);
			tmp = tmp->prox;
		}
		tmpsrc = tmpsrc->prox;
	};
	/* ANALISE DE PRIORIDADE */
	jobsremain = 2;
	slot = NULL;
	while (jobsremain){
		jobsremain = 0;
		/* SELECIONA O PROXIMO PROCESSO E FAZ A ALOCACAO */
		if (slot == NULL){
			slot = copia;
			jobsremain = 2;
		} 
		else{
			passes = 0;
			do{
				if (slot->prox == NULL){
					passes++;
					slot = copia;
				} 
				else
					slot = slot->prox;
			} 
			while (passes <= 3 && slot->turnaroundTime == slot->execution);
			if (passes <= 3) 
				jobsremain = 2;	
		}
		/* EXECUTA UM CICLO */
		tmp = copia;
		while (tmp != NULL){
			if (tmp->turnaroundTime > tmp->execution){
				if (tmp == slot) 
					tmp->execution += quantum; 
				else 
					tmp->wait += quantum;
			}
			tmp = tmp->prox;
		}
	}
	/* Exibe os resultados e elimina as replicações */
	tmp = copia;
	while (tmp != NULL){
		printf("Processo: %d\tSurto: %d\tEspera: %d\tRetorno: %d\n", tmp->id,
				tmp->turnaroundTime, tmp->wait, tmp->execution + tmp->wait);
		tmpsrc = tmp;
		tmp = tmp->prox;
		free(tmpsrc);
	}
	printf("\n");
}



int main (void){
    
    struct process *list, *aux;
    list       = init_process(1, 5, 0);
    list->prox = init_process(2, 2, 0); aux = list->prox;
    aux->prox  = init_process(3, 4, 1); aux = aux->prox;
    aux->prox  = init_process(4, 3, 3); 
       
    listproc(list);
    rr(list,0);   
    while (list != NULL){
        aux  = list;
        list = list->prox;
        free(aux);
    };
    return(0);
};


