from helpers import extend_file_paths
from polygon_view import PolygonSetView
from matplotlib.patches import Polygon
from matplotlib.patches import PathPatch
from matplotlib.collections import PatchCollection
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np


polygon_set = PolygonSetView('./examples/3/p.txt')


np.random.seed(1968081271)
ax = plt.gca()

patches = [ PathPatch(p) for p in polygon_set.fetch_paths()]
ax.add_collection(PatchCollection(patches))
ax.axis('equal')

plt.show()