const int N = 100000;
const int A = 256;
int p[N], pn[N], cnt[N], c[N], cn[N], n;
char s[N + 1];

void solve() {
  fill(cnt, cnt + A, 0);
  forn (i, n) cnt[s[i]]++;
  forn (i, A - 1) cnt[i + 1] += cnt[i];
  forn (i, n) p[--cnt[s[i]]] = i;
  c[p[0]] = 0;
  for (int i = 1, classes = 1; i < n; ++i) {
    if (s[p[i]] != s[p[i-1]]) classes++;
    c[p[i]] = classes - 1;
  }
  for (int h = 0; (1 << h) < n; ++h) {
    forn (i, n) {
      pn[i] = p[i] - (1 << h);
      if (pn[i] < 0)  pn[i] += n;
    }
    fill(cnt, cnt + A, 0);
    forn (i, n) cnt[c[pn[i]]]++;
    forn (i, A - 1) cnt[i + 1] += cnt[i];
    for (int i = n - 1; i >= 0; --i) p[--cnt[c[pn[i]]]] = pn[i];
    cn[p[0]] = 0;
    for (int i = 1, classes = 1; i < n; ++i) {
      int mid1 = (p[i]+(1<<h)) % n,  mid2 = (p[i-1]+(1<<h)) % n;
      if (c[p[i]] != c[p[i-1]] || c[mid1] != c[mid2]) classes++;
      cn[p[i]] = classes - 1;
    }
    copy(cn, cn + n, c);
  }
}