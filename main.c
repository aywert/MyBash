#include <stdio.h>
#include <sys/types.h>
#include "io.h"

int init_processes(char* GlobalArgv);
char** get_argv_per_process(char* sample_argv, size_t length);
int get_bash_argv(void);

char GlobalArgv[4096];
 
int main(void)
{
  int run = 1; 
  while(run)
  {
    printf("bash: ");

    get_bash_argv();

    init_processes(GlobalArgv);
    
    run = 0;
  }

  return 0;
}

int get_bash_argv(void)
{
  char ch; int i = 0;
  while((ch = getchar()) != '\n') {
    GlobalArgv[i] = ch; i++;
  }

  return 0;
}

int init_processes(char* GlobalArgv)
{
  char* sample_argv = strtok(GlobalArgv, "|");

  while(sample_argv!= NULL) {
    printf("it is me: '%s'\n", sample_argv);
    char** argv_array = get_argv_per_process(sample_argv, strlen(sample_argv));

    // for (int i = 0; i < CountWords(sample_argv, strlen(sample_argv)); i++)
    //   printf("argv_array[%d] = '%s'\n", i, argv_array[i]);
    
    free(argv_array); argv_array = NULL;

    sample_argv = strtok(NULL, "|"); 
  }

  return 0;
}

char** get_argv_per_process(char* sample_argv, size_t length)
{
  //printf("sample_argv = %s\nlength = %zu\n", sample_argv, length);
  int words = CountWords(sample_argv, length);
  char** argv_array = (char**)calloc(CountWords(sample_argv, length), sizeof(char*));
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

  for (int i = 0; i < words; i++)
  {
    printf("argv_array[%d] = '%s'\n", i, argv_array[i]);
  }
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