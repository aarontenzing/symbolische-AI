#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.c" 


int randomNumber(int upperBound) {

    // srand(2020);
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
    Zone* zones = createZones(data->num_zones); 
    RequestNode *head = readInput(zones);
    // printf("werkt dit? zones[%d].id: %s\n", 5, zones[5].adj_zones);
    // printf("werkt dit? zones[%d].voertuigen[0]: %d\n", 1, zones[1].voertuigen[0]);
    

    // Create vehicles
    int vehicles[data->num_vehicles];
    memset(vehicles, 0, sizeof(vehicles));

    // Create zones
    //int zones[data->num_zones];
    //memset(zones, 0, sizeof(zones));


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
        //zones[rand_zone] = i; // assign vehicle to zone
        int j = 0;
        while(zones[rand_zone].voertuigen[j] != 0){
            j++;
        }
        zones[rand_zone].voertuigen[j] = i;
    }

    

    for (int i = 0; i < data->num_zones; i++) {
        for (int j = 0; j < data->num_vehicles; j++) {
            
            printf("zone %d: %d\n",i,zones[i].voertuigen[j]);

        }
    }


    // char * save2;
    // cars = strtok_r(cars,",",&save2);
    // while(cars != NULL){
    //     int k;
    //     sscanf(cars,"%3s%d",&k);
    //     cars = strtok_r(NULL,",",&save2);

    // }

    // Array van requests 
    // index == request, value == vehicle
    // Assign a appropriate vehicle to a request.
    RequestNode* req;
    for (int i = 0; i < data->num_requests; i++) {
        // request -> enkel toewijzen aan voertuig in eigen zone, aanliggende zone
        req = getItem(head, i);
        printf("req id: %d \n", req->data.id);
        //int currVehicle = zones[req->data.zone_id];
        // check of current vehicle mogelijk is voor huidige zone??
        
    }
    
    return 0;
}


