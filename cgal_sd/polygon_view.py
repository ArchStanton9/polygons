import os
from matplotlib.path import Path


def _read_contour(it):
    points_count = int(next(it))
    if points_count < 3:
        raise ValueError("Contour should contain at least 3 points")

    points = []
    for v in range(points_count):
        p1, p2 = next(it).split()
        points.append([float(p1), float(p2)])

    return points


def _read_part_of_path(it):
    vertices = _read_contour(it)
    codes = [Path.MOVETO] + [Path.LINETO] * (len(vertices) - 1)

    vertices.append([0, 0])
    codes.append(Path.CLOSEPOLY)

    return vertices, codes


def _read_pwh(it):
    vertices, codes = _read_part_of_path(it)

    holes_count = int(next(it))
    for h in range(holes_count):
        hole_vertices, hole_codes = _read_part_of_path(it)
        vertices.extend(hole_vertices)
        codes.extend(hole_codes)

    return Path(vertices, codes)


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
        it = iter(lines)
        polygons_count = int(next(it))
        if polygons_count is not 1:
            raise ValueError("Can not read more than 1 polygon")

        self.path = _read_pwh(it)

    def fetch_path(self):
        current_ts = self._get_timestamp()

        if current_ts > self.timestamp or self.path is None:
            self.read_vertices()
            self.path = Path(self.vertices, self.codes)
            self.timestamp = current_ts

        return self.path


class PolygonSetView(PolygonView):
    def __init__(self, filename, color='blue'):
        super().__init__(filename, color)
        self.paths = None

    def _read_vertices(self, lines):
        it = iter(lines)
        polygons_count = int(next(it))
        self.paths = [_read_pwh(it) for i in range(polygons_count)]

    def fetch_paths(self):
        current_ts = self._get_timestamp()

        if current_ts > self.timestamp or self.paths is None:
            self.read_vertices()
            self.timestamp = current_ts

        return self.paths
