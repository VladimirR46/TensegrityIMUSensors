import matplotlib.pyplot as plt
import numpy as np
import math

file = open( "mod.txt", "r" )

val = []
val2 = []
for line in file:
    values = [ i for i in line.strip().split( " " ) ]
    #print(values)
    val.append(float(values[4]))
    val2.append(float(values[3]))

plt.plot(val)
plt.plot(val2)
plt.show()
file.close()
