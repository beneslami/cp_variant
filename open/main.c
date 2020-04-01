#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 64

int main(int argc, char **argv){
  char *source;
  char *destination;
  int src_fd, dst_fd, bytes_read;
  int total_size = 0;
  char buffer[BUFFSIZE];

  if (argc < 3){
    printf("copy <source> <destination>\n");
    exit(EXIT_FAILURE);
  }
  source = argv[1];
  destination = argv[2];

  src_fd = open(source, O_RDONLY);
  if(src_fd < 0){
    perror("src_fd");
    exit(EXIT_FAILURE);
  }

  dst_fd = open(destination, O_WRONLY | O_CREAT, 0777);
  if(dst_fd < 0){
    perror("dst_fd");
    exit(EXIT_FAILURE);
  }

  while((bytes_read = read(src_fd, buffer, BUFFSIZE)) > 0){
    write(dst_fd, buffer, bytes_read);
    total_size += bytes_read;
  }
  printf("%d\n", total_size);
  close(src_fd);
  close(dst_fd);
  return 0;
}
