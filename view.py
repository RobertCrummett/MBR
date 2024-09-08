from matplotlib import pyplot as plt

class Point:
	def __init__(self, x, y):
		self.x = x
		self.y = y

points, hull, rect = [], [], []

with open("mbr.txt") as file:
	for line in file:
		label, px, py = line.split()

		px = float(px[1:-1])
		py = float(py[:-1])
		p = Point(px, py)

		if label == "[POINT]":
			points.append(p)
		elif label == "[HULL]":
			hull.append(p)
		elif label == "[RECT]":
			rect.append(p)
		else:
			raise NotImplementedError

hull.append(hull[0])
rect.append(rect[0])

fig, ax = plt.subplots()

ax.scatter([p.x for p in points], [p.y for p in points], s=30, c='r')
ax.plot([p.x for p in hull], [p.y for p in hull], '-b')
ax.plot([p.x for p in rect], [p.y for p in rect], '-g')

for i, p in enumerate(rect[:-1]):
	ax.text(p.x, p.y, str(i))

plt.show()

