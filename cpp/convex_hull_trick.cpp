//SOLVE dp[i] = min(a[i] * b[j] + dp[j]), j = 0...i - 1
//a[i]<a[i+1], b[i]>b[i+1] (dp[0]=0,a[0]=1, b[n-1]=0)
//in O(NlogN) time and O(N) memory

const int N = 100000;
int n, l = 0, r = 0, a[N], b[N], st[N];
ll dp[N];
// point (i1, i3) is under line i2 / p12.x > p23.x
bool bad(int i1, int i2, int i3) {
  return (dp[i2] - dp[i1]) / (b[i1] - b[i2])
       > (dp[i3] - dp[i2]) / (b[i2] - b[i3]);
}

void add(int i) {
  while (r-l > 1 && bad(st[r-2], st[r-1], i)) r--;
  st[r++] = i;
}

ll val(int i, int j) { return 1LL * a[i] * b[j] + dp[j]; }

ll query(int i) {
  l = min(l, r - 1);
  while (l+1 < r && val(i, st[l+1]) < val(i, st[l])) l++;
  return val(i, st[l]);
}

void solve() {
  cin >> n;
  forn (i, n) cin >> a[i];
  forn (i, n) cin >> b[i];
  dp[0] = 0;
  add(0);
  for (int i = 1; i < n; ++i) {
    dp[i] = query(i);
    add(i);
  }
  cout << dp[n - 1] << endl;
}