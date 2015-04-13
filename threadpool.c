

#include "threadpool.h"

/* We define a queue of jobs which will be processed in thread pool*/
typedef struct job{
    void * (*process)(void *arg); /* process() will employed on job*/
    void *arg;                    /* argument to process */
    struct job * next;
}Job;

/*
 * A threadpool must have the following parts:
 * threads:         a list of threads
 * jobs   :         jobs in thread pool
 * pool_lock:       pthread_mutex_lock for thread pool
 * job_ready:       pthread_cond_t for job ready
 * destroy:         flag indicate whether the pool shall be destroyed
 * size:            current size of jobs list
 * thread_num:      max thread num initialized in pool
 */
typedef struct thread_pool{
    pthread_mutex_t pool_lock; /* pthread_mutex_lock for thread pool */
    pthread_cond_t  job_ready; /* pthread_cond_t for job ready */
    Job * jobs;                /* jobs in thread pool */
    int destroy;               /* flag indicate whether the pool shall be destroyed */
    pthread_t * threads;       /* a list of threads */
    unsigned int thread_num;   /* max thread num initialized in pool */
    int size;                  /* current size of jobs list */
}Thread_pool;

/* global Thread_pool variable*/
static Thread_pool * pool = NULL;

/*Initialize the thread pool*/
int pool_init(unsigned int thread_num)
{
    int i;
    pool=(Thread_pool *)malloc(sizeof(Thread_pool));
    if(NULL==pool)
        return -1;

    // Init
    pthread_mutex_init(&(pool->pool_lock),NULL);
    pthread_cond_init(&(pool->job_ready),NULL);
    pool->jobs = NULL;
    pool->thread_num = thread_num;
    pool->size = 0;
    pool->destroy = 0;
    pool->threads = (pthread_t *)malloc(thread_num * sizeof(pthread_t));

    // create threads according to thread_num
    for(i = 0; i < thread_num; i++)
    {
        pthread_create(&(pool->threads[i]),NULL,routine,NULL);
    }

    return 0;
}

/* Add job into the job list assigned it to some thread */
int pool_add_job(void *(*process)(void *),void *arg)
{
    Job *newjob = (Job *)malloc(sizeof(Job));
    newjob->process = process;
    newjob->arg = arg;
    newjob->next = NULL;

    pthread_mutex_lock(&(pool->pool_lock));
    Job *temp = pool->jobs;
    if(temp != NULL) {
        /* Insert the new job into the last position of job list. */
        while(temp->next)
            temp = temp->next;
        temp->next = newjob;
    }
    else{
        /* Insert the new job into the job list as the first job. */
        pool->jobs = newjob;
    }

    /* For debug */
    assert(pool->jobs != NULL);
    // The current number of jobs in job list
    pool->size++;
    pthread_mutex_unlock(&(pool->pool_lock));

    /* Rouse a thread to process this new job when adding an new job to job list */
    pthread_cond_signal(&(pool->job_ready));

    return 0;
}


/*Destroy the thread pool*/
int pool_destroy(void)
{
    if(pool->destroy)/*Alread destroyed!*/
        return -1;

    int i;
    pool->destroy=1;
    pthread_cond_broadcast(&(pool->job_ready));/*notify all threads*/
    for(i = 0; i < pool->thread_num; i++)
        pthread_join(pool->threads[i], NULL);
    free(pool->threads);
    Job * head = NULL;
    while(pool->jobs) {
        head = pool->jobs;
        pool->jobs = pool->jobs->next;
        free(head);
    }
    pthread_mutex_destroy(&(pool->pool_lock));
    pthread_cond_destroy(&(pool->job_ready));

    free(pool);
    pool = NULL;

    return 0;
}


/* Every thread call this function */
void* routine(void *arg)
{
    printf("start thread %u\n", (unsigned int)pthread_self());

    while(1) {
        pthread_mutex_lock(&(pool->pool_lock));

        while(pool->size == 0 && !pool->destroy) {
            /* if no job then thread has been blocking */
            printf("thread %u is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&(pool->job_ready), &(pool->pool_lock));
        }

        if(pool->destroy){
            pthread_mutex_unlock(&(pool->pool_lock));
            printf("thread %u will exit\n", (unsigned int)pthread_self());
            pthread_exit(NULL);
        }

        printf("thread %u is starting to work\n", (unsigned int)pthread_self());

        /* For debug */
        assert(pool->size!=0);
        assert(pool->jobs!=NULL);

        // Fetch a job from job list(FIFO) to perform it.
        pool->size--;
        Job *job = pool->jobs;
        pool->jobs = job->next;
        pthread_mutex_unlock(&(pool->pool_lock));

        /*process job*/
        (*(job->process))(job->arg);

        free(job);
        job = NULL;
    }

    /* You should never get here */
    pthread_exit(NULL);
}





