/*  CENTRO FEDERAL DE EDUCACAO TECNOLOGICA DE MINAS GERAIS		*
*   LABORATORIO DE SISTEMAS OPERACIONAIS						*
*	DANIELE DE SOUZA LIMA										*
*																*
*	ALGORITMO DE ESCALONAMENTO SJF (Shortest Job First = 		*
*   MENOR TAREFA PRIMEIRO)										*
*   ATRIBUIR O PROCESSADOR A MENOR (MAIS CURTA) TAREFA DA FILA  *
*   DE TAREFAS PRONTAS.											*
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


/* ESCALONAMENTO SJF*/
void sjf (struct process *proc){
	int duration, wait, shortest;
	struct process *copy, *tmpsrc, *tmp, *beforeshortest;
	printf("\tEscalonamento SJF\n");
	printf("\n");
	/* REPLICAR A LISTA DE PROCESSOS */
	tmpsrc = proc;
	copy = tmp = NULL;
	while (tmpsrc != NULL){
		if (copy == NULL) {
			copy = init_process(tmpsrc->id, tmpsrc->turnaroundTime, 
								   tmpsrc->priority);
			tmp = copy;
		} 
		else{
			tmp->prox = init_process(tmpsrc->id, tmpsrc->turnaroundTime, 
									 tmpsrc->priority);
			tmp = tmp->prox;
		}
		tmpsrc = tmpsrc->prox;
	}
	duration = 0;
	while (copy != NULL){ /* ENCONTRA O PROXIMO PROCESSO*/
		beforeshortest = NULL;
		shortest = copy->turnaroundTime;
		tmp = copy->prox;
		tmpsrc = copy;
		while (tmp != NULL){
			if (tmp->turnaroundTime < shortest){
	  			shortest = tmp->turnaroundTime;
	  			beforeshortest = tmpsrc;
			}
			tmpsrc = tmp;
			tmp = tmp->prox;
		}
		/* EXECUTA O PROCESSO E DELETA A COPIA DA LISTA DE PROCESSOS */
		if (beforeshortest == NULL){
			/* ALOCA O PRIMEIRO PROCESSO, CASO ESSE SEJA MENOR */
			wait      = duration;
			duration += copy->turnaroundTime;
			printf("Processo: %d\tEspera: %d\tDuracao: %d\n", 
                    copy->id, wait, duration);
			tmpsrc = copy;
			copy   = copy->prox;
	        free(tmpsrc);
		} 
		else{
		/* ALOCA O PRIMEIRO PROCESSO, CASO NAO HAJA OCORRENCIA DE OUTRO MENOR */
			tmp       = beforeshortest->prox;
			wait      = duration;
			duration += tmp->turnaroundTime;
			printf("Processo: %d\tEspera: %d\tDuracao: %d\n", 
					tmp->id, wait, duration);
			beforeshortest->prox = tmp->prox;
			free(tmp);
		}
	}
	printf("\n");
	 //system ("pause");   
}


int main (void){
    
    struct process *list, *aux;
    list       = init_process(1, 5, 0);
    list->prox = init_process(2, 2, 0); aux = list->prox;
    aux->prox  = init_process(3, 4, 1); aux = aux->prox;
    aux->prox  = init_process(4, 3, 3); 
       
    listproc(list);
    sjf(list);   
    while (list != NULL){
        aux  = list;
        list = list->prox;
        free(aux);
    };
    return(0);
};


