/* Author: Ana Paula Mendes
 * Github: /anapaulamendes 
 * To compile the code: gcc -pthread code.c
 * To run the code: ./a.out
 */


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
 
#define N 5
#define LEFT (i + N-1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
 
int state[N];
sem_t mutex;
sem_t S[N];

int individuals[N] = {0, 1, 2, 3, 4};

char get_philosopher(int i) {
  char a = 'A',
       b = 'B',
       c = 'C',
       d = 'D',
       e = 'E';

  if (i == 0) {
    return a;
  } else if (i == 1) {
    return b;
  } else if (i == 2) {
    return c;
  } else if (i == 3) {
    return d;
  } else {
    return e;
  }
}

char get_state(int i) {
  char t = 't',
       h = 'h',
       e = 'e';

  if (i == 0) {
    return t;
  } else if (i == 1) {
    return h;
  } else {
    return e;
  }
}

void print_state() {
  int i;

  for (i = 0; i < N; i++) {
    if (i == 0) {
      printf("[ ");
    }
    printf("%c: %c ", get_philosopher(i), get_state(state[i]));
    if (i == N-1) {
      printf("] - ");
    }
  }
}

void think(int i, int left) {
  printf("%c return the forks: %d and %d. ", get_philosopher(i), left+1, i+1);
  printf("%c is thinking.\n", get_philosopher(i));
}

void hungry(int i){
  printf("%c is hungry.\n", get_philosopher(i));
}

void eat(int i, int left) {
  printf("%c takes the forks: %d and %d. ", get_philosopher(i), left+1, i+1);
  printf("%c is eating.\n", get_philosopher(i));
}

void test(int i) {
  if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
    state[i] = EATING;
    sleep(2);
    print_state();
    eat(i, LEFT);
    sem_post(&S[i]);
  }
}

void take_forks(int i) {
  sem_wait(&mutex);
  state[i] = HUNGRY;
  print_state();
  hungry(i);
  test(i);
  sem_post(&mutex);
  sem_wait(&S[i]);
  sleep(1);
}

void put_forks(int i) {
  sem_wait(&mutex); 
  state[i] = THINKING;
  print_state();
  think(i, LEFT);
  test(LEFT);
  test(RIGHT);
  sem_post(&mutex);
}

void *philosopher(void *individual) {
  while (1) {
    int *i = individual;
    sleep(1);
    take_forks(*i);
    sleep(0);
    put_forks(*i);
  }
}
 
int main() {
  int i;

  pthread_t thread_id[N];
  sem_init(&mutex, 0, 1);

  for (i = 0; i < N; i++)
    sem_init(&S[i], 0, 0);

  for (i = 0; i < N; i++) {
    pthread_create(&thread_id[i], NULL, philosopher, &individuals[i]);
    print_state();
    printf("%c is thinking.\n", get_philosopher(i));

  }

  for (i = 0; i < N; i++)
    pthread_join(thread_id[i], NULL);
  
  return 0;
}

