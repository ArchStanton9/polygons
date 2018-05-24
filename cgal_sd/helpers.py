import glob
from matplotlib.pyplot import cm


def extend_file_paths(paths):
    for path in paths:
        if str(path).count('*') > 0:
            wildcard_paths = glob.glob(path)
            for wp in wildcard_paths:
                yield wp
        else:
            yield path

def get_cmap(n, name='hsv'):
    '''Returns a function that maps each index in 0, 1, ..., n-1 to a distinct 
    RGB color; the keyword argument name must be a standard mpl colormap name.'''
    return cm.get_cmap(name, n)