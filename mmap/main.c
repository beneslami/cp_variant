#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char **argv){
  char *source, *destination;
  int src_fd, dst_fd;
  size_t bytes_read;

  if(argc < 3){
    printf("copy <source> <destination>\n");
    exit(EXIT_FAILURE);
  }

  source = argv[1];
  destination = argv[2];

  src_fd = open(source, O_RDONLY, 0777);
  if(src_fd < 0){
    perror("src_fd");
    exit(EXIT_FAILURE);
  }
  bytes_read = lseek(src_fd, 0, SEEK_END);
  void *src_map = mmap(NULL, bytes_read, PROT_READ, MAP_SHARED, src_fd, 0);
  if(src_map == (void*) -1){
    perror("src_map");
    exit(EXIT_FAILURE);
  }

  dst_fd = open(destination, O_RDWR | O_CREAT, 0777);
  if(dst_fd < 0){
    perror("dst_fd");
    exit(EXIT_FAILURE);
  }

  lseek(dst_fd, bytes_read -1, SEEK_SET);
  write(dst_fd, "\0", 1);
  void *dst_map = mmap(NULL, bytes_read, PROT_WRITE, MAP_SHARED, dst_fd, 0);
  if(dst_map == (void*) -1){
    perror("dst_map");
    exit(EXIT_FAILURE);
  }

  memcpy(dst_map, src_map, bytes_read);

  munmap(src_map, bytes_read);
  munmap(dst_map, bytes_read);
  close(src_fd);
  close(dst_fd);

  return 0;
}
