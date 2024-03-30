#define LINE_LENGTH 500
#define MAX 100
#define ID_LENGTH 20

char* input_filepath;
char* output_filepath = "solution.csv";

typedef struct Request {
    int id;
    int zone_id;
    int day;
    int start_time;
    int duration;
    int vehicles[MAX];
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
    int adj_zones[MAX]; 
    int voertuigen[MAX];
} Zone;

typedef struct {
    Info* data;
    Zone* zones;
    RequestNode* head;
    int* vehicles;
    int* requests;
    int bestCost;
    double time_limit;

} ThreadArgs;
