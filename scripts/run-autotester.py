import os
import subprocess
import json

with open('source_query_pairs.json', 'r') as f:
    data = json.load(f)
    pairs = data.get("pairs", [])


for pair in pairs:
    source_file = os.path.join("../../../tests", pair["source"])
    query_file = os.path.join("../../../tests", pair["query"])

    if os.path.exists(source_file) and os.path.exists(query_file):
        query_file_w_extension = os.path.basename(query_file)
        query_file_name = os.path.splitext(query_file_w_extension)[0]
        output_file = os.path.join("../../../tests", f"{query_file_name}_out.xml")
        print(["./autotester", source_file, query_file, output_file])
        subprocess.run(["./autotester", source_file, query_file, output_file], check=True)
    else:
        print(f"Error: One or both files do no exist: {source_file}, {query_file}")