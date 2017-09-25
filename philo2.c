//Stephen Bock -11357369

#define _POSIX_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pthread.h>
#include<math.h>

void Philo(int *i);
int randomGaussian_r(int mean, int stddev, unsigned int* state);
pthread_mutex_t chops[5];

int philos[] = { 1, 2, 3, 4, 5 };

int main(int argc, char* argv[])
{
  pthread_t philo1, philo2, philo3, philo4, philo5;

  pthread_mutex_init(&chops[0], NULL);
  pthread_mutex_init(&chops[1], NULL);
  pthread_mutex_init(&chops[2], NULL);
  pthread_mutex_init(&chops[3], NULL);
  pthread_mutex_init(&chops[4], NULL);

  pthread_create(&philo1, NULL, Philo, (void *) &philos[0]);
  pthread_create(&philo2, NULL, Philo, (void *) &philos[1]);
  pthread_create(&philo3, NULL, Philo, (void *) &philos[2]);
  pthread_create(&philo4, NULL, Philo, (void *) &philos[3]);
  pthread_create(&philo5, NULL, Philo, (void *) &philos[4]);

  pthread_join(philo1, NULL);
  pthread_join(philo2, NULL);
  pthread_join(philo3, NULL);
  pthread_join(philo4, NULL);
  pthread_join(philo5, NULL);

  printf("All philosophers are full and no one is left at the table\n");
  exit(0);
}



void Philo(int *i)
{
    int ID = *((int*) i);
    unsigned int state = (unsigned int)ID;
    int totalEat = 0;
    int totalThink = 0;
    int j = *i;
    while (1)
    {
      int z = 0;
      pthread_mutex_lock(&chops[j-1]);
      if (j != 5)                  //There is no chopstick 5, philo 5 will eat from chopstick 0
      {
      z = pthread_mutex_trylock(&chops[j]);
      }
      else
      {
      z = pthread_mutex_trylock(&chops[0]);
      }
      if (z == 0)              //Have succeded in getting both chopsticks
      {
        int eat = randomGaussian_r(9, 3, &state);  //Create the random numbers
        if( eat < 0 ) { eat = 0; }
        int think = randomGaussian_r(11, 7, &state);
        if( think < 0 ) { think = 0; }
        printf("Philo #%i eating for %i seconds\n", j, eat);
        sleep(eat);
        totalEat = totalEat + eat;
        pthread_mutex_unlock(&chops[j-1]); //unlock chopsticks
        if (j != 5)
        {
          pthread_mutex_unlock(&chops[j]);
        }
        else
        {
          pthread_mutex_unlock(&chops[0]);
        }
        printf("Philo #%i has now eaten for %i seconds, now thinking for %i seconds\n", j, totalEat, think);
        sleep(think);                         //Thinking now that its over
        totalThink = totalThink + think;
      }
      else  //Wasnt able to get 2nd chopstick, unlock the first one
      {
        pthread_mutex_unlock(&chops[j-1]);
        sleep(1);
      }
      if (totalEat > 100)
      {
        printf("Philo #%i is now full, total thinking time = %i\n", j, totalThink);
        return;
      }
    }

}

int randomGaussian_r(int mean, int stddev, unsigned int* state) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand_r(state) / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand_r(state) / (double) RAND_MAX;
	if (rand_r(state) & (1 << 5))
		return (int) floor(mu + sigma * cos(f2) * f1);
	else
		return (int) floor(mu + sigma * sin(f2) * f1);
}
