#define LINE_LENGTH 500
#define MAX 100

typedef struct Request {
    char id[10];
    char zone_id[10];
    int day;
    int start_time;
    int duration;
    char vehicles[100];
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
    char id[10];
    char adj_zones[100]; 
} Zone;