#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

// ============================================
// Segment Tree with Lazy Propagation
// Supports range update and range query
// ============================================
class LazySegmentTree {
private:
    vector<long long> tree, lazy;
    int n;

    void push(int node, int start, int end) {
        if (lazy[node] != 0) {
            tree[node] += (end - start + 1) * lazy[node];

            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }

            lazy[node] = 0;
        }
    }

    void updateRange(int node, int start, int end, int l, int r, long long val) {
        push(node, start, end);

        if (start > end || start > r || end < l) return;

        if (l <= start && end <= r) {
            tree[node] += (end - start + 1) * val;

            if (start != end) {
                lazy[2 * node] += val;
                lazy[2 * node + 1] += val;
            }
            return;
        }

        int mid = (start + end) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);

        push(2 * node, start, mid);
        push(2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long queryRange(int node, int start, int end, int l, int r) {
        if (start > end || start > r || end < l) return 0;

        push(node, start, end);

        if (l <= start && end <= r) {
            return tree[node];
        }

        int mid = (start + end) / 2;
        long long p1 = queryRange(2 * node, start, mid, l, r);
        long long p2 = queryRange(2 * node + 1, mid + 1, end, l, r);

        return p1 + p2;
    }

public:
    LazySegmentTree(int size) : n(size) {
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }

    void update(int l, int r, long long val) {
        updateRange(1, 0, n - 1, l, r, val);
    }

    long long query(int l, int r) {
        return queryRange(1, 0, n - 1, l, r);
    }
};

// ============================================
// Segment Tree with Dynamic Updates
// Supports point update and range max query
// ============================================
class MaxSegmentTree {
private:
    vector<long long> tree;
    int n;

    void build(const vector<long long>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
        } else {
            int mid = (start + end) / 2;
            build(arr, 2 * node, start, mid);
            build(arr, 2 * node + 1, mid + 1, end);
            tree[node] = max(tree[2 * node], tree[2 * node + 1]);
        }
    }

    void update(int node, int start, int end, int idx, long long val) {
        if (start == end) {
            tree[node] = val;
        } else {
            int mid = (start + end) / 2;
            if (idx <= mid) {
                update(2 * node, start, mid, idx, val);
            } else {
                update(2 * node + 1, mid + 1, end, idx, val);
            }
            tree[node] = max(tree[2 * node], tree[2 * node + 1]);
        }
    }

    long long query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return -INF;

        if (l <= start && end <= r) {
            return tree[node];
        }

        int mid = (start + end) / 2;
        long long p1 = query(2 * node, start, mid, l, r);
        long long p2 = query(2 * node + 1, mid + 1, end, l, r);

        return max(p1, p2);
    }

public:
    MaxSegmentTree(const vector<long long>& arr) {
        n = arr.size();
        tree.assign(4 * n, -INF);
        build(arr, 1, 0, n - 1);
    }

    void update(int idx, long long val) {
        update(1, 0, n - 1, idx, val);
    }

    long long query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }
};

// ============================================
// Persistent Segment Tree (Advanced)
// Supports historical queries
// ============================================
struct Node {
    long long sum;
    int left, right;
};

class PersistentSegmentTree {
private:
    vector<Node> nodes;
    vector<int> roots;
    int n;

    int build(int start, int end) {
        int node = nodes.size();
        nodes.push_back({0, -1, -1});

        if (start == end) {
            return node;
        }

        int mid = (start + end) / 2;
        nodes[node].left = build(start, mid);
        nodes[node].right = build(mid + 1, end);

        return node;
    }

    int update(int prev, int start, int end, int idx, long long val) {
        int node = nodes.size();
        nodes.push_back(nodes[prev]);

        if (start == end) {
            nodes[node].sum = val;
            return node;
        }

        int mid = (start + end) / 2;
        if (idx <= mid) {
            nodes[node].left = update(nodes[prev].left, start, mid, idx, val);
        } else {
            nodes[node].right = update(nodes[prev].right, mid + 1, end, idx, val);
        }

        nodes[node].sum = nodes[nodes[node].left].sum + nodes[nodes[node].right].sum;
        return node;
    }

    long long query(int root, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) {
            return nodes[root].sum;
        }

        int mid = (start + end) / 2;
        long long p1 = query(nodes[root].left, start, mid, l, r);
        long long p2 = query(nodes[root].right, mid + 1, end, l, r);

        return p1 + p2;
    }

public:
    PersistentSegmentTree(int size) : n(size) {
        roots.push_back(build(0, n - 1));
    }

    void update(int idx, long long val) {
        roots.push_back(update(roots.back(), 0, n - 1, idx, val));
    }

    long long query(int version, int l, int r) {
        return query(roots[version], 0, n - 1, l, r);
    }

    int getVersion() {
        return roots.size() - 1;
    }
};

// ============================================
// Test Cases
// ============================================
int main() {
    cout << "=== Segment Tree Advanced Templates ===" << endl;

    // Test 1: Range add + Range sum
    cout << "\n1. Lazy Segment Tree (Range Add + Range Sum):" << endl;
    LazySegmentTree lst(10);
    for (int i = 0; i < 10; i++) {
        lst.update(i, i, i);
    }

    cout << "Initial: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]" << endl;
    cout << "Sum [0, 4]: " << lst.query(0, 4) << endl;

    lst.update(2, 7, 10);
    cout << "After adding 10 to [2, 7], sum [2, 7]: " << lst.query(2, 7) << endl;

    // Test 2: Point update + Range max
    cout << "\n2. Max Segment Tree (Point Update + Range Max):" << endl;
    vector<long long> arr = {3, 2, -1, 6, 5, 4, -3, 3, 7, 2};
    MaxSegmentTree mst(arr);

    cout << "Array: ";
    for (auto x : arr) cout << x << " ";
    cout << endl;

    cout << "Max [2, 6]: " << mst.query(2, 6) << endl;
    mst.update(3, 10);
    cout << "After updating index 3 to 10, max [2, 6]: " << mst.query(2, 6) << endl;

    // Test 3: Persistent Segment Tree
    cout << "\n3. Persistent Segment Tree (Historical Queries):" << endl;
    PersistentSegmentTree pst(5);

    cout << "Version 0 (empty)" << endl;

    for (int i = 0; i < 5; i++) {
        pst.update(i, i + 1);
        cout << "Version " << (i + 1) << " after setting index " << i << " to " << (i + 1) << endl;
    }

    cout << "Query [1, 3] at version 3: " << pst.query(3, 1, 3) << endl;
    cout << "Query [1, 3] at version 5: " << pst.query(5, 1, 3) << endl;

    return 0;
}
