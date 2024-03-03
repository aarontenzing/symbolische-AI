#define LINE_LENGTH 500
#define MAX 100
#define ID_LENGTH 20

typedef struct Request {
    char id[ID_LENGTH];
    char zone_id[ID_LENGTH];
    int day;
    int start_time;
    int duration;
    char vehicles[MAX];
    int penalty1;
    int penalty2;
} Request;

typedef struct RequestNode {
    Request data;
    struct RequestNode *next;
} RequestNode;

typedef struct Info {
    int num_requests;
    int num_zones;
    int num_vehicles;
    int num_days;
} Info;

typedef struct Zone {
    char id[ID_LENGTH];
    char adj_zones[MAX]; 
} Zone;

typedef struct Vechicle {
    char id[ID_LENGTH];
} Vechicle;