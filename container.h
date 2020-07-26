#include <stdbool.h>

typedef struct
{
    // No need for holding the container level, index in array will do that.
    int curr_crit_level;
    bool global; // Indicates whether this container is a global container or not.
    Job *jobs;
}
Container;
