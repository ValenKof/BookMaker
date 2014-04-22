const int N = 100000;
const int M = 100000;
int n, m, k;
bool is_cur[M], is_del[M];
int q_u[M], q_v[M], q_id[M];
char q_t[M];
int p[N], w[N], used[N];
map<int, int> g[N];

int dsu_find(int v) {
  return p[v] = (p[v] == v ? v : dsu_find(p[v]));
}

void dsu_unite(int u, int v) {
  u = dsu_find(u), v = dsu_find(v);
  if (u == v) return;
  if (w[u] > w[v]) swap(u, v);
  p[u] = v, w[v] += w[u];
}

void add_arc(int u, int v) {
  u = dsu_find(u), v = dsu_find(v);
  ++g[u][v], ++g[v][u];
}

void del_arc(int u, int v) {
  u = dsu_find(u), v = dsu_find(v);
  auto it = g[u].find(v);
  if (--it->second == 0) g[u].erase(it);
  it = g[v].find(u);
  if (--it->second == 0) g[v].erase(it);
}

void input() {
	cin >> n >> m;
  k = (int)(sqrt((double)(m)) + 1);
 
	fill(used, used + n, -1);
  map<PII, stack<int> > st;
  forn (i, m) {
		cin >> q_t[i] >> q_u[i] >> q_v[i];
    --q_u[i], --q_v[i], q_id[i] = -1;
    if (q_u[i] > q_v[i]) swap(q_u[i], q_v[i]);
    if (q_t[i] == '+') {
      st[mp(q_u[i], q_v[i])].push(i);
      is_cur[i] = is_del[i] = false;
    }
    if (q_t[i] == '-') {
      q_id[i] = st[mp(q_u[i], q_v[i])].top();
      st[mp(q_u[i], q_v[i])].pop();
    }
  }
}

bool dfs(int u, int need, int t) {
  if (u == need) return true;
  used[u] = t;
  for (auto it : g[u]) {
    int v = it.first;
    if (used[v] != t && dfs(v, need, t)) return true;
  }
  return false;
}

void solve() {
  for (int i = 0; i < m; i += k) {
    for (int j = i; j < min(m, i + k); ++j) {
      if (q_t[j] == '-') is_cur[q_id[j]] = true;
    }
    forn (j, n) {
      p[j] = j, w[j] = 1, g[j].clear();
    }
    forn (j, i) {
      if (q_t[j] == '+' && !is_cur[j] && !is_del[j]) {
        dsu_unite(q_u[j], q_v[j]);
      }
    }
    forn (j, i) {
      if (q_t[j] == '+' && is_cur[j]) {
        add_arc(q_u[j], q_v[j]);
      }
    }
    for (int j = i; j < min(m, i + k); ++j) {
      if (q_t[j] == '-') del_arc(q_u[j], q_v[j]);
      if (q_t[j] == '+') add_arc(q_u[j], q_v[j]);
      if (q_t[j] == '?') printf("%c\n", 
          dfs(dsu_find(q_u[j]), dsu_find(q_v[j]), j) ? '+' : '-');
    }    
    for (int j = i; j < min(m, i + k); ++j) {
      if (q_t[j] == '-') {
        is_cur[q_id[j]] = false;
        is_del[q_id[j]] = true;
      }
    }
  }
}
