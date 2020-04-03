#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>

int main(int argc, char **argv){
  char *source, *destination;
  int src_fd, dst_fd;
  size_t bytes_read;
  struct timeval start, end;
  int overall_time = 0;

  if(argc < 3){
    printf("copy <source> <destination>\n");
    exit(EXIT_FAILURE);
  }

  source = argv[1];
  destination = argv[2];

  gettimeofday(&start, NULL);
  src_fd = open(source, O_RDONLY, 0777);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("open = %d\n", (end.tv_usec - start.tv_usec));

  if(src_fd < 0){
    perror("src_fd");
    exit(EXIT_FAILURE);
  }
  gettimeofday(&start, NULL);
  bytes_read = lseek(src_fd, 0, SEEK_END);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("lseek = %d\n", (end.tv_usec - start.tv_usec));

  gettimeofday(&start, NULL);
  void *src_map = mmap(NULL, bytes_read, PROT_READ, MAP_SHARED, src_fd, 0);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("mmap = %d\n", (end.tv_usec - start.tv_usec));

  if(src_map == (void*) -1){
    perror("src_map");
    exit(EXIT_FAILURE);
  }

  gettimeofday(&start, NULL);
  dst_fd = open(destination, O_RDWR | O_CREAT, 0777);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("open = %d\n", (end.tv_usec - start.tv_usec));
  if(dst_fd < 0){
    perror("dst_fd");
    exit(EXIT_FAILURE);
  }

  gettimeofday(&start, NULL);
  lseek(dst_fd, bytes_read -1, SEEK_SET);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("lseek = %d\n", (end.tv_usec - start.tv_usec));

  gettimeofday(&start, NULL);
  write(dst_fd, "\0", 1);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("write = %d\n", (end.tv_usec - start.tv_usec));

  gettimeofday(&start, NULL);
  void *dst_map = mmap(NULL, bytes_read, PROT_WRITE, MAP_SHARED, dst_fd, 0);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("mmap = %d\n", (end.tv_usec - start.tv_usec));

  if(dst_map == (void*) -1){
    perror("dst_map");
    exit(EXIT_FAILURE);
  }

  gettimeofday(&start, NULL);
  memcpy(dst_map, src_map, bytes_read);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("memcpy = %d\n", (end.tv_usec - start.tv_usec));

  gettimeofday(&start, NULL);
  munmap(src_map, bytes_read);
  munmap(dst_map, bytes_read);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("munmap = %d\n", (end.tv_usec - start.tv_usec));

  gettimeofday(&start, NULL);
  close(src_fd);
  close(dst_fd);
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("close = %d\n", (end.tv_usec - start.tv_usec));
  printf("overall time elapsed = %d\n", overall_time);
  return 0;
}
