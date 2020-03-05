#include <assert.h>
#include <stdlib.h>
#include <ucontext.h>
#include "thread.h"
#include "interrupt.h"
#include <stdbool.h>

#define LOCKED 1
#define UNLOCKED 0

// Thread states
typedef int Tstate;
enum {
    S_RUNNING,
    S_READY,
    S_BLOCKED,
    S_EXITED
};

/* This is the wait queue structure */
struct wait_queue {
    struct queue* wq;
};

struct queue {
    struct thread* head;
    struct thread* tail;
};
struct queue* rq; // Ready queue
struct queue* eq; // Exit queue

/* This is the thread control block */
struct thread {
    Tid id; // ID number of thread
    Tstate state; // Current state - ready, running, waiting
    ucontext_t tcontext; // Thread context
    void* stackAddress; // Keep track of memory address allocated for thread stack
    
    struct thread* next;
    struct wait_queue* threadwq;
    struct lock* lock;
};
struct thread* currentThread; // Pointer to currently running thread

struct thread** threadAvail; // Direct pointer to a thread, it if does not exist, points to NULL

// Checks whether a queue is empty
bool queue_isempty(struct queue* q)
{
    return q->head==NULL;
}

// Checks whether a thread is a member of a queue
bool queue_ismember(struct queue* q, Tid id)
{
    // Preliminary check
    if(id<0 || id>=THREAD_MAX_THREADS || threadAvail[id]==NULL) return false;
    
    // Go through queue and return true if it found the thread
    struct thread* curr = q->head;
    while(curr!=NULL) {
        if(curr->id == id) return true;
        curr = curr->next;
    }
    
    // If it gets to here it is not part of the queue
    return false;
}

// Returns first thread in ready queue and takes it out of the queue
struct thread* queue_pop(struct queue* q)
{
    if(queue_isempty(q)) return NULL;
    
    struct thread* toPop = q->head; // Get reference to first item in queue
    
    // Remove the first item from queue
    q->head = q->head->next;
    // In case only one item is left in the queue
    if(q->head==q->tail) q->tail = NULL; 
    
    // Set the next field in the returning thread to NULL to prevent
    // unintentional access to queue
    toPop->next = NULL;
    
    return toPop;
}

// Enqueues a thread to the back of the ready queue
void queue_push(struct queue* q, struct thread* toPush)
{
    // If queue is empty
    if(queue_isempty(q)) q->head = toPush;
    // If there was only 1 item in the queue
    else if(q->tail==NULL) {
        q->head->next = toPush;
        q->tail = toPush;
    } else { // More generally, with 2 or more items in queue
        q->tail->next = toPush;
        q->tail = toPush;
    }
}

// Finds a thread with corresponding id and removes it from queue
struct thread* queue_find(struct queue* q, Tid want_tid)
{
    struct thread* toPop;
    struct thread* curr = q->head;
    
    // If the wanted thread is the first in the queue
    if(q->head->id == want_tid) {
        q->head = q->head->next; // Remove from queue
        curr->next = NULL; // Prevent unintentional access to queue
        
        // In case only one item is left in the queue
        if(q->head==q->tail) q->tail = NULL;
        
        return curr;
    }

    // Otherwise, go through the ready queue and check if the next thread
    // to the current item is the wanted thread, if it is, pop it from the list
    while(curr->next!=NULL) {
        if(curr->next->id==want_tid) {
            toPop = curr->next; // Get wanted thread
            
            // In case popped item was the tail of queue
            if(curr->next == q->tail) {
                q->tail = curr;
                curr->next = NULL;
            } else curr->next = curr->next->next; // Remove from queue
            // In case only one item is left in the queue
            if(q->head==q->tail) q->tail = NULL;
            if(q->tail==NULL) q->head->next = NULL; // Just a failsafe...
            
            toPop->next = NULL; // Prevent unintentional access to queue
            
            return toPop;
        }
        curr = curr->next;
    }
    
    // If control reaches here the wanted thread was not found
    return NULL;
}

