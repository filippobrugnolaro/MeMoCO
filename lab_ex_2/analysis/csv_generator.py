import csv
import re
import sys

tsc = sys.argv[1] # e.g. TSC1
type_instance = sys.argv[2] # random, regularity

# Read input text from file
file_path = "averages/" + tsc + "_" + type_instance + ".txt"  # Change this to the actual path of your input file
file_path_final = "averages/TSC_final_" + type_instance + ".txt"  # Change this to the actual path of your input file
error_file_path = "errors/" + tsc + "_" + type_instance + ".txt"  # Change this to the actual path of your input file
error_file_path_final = "errors/TSC_final_" + type_instance + ".txt"  # Change this to the actual path of your input file
with open(file_path, 'r') as file:
    input_text = file.read()

with open(error_file_path, 'r') as file:
    error_text = file.read()

# Initialize CSV data
csv_data = []
csv_data.append(["\hline"])

# Parse input text and extract relevant information
pattern = re.compile(r'N = (\d+), results_(\w+)_(\d+)\.txt\s+Average Time: (.+) s\s+Average Solution: (.+)')
matches = pattern.findall(input_text)

i=0
for match in matches:
    n, pos, index, exec_time, solution = match
    percentage_error = error_text.splitlines()[i]
    csv_data.append([f"pos\_n{n}\_{index} & " + str(int(float(solution))) + " & " + "{:.3f}".format(float(exec_time)) + " & " + "{:.2f}".format(float(percentage_error)) + " \\\\"])
    csv_data.append(["\hline"])
    i+=1

# Write CSV data to a file
output_file_path = "csv/" + tsc + "_" + type_instance + ".csv"
output_file_path_final = "csv/TSC_final_" + type_instance + ".csv"
with open(output_file_path, 'w', newline='') as csv_file:
    csv_writer = csv.writer(csv_file)
    csv_writer.writerows(csv_data)