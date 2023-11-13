import os
import subprocess
import sys
import run_evaluator
from collections import defaultdict
from pathlib import Path
import json
import xml.etree.ElementTree as ET

NUM_RUNS = 10

def check_output_all_correct(root, name):
    wrong_queries = []
    for query_idx, query in enumerate(root[1]):
        missing_list = query.findall(".//missing")
        additional_list = query.findall(".//additional")
        timeout_list = query.findall(".//timeout")
        if missing_list or additional_list or timeout_list:
            wrong_queries.append(query_idx)
    if wrong_queries:
        print(
                name +
                ("." * (80 - len(name))) +
                f"Queries {', '.join(map(str, wrong_queries))} is/are wrong"
                )

if __name__ == "__main__":
    autotester_dir = Path(sys.argv[1])

    test_dir = autotester_dir / "../../../tests"
    timings = defaultdict(list)

    with open("source_query_pairs.json", "r") as f:
        data = json.load(f)
        pairs = data.get("pairs", [])

    for _ in range(NUM_RUNS):
        for pair in pairs:
            source_file = str(test_dir / pair["source"])
            query_file = str(test_dir / pair["query"])

            if os.path.exists(source_file) and os.path.exists(query_file):
                query_file_w_extension = os.path.basename(query_file)
                query_file_name = os.path.splitext(query_file_w_extension)[0]
                output_file = str(test_dir / f"{query_file_name}_out.xml")
                print(["./autotester", source_file, query_file, output_file])
                os.system(str("." / autotester_dir / "autotester") + " " + source_file + " " + query_file + " " + output_file)
            else:
                print(f"Error: One or both files do no exist: {source_file}, {query_file}")
        
        for output_file in test_dir.glob("*.xml"):
            tree = ET.parse(str(output_file))
            root = tree.getroot()
            time_taken = run_evaluator.calculate_average_time_taken(root)
            timings[output_file].append(time_taken)

    print("\n" + "-" * 50 + f"AVERAGE TIMINGS OVER {NUM_RUNS} RUN(S)" + "-" * 50)
    outputs = []
    for output_file, timing in timings.items():
        name = str(output_file)[str(output_file).find("tests/") + 6:]
        outputs.append(
                name +
                ("." * (80 - len(name))) +
                str(sum(timing) / len(timing))
                )
    outputs.sort()
    for output in outputs:
        print(output)

    print("-" * 50 + "Queries with wrong outputs:" + "-" * 50)
    for output_file in test_dir.glob("*.xml"):
        name = str(output_file)[str(output_file).find("tests/") + 6:]
        tree = ET.parse(str(output_file))
        root = tree.getroot()
        check_output_all_correct(root, name)
    print("-" * 127)