void
thread_init(void)
{
    threadAvail = malloc(sizeof(struct thread*)*THREAD_MAX_THREADS);
    for(int i=0; i<THREAD_MAX_THREADS; i++) {
        threadAvail[i] = NULL;
    }
    
    currentThread = (struct thread*)malloc(sizeof(struct thread));
    currentThread->id = 0;
    currentThread->state = S_RUNNING;
    currentThread->next = NULL;
    currentThread->threadwq = wait_queue_create();
    currentThread->lock = NULL;
    threadAvail[0] = currentThread;
    
    rq = (struct queue*)malloc(sizeof(struct queue));
    rq->head = NULL;
    rq->tail = NULL;
    
    eq = (struct queue*)malloc(sizeof(struct queue));
    eq->head = NULL;
    eq->tail = NULL;
}

// Runs the function the thread is supposed to run, and the exits afterwards
void thread_stub(void (*fn) (void *), void *parg)
{
    interrupts_on(); // Interrupts are off in the ucontext saved when thread is created
    fn(parg);
    thread_exit();
}

Tid
thread_id()
{
    return currentThread->id;
}

Tid
thread_create(void (*fn) (void *), void *parg)
{
    int old = interrupts_set(0);
    // Find an available thread id
    int newID = 0;
    while(newID<THREAD_MAX_THREADS && threadAvail[newID]!=NULL) {
        newID++;
    }
    // Maximum number of threads reached
    if(newID==THREAD_MAX_THREADS) {
        interrupts_set(old);
        return THREAD_NOMORE;
    }
    
    // Make sure there is enough memory for the thread and its stack
    struct thread* newThread = (struct thread*)malloc(sizeof(struct thread));
    if(newThread==NULL) {
        interrupts_set(old);
        return THREAD_NOMEMORY;
    }
    
    void* stackPtr = malloc(THREAD_MIN_STACK);
    if(stackPtr==NULL) {
        free(newThread); // Free the allocated thread if there isn't memory for its stack
        interrupts_set(old);
        return THREAD_NOMEMORY;
    }
    
    newThread->threadwq = wait_queue_create();
    if(newThread->threadwq==NULL) {
        free(newThread);
        free(stackPtr);
        interrupts_set(old);
        return THREAD_NOMEMORY;
    }
    
    // Start initializing thread components
    threadAvail[newID] = newThread;
    newThread->id = newID;
    newThread->state = S_READY;
    newThread->next = NULL;
    newThread->lock = NULL;
    newThread->stackAddress = stackPtr;
    
    // Setting ucontext, start with the current context, interrupts are off here
    int err = getcontext(&(newThread->tcontext));
    assert(!err);
    
    // Update the registers
    // Start of instructions
    newThread->tcontext.uc_mcontext.gregs[REG_RIP] = (unsigned long)&thread_stub; 
    // Function to run
    newThread->tcontext.uc_mcontext.gregs[REG_RDI] = (unsigned long)fn;
    // Function parameters
    newThread->tcontext.uc_mcontext.gregs[REG_RSI] = (unsigned long)parg;
    // Top of stack, aligned to 16 bytes
    newThread->tcontext.uc_mcontext.gregs[REG_RSP] = (unsigned long)(stackPtr+THREAD_MIN_STACK-8);
    
    // Set stack pointer and stack size
    newThread->tcontext.uc_stack.ss_sp = stackPtr;
    newThread->tcontext.uc_stack.ss_size = THREAD_MIN_STACK-8;
    
    // Push the tread into the ready queue
    queue_push(rq,newThread);
    
    interrupts_set(old);
    return newThread->id;
}

Tid
thread_yield(Tid want_tid)
{
    int old = interrupts_set(0);
    // Check validity of want_tid
    if(want_tid<THREAD_SELF || want_tid>=THREAD_MAX_THREADS || 
       (want_tid>=0 && want_tid!=currentThread->id && !queue_ismember(rq,want_tid))) {
        interrupts_set(old);
        return THREAD_INVALID;
    } else if(want_tid==THREAD_ANY && queue_isempty(rq)) {
        interrupts_set(old);
        return THREAD_NONE;
    }
    
    struct thread* nextThread; // The thread to switch to
    volatile bool setcontext_called = false;
    int err,nextID;
    
    // Enqueue current thread and save context
    currentThread->state = S_READY;
    queue_push(rq,currentThread);
    err = getcontext(&(currentThread->tcontext)); // Interrupts are saved as off here
    assert(!err);
    
    // Switch context if not done so already
    if(!setcontext_called) {
        setcontext_called = true;
        
        // Find the next thread
        if(want_tid==THREAD_ANY) nextThread = queue_pop(rq);
        else if(want_tid==THREAD_SELF) nextThread = queue_find(rq,currentThread->id);
        else nextThread = queue_find(rq,want_tid);

        nextThread->state = S_RUNNING;
        nextID = nextThread->id;
        
        currentThread = nextThread;
        err = setcontext(&(currentThread->tcontext));
        assert(!err);
    }
    
    // Kill all of the exited threads
    while(!queue_isempty(eq)) {
        struct thread* exitedThread = queue_pop(eq);
        free(exitedThread->stackAddress);
        wait_queue_destroy(exitedThread->threadwq);
        free(exitedThread);
    }
    
    interrupts_set(old); // Turn on interrupts upon resuming threads
    return nextID;
}

