#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>


long long current_timestamp() {  
  struct timeval te;  
  gettimeofday(&te, NULL); // get current time  
  long long microseconds = ((long long)te.tv_sec * 1000000LL) + (long long)te.tv_usec; // Converstions to make sure it doesn't overflow
  return microseconds;  
}