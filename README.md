# cp_variant
two variants of a file copy program

## open:
The program operates using a daily open/read/write system calls. I measured elapsed time for each system call and calculate overall elapsed time. The extracted data is as below table:

|Chuck Size| open | write | overall |
|----------|------|-------|---------|
  64    |    
  128   |
  256   |
  512   |
  1024  |
  2048  |
  4096  |
  8192  |
  16384 |
  32768 |
  65536 |  
  131072|
  262144|
  524288|
  1048576|
  2097152|
  4194304|
