const int TASK_COUNT = 30;
// const int CORE_COUNT = 3;
const int CPU_COUNT = ;
const int GPU_COUNT = ;


/* Task data structures */

typedef enum{
    SUSPENDED,
    READY,
    RUNNING
}task_state;

typedef enum{
    CPU,
    GPU,
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

task_descriptor tri_fp(task_descriptor [TASK_COUNT] Ready_Task_CPU) {
    task_descriptor [TASK_COUNT] cpu_tasks;
    int cpu_count = 0;

    // Récupération des tâches CPU
    for (int i = 0; i < TASK_COUNT; i++) {
        if (Ready_Task_CPU[i].id != NULL && Ready_Task_CPU[i].core == CPU) {
            cpu_tasks[cpu_count] = Ready_Task_CPU[i];
            cpu_count++;
        }
    }

    // Tri FP (priorité décroissante) sur les tâches CPU
    for (int i = 0; i < cpu_count - 1; i++) {
        for (int j = i + 1; j < cpu_count; j++) {
            if (cpu_tasks[i].Priority < cpu_tasks[j].Priority) {
                task_descriptor tmp = cpu_tasks[i];
                cpu_tasks[i] = cpu_tasks[j];
                cpu_tasks[j] = tmp;
            }
        }
    }

    // Réécriture dans Ready_Task[0] (CPU)
    for (int i = 0; i < TASK_COUNT; i++) {
        if (i < cpu_count) {
            Ready_Task_CPU[i] = cpu_tasks[i];
        } else {
            Ready_Task_CPU[i].id = NULL;
            Ready_Task_CPU[i].Priority = NULL;
            Ready_Task_CPU[i].state = SUSPENDED;
            Ready_Task_CPU[i].core = UNKOWN;
        }
    }
    return Ready_Task_CPU;
}

task_descriptor list_fifo(task_descriptor [TASK_COUNT] Ready_Task_GPU) {
    task_descriptor [TASK_COUNT] gpu_tasks;
    int gpu_count = 0;

    // Récupération des tâches GPU
    for (int i = 0; i < TASK_COUNT; i++) {
        if (Ready_Task_GPU[i].id != NULL && Ready_Task_GPU[i].core == GPU) {
            gpu_tasks[gpu_count] = Ready_Task_GPU[i];
            gpu_count++;
        }
    }

    // Mettre d’abord les tâches READY

    int ready_index = 0;
    for (int i = 0; i < gpu_count; i++) {
        if (gpu_tasks[i].state == READY) {
            Ready_Task_GPU[ready_index] = gpu_tasks[i];
            ready_index++;
        }
    }

    // Réécriture dans Ready_Task[1] (GPU) : FIFO => pas de tri
    for (int i = ready_index; i < TASK_COUNT; i++) {
        Ready_Task_GPU[i].id = NULL;
        Ready_Task_GPU[i].Priority = NULL;
        Ready_Task_GPU[i].state = SUSPENDED;
        Ready_Task_GPU[i].core = UNKOWN;
    }
    return Ready_Task_GPU;
}

void insert_cpu_task(task_descriptor [TASK_COUNT] &List_Task, task_descriptor [TASK_COUNT] &Ready_Task_CPU, int id) {
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
        if (Ready_Task_CPU[i].id == id) {
            already_exists = 1;
        }
        i = i + 1;
    }

    if (already_exists == 0 && index != -1) {
        i = 0;
        while (i < TASK_COUNT && null_index == -1) {
            if (Ready_Task_CPU[i].id == NULL) {
                null_index = i;
            }
            i = i + 1;
        }

        if (null_index != -1) {
            i = null_index;
            while (i < TASK_COUNT - 1) {
                Ready_Task_CPU[i] = Ready_Task_CPU[i + 1];
                i = i + 1;
            }
            Ready_Task_CPU[TASK_COUNT - 1].id = NULL;
        }

        i = 0;
        int position_insertion = -1;
        while (i < TASK_COUNT && position_insertion == -1) {
            if (Ready_Task_CPU[i].id == NULL) {
                position_insertion = i;
            }
            i = i + 1;
        }
        if (position_insertion != -1) {
            Ready_Task_CPU[position_insertion] = List_Task[index];
            Ready_Task_CPU[position_insertion].state = READY;
        }
    }
}

