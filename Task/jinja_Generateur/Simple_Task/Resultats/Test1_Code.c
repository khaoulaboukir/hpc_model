const int TASK_COUNT = 6;
const int CORE_COUNT = 3;


/* Task data structures */

typedef enum{
    SUSPENDED,
    READY,
    RUNNING
}task_state;

typedef enum{
    CPU,
    GPU,
    BOTH,
    UNKOWN
}task_core;

typedef struct{
	task_state state;
    task_core core;
	int id;
	int Priority;
}task_descriptor;


typedef enum{
    ACTIVE,
    IDLE
}core_state;

typedef enum{
    GPU,
    CPU
}core_type;

typedef struct{
    core_state state;
    core_type type;
    int id;
    int selected_task;
}core_descriptor;


/* Task lists + mainting functions*/

task_descriptor sort_list(task_descriptor [TASK_COUNT] Sorted_Task) {
    int i, j, index = 0;

    task_descriptor [TASK_COUNT] cpu_tasks;
    task_descriptor [TASK_COUNT] gpu_tasks;
    task_descriptor [TASK_COUNT] unknown_tasks;

    int cpu_count = 0;
    int gpu_count = 0;
    int unk_count = 0;

    for (i = 0; i < TASK_COUNT; i++) {
        if (Sorted_Task[i].id != -1) {
            if (Sorted_Task[i].core == CPU) {
                cpu_tasks[cpu_count] = Sorted_Task[i];
                cpu_count++;
            } else if (Sorted_Task[i].core == GPU) {
                gpu_tasks[gpu_count] = Sorted_Task[i];
                gpu_count++;
            } else {
                unknown_tasks[unk_count] = Sorted_Task[i];
                unk_count++;
            }
        }
    }

    for (i = 0; i < cpu_count - 1; i++) {
        for (j = i + 1; j < cpu_count; j++) {
            if (cpu_tasks[i].Priority < cpu_tasks[j].Priority) {
                task_descriptor temp = cpu_tasks[i];
                cpu_tasks[i] = cpu_tasks[j];
                cpu_tasks[j] = temp;
            }
        }
    }

    index = 0;
    for (i = 0; i < cpu_count; i++) {
        Sorted_Task[index] = cpu_tasks[i];
        index++;
    }
    for (i = 0; i < gpu_count; i++) {
        Sorted_Task[index] = gpu_tasks[i];
        index++;
    }
    for (i = 0; i < unk_count; i++) {
        Sorted_Task[index] = unknown_tasks[i];
        index++;
    }

    while (index < TASK_COUNT) {
        Sorted_Task[index].id = -1;
        Sorted_Task[index].Priority = -1;
        Sorted_Task[index].state = SUSPENDED;
        Sorted_Task[index].core = UNKOWN;
        index++;
    }

    return Sorted_Task;
}

void insert_task(task_descriptor [TASK_COUNT] &List_Task, task_descriptor [TASK_COUNT] &Sorted_Task, int id) {
    int i;
    int index = -1;
    int null_index = -1;
    int already_exists = 0;

    i = 0;
    while (i < TASK_COUNT && index == -1) {
        if (List_Task[i].id == id) {
            index = i;
        }
        i = i + 1;
    }

    i = 0;
    while (i < TASK_COUNT && already_exists == 0) {
        if (Sorted_Task[i].id == id) {
            already_exists = 1;
        }
        i = i + 1;
    }

    if (already_exists == 0 && index != -1) {
        i = 0;
        while (i < TASK_COUNT && null_index == -1) {
            if (Sorted_Task[i].id == -1) {
                null_index = i;
            }
            i = i + 1;
        }

        if (null_index != -1) {
            i = null_index;
            while (i < TASK_COUNT - 1) {
                Sorted_Task[i] = Sorted_Task[i + 1];
                i = i + 1;
            }
            Sorted_Task[TASK_COUNT - 1].id = -1;
        }

        i = 0;
        int position_insertion = -1;
        while (i < TASK_COUNT + 10 && position_insertion == -1) {
            if (Sorted_Task[i].id == -1) {
                position_insertion = i;
            }
            i = i + 1;
        }

        if (position_insertion != -1) {
            Sorted_Task[position_insertion] = List_Task[index];
            Sorted_Task[position_insertion].state = READY;
        }
    }
}


/* Core API */


int is_selected_task(core_descriptor [CORE_COUNT] &List_Core, int id){
    int i;
    for(i=0; i < CORE_COUNT; i++){
        if(List_Core[i].selected_task == id){
            return 1;
        }
    }
return 0;
}


/* Scheduler functions */

void scheduler(task_descriptor [TASK_COUNT] &Sorted_Task, core_descriptor [CORE_COUNT] &List_Core){
    Sorted_Task = sort_list(Sorted_Task);
    int i = 0;
    int task_found = 0;

    while (i < TASK_COUNT && task_found == 0) {
        int task_id = Sorted_Task[i].id;
        task_core required_core = Sorted_Task[i].core;
        int already_assigned = 0;

        int j = 0;
        while (j < CORE_COUNT) {
            if (List_Core[j].selected_task == task_id) {
                already_assigned = 1;
            }
            j++;
        }

        if (already_assigned == 0 && task_id != -1) {
            int assigned = 0;
            j = 0;

            if (required_core == BOTH) {
                while (j < CORE_COUNT && assigned == 0) {
                    if (List_Core[j].selected_task == -1 && List_Core[j].type == CPU) {
                        List_Core[j].state = ACTIVE;
                        List_Core[j].selected_task = task_id;
                        assigned = 1;
                        task_found = 1;
                    }
                    j++;
                }

                j = 0;
                while (j < CORE_COUNT && assigned == 0) {
                    if (List_Core[j].selected_task == -1 && List_Core[j].type == GPU) {
                        List_Core[j].state = ACTIVE;
                        List_Core[j].selected_task = task_id;
                        assigned = 1;
                        task_found = 1;
                    }
                    j++;
                }
            }
            else {
                j = 0;
                while (j < CORE_COUNT && assigned == 0) {
                    if (List_Core[j].selected_task == -1 && List_Core[j].type == required_core) {
                        List_Core[j].state = ACTIVE;
                        List_Core[j].selected_task = task_id;
                        assigned = 1;
                        task_found = 1;
                    }
                    j++;
                }
            }
        }

        i++;
    }
}

void no_need_scheduler(task_descriptor [TASK_COUNT] &Sorted_Task, int id){
    int i;
    int removed = 0;

    for(i = 0; i < TASK_COUNT; i++){
        if(Sorted_Task[i].id == id && removed == 0){
            Sorted_Task[i].state = SUSPENDED;
            Sorted_Task[i].core = UNKOWN;
            Sorted_Task[i].id = -1;
            Sorted_Task[i].Priority = -1;
            removed = 1;
        }
    }

    Sorted_Task = sort_list(Sorted_Task);
}


/* Task API */

void free_core(core_descriptor [CORE_COUNT] &List_Core, int id){
    int i;
    for(i=0; i < CORE_COUNT; i++){
        if(List_Core[i].selected_task == id){
            List_Core[i].state = IDLE;
            List_Core[i].selected_task = -1;
        }
    }
}