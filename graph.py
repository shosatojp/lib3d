import numpy as np
from matplotlib import pyplot as plt

x = np.arange(1, 200, 0.01)
f = 100
n = 10
y = (x-n)/(f-n) * f/x
plt.plot(x, y)
plt.show()
