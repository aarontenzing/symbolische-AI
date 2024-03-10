#define LINE_LENGTH 500
#define MAX 100
#define ID_LENGTH 20

typedef struct Request {
    int id;
    int zone_id;
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
    int id;
    char adj_zones[MAX]; 
} Zone;