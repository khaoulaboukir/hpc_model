// insert here your type definitions using C-like syntax

// insert here your function definitions 
// using C-like syntax


/* Const */

const int TASK_COUNT = 7;
const int CORE_COUNT = 3;
const int NULL = -1;


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

task_descriptor sort_list(task_descriptor [TASK_COUNT] Ready_Task) {
    int i, j, index = 0;

    task_descriptor [TASK_COUNT] cpu_tasks;
    task_descriptor [TASK_COUNT] gpu_tasks;
    task_descriptor [TASK_COUNT] unknown_tasks;

    int cpu_count = 0;
    int gpu_count = 0;
    int unk_count = 0;

    for (i = 0; i < TASK_COUNT; i++) {
        if (Ready_Task[i].id != NULL) {
            if (Ready_Task[i].core == CPU) {
                cpu_tasks[cpu_count] = Ready_Task[i];
                cpu_count++;
            } else if (Ready_Task[i].core == GPU) {
                gpu_tasks[gpu_count] = Ready_Task[i];
                gpu_count++;
            } else {
                unknown_tasks[unk_count] = Ready_Task[i];
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
        Ready_Task[index] = cpu_tasks[i];
        index++;
    }
    for (i = 0; i < gpu_count; i++) {
        Ready_Task[index] = gpu_tasks[i];
        index++;
    }
    for (i = 0; i < unk_count; i++) {
        Ready_Task[index] = unknown_tasks[i];
        index++;
    }

    while (index < TASK_COUNT) {
        Ready_Task[index].id = NULL;
        Ready_Task[index].Priority = NULL;
        Ready_Task[index].state = SUSPENDED;
        Ready_Task[index].core = UNKOWN;
        index++;
    }

    return Ready_Task;
}

void insert_task(task_descriptor [TASK_COUNT] &List_Task, task_descriptor [TASK_COUNT] &Ready_Task, int id) {
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
        if (Ready_Task[i].id == id) {
            already_exists = 1;
        }
        i = i + 1;
    }

    if (already_exists == 0 && index != -1) {
        i = 0;
        while (i < TASK_COUNT && null_index == -1) {
            if (Ready_Task[i].id == NULL) {
                null_index = i;
            }
            i = i + 1;
        }

        if (null_index != -1) {
            i = null_index;
            while (i < TASK_COUNT - 1) {
                Ready_Task[i] = Ready_Task[i + 1];
                i = i + 1;
            }
            Ready_Task[TASK_COUNT - 1].id = NULL;
        }

        i = 0;
        int position_insertion = -1;
        while (i < TASK_COUNT + 10 && position_insertion == -1) {
            if (Ready_Task[i].id == NULL) {
                position_insertion = i;
            }
            i = i + 1;
        }

        if (position_insertion != -1) {
            Ready_Task[position_insertion] = List_Task[index];
            Ready_Task[position_insertion].state = READY;
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

void scheduler(task_descriptor [TASK_COUNT] &Ready_Task, core_descriptor [CORE_COUNT] &List_Core){
    Ready_Task = sort_list(Ready_Task);
    int i = 0;
    int task_found = 0;

    while (i < TASK_COUNT && task_found == 0) {
        int task_id = Ready_Task[i].id;
        task_core required_core = Ready_Task[i].core;
        int already_assigned = 0;

        int j = 0;
        while (j < CORE_COUNT) {
            if (List_Core[j].selected_task == task_id) {
                already_assigned = 1;
            }
            j++;
        }

        if (already_assigned == 0 && task_id != NULL) {
            int assigned = 0;
            j = 0;

            if (required_core == BOTH) {
                while (j < CORE_COUNT && assigned == 0) {
                    if (List_Core[j].selected_task == NULL && List_Core[j].type == CPU) {
                        List_Core[j].state = ACTIVE;
                        List_Core[j].selected_task = task_id;
                        assigned = 1;
                        task_found = 1;
                    }
                    j++;
                }

                j = 0;
                while (j < CORE_COUNT && assigned == 0) {
                    if (List_Core[j].selected_task == NULL && List_Core[j].type == GPU) {
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
                    if (List_Core[j].selected_task == NULL && List_Core[j].type == required_core) {
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

void no_need_scheduler(task_descriptor [TASK_COUNT] &Ready_Task, int id){
    int i;
    int removed = 0;

    for(i = 0; i < TASK_COUNT; i++){
        if(Ready_Task[i].id == id && removed == 0){
            Ready_Task[i].state = SUSPENDED;
            Ready_Task[i].core = UNKOWN;
            Ready_Task[i].id = NULL;
            Ready_Task[i].Priority = NULL;
            removed = 1;
        }
    }

    Ready_Task = sort_list(Ready_Task);
}


/* Task API */

void free_core(core_descriptor [CORE_COUNT] &List_Core, int id){
    int i;
    for(i=0; i < CORE_COUNT; i++){
        if(List_Core[i].selected_task == id){
            List_Core[i].state = IDLE;
            List_Core[i].selected_task = NULL;
        }
    }
}


initially {
    
    int need_sched = 0;
    int no_need_cpu = 0;
    int no_need_gpu = 0;
    int cpu_flag = 0;
    int gpu_flag = 0;
    int selected_task = NULL;

    /* Task Declaration */
    
    /* Task_form = {state, id, priority, period, execution_time} */
    task_descriptor [TASK_COUNT] List_Task;
		List_Task[0].state = SUSPENDED;
        List_Task[0].core = CPU;
		List_Task[0].id = 0;
		List_Task[0].Priority = 1;

		List_Task[1].state = SUSPENDED;
        List_Task[1].core = CPU;
		List_Task[1].id = 1;
		List_Task[1].Priority = 2;

        List_Task[2].state = SUSPENDED;
        List_Task[2].core = GPU;
		List_Task[2].id = 2;
		List_Task[2].Priority = 3;

        List_Task[3].state = SUSPENDED;
        List_Task[3].core = CPU;
		List_Task[3].id = 3;
		List_Task[3].Priority = 4;

        List_Task[4].state = SUSPENDED;
        List_Task[4].core = CPU;
		List_Task[4].id = 4;
		List_Task[4].Priority = 5;

        List_Task[5].state = SUSPENDED;
        List_Task[5].core = CPU;
		List_Task[5].id = 5;
		List_Task[5].Priority = 6;

        List_Task[6].state = SUSPENDED;
        List_Task[6].core = GPU;
		List_Task[6].id = 6;
		List_Task[6].Priority = 0;

	task_descriptor [TASK_COUNT] Ready_Task;
	int i;
    for(i = 0; i < TASK_COUNT; i++){
		Ready_Task[i].state = SUSPENDED;
        Ready_Task[i].core = UNKOWN;
		Ready_Task[i].id = NULL;
		Ready_Task[i].Priority = NULL;
	}

    /* CPU Declaration */
    
    /* CPU_form = {state, id, selected_task} */
    core_descriptor [CORE_COUNT] List_Core;
		List_Core[0].state = IDLE;
        List_Core[0].type = CPU;
		List_Core[0].id = 0;
		List_Core[0].selected_task = NULL;
		
        List_Core[1].state = IDLE;
        List_Core[1].type = CPU;
		List_Core[1].id = 1;
		List_Core[1].selected_task = NULL;
        
        List_Core[2].state = IDLE;
        List_Core[2].type = GPU;
		List_Core[2].id = 2;
		List_Core[2].selected_task = NULL;


// insert here the state variables declarations 
// and possibly some code to initialize them 
// using C-like syntax
 
 }
 