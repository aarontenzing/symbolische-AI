#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.c" 


int randomNumber(int upperBound) {
    // random number between 0 and RAND_MAX
    double normalized_random = (double)rand() / RAND_MAX;

    // Scale the random number to the desired range
    int random_number = (int)(normalized_random * upperBound);

    return random_number;
}

int main() {

    Info *data = createInformation();
    readInfo(data);

    // information
    printf("Number of requests: %d\n", data->num_requests);
    printf("Number of zones: %d\n", data->num_zones);
    printf("Number of vehicles: %d\n", data->num_vehicles);
    printf("Number of days: %d\n", data->num_days);
    printf("\n");

    // Create the data structures
    RequestNode *head = readInput(createZones(data->num_zones));

    // Create vehicles
    int vehicles[data->num_vehicles];
    memset(vehicles, 0, sizeof(vehicles));

    // Create zones
    int zones[data->num_zones];
    memset(zones, 0, sizeof(zones));

    // Create requests (reservaties)
    int requests[data->num_requests];
    memset(requests, 0, sizeof(requests));

    // Array of length vehicles 
    // index == vehicle, value == zone
    // Assign a random zone to a vehicle.
    int rand_zone;
    for (int i = 0; i < data->num_vehicles; i++) {
        rand_zone = randomNumber(data->num_zones);
        printf("Vehicle %d assign to zone %d.\n", i, rand_zone); 
        vehicles[i] = rand_zone; // assign zone to vehicle
        zones[rand_zone] = i; // assign vehicle to zone
    }

    // Array van requests 
    // index == request, value == vehicle
    // Assign a appropriate vehicle to a request.
    RequestNode* req;
    for (int i = 0; i < data->num_requests; i++) {
        // request -> enkel toewijzen aan voertuig in eigen zone, aanliggende zone
        req = getItem(head, i);
        printf("req id: %d \n", req->data.id);
        int currVehicle = zones[req->data.zone_id];
        // check of current vehicle mogelijk is voor huidige zone??
        
    }
    
    return 0;
}


