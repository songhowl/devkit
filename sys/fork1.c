#include <stdio.h>
#include <stdlib.h>

void main(){ 
int i; 
if ( fork() == 0 ) { 
/* 子进程程序 */ 
for ( i = 1; i <1000; i ++ ) printf("This is child process id=%u\n",getpid()); 
} 
else { 
/* 父进程程序*/ 
for ( i = 1; i <1000; i ++ ) printf("This is process process id=%d\n",getpid()); 
} 
} 

