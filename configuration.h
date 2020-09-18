#define FILE_INPUT_A "input_A.txt"
#define FILE_INPUT_B "input_B.txt"
#define FILE_INPUT_C "input_C.txt"
#define FILE_INPUT_D "input_D.txt"

#define FILE_OUTPUT "output.txt"

#define NUM_PROCESSORS 2

#define CHANCE_MISS_DEADLINE 0.1 // The chance with which a job will miss its deadline and go into the next higher criticality level.
#define MIN_PERCENT_EXECUTION 50 // The min percent of wcet that the aet will be.
#define ZERO_AET_TOLERANCE 1e-2 // Below this number, the number is considered as 0.