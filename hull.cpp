#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

typedef int coord_t;
typedef int coord2_t;

struct Point {
	coord_t x, y;

	bool operator<(const Point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};

// 3D cross product between OA and OB
coord2_t cross(const Point &O, const Point &A, const Point &B) {
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



int main() {
	std::random_device rnd_device;

	std::mt19937 mersenne_engine {rnd_device()};
	std::uniform_int_distribution<int> dist {1, 52};

	auto gen = [&](){
		return dist(mersenne_engine);
	};

	const size_t vsize = 10;
	std::vector<int> vx(vsize), vy(vsize);
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
