import matplotlib.pyplot as plt
import sys

if len(sys.argv) == 3:       # ./plot.py <number of points> <instance_type>
    file_path = "test/posFiles/pos_n" + sys.argv[1] + "_" + sys.argv[2] + ".dat"
    saving_path = "test/images/" + "/plot_n" + sys.argv[1] + "_" + sys.argv[2] + ".png"
else:
    raise ValueError("Non valid number of arguments. Please provide at least 2 arguments.")

# Read the file
with open(file_path, 'r') as file:
    num_points = int(file.readline().strip())
    lines = file.readlines()

# Get the points
points = [tuple(map(float, line.strip().split())) for line in lines]

# Check if the number of points specified in the file matches the actual number of points
if num_points != len(points):
    raise ValueError("Number of points specified in the file does not match the actual number of points.")

# Unzip the points
x_coordinates, y_coordinates = zip(*points)

# Set the major ticks
ax = plt.figure().gca()
ax.yaxis.set_major_locator(plt.MaxNLocator(integer=True))
ax.xaxis.set_major_locator(plt.MaxNLocator(integer=True))

# Plot the points
plt.scatter(x_coordinates, y_coordinates, color='red', marker='.')

# Set the ticks
plt.yticks(range(0, int(sys.argv[1]) + 1, int(sys.argv[1]) // 10))
plt.xticks(range(0, int(sys.argv[1]) + 1, int(sys.argv[1]) // 10))

# Set the title, labels and grid
plt.title("Scatter Plot of " + "plot_n" + sys.argv[1] + "_" + sys.argv[2] + ".dat")
plt.xlabel('X')
plt.ylabel('Y')
plt.grid(True)

# Save the plot
plt.savefig(saving_path)
print("Plot " + sys.argv[2] + " saved correctly")
