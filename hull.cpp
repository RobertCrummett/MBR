#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

typedef double coord_t;

struct Point {
	coord_t x, y;

	bool operator<(const Point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};

struct Rectangle {
	Point p1, p2, p3, p4;

	coord_t area() const {
		return abs(p1.x * (p2.y - p3.y) + 
				p2.x * (p3.y - p1.y) + 
				p3.x * (p1.y - p2.y));
	}
};

// 3D cross product between OA and OB
coord_t cross(const Point &O, const Point &A, const Point &B) {
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Monotone chain convex hull algorithm
std::vector<Point> convex_hull(std::vector<Point> points) {
	size_t n = points.size(), k = 0;

	if (n <= 3) {
		return points;
	}

	std::vector<Point> hull(2 * n);
	
	std::sort(points.begin(), points.end());

	// Lower hull
	for (size_t i = 0; i < n; ++i) {
		while (k >= 2 && cross(hull[k-2], hull[k-1], points[i]) <= 0) {
			k--;
		}
		hull[k++] = points[i];
	}
	
	// Upper hull
	for (size_t i = n-1, t = k+1; i > 0; --i) {
		while (k >= t && cross(hull[k-2], hull[k-1], points[i-1]) <= 0) {
			k--;
		}
		hull[k++] = points[i-1];
	}

	hull.resize(k-1);

	return hull;
}

// Rotating callipers minimum bounding rectangle
void rotating_calipers(std::vector<Point> hull) {
		
}

int main() {
	const size_t vsize = 10;

	std::random_device rnd_device;

	std::mt19937 mersenne_engine {rnd_device()};
	std::uniform_real_distribution<coord_t> dist {1.0, 52.0};

	auto gen = [&](){
		return dist(mersenne_engine);
	};

	std::vector<coord_t> vx(vsize), vy(vsize);
	std::generate(vx.begin(), vx.end(), gen);
	std::generate(vy.begin(), vy.end(), gen);

	std::vector<Point> points;
	points.reserve(vsize);
	for (size_t i = 0; i < vsize; i++) {
		std::cout << "[POINT] (" << vx[i] << ", "
			<< vy[i] << ")" << std::endl;
	
		points.push_back({vx[i], vy[i]});
	}

	std::vector<Point> hull = convex_hull(points);

	for (const auto& h: hull) {
		std::cout << "[HULL] (" << h.x << ", "
			<< h.y << ")" << std::endl;
	}
}
