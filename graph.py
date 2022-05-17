
from matplotlib import pyplot as plt
from matplotlib import style
import sys
from numpy import genfromtxt
graph  = "RTTOutput.txt"
data= genfromtxt(graph, delimiter=",", names=["x", "y"])
plt.xlabel('Hop Number')
plt.ylabel('RTT(in ms)')
plt.bar(data['x'], data['y'])
savegraph = "RTT_VS_HOPNUMBER.png"
plt.savefig(savegraph)
plt.show()
