import os
import traceback

class PolygonView:
    def __init__(self, path, color='blue'):
        self.path = path
        self.valid = True
        self.color = color
        self.timestamp = self._get_timestamp()        
        self.vertices = self._read_vertices()

    def _get_timestamp(self):
        return os.path.getmtime(self.path)

    def _read_vertices(self):
        lines = list(map(lambda x: str(x).strip(), open(self.path).readlines()))
        lines = list(filter(lambda x: len(x), lines))

        if len(lines) == 0:
            self.valid = False
            pass

        try:
            polygons_count = int(lines[0])
            print(f"Polygons count: {polygons_count}")
            cursor = 1
            for p in range(polygons_count):
                vertices_count = int(lines[cursor])
                cursor += 1
                points = []

                for v in range(vertices_count):
                    line = lines[cursor]
                    cursor += 1
                    p1, p2 = line.split()
                    points.append([float(p1), float(p2)])

                return points
            
        except Exception as ex:
            self.valid = False
            print(traceback.format_exc())
            return []

    @property
    def is_valid(self):
        return self.valid
    
    @property
    def is_changed(self):
        return self._get_timestamp() > self.timestamp

    def fetch_vertices(self):
        current_ts = self._get_timestamp()
        
        if current_ts > self.timestamp:
            self.vertices = self._read_vertices()
            self.timestamp = current_ts

        return self.vertices
