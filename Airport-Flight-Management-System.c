#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10
#define RUNWAYS 3

/* ---------- Flight Structure ---------- */

struct Flight {
    int flightNo;
    char name[30];
    char operation[15];
};

/* ---------- Queue ---------- */

struct Queue {
    struct Flight data[MAX];
    int front;
    int rear;
};

/* ---------- Priority Queue ---------- */

struct PriorityFlight {
    struct Flight data;
    int priority;
};

/* ---------- Linked List ---------- */

struct Node {
    struct Flight data;
    struct Node *next;
};

/* ---------- Global Variables ---------- */

struct Queue normalQueue = {{}, -1, -1};

struct PriorityFlight priorityQueue[MAX];
int prioritySize = 0;

struct Node *flightRecords = NULL;

/* Array for runway status
   0 = Free
   1 = Occupied */
int runwayStatus[RUNWAYS] = {0, 0, 0};


/* ---------- Queue Functions ---------- */

void enqueue(struct Flight flight) {

    if (normalQueue.rear == MAX - 1) {
        printf("Normal queue is full.\n");
        return;
    }

    if (normalQueue.front == -1) {
        normalQueue.front = 0;
    }

    normalQueue.rear++;
    normalQueue.data[normalQueue.rear] = flight;

    printf("Flight added to normal queue.\n");
}


struct Flight dequeue() {

    struct Flight flight = {0, "", ""};

    if (normalQueue.front == -1 ||
        normalQueue.front > normalQueue.rear) {

        return flight;
    }

    flight = normalQueue.data[normalQueue.front];
    normalQueue.front++;

    return flight;
}


/* ---------- Priority Queue Functions ---------- */

void insertPriority(struct Flight flight, int priority) {

    int i;

    if (prioritySize == MAX) {
        printf("Priority queue is full.\n");
        return;
    }

    i = prioritySize - 1;

    while (i >= 0 &&
           priorityQueue[i].priority < priority) {

        priorityQueue[i + 1] = priorityQueue[i];
        i--;
    }

    priorityQueue[i + 1].data = flight;
    priorityQueue[i + 1].priority = priority;

    prioritySize++;

    printf("Emergency flight added to priority queue.\n");
}


struct Flight removePriority() {

    struct Flight flight = {0, "", ""};
    int i;

    if (prioritySize == 0) {
        return flight;
    }

    flight = priorityQueue[0].data;

    for (i = 0; i < prioritySize - 1; i++) {
        priorityQueue[i] = priorityQueue[i + 1];
    }

    prioritySize--;

    return flight;
}


/* ---------- Linked List Functions ---------- */

void addRecord(struct Flight flight) {

    struct Node *newNode;

    newNode = (struct Node *)malloc(sizeof(struct Node));

    newNode->data = flight;
    newNode->next = flightRecords;

    flightRecords = newNode;
}


void displayRecords() {

    struct Node *temp;

    printf("\n---------- Flight Records ----------\n");

    if (flightRecords == NULL) {
        printf("No completed flights.\n");
        return;
    }

    temp = flightRecords;

    while (temp != NULL) {

        printf("Flight No: %d\n", temp->data.flightNo);
        printf("Flight Name: %s\n", temp->data.name);
        printf("Operation: %s\n\n", temp->data.operation);

        temp = temp->next;
    }
}


/* ---------- Add Flight ---------- */

void addFlight() {

    struct Flight flight;

    printf("\nEnter Flight Number: ");
    scanf("%d", &flight.flightNo);

    printf("Enter Flight Name: ");
    scanf("%s", flight.name);

    strcpy(flight.operation, "Normal");

    enqueue(flight);
}


/* ---------- Request Landing ---------- */

void requestLanding() {

    struct Flight flight;

    printf("\nEnter Flight Number: ");
    scanf("%d", &flight.flightNo);

    printf("Enter Flight Name: ");
    scanf("%s", flight.name);

    strcpy(flight.operation, "Landing");

    enqueue(flight);
}


/* ---------- Request Takeoff ---------- */

void requestTakeoff() {

    struct Flight flight;

    printf("\nEnter Flight Number: ");
    scanf("%d", &flight.flightNo);

    printf("Enter Flight Name: ");
    scanf("%s", flight.name);

    strcpy(flight.operation, "Takeoff");

    enqueue(flight);
}


/* ---------- Emergency Landing ---------- */

void emergencyLanding() {

    struct Flight flight;

    printf("\nEnter Flight Number: ");
    scanf("%d", &flight.flightNo);

    printf("Enter Flight Name: ");
    scanf("%s", flight.name);

    strcpy(flight.operation, "Emergency");

    insertPriority(flight, 10);
}


/* ---------- Process Next Flight ---------- */

void processNextFlight() {

    struct Flight flight;
    int i;

    /* Emergency flight gets highest priority */

    if (prioritySize > 0) {
        flight = removePriority();
    } else {
        flight = dequeue();
    }

    if (flight.flightNo == 0) {
        printf("\nNo flights waiting.\n");
        return;
    }

    /* Find a free runway */

    for (i = 0; i < RUNWAYS; i++) {

        if (runwayStatus[i] == 0) {

            runwayStatus[i] = 1;

            printf("\n---------- Flight Processed ----------\n");
            printf("Flight Number: %d\n", flight.flightNo);
            printf("Flight Name: %s\n", flight.name);
            printf("Operation: %s\n", flight.operation);
            printf("Runway: %d\n", i + 1);

            /* Store flight in linked list */

            addRecord(flight);

            return;
        }
    }

    printf("\nAll runways are occupied.\n");
}


