#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif

#define STACKSIZE 32768		/* tamanho de pilha das threads */
#define _XOPEN_SOURCE 600	/* para compilar no MacOS */


/*

 Contexto de um processo: conteudo de registradores
 						  - espaco de enderecamento do usuario: textos, dados, pilha do usuario, memoria partilhada
 						  - informacoes de controle, variaveis de ambiente 

 Troca de contexto: salva o conteudo dos registradores da CPU e os carrega com os valores referente ao do processo que esta ganhando
                    a utilizacao do processador
                    
 					quando um processador muda de um processo a outro o seu estado (o processador registra e associa dados)
                    deve ser salvo, pois algum tempo depois, sera reiniciado o processo e continuara como se nunca fora interrompido.
                    Uma vez esse estado tenha sido salvo, o proximo processo em espera e ativado. Isso envolve carga nos registros dos 						
					registradores do processador e na memoria, com todos os dados previamente salvos e iniciando na instrucao que
                    seria executada quando houve a ultima interrupcao. 
                    
                    time slice (periodo de quantum): periodo de tempo que um processo executa antes de ser trocado.         
        
 */


ucontext_t ContextPing, ContextPong, ContextMain; 
/* Variaveis do tipo ucontext_t: armazenam contextos de execucao*/

/*****************************************************/

void BodyPing (void * arg)
{
   int i ;

   printf ("%s iniciada\n", (char *) arg) ;
   
   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", (char *) arg, i) ;
      swapcontext (&ContextPing, &ContextPong);
      /* salva o contexto atual em &ContextPing e restaura o contexto salvo anteriormente em  &ContextPong 
         => Salva o contexto atual de Ping e seta o contexto atual como o contexto Pong
         => Volta em main continuando onde parou, ou seja em "getcontext (&ContextPong);"
      */
                                                                                                                                                                                                                                                                                                                                                                                

  																																																																																												
   printf ("%s FIM\n", (char *) arg) ;

   swapcontext (&ContextPing, &ContextMain) ; 
   /* salva o contexto atual em &ContextPing e restaura o contexto salvo anteriormente em  &ContextMain 
      => Salva o contexto atual de Ping e seta o contexto atual como o contexto Main 
   */
}

/*****************************************************/

void BodyPong (void * arg)
{
   int i ;

   printf ("%s iniciada\n", (char *) arg) ;

   for (i=0; i<4; i++)
   {
      printf ("%s %d\n", (char *) arg, i) ;
      swapcontext (&ContextPong, &ContextPing);
      /* salva o contexto atual em &ContextPong e restaura o contexto salvo anteriormente em &ContextPing 
         => Salva o contexto atual de Pong e seta o contexto atual como o contexto Ping
         => Volta em main continuando onde parou, ou seja em "BodyPing"
      */
   }
   printf ("%s FIM\n", (char *) arg) ;

   swapcontext (&ContextPong, &ContextMain) ; 
   /* salva o contexto atual em &ContextPong e restaura o contexto salvo anteriormente em &ContextMain */
   
}

/*****************************************************/

int main (int argc, char *argv[])
{
   char *stack ;

   printf ("Main INICIO\n");

   getcontext (&ContextPing); 
   /* salva o contexto atual em &ContextPing => inicializa a estrutura do tipo ucontext_t para o contexto atual*/

   stack = malloc (STACKSIZE); /* Aloca memoria para o tamanho da pilha */
   if (stack) /* cria a pilha alocada */
   {
      ContextPing.uc_stack.ss_sp = stack;        /* seta a pilha apontando para o comeco da area de memoria que foi alocada*/
      ContextPing.uc_stack.ss_size = STACKSIZE;  /* ss_size => onde foi alocado regiao de memoria */ 
      ContextPing.uc_stack.ss_flags = 0;         /* estado atual do sinal da pilha */
      ContextPing.uc_link = 0;                   /* faz o que, Anolan?  aponta para o contexto que vai ser executado */
   }
   else
   {
      perror ("Erro na criação da pilha: ");
      exit (1);
   }

   makecontext (&ContextPing, (void*)(*BodyPing), 1, "    Ping"); 
   /* MAKECONTEXT: altera um contexto previamente salvo fazendo com que ele chame uma funcao especifica quando esse contexto 
                   for chamado ao inves de saltar para o ponto onde o contexto foi originalmente salvo.
      => ajusta alguns valores internos do contexto salvo em &ContextPing. 
      => modifica o contexto em ping, inicializado pelo getcontext 
      => a execucao continuara em BodyPing ao usar setcontext ou swapcontext 
   */

   getcontext (&ContextPong); /* salva o contexto atual em &ContextPong */

   stack = malloc (STACKSIZE);
   if (stack)
   {
      ContextPong.uc_stack.ss_sp = stack;
      ContextPong.uc_stack.ss_size = STACKSIZE;
      ContextPong.uc_stack.ss_flags = 0;
      ContextPong.uc_link = 0;
   }
   else
   {
      perror ("Erro na criação da pilha: ");
      exit (1);
   }

   makecontext (&ContextPong, (void*)(*BodyPong), 1, "        Pong"); 
   /* ajusta alguns valores internos do contexto salvo em &ContextPong 
      => modifica o contexto em pong, inicializado pelo getcontext 
      => a execucao continuara em BodyPong ao usar setcontext ou swapcontext   
   */

   swapcontext (&ContextMain, &ContextPing); //Vai para BodyPing
   /* salva o contexto atual em &ContextMain e restaura o contexto salvo anteriormente em &ContextPing  
      => salva o contexto atual de main e vai para Ping */
   swapcontext (&ContextMain, &ContextPong); //Vai para BodyPong
   /* salva o contexto atual em &ContextMain e restaura o contexto salvo anteriormente em &ContextPong  
      => salva o contexto atual de main e vai para Pong */

   printf ("Main FIM\n");

   exit (0);
}


