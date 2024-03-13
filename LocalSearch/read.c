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
    zones->id = 0;
    zones->adj_zones[0] = '\0';
    return zones;
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

    FILE *file = fopen("360_5_71_25.csv", "r");
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


RequestNode* readInput(Zone *zones) {

    FILE *file = fopen("360_5_71_25.csv", "r");

    if (file == NULL) {
        printf("Error: file not found\n");
        return NULL;
    } 
    
    // Read the number of requests on the first line
    char line[LINE_LENGTH]; 
    fgets(line, LINE_LENGTH, file);

    // Read all the requests
    Request request;
    RequestNode* head = NULL;
    
    while (fgets(line, LINE_LENGTH, file) != NULL) {
        
        if (strstr(line, "+Zones:") != NULL) {
            break;
        }
        sscanf(line, "req%d;z%d;%d;%d;%d;%[^;];%d;%d", 
            &request.id, &request.zone_id, &request.day, &request.start_time, 
            &request.duration, request.vehicles, &request.penalty1, &request.penalty2);

        RequestNode* newRequest = createRequest(request);
        head = appendRequest(head, newRequest);
    }

    // Read the zones
    int i = 0;
    while (fgets(line, LINE_LENGTH, file) != NULL) {

        if (strstr(line, "+Vehicles:") != NULL) {
            break;
        }
        sscanf(line, "z%d;%[^\n]", &zones[i].id, zones[i].adj_zones); 
        // printf("zones[%d].id: %s\n", i, zones[i].adj_zones);
        i++;
    }

    fclose(file);
    return head;
}


RequestNode* getItem(RequestNode* head, int index) {
    RequestNode* curr = head;
    int i = 0;
    
    while (i < index) {
        if (curr->next == NULL) {
            printf("ERROR");
            return NULL;
        }
        curr = curr->next;
        i++;
    }

    return curr;
}