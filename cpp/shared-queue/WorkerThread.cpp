
#include <pthread.h>
#include <stdlib.h>

#include <unistd.h>
#include <queue>
#include <iostream>

template<class T>
class WorkerThread
{
    typedef int (*WTCallback)(T *item); 

    pthread_mutex_t mutex;
    pthread_cond_t condition;

    std::deque<T *> queue;
	int timeout; // in seconds
    WTCallback theCallback;

private:

    int processQueue();

	static void *runThread(void *args);

public:

    WorkerThread(WTCallback theCallback, int timeout);

    int push(T *item);

    int start();
};

template<class T>
WorkerThread<T>::WorkerThread(WTCallback theCallback, int timeout)
{
    this->mutex = PTHREAD_MUTEX_INITIALIZER;
    this->condition = PTHREAD_COND_INITIALIZER;
    this->timeout = timeout;
    this->theCallback = theCallback;
}

template<class T>
int WorkerThread<T>::processQueue()
{

    while(true)
    {
        pthread_mutex_lock(&mutex);
        while (queue.size() == 0)
        {
            pthread_cond_wait(&condition, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        theCallback(queue.front());
        queue.pop_front();
    }
}

template<class T>
void *WorkerThread<T>::runThread(void *arg)
{
    WorkerThread *workerThread = (WorkerThread *) arg;
    workerThread->processQueue();
}


template<class T>
int WorkerThread<T>::push(T *item)
{

    pthread_mutex_lock(&mutex);

    queue.push_back(item);

    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&mutex);
}

template<class T>
int WorkerThread<T>::start()
{
   pthread_t tid;
   pthread_create(&tid, NULL, WorkerThread::runThread, this);
}

int callback(int *x)
{
    std::cout << "received " << *x << std::endl;
    std::cout << "sleeping  " << std::endl;
    sleep(5);
    std::cout << "woke up  " << std::endl;
}

int main(int argc, char *argv[])
{

    WorkerThread<int> *thread = new WorkerThread<int>(callback, 3);  

    thread->start();

    int i = 0;

    for (i=1; i< argc; i++)
    {
        int *x = new int;

        *x = atoi(argv[i]);

        std::cout<< "pushing " << *x << std::endl;

        thread->push(x);
    }

    sleep(5*argc +5);
}
