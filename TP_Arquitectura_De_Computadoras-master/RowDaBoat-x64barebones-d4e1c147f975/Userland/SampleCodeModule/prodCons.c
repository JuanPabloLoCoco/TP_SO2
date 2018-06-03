#include <syscalls.h>
#include <stdlib.h>
#include <prodCons.h>
#include <math.h>
#include <defs.h>
#include <ctype.h>

static void producer();
static void consumer();
static void control_speed();
static void print_speeds();

static void pause();
static void unpause();
static void print_commands();

static int producerSleep = INITIAL_PROD_SLEEP;
static int consumerSleep = INITIAL_CONS_SLEEP;

static int * buffer;
static int buffer_size;
static int index;

static int buffer_mutex;
static int semaphore_variable;

static int paused;

static void producer() {
  int product;

  yield();

  while(1) {
    product = rand_int();
    mutex_lock(buffer_mutex);

    while (index == buffer_size) {
      printf("Cannot insert. Buffer full!\n");
      semaphore_wait(semaphore_variable, buffer_mutex);
    }

    buffer[index++] = product;
    printf("Produced %d\n", product);
    printf("Number of items: %d / %d\n\n", index, buffer_size);
    semaphore_signal(semaphore_variable);
    mutex_unlock(buffer_mutex);
    sleep(producerSleep * SLEEP_MULTIPLIER);
  }
}

static void consumer()
{
  int product;

  yield();

  while(1) {
    mutex_lock(buffer_mutex);

    while (index == 0) {
      printf("Cannot consume. Buffer empty!\n");
      semaphore_wait(semaphore_variable, buffer_mutex);
    }

    product = buffer[--index];

    printf("Consumed %d\n", product);
    printf("Number of items: %d / %d\n\n", index, buffer_size);
    semaphore_signal(semaphore_variable);
    mutex_unlock(buffer_mutex);
    sleep(consumerSleep * SLEEP_MULTIPLIER);
  }
}

void start_producer_consumer_problem2()
{
  int buf_size = 100;
  int prod_pid;
  int cons_pid;

  paused = 0;
  index = 0;
  buffer = malloc(buf_size * sizeof(* buffer));
  buffer_size = buf_size;

  print_commands();

  printf("Buffer size: %d\n", buf_size);

  buffer_mutex = mutex_open("PROD_CONS_2");
  semaphore_variable = semaphore_open("PROD_CONS_COND");

  prod_pid = exec(&producer,(uint64_t)NULL, "producer2");
  cons_pid = exec(&consumer,(uint64_t)NULL,"consumer2");

  control_speed();
  kill(prod_pid);
  kill(cons_pid);
  mutex_close(buffer_mutex);
  semaphore_close(semaphore_variable);
  free(buffer);
}

static void control_speed()
{
  char c;
  while((c = getchar())) {
    if (!isspace(c)) {
      switch (c) {
        case 'h':
          print_commands();
          break;
        case 'p':
          if (paused)
            unpause();
          else
            pause();
          break;
        case 'a':
          producerSleep++;
          print_speeds();
        break;
        case 's':
          producerSleep = producerSleep > 0 ? producerSleep-1 : 0;
          print_speeds();
        break;
        case 'z':
          consumerSleep++;
          print_speeds();
        break;
        case 'x':
          consumerSleep = consumerSleep > 0 ? consumerSleep-1 : 0;
          print_speeds();
        break;
        case 'q':
          getchar(); /* Limpia el \n que queda en el buffer */
          return;
        break;
        default:
          printf("Invalid command %c\n", c);
          print_commands();
          break;
      }
    }
  }
}

static void print_speeds()
{
  printf("Producer sleep: %d\n", producerSleep);
  printf("Consumer sleep: %d\n", consumerSleep);
}

static void print_commands()
{
  printf("\nCommands\n");
  printf("h   Prints this help.\n");
  printf("p   Toggles pause.\n");
  printf("a   Increases producer sleep.\n");
  printf("s   Decreases producer sleep.\n");
  printf("z   Increases consumer sleep.\n");
  printf("x   Decreases consumer sleep.\n");
  printf("q   Ends producer consumer problem.\n");
  putchar('\n');
}

static void pause()
{
  paused = 1;
  mutex_lock(buffer_mutex);
  printf("Paused\n");
}

static void unpause()
{
  paused = 0;
  printf("Unpaused\n");
  mutex_unlock(buffer_mutex);
}