void
thread_exit()
{
    int old = interrupts_set(0); // Should get turned on by next running thread
    
    // Wakeup all threads waiting on exiting thread
    thread_wakeup(currentThread->threadwq,1);
    
    struct thread* nextThread = queue_pop(rq); // Get the next thread to run
    
    if(nextThread==NULL) { // No more threads, exit
        interrupts_set(old); // In this case no next thread, so need to turn interrupts back on
        exit(0);
    }
    
    // Push the current thread into the exit queue and switch into the next one
    queue_push(eq,currentThread);
    currentThread->state = S_EXITED;
    threadAvail[currentThread->id] = NULL;
    
    nextThread->state = S_RUNNING;
    currentThread = nextThread;
    int err = setcontext(&(currentThread->tcontext));
    assert(!err);
}

Tid
thread_kill(Tid tid)
{
    int old = interrupts_set(0);
    // Look for the thread to kill (if it is the current running thread it
    // will return NULL anyways)
    struct thread* toKill = NULL;
    if(!queue_isempty(rq)) toKill = queue_find(rq,tid); // Look in ready queue
    else { // Look in all of the wait queues
        for(int i=0; i<THREAD_MAX_THREADS && toKill==NULL; i++) {
            if(threadAvail[i]!=NULL && !queue_isempty(threadAvail[i]->threadwq->wq)) {
                toKill = queue_find(threadAvail[i]->threadwq->wq,tid);
            }
        }
    }
    if(toKill==NULL) {
        interrupts_set(old);
        return THREAD_INVALID;
    }
    
    // Wakeup all threads waiting on exiting thread
    thread_wakeup(toKill->threadwq,1);
    
    // If thread exists, can kill it immediately since it is not being run
    threadAvail[tid] = NULL;
    free(toKill->stackAddress);
    wait_queue_destroy(toKill->threadwq);
    free(toKill);
    
    interrupts_set(old);
    return tid;
}

/*******************************************************************
 * Important: The rest of the code should be implemented in Lab 3. *
 *******************************************************************/

/* make sure to fill the wait_queue structure defined above */
struct wait_queue *
wait_queue_create()
{
    struct wait_queue *wq;

    wq = malloc(sizeof(struct wait_queue));
    assert(wq);

    wq->wq = (struct queue*)malloc(sizeof(struct queue));
    wq->wq->head = NULL;
    wq->wq->tail = NULL;

    return wq;
}

void
wait_queue_destroy(struct wait_queue *wq)
{
    int old = interrupts_set(0);
    // Ensure that the wait queue is empty
    assert(queue_isempty(wq->wq));
    
    free(wq->wq);
    free(wq);
    wq = NULL;
    interrupts_set(old);
}

Tid
thread_sleep(struct wait_queue *queue)
{
    int old = interrupts_set(0);
    if(queue==NULL) { // Invalid queue
        interrupts_set(old);
        return THREAD_INVALID; 
    } else if(queue_isempty(rq)) { // No threads available to run
        interrupts_set(old);
        return THREAD_NONE;
    }
    
    struct thread* nextThread; // Get the next thread to run
    volatile bool setcontext_called = false;
    int err,nextID;
    
    // Push the current thread into the wait queue and switch into the next one
    currentThread->state = S_BLOCKED;
    queue_push(queue->wq,currentThread);
    err = getcontext(&(currentThread->tcontext));
    assert(!err);
    
    // Switch context if not done so already
    if(!setcontext_called) {
        setcontext_called = true;

        nextThread = queue_pop(rq);
        nextThread->state = S_RUNNING;
        nextID = nextThread->id;
        
        currentThread = nextThread;
        err = setcontext(&(currentThread->tcontext));
        assert(!err);
    }
    
    // Kill all of the exited threads
    while(!queue_isempty(eq)) {
        struct thread* exitedThread = queue_pop(eq);
        free(exitedThread->stackAddress);
        wait_queue_destroy(exitedThread->threadwq);
        free(exitedThread);
    }
    
    interrupts_set(old);
    return nextID;
}

