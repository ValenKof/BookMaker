const int N = 100000; // min <= a[i] <= max
int a[N], d[N + 1], len[N], order[N];

void solve(int n, int min, int max, int& ans) {
  fill(d, d+n+1, min-1); d[0] = max+1;
  for (int i = n-1; i >= 0; --i) {
    int j = lower_bound(d, d+n+1, a[i], greater<int>()) - d;
    d[j] = a[i], len[i] = j;
  }
  ans = lower_bound(d, d+n+1, min-1, greater<int>()) - d - 1;
  for (int i = 0, goal = ans, last = min-1; goal != 0; ++i)
    if (len[i] == goal && last < a[i])
      order[--goal] = i, last = a[i];
}