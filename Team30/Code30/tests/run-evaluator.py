import sys
import xml.etree.ElementTree as ET

def calculate_average_time_taken(root):
    """
    Calculate the average of all numerical values within 'time_taken' tags.
    """
    # Find all elements with the 'time_taken' tag
    time_taken_elements = root.findall(".//time_taken")

    # Extract the numbers from these elements and convert them to float
    try:
        times = [float(element.text) for element in time_taken_elements]
    except ValueError:
        print("Error: Non-numerical value found within 'time_taken' elements.")
        return

    # Calculate and return the average
    if times:
        average_time = sum(times) / len(times)
        return average_time
    else:
        print("No 'time_taken' elements found.")
        return

def main(xml_path):
    try:
        # Parsing the XML file
        tree = ET.parse(xml_path)
        root = tree.getroot()

        # Calculate the average time taken
        average_time = calculate_average_time_taken(root)

        if average_time is not None:
            print(f"Average time taken: {average_time}")
    except ET.ParseError as e:
        print(f'An error occurred while parsing the XML file: {e}')
    except FileNotFoundError:
        print('The file was not found. Please check the provided file path.')
    except Exception as e:
        print(f'An unexpected error occurred: {e}')

if __name__ == "__main__":
    # Checking if the path argument was provided
    if len(sys.argv) < 2:
        print("Usage: python {} path_to_xml".format(sys.argv[0]))
        sys.exit(1)

    # The path to the XML file is the first command-line argument
    path_to_xml = sys.argv[1]

    # Call the main function with the path to the XML
    main(path_to_xml)
