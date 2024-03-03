#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prog.h"


Info* createInformation() {

    Info *data = (Info *)malloc(sizeof(Info));
    if (data == NULL) {
        printf("Memory allocation failed for Data.\n");
        exit(EXIT_FAILURE);
    }
    // Initialize all members to zero or default values
    data->num_requests = 0;
    data->num_zones = 0;
    data->num_vehicles = 0;
    data->num_days = 0;
    return data;
}


Zone* createZones(int num_zones) {
    Zone *zones = (Zone *)malloc(num_zones * sizeof(Zone));
    if (zones == NULL) {
        printf("Memory allocation failed for Zones.\n");
        exit(EXIT_FAILURE);
    }
    zones->id[0] = '\0';
    zones->adj_zones[0] = '\0';
    return zones;
}


Vechicle* createVechicles(int num_vehicles) {
    Vechicle *vechicles =  (Vechicle *)malloc(num_vehicles * sizeof(Vechicle));
    if (vechicles == NULL) {
        printf("Memory allocation failed for Vechicles.\n");
        exit(EXIT_FAILURE);
    }
    vechicles->id[0] = '\0';
    return vechicles;
}


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


void readInfo(Info *data) {

    FILE *file = fopen("..\\input\\360_5_71_25.csv", "r");
    fscanf(file, "+Requests: %d", &data->num_requests);

    char line[LINE_LENGTH]; 

    while (fgets(line, LINE_LENGTH, file) != NULL) {
        if (strstr(line, "+Zones:") != NULL) {
            sscanf(line, "+Zones: %d", &data->num_zones);
            break;
        }
    }

    while (fgets(line, LINE_LENGTH, file) != NULL) {
        if (strstr(line, "+Vehicles:") != NULL) {
            sscanf(line, "+Vehicles: %d", &data->num_vehicles);
            break;
        }
    }

    while (fgets(line, LINE_LENGTH, file) != NULL) {
        if (strstr(line, "+Days:") != NULL) {
            sscanf(line, "+Days: %d", &data->num_days);
            break;
        }
    }

    fclose(file);
   
}


RequestNode* readInput(Zone *zones, Vechicle *vehicles) {

    FILE *file = fopen("..\\input\\360_5_71_25.csv", "r");

    if (file == NULL) {
        printf("Error: file not found\n");
        return NULL;
    } 
    
    // Read the number of requests on the first line
    char line[LINE_LENGTH]; 
    fgets(line, LINE_LENGTH, file);
    memset(line, '\0', LINE_LENGTH);

    // Read all the requests
    Request request;
    RequestNode* head = NULL;

    while (fgets(line, LINE_LENGTH, file) != NULL && strstr(line, "+Zones:") != NULL) {

        sscanf(line, "%[^;];%[^;];%d;%d;%d;%[^;];%d;%d", 
            request.id, request.zone_id, &request.day, &request.start_time, 
            &request.duration, request.vehicles, &request.penalty1, &request.penalty2);

        RequestNode* newRequest = createRequest(request);
        head = appendRequest(head, newRequest);
    }

    // Read the zones
    int i = 0;
    while (fgets(line, LINE_LENGTH, file) != NULL && strstr(line, "+Vehicles:") != NULL) {

        sscanf(line, "%[^;];%[^\n]", zones[i].id, zones[i].adj_zones); 
        //printf("zones[%d].id: %s\n", i, zones[i].adj_zones);
        i++;
    }

    // Read the vehicles
    i = 0;
    while (fgets(line, LINE_LENGTH, file) != NULL && strstr(line, "+Days:") != NULL) {
        
        sscanf(line, "%[^\n]", vehicles[i].id);
        //printf("vehicles[%d]: %s\n", i, vehicles[i].id);
        i++;
    }

    fclose(file);
    return head;
}

int main() {

    Info *data = createInformation();
    readInfo(data);
    

    // Create the data structures
    Zone *zones = createZones(data->num_zones);
    Vechicle *vehicles = createVechicles(data->num_vehicles);

    RequestNode *head = readInput(zones, vehicles);

    printf("zones[%d].id: %s\n", 0, zones[0].adj_zones);
    printf("vehicles[%d]: %s\n", 0, vehicles[0].id);

    //printf("Penalty 1: %d 2:%d\n", head->next->data.penalty1, head->next->data.penalty2);

    // information
    printf("Number of requests: %d\n", data->num_requests);
    printf("Number of zones: %d\n", data->num_zones);
    printf("Number of vehicles: %d\n", data->num_vehicles);
    printf("Number of days: %d\n", data->num_days);

    //printRequest(head->next);

    /*for (int i = 0; i < data->num_zones; i++) {
        printf("%s %s\n", zones[i].id, zones[i].adj_zones);
    }*/
    
    return 0;
}


