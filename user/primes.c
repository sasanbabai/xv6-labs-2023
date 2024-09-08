#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
work(int lp[])
{
  int prime, num, forked = 0;
  int rp[2];
 
  pipe(rp);
  
  read(lp[0], &prime, sizeof(int));
  printf("prime %d\n", prime);
  
  while(read(lp[0], &num, sizeof(int))){
    if(num % prime == 0)
      continue;

    if(!forked){
      if(fork() == 0){
	close(rp[1]);      
        work(rp);
	close(rp[0]);
	return 0;
      }
      close(rp[0]);
      forked = 1;
    }
    write(rp[1], &num, sizeof(int));
  }
  
  close(rp[1]);
  wait(0);
  return 0;
}

int
main(int argc, char *argv[])
{
  int rp[2];
  pipe(rp);
  
  if(fork() == 0){
    close(rp[1]);	  
    work(rp);
    close(rp[0]);
  }else{
    close(rp[0]);	  
    for(int i = 2; i <= 35; i++){
      write(rp[1], &i, sizeof(int));
    }
    close(rp[1]);
    wait(0);
  }
  
  exit(0);
}
