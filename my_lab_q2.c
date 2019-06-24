////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//				ORT BRAUDE COLLAGE	
//		
//				OPERATING SYSTEMS
//				
//				LAB 10 QUESTION NUMBER 2:
// 
//				WRITTIEN BY :	"	REHAM ABASS	" .
//
//				DATE: 17.06.2019
//
//		THE PHILOSOPHERS PROBLEM : 
//			N PHILOSOPHERS SEAT ON ROUND TABLE , THEY CAN THINK,BE HUNGRY, EAT.
//  		THE ONLY CONDITION : EACH TWO NEIGHBORS CAN'T EAT IN THE SAME TIME BEACUSE THERE IS ONE STICK BETWEEN EACH TWO OF THEM.
//			SO HOW AWE ARE GOING TO ORDER THE EATING PROBLEM BETWEEN THEM, WHILE PASSING EACH ONE IN THE THREE STATES(THINK,EAT,BE HUNGRY)? 
//			BY USING SEMAPHORES ,DEADLOCKS, MUTEX,THREADS.
//		< LIBRARIES.h > :							
//
#include<stdio.h>			// FOR INPUT OUTPUT 
#include<stdlib.h>			//FOR ALLOCATION WILL BE MADE AFTER KNOWING N - NUMBER OF PHILOSOPHERS
#include<pthread.h>			//EACH PHILOSOPHER GONNA HAD IT'S THREAD
#include<semaphore.h>		//SEMAPHORES FOR EACH PHILOSOPHER & MUTEX FOR CRITICAL BLOCK 
#include<unistd.h>			//IT DEFINES SYMBOLIC CONSTANS 
#include<assert.h>			//DEFINIG MACROS IN THIS LAB.
#include<time.h>			// RANDOM TIMER FOR USLEEP() 
#include<string.h>			//SCANF STRING , USING ATOI() , 

//		CONSTANT VARIABLES:
						
#define LEFT (i-1)% N		//MACRO CALCULATES INDEX OF LEFT NEIGHBOR ,BECAUSE THEY SET ON ROUND TABLE
#define RIGHT (i+1+N)% N	//MACRO CALCULATES INDEX OF RIGHT NEIGHBOR ,BECAUSE THEY SET ON ROUND TABLE
#define MAX_TIME 5			// FOR RANDOM TIME FOR USLEEP() BETWWEN 0 AND 4

// 		STATES OF PHILOSOPHERS:
#define THINKING 0			//THINKING STATE OF PHILOSOPHER =	0
#define HUNGRY   1			//HUNGRY STATE OF PHILOSOPHER =		1
#define EATING   2			//EATING STATE OF PHILOSOPHER =		2


//GLOBAL VARIABLES:
// WHY TO USE GLOBAL VARIABLES? BECAUSE THEY WILL BE USED IN MANY THREADS IN THE SAME TIME.

int N ;							//NUMBER OF PHILOSOPHERS ,GETTING IT IN RUNNING TIME 
sem_t* mutex = NULL;			// POINTER TO SEMAPHORE ON CRITICAL CODE BLOCK,GONNA BE BINARY COUNTER (0 OR 1 ONLY)
sem_t* eater_semaphores = NULL;//WILL BE ALLOCATED AFTER KNOWING NUMBER OF PHILOSOPHERS ON RUNNING TIME
int *state = NULL;				//ARRAY OF SEMAPHORES, EACH PHILOSOPHER HAS STATE : THINKING OR HUNGRY OR EATING
int *array = NULL;
int count1,countN ;
int *countTH;					//counts number of times we got "thinking" for the same philosoph ,so we don't print the same message on screen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION 1: 
void eat(int i) {									//JUST PRINT AND WAIT 
        int time = rand() % MAX_TIME;				//RANDOM TIME BETWEEN 0..4		
        printf("Philosopher #%d is eating...\n", i + 1);
		
		if(count1 == 1 && countN == 1){
			printf("\n\n 1 and %d are eating in the same time!\n", N);
			}
        sleep(time);								//LOOKS LIKE REALITY, HE TAKES TIME TO EAT.
        printf("Philosopher #%d stopped eating...\n", i + 1);	
	countTH[i]=0;
}
//////////////////////////////////////////////////////////////////////////////
//FUNCTION 2:
void think(int i) {									//JUST PRINT AND WAIT

		countTH[i]++;
        int time = rand() % MAX_TIME;				//RANDOM TIME BETWEEN 0..4
        printf("Philosopher #%d is thinking...\n", i + 1);
        sleep(time);								//THINKING TAKES SOME SECONDS,WHILE OTHERS EATING
}

