import os
import subprocess
import json
import csv
import xml.etree.ElementTree as ET

def extract_parsing_time(xml_file):
    try:
        tree = ET.parse(xml_file)
        root = tree.getroot()
        parsing_time = root.find('.//parsing_time_taken')
        if parsing_time is not None:
            return float(parsing_time.text)
    except ET.ParseError:
        print(f"Error parsing XML file: {xml_file}")
    return None

with open('source_query_pairs.json', 'r') as f:
    data = json.load(f)
    pairs = data.get("pairs", [])

# Open the CSV file for writing
with open('parsing_times.csv', 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    # Write the header
    csvwriter.writerow(['Query File', 'Parsing Time Taken'])

    for pair in pairs:
        source_file = os.path.join("../../../tests", pair["source"])
        query_file = os.path.join("../../../tests", pair["query"])

        if os.path.exists(source_file) and os.path.exists(query_file):
            query_file_w_extension = os.path.basename(query_file)
            query_file_name = os.path.splitext(query_file_w_extension)[0]
            output_file = os.path.join("../../../tests", f"{query_file_name}_out.xml")
            print(["./autotester", source_file, query_file, output_file])
            subprocess.run(["./autotester", source_file, query_file, output_file], check=True)

            # Extract parsing time and write to CSV
            parsing_time = extract_parsing_time(output_file)
            if parsing_time is not None:
                csvwriter.writerow([query_file_name, parsing_time])
            else:
                print(f"Could not extract parsing time for {query_file_name}")
        else:
            print(f"Error: One or both files do not exist: {source_file}, {query_file}")
