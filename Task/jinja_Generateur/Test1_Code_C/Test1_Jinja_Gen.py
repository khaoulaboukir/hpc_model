import json
from jinja2 import Environment, FileSystemLoader
import os

base_path = "/media/khalil-hamdoune/Khalil/Application/romeo-3.10.2/Test Jinja/Test1"

template_path = os.path.join(base_path, 'Templates')
result_path = os.path.join(base_path, 'Resultats')

with open(os.path.join(base_path, 'Test1_Data.json')) as f:
    data = json.load(f)

nmbr_cpu = sum(1 for core in data["cores"] if core["type"] == "CPU")
nmbr_gpu = sum(1 for core in data["cores"] if core["type"] == "GPU")

env = Environment(loader=FileSystemLoader(template_path))

c_template = env.get_template('Test1_Code.c.j2')
c_code = c_template.render(task_count=len(data["simple_tasks"]), core_count=len(data["cores"]))

c_filename = f"{data['path']}_Code.c"
with open(os.path.join(result_path, c_filename), 'w') as f:
    f.write(c_code)

seen_ids = {}
for i, task in enumerate(data["simple_tasks"]):
    assert "id" in task, f"La tâche à l'index {i} n'a pas de champ 'id'"
    task_id = task["id"]

    if task_id in seen_ids:
        raise ValueError(
            f"Tâches avec ID en double : tâche {seen_ids[task_id]} et tâche {i} ont le même id = {task_id}")
    seen_ids[task_id] = i

    assert "execution_time" in task, f"La tâche {task_id} n'a pas de champ 'execution_time'"
    assert "BCET" in task["execution_time"], f"BCET manquant dans la tâche {task_id}"
    assert "WCET" in task["execution_time"], f"WCET manquant dans la tâche {task_id}"

template = env.get_template('Test1_Template.xml.j2')
output = template.render(
    path=data["path"],
    cores=data["cores"],
    simple_tasks=data["simple_tasks"],
    globals=data["globals"],
    nb_cpu=nmbr_cpu,
    nb_gpu=nmbr_gpu,
    task_count=len(data["simple_tasks"]),
    core_count=len(data["cores"]),
    c_file=c_filename
)

xml_filename = f"{data['path']}_out.xml"
with open(os.path.join(result_path, xml_filename), 'w') as f:
    f.write(output)

print("Fichier ROMEO bien générer")