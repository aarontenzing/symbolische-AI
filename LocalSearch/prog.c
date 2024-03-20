#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read.c" 

void solution(int num_vehicles, int num_requests, int vehicles[num_vehicles], int requests[num_requests], int totalCost) {
    FILE *file = fopen(output_filepath, "w");
    
    // Unassigned requests
    int unassigned[num_requests];
    memset(unassigned, -1, sizeof(unassigned));
    int idx = 0;

    if (file == NULL) {
        printf("Error opening the file");
        return;
    }

    fprintf(file, "%d\n", totalCost);
    fprintf(file, "+Vehicle assignments\n");

    for (int i = 0; i < num_vehicles; i++) {
        fprintf(file, "car%d;z%d\n", i, vehicles[i]);
    }

    fprintf(file, "+Assigned requests\n");

    for (int i = 0; i < num_requests; i++) {
        if (requests[i] == -1) {
            unassigned[idx] = i;
            idx++;
            continue;
        }
        fprintf(file, "req%d;car%d\n", i, requests[i]);
    }

    fprintf(file, "+Unassigned requests\n");

    int i = 0;
    while(unassigned[i] != -1) {
        fprintf(file, "req%d\n", unassigned[i]);
        i++;
    }

    fclose(file);   
}

int randomNumber(int upperBound) {
    // Seed the random number generator with the current time
    return rand() % upperBound;
}

int assign(RequestNode* head,RequestNode* req,int id,Zone* zones,int num_vehicles, int availvehic[num_vehicles], int request, int num_requests ,int requests[num_requests]){
    RequestNode* oldreq;
    // printf("req->data.zone_id: %d ",id);
    for (int j = 0; j < num_vehicles; j++) {
            
        int currVehicle = zones[id].voertuigen[j];

        if(currVehicle == -1){
                continue;
        }

        for (int l = 0; l < num_vehicles; l++) {
            if(availvehic[l] == -1){
                continue;
            }
            if(currVehicle == availvehic[l]){
                for(int k = 0; k < num_requests; k++){
                    
                    if(requests[k] == currVehicle){

                        oldreq = getItem(head,k);
                        //printf("\nRequest: %d ,Old day %d, New old day %d,total time: %d , new start time: %d\n",k,oldreq->data.day,req->data.day,oldreq->data.start_time + oldreq->data.duration,req->data.start_time);
                        if(oldreq->data.day == req->data.day && oldreq->data.start_time + oldreq->data.duration > req->data.start_time){
                            //request = currVehicle;
                            //printf("Old total time: %d , new start time: %d\n",oldreq->data.start_time + oldreq->data.duration,req->data.start_time);
                            return request;
                        }

                    }           
                }
                request = currVehicle;
            }
            
        }
            
    }
    return request;

}

