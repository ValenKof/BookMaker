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
  vector<int> d1(n), d2(n); // d2[0] useless
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = (i > r ? 0 : min(d1[l+r-i], r-i)) + 1;
    while (0 <= i-k && i+k < n && s[i-k] == s[i+k]) ++k;
    d1[i] = --k; // [i - d, i + d], len = 2 * d + 1
    if (i + k > r) l = i - k, r = i + k;
  }
  for (int i = 0, l = 0, r = -1; i < n; ++i) {
    int k = (i > r ? 0 : min(d2[l+r-i+1], r-i+1)) + 1;
    while (0 <= i-k && i+k-1 < n && s[i-k] == s[i+k-1]) ++k;
    d2[i] = --k; // [i - d, i + d), len = 2 * d
    if (i + k - 1 > r) l = i - k, r = i + k - 1;
  }
}

VI z_function(const string& s) {
  int n = sz(s);
  VI z(n, 0); // z[i] = lcp(s[0..n], s[i..n]), z[0] = 0 !
  for (int i = 1, l = 0, r = 0; i < n; ++i) {
    if (i <= r) z[i] = min(r - i + 1, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    if (i + z[i] - 1 > r) l = i,  r = i + z[i] - 1;
  }
  return z;
}

VI prefix_function(const string& s) {
  int n = sz(s);
  VI pi(n, 0); // p[i] = max k <= i : s[0..k) = s(i-k..i]
  for (int i = 1; i < n; ++i) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) ++j;
    pi[i] = j;
  }
  return pi;
}

// create - O(pattern) memory, O(pattern) time
// query - O(1) memory, O(text) time

struct Matcher {
  string pattern_;
  vector<int> prefix_;

  Matcher(const string& pattern)
    : pattern_(pattern)
    , prefix_(sz(pattern))
  {
    prefix_[0] = 0;
    for (int i = 1; i < sz(pattern_); ++i) {
      int j = prefix_[i - 1];
      while (j > 0 && pattern_[i] != pattern_[j]) {
        j = prefix_[j - 1];
      }
      if (pattern_[i] == pattern_[j]) {
        j++;
      }
      prefix_[i] = j;
    }
  }

  int match(const string& text) {
    int j = 0;
    for (int i = 0; i < sz(text); ++i) {
      while (j > 0 && (j == sz(pattern_) || text[i] != pattern_[j])) {
        j = prefix_[j - 1];
      }
      if (text[i] == pattern_[j]) {
        j++;
      }
      if (j == sz(pattern_)) {
        return i + 1 - sz(pattern_);
      }
    }
    return -1;
  }
};
