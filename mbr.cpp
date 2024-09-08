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
Rectangle minimum_area_bounding_rectangle(std::vector<Point> hull) {
	coord_t area = std::numeric_limits<coord_t>::max();	
    Rectangle mbr{};

	Point tail = hull.back();

	for (const auto& head: hull) {
		Point U0 { head.x - tail.x, head.y - tail.y };

		coord_t magnitude = sqrt(U0.x * U0.x + U0.y * U0.y);
		U0.x /= magnitude;	
		U0.y /= magnitude;	

		Point U1 { -U0.y, U0.x };
		
		coord_t min0 = 0.0, max0 = 0.0, max1 = 0.0;
		
		for (const auto& vertex: hull) {
			Point D { vertex.x - tail.x, vertex.y - tail.y };

			coord_t projection0 = D.x * U0.x + D.y * U0.y;

			min0 = projection0 < min0 ? projection0 : min0;
			max0 = projection0 > max0 ? projection0 : max0;

			coord_t projection1 = D.x * U1.x + D.y * U1.y;

			max1 = projection1 > max1 ? projection1 : max1;
		}
		
		coord_t trial_area = (max0 - min0) * max1;

		if (trial_area < area) {
			area = trial_area;
		
			mbr.p1.x = tail.x + min0 * U0.x;
			mbr.p1.y = tail.y + min0 * U0.y;

			mbr.p2.x = tail.x + min0 * U0.x + max1 * U1.x;
			mbr.p2.y = tail.y + min0 * U0.y + max1 * U1.y;

			mbr.p3.x = tail.x + max0 * U0.x + max1 * U1.x;
			mbr.p3.y = tail.y + max0 * U0.y + max1 * U1.y;

			mbr.p4.x = tail.x + max0 * U0.x;
			mbr.p4.y = tail.y + max0 * U0.y;
		}

		tail = head;
	}
	return mbr;
}

int main() {
	const size_t numberpoints = 20;

	std::random_device rnd_device;

	std::mt19937 mersenne_engine {rnd_device()};
	std::uniform_real_distribution<coord_t> dist {-50.0, 50.0};

	auto gen = [&](){
		return dist(mersenne_engine);
	};

	std::vector<coord_t> vx(numberpoints), vy(numberpoints);
	std::generate(vx.begin(), vx.end(), gen);
	std::generate(vy.begin(), vy.end(), gen);

	std::vector<Point> points;
	points.reserve(numberpoints);

	for (size_t i = 0; i < numberpoints; i++) {
		std::cout << "[POINT] (" << vx[i] << ", "
			<< vy[i] << ")" << std::endl;
	
		points.push_back({vx[i], vy[i]});
	}

	std::vector<Point> hull = convex_hull(points);

	for (const auto& h: hull) {
		std::cout << "[HULL] (" << h.x << ", "
			<< h.y << ")" << std::endl;
	}

	Rectangle mbr = minimum_area_bounding_rectangle(hull);

	std::cout << "[RECT] (" << mbr.p1.x << ", " << mbr.p1.y << ")" << std::endl;
	std::cout << "[RECT] (" << mbr.p2.x << ", " << mbr.p2.y << ")" << std::endl;
	std::cout << "[RECT] (" << mbr.p3.x << ", " << mbr.p3.y << ")" << std::endl;
	std::cout << "[RECT] (" << mbr.p4.x << ", " << mbr.p4.y << ")" << std::endl;
}
