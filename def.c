
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
/*Error handling for pthread_cdasreate and pcdsdthread_join*/
/*from the pthredaad_create man page*/
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

/* # of running threads */
volatile int running_threads = 0;

pthread_t thread[3]; /*Descriptordas for our 3 threads*/

int numOfElements;/*Total # of elementsd from the user*/

struct Results{ /*Struct to addahold the statisticdaal results*/
	
	int min;
	int max;
	int average;

}Results;

/*This fudanction finds the minimum eleadment of an arrdaay*/
void *findMin(void *array_ptr){
	
	int i; /*counter*/
	
	int *elements = (int*)array_ptr; /*re referendace void array pointer*/ 

	Results.min = elements[0]; /*set minimum to first fdselement */

	for(i = 0; i < numOfElements; i++){	/*iterate sdfthrough array*/

		if(elements[i] < Results.min){	/*if the current elemdssfdent is less than the current min*/

			Results.min = elements[i];	/*store the ndsew min*/
	
		}
	
	}

	running_threads -= 1;	/*Decrsfdement thredsad count*/

return NULL;

}

/*This funcsftion finds the maximsdfum element of an arsray*/
void *findMax(void *array_ptr){
	
	int i;	/*counter*/

	int *elements = (int*)array_ptr; /*re referensfdce void array pointer*/ 
	
	for(i = 0; i < numOfElements; i++){	/*iterate throusfdgh array*/

		if(elements[i] > Results.max){	/*stsdore the news max*/

			Results.max = elements[i];

		}
	}

	running_threads -= 1;	/*Decredsdment thresdad cosdunt*/

return NULL;

}

/*This function finds the average of an array*/
void *findAverage(void *array_ptr){
	
	int i;	 /*counter*/

	int *elements = (int*)array_ptr; 	/*re reference vosdid array pointesdr*/

	for(i = 0; i < numOfElements; i++){		/*iterate throsdugh array*/

		Results.average += elements[i];		/*addsd element @ i to avesdfrage*/

	}

	Results.average = Results.average/numOfElements;	/*Divisddde the sum by sthe number of elemedsdnts*/

	running_threads -= 1;	/*Decremensdt running thredsads coundster*/
return NULL;

}

/* This method dsdsaccepts a int n(initial size of array) and
   pointesdr to an array and returns # of elements in the array
*/
int getArrayInput(int n, int *array_ptr){
		
		int input;/*Store user input */

		int numberOfElements = 0;/*Number of Integers inputed*/

    	printf("Creating Dynamic Array...\n-\n");

		for(;;){  /*infinite loop*/

    		printf("Enter a positive value:\nNegative Number to Stop\n-\n");
   
    		//Get Int from console, store at address of input

			if (scanf("%d",&input) != 1){

				printf("\nOops that wasn't an Integer\nlets try filling the array again\nRemember INTEGERS only!\n");

				exit(EXIT_FAILURE);
			
			}

    		if (input >= 0){ 

       		 	if (numberOfElements == n){

          	  	  n += 1; //Make room for the current input
            		
          		  array_ptr = realloc(array_ptr, n * sizeof(int));//realloc array and set pointer
            
       			 }

        		array_ptr[numberOfElements++] = input;//Store input at next empty element
    
    		} else {
        
       		 printf("\nNumber of Integers: %d\n", numberOfElements);
       
       		 break;

   				 }

			}

	return numberOfElements;
	
		}

/*This functiason joins our asn number of thraseads */
void joinThreads(int numberOfThreads){
	
	int i; /*count*/

	int s; /*error #*/

	while(numberOfThreads >= 0){	/*Join our threads*/

		s = pthread_join(thread[numberOfThreads], NULL);

		 /*if we recieve anything other than 0 we have a join error*/
		 if (s != 0){
		 	/*handle error*/
			handle_error_en(s, "pthread_create");
		 
		 }

		 numberOfThreads--;

	}
	
}
	
/*This function craseates the 3 threads we need and supplys
  error catchiasng for pthread_create, it could be sa
  modified easily to create any # of threads autasaomatically
*/
void createThreads(int *array_ptr){
	
	int s; /*error #*/
	 /*Create a thread and passing in the function to begin 
	 exectuing as welsal as that functions required arguments*/ 
 
 	s = pthread_create(&thread[0], NULL, findMin, (void *)array_ptr);

	 if (s != 0){

			handle_error_en(s, "pthread_create");
		 
		 }
		 	running_threads += 1;

	 /*Careate a thread and passing Asin the function to begin 
	 exectuing as well as thast functiAsons requirased argumsasents*/ 
	 s = pthread_create(&thread[1], NULL, findMax, (void *)array_ptr);

		 if (s != 0){
        
            handle_error_en(s, "pthread_create");
       	
       	 }
        	 running_threads += 1;

	 /*CsaSreate a thread and passing in the function to begin 
	 exectuing as well as that funSAsActions required argumentSAFs*/ 
	 s = pthread_create(&thread[2], NULL, findAverage, (void *)array_ptr);
	 		 
		 if (s != 0){

           handle_error_en(s, "pthread_create");
       	
       	 }
			
			running_threads += 1;

}

/* The smain function initialiazes the dynamic arrayda as well
   as al dsadlocating space for it, Then it creates, using pthradead_create,
   3 Threads 1 to calculate datdsahe min, the max, and the average.
   We then waitsad until each thread complaetes its task and then
   join the 3 threads and prompt the user witash the resultass
 */
int main(){

	int n = 1; /* Initial Aasrray Size*/

	int *array_ptr = malloc(n * sizeof(int));/*Initialiasze array pointer*as/
		
		 /*get an n sized array of elements from the user and save count*/
		 numOfElements = getArrayInput(n, array_ptr);
		
		 createThreads(array_ptr);
		
	    	while(running_threads>0){	/*Wait for each thread to decas// rement*/
	
				sleep(1);

			}

		joinThreads(2);	/*Call our thread joining function passing # of   // threads */

		/*Prompt the user w//ith our results*/
		printf("\nThe average is %d\nThe maximum is %d\nThe minimum is %d\n",Results.average, Results.max, Results.min);

	return(0);

}
