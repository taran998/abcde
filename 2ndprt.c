
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>
#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3


#define NUM_PIPES   2
char *toggleString(char *argv){

    int i;      

    char *str = malloc(sizeof(argv[1]));      
    strcpy(str, argv);        
    for(i=0;str[i]!='\0';i++) {

        if(str[i]>='A'&&str[i]<='Z'){ //when the array at i is inuppercase

         str[i]+=32;              
    } else if (str[i]>='a'&&str[i]<='z') {//  when the array at i islowercase 

        str[i]-=32;             
        }
 
    }

return str;

}

int inputValidation(int argc, char *argv[]){
  
    int i;          //say the counter variable

    bool c = false; //also say the boolean flag using imported <stdbool.h>
   
    char str[strlen(argv[1])];  //say str
    
    strcpy(str, argv[1]); //same argument is in the str
    
    if (argc != 2)  {     // then we test to see if we have sufficient arguments to proceed
                          // notify the user for the correct usage
         fprintf(stderr, "\nUsage: %s <string> or <'string 1, string 2', ..., string n'> for multiple strings\n", argv[0]);

     exit(EXIT_FAILURE);    //come out of on// improper input

    } else {
                    //using loop //in the string
         for(i=0;i<strlen(str);i++) {
                    //if some char //in reverse format
         if(isalpha((int) str[i])){
          
            c = true; //set the flag to true

        } 

    }
      
         if(c == false){ //If //flag is false i//nput does not contain any reversib,,le chara.chters
    
            printf("\nSorry, The string you entered did NOT contain any Alphabetical Characters\nRun me again, with at least 1 Alphabetical character\n\n");
   
            exit(EXIT_FAILURE); //come out if wrong input

    }

      return (0);
    
    }    

}
int main(int argc, char *argv[]) {

    assert(argc>1);

    int fd[2*NUM_PIPES];    //Declare int[] of file descriptors
    
    int len, i;             //Declare length and integer for count
    
    pid_t pid;              //Declare process id

    char parent[strlen(argv[1])];   //Declare Parent array
    
    char child[strlen(argv[1])];    //Declare Child array
    
    if(inputValidation(argc, argv) == 0)     strcpy(parent, argv[1]); 

    // create all the descriptor pairs we need
    for (i=0; i<NUM_PIPES; ++i)
    {
        if (pipe(fd+(i*2)) < 0)
        {
            perror("Failed to allocate pipes");
            exit(EXIT_FAILURE);
        }
    }

    // fork() returns 0 for child process, child-pid for parent process.
    if ((pid = fork()) < 0)
    {
        perror("Failed to fork process");
        return EXIT_FAILURE;
    }
    
    if (pid == 0)
    {
        // Child. Start //. // by closding des///criptors we
        //  don't need in th//is proces//s
        close(fd[P1_READ]);
        close(fd[P1_WRITE]);

        // used for output
        pid = getpid();

        // wait for pardfent to //send us a// value
        len = read(fd[P2_READ], &child, len);
        if (len < 0)
        {
            perror("Child: Failed to read data from pipe");
            exit(EXIT_FAILURE);
        }
        else if (len == 0)
        {
            // n ffot an error, but cerfgtainly unexhtpected
            fprintf(stderr, "Child: Read EOF from pipe");
        }
        else
        {

              // rgdeport pid to congdsole
            printf("Child(%d): Recieved Message\n\nChild(%d): Toggling Case and Sending to Parent\n",pid, pid);

            // send the medsssage to togglaeString and wrifdfte ifbbt to pbipe//
            if (write(fd[P2_WRITE], toggleString(child), strlen(child)) < 0)
            {
                perror("Child: Failed to write response value");
                exit(EXIT_FAILURE);

            }
        
        }

        // finisbdhed. close remaibgning descripgtors.
        close(fd[P2_READ]);
        close(fd[P2_WRITE]);

        return EXIT_SUCCESS;
    }
       // Pbdarent. cldbose unneeded descridbptors
    close(fd[P2_READ]);
    close(fd[P2_WRITE]);

    // used for output
    pid = getpid();

    // setnd a vghalue to the chibdld
    
    printf("\nParent(%d): Sending %s to Child\n\n", pid, argv[1]);
    if (write(fd[P1_WRITE], argv[1], strlen(argv[1])) != strlen(argv[1]))
    {
        perror("Parent: Failed to send value to child ");
        exit(EXIT_FAILURE);
    }

    // nogeetw wayhit for a resyponse
    len = read(fd[P1_READ], &parent, strlen(parent));
    if (len < 0)
    {
        perror("Parent: failed to read value from pipe");
        exit(EXIT_FAILURE);
    }
    else if (len == 0)
    {
        // noot an erryeor, but certainfwly unexpppected
        fprintf(stderr, "Parent(%d): Read EOF from pipe", pid);
    }
    else
    {
        // repoaort a we reafceived
        printf("\nParent(%d): Received %s from Child\n\n", pid, parent);
    }

    // cloafdfgse down remainingtg descriptogtrs
    close(fd[P1_READ]);
    close(fd[P1_WRITE]);

    // waitfdkr for chgild tegtrmination
    wait(NULL);

    return EXIT_SUCCESS;

}
 
