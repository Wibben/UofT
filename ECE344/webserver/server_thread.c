#include "request.h"
#include "server_thread.h"
#include "common.h"
#include <pthread.h>
#include <stdbool.h>

static void file_data_free(struct file_data *data); // Prototypes for cache_evict
static struct file_data * file_data_init(void);

struct cacheNode {
    struct file_data* data;
    int userCount;
    
    struct cacheNode* next;
};

// Use a hash table for caching
struct fileCache {
    int size; // Size of hash table
    struct cacheNode** head;
};

// Used for an LRU list
struct LRU {
    struct cacheNode* dataNode;
    struct LRU* next;
};

struct server {
	int nr_threads;
	int max_requests;
	int max_cache_size;
	int exiting;
	
        pthread_t** worker;
        struct fileCache* cache;
        struct LRU* LRU; // List of least recently used files
        int cacheUsed;
        
        // Circular buffer
        int* buffer;
        int in,out;
        
        // Concurrency variables
        pthread_mutex_t* lock;
        pthread_mutex_t* cacheLock;
        pthread_cond_t* bufferFull;
        pthread_cond_t* bufferEmpty;
};

// Adds or moves a cache node to the end of the LRU list
void update_LRU(struct server* sv, struct cacheNode* dataNode)
{
    if(sv->LRU==NULL) {
        struct LRU* newLRU = (struct LRU *)malloc(sizeof(struct LRU));
        newLRU->dataNode = dataNode;
        newLRU->next = NULL;
        sv->LRU = newLRU;
        return;
    }
    
    // find and remove the LRU from the list and have current point to end of list
    struct LRU* current = sv->LRU;
    struct LRU* found = NULL;
    // Check first node
    if(current->dataNode==dataNode) {
        // Quick check for if there is only one node in the LRU list
        if(current->next==NULL) return; // Do nothing since end of the list is its current position
        
        found = current;
        current = current->next;
        sv->LRU = current;
        found->next = NULL;
    }
    
    // Check rest of list (if cache node was the first node in LRU this will just
    // traverse to the end of the list)
    while(current->next!=NULL) {
        // If found, remove from the list
        if(current->next->dataNode==dataNode) {
            found = current->next;
            current->next = current->next->next;
            found->next = NULL;
        } else current = current->next;
    }
    
    // If not found, make a new node and add the cache node
    // if found, append to end of list (after current)
    if(found==NULL) {
        struct LRU* newLRU = (struct LRU *)malloc(sizeof(struct LRU));
        newLRU->dataNode = dataNode;
        newLRU->next = NULL;
        current->next = newLRU;
    } else {
        current->next = found;
    }
}

/* Hash Table functions*/

