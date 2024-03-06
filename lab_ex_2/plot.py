import matplotlib.pyplot as plt
import sys

if len(sys.argv) == 3:       # ./plot.py <number of points> <random>
    file_path = "test/posFiles/pos_n" + sys.argv[1] + "_" + sys.argv[2] + ".dat"
    saving_path = "test/images/" + "/plot_n" + sys.argv[1] + "_" + sys.argv[2] + ".png"
else:
    raise ValueError("Non valid number of arguments. Please provide at least 2 arguments.")

with open(file_path, 'r') as file:
    num_points = int(file.readline().strip())
    lines = file.readlines()

points = [tuple(map(float, line.strip().split())) for line in lines]

if num_points != len(points):
    raise ValueError("Number of points specified in the file does not match the actual number of points.")

x_coordinates, y_coordinates = zip(*points)

ax = plt.figure().gca()
ax.yaxis.set_major_locator(plt.MaxNLocator(integer=True))
ax.xaxis.set_major_locator(plt.MaxNLocator(integer=True))

plt.scatter(x_coordinates, y_coordinates, color='red', marker='.')

plt.yticks(range(0, int(sys.argv[1]) + 1, int(sys.argv[1]) // 10))
plt.xticks(range(0, int(sys.argv[1]) + 1, int(sys.argv[1]) // 10))

plt.title("Scatter Plot of " + "plot_n" + sys.argv[1] + "_" + sys.argv[2] + ".dat")
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)
plt.savefig(saving_path)
print("Plot " + sys.argv[2] + " saved correctly")
