import sys
# import math

def parse_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()

    times = []
    to_values = []

    for line in lines:
        if line.startswith("Time:"):
            time_value = float(line.split(":")[1].strip().split()[0])
            times.append(time_value)
        elif line.startswith("TO  :"):
            to_value = int(line.split(":")[1].strip())
            to_values.append(to_value)

    return times, to_values

def calculate_average_to_value(data):
    return sum(data) / len(data) if len(data) > 0 else 0

def calculate_average_times(data):
    return sum(data) / len(data) if len(data) > 0 else 0

def write_averages_to_file(avg_time, avg_to_value, output_file_path, n, i):
    with open(output_file_path, 'a') as output_file:
        output_file.write(f"N = {n}, results_n{n}_{i}.txt \n")
        output_file.write(f"Average Time: {avg_time} s\n")
        output_file.write(f"Average Solution: {avg_to_value}\n\n")

if __name__ == "__main__":

    n = sys.argv[1]
    tsc = sys.argv[1]           # e.g. TSC1 (Tabu Search Calibration 1)
    instance_type = sys.argv[2] # "random" or "regularity"
    for n in range(40, 141, 20):
        for i in range(0, 3):
            file_path = "test/results/" + tsc + "/" + instance_type + "/n" + str(n) + "/results_n" + str(n) + "_" + str(i) + ".txt"  # Replace with the actual file path
            output_file_path = "averages/" + tsc + "_" + instance_type + ".txt"

            times, to_values = parse_file(file_path)

            avg_time = calculate_average_times(times)
            avg_to_value = calculate_average_to_value(to_values)

            write_averages_to_file(avg_time, avg_to_value, output_file_path, str(n), str(i))
