/* din_philo.c */    
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>


#define PHILOS 5
#define Sleep_Time 3

void *philosopher (void *id);
int food_on_table (), numRefeicoes = 0;

//Mutexes
pthread_mutex_t chopstick[PHILOS], mut, saleiro;
pthread_mutex_t food_lock;

//Threads
pthread_t philo[PHILOS];



int main (int argn, char **argv)
{
	long i;

	//Inicia mutexes
	pthread_mutex_init (&saleiro, NULL);
	pthread_mutex_init (&mut, NULL);
	pthread_mutex_init (&food_lock, NULL);
	for (i = 0; i < PHILOS; i++)
		pthread_mutex_init (&chopstick[i], NULL);
	//Cria threads
	for (i = 0; i < PHILOS; i++)
		pthread_create (&philo[i], NULL, philosopher, (void *)i);

	while(1){
		sleep (1) ;
		pthread_mutex_lock (&mut) ;
		printf ("Refeições por segundo: %d\n", numRefeicoes) ;
		numRefeicoes = 0 ;
		pthread_mutex_unlock (&mut) ;
	}
	return 0;
}

void * philosopher (void *num){
	long id;
	int i, left_chopstick, right_chopstick;
	id = (long)num;
	left_chopstick = id;
	right_chopstick = (id + 1)%PHILOS;

	while (1) {

		// meditar (duração aleatória entre 0 e 2 segundos)
		int pause = rand() % Sleep_Time;
//		sleep (pause);
		
		//pega saleiro
		pthread_mutex_lock (&saleiro);
//		printf("%ld pegou o saleiro\n",id);

		// pegar o palito à sua esquerda
		pthread_mutex_lock (&chopstick[left_chopstick]);
//		printf("%ld pegou o palito da esquerda\n",id);

		// pegar o palito à sua direita
		pthread_mutex_lock (&chopstick[right_chopstick]);
//		printf("%ld pegou o palito da direita\n",id);

		// comer (duração aleatória entre 0 e 2 segundos)
//		printf("%ld comecou de comer\n",id);
		numRefeicoes++;
		pause = rand() % Sleep_Time;
//		sleep (pause);
//		printf("%ld terminou de comer\n",id);

		// soltar os palitos
		pthread_mutex_unlock (&chopstick[left_chopstick]);
		pthread_mutex_unlock (&chopstick[right_chopstick]);
		pthread_mutex_unlock (&saleiro);

	}
	return (NULL);
}




