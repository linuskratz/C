#include <syscall.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

size_t strlen(const char *s) {
  size_t ret = 0;
  while(*s++ != '\0') ret++;
  return ret;
}

ssize_t syscall (long code, long arg1, long arg2, long arg3){
  long ret = 0;
  __asm__ (""::"a"(code), "D"(arg1), "S"(arg2), "d"(arg3));
  __asm__ ( "syscall;"
            "movq %%rax, %0;"
            :"=r"(ret));
  return ret;
}





int open(const char *pathname, int flags) {
  return syscall(2, (long)pathname, (long)flags, 0);
}

ssize_t read(int fd, void *buf, size_t count) {
  return syscall(0, (long) fd, (long) buf, (long) count);
}

ssize_t write(int fd, const void *buf, size_t count) {
  return syscall(1, (long) fd, (long) buf, (long) count);
  /*
  __asm__ ("movq $1, %%rax;"
          "movq %1, %%rdi;"
          "movq %2, %%rsi;"
          "movq %3, %%rdx;"
          "syscall;"
          "movq %%rax, %0;"
          :"=r"(ret)
          :"r"((long)fd), "r"(buf), "r"(count));

  return (ssize_t)ret;
    */
}


/**/





int close(int fd) {
  return syscall(3, (long) fd, 0, 0);
}

int puts(const char *s) {
  return write(STDOUT_FILENO, s, strlen(s));
}

void putf(int fd) {
  char buf[1025] = {'\0'};
  ssize_t ret = 0;
  do {
    ret = read(fd, buf, sizeof(buf) - 1);
    if (ret >= 0) buf[ret] = '\0';
    puts(buf);
  } while(ret == sizeof(buf) - 1);
  close(fd);
}

int print_file(const char *path) {
  int fd = open(path, 0);
  if (fd < 0){
    puts("%");
    puts(path);
    puts("%: ");
    if (fd == -13){
      puts("Permission denied\n");
    }else if (fd == -2){
      puts("No such file or directory\n");
    }else{
      puts("Some other error\n");
    }
    return 1;
  }else{
    putf(fd);
  }
  return 0;
  /* TODO 4. Fehler-Behandlung des open Syscalls
   * (die folgenden drei sind ausreichend für diese Aufgabe):
   * - puts("%path%: Permission denied\n");
   * - puts("%path%: No such file or directory\n");
   * - puts("%path%: Some other error\n");          // <-- all other errors
   */
}

void _exit(int status) {
  __asm__ ( "movq %0, %%rdi;"
            "movq $60, %%rax;"
            "syscall;"
            ::"r"((long)status));
  _exit(-1);
}

int main(int argc, char* argv[]) {
  int ret = EXIT_SUCCESS;
  if (argc == 1) {
    puts("Usage: ");
    puts(argv[0]);
    puts(" [FILE]\n");
    return EXIT_FAILURE;
  } else {
    for(int i = 1; i < argc; i++) {
      if (print_file(argv[i]) == 1) ret = EXIT_FAILURE;
    }
    return ret;
  }
}

void _start() {
  long argc;
  long argv;
  __asm__("movq %%rsp, %%rax;"
          "addq $40, %%rax;"
          "movq (%%rax), %0;"
          "addq $8, %%rax;"
          "movq %%rax, %1;"
          :"=r"(argc), "=r" (argv));
   int ret= main((int)argc,(char **)argv);
   _exit(ret);
  /* TODO 2. Auslesen der argc und argv Argumente und deren Übergabe an die main-Methode */
}
