const int N = 1000;
VI g[N];
int n, m, mt[N], p[N], base[N];
bool blossom[N], used[N];

int lca(int a, int b) {
  static bool used[N];
  fill(used, used + n, false);
  while (true) {
    a = base[a];
    used[a] = true;
    if (mt[a] == -1) break;
    a = p[mt[a]];
  }
  while (true) {
    b = base[b];
    if (used[b]) return b;
    b = p[mt[b]];
  }
}

void mark_path(int v, int b, int children) {
  while (base[v] != b) {
    blossom[base[v]] = blossom[base[mt[v]]] = true;
    p[v] = children;
    children = mt[v];
    v = p[mt[v]];
  }
}

int find_path(int root) {
  static int q[N];
  fill(used, used + n, false);
  fill(p, p + n, -1);
  forn (i, n) base[i] = i;
  used[root] = true;
  int qh = 0, qt = 0;
  q[qt++] = root;
  while (qh != qt) {
    int v = q[qh++];
    forn (i, sz(g[v])) {
      int to = g[v][i];
      if (base[v] == base[to] || mt[v] == to) continue;
      if (to == root || mt[to] != -1 && p[mt[to]] != -1) {
        int curbase = lca(v, to);
        forn (j, n) blossom[j] = false;
        mark_path(v, curbase, to);
        mark_path(to, curbase, v);
        forn (j, n) {
          if (!blossom[base[j]]) continue;
          base[j] = curbase;
          if (!used[j]) {
            used[j] = true;
            q[qt++] = j;
          }
        }
      } else if (p[to] == -1) {
        p[to] = v;
        if (mt[to] == -1) return to;
        to = mt[to];
        used[to] = true;
        q[qt++] = to;
      }
    }
  }
  return -1;
}

void maxMatch() {
  /* readGraph() */
  fill(mt, mt + n, -1);
  forn (u, n) {
    if (mt[u] != -1) continue;
    for (int v : g[u]) {
      if (mt[v] != -1) continue;
      mt[v] = u, mt[u] = v;
      break;
    }
  }
  forn (i, n) {
    if (mt[i] != -1) continue;
    int v = find_path(i);
    while (v != -1) {
      int pv = p[v];
      int ppv = mt[pv];
      mt[v] = pv;
      mt[pv] = v;
      v = ppv;
    }
  }
  /* print_ans(); */
}