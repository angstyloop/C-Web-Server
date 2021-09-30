#include <pthread.h>

pthread_mutex_t count_mutex;
long long count;

// Use the mutex lock to ensure an atomic update of the shared
// variable $count.
void increment_count(){
  pthread_mutex_lock(&count_mutex);
  ++count;
  pthread_mutex_unlock(&count_mutex);
}

// Use the mutex lock to guarantee that the 64-bit quantity
// count is read atomically. While reading an integer value is 
// an atomic operation (because integer is the common word size
// on most machines, on a 32-bit architecture reading a long 
// long (which is really two 32-bit quantities) is not an
// atomic operation.
long long get_count(){
  long long c=0;
  pthread_mutex_lock(&count_mutex);
  c = count;
  pthread_mutex_unlock(&count_mutex);
  return c;
}
