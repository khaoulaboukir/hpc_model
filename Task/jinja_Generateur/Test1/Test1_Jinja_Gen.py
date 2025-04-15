import json
from jinja2 import Environment, FileSystemLoader
import os

base_path = "/media/khalil-hamdoune/Khalil/Application/romeo-3.10.2/Test Jinja/Test1"

with open(os.path.join(base_path, 'Test1_Data.json')) as f:
    data = json.load(f)

nmbr_cpu = sum(1 for core in data["cores"] if core["type"] == "CPU")
nmbr_gpu = sum(1 for core in data["cores"] if core["type"] == "GPU")

for task in data["tasks"]:
    assert "id" in task, "❌ Il manque un champ 'id' dans une tâche"
    assert "execution_time" in task, f"❌ Il manque 'execution_time' dans la tâche {task.get('id', '?')}"
    assert "BCET" in task["execution_time"] and "WCET" in task["execution_time"], f"❌ BCET/WCET manquant dans la tâche {task['id']}"

env = Environment(loader=FileSystemLoader(base_path))
template = env.get_template('Test1_Template.xml.j2')

output = template.render(
    path=data["path"],
    cores=data["cores"],
    tasks=data["tasks"],
    globals=data["globals"],
    nb_cpu=nmbr_cpu,
    nb_gpu=nmbr_gpu,
    task_count=len(data["tasks"]),
    core_count=len(data["cores"])
)

output_file = os.path.join(base_path, f"{data['path']}_out.xml")

with open(output_file, 'w') as f:
    f.write(output)

print("Fichier ROMEO bien généré")
