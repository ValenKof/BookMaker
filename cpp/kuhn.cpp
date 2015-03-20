vector<vector<int>> g;
vector<int> pt, mt, used;
int it;
 
inline bool dfs(int u) {
    if (used[u] == it) return false;
    used[u] = it;
    for (int v : g[u]) {
        if (mt[v] == -1) {
            mt[v] = u, pt[u] = v;
            return true;
        }
    }
    for (int v : g[u]) {
        if (dfs(mt[v])) {
            mt[v] = u, pt[u] = v;
            return true;
        }
    }
    return false;
}

inline bool cycle(int u) {
    if (used[u] == 0) return false;
    if (used[u] == 1) return true;
    used[u] = 1;
    for (int v : g[u])
        if (mt[v] != u && mt[v] != -1 && cycle(mt[v]))
            return true;
    used[u] = 0;
    return false;
}

void solve(int n, int m) {
    pt.assign(n, -1), mt.assign(m, -1), used.assign(n, -1);
    int size = 0;
    it = -1;
    for (int ok = 1; ok; ) {
        ok = 0, it++;
        forn (i, sz(g)) {
            if (pt[i] == -1 && dfs(i)) {
                ok = 1;
                size++;
            }
        }
    }
    bool unique = true;
    used.assign(n, -1);
    forn (i, n) {
        if (cycle(i)) {
            unique = false;
            break;
        }
    }
}