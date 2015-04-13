


#include "./common.h"


void * routine(void * arg);
int pool_add_job(void *(*process)(void * arg),void *arg);
int pool_init(unsigned int thread_num);
int pool_destroy(void);
void *test(void *);

