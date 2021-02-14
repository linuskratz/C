// Do magic ╰( ͡° ͜ʖ ͡° )つ──☆*:・ﾟ and implement your "n Processes" programm here.
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


int main() {
  char * buf = (char *) malloc(10*sizeof(char));
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  printf("%s\n","Enter process count:");
  fgets(buf, 10, stdin);
  buf[strlen(buf)-1] = 0;
  int n = atoi(buf);
  if (n>0){
  for (int i = 0; i < n; i++) {
    if (fork() == 0){
      printf("I'm your child! PID: %i, PPID: %i\n", getpid(), getppid());
      pthread_mutex_lock(&lock);
      fgets(buf, 5, stdin);
      buf[strlen(buf)-1] = 0;
      if (strcmp(buf, "END")==0){
        printf("Child with PID %i terminated.\n", getpid());
      }
      pthread_mutex_unlock(&lock);
      exit(0);
      }
  }
  while (wait(NULL) != -1){}
  }
  free(buf);
  pthread_mutex_destroy(&lock);
  printf("Parent with PID %i terminated.\n", getpid());
  return 0;
}
