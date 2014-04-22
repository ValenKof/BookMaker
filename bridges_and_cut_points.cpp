const int N = 100000;
VI g[N], cut_points;
bool used[N];
int n, timer, tin[N], fup[N];
vector<PII> bridges;

void dfs(int u, int p) {
  used[u] = true;
  tin[u] = fup[u] = timer++;
  int children = 0;
  for (int v : g[u]) {
    if (v == p) continue;
    if (used[v]) {
      mn(fup[u], tin[v]);
    } else {
      dfs(v, u);
      mn(fup[u], fup[v]);
      if (fup[v] >= tin[u] && p != -1) {
        cut_points.push_back(u);
      }
      if (fup[v] > tin[u]) {
        bridges.push_back(make_pair(u, v));
      }
      children++;
    }
  }
  if (p == -1 && children > 1) {
    cut_points.push_back(u);
  }
}
 
void solve() {
  timer = 0;
  fill(used, used + n, false);
	bridges.resize(0);
  cut_points.resize(0);
  forn (i, n) if (!used[i]) dfs(i, -1);
}