void swap(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

void swap_car(int num_vehicles, int vehicles[num_vehicles], Zone* zones) {
    
    int car1, car2;
    do {
        car1 = randomNumber(num_vehicles);
        car2 = randomNumber(num_vehicles);
        
    } while(car1 == car2 || vehicles[car1] == vehicles[car2]);

    //printf("vehicle %d zone %d, vehicle %d zone %d \n", car1, vehicles[car1], car2, vehicles[car2]);
    // swap zones in vehicles array
    swap(vehicles + car1, vehicles + car2);
    //printf(" swapped : vehicle %d zone %d, vehicle %d zone %d \n", car1, vehicles[car1], car2, vehicles[car2]);
    int zone1 = vehicles[car1];
    int zone2 = vehicles[car2];

    // change voertuig in zone array (struct)
    int i = 0;
    while (zones[zone1].voertuigen[i] != car2) {
        i++;
    }
    zones[zone1].voertuigen[i] = car1;

    i = 0;
     while (zones[zone2].voertuigen[i] != car1) {
        i++;
    }
    zones[zone2].voertuigen[i] = car2;

}

int main(int argc, char *argv[]) {

    // <input_file> <solution_file> <time_limit> <random_seed> <num_threads>
    double time_limit;
    unsigned int seed = 6969;
    if (argc == 5) {
        input_filepath = argv[1];
        output_filepath = argv[2];
        time_limit = atoi(argv[3]); 
        seed = atoi(argv[4]);
    }
    else {
        input_filepath = "voorbeeld_input.csv";
        output_filepath = "solution.csv";
        time_limit = 5;
    }

    int totalCost = 0;
    int bestCost = 0;
    srand(seed);

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
   
    // Assign a random zone to a vehicle.
    srand(time(NULL));
    int rand_zone;
    for (int i = 0; i < data->num_vehicles; i++) {
        rand_zone = randomNumber(data->num_zones);
        //printf("random %d\n", rand_zone);
        vehicles[i] = rand_zone; // assign zone to vehicle

        //assign vehicle to zone
        int j = 0;
        while(zones[rand_zone].voertuigen[j] != -1){
            j++;
        }
        zones[rand_zone].voertuigen[j] = i;
    }

    // Initial solution
    // Assign a appropriate vehicle to a request.
    RequestNode* req;
    for (int i = 0; i < data->num_requests; i++) {
        req = getItem(head, i);
        char *save;
        char* car;
        car = strtok_r(req->data.vehicles,",",&save);
        int k = 0;
        memset(availvehic, -1, sizeof(availvehic)); 
        // vehicles die bij request moeten horen parsen
        while(car != NULL){
            sscanf(car,"%*3s%d",&availvehic[k]);
            car = strtok_r(NULL,",",&save);
            k++;
        }

        //staat er 1 van de auto's in die zone in de zones struct
        requests[i] = assign(head, req, req->data.zone_id, zones, data->num_vehicles, availvehic, requests[i], i, requests);

        // kijken of we auto's van aanliggende zones kunnen toewijzen
        if(requests[i]==-1){
            int n = 0;
            while(zones[req->data.zone_id].adj_zones[n] != -1){
                int adj_zone = zones[req->data.zone_id].adj_zones[n];
                n++;
                // printf("adj_zone: %d\n",adj_zone);
                requests[i] = assign(head,req,adj_zone,zones,data->num_vehicles,availvehic,requests[i],i,requests);   
                if(requests[i] != -1){
                    totalCost += req->data.penalty2;
                    break;
                }
            }
        }
        if(requests[i]==-1){
            totalCost += req->data.penalty1;
        }

        (requests[i] != -1) ? printf("req%d: car%d\n", i, requests[i]) : 0;
        
    }
    printf("Totalcost: %d\n",totalCost);
    
    // LocalSearch
    // vehicles, requests & totalCost -> when best solution found write to file
    bestCost = totalCost;
    solution(data->num_vehicles, data->num_requests, vehicles, requests, totalCost);
    totalCost = 0;
    time_t start_time = time(NULL);
    printf("time limit: %d\n", time_limit);

    while (difftime(time(NULL), start_time) < time_limit) {
        swap_car(data->num_vehicles, vehicles, zones);
        memset(requests, -1, sizeof(requests));

        // Assign a appropriate vehicle to a request. 
        RequestNode* req;
        for (int i = 0; i < data->num_requests; i++) {
            req = getItem(head, i);
            char *save;
            char* car;
            car = strtok_r(req->data.vehicles,",",&save);
            int k = 0;
            memset(availvehic, -1, sizeof(availvehic)); 
            
            // vehicles die bij request moeten horen parsen
            while(car != NULL){
                sscanf(car,"%*3s%d",&availvehic[k]);
                car = strtok_r(NULL,",",&save);
                k++;
            }

            //staat er 1 van de auto's in die zone in de zones struct
            requests[i] = assign(head, req, req->data.zone_id, zones, data->num_vehicles, availvehic, requests[i], i, requests);

            // kijken of we auto's van aanliggende zones kunnen toewijzen
            if(requests[i]==-1){
                int n = 0;
                while(zones[req->data.zone_id].adj_zones[n] != -1){
                    int adj_zone = zones[req->data.zone_id].adj_zones[n];
                    n++;
                    // printf("adj_zone: %d\n",adj_zone);
                    requests[i] = assign(head,req,adj_zone,zones,data->num_vehicles,availvehic,requests[i],i,requests);   
                    if(requests[i] != -1){
                        totalCost += req->data.penalty2;
                        break;
                    }
                }
            }
            if(requests[i]==-1){
                totalCost += req->data.penalty1;
            }
            //printf("req%d: car%d\n",i,requests[i]);
            
        }

        printf("Totalcost: %d\n",totalCost);
        
        if (totalCost < bestCost) {
            bestCost = totalCost;
            solution(data->num_vehicles, data->num_requests, vehicles, requests, totalCost);
        }
        totalCost = 0;
    }

    printf("bestCost %d\n", bestCost);

    return 0;
}


