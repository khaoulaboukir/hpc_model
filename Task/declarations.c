// insert here your type definitions using C-like syntax

// insert here your function definitions 
// using C-like syntax

/* Contants and identifiers */
const int NEED_SCHEDULE = 1;
const int NO_NEED_SCHEDULE = 0;
const int TASK_COUNT = 2;
const int CORE_COUNT = 1;

const int NULL = -1;




/* Task data structures */
typedef struct{
	task_state state;
	int id;
	int Priority;
	int Period;
	int WCET;
}task_descriptor;



typedef enum{
    SUSPENDED,
    READY,
    RUNNING
}task_state;


/* Task lists + mainting functions*/
int add_task(int id){
    int result = 1;


    result = sort_list();
    return result;
}

int remove_task(int id){
    int result = 1:


    result = sort_list();
}

int sort_list(){
    int result = 1;

    return result;
}



/* Scheduler functions */
int start(){
    int result = 1;
    
    return result;
}

int preempet()




/* Task API */
int is_ready(TASK [Task_count] tab_Task, int id) {
	if(tab_Task[id].id == id){
		tab_Task[id].state = READY;
		return 1;
	}
	else{
		return 0;
	}
}

 int activate_task(int task_id){
    int result = 1;

    return result;
 }

int terminate_task(int task_id){
    int result = 1;

    return result;

}









// int Is_Task_Completed(TASK [Task_count] tab_Task, int id, int clock, int start_time){
// int i;
// for(i = 0; i < Task_count; i++){
// if (tab_Task[i].id == id){
// if ((clock - start_time) >= tab_Task[i].WCET){
// return 1;
// }
// return 0;
// }
// }
// return 0;
// }

int Terminate_Task(TASK [Task_count] tab_Task, int id){
int i;
	for(i = 0; i < Task_count; i++){
		if (tab_Task[i].id == id){
			tab_Task[i].Statut.Waiting = 0;
			tab_Task[i].Statut.Running = 0;
			tab_Task[i].Statut.Suspend = 1;
			return tab_Task[i].WCET;
		}
	}
}

// typedef struct{
// TASK [Task_count] tab_Task;
// int cmpt;
// }two_Var;

// two_Var Add_Task(TASK [Task_count] tab_Task, int cmpt, int id, int p, int t, int w){
// two_Var result;
// tab_Task[cmpt].Statut.Waiting = 1;
// tab_Task[cmpt].Statut.Suspend = 0;
// tab_Task[cmpt].Statut.Running = 0;
// tab_Task[cmpt].id = id;
// tab_Task[cmpt].Priority = p;
// tab_Task[cmpt].Period = t;
// tab_Task[cmpt].WCET = w;
// cmpt ++;
// result.tab_Task = tab_Task;
// result.cmpt = cmpt;
// return result;
// }

int Add_Task(TASK [Task_count] tab_Task, int id, int p, int t, int w){
	tab_Task[id].Statut.Waiting = 1;
	tab_Task[id].Statut.Suspend = 0;
	tab_Task[id].Statut.Running = 0;
	tab_Task[id].id = id;
	tab_Task[id].Priority = p;
	tab_Task[id].Period = t;
	tab_Task[id].WCET = w;
	return tab_Task;
}

int Sort(TASK [Task_count] tab_Task){
int i, j, max_idx;
int Selected_Task = NULL;
TASK temp;
int wait_count = 0;
	for (i = 0; i < Task_count; i++){
		if (tab_Task[i].Statut.Waiting == 1){
			temp = tab_Task[i];
			tab_Task[i] = tab_Task[wait_count];
			tab_Task[wait_count] = temp;
			wait_count = wait_count + 1;
		}
	}
	for(i = 0; i < wait_count - 1; i++){
		max_idx = i;
		for(j = i +1; j < wait_count; j++){
			if (tab_Task[j].Priority > tab_Task[max_idx].Priority){
				max_idx = j;
			}
		}
		if (max_idx != i){
			temp = tab_Task[i];
			tab_Task[i] = tab_Task[max_idx];
			tab_Task[max_idx] = temp;
		}
	}
	for(i = 0; i < wait_count; i++){
		if(tab_Task[i].Statut.Waiting == 1 && Selected_Task == NULL){
			Selected_Task = tab_Task[i].id;
		}
	}
	return Selected_Task;
}

int Exec_Task(TASK [Task_count] tab_Task, int id){
int i;
	for(i = 0; i < Task_count; i++){
		if (tab_Task[i].id == id){
			tab_Task[i].Statut.Waiting = 0;
			tab_Task[i].Statut.Running = 1;
			return 0;
		}
	}
	return 1;
}

int Wait_Task(TASK [Task_count] tab_Task, int id){
int i;
	for(i = 0; i < Task_count; i++){
		if (tab_Task[i].id == id){
			tab_Task[i].Statut.Waiting = 1;
			tab_Task[i].Statut.Running = 0;
			return 1;
		}
		return 0;
	}
}

void Susp_Task(TASK [Task_count] tab_Task, int id){
int i;
	for(i = 0; i < Task_count; i++){
		if (tab_Task[i].id == id){
			tab_Task[i].Statut.Running = 0;
			tab_Task[i].Statut.Suspend = 1;
		}
	}
}

int period_Task(TASK [Task_count] tab_Task, int id){
int i;
	for(i = 0; i < Task_count; i++){
		if (tab_Task[i].id == id){
			return tab_Task[i].Period;
		}
	}
}

int other_Task_Waiting(TASK [Task_count] tab_Task){
int i;
	for(i = 0; i < Task_count; i++){
		if (tab_Task[i].Statut.Waiting == 1){
			return 1;
		}
	}
	return 0;
}

initially {
int clock = 0, start_time = 0; 
// int cmpt = 0;
int task_in_process = NULL;
int wt = NULL;
int Completed_Task = NULL;
// for (int i = 0; i < Task_count; i++){
// tab_Task[i].Statut.Running = 0;
// tab_Task[i].Statut.Waiting = 0;
// tab_Task[i].Statut.Suspend = 1;
// tab_Task[i].id = NULL;
// tab_Task[i].Priority = NULL;
// tab_Task[i].Period = NULL;
// tab_Task[i].WCET = NULL;
// }
 TASK [Task_count] tab_Task={{{0, 0, 1}, NULL, NULL, NULL, NULL},{{0, 0, 1}, NULL, NULL, NULL, NULL}};

// insert here the state variables declarations 
// and possibly some code to initialize them 
// using C-like syntax
}