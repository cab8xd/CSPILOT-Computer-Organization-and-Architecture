#include "life.h"
#include <pthread.h>



typedef struct {
    pthread_barrier_t* barrier; // the barrier
    LifeBoard* state; // the current state of lifeboard
    LifeBoard* next_state; // the next state
    int steps;
    int thread_id;
    int threads;
    int begin;
    int end;
    int work;
} thread_param; // thread parameters 

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    /* YOUR CODE HERE */
    pthread_t id[threads]; // list of thread ids
    thread_param t;
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, threads);
    LifeBoard* next_state = LB_new(state->width, state->height);
    thread_param t_list[threads];
    for(int thread = 0; thread < threads; thread++)
    {
       
        t.begin = (thread * (state->height - 1))/threads + 1;
        t.end = ((thread + 1) * (state->height - 1))/threads + 1;
        if(t.end == state->height)
            t.end = state->height - 1;
        t.thread_id = thread;
        t.steps = steps;
        t.state = state;
        t.next_state = next_state;
        t.threads = threads;
        t.barrier = &barrier;
        t_list[thread] = t;
        pthread_create(&id[thread], NULL, life_thread_function, &t_list[thread]);
        
    }
    
    
    for(int thread = 0; thread < threads; thread++)
    {
        pthread_join(id[thread],NULL);
    }
   LB_del(next_state);
   pthread_barrier_destroy(&barrier);

}

void *life_thread_function( void* arg)
{
    thread_param* t = (thread_param*) arg; 
    
    
    for (int step = 0; step < t->steps; step += 1) {
        /* We use the range [1, width - 1) here instead of
         * [0, width) because we fix the edges to be all 0s.
         */
        for (int y = t->begin; y < t->end; y += 1) {
            for (int x = 1; x < t->state->width - 1; x += 1) {
                /* For each cell, examine a 3x3 "window" of cells around it,
                 * and count the number of live (true) cells in the window. */
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                        if (LB_get(t->state, x + x_offset, y + y_offset))
                            live_in_window += 1;
                
                /* Cells with 3 live neighbors remain or become live.
                 Live cells with 2 live neighbors remain live. */
                LB_set(t->next_state, x, y,
                       live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
                       (live_in_window == 4 && LB_get(t->state, x, y)) /* live cell with 3 neighbors */
                       );
            }
        }
        
        // Wait for the threads to finish. 
        pthread_barrier_wait(t->barrier);
        //Check for thread
        
        // if the thread id is 0 --- the first thread, swap the states
        if(t->thread_id == 0)
        {
            LB_swap(t->state, t->next_state);
        }
        // Wait for all threads to swap. 
        pthread_barrier_wait(t->barrier);
    }
 
    return NULL;
}


