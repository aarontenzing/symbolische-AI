#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
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
    // printf("unassigned[%d]: %d\n",i,unassigned[i]);
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

int assign(RequestNode* head,RequestNode* req,int id,Zone* zones,int num_vehicles, int request, int num_requests ,int requests[num_requests]){
    RequestNode* oldreq;
    for (int j = 0; j < num_vehicles; j++) {
        // printf("j: %d \n",j);            
        int currVehicle = zones[id].voertuigen[j];
    
        if(currVehicle == -1){
                continue;
        }

        for (int l = 0; l < num_vehicles; l++) {
            if(req->data.vehicles[l] == -1){
                continue;
            }
            if(currVehicle == req->data.vehicles[l]){
                for(int k = 0; k < num_requests; k++){
                    
                    if(requests[k] == currVehicle){
                        // print("requests: %d\n",requests[k]);
                        oldreq = getItem(head,k);
                        if(oldreq->data.day == req->data.day && oldreq->data.start_time + oldreq->data.duration > req->data.start_time){
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
    // swap zones in vehicles array
    swap(vehicles + car1, vehicles + car2);
    int zone1 = vehicles[car1];
    int zone2 = vehicles[car2];

    // change voertuig in zone array (struct)
    int i = 0;
    while (zones[zone1].voertuigen[i] != car2) {
        i++;
    }
    zones[zone1].voertuigen[i] = car1;

    i = 0;
    // printf("voertuig: %d , car1: %d\n",zones[zone2].voertuigen[i],car1);
    while (zones[zone2].voertuigen[i] != car1) {
        i++;
    }
    zones[zone2].voertuigen[i] = car2;

}

void* localsearch(void *args){
    // Assign a random zone to a vehicle.
    ThreadArgs *thread_args = (ThreadArgs *)args;
    Info* data = thread_args->data;
    Zone* zones = thread_args->zones;
    RequestNode *head = thread_args->head;
    int totalCost;
    int bestCost;
    double time_limit = thread_args->time_limit;
    int vehicles[data->num_vehicles];
    int requests[data->num_requests];
    memset(vehicles, -1, sizeof(vehicles));
    memset(requests, -1, sizeof(requests));

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
    RequestNode* req;
    for (int i = 0; i < data->num_requests; i++) {
        req = getItem(head, i);
        //printf("request%d: %d\n",i,thread_args->requests[i]);
        //staat er 1 van de auto's in die zone in de zones struct
        requests[i] = assign(head, req, req->data.zone_id, zones, data->num_vehicles, requests[i], i, requests);
        // printf("request%d: %d\n",i,thread_args->requests[i]);
        // kijken of we auto's van aanliggende zones kunnen toewijzen
        if(requests[i]==-1){
            int n = 0;
            // printf("adj zone: %d\n",zones[req->data.zone_id].adj_zones[n]);
            while(zones[req->data.zone_id].adj_zones[n] != -1){
                int adj_zone = zones[req->data.zone_id].adj_zones[n];
                n++;
                // printf("adj_zone: %d\n",adj_zone);
                requests[i] = assign(head,req,adj_zone,zones,data->num_vehicles,requests[i],i,requests);   
                // Auto toegewezen aan aanliggende zone
                if(requests[i] != -1){
                    totalCost += req->data.penalty2;
                    break;
                }
            }
        }
        if(thread_args->requests[i]==-1){
            totalCost += req->data.penalty1;
        }

        //(requests[i] != -1) ? printf("req%d: car%d\n", i, requests[i]) : 0;
        
    }
    printf("Totalcost: %d\n",totalCost);
    
    // LocalSearch
    // vehicles, requests & totalCost -> when best solution found write to file
    bestCost = totalCost;
    for(int a = 0;a<data->num_requests;a++){
        thread_args->requests[a] = requests[a];
    }
    for(int a = 0;a<data->num_vehicles;a++){
        thread_args->vehicles[a] = vehicles[a];
    }
    thread_args->bestCost = totalCost;
    //solution(data->num_vehicles, data->num_requests, thread_args->vehicles, thread_args->requests, totalCost);
    totalCost = 0;
    time_t start_time = time(NULL);
    printf("time limit: %f\n", time_limit);

    while (difftime(time(NULL), start_time) < time_limit) {
        swap_car(data->num_vehicles, vehicles, zones);
        memset(requests, -1, data->num_requests * sizeof(int));

        // Assign a appropriate vehicle to a request. 
        RequestNode* req;
        for (int i = 0; i < data->num_requests; i++) {
            req = getItem(head, i);
            //staat er 1 van de auto's in die zone in de zones struct
            //requests[i] = assign(head, req, req->data.zone_id, zones, data->num_vehicles, availvehic, requests[i], i, requests);
            requests[i] = assign(head, req, req->data.zone_id, zones, data->num_vehicles, requests[i], i, requests);

            // kijken of we auto's van aanliggende zones kunnen toewijzen
            if(thread_args->requests[i]==-1){
                int n = 0;
                while(zones[req->data.zone_id].adj_zones[n] != -1){
                    int adj_zone = zones[req->data.zone_id].adj_zones[n];
                    n++;
                    // printf("adj_zone: %d\n",adj_zone);
                    requests[i] = assign(head,req,adj_zone,zones,data->num_vehicles,requests[i],i,requests);   
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

        //printf("Totalcost: %d\n",totalCost);
        
        if (totalCost < bestCost) {
            bestCost = totalCost;
            for(int a = 0;a<data->num_requests;a++){
                thread_args->requests[a] = requests[a];
            }
            for(int a = 0;a<data->num_vehicles;a++){
                thread_args->vehicles[a] = vehicles[a];
            }
            thread_args->bestCost = totalCost;
            //solution(data->num_vehicles, data->num_requests, thread_args->vehicles, thread_args->requests, totalCost);
        }
        totalCost = 0;
    }
    printf("bestCost %d\n", bestCost);

    // for(int i=0;i<data->num_vehicles;i++){
    //     thread_args->vehicles[i] = vehicles[i]; 
    // }
    // for(int i=0;i<data->num_requests;i++){
    //     thread_args->requests[i] = requests[i]; 
    // }
    thread_args->bestCost = bestCost;

}

int main(int argc, char *argv[]) {

    // <input_file> <solution_file> <time_limit> <random_seed> <num_threads>
    //char* input_filepath,solution_file;
    double time_limit=5;
    unsigned int seed = 6969;
    int bestTotalCost,num_threads=2;
    if (argc == 6) {
        input_filepath = argv[1];
        output_filepath = argv[2];
        time_limit = atoi(argv[3]); 
        seed = atoi(argv[4]);
        num_threads = atoi(argv[5]);
    }
    else {
        input_filepath = "voorbeeld_input.csv";
        output_filepath = "solution.csv";
    }

    srand(seed);
    Info *data = createInformation();
    readInfo(data);

    // information
    // printf("Number of requests: %d\n", data->num_requests);
    // printf("Number of zones: %d\n", data->num_zones);
    // printf("Number of vehicles: %d\n", data->num_vehicles);
    // printf("Number of days: %d\n", data->num_days);
    // printf("\n");

    // Create the data structures
    Zone* zones;
    RequestNode *head;
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs thread_args[num_threads];
    
    for(int i=0;i<num_threads;i++){
        // zones = createZones(data->num_zones);
        // head = readInput(zones);
        thread_args[i].data = data;
        thread_args[i].zones = createZones(data->num_zones);
        thread_args[i].head = readInput(thread_args[i].zones);
        thread_args[i].vehicles = malloc(data->num_vehicles * sizeof(int));
        thread_args[i].requests = malloc(data->num_requests * sizeof(int));
        thread_args[i].time_limit=time_limit;
        memset(thread_args[i].vehicles, -1, data->num_vehicles * sizeof(int));
        memset(thread_args[i].requests, -1, data->num_requests * sizeof(int));
        pthread_create(&threads[i], NULL, localsearch, (void *)&thread_args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    free(threads);

    bestTotalCost = thread_args[0].bestCost;
    int idx=0;
    for (int i = 1; i < num_threads; i++) {
        if (thread_args[i].bestCost < bestTotalCost) {
            bestTotalCost = thread_args[i].bestCost;
            idx=i;
        }
    }

    // for (int i = 0; i < data->num_requests; i++) {
    //     printf("req%d : %d \n",i,thread_args[idx].requests[i]);
        
    // }

    solution(data->num_vehicles,data->num_requests, thread_args[idx].vehicles, thread_args[idx].requests, bestTotalCost);

    // solution(data->num_vehicles,data->num_requests, thread_args.vehicles, thread_args.requests, thread_args.bestCost);


    return 0;
}


