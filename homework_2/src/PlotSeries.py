#!/usr/bin/env python3

import sys
import numpy as np
import matplotlib.pyplot as plt
import os

# Print when arguments are missing
if len(sys.argv) < 2:
    print('Info: python3 Plot.py [fileName.extension] ')
    sys.exit(-1)

# Take filename as first positional argument
filename, file_extension = os.path.splitext(sys.argv[1])

print(file_extension)

# Take delimiter as second (optional) positional argument
if (file_extension == '.csv'):
    delim = ','
elif (file_extension == '.psv'):
    delim = '|'
else:
    delim = '\t'

data = np.loadtxt(sys.argv[1], delimiter=delim, unpack=True)


# Plot the series
fig = plt.figure()
axs = fig.add_subplot(1, 1, 1)
axs.plot(data[0], data[1], linestyle='-', marker='o', c='r')

#Third column for analytical prediction
if len(data) > 2:
    axs.plot(data[0], data[2], linestyle='--', c='b')
plt.show()
