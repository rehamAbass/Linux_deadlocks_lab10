//////////////////////	LAB 10 Q2 : 

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>


#define LEFT (i + N - 1) % N	//מקרו המגדיר את מספר השכן משמאל
#define RIGHT (i + 1) % N		//מקרו המגדיר את מספר השכן מימין
#define THINKING 0				//פילוסוף חושב
#define HUNGRY 1				//(פילוסוף רעב (בהמתנה למקלות
#define EATING 2				//פילוסוף אוכל
#define MAX_TIME 5				//זמן מקסימלי שמותר לבזבז על אוכל ו/או חשיבה

int N;							//מספר פילוסופים
sem_t *mutex = NULL;			// מצביע לסמפור בינארי
sem_t *eaters = NULL;			//Array of semaphores[N]מערך סמפורים של פילוסופים
int* state = NULL;				//(מערך שלמים לקביעת מצב פילוסוף (אוכל, רעב, חושב

/////////////////////////////////////////////////////////////////////////////////////////////
 
void eat(int i){ 
int time = rand() % MAX_TIME;	//time = value :  0 ..  Max_time-1
    printf("Philosopher #%d is eating...\n", i + 1);
    sleep(time);
    printf("Philosopher #%d stopped eating...\n", i + 1);
}
 /////////////////////////////////////////////////////////////////////////////////////////////
 
void think(int i){
    int time = rand() % MAX_TIME;	//time =  value :  0 ..  Max_time-1
    printf("Philosopher #%d is thinking...\n", i + 1);
    sleep(time);
}
 /////////////////////////////////////////////////////////////////////////////////////////////
 
void test(int i){	// בדיקה האם פילוסוף רעב יכול להתחיל לאכול
if (state[i] == HUNGRY && state[RIGHT] != EATING && state[LEFT] != EATING)// בודקים האם הפילוסוף רעב והשכנים לא אוכלים (המקלות פנויים
{
	state[i] = EATING;	// שינוי סטסוס - הפילוסוף אוכל 
	sem_post(&eaters[i]);	// פילוסוף התחיל לאכול, לכן הסמפור שלו עולה כדי שהשכנים שלו לא ינעלו אותו
	}
}
 
/////////////////////////////////////////////////////////////////////////////////////////////
 
void take_sticks(int i){ //פילוסוף מנסה לקחת מקל

	sem_wait(mutex);	//נכנסים לקטע קריטי ונועלים אותו כדי שהמקלות יוכל לקחת רק פילוסוף אחד
	state[i] = HUNGRY;	//הפילוסוף רעב
    test(i); //מנסה לקחת 2 מקלות בעזרת פונקציית טסט
    sem_post(mutex); //עוזבים את הקטע הקריטי
    sem_wait(&eaters[i]); // אם לא הצליח להתחיל לאכול, ננעל. השכנים יוכלו "לפתוח" אותו אחרי שיוכלו
}
/////////////////////////////////////////////////////////////////////////////////////////////
 
void put_sticks(int i){	//פילוסוף מחזיר מקל

    sem_wait(mutex); //נכנסים לקטע קריטי ונועלים אותו כדי שרק פילוסוף אחד יעבוד עם המקלות
	state[i] = THINKING;   //פילוסוף סיים לאכול, התחיל לחשוב
         //בודקים האם השכן משמאל רעב, ז"א, ממתין למקל, אז נותנים לו סימן שיתחיל לאכול
    test(LEFT);
	test(RIGHT); 
 
    sem_post(mutex);
}

/////////////////////////////////////////////////////////////////////////////////////////////
 
void* philosopher(void* arg){	//הקוד הזה מתבצע על ידי כל חוט המתאר התנהגות של פילוסוף

int i = *((int*)arg);
while (1 == 1){	//לולאה אין סופית
think(i); //קודם כל הפילוסוף חושב
take_sticks(i); //אחר כך מנסה לקחת את המקלות
eat(i); 
put_sticks(i);	//שם את המקלות על השולחן
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
 
int main(int argc, char* argv[]){
	 int i; // to use as indexes
		   void*  result;
	  if (argc != 2){
		  fprintf(stderr, "Usage: %s <NUMBER_OF_PHILOSOPHERS>\n", argv[0]);
	 	 return 1;
	  }
	  //הכנה...
	  N = atoi(argv[1]);	//CONVERT NUMBER OF PHILOSOPHES 
		  

	if (N <= 1){
	 	 fprintf(stderr, "Error by transformation of the argument...\n");return 2;//לא פחות מ-2 פילוסופים
			}
	  mutex = (sem_t*)malloc(sizeof(sem_t));
	 if(mutex == NULL){printf("\nAllocation Error in mutex\n"); exit(-1);}

	 if (sem_init(mutex, 0, 1) != 0){fprintf(stderr, "Error by creating semaphore...\n");return 3;}


		////////////////////////////////////////////////////////
		// Allocation because we didn't knew the N in compliation time 
		// after getting N in run Time

		 eaters = (sem_t*)malloc(N * sizeof(sem_t));
		if(eaters == NULL){printf("\nAllocation Error in eaters\n"); exit(-1);}
		  state = (int*)malloc(N * sizeof(int)); //state[N] // state = (int*)malloc(3 * sizeof(int));     
		if(state == NULL){printf("\nAllocation Error in state\n"); exit(-1);}

		////////////////////////////////////////////////////////

	  memset(state, 0, N); //state[N] ={0,0,0,..,0}
	 //copies the character 0 to the first n characters of the string pointed to, 

	  srand(time(NULL));
	  pthread_t *philosophers = (pthread_t*)malloc(N * sizeof(pthread_t)); // Array of threads [N]

	 if(philosophers == NULL){printf("\nAllocation Error in philosophers threads\n"); exit(-1);}
	  
	 for (i = 0; i < N; i++){   // All the eating semaphores are loucked 
		 if (sem_init(&eaters[i], 0, 0) != 0){fprintf(stderr, "Error by creating semaphore...\n");return 3;}
	  }


	  for (i = 0; i < N; i++){ 	// creating N threads each one call philosopher function 
		  if (pthread_create(&philosophers[i], NULL, philosopher, (void*)&i) != 0){fprintf(stderr, "Error by creating thread\n");return 2;}

	  usleep(100000);
  	}


	  for (i = 0; i < N; i++){	// waiting for all threads to finish
 		 if (pthread_join(philosophers[i], &result) != 0){fprintf(stderr, "Error by joining thread\n");return 3;}
 	 }


  return 0;
}
