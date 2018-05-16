#include "include/semaphore.h"
#include <strings.h>
#include "include/mutex.h"
#include "include/process.h"
#include "include/scheduler.h"
#include "include/videoDriver.h"
#include "include/lib.h"
#include <ipc_info.h>
#include <queueADT.h>


typedef struct {
  	char name[SEMAPHORE_NAME_LEN];
    uint64_t state;
    uint64_t lock_queue;
	queueADT process_queue;
} semaphore;

static semaphore open_semaphore[MAX_SEMAPHORE];

static int array_lock;

static int is_open(int key);
static void create_new_semaphore(int key, char * name);
static void sync_enqueue(semaphore * v, process * p);
static process * sync_dequeue(semaphore * v);
static void fill_semaphore_info(semaphore_info * sem_info, int key);

static int is_open(int key)
{
	return key >= 0 && key < MAX_SEMAPHORE && open_semaphore[key].state == OPEN;
}

int initialize_semaphore()
{
	array_lock = mutex_open(SEMAPHORE_IDENTIFIER);
	return 1;
}

int semaphore_open(char * name)
{
  int i;
	mutex_lock(array_lock);

	for(i = 0; i < MAX_SEMAPHORE && open_semaphore[i].state == OPEN; i++)
  {
		if (strcmp(open_semaphore[i].name, name) == 0)
    {
			mutex_unlock(array_lock);
			return i;
		}
	}

	if (i == MAX_SEMAPHORE) {
		mutex_unlock(array_lock);
		return MAX_SEMAPHORE_OPEN_ERROR;
	}

	create_new_semaphore(i, name);

	mutex_unlock(array_lock);

	return i;
}

static void create_new_semaphore(int key, char * name)
{
	char aux[MUTEX_NAME_LEN];
	int digits = itoa(key, aux, 10);

	strcpyWithNoIndex(aux+digits, SEMAPHORE_IDENTIFIER);

	open_semaphore[key].lock_queue = mutex_open(aux);

	strcpyWithNoIndex(open_semaphore[key].name, name);
	open_semaphore[key].state = OPEN;
	open_semaphore[key].process_queue = create_queue();
}

int semaphore_wait(int semaphore_key, int lock_key) {
	if (is_open(semaphore_key))
  {
		semaphore * v = &open_semaphore[semaphore_key];
		process * p = get_current_process();
		mutex_unlock(lock_key); /* Se desbloquea el mutex de la zona critica */
		sync_enqueue(v, p);
		block_process(p);  /* Se bloquea hasta que otro proceso haga signal o broadcast */
		yield_process();
		mutex_lock(lock_key); /* Aseguramos que al desbloquearse, recupere mutex de zona critica */
		return 1;
	}

	return NOT_OPEN_ERROR;
}

int semaphore_signal(int key)
{
	if (is_open(key))
  {
		semaphore * v = &open_semaphore[key];
		process * p = sync_dequeue(v);
		if (p != NULL)
			unblock_process(p);  /* Deslboquea proceso que estaba bloqueado por cond_wait */
		return 1;
	}

	return NOT_OPEN_ERROR;
}

int semaphore_broadcast(int key)
{
	if (is_open(key))
  {
		semaphore * v = &open_semaphore[key];
		mutex_lock(v->lock_queue);
		while(!is_empty(v->process_queue))
			unblock_process(dequeue(v->process_queue));
		mutex_unlock(v->lock_queue);
		return 1;
	}

	return NOT_OPEN_ERROR;
}

int semaphore_close(int key)
{
  if (is_open(key))
  {
    semaphore * v = &open_semaphore[key];
    semaphore_broadcast(key);
    destroy_queue(v->process_queue);
    mutex_close(v->lock_queue);
    v->state = CLOSED;
    return 1;
  }
  return NOT_OPEN_ERROR;
}

static void sync_enqueue(semaphore * v, process * p)
{
	mutex_lock(v->lock_queue);
	enqueue(v->process_queue, p);
	mutex_unlock(v->lock_queue);
}

static process * sync_dequeue(semaphore * v)
{
	process * p = NULL;
	mutex_lock(v->lock_queue);
	if (!is_empty(v->process_queue))
		p = dequeue(v->process_queue);
	mutex_unlock(v->lock_queue);
	return p;
}

int get_semaphore_info(semaphore_info info_array[])
{
  int i, j;

  mutex_lock(array_lock);

  for (i = j = 0; i < MAX_SEMAPHORE; i++)
  {
    if (is_open(i))
      fill_semaphore_info(&info_array[j++], i);
  }
  mutex_unlock(array_lock);
  return j;
}

static void fill_semaphore_info(semaphore_info * semaphore_info, int key) {
  strcpyWithNoIndex(semaphore_info->name, open_semaphore[key].name);
  semaphore_info->key = key;
}
