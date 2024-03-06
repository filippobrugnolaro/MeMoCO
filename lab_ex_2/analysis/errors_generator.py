import csv, sys

# Assuming you have the CSV data in a file named 'input.csv'
tsc = sys.argv[1] # e.g. TSC1
type_instance = sys.argv[2] # random, regularity

cplex_file = 'CPLEX_csv/CPLEX_' + type_instance + '.csv'
tsc_file = 'csv/' + tsc + '_' + type_instance + '.csv'
tsc_file_final = 'csv/TSC_final_' + type_instance + '.csv'
output_file_path = 'errors/' + tsc + "_" + type_instance + ".txt"
output_file_path_final = 'errors/' + "TSC_final_" + type_instance + ".txt"
output_table_path = 'errors/' + tsc + "_" + type_instance + "_table.txt"

def extract_second_column_from_csv(input_file):
    with open(input_file, 'r') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        data = [row[0].replace(" ", "").split('&') for row in csv_reader]
        data = [arr for arr in data if '\\hline' not in arr]
        data = [int(arr[1]) for arr in data]
    return data

def extract_second_column_cplex(input_file):
    with open(input_file, 'r') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        data = [row[0].replace(" ", "").split('&') for row in csv_reader]
        data = [arr for arr in data if '\\hline' not in arr]
        result = []
        for i in range(0, len(data), 5):
            result.extend([int(data[i][1]), int(data[i + 1][1]), int(data[i + 2][1])])
    return result

def extract_second_column_cplex_total(input_file):
    with open(input_file, 'r') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        data = [row[0].replace(" ", "").split('&') for row in csv_reader]
        data = [arr for arr in data if '\\hline' not in arr]
        result = []
        for i in range(0, len(data), 3):
            result.extend([int(data[i][1]), int(data[i + 1][1]), int(data[i + 2][1])])
    return result

def calculate_percentage_error(data1, data2):
    return [round((float(abs(data2[i] - data1[i])) / float(data1[i])) * 100, 2) for i in range(len(data1))]

if __name__ == "__main__":
    data1 = []
    data2 = []
    if len(sys.argv) > 3:
        data1 = extract_second_column_cplex_total(cplex_file)
        data2 = extract_second_column_from_csv(tsc_file_final)
    else:
        data1 = extract_second_column_cplex(cplex_file)
        data2 = extract_second_column_from_csv(tsc_file)
    percentage_error = calculate_percentage_error(data1, data2)
    with open(output_file_path_final, 'a') as output_file:
        for i in range(len(percentage_error)):
            output_file.write(f"{percentage_error[i]}\n")