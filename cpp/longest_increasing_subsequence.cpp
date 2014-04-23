const int N = 100000;
const int INF = (int)(1e9);
int a[N], d[N + 1];

void solve(int n) {
  fill(d, d + n + 1, INF);
  d[0] = -INF;
  forn (i, n) {
    int j = upper_bound(d, d + n, a[i]) - d;
    if (d[j-1] < /* <= */ a[i] && a[i] < d[j]) d[j] = a[i];
  }
}