void insert_gpu_task(task_descriptor [TASK_COUNT] &List_Task, task_descriptor [TASK_COUNT] &Ready_Task_GPU, int id) {
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
        if (Ready_Task_GPU[i].id == id) {
            already_exists = 1;
        }
        i = i + 1;
    }

    if (already_exists == 0 && index != -1) {
        i = 0;
        while (i < TASK_COUNT && null_index == -1) {
            if (Ready_Task_GPU[i].id == NULL) {
                null_index = i;
            }
            i = i + 1;
        }

        if (null_index != -1) {
            i = null_index;
            while (i < TASK_COUNT - 1) {
                Ready_Task_GPU[i] = Ready_Task_GPU[i + 1];
                i = i + 1;
            }
            Ready_Task_GPU[TASK_COUNT - 1].id = NULL;
        }

        i = 0;
        int position_insertion = -1;
        while (i < TASK_COUNT && position_insertion == -1) {
            if (Ready_Task_GPU[i].id == NULL) {
                position_insertion = i;
            }
            i = i + 1;
        }
        if (position_insertion != -1) {
            Ready_Task_GPU[position_insertion] = List_Task[index];
            Ready_Task_GPU[position_insertion].state = READY;
        }
    }
}

/* Core API */

int is_selected_task_cpu(core_descriptor [CPU_COUNT] &List_CPU, int id){
    int i;
    for(i=0; i < CPU_COUNT; i++){
        if(List_CPU[i].selected_task == id){
            return 1;
        }
    }
return 0;
}

int is_selected_task_gpu(core_descriptor [GPU_COUNT] &List_GPU, int id){
    int i;
    for(i=0; i < GPU_COUNT; i++){
        if(List_GPU[i].selected_task == id){
            return 1;
        }
    }
return 0;
}

/* Scheduler functions */

void scheduler_fp(task_descriptor [TASK_COUNT] &Ready_Task_CPU, core_descriptor [CPU_COUNT] &List_CPU){
    Ready_Task_CPU = tri_fp(Ready_Task_CPU);

    int task_found = 0;
    int i = 0;

    while (i < TASK_COUNT && task_found == 0) {
        int task_id = Ready_Task_CPU[i].id;
        int j = 0;
        int already_assigned = 0;

        while (j < CPU_COUNT) {
            if (List_CPU[j].selected_task == task_id) {
                already_assigned = 1;
            }
            j++;
        }

        if (already_assigned == 0 && task_id != NULL) {
            int assigned = 0;
            j = 0;

            while (j < CPU_COUNT && assigned == 0) {

                if (List_CPU[j].selected_task == NULL) {
                    List_CPU[j].state = ACTIVE;
                    List_CPU[j].selected_task = task_id;
                    assigned = 1;
                    task_found = 1;
                }
                j++;
            }
        }
        i++;
    }
}

void scheduler_fifo(task_descriptor [TASK_COUNT] &Ready_Task_GPU, core_descriptor [GPU_COUNT] &List_GPU){
    Ready_Task_GPU = list_fifo(Ready_Task_GPU);

    int task_found = 0;
    int i = 0;

    while (i < TASK_COUNT && task_found == 0) {
        int task_id = Ready_Task_GPU[i].id;
        int j = 0;
        int already_assigned = 0;

        while (j < GPU_COUNT) {
            if (List_GPU[j].selected_task == task_id) {
                already_assigned = 1;
            }
            j++;
        }

        if (already_assigned == 0 && task_id != NULL) {
            int assigned = 0;
            j = 0;

            while (j < GPU_COUNT && assigned == 0) {

                if (List_GPU[j].selected_task == NULL) {
                    List_GPU[j].state = ACTIVE;
                    List_GPU[j].selected_task = task_id;
                    assigned = 1;
                    task_found = 1;
                }
                j++;
            }
        }
        i++;
    }
}

void no_need_scheduler_fp(task_descriptor [TASK_COUNT] &Ready_Task_CPU, core_descriptor [CPU_COUNT] &List_CPU, int id){
    int i;

    for(i = 0; i < TASK_COUNT; i++){
        if(Ready_Task_CPU[i].id == id){
            Ready_Task_CPU[i].state = SUSPENDED;
            Ready_Task_CPU[i].id = NULL;
            Ready_Task_CPU[i].Priority = NULL;
        }
    }

    for(i = 0; i < CPU_COUNT; i++){
        if(List_CPU[i].selected_task == id){
            List_CPU[i].state = IDLE;
            List_CPU[i].selected_task = NULL;
        }
    }

    Ready_Task_CPU = tri_fp(Ready_Task_CPU);
}

void no_need_scheduler_fifo(task_descriptor [TASK_COUNT] &Ready_Task_GPU, core_descriptor [GPU_COUNT] &List_GPU, int id){
    int i;

    for(i = 0; i < TASK_COUNT; i++){
        if(Ready_Task_GPU[i].id == id){
            Ready_Task_GPU[i].state = SUSPENDED;
            Ready_Task_GPU[i].id = NULL;
            Ready_Task_GPU[i].Priority = NULL;
        }
    }

    for(i=0; i < GPU_COUNT; i++){
        if(List_GPU[i].selected_task == id){
            List_GPU[i].state = IDLE;
            List_GPU[i].selected_task = NULL;
        }
    }

    Ready_Task_GPU = list_fifo(Ready_Task_GPU);
}