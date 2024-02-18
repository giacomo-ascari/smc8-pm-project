# https://www.youtube.com/watch?v=rGNUHigqUBM
# https://www.youtube.com/watch?v=iKAVRgIrUOU

# 1d simulation of incompressible and inviscid

# fluid simulation
# 1. modify velocity values (eg. add gravity)
# 2. make the fuild incompressible (projection)
# 3. move the velocity field (advection)
# repeat

# velocity is a one-dimensional vector lol
class Velocity:
    def __init__(self):
        self.x = 0
        pass

class Cell:
    def __init__(self):
        self.v = Velocity()
        pass



n_cells = 100

tube = []

for i in range(n_cells):
    tube.append({"p": 1, })