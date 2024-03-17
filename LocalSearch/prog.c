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

    int currVehicle;
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
    // const int SOURCE[MAX] = {1,4,5,2,3,0};
    // memcpy(vehicles, SOURCE, sizeof(vehicles));
    int availvehic[data->num_vehicles];
    memset(availvehic, -1, sizeof(availvehic));


    // Create adjacent zones
    int adjzones[data->num_zones];
    memset(zones, -1, sizeof(zones));


    // Create requests (reservaties)
    int requests[data->num_requests];
    memset(requests, -1, sizeof(requests));

    // Array of length vehicles 
    // index == vehicle, value == zone
    // Assign a random zone to a vehicle.
    int rand_zone;
    for (int i = 0; i < data->num_vehicles; i++) {
        rand_zone = randomNumber(data->num_zones);
        printf("Vehicle %d assign to zone %d.\n", i, rand_zone); 
        vehicles[i] = rand_zone; // assign zone to vehicle
        //assign vehicle to zone
        int j = 0;
        while(zones[rand_zone].voertuigen[j] != -1){
            j++;
        }
        zones[rand_zone].voertuigen[j] = i;
    }

    

    // for (int i = 0; i < data->num_zones; i++) {
    //     for (int j = 0; j < data->num_vehicles; j++) {
            
    //         printf("zone %d: %d\n",i,zones[i].voertuigen[j]);

    //     }
    // }

    // Array van requests 
    // index == request, value == vehicle
    // Assign a appropriate vehicle to a request.
    RequestNode* req;
    for (int i = 0; i < data->num_requests; i++) {
        // request -> enkel toewijzen aan voertuig in eigen zone, aanliggende zone
        req = getItem(head, i);
        // printf("req id: %d \n", req->data.id);
        //int currVehicle = zones[req->data.zone_id];
        // check of current vehicle mogelijk is voor huidige zone??
        
        // lijst met beschikbare voertuigen op die zone
        char *save;
        char* car;
        car = strtok_r(req->data.vehicles,",",&save);
        int k = 0;
        memset(availvehic, -1, sizeof(availvehic));
        while(car != NULL){
            
            sscanf(car,"%*3s%d",&availvehic[k]);
            car = strtok_r(NULL,",",&save);
            k++;
        }
        // for (int j = 0; j < data->num_vehicles; j++) {
        //     printf("available vehicles : %d\n",availvehic[j]);
        // }

        //staat er 1 van de auto's in die zone in de zones struct
        for (int j = 0; j < data->num_vehicles; j++) {
            
            currVehicle = zones[req->data.zone_id].voertuigen[j];

            if(currVehicle == -1){
                    continue;
            }

            for (int l = 0; l < data->num_vehicles; l++) {
            
                if(availvehic[l] == -1){
                    continue;
                }
                if(currVehicle == availvehic[l]){
                    requests[i] = currVehicle;
                    printf("req%d: car%d\n",i,requests[i]);
                    
                }
                
            }
            
        }

        // kijken of we auto's van aanliggende zones kunnen toewijzen
        // if(requests[i]==-1){
        //         int n = 0;
        //         while(zones[req->data.zone_id].adj_zones[n] != -1){
        //             int loc = zones[req->data.zone_id].adj_zones[n];

                    
        //         }
        //     }
        
    }
    
    return 0;
}