// Using djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
int hash(struct server* sv, char* str)
{
    unsigned long hash = 5381;
    int c;
    
    while((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    
    return hash%(sv->cache->size);
}

struct fileCache* cache_init(long size)
{
    struct fileCache* cache;
    
    cache = (struct fileCache *)malloc(sizeof(struct fileCache));
    assert(cache);
    
    // Initialize hash table
    cache->size = size;
    cache->head = (struct cacheNode**)malloc(sizeof(struct cacheNode*)*cache->size);
    assert(cache->head);
    for(int i=0; i<cache->size; i++) { // Set all heads to point to NULL
        cache->head[i] = NULL;
    }
    
    return cache;
}

// Lookup a file in the cache
struct cacheNode* cache_lookup(struct server* sv, char* filename)
{
    int index = hash(sv,filename);
    struct cacheNode* current = sv->cache->head[index];
    
    // Look through files at current index and see if there's a match
    while(current!=NULL) {
        if(strcmp(current->data->file_name,filename)==0) {
            current->userCount++; // Note that this file is in use
            update_LRU(sv,current);
            return current; // Return the desired file
        }
        
        current = current->next;
    }
    
    // If it gets to here the file does not exist
    return NULL;
}

// Remove a single item from cache
void cache_remove(struct server* sv, struct cacheNode* dataNode)
{
    int index = hash(sv,dataNode->data->file_name);
    struct cacheNode* current = sv->cache->head[index];
    struct cacheNode* prev = NULL;
    while(current!=NULL) {
        if(current==dataNode) {
            if(prev==NULL) sv->cache->head[index] = current->next;
            else prev->next = current->next;
            
            dataNode->next = NULL;
            sv->cacheUsed -= dataNode->data->file_size; // Update used memory
            file_data_free(current->data);
            free(dataNode);
        }
        
        prev = current;
        current = current->next;
    }
}

// Evict files from the cache until amount of free memory reaches size
void cache_evict(struct server* sv, int size)
{
    struct LRU* current = sv->LRU;
    struct LRU* prev = NULL;
    // Evict until there is enough memory
    while(sv->max_cache_size-sv->cacheUsed < size && current!=NULL) {
        // Only evict if nothing is using the cached resource
        if(current->dataNode->userCount <= 0) {
            // Remove from LRU list and cache
            struct LRU* toEvict = current;
            if(prev==NULL) sv->LRU = current->next;
            else {
                prev->next = current->next;
                current = current->next;
            }
            
            toEvict->next = NULL;
            cache_remove(sv,toEvict->dataNode);
            free(toEvict);
        } else {
            prev = current;
            current = current->next;
        }
    }
}

// Insertion into cache
struct cacheNode* cache_add(struct server* sv, struct file_data* file)
{
    // Looks through the current files to see if it already is in cache
    int index = hash(sv,file->file_name);
    struct cacheNode* current = cache_lookup(sv,file->file_name);
    
    if(current!=NULL) return current; // If it is in cache return the node
    
    // Do cache eviction (if required)
    cache_evict(sv,file->file_size);
    if(sv->max_cache_size-sv->cacheUsed < file->file_size) return NULL; // Not enough memory, do not cache
    
    // Create a new node to insert into the cache
    struct cacheNode* newCache = (struct cacheNode*)malloc(sizeof(struct cacheNode));
    newCache->data = file;
    newCache->next = NULL;
    newCache->userCount = 1; // If caching, at least one user is using the file
    
    // Insert into cache hash table
    if(sv->cache->head[index]==NULL) {
        sv->cache->head[index] = newCache;
    } else {
        current = sv->cache->head[index];
        while(current->next!=NULL) {
            current = current->next;
        }
        current->next = newCache;
    }
    sv->cacheUsed += file->file_size; // Update amount of memory used
    update_LRU(sv,newCache);
    
    return newCache;
}

// Free the cache
void cache_destroy(struct fileCache* cache)
{
    // Go through the hash table and free every non-null item
    for(int i=0; i<cache->size; i++) {
        struct cacheNode* current = cache->head[i];
        while(current!=NULL) {
            struct cacheNode* toDelete = current;
            current = current->next;
            toDelete->next = NULL;
            file_data_free(toDelete->data);
            free(toDelete);
        }
        cache->head[i] = NULL;
    }
    free(cache->head);
    
    free(cache);
}

/* static functions */

/* initialize file data */
static struct file_data *
file_data_init(void)
{
	struct file_data *data;

	data = Malloc(sizeof(struct file_data));
	data->file_name = NULL;
	data->file_buf = NULL;
	data->file_size = 0;
	return data;
}

/* free all file data */
static void
file_data_free(struct file_data *data)
{
	free(data->file_name);
	free(data->file_buf);
	free(data);
}

static void
do_server_request(struct server *sv, int connfd)
{
    int ret;
    struct request *rq;
    struct file_data *data;

    data = file_data_init();

    /* fill data->file_name with name of the file being requested */
    rq = request_init(connfd, data);
    if (!rq) {
        file_data_free(data);
        return;
    }
    
    if(sv->max_cache_size>0) { // If there is a cache use it
        pthread_mutex_lock(sv->cacheLock);
        struct cacheNode* lookup = cache_lookup(sv,data->file_name);

        if(lookup==NULL) { // Cache miss
            pthread_mutex_unlock(sv->cacheLock);
            /* read file, 
             * fills data->file_buf with the file contents,
             * data->file_size with file size. */
            ret = request_readfile(rq);
            if(ret==0) goto out; // Couldn't read file
            pthread_mutex_lock(sv->cacheLock);
            
            lookup = cache_add(sv,data);
        } else { // Cache hit
            // Load in the file and send it off
            data->file_buf = zlookup->data->file_buf;
            data->file_size = lookup->data->file_size;
            lookup->userCount++; // Note that this file is in use
            pthread_mutex_unlock(sv->cacheLock);
        }
        pthread_mutex_unlock(sv->cacheLock);
        
        // Send file to client
        request_sendfile(rq);

        // File is done being used
        pthread_mutex_lock(sv->cacheLock);
        // File is no longer in use by this user, only necessary if it was cached
        if(lookup!=NULL) lookup->userCount--; 
        pthread_mutex_unlock(sv->cacheLock);
    } else { // If no cache do what happened previously
        /* read file, 
         * fills data->file_buf with the file contents,
         * data->file_size with file size. */
        ret = request_readfile(rq);
        if (ret == 0) { /* couldn't read file */
            goto out;
        }
        /* send file to client */
        request_sendfile(rq);
    }
out:
    request_destroy(rq);
    if(sv->max_cache_size==0 || data->file_size>sv->max_cache_size) file_data_free(data);
}

/* entry point functions */

void worker_loop(void * sv_in)
{
    struct server* sv = (struct server*) sv_in;
    
    // Worker will keep doing this until exit has been called
    while(sv->exiting==0) {
        pthread_mutex_lock(sv->lock);
        // Empty buffer (also server is not exiting))
        while(sv->in==sv->out && sv->exiting==0) {
            pthread_cond_wait(sv->bufferEmpty,sv->lock);
        }
        
        // Get connfd from buffer
        int connfd = sv->buffer[sv->out];
        sv->buffer[sv->out] = 0;
        sv->out = (sv->out+1)%sv->max_requests;
        
        pthread_cond_signal(sv->bufferFull); // Signal requests waiting on buffer slots
        
        pthread_mutex_unlock(sv->lock);
        
        if(sv->exiting==0) do_server_request(sv,connfd);
    }
}

struct server *
server_init(int nr_threads, int max_requests, int max_cache_size)
{
    struct server *sv;

    sv = Malloc(sizeof(struct server));
    sv->nr_threads = nr_threads;
    sv->max_requests = max_requests+1;
    sv->max_cache_size = max_cache_size;
    sv->exiting = 0;
    
    sv->LRU = NULL;
    
    sv->lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    sv->cacheLock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    sv->bufferFull = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    sv->bufferEmpty = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    
    pthread_mutex_init(sv->lock,NULL);
    pthread_mutex_init(sv->cacheLock,NULL);
    pthread_cond_init(sv->bufferFull,NULL);
    pthread_cond_init(sv->bufferEmpty,NULL);

    if (nr_threads > 0 || max_requests > 0 || max_cache_size > 0) {
        // Creating buffer for requests
        if(max_requests>0) {
            sv->in = sv->out = 0;
            sv->buffer = malloc(sizeof(int) * (max_requests+1));
        }
        
        // Creating cache for files
        if(max_cache_size > 0) {
            sv->cache = cache_init(max_cache_size/(12288)+1); // 12288 is expected mean file size
            sv->cacheUsed = 0;
        }
        
        // Creating worker threads
        sv->worker = (pthread_t **)malloc(sizeof(pthread_t *) * nr_threads);
        for(int i=0; i<nr_threads; i++) {
            sv->worker[i] = (pthread_t *)malloc(sizeof(pthread_t));
            pthread_create(sv->worker[i],NULL,(void *)&worker_loop,(void *)sv);
        }
    }

    return sv;
}

void
server_request(struct server *sv, int connfd)
{
    if (sv->nr_threads == 0) { /* no worker threads */
        do_server_request(sv, connfd);
    } else {
        /*  Save the relevant info in a buffer and have one of the
         *  worker threads do the work. */
        pthread_mutex_lock(sv->lock);
        // Full buffer (also server is not exiting))
        while((sv->in-sv->out + sv->max_requests)%sv->max_requests == sv->max_requests-1 && sv->exiting==0) {
            pthread_cond_wait(sv->bufferFull,sv->lock);
        }
        
        // Add to buffer
        sv->buffer[sv->in] = connfd;
        sv->in = (sv->in+1)%sv->max_requests;
        
        pthread_cond_signal(sv->bufferEmpty); // Signal workers waiting on requests
        
        pthread_mutex_unlock(sv->lock);
    }
}

void
server_exit(struct server *sv)
{
    /* when using one or more worker threads, use sv->exiting to indicate to
     * these threads that the server is exiting. make sure to call
     * pthread_join in this function so that the main server thread waits
     * for all the worker threads to exit before exiting. */
    sv->exiting = 1;

    pthread_cond_broadcast(sv->bufferEmpty);
    pthread_cond_broadcast(sv->bufferFull);
    // Wait for all threads to join
    for(int i=0; i<sv->nr_threads; i++) {
        pthread_join(*(sv->worker[i]),NULL);
    }

    // Freeing all allocated resources
    if(sv->max_cache_size>0) {
        cache_evict(sv,sv->max_cache_size);
        cache_destroy(sv->cache);
    }
    for(int i=0; i<sv->nr_threads; i++) {
        free(sv->worker[i]);
    }
    free(sv->worker);
    free(sv->buffer);
    free(sv->lock);
    free(sv->cacheLock);
    free(sv->bufferEmpty);
    free(sv->bufferFull);
    free(sv);
}
