VI buildLCP(string& a, VI& sa) {
  int n = sz(a);
  VI rev(n), lcp(n - 1);
  forn (i, n) rev[sa[i]] = i;
  int last = 0;
  forn (i, n) {
    last = max(last - 1, 0);
    int j = rev[i];
    if (j == n - 1) continue;
    int k = sa[j + 1];
    while (max(i, k) + last < n &&
      a[i + last] == a[k + last]) last++;
    lcp[j] = last;
  }
  return lcp;
}
