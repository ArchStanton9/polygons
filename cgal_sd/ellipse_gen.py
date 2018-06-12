import math

pi = 3.14
n = 21
r = 0.45
a = 1
b = 1

points = [
    ( a * math.cos(2 * pi / n * x) * r + 6,  b * math.sin(2 * pi / n * x) * r + 6.5)
    for x in range(0, n)
]

with open("R.txt", mode='w') as file:
    file.write(f'1\n{n}\n')
    file.writelines([f'{x} {y}\n' for x, y in points])
    file.write('0')
