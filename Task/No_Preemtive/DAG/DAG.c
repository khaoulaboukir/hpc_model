// insert here your type definitions using C-like syntax

// insert here your function definitions 
// using C-like syntax


/* Const */

const int TASK_COUNT = 5;
const int CORE_COUNT = 2;
const int NULL = -1;


/* Task data structures */

typedef enum{
    SUSPENDED,
    READY,
    RUNNING
}task_state;

typedef struct{
	task_state state;
	int id;
	int Priority;
	// int Period;
	// int WCET;
}task_descriptor;


typedef enum{
    ACTIVE,
    IDLE
}core_state;

typedef struct{
    core_state state;
    int id;
    int selected_task;
}core_descriptor;


/* Task lists + mainting functions*/

task_descriptor sort_list(task_descriptor [TASK_COUNT] Sorted_Task){
    // int result = 1;
    int i, j;
    task_descriptor temp;

    for(i = 0; i < TASK_COUNT - 1; i++){
        for(j = i; j < TASK_COUNT; j++){
            if(Sorted_Task[i].Priority < Sorted_Task[j].Priority){
                temp = Sorted_Task[i];
                Sorted_Task[i] = Sorted_Task[j];
                Sorted_Task[j] = temp;
            }
        }
    }
    return Sorted_Task;
}

void insert_task(task_descriptor [TASK_COUNT] &List_Task, task_descriptor [TASK_COUNT] &Sorted_Task, int id) {
    int i = 0;
    int index = NULL;
    int found = 0;

    // Trouver l’index dans List_Task
    while (i < TASK_COUNT) {
        if (List_Task[i].id == id && found == 0) {
            index = i;
            found = 1;
        }
        i++;
    }

    // Réinitialiser l’indice pour insérer dans Sorted_Task
    i = 0;
    found = 0;
    while (i < TASK_COUNT) {
        if (Sorted_Task[i].id == NULL && found == 0) {
            Sorted_Task[i] = List_Task[index];
            Sorted_Task[i].state = READY;
            found = 1;
        }
        i++;
    }
}

void suspend_task(task_descriptor [TASK_COUNT] &Sorted_Task, int id){
    int i;
    for(int i = 0; i < TASK_COUNT; i++){
	    if(Sorted_Task[i].id == id){
            Sorted_Task[0].state = SUSPENDED;
            Sorted_Task[0].Priority = NULL;
        }
    }
    Sorted_Task = sort_list(Sorted_Task);
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

core_descriptor maj_core(core_descriptor [CORE_COUNT] List_Core, int id) {
    int i = 0;
    while (i < CORE_COUNT) {
        if (List_Core[i].selected_task == NULL) {
            List_Core[i].state = ACTIVE;
            List_Core[i].selected_task = id;
            i = CORE_COUNT;
        } else {
            i++;
        }
    }
    return List_Core;
}


/* Scheduler functions */

void scheduler(task_descriptor [TASK_COUNT] &Sorted_Task, core_descriptor [CORE_COUNT] &List_Core){
    Sorted_Task = sort_list(Sorted_Task);

    int i = 0;
    int task_found = 0;

    while (i < TASK_COUNT && task_found == 0) {
        int task_id = Sorted_Task[i].id;
        int already_assigned = 0;

        // Vérifier si cette tâche est déjà assignée à un core
        int j = 0;
        while (j < CORE_COUNT) {
            if (List_Core[j].selected_task == task_id) {
                already_assigned = 1;
            }
            j++;
        }

        // Si cette tâche n’est pas encore utilisée, l’affecter à un core
        if (already_assigned == 0 && task_id != NULL) {
            List_Core = maj_core(List_Core, task_id);
            task_found = 1;
        }

        i++;
    }
}

void no_need_scheduler(task_descriptor [TASK_COUNT] &Sorted_Task, int id){
    Sorted_Task = sort_list(Sorted_Task);
    int i;
    for(i = 0; i < TASK_COUNT; i++){
        if(Sorted_Task[i].id == id){
            Sorted_Task[i].state = SUSPENDED;
            Sorted_Task[i].id = NULL;
            Sorted_Task[i].Priority = NULL;
            // Sorted_Task[i].Period = NULL;
            // Sorted_Task[i].WCET = NULL;
        }
    }
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
    int no_need_sched = 0;
    int selected_task = NULL;

    /* Task Declaration */
    
    /* Task_form = {state, id, priority, period, execution_time} */
    task_descriptor [TASK_COUNT] List_Task;
		List_Task[0].state = SUSPENDED;
		List_Task[0].id = 0;
		List_Task[0].Priority = 1;
		// List_Task[0].Period = 5;
		// List_Task[0].WCET = 2;
		List_Task[1].state = SUSPENDED;
		List_Task[1].id = 1;
		List_Task[1].Priority = 2;
		// List_Task[1].Period = 7;
		// List_Task[1].WCET = 3;
        List_Task[2].state = SUSPENDED;
		List_Task[2].id = 2;
		List_Task[2].Priority = 3;
		// List_Task[1].Period = 7;
		// List_Task[1].WCET = 3;
        List_Task[3].state = SUSPENDED;
		List_Task[3].id = 3;
		List_Task[3].Priority = 4;
		// List_Task[1].Period = 7;
		// List_Task[1].WCET = 3;
        List_Task[4].state = SUSPENDED;
		List_Task[4].id = 4;
		List_Task[4].Priority = 5;
		// List_Task[1].Period = 7;
		// List_Task[1].WCET = 3;
	
	task_descriptor [TASK_COUNT] Sorted_Task;
	int i;
    for(i = 0; i < TASK_COUNT; i++){
		Sorted_Task[i].state = SUSPENDED;
		Sorted_Task[i].id = NULL;
		Sorted_Task[i].Priority = NULL;
		// Sorted_Task[i].Period = NULL;
		// Sorted_Task[i].WCET = NULL;
	}

    /* CPU Declaration */
    
    /* CPU_form = {state, id, selected_task} */
    core_descriptor [CORE_COUNT] List_Core;
		List_Core[0].state = IDLE;
		List_Core[0].id = 0;
		List_Core[0].selected_task = NULL;
		List_Core[1].state = IDLE;
		List_Core[1].id = 1;
		List_Core[1].selected_task = NULL;


// insert here the state variables declarations 
// and possibly some code to initialize them 
// using C-like syntax
 
 }