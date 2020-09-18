float min(float, float);
float max(float, float);

void init_scheduler();
void print_ready_queues();
float get_actual_runtime(float);
int get_deadline_miss();
bool check_ready_queues_empty();
void reflect_increasing_criticality_in_ready_queues();
void add_job_to_ready_queue_helper();
void add_jobs_to_ready_queue();
float get_earliest_arrival_time();
void check_and_lower_criticality();
float get_next_decision_point();
void run_jobs();
void scheduler();