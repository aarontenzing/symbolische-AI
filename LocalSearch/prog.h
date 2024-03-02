#define LINE_LENGTH 500
#define MAX 100
#define ID 10

typedef struct Request {
    char id[ID];
    char zone_id[ID];
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
    char id[ID];
    char adj_zones[MAX]; 
} Zone;