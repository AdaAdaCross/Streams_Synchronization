#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <string>
#include <list>
#include <algorithm>
#include <pthread.h>

int honey = 0;
int bee_honey;
pthread_mutex_t mutex;
bool bear_alive = true;

void* bee_thread(void* data)
{
  while(true)
  {
    int sleep_time = rand() % 10;
    printf("Bee will be in flight for %d sec\n", sleep_time);
    sleep(sleep_time);

    pthread_mutex_lock(&mutex);
    honey+=bee_honey;
    if (!bear_alive)
    {
       pthread_mutex_unlock(&mutex);
       break;
    }
    printf("Bee brought %d honey, honey = %d\n", bee_honey, honey);
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main(int argc, char* argv[])
{
 if (argc < 4)
 {
    printf("Not enough arguments!\n");
    return -1;
 }

  srand(time(0));

  int code = pthread_mutex_init(&mutex, NULL);
  if (code)
  {
    printf("Error init mutex %d", code);
  }

  bee_honey = std::atoi(argv[1]);
  int bear_eat_time = std::atoi(argv[2]);
  int bear_eat_rate = std::atoi(argv[3]);

  int bear_hungry_time = 0;

  printf("Start\n");

  pthread_t bees[5];
  for (int i=0; i<5; i++)
  {
    code = pthread_create(&bees[i], NULL, bee_thread, NULL);
  }

  printf("Start wait\n");
  int timer = std::time(0);

while (bear_hungry_time <= bear_eat_time)
{
    sleep(1);
    printf("Bear woke up\n");

    pthread_mutex_lock(&mutex);
    if (honey < bear_eat_rate)
    {
       bear_hungry_time++;
       printf("Bear hungry for %d sec\n", bear_hungry_time);
    }
    else
    {
       honey -=bear_eat_rate;
       bear_hungry_time = 0;
       printf("Bear ate, honey = %d\n", honey);
    }
    pthread_mutex_unlock(&mutex);
}

  printf("Bear dead\n");

  pthread_mutex_lock(&mutex);
  bear_alive=false;
  pthread_mutex_unlock(&mutex);

  printf("Wait for bees\n");
  for (int i=0; i<5; i++)
  {
    pthread_join(bees[i], NULL);
  }
  printf("Success!\n");
  return 0;
}
