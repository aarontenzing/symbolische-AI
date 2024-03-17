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

int assign(int id,Zone* zones,int size, int availvehic[size],int request){

    // for (int j = 0; j < data->num_vehicles; j++) {
    for (int j = 0; j < size; j++) {
            
        int currVehicle = zones[id].voertuigen[j];

        if(currVehicle == -1){
                continue;
        }

        for (int l = 0; l < size; l++) {
        
            if(availvehic[l] == -1){
                continue;
            }
            if(currVehicle == availvehic[l]){
                request = currVehicle;
                // printf("req\n");
                
            }
            
        }
            
    }
    return request;

}


int main() {
    int totalCost = 0;
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

    // Create vehicles
    int vehicles[data->num_vehicles];
    memset(vehicles, 0, sizeof(vehicles));
    int availvehic[data->num_vehicles];
    memset(availvehic, -1, sizeof(availvehic));

    // Create adjacent zones
    int adjzones[data->num_zones];
    memset(adjzones, -1, sizeof(adjzones));

    // Create requests (reservaties)
    int requests[data->num_requests];
    memset(requests, -1, sizeof(requests));

    // Array of length vehicles 
    // index == vehicle, value == zone
    // Assign a random zone to a vehicle.
    int rand_zone;
    for (int i = 0; i < data->num_vehicles; i++) {
        rand_zone = randomNumber(data->num_zones);
        //printf("Vehicle %d assign to zone %d.\n", i, rand_zone); 
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
        //printf("req id: %d \n", req->data.id);
        
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
        requests[i] = assign(req->data.zone_id,zones,data->num_vehicles,availvehic,requests[i]);

        // kijken of we auto's van aanliggende zones kunnen toewijzen
        if(requests[i]==-1){
            // printf("Zone ID: %d\n",req->data.zone_id);
            // printf("Eerste aanliggende Zone: %d\n",zones[req->data.zone_id].adj_zones[0]);
            //exit(0);
            int n = 0;
            while(zones[req->data.zone_id].adj_zones[n] != -1){
                int adj_zone = zones[req->data.zone_id].adj_zones[n];
                n++;
                // printf("adj_zone: %d\n",adj_zone);
                requests[i] = assign(adj_zone,zones,data->num_vehicles,availvehic,requests[i]);   
                if(requests[i] != -1){
                    totalCost += req->data.penalty2;
                    break;
                }
            }
        }
        if(requests[i]==-1){
            totalCost+= req->data.penalty1;
        }
        printf("req%d: car%d\n",i,requests[i]);
        
    }
    printf("Totalcost: %d\n",totalCost);
    return 0;
}


