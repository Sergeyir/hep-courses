#!/usr/bin/env python3

#  @file  ndim_integration.py
#  @brief Contains a simple example that shows how N-dimensional integration can be performed with the use of python vegas library
#
#  This file is a part of a project hep-courses/jets3 (https://github.com/Sergeyir/hep-courses/jets3).
#
#  @author Sergei Antsupov (antsupov0124@gmail.com)

import numpy as np
import vegas

# number of ndimensions
ndim = 4
# setting integration limits
low_lim = np.array([-1, -1, -1, -1])
upp_lim = np.array([1, 1, 1, 1])

# the following decorator is recommended - it allows to pass numpy array in batches which speeds up the calculation
@vegas.batchintegrand
def gaus4d(x):
    # due to setting @vegas.batchintegrand x is 2D array, each column must 
    # be processed individually (in the current case), which axis=1 specifies for the sum
    return np.exp(-np.sum(x**2/2., axis=1))

# See https://vegas.readthedocs.io/en/latest/tutorial.html for more info and tutorials on vegas

# define integrator and pass to it integration ranges (from the size of the list it can deduce the number of dimensions)
integ = vegas.Integrator(list(zip(low_lim, upp_lim)))

# Perform calculations
# nitn is number of iterations for the algorithm to adapt (i.e. construct weighted sample). Recommended value - 10
# neval - number of integration steps
result = integ(gaus4d, nitn=10, neval=200_000)

print(f"Integral = {result.mean}")
print(f"Error    = {result.sdev}")
print(result.summary())