/* when the 'all' parameter is 1, wakeup all threads waiting in the queue.
 * returns whether a thread was woken up on not. */
int
thread_wakeup(struct wait_queue *queue, int all)
{
    int old = interrupts_set(0);
    if(queue==NULL || queue_isempty(queue->wq)) { // No threads to be woken up
        interrupts_set(old);
        return 0; 
    }
    
    int numWoken = 0; // Counts number of queues that were woken up
    
    // Move at least the first thread in the wait queue to the ready queue
    do {
        struct thread* toWakeup = queue_pop(queue->wq);
        toWakeup->state = S_READY;
        queue_push(rq,toWakeup);
        numWoken++;
    } while(!queue_isempty(queue->wq) && all==1); // Run until wait queue is empty if waking up all threads
    
    interrupts_set(old);
    return numWoken;
}

/* suspend current thread until Thread tid exits */
Tid
thread_wait(Tid tid)
{
    int old = interrupts_set(0);
    // Make sure thread to wait for exists and is not current thread
    if(tid==currentThread->id || tid<0 || tid>=THREAD_MAX_THREADS || threadAvail[tid]==NULL) {
        interrupts_set(old);
        return THREAD_INVALID;
    }
    
    thread_sleep(threadAvail[tid]->threadwq);

    interrupts_set(old);
    return tid;
}

struct lock {
    struct wait_queue* lockwq;
    int l;
};

struct lock *
lock_create()
{
    struct lock *lock;

    lock = malloc(sizeof(struct lock));
    assert(lock);
    
    lock->lockwq = wait_queue_create();
    lock->l = UNLOCKED;

    return lock;
}

void
lock_destroy(struct lock *lock)
{
    int old = interrupts_set(0);
    assert(lock->l == UNLOCKED);
    
    wait_queue_destroy(lock->lockwq);
    free(lock);
    interrupts_set(old);
}

void
lock_acquire(struct lock *lock)
{
    int old = interrupts_set(0);
    while(lock->l==LOCKED) {
        thread_sleep(lock->lockwq);
    }
    lock->l = LOCKED;
    currentThread->lock = lock; // Note which thread acquired the lock
    interrupts_set(old);
}

void
lock_release(struct lock *lock)
{
    int old = interrupts_set(0);
    // Make sure the current thread acquired the lock
    if(currentThread->lock==lock) {
        lock->l = UNLOCKED;
        currentThread->lock = NULL;
        thread_wakeup(lock->lockwq,1); // Wakeup all threads waiting on lock
    }
    interrupts_set(old);
}

struct cv {
    struct wait_queue* cvwq;
};

struct cv *
cv_create()
{
    struct cv *cv;

    cv = malloc(sizeof(struct cv));
    assert(cv);

    cv->cvwq = wait_queue_create();

    return cv;
}

void
cv_destroy(struct cv *cv)
{
    int old = interrupts_set(0);
    assert(queue_isempty(cv->cvwq->wq));

    wait_queue_destroy(cv->cvwq);
    free(cv);
    interrupts_set(old);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
    int old = interrupts_set(0);
    // Make sure the current thread acquired the lock
    if(currentThread->lock==lock) { 
        lock_release(lock); // Release lock
        thread_sleep(cv->cvwq);
        lock_acquire(lock); // Reacquire lock
    }
    interrupts_set(old);
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
    int old = interrupts_set(0);
    // Make sure the current thread acquired the lock
    if(currentThread->lock==lock) {
        thread_wakeup(cv->cvwq,0); // Waking up only 1 thread
    }
    interrupts_set(old);
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
    int old = interrupts_set(0);
    // Make sure the current thread acquired the lock
    if(currentThread->lock==lock) {
        thread_wakeup(cv->cvwq,1); // Waking up all threads
    }
    interrupts_set(old);
}
