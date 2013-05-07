import math

import h5py
import numpy



f = h5py.File("test.hdf5", "w")
d = f.create_dataset('data', (16,16, 16), int, chunks=(4, 4, 4))
d[...] = numpy.arange(0, 16**3).reshape((16, 16, 16))/16
f.close()

