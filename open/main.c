#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define BUFFSIZE 1048576
#define EVALUATE 0

int main(int argc, char **argv){
  char *source;
  char *destination;
  int src_fd, dst_fd, bytes_read;
  int total_size = 0;
  int overall_time = 0;
  char buffer[BUFFSIZE];
  struct timeval start, end;

  if (argc < 3){
    printf("copy <source> <destination>\n");
    exit(EXIT_FAILURE);
  }
  source = argv[1];
  destination = argv[2];
#if EVALUATE
  gettimeofday(&start, NULL);
#endif
  src_fd = open(source, O_RDONLY);
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
  dst_fd = open(destination, O_WRONLY | O_CREAT, 0777);
#if EVALUATE
  gettimeofday(&end, NULL);
  overall_time += (end.tv_usec - start.tv_usec);
  printf("open = %d\n", (end.tv_usec - start.tv_usec));
#endif
  if(dst_fd < 0){
    perror("dst_fd");
    exit(EXIT_FAILURE);
  }
//#if EVALUATE
  gettimeofday(&start, NULL);
//#endif
  while((bytes_read = read(src_fd, buffer, BUFFSIZE)) > 0){
    int bytes_write = write(dst_fd, buffer, bytes_read);
    if(bytes_read != bytes_write){
      printf("Failed copying\n");
        exit(EXIT_FAILURE);
    }
  }
//#if EVALUATE
  gettimeofday(&end, NULL);
  printf("overall = %ld.%ld\n", (long int)(end.tv_sec - start.tv_sec), (long int)(end.tv_usec - start.tv_usec)/1000);
  //overall_time += (end.tv_usec - start.tv_usec);
  //printf("write = %d\n", (end.tv_usec - start.tv_usec));
//#endif
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
