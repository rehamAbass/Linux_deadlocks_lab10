//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////		ORT BRAUDE COLLAGE
//					LAB 10 QUESTION NUMBER 1:
//
//					WRITTEN BY: "	REHAM ABASS	"
//
//					DATE: 	17.06.2019
//
//					SEMAPHORES AND DEAD LOCKES:
//
//
//

// LIBRARIES:
#include<stdio.h>				// for input & Output
#include<stdlib.h>				//for allocations
#include<semaphore.h>			//for using semaphores , deadlockes ,mutex, and their functions.
#include<conio.h>				//also for input/ outputs
#include<time.h>				//for random numbers and sleep
#include<pthread.h>				//for creating threads anf their functions..
#include<string.h>				//for using strings and scanf..


//DEFINITIONS :
#define KARMAIL 0
#define TBERIA	1
#define HAIFA	2
#define TSFAT	3

//Global variables:
sem_t * mutex = NULL ;		//Binary semaphore ,1 means we are dealling with high priority cities(TSFAT && HAIFA) THEY ARE IN MEETING NOW CAN'T DISTURB THEM
int *array_i = NULL ;
///////////////////////////////////////////////////////////////////

void Calls(int City1 , int City2){
	sem_wait(mutex);	// if mutex >0 mutex-- and go inside , else don't !! (else means mutex=0)

	
	printf("%d started to talk with %d\n",City1,City2);
	sleep();
	printf("%d finished to talk with%d\n",City1,City2);
	

}
///////////////////////////////////////////////////////////////////
void * Func( void * index){
		//each thread call the function "	Func" randomaly take City number
		// this function gonna call the function "	Calls"
				
		
	int i=*(int*)index;		//converting the index of the city
	int a;

	do{
		a = rand()%4;		// 0..3
		}while(a == i);		// City can't Call itself ! try another number.

	if(	( (a ==  HAIFA) && (i == TSFAT ) )	||	 ( (a ==  TSFAT)&& (i == HAIFA) )	){	//lock the semaphores of low priority !! 
		sem_wait(mutex);

		}
	Calls(a);				// City i Calls City a.
	
//NOTES:
		// 1. oNE CITY CAN TALK JUST WITH ONE OTHER cITY
		// 2. ALL THE cITIES MUST BE IN CONTACT, AT LEAST WITH ONE ANOTHER CITY , AND , AT MAXIMUM ONE OTHER cITY
		// 3. THAT INCLUDES CALL OR BEING CALLED
		// 4. CONVERSATIONS OF THE CITIES MUST BE IN THE SAME TIME IN THREADS [	|| IN PARALLEL ||	]
		// 5. IN  "TSFAT"  AND " HAIFA " HAVE HEIGHR PRIORITIES THAN THE OTHERS. 
//	********  THATS MEANS: THE OTHRS CAN'T CALL EACHOTHER / OR THEM  WHEN THEY ARE IN MEETING TOGETHER !!!!
//	********  IF tBERIA AND KARMAIL ARE IN MEETING , THE OTHERS WHICH ARE IN HEIGH PRIORIY CAN BE GO TO MEET IN THE SAME TIME


}			
//////////////////////////////////////////////////////////////////
int main( int argc , char*argv[]){
			// main must creat 4 threads , each one for each city
	int i;

	array_i	= (int *)malloc(4*sizeof(int));
	if(array_i == NULL){printf("\nCan't allocate array of indexes\n");exit(-1);}
	 
	for( i=0 ; i< 4 ; i++){		// because of the threads , using of &i won't be correct , i change all the time
		array_i[i]=i;			//so we prefer to use pointer to index, each index has another pointer
		}

	//ALLOCATE  and iniate SEMAPHORES:
	mutex = (sem_t *)malloc(sizeof(sem_t));	
	if(mutex == NULL){printf("\nCan't creat mutex semaphore!"); exit(-1);}
	
	sem_init(mutex , 0, 0);		// iniating the semaphore with 0 which means: we don't know if there a meeting between HAIFA && TSFAT

	//Allocating  and creating threads:
	pthread_t *array_threads = (pthread_t *)malloc(4*sizeof(pthread_t));
	if(array_threads == NULL){printf("\nCan't allocate threads\n");exit(-1);}
	for( i=0 ; i< 4 ; i++){
		pthread_creat( &array_threads[i] ,NULL ,Func ,(void*)&array_i[i] );
		}

	for( i=0 ; i< 4 ; i++){		// waiting for all threads to finish 
		pthread_join(&array_threads[i] ,NULL);
		}
	sleep(20);	
	free(array_threads);		//must free all the allocated memoray
	free(array_i);				//must free all the allocated memoray
	return 0;
}
/////////////////////////////////////////////////////////////////////
