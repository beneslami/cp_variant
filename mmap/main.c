#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>

#define EVALUATE 0

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
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  src_fd = open(source, O_RDONLY, 0777);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("open = %d\n", (end.tv_usec - start.tv_usec));
#endif

  if(src_fd < 0){
    perror("src_fd");
    exit(EXIT_FAILURE);
  }
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  bytes_read = lseek(src_fd, 0, SEEK_END);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("lseek = %d\n", (end.tv_usec - start.tv_usec));
#endif
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  void *src_map = mmap(NULL, bytes_read, PROT_READ, MAP_SHARED, src_fd, 0);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("mmap = %d\n", (end.tv_usec - start.tv_usec));
#endif

  if(src_map == (void*) -1){
    perror("src_map");
    exit(EXIT_FAILURE);
  }
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  dst_fd = open(destination, O_RDWR | O_CREAT, 0777);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("open = %d\n", (end.tv_usec - start.tv_usec));
#endif
  if(dst_fd < 0){
    perror("dst_fd");
    exit(EXIT_FAILURE);
  }
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  lseek(dst_fd, bytes_read -1, SEEK_SET);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("lseek = %d\n", (end.tv_usec - start.tv_usec));
#endif
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  write(dst_fd, "\0", 1);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("write = %d\n", (end.tv_usec - start.tv_usec));
#endif
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  void *dst_map = mmap(NULL, bytes_read, PROT_WRITE, MAP_SHARED, dst_fd, 0);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("mmap = %d\n", (end.tv_usec - start.tv_usec));
#endif
  if(dst_map == (void*) -1){
    perror("dst_map");
    exit(EXIT_FAILURE);
  }
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  memcpy(dst_map, src_map, bytes_read);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("memcpy = %d\n", (end.tv_usec - start.tv_usec));
#endif
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  munmap(src_map, bytes_read);
  munmap(dst_map, bytes_read);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("munmap = %d\n", (end.tv_usec - start.tv_usec));
#endif
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  close(src_fd);
  close(dst_fd);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("close = %d\n", (end.tv_usec - start.tv_usec));
  printf("overall time elapsed = %d\n", overall_time);
#endif
  return 0;
}
