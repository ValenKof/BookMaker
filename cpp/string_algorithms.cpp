string min_cyclic_shift(string s) {
  s += s;
  int n = sz(s), i = 0, ans = 0;
  while (i < n / 2) {
    ans = i;
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      k = (s[k] < s[j]) ? (i) : (k + 1);
      j++;
    }
    while (i <= k) i += j - k;
  }
  return s.substr(ans, n / 2);
}

void pal(const string& s) {
  int n = sz(s);
  VI d1(n), d2(n);
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = (i > r ? 0 : min(d1[l + r - i], r - i)) + 1;
    while (i+k < n && i-k >= 0 && s[i+k] == s[i-k]) k++;
    d1[i] = k--;
    if (i + k > r) l = i - k,  r = i + k;
  }
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = (i > r ? 0 : min(d2[l + r - i + 1], r - i + 1)) + 1;
    while (i+k-1 < n && i-k >= 0 && s[i+k-1] == s[i-k]) k++;
    d2[i] = --k;
    if (i + k - 1 > r) l = i - k,  r = i + k - 1;
  }
}

VI z_function(const string& s) {
  int n = sz(s);
  VI z(n);
  for (int i = 1, l = 0, r = 0; i < n; ++i) {
    if (i <= r) z[i] = min(r - i + 1, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    if (i + z[i] - 1 > r) l = i,  r = i + z[i] - 1;
  }
  return z;
}

VI prefix_function(const string& s) {
  int n = sz(s);
  VI pi(n);
  for (int i = 1; i < n; ++i) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) ++j;
    pi[i] = j;
  }
  return pi;
}