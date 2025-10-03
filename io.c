#include "io.h"

ssize_t Read(int fd, char buf[], ssize_t size)
{
  int n = read(fd, buf, size);
  if(n < 0)
  {
    fprintf(stderr, "MyCat: %s\n", strerror(errno));
    exit(-1);
  }

  return n;
}

ssize_t Write(int fd, char buf[], ssize_t size)
{
  int n = write(fd, buf, size);

  if(n < 0)
  {
    fprintf(stderr, "MyCat: %s\n", strerror(errno));
    exit(-1);
  }

  return n;
}

int Open(char * filename)
{
  int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
      fprintf(stderr, "MyCat: %s: %s\n", filename, strerror(errno));
      exit(-1);
    }

  return fd;
}

void CopyFile(int fd_read, int fd_write)
{
  char buf[4096] = {0};
  int n = 0;
  ssize_t written = 0;

  do
  {
    n = Read(fd_read, buf, 4096);
    written = 0;
    
    while(n != written)
    {
      ssize_t w = Write(fd_write, buf + written, n - written); //Write replace by write with checking
      written += w;
    }
  } while(n != 0);

  return;
}

int CountWords(char* buf, size_t size_buf)
{
  size_t words = 0;
  bool is_word_new = false;
  bool is_word_old = false;

  for(size_t i = 0; i < size_buf; i++)
  {
    switch(buf[i])
    {
      case '\n': return words; break;
      case ' ':  is_word_new = false; break;
      default:   is_word_new = true; break;
    }
    
    if (!is_word_old && is_word_new)
      words++;
    is_word_old = is_word_new;
  }

  return words;
}