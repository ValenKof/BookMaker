int mod(int a, int b) { return ((a % b) + b) % b; }
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
int lcm(int a, int b) { return a / gcd(a, b) *b; }

int gcd(int a, int b, int &x, int &y) {
  if (a == 0) {
    x = 0; y = 1;
    return b;
  }
  int x1, y1;
  int d = gcd(b % a, a, x1, y1);
  x = y1 - (b / a) * x1;
  y = x1;
  return d;
}

// finds all solutions to ax = b (mod n)
VI modular_linear_equation_solver(int a, int b, int n) {
  int x, y;
  VI solutions;
  int d = gcd(a, n, x, y);
  if (!(b % d)) {
    x = mod(x * (b / d), n);
    forn (i, d) solutions.pb(mod(x + i*(n/d), n));
  }
  return solutions;
}

// solve a^x = b (mod m) in sqrt(m) * log(m)
int solve(int a, int b, int m) {
  int n = (int) sqrt(m + 0.0) + 1;
  int an = 1;
  forn (i, n) an = (an * a) % m; // or bin_pow(a, n, m);
  map<int, int> vals;
  for (int i = 1, cur = an; i <= n; ++i) {
    if (!vals.count(cur)) vals[cur] = i;
    cur = (cur * an) % m;
  }
  for (int i = 0, cur = b; i <= n; ++i) {
    if (vals.count(cur)) {
      int ans = vals[cur] * n - i;
      if (ans < m) return ans;
    }
    cur = (cur * a) % m;
  }
  return -1;
}

// computes b such that ab = 1 (mod n), returns -1 on failure
int mod_inverse(int a, int n) {
  int x, y;
  int d = gcd(a, n, x, y);
  if (d > 1) return -1;
  return mod(x, n);
}

// Chinese remainder theorem (special case): find z such that
// z % x = a, z % y = b. Here, z is unique modulo M = lcm(x,y).
// Return (z,M). On failure, M = -1.
PII chinese_rem_theorem(int x, int a, int y, int b) {
  int s, t;
  int d = gcd(x, y, s, t);
  if (a % d != b % d) return mp(0, -1);
  return mp(mod(s * b * x + t * a * y, x * y) / d, x * y/d);
}

// Chinese remainder theorem: find z such that
// z % x[i] = a[i] for all i. Note that the solution is
// unique modulo M = lcm_i (x[i]). Return (z,M). On
// failure, M = -1.
PII chinese_remainder_theorem(const VI &x, const VI &a) {
  PII ret = make_pair(a[0], x[0]);
  for (int i = 1; i < sz(x); ++i) {
    ret = chinese_rem_theorem(ret.first, ret.second, x[i], a[i]);
    if (ret.second == -1) break;
  }
  return ret;
}

// Garner pseudocode !!! r[i][j] = inv p[i] mod p[j]
BigInt value(VI& a, VI& p, VVI& r, int n, bool can_be_neg) {
  BigInt result = 0, mult = 1;
  VI x(n);
  for (int i = 0; i < n; ++i) {
      x[i] = a[i];
      for (int j = 0; j < i; ++j) {
        ll cur = 1LL * (x[i] - x[j]) * r[j][i];
        x[i] = cur % p[i];
        if (x[i] < 0) x[i] += p[i];
      }
      result += mult * x[i];
      mult *= p[i];
  }
  if (can_be_neg && result >= (mult >> 1)) result -= mult;
  return result;
}

// computes x and y such that ax + by = c; on failure, x = y =-1
void linear_diophantine(int a, int b, int c, int &x, int &y) {
  int d = gcd(a, b);
  if (c % d) {
    x = y = -1;
  } else {
    x = c / d * mod_inverse(a / d, b / d);
    y = (c - a * x) / b;
  }
}

void mod_inverse_all(int m, int r[]) {
  r[1] = 1;
  for (int i = 2; i < m; ++i)
    r[i] = (m - (m / i) * r[m % i] % m) % m;
}