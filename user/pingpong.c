#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSGSIZE 5

int
main(int argc, char *argv[])
{
  char buffer[MSGSIZE];
  int p[2];
  if(pipe(p) < 0){
    fprintf(2, "failed to create the pipei\n");
    exit(1);
  };
  int pid = fork();
  if(pid > 0) {
    if(write(p[1], "ping", MSGSIZE) != MSGSIZE){
      fprintf(2, "parent failed writing to the pipe\n");
      exit(1);
    }
    if(read(p[0], buffer, MSGSIZE) == -1){
      fprintf(2, "parent failed reading from the pipe\n");
      exit(1);
    }
    printf("%d: received %s\n", getpid(), buffer);
  } else if(pid == 0) {
    close(0);	  
    if(read(p[0], buffer, MSGSIZE) == -1){
      fprintf(2, "chiled failed reading from the pipe\n");
      exit(1);
    }      
    if(write(p[1], "pong", MSGSIZE) != MSGSIZE){
      fprintf(2, "chiled failed writing to the pipe\n");
      exit(1);
    }
    printf("%d: received %s\n", getpid(), buffer);	  
  } else{
    fprintf(2, "fork error\n");
    exit(1);
  }
  
  exit(0);
}

