from matplotlib import pyplot as plt


class LineBuilder:
    def __init__(self, line):
        self.line = line
        self.buffer = []
        self.result = ''
        self.xs = list()
        self.ys = list()
        self.start = None
        self.cid = line.figure.canvas.mpl_connect('button_press_event', self)

    def __call__(self, event):
        print('click', event)
        if event.inaxes != self.line.axes:
            return

        if event.button is 1:
            self.left_click(event)
        elif event.button is 3:
            self.right_click(event)

        self.line.set_data(self.xs, self.ys)
        self.line.figure.canvas.draw()

    def left_click(self, event):
        self.xs.append(event.xdata)
        self.ys.append(event.ydata)
        self.buffer.append([event.xdata, event.ydata])

        if self.start is None:
            self.start = (event.xdata, event.ydata)
            return

    def right_click(self, event):
        if len(self.buffer):
            self.result += str(len(self.buffer)) + '\n'
            for x, y in self.buffer:
                self.result += f'{x} {y} \n'

            self.result += '\n\n'
            self.xs.append(self.start[0])
            self.ys.append(self.start[1])
            self.buffer.clear()
            self.start = None


fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_title('click to build line segments')
line, = ax.plot([0], [0])  # empty line

ax.set_xlim(-0.3, 0.3)
ax.set_ylim(-0.3, 0.3)
linebuilder = LineBuilder(line)

plt.show()
print(linebuilder.result)
