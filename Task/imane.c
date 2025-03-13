// insert here the state variables declarations 
// and possibly some code to initialize them 
// using C-like syntax

const int UNLOCKED = 0;
const int NONE = -1;

const int RUNNING = 0;
const int WAITING = 1;
const int READY = 2;
const int SUSPENDED = 3;

const int TASK_COUNT = 2;

const int NO_IT = 0;
const int SEND_IT = 1;

typedef struct {
  int prio;
  int state;
} TaskDescriptor;

typedef struct {
  TaskDescriptor[TASK_COUNT] tasks;
  int running;
  int waiting;
  int it_pending;
  int locked;
} KernelState;

typedef struct {
  int gate;
  int it;
} SEMA4Gate;

initially {
  SEMA4Gate gate = { UNLOCKED, NO_IT };
  int caller = NONE;
  int[2] endOfGetSpinLock = {0, 0};
  int[2] endOfGetSpinLockIT = {0, 0};
  int[2] endOfReleaseSpinLock = {0, 0};
  KernelState kern = {
    {
      {1,RUNNING}, {0,READY}
    },
    0,
    NONE,
    0,
    0
  };
}

// insert here your type definitions using C-like syntax


// insert here your function definitions 
// using C-like syntax

void lock(int core, SEMA4Gate &ioGate)
{
   if (ioGate.gate == UNLOCKED) {
     ioGate.gate = core + 1;
   }
}

void Schedule(KernelState &inKern)
{
  int rdy = NONE;
  /* get the highest priority ready task if any */
  for (int i = 0; i < TASK_COUNT; i++) {
    if (inKern.tasks[i].state == READY) {
      rdy = i;
    }
  }

  if (inKern.running == NONE) {
    /*
     * If no running task available, elect the rdy.
     */
    if (rdy != NONE) {
      inKern.running = rdy;
      inKern.tasks[rdy].state = RUNNING;
    }
  } else {
    /* 
     * A running task exists, replace it if rdy exists
     * and has a higher priority.
     */
    if (rdy != NONE) {
      if (inKern.tasks[rdy].prio > inKern.tasks[inKern.running].prio) {
        inKern.tasks[inKern.running].state = READY;
        inKern.tasks[rdy].state = RUNNING;
        inKern.running = rdy;
      }
    }
  }
}


void unWait(KernelState &inKern)
{
  if (inKern.waiting != NONE) {
    inKern.tasks[inKern.waiting].state = READY;
    inKern.waiting = NONE;
    Schedule(inKern);
  }
}

void unlock(SEMA4Gate &ioGate, KernelState &ioKern)
{
  ioGate.gate = UNLOCKED;
  if (ioGate.it == SEND_IT) {
    ioGate.it = NO_IT;
    ioKern.it_pending = 1;
  }
}

int isLockedBy(int core, SEMA4Gate inGate)
{
  if (inGate.gate == core + 1) {
    return 1;
  } else {
    return 0;
  } 
}

int isRunning(int core, KernelState &inKern, int id)
{
  if (core == 0) {
    if (inKern.running == id) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 1;
  } 
}

void wait(KernelState &inKern, int inCaller)
{
  if (inCaller == inKern.running) {
    inKern.running = NONE;
    inKern.tasks[inCaller].state = WAITING;
    inKern.waiting = inCaller;
    Schedule(inKern);
  }
}

void Terminate(KernelState &ioKern)
{
  ioKern.tasks[ioKern.running].state = SUSPENDED;
  ioKern.running = NONE;
  Schedule(ioKern);
}
