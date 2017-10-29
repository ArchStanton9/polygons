import build.wrapper.Debug.wrapper as CGAL
import matplotlib.pyplot as plt

random = list(CGAL.random(1000).points())
halton = list(CGAL.halton(1000).points())

plt.figure(1)
plt.subplot(211)
plt.scatter([p.x for p in random], [p.y for p in random], s=1)
plt.axis([-0.5, 0.5, -0.5, 0.5])

plt.subplot(212)
plt.scatter([p.x for p in halton], [p.y for p in halton], s=1)
plt.axis([-0.5, 0.5, -0.5, 0.5])

plt.show()