#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "CarQueue.h"

// Define structure to return with the traffic stats
typedef struct Data{
    int numCars;
    double avg;
    int max;
    int time;
} Stats;

/*
 * Description: Initiates stats in Stats structure with values of 0.
 * Parameters: Stats* stats
 */
void initiateStats(Stats* stats) {
    stats->numCars = 0;
    stats->avg = 0;
    stats->max = 0;
    stats->time = 0;
}

// Zips 2 stat structures together into a new struct called Zip
typedef struct Zip {
    Stats leftStats;
    Stats rightStats;
}Zip;

/*
 * Description: returns a randomly generated integer between 1 and 100
 */
int generateNumber() {
    return rand() % 100;
}

/*
 * Description: Generates a sequence of 500 iterations over which cars might randomly join the queue
 * Parameters: int* leftSequence, int* rightSequence, int joinsLeft, int joinsRight
 */
void sequenceGenerator(int* leftSequence, int* rightSequence, int joinsLeft, int joinsRight) {
    int i, prob;
    for (i = 0; i < 500; i++) {
        prob = generateNumber();
        if (prob <= joinsLeft) {
            leftSequence[i] = 1;
        }
        else {
            leftSequence[i] = 0;
        }
        if (prob <= joinsRight) {
            rightSequence[i] = 1;
        }
        else {
            rightSequence[i] = 0;
        }
    }
}

/*
 * Description: Runs one simulation based on the given parameters.
 * Parameters: int joinsLeft, int joinsRight, int timeLeft, int timeRight.
 * Return: A Zip with the simulation stats
 * */
Zip runOneSimulation(int joinsLeft, int joinsRight, int timeLeft, int timeRight){
    int leftSequence[500];
    int rightSequence[500];
    int Light = 0; //0 for left, 1 for right

    Stats rightStats;
    Stats leftStats;

    initiateStats(&rightStats);
    initiateStats(&leftStats);

    //Create queue
    Queue* lQueue, * rQueue;
    lQueue = CreateQueue();
    rQueue = CreateQueue();

    //Generate sequence
    sequenceGenerator(leftSequence, rightSequence, joinsLeft, joinsRight);

    //Variables
    int time = 0, timesince = 0, wait = 0;

    //Results
    int max = 0, cars = 0, timeTaken = 0;
    double avg = 0;

    while (time < 500 || !isEmpty(lQueue) || !isEmpty(rQueue)){
        //Change light
        //Left light
        if (Light == 0 && timesince == timeLeft){
            Light = 1;
            timesince = 0;
            //Right light
        } else if (Light == 1 && timesince == timeRight){
            Light = 0;
            timesince = 0;
        } else {
            timesince++;
            if (leftSequence[time] == 1  && time < 500){
                Enqueue(lQueue, time);
            }
            if (rightSequence[time] == 1  && time < 500){
                Enqueue(rQueue, time);
            }
            if (Light == 0){
                if (!isEmpty(lQueue)) {
                    wait = time - firstValue(lQueue);
                    Dequeue(lQueue);
                    leftStats.numCars++;
                    leftStats.avg += wait;
                    if (wait > leftStats.max) {
                        leftStats.max = wait;
                    }
                }
            }
            else {
                if (!isEmpty(rQueue)) {
                    wait = time - firstValue(rQueue);
                    Dequeue(rQueue);
                    rightStats.numCars++;
                    rightStats.avg += wait;
                    if (wait > rightStats.max) {
                        rightStats.max = wait;
                    }
                }
            }
        }
        time++;
        if (time < 500 || !isEmpty(lQueue)) {
            leftStats.time++;
        }
        if (time < 500 || !isEmpty(rQueue)) {
            rightStats.time++;
        }
    }
    leftStats.avg = leftStats.avg / leftStats.numCars;
    rightStats.avg = rightStats.avg / rightStats.numCars;
    leftStats.time -= 499;
    rightStats.time -= 499;

    //Queue deletion for memory management
    DeleteQueue(lQueue);
    DeleteQueue(rQueue);

    //Zipping of both structures together
    Zip zip;
    zip.leftStats = leftStats;
    zip.rightStats = rightStats;

    return zip;
}

/*
 * Description: Calculates the global stats of the simulation
 *  Parameters: Stats* stats, Stats* result
 */
void calculAverage(Stats* stats, Stats* result) {
    stats->numCars += result->numCars;
    stats->avg += (double)result->avg;
    stats->max += result->max;
    stats->time += result->time;
}

/*
 * Description: Prints the stats of the simulation to the stdout stream
 * Parameters: int JoinsLeft, intJoinsRight, int timeLeft, int timeRight, Stats leftAvgStats, Stats rightAvgStats
 */
void printSimulation(const int joinsLeft, const int joinsRight, const int timeLeft, const int timeRight, const Stats leftAvgStats, const Stats rightAvgStats) {
    printf("Parameter Values: \n");
    printf("  from left: \n");
    printf("    Traffic arrival rate: %d\n", joinsLeft);
    printf("    Traffic light period: %d\n", timeLeft);
    printf("  from right: \n");
    printf("    Traffic arrival rate: %d\n", joinsRight);
    printf("    Traffic light period: %d\n", timeRight);
    printf("Results (averaged over 100 runs):\n");
    printf("  from left: \n");
    printf("    Number of vehicles: %d\n", leftAvgStats.numCars);
    printf("    Average waiting time: %f\n", leftAvgStats.avg);
    printf("    Maximum waiting time: %d\n", leftAvgStats.max);
    printf("    Clearance time: %d\n", leftAvgStats.time);
    printf("  from right: \n");
    printf("    Number of vehicles: %d\n", rightAvgStats.numCars);
    printf("    Average waiting time: %f\n", rightAvgStats.avg);
    printf("    Maximum waiting time: %d\n", rightAvgStats.max);
    printf("    Clearance time: %d\n", rightAvgStats.time);
}

/*
 * Description: Runs 100 simulations
 * Parameters: int joinsLeft, int joinsRight, int timeLeft, int timeRight
 */
void runSimulations(const int joinsLeft, const int joinsRight, const int timeLeft, const int timeRight){
    Stats leftAvgStats;
    Stats rightAvgStats;

    initiateStats(&leftAvgStats);
    initiateStats(&rightAvgStats);

    for (int i =0; i<100;i++){
        Zip zip= runOneSimulation(joinsLeft, joinsRight, timeLeft, timeRight);
        calculAverage(&leftAvgStats, &(zip.leftStats));
        calculAverage(&rightAvgStats, &(zip.rightStats));
    }
    leftAvgStats.numCars /= 100;
    leftAvgStats.avg /= 100;
    leftAvgStats.max /= 100;
    leftAvgStats.time /= 100;

    rightAvgStats.numCars /= 100;
    rightAvgStats.avg /= 100;
    rightAvgStats.max /= 100;
    rightAvgStats.time /= 100;

    printSimulation(joinsLeft, joinsRight, timeLeft, timeRight, leftAvgStats, rightAvgStats);
}

int main() {
    srand(time(0));
    int left, right, timeLeft, timeRight;
    left = generateNumber();
    right = generateNumber();
    timeLeft = 74;
    timeRight = 26;
    runSimulations(left, right, timeLeft, timeRight);
}
