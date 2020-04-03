# cp_variant
two variants of a file copy program

## open:
The program operates using a daily open/read/write system calls. I measured elapsed time for each system call and calculate overall elapsed time. The extracted data is as below table:

|Chuck Size| open | write | overall |
|----------|------|-------|---------|
  64    |   53 |  148602 | 152866|
  128   |   69  |620220 | 624496|
  256   |   62  |373219 | 377616|
  512   |   52  | 221955| 226341|
  1024  |   95  | 162260| 166678|
  2048  |   53  | 118529| 122987|
  4096  |   54  | 104302| 108688|
  8192  |   94  | 69275|  73773|
  16384 |   74  | 44933|  49712|
  32768 |   49  | 29641|  34209|
  65536 |   49  | 25324|  29755|
  131072|   63  | 18946|  23879|
  262144|   58  | 18173|  23175|
  524288|   81  | 14188|  23722|
  1048576|  45  | 14612|  18985|
  2097152|  72  | 15279|  20007|
  4194304|  53  | 16091|  20788|

As you may see in the table, there might be a high correlation between write systemcall and overall elapsed time. So I quantitatively calculated:

+ correlation(write, overall) = 0.99
