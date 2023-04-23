#include <stdlib.h>

#define Front queue->front
#define Rear queue->rear
#define qsize queue->size

//Define structs of node and queue
typedef struct Car {
    struct Car* next;
    int arrivalTime;
} Car;

typedef struct Queue {
    Car* front;
    Car* rear;
    int size;
} Queue;

//Define functions declared below.
Queue* CreateQueue();
Car* makeCar(const int arrivalTime);
int Enqueue(Queue* queue, const int arrivalTime);
int firstValue(Queue* queue);
int Dequeue(Queue* queue);
int isEmpty(Queue* queue);
int DeleteQueue(Queue* queue);

/*
  Description: Create new Queue
  Return: new Queue initialized with default values
  front = NULL, rear = NULL, size = 0
*/
Queue* CreateQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue != NULL) {
        Front = NULL;
        Rear = NULL;
        qsize = 0;
    }
    return queue;
}

/*
  Description: make a new node named car.
  Return: new car initialized with default values
  next = NULL
*/
Car* makeCar(const int arrivalTime) {
    Car* newCar = (Car*)malloc(sizeof(Car));
    if (newCar != NULL) {
        newCar->next = NULL;
        newCar->arrivalTime = arrivalTime;
    }
    return newCar;
}


/*
  Description: return the arrival time of the first element
  Parameters: Queue* queue, int arrivalTime
  Return: 0 if all process terminated successfully, -1 if queue is NULL, -2 if queue is empty
*/
int firstValue(Queue* queue) {
    if (queue != NULL) {
        if (!isEmpty(queue)) {
            return Front->arrivalTime;
        }
        else return -2;
    }
    else return -1;
}

/*
  Description: De-queue a car at the front of the queue.
  Parameters: Queue* queue
  Return: 0 if all process terminated successfully, -1 if queue is NULL
*/
int Dequeue(Queue* queue) {
    if (queue != NULL) {
        if (!isEmpty(queue)) {
            Car* deCar = Front;
            Front = deCar->next;
            free(deCar);
            qsize--;
            if (qsize == 0) {
                Front = NULL;
                Rear = NULL;
            }
            return 0;
        }
        else {
            return -2;
        }
    }
    else return -1;
}

/*
  Description: Check if the queue is empty
  Parameters: Queue* queue
  Return: 0 if queue is empty, 1 if queue is not empty, -1 if queue is NULL
*/
int isEmpty(Queue* queue) {
    if (queue != NULL) {
        return qsize == 0;
    }
    return -1;
}

/*
  Description: Delete all members of the queue as well as itself
  Parameters: Queue* queue
  Return: 0 if all process terminated successfully, -1 if queue is NULL
*/
int DeleteQueue(Queue* queue) {
    if (queue != NULL) {
        while (!isEmpty) {
            Dequeue(queue);
        }
        free(queue);
        return 0;
    }
    return -1;
}