/* ---------- Display Waiting Flights ---------- */

void displayWaitingFlights() {

    int i;

    printf("\n---------- Emergency Flights ----------\n");

    if (prioritySize == 0) {
        printf("No emergency flights.\n");
    } else {

        for (i = 0; i < prioritySize; i++) {

            printf("Flight %d - %s - Priority %d\n",
                   priorityQueue[i].data.flightNo,
                   priorityQueue[i].data.name,
                   priorityQueue[i].priority);
        }
    }

    printf("\n---------- Normal Flights ----------\n");

    if (normalQueue.front == -1 ||
        normalQueue.front > normalQueue.rear) {

        printf("No normal flights.\n");
    } else {

        for (i = normalQueue.front;
             i <= normalQueue.rear;
             i++) {

            printf("Flight %d - %s - %s\n",
                   normalQueue.data[i].flightNo,
                   normalQueue.data[i].name,
                   normalQueue.data[i].operation);
        }
    }
}


/* ---------- Search Flight ---------- */

void searchFlight() {

    int number;
    int i;
    struct Node *temp;

    printf("\nEnter Flight Number: ");
    scanf("%d", &number);

    /* Search normal queue */

    for (i = normalQueue.front;
         i <= normalQueue.rear;
         i++) {

        if (normalQueue.data[i].flightNo == number) {

            printf("Flight found in normal queue.\n");
            return;
        }
    }

    /* Search priority queue */

    for (i = 0; i < prioritySize; i++) {

        if (priorityQueue[i].data.flightNo == number) {

            printf("Flight found in priority queue.\n");
            return;
        }
    }

    /* Search linked list */

    temp = flightRecords;

    while (temp != NULL) {

        if (temp->data.flightNo == number) {

            printf("Flight found in completed records.\n");
            return;
        }

        temp = temp->next;
    }

    printf("Flight not found.\n");
}


/* ---------- Cancel Flight ---------- */

void cancelFlight() {

    int number;
    int i;

    printf("\nEnter Flight Number to Cancel: ");
    scanf("%d", &number);

    /* Cancel from normal queue */

    for (i = normalQueue.front;
         i <= normalQueue.rear;
         i++) {

        if (normalQueue.data[i].flightNo == number) {

            for (; i < normalQueue.rear; i++) {
                normalQueue.data[i] =
                    normalQueue.data[i + 1];
            }

            normalQueue.rear--;

            printf("Flight cancelled successfully.\n");
            return;
        }
    }

    /* Cancel from priority queue */

    for (i = 0; i < prioritySize; i++) {

        if (priorityQueue[i].data.flightNo == number) {

            for (; i < prioritySize - 1; i++) {
                priorityQueue[i] =
                    priorityQueue[i + 1];
            }

            prioritySize--;

            printf("Flight cancelled successfully.\n");
            return;
        }
    }

    printf("Flight not found.\n");
}


/* ---------- Airport Status ---------- */

void airportStatus() {

    int i;

    printf("\n---------- Airport Status ----------\n");

    for (i = 0; i < RUNWAYS; i++) {

        if (runwayStatus[i] == 0) {
            printf("Runway %d: FREE\n", i + 1);
        } else {
            printf("Runway %d: OCCUPIED\n", i + 1);
        }
    }

    printf("\nWaiting Emergency Flights: %d\n",
           prioritySize);

    if (normalQueue.front == -1 ||
        normalQueue.front > normalQueue.rear) {

        printf("Waiting Normal Flights: 0\n");
    } else {

        printf("Waiting Normal Flights: %d\n",
               normalQueue.rear -
               normalQueue.front + 1);
    }
}


/* ---------- Main Function ---------- */

int main() {

    int choice;

    do {

        printf("\n\n========== AIRPORT MANAGEMENT SYSTEM ==========\n");

        printf("1. Add Flight\n");
        printf("2. Request Landing\t");
        printf("3. Request Takeoff\t");
        printf("4. Emergency Landing\t");
        printf("5. Process Next Flight\t");
        printf("6. Display Waiting Flights\t");
        printf("7. Search Flight\t");
        printf("8. Display Completed Flights\t");
        printf("9. Cancel Flight\t");
        printf("10. Airport Status\t");
        printf("11. Exit\t");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addFlight();
                break;

            case 2:
                requestLanding();
                break;

            case 3:
                requestTakeoff();
                break;

            case 4:
                emergencyLanding();
                break;

            case 5:
                processNextFlight();
                break;

            case 6:
                displayWaitingFlights();
                break;

            case 7:
                searchFlight();
                break;

            case 8:
                displayRecords();
                break;

            case 9:
                cancelFlight();
                break;

            case 10:
                airportStatus();
                break;

            case 11:
                printf("\nExiting Airport Management System...\n");
                break;

            default:
                printf("\nInvalid choice.\n");
        }

    } while (choice != 11);

    return 0;
}