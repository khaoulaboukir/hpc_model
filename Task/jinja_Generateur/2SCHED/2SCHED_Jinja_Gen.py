import json
from jinja2 import Environment, FileSystemLoader
import os
from collections import defaultdict

base_path = "/media/khalil-hamdoune/Khalil/Application/romeo-3.10.2/Test Jinja/Done_Test/Before/2SCHED"
template_path = os.path.join(base_path, 'Templates')
result_path = os.path.join(base_path, 'Resultats')

with open(os.path.join(base_path, '2SCHED_Data.json')) as f:
    data = json.load(f)

# Calcul du nombre de CPU et GPU
nmbr_cpu = sum(1 for core in data["cores"] if core["type"] == "CPU")
nmbr_gpu = sum(1 for core in data["cores"] if core["type"] == "GPU")

# Initialisation de Jinja
env = Environment(loader=FileSystemLoader(template_path), trim_blocks=True, lstrip_blocks=True)

# Rendu du fichier C
task_list = [subtask for task in data["tasks"] for subtask in task["subtasks"]]
c_template = env.get_template('2SCHED_Code.c.j2')
c_code = c_template.render(task_count=len(task_list), core_count=len(data["cores"]))

c_filename = f"{data['path']}_Code.c"
with open(os.path.join(result_path, c_filename), 'w') as f:
    f.write(c_code)

# Vérification des tâches (unicité et champs nécessaires)
seen_ids = {}
for i, task in enumerate(task_list):
    if task.get("type") == "endCond" or "Cond" or "Alt" or "endAlt":
        continue

    assert "id" in task, f"La tâche à l'index {i} n'a pas de champ 'id'"
    task_id = task["id"]
    if task_id in seen_ids:
        raise ValueError(f"Tâches avec ID en double : tâche {seen_ids[task_id]} et tâche {i} ont le même id = {task_id}")
    seen_ids[task_id] = i
    assert "execution_time" in task, f"La tâche {task_id} n'a pas de champ 'execution_time'"
    assert "BCET" in task["execution_time"], f"BCET manquant dans la tâche {task_id}"
    assert "WCET" in task["execution_time"], f"WCET manquant dans la tâche {task_id}"

# Détection des sous-tâches sans prédécesseurs
def get_independent_subtasks(tasks):
    all_independent = []
    for task_index, task in enumerate(tasks):
        period = task.get("period")
        for st in task.get("subtasks", []):
            if not st.get("predecessors"):  # si vide ou inexistant
                st["dag_period"] = period
                st["dag_index"] = task_index
                all_independent.append(st)
    return all_independent, len(all_independent)

src_node, count_src_node = get_independent_subtasks(data["tasks"])

simple_tasks = []
complex_tasks = []

for task in data["tasks"]:
    if len(task["subtasks"]) == 1:
        simple_tasks.append(task)
    elif len(task["subtasks"]) > 1:
        complex_tasks.append(task)

count_simple_tasks = len(simple_tasks)

src = []
for task in complex_tasks:
    sources = [st for st in task["subtasks"] if not st.get("predecessors")]
    src.append({
        "task": task,
        "src_nodes": sources
    })

for entry in src:
    task = entry["task"]
    sources = entry["src_nodes"]
    other_nodes = [st for st in task["subtasks"] if st not in sources]
    entry["other_nodes"] = other_nodes

core_types = {core["type"] for core in data["cores"]}
P_Ord = {core["P_Ord"] for core in data["cores"]}

scheduler_models = defaultdict(int)

for core in data["cores"]:
    key = (core["type"], core["P_Ord"])
    scheduler_models[key] += 1

template = env.get_template('2SCHED_Template.xml.j2')
output = template.render(
    path=data["path"],
    cores=data["cores"],
    tasks=data["tasks"],
    core_types=core_types,
    P_Ord=P_Ord,
    task_list=task_list,
    scheduler_models=scheduler_models,
    simple_tasks=simple_tasks,
    complex_tasks=complex_tasks,
    src=src,
    src_node=src_node,
    count_src_node=count_src_node,
    count_simple_tasks=count_simple_tasks,
    globals=data["globals"],
    globals_tab=data["globals_tab"],
    nb_cpu=nmbr_cpu,
    nb_gpu=nmbr_gpu,
    task_count=len(task_list),
    core_count=len(data["cores"]),
    cpu_count=nmbr_cpu,
    gpu_count=nmbr_gpu,
    exec_ids={"map": {}}
)

xml_filename = f"{data['path']}_out.xml"
with open(os.path.join(result_path, xml_filename), 'w') as f:
    f.write(output)

print("Fichier ROMEO bien généré")
