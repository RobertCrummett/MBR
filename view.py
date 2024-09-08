from matplotlib import pyplot as plt
plt.style.use("dark_background")

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

ax.scatter([p.x for p in points], [p.y for p in points], s=30, label = "Obs. Data", color = "C0")
ax.plot([p.x for p in hull], [p.y for p in hull], label = "Convex Hull", color = "C1")
ax.plot([p.x for p in rect], [p.y for p in rect], label = "Min. Area Rect.", color = "C2")

# for i, p in enumerate(rect[:-1]):
#	ax.text(p.x, p.y, str(i))

ax.set_ylim([min((r.y for r in rect)) - 25, max((r.y for r in rect))])
ax.legend(loc =	"lower center")
ax.axis("off")
plt.savefig("mbr.png", dpi=600)

