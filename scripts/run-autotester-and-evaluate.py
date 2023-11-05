import os
import subprocess
import sys
import run_evaluator
from collections import defaultdict
from pathlib import Path
import json
import xml.etree.ElementTree as ET

NUM_RUNS = 20

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
                # output_file = os.path.join("../../../tests", f"{query_file_name}_out.xml")
                print(["./autotester", source_file, query_file, output_file])
                os.system(str("." / autotester_dir / "autotester") + " " + source_file + " " + query_file + " " + output_file)
                # subprocess.run(["." / autotester_dir / "autotester", source_file, query_file, output_file], check=True)
            else:
                print(f"Error: One or both files do no exist: {source_file}, {query_file}")
        
        # os.system("python3 run-autotester.py")
        for output_file in test_dir.glob("*.xml"):
            if (str(output_file) != "analysis.xml"):
                tree = ET.parse(str(output_file))
                root = tree.getroot()
                time_taken = run_evaluator.calculate_average_time_taken(root)
                timings[output_file].append(time_taken)

    print("Average timings over", NUM_RUNS, "run(s):")
    outputs = []
    for output_file, timing in timings.items():
        outputs.append(str(output_file) + " : " + str(sum(timing) / len(timing)))
    outputs.sort()
    for output in outputs:
        print(output)
