#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    double x, y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Cross product of vectors OA and OB
double cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Distance squared between two points
double distSquared(const Point& A, const Point& B) {
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}

// ============================================
// Algorithm 1: Andrew's Monotone Chain (O(n log n))
// ============================================
vector<Point> convexHullAndrew(vector<Point> points) {
    int n = points.size();
    if (n <= 3) return points;

    sort(points.begin(), points.end());

    vector<Point> hull;

    // Build lower hull
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    // Build upper hull
    int lowerSize = hull.size();
    for (int i = n - 2; i >= 0; i--) {
        while (hull.size() > lowerSize && cross(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    hull.pop_back();
    return hull;
}

// ============================================
// Algorithm 2: Graham Scan (O(n log n))
// ============================================
vector<Point> convexHullGraham(vector<Point> points) {
    int n = points.size();
    if (n <= 3) return points;

    // Find the point with lowest y-coordinate (leftmost if tie)
    int minIdx = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].y < points[minIdx].y ||
            (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
            minIdx = i;
        }
    }

    swap(points[0], points[minIdx]);
    Point pivot = points[0];

    // Sort by polar angle with respect to pivot
    sort(points.begin() + 1, points.end(), [&](const Point& a, const Point& b) {
        double cp = cross(pivot, a, b);
        if (cp == 0) {
            return distSquared(pivot, a) < distSquared(pivot, b);
        }
        return cp > 0;
    });

    vector<Point> hull;
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    return hull;
}

// ============================================
// Algorithm 3: Jarvis March / Gift Wrapping (O(nh))
// ============================================
vector<Point> convexHullJarvis(vector<Point> points) {
    int n = points.size();
    if (n <= 3) return points;

    // Find the leftmost point
    int leftmost = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[leftmost].x ||
            (points[i].x == points[leftmost].x && points[i].y < points[leftmost].y)) {
            leftmost = i;
        }
    }

    vector<Point> hull;
    int current = leftmost;

    do {
        hull.push_back(points[current]);

        int next = (current + 1) % n;
        for (int i = 0; i < n; i++) {
            if (cross(points[current], points[next], points[i]) < 0) {
                next = i;
            }
        }

        current = next;
    } while (current != leftmost);

    return hull;
}

// Print convex hull
void printHull(const vector<Point>& hull) {
    cout << "Convex Hull (" << hull.size() << " points):" << endl;
    for (int i = 0; i < hull.size(); i++) {
        cout << "(" << hull[i].x << ", " << hull[i].y << ")";
        if (i < hull.size() - 1) cout << " -> ";
    }
    cout << endl;
}

// Calculate convex hull area
double hullArea(const vector<Point>& hull) {
    double area = 0;
    int n = hull.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += hull[i].x * hull[j].y;
        area -= hull[j].x * hull[i].y;
    }
    return abs(area) / 2.0;
}

int main() {
    // Test case
    vector<Point> points = {
        Point(0, 3),
        Point(1, 1),
        Point(2, 2),
        Point(4, 4),
        Point(0, 0),
        Point(1, 2),
        Point(3, 1),
        Point(3, 3)
    };

    cout << "=== Convex Hull Algorithms ===" << endl;
    cout << "Input points: " << points.size() << endl;

    // Andrew's Monotone Chain
    cout << "\n1. Andrew's Monotone Chain:" << endl;
    auto hull1 = convexHullAndrew(points);
    printHull(hull1);
    cout << "Area: " << hullArea(hull1) << endl;

    // Graham Scan
    cout << "\n2. Graham Scan:" << endl;
    auto hull2 = convexHullGraham(points);
    printHull(hull2);
    cout << "Area: " << hullArea(hull2) << endl;

    // Jarvis March
    cout << "\n3. Jarvis March (Gift Wrapping):" << endl;
    auto hull3 = convexHullJarvis(points);
    printHull(hull3);
    cout << "Area: " << hullArea(hull3) << endl;

    return 0;
}
