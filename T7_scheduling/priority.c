/*  CENTRO FEDERAL DE EDUCACAO TECNOLOGICA DE MINAS GERAIS		*
*   LABORATORIO DE SISTEMAS OPERACIONAIS						*
*	DANIELE DE SOUZA LIMA										*
*																*
*	ALGORITMO DE ESCALONAMENTO PRIORIDADE:						*
*   O PROCESSO DE MENOR VALOR DE PRIORIDADE OBTEM PRIORIDADE    * 
*   MAIOR NA FILA DE PROCESSOS									*
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
    proc->id             = id;
    proc->turnaroundTime = turnaroundTime;
    proc->priority       = priority;
    proc->execution      = 0;
    proc->wait           = 0;
    proc->prox           = NULL;
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


void priority (struct process *proc) {
	int duration, wait, maior;
	struct process *copy, *tmpsrc, *tmp, *maiorprimeiro;
	printf("\tEscalonamento por Prioridade\n");
	printf("\n");

    /* DUPLICAR A LISTA DE PROCESSOS*/
  	tmpsrc = proc;
  	copy   = tmp = NULL;
    while (tmpsrc != NULL){
    	if (copy == NULL){
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
  	/* FUNDAMENTOS PRINCIPAIS */
  	duration = 0;
  	while (copy != NULL){
    	/* LOCALIZAR O PROXIMO PROCESSO */
    	maiorprimeiro = NULL;
    	maior		  = copy->priority;
    	tmp 		  = copy->prox;
    	tmpsrc        = copy;
    	while (tmp != NULL){
    		if (tmp->priority < maior){
      			maior         = tmp->priority;
      			maiorprimeiro = tmpsrc;
            }
    		tmpsrc = tmp;
    		tmp = tmp->prox;
    	}
     	if (maiorprimeiro == NULL) {
    	/* VERIFICA SE O PRIMEIRO PROCESSO POSSUI MAIOR PRIORIDADE */
    	wait = duration;
    	duration += copy->turnaroundTime;
        printf("Processo: %d\tEspera: %d\tDuracao: %d\n", copy->id, wait, 
				duration);
    	tmpsrc = copy->prox;
    	free(copy);
    	copy = tmpsrc;
    	} 
		else{
    		/*CASO O PRIMEIRO NAO POSSUA MAIOR PRIORIDADE */
    		tmp    = maiorprimeiro->prox;
    		wait   = duration;
   		    duration += tmp->turnaroundTime;
    		printf("Processo: %d\tEspera: %d\tDuracao: %d\n", tmp->id, wait,
		            duration);
    		maiorprimeiro->prox = tmp->prox;
    		free(tmp);
    	}
	}
  	printf("\n\n");
}

int main (void){
    
    struct process *list, *aux;
    list       = init_process(1, 5, 2);
    list->prox = init_process(2, 2, 3); aux = list->prox;
    aux->prox  = init_process(3, 4, 1); aux = aux->prox;
    aux->prox  = init_process(4, 3, 4); 
       
    listproc(list);
    priority(list);   
    while (list != NULL){
        aux  = list;
        list = list->prox;
        free(aux);
    };
    return(0);
};


