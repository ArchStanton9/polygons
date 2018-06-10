import math

pi = 3.14
n = 100
r = 5
points = [
    (math.cos(2 * pi / n * x) * r, math.sin(2 * pi / n * x) * r)
    for x in range(0, n)
]

with open("R.txt", mode='w') as file:
    file.write(f'1\n{n}\n')
    file.writelines([f'{x} {y}\n' for x, y in points])
    file.write('0')
