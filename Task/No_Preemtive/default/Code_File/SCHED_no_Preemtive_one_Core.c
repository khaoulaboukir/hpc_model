// insert here your type definitions using C-like syntax

// insert here your function definitions 
// using C-like syntax


/* Const */

const int TASK_COUNT = 2;
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
	int WCET;
}task_descriptor;


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

void insert_task(task_descriptor [TASK_COUNT] &List_Task, task_descriptor [TASK_COUNT] &Sorted_Task, int id){
    int i, index = NULL;

    for (i = 0; i < TASK_COUNT; i++) {
        if (List_Task[i].id == id) {
            index = i;
        }
    }

    for(i = 0; i < TASK_COUNT - 1; i++){
        Sorted_Task[i] = Sorted_Task[i + 1];
    }

    Sorted_Task[TASK_COUNT - 1] = List_Task[index];
    Sorted_Task[TASK_COUNT - 1].state = READY;
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


/* Scheduler functions */

int scheduler(task_descriptor [TASK_COUNT] &Sorted_Task){
    Sorted_Task = sort_list(Sorted_Task);
    return Sorted_Task[0].id;
}

int no_need_scheduler(task_descriptor [TASK_COUNT] &Sorted_Task){
    Sorted_Task[0].state = SUSPENDED;
    Sorted_Task[0].Priority = NULL;
    Sorted_Task = sort_list(Sorted_Task);
    if(Sorted_Task[0].id == NULL){
        return NULL;
    }
    else{
        return Sorted_Task[0].id;
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
		List_Task[0].WCET = 2;
		List_Task[1].state = SUSPENDED;
		List_Task[1].id = 1;
		List_Task[1].Priority = 2;
		// List_Task[1].Period = 7;
		List_Task[1].WCET = 3;
	
	task_descriptor [TASK_COUNT] Sorted_Task;
	int i;
    for(i = 0; i < TASK_COUNT; i++){
		Sorted_Task[i].state = SUSPENDED;
		Sorted_Task[i].id = NULL;
		Sorted_Task[i].Priority = NULL;
		// Sorted_Task[i].Period = NULL;
		Sorted_Task[i].WCET = NULL;
	}


// insert here the state variables declarations 
// and possibly some code to initialize them 
// using C-like syntax
 
 }