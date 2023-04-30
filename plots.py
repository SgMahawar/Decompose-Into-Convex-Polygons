import matplotlib.pyplot as plt
import numpy as np
import ast

polygons=[]
with open('output.txt', 'r') as file:
    # Read the array as a string and convert it to a Python object
    input_str = file.read().strip()
    input_array = ast.literal_eval(input_str)
    polygons=input_array

# Define the vertices of the polygon
# polygons=[[(0,0),(0,2),(2,2),(1,1)],
#             [(1,1),(2,0),(3,-2),(1,-1)],
#             [(1,-1),(-1,-2),(-1,1),(0,0)],
#             [(0,0),(1,1),(1,-1),(0,0)]]

for polygon in polygons:
    polygon.append(polygon[0])

print(polygon)
# Create a figure and axis object
fig, ax = plt.subplots()
colors = "bgrcmykw"
color_index = 0
# Plot the polygon using the vertices
for vertices in polygons:
    print(vertices)
    ax.plot([vertex[0] for vertex in vertices],
            [vertex[1] for vertex in vertices],
            color=colors[color_index])
    color_index+=1
    color_index=color_index%8

# Set the axis limits and title
# ax.set_xlim(-1, 3)
# ax.set_ylim(-2, 2)
ax.set_title('Merged Polygons')

# Display the plot
plt.show()
