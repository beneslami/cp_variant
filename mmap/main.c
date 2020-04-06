#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>

#define BUFFSIZE 4096*1024

int main(int argc, char **argv){
  char *source, *destination;
  int src_fd, dst_fd;
  unsigned int bytes_read;
  struct timeval start, end;
  int overall_time = 0;
  int offset = 0;

  if(argc < 3){
    printf("copy <source> <destination>\n");
    exit(EXIT_FAILURE);
  }

  source = argv[1];       /* source file */
  destination = argv[2];  /* Destiniation file */

  src_fd = open(source, O_RDONLY, 0777);
  if(src_fd < 0){
    perror("src_fd");
    exit(EXIT_FAILURE);
  }
  bytes_read = lseek(src_fd, 0, SEEK_END);

  dst_fd = open(destination, O_RDWR | O_CREAT, 0777);
  if(dst_fd < 0){
    perror("dst_fd");
    exit(EXIT_FAILURE);
  }
  lseek(dst_fd, bytes_read -1, SEEK_SET);
  write(dst_fd, "", 1);

  int bytes = BUFFSIZE;

  while(bytes_read > 0){
    if(bytes_read < bytes){
      bytes = bytes_read;
      bytes_read = 0;
    }
    else{
      bytes_read -= bytes;
    }

    void *src_map = mmap(NULL, bytes, PROT_READ, MAP_SHARED, src_fd, offset);
    if(src_map == (void*) MAP_FAILED){
      perror("src_map");
      exit(EXIT_FAILURE);
    }

    void *dst_map = mmap(NULL, bytes, PROT_WRITE, MAP_SHARED, dst_fd, offset);
    if(dst_map == (void*) MAP_FAILED){
      perror("dst_map");
      exit(EXIT_FAILURE);
    }

    memcpy(dst_map, src_map, bytes);

    int src_unmp = munmap(src_map, bytes);
    if(src_unmp == -1){
      perror("src_unmap");
      exit(EXIT_FAILURE);
    }
    int dst_unmp = munmap(dst_map, bytes);
    if(dst_unmp == -1){
      perror("dst_unmap");
      exit(EXIT_FAILURE);
    }
    offset += bytes;
  }

  close(src_fd);
  close(dst_fd);

  return 0;
}


/* Fancy piece of code to percentize the operaton!!!
int maximum = bytes_read/BUFFSIZE;
int i = 0;
float percent = ((float)(i)/maximum)*100;
printf("%.2f\n", percent);
*/
