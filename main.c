#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "io.h"

int init_processes(size_t Num);
char** get_argv_per_process(char* sample_argv, size_t length);
int get_bash_argv(void);

char GlobalArgv[4096];
char* Operations[255];
 
int main(void)
{
  int run = 1; 
  while(run)
  {
    //printf("bash: ");
    system("echo -n $USER@~: ");
    fflush(stdout);

    size_t Num = get_bash_argv();

    init_processes(Num);

    //run = 0;
  }

  return 0;
}

int get_bash_argv(void)
{
  char ch; int i = 0;
  while((ch = getchar()) != '\n') {
    GlobalArgv[i] = ch; i++;
  }

  size_t index = 0;
  char* sample_argv = strtok(GlobalArgv, "|");
  
  while(sample_argv!= NULL) {
    Operations[index++] = sample_argv;
    sample_argv = strtok(NULL, "|"); 
  }

  return index;
}

int init_processes(size_t Num)
{
  int input_fd = STDIN_FILENO; // first process reading from stdin
  
  for(size_t index = 0; index < Num; index++){

    int fds[2];
    pipe(fds); // fds[0] - reading end | fds[1] = writing end 

    pid_t pid = fork();
    if (pid == 0)
    {
      if (index < Num - 1)
        dup2(fds[1], 1); //writing in output means writing in write end of the pipe

      dup2(input_fd, 0); //reading from input_fd(at the begging it equals to stdin, then to reading end of pipe)
      char** argv_array = get_argv_per_process(Operations[index], strlen(Operations[index]));
      execvp(argv_array[0], argv_array);  
      //if execvp returned it means that error occured and we need to clean after execvp 
      perror("execvp failed");
      //close(new_fd);
      free(argv_array); argv_array = NULL;
      exit(0);
    } 

    //int new_fd = dup2(fds[0], 0); //reading from stdin means reading from reading end of the pipe
    close(fds[1]); //closing writing end of pipe
    input_fd = fds[0]; // reading end of previous pipe;
  }

  int status;
  for (size_t i = 0; i < Num; i++) {
    wait(&status);
  }

  return 0;
}

char** get_argv_per_process(char* sample_argv, size_t length)
{
  //printf("sample_argv = %s\nlength = %zu\n", sample_argv, length);
  int argc = CountWords(sample_argv, length);
  char** argv_array = (char**)calloc(argc+1, sizeof(char*));
  argv_array[argc] = NULL;

  size_t argv_index = 0;
  
  bool is_word_new = false;
  bool is_word_old = false;
  char* word_begin = NULL;

  for (size_t i = 0; i < length; i++) {

    if (sample_argv[i] == ' ' || sample_argv[i] == '\n') {
      sample_argv[i] = '\0';
      is_word_new = false;
    }

    else {
      is_word_new = true;
    }

    if (is_word_old == false && is_word_new == true) {//means that word begin here
      word_begin = &sample_argv[i];
    }  

    if ((is_word_old == true && is_word_new == false )|| i == length-1) {//means that word ends here
      argv_array[argv_index++] = word_begin;
    }  

    is_word_old = is_word_new;

  }

  // for (int i = 0; i < argc; i++)
  // {
  //   printf("argv_array[%d] = '%s'\n", i, argv_array[i]);
  // }
  return argv_array;
}

/*int fds[2];

  if (pipe(fds) != 0)
  {
    fprintf(stderr, "Mybash: %s\n", strerror(errno));
    exit(-1);
  }

  pid_t pid = fork();
  if (pid == -1)
  {
    fprintf(stderr, "fork: %s\n", strerror(errno));
    exit(-1);
  }

  if (pid == 0)
  {
    close(fds[1]); 
    close(fds[0]);
    exit(1);
  }*/