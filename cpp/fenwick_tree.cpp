const int N = 128;
struct point { int x, y, z; };
int n, k, q;
point p, l, r;
int t[N + 1][N + 1][N + 1];

void update() {
  for (int x = p.x + 1; x <= n; x += x & -x)
  for (int y = p.y + 1; y <= n; y += y & -y)
  for (int z = p.z + 1; z <= n; z += z & -z)
    t[x][y][z] += k;
}

int s(int xx, int yy, int zz, int sum = 0) {
  for (int x = xx + 1; x > 0; x -= x & -x)
  for (int y = yy + 1; y > 0; y -= y & -y)
  for (int z = zz + 1; z > 0; z -= z & -z)
    sum += t[x][y][z];
  return sum;
}

int ans() {
  --l.x, --l.y, --l.z;
  return s(r.x, r.y, r.z) - s(l.x, l.y, l.z)
       - s(r.x, r.y, l.z) - s(r.x, l.y, r.z) - s(l.x, r.y, r.z)
       + s(r.x, l.y, l.z) + s(l.x, r.y, l.z) + s(l.x, l.y, r.z);
}

void solve() {
  cin >> n;
  while ((cin >> q) && q != 3) {
    if (q == 1) { // 0 <= x, y, z < N
      cin >> p.x >> p.y >> p.z >> k;
      update();
    } else { // s[l.x...r.x][l.y...r.y][l.z...r.z]
      cin >> l.x >> l.y >> l.z >> r.x >> r.y >> r.z;
      cout << ans() << endl;
    }
  }
}