#!/opt/python-3.4/linux/bin/python3
import numpy as np
from random import shuffle
np.random.seed(4711) # for repeatability
c1 = np.random.multivariate_normal([10, 0], [[3, 1], [1,4]], size = [100, ])
l1 = np.zeros(100)
l2 = np.ones(100)
c2 = np.random.multivariate_normal([0, 10], [[3, 1], [1, 4]], size = [100, ])
# add noise
np.random.seed(1) # for repeatability
noise1x = np.random.normal(0, 2, 100)
noise1y = np.random.normal(0, 8, 100)
noise2 = np.random.normal(0, 8, 100)
c1[:, 0] += noise1x
c1[:, 1] += noise1y
c2[:, 1] += noise2
c1 = c1.tolist()
c1.extend(c2.tolist())
#shuffle(c1)
for i in range(len(c1)):
  print(c1[i][0], ',', c1[i][1])