/////////////////////////////////////////////////////////////////////////////////
//  FUNCTION 3: 
// CHECK IF PHILOSOPHER IS HUNGRY :
void test(int i) { 
	
	//CHECK IF 	" STICKERS ARE FREE "	 SO HE CAN USE THEM TO EAT 
	if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING ){//IF HIS NEIGHBERS STATES (LEFT AND RIGHT) AREN'T EATING!
		state[i]=EATING;    					//UPDATING HIS STATE TO EATING

		sem_post(&eater_semaphores[i]);			//NOW OTHES CAN EAT						
	}					
}
////////////////////////////////////////////////////////////////////////////////

//FUNCTION 4:
//	PHILOSOPH TRIES TO TAKE THE STICKS BESIDE HIM 
void take_sticks(int i) {	
	//LOCK CRITICAL CODE BY MUTEX,SO THE SAME STICKS(AT THE SAME TIME) CAN'T BE TAKEN BY MORE THAN ONE PHILOSOPHER 
	sem_wait(mutex);						//actually it's a pointer to semaphore
	state[i]=HUNGRY;
	printf("Philosopher #%d is thinking...\n", i + 1);		
	test(i);								//TRIES TO TAKE TWO STICKS BY  TEST-FUNCTION
	sem_post(mutex);						//UNLOCK CRITICAL CODE
	sem_wait(&eater_semaphores[i]);	
											//IF CAN'T EAT, SO LOCK his sema. NEIGHBERS CAN UNLOCK HIM AFTER THEY EAT
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION 5: 
//	PHILOSOPHER RETURNS THE STICKS HE HAD USE
void put_sticks(int i) {
	sem_wait(mutex);						// INSIDE CRITICAL BLOCK 
	state[i]=THINKING;						// PHILOSOPHER END TO "EAT" BEGIN TO THINK.
	test(RIGHT);							//CHECK IF HIS 	LEFT	 IS "HUNGER" : MEANS HE IS WAITING TO StICKS , LET HIM TRY BEGIN TO EAT
	test(LEFT);								//CHECK IF HIE 	RIGHT 	IS "HUNGER" : MEANS HE IS WAITING TO StICKS , LET HIM TRY BEGIN TO Eat
	sem_post(mutex);
}
/////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION 6: 
//	THIS FUNCTION OPERATES BY ALL THE THREADS WHICH ARE DEALLING WITH PHILOSOPHERS
void* philosopher(void* arg) {
        int i = *((int*)arg);				//CONVERTING INDEX OF PHILOSOPHER
	

		while (1 == 1) {					// INFINTE LOOP
					
					if(countTH[i] == 0)
						think(i);						// FIRST OF ALL: PHILOSOPHER MUST "	THINK	"
					
						
					if(i == 0 ) 
						count1++;
					if(i == (N-1))
						countN++;
					if(count1 == 1 && countN == 1){

						}
					else{

					take_sticks(i);					//PHILOS' TRIES TO TAKE THE STICKS
					eat(i);							//EAT	
					put_sticks(i);					//PUT STICKERS

					}
					if(i == 0 ) count1--;
					if(i == (N-1))countN--;
		}

}
	
/////////////////////////////////////////////////////////////////////////////////
											//PRIMARY FUNCTION  :  MAIN :
int main(int argc , char* argv[]){ 			// argv[1] = MUST BE NUMBER OF PHILOSOPHERS
											//FIRST OF ALL  - WE NEED NUMBER OF PHILOSOPHERS
		if(argc != 2 ){ fprintf(stderr, "Usage: %s <NUMBER_OF_PHILOSOPHERS>\n",argv[0]); return 1;}
	
		N = atoi(argv[1]);					// N IS A GLOBAL INT VARIABLE

        if (N <= 1){fprintf(stderr,"Error by transformation of the argument...\n"); return 2;}//?? ???? ?-2 ????????? 		
		//void* result;
   		int i;

		count1=0;
		countN=0; 


	//ALLOCATIONS AND INIATIONS:

        mutex = (sem_t*)malloc(sizeof(sem_t));// MUTEX IS A POINTER TO A SEMAPHORE -WHICH WILL MASTER THE CRITICAL CODE BLOCK
        if (sem_init(mutex, 0, 1) != 0) { 	//INIATE MUTEX TO 1 , SO HE IS RELEASED
                fprintf(stderr, "Error by creating semaphore...\n");return 3;}
 
	// ALLOCATE MEMORY FOR ARRAY OF SEMAPHORES FOR EACH PHILOSOPHER
		eater_semaphores=(sem_t*)malloc( N * sizeof(sem_t) );	//KNOW "N" IN RUNNING TIME SO ARRAY MUST BE ALLOCATED
		if( eater_semaphores == NULL ){printf("\nCan't allocate eater_semaphores array of semaphores!");exit(-1);}

	//ALLOCATE ARRAY OF INTEGERS FOR THE STATES OF ALL OF THE PHILOSOPHERS - STATE CAN BE{ 0 = "THINKING" / 1 = "HUNGRY" / 2=  "EATING" }
    	state = (int*)malloc(N * sizeof(int));
		if(state == NULL){printf("\nCan't allocate array of states");exit(-1);}

	// AFTER ALLOCATING THE ARRAY OF STATES , MUST BE INIATED TO ALL PHILOSOFERS:
        memset(state, 0, N);				//INIATING STATE OF ALL OF PHILOSOPHERS =THINKING = 0 
        srand(time(NULL));					// RANDOM A TIME
	//ALLOCATE ARRAY OF THREADS  FOR THE USE OF PHILOSOPHERS, EACH THREAD FOR EACH PHILOSOPHER:
        pthread_t *philosophers = (pthread_t*)malloc(N * sizeof(pthread_t)); 
 		if(philosophers == NULL){
			printf("\nCan't allocate array of threads\n"); exit(-1);}
	
	//INIATE ALL THE SEMAPHORES OF ALL OF THE PHILOSOPHERS
        for (i = 0; i < N; i++) {
                if (sem_init(&eater_semaphores[i], 0, 0) != 0){fprintf(stderr, "Error by creating semaphore...\n");return 3;}
			}	


//allocate and iniate  array of counters:

	       int *countTH= (int*)malloc(N * sizeof(int)); 
 		if(countTH == NULL){
			printf("\nCan't allocate array of counts\n"); exit(-1);}
        for (i = 0; i < N; i++) {
                countTH[i]=0;
			}	



	

	//ALLOCATE ARRAY OF INTEGERS FOR THE threads and iniate it :

    	array = (int*)malloc(N * sizeof(int));
		if(array == NULL){printf("\nCan't allocate array");exit(-1);}
		 for (i = 0; i < N; i++) {
				array[i]=i;
			}

	//CREAT THREADS FOR EACH PHILOSOPHER AND RUN IT'S FUNCTION BY THIS THREAD
        for (i = 0; i < N; i++) {
                if (pthread_create(&philosophers[i], NULL, philosopher, (void*)&array[i]) != 0) {fprintf(stderr, "Error by creating thread\n");return 2;}
       	   usleep(100000);  				// WAIT 100000 MILI SICONDS BETWEEN EACH THREAD CREATING
       }									// IN USLEEP(10^6) MEANS 1 SECOND

     //WAITING FOR ALL THE THREADS TO FINISH THEIR WORKS :
   		for (i = 0; i < N; i++) {				//&result inplace of NULL
    	      if (pthread_join(philosophers[i], NULL) != 0) { fprintf(stderr, "Error by joining thread\n");  return 3;}
			}

return 0;
}
