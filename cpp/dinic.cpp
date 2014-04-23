const int N = 10000;
const int M = 100000;
const int INF = 2000000001;
int s, t, n, m, freeArc;

int LINK[M], CAP[M], DEST[M]; // M
int HEAD[N], PRO[N], QUEUE[N], LEVEL[N]; // N

inline void addArc(int u, int v, int c) {
  CAP[freeArc] = c;
  DEST[freeArc] = v;
  LINK[freeArc] = HEAD[u];
  HEAD[u] = freeArc++;
}

inline void addEdge(int u, int v, int c) {
  addArc(u, v, c);
  addArc(v, u, c);
}

bool bfs() {
  fill(LEVEL, LEVEL + n, -1);
  int qt = 0, qh = 0;
  QUEUE[qt++] = s;
  LEVEL[s] = 0;
  while (qh != qt) {
    int u = QUEUE[qh++];
    if (u == t) return true;
    for (int arc = HEAD[u]; arc != -1; arc = LINK[arc]) {
      int v = DEST[arc];
      if (CAP[arc] > 0 && LEVEL[v] == -1) {
        QUEUE[qt++] = v;
        LEVEL[v] = LEVEL[u] + 1;
      }
    }
  }
  return LEVEL[t] != -1;
}

int dfs(int v, int f, int pushed = 0) {
  if (v == t || f == 0) return f;
  for (int& arc = PRO[v]; arc != -1; arc = LINK[arc]) {
    int to = DEST[arc];
    if ((LEVEL[v] + 1 == LEVEL[to]) && (CAP[arc] > 0)) {
      int p = dfs(to, min(CAP[arc], f));
      if (p > 0) {
        CAP[arc] -= p, CAP[arc ^ 1] += p;
        pushed += p, f -= p;
        if (f == 0) {
          return pushed;
        }
      }
    }
  }
  return pushed;
}

void solve() {
  cin >> n >> m;
  s = 0 , t = n - 1, freeArc = 0;
  fill(HEAD, HEAD + n, -1);
  for (int i = 0, u, v, c; i < m; ++i) {
    cin >> u >> v >> c;
    addEdge(u, v, c);
  }
  int flow = 0, pushed = 0;
  while (bfs()) {
    copy(HEAD, HEAD + n, PRO);
    while ((pushed = dfs(s, INF)) > 0) {
      flow += pushed;
    }
  }
  cout << flow << endl;
  forn (i, m) {
    int arc = 2 * i;
    int cap = (CAP[arc ^ 1] - CAP[arc]) / 2;
    if (cap >= 0) cout << DEST[arc^1] << DEST[arc] << +cap;
    else          cout << DEST[arc] << DEST[arc^1] << -cap;
  }
}