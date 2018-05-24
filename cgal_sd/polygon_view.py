import os
from matplotlib.path import Path


def _read_contour(it):
    points_count = int(next(it))
    points = []
    for v in range(points_count):
        p1, p2 = next(it).split()
        points.append([float(p1), float(p2)])

    return points


class PolygonView:
    def __init__(self, filename, color='blue'):
        self.filename = filename
        self.valid = True
        self.color = color
        self.timestamp = self._get_timestamp()
        self.vertices = []

    def _get_timestamp(self):
        return os.path.getmtime(self.filename)

    def _read_lines(self):
        lines = list(map(lambda x: str(x).strip(), open(self.filename).readlines()))
        return list(filter(lambda x: len(x), lines))

    def read_vertices(self):
        lines = self._read_lines()

        if len(lines) == 0:
            self.valid = False
            pass

        try:
            self._read_vertices(lines)
        except StopIteration:
            return
        except Exception as ex:
            self.valid = False
            print(ex)
            return []

    def _read_vertices(self, lines):
        it = iter(lines)

        polygons_count = int(next(it))
        if polygons_count is not 1:
            raise ValueError("Can not read more than 1 polygon")

        self.vertices = _read_contour(it)

    @property
    def is_valid(self):
        return self.valid

    @property
    def is_changed(self):
        return self._get_timestamp() > self.timestamp

    def fetch_vertices(self):
        current_ts = self._get_timestamp()

        if current_ts > self.timestamp or len(self.vertices) is 0:
            self.read_vertices()
            self.timestamp = current_ts

        if not self.valid:
            return []

        return self.vertices


class PolygonWithHolesView(PolygonView):
    def __init__(self, filename, color='blue'):
        super().__init__(filename, color)
        self.codes = []
        self.path = None

    def _read_vertices(self, lines):
        self.vertices.clear()
        self.codes.clear()

        it = iter(lines)
        polygons_count = int(next(it))
        if polygons_count is not 1:
            raise ValueError("Can not read more than 1 polygon")

        exterior = _read_contour(it)
        self._add_points(exterior)

        holes_count = int(next(it))
        for h in range(holes_count):
            hole = _read_contour(it)
            self._add_points(hole)

    def _add_points(self, points):
        count = len(points)
        if count < 3:
            raise ValueError("Polygon should contain at least 3 points")

        self.vertices.append(points[0])
        self.codes.append(Path.MOVETO)

        for i in range(1, count):
            self.vertices.append(points[i])
            self.codes.append(Path.LINETO)

        self.vertices.append([0, 0])
        self.codes.append(Path.CLOSEPOLY)

    def fetch_path(self):
        current_ts = self._get_timestamp()

        if current_ts > self.timestamp or self.path is None:
            self.read_vertices()
            self.path = Path(self.vertices, self.codes)
            self.timestamp = current_ts

        return self.path
