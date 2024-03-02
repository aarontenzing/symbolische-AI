#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prog.h"

// Global variables
Zone zones[MAX];
char vehicles[MAX];
Info data;

RequestNode* createRequest(Request requestData) {

    RequestNode* newRequest = (RequestNode*) malloc(sizeof(RequestNode));
    if (newRequest == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newRequest->data = requestData;
    newRequest->next = NULL;
    return newRequest;
}


RequestNode* appendRequest(RequestNode *head, RequestNode *newNode) {
    
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return head; // Return the original head if memory allocation fails
    }

    // If the list is empty, make the new node the head
    if (head == NULL) {
        return newNode;
    }

    RequestNode *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Append the new node to the end of the list
    current->next = newNode;
    return head;



}


void printRequest(RequestNode *head) {
    RequestNode *current = head;
    while (current != NULL) {
        printf("%s %s %d %d %d %s %d %d\n", 
            current->data.id, current->data.zone_id, current->data.day, 
            current->data.start_time, current->data.duration, current->data.vehicles, 
            current->data.penalty1, current->data.penalty2);
        current = current->next;
    }
}


RequestNode* readInput() {

    FILE *file = fopen("..\\input\\360_5_71_25.csv", "r");

    if (file == NULL) {
        printf("Error: file not found\n");
        return NULL;
    } 
    
    // Read the number of requests on the first line
    fscanf(file, "+Requests: %d", &data.num_requests);

    // Read all the requests
    char line[LINE_LENGTH]; 
    Request request;
    RequestNode* head = NULL;

    while (fgets(line, LINE_LENGTH, file) != NULL) {

        if (strstr(line, "+Zones:") != NULL) {
            sscanf(line, "+Zones: %d", &data.num_zones);
            break;
        }

        sscanf(line, "%[^;];%[^;];%d;%d;%d;%[^;];%d;%d", 
            request.id, request.zone_id, &request.day, &request.start_time, 
            &request.duration, request.vehicles, &request.penalty1, &request.penalty2);

        RequestNode* newRequest = createRequest(request);
        head = appendRequest(head, newRequest);
    }

    // Read the zones
    int i = 0;
    while (fgets(line, LINE_LENGTH, file) != NULL) {

        if (strstr(line, "+Vehicles:") != NULL) {
            sscanf(line, "+Vehicles: %d", &data.num_vehicles);
            break;
        }
        
        sscanf(line, "%[^;];%[^\n]", zones[i].id, zones[i].adj_zones);
        i++;
    }

    // Read the vehicles
    int j = 0;
    while (fgets(line, LINE_LENGTH, file) != NULL) {

        if (strstr(line, "+Days:") != NULL) {
            sscanf(line, "+Days: %d", &data.num_days);
            break;
        }

        sscanf(line, "%c", &vehicles[j]);
        j++;
    }

    fclose(file);
    return head;
}

int main() {

    
    RequestNode *head = readInput(&data);
    //printRequest(head);
    printf("Number of requests: %d\n", data.num_requests); 
    printf("Number of zones: %d\n", data.num_zones);

    for (int i = 0; i < data.num_zones; i++) {
        printf("%s %s\n", zones[i].id, zones[i].adj_zones);
    }
    
    return 0;
}


