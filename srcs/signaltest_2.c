#include<stdio.h> 
#include<unistd.h> 
#include<stdlib.h> 
#include<signal.h> 
#include<sys/wait.h> 

int val = 10; 

void handler(int sig) 
{ 
	if (sig == SIGCHLD)
		printf("sigchld received, nb : %d\n", sig);
    val += 5; 
} 

int	main() 
{ 
    pid_t pid; 

    signal(SIGCHLD, handler);//catch sigchld signals. Fork send this signal
    if ((pid = fork()) == 0) 
    { 
        val -= 3; 
        exit(0);//Child exits so child printf won't be executed
    } 
    waitpid(pid, NULL, 0);//wait child to be terminated 
    printf("val = %d\n", val);// value will be 10 + 5 because of parent caught fork sigchld 
    exit(0); 
} 
