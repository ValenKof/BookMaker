// 2-phase simplex algorithm for solving linear programs of the form
//
// maximize c^T x
// subject to Ax <= b
// x >= 0
//
// INPUT: A -- an m x n matrix
// b -- an m-dimensional vector
// c -- an n-dimensional vector
// x -- a vector where the optimal solution will be stored
//
// OUTPUT: value of the optimal solution (infinity if unbounded
// above, nan if infeasible)

const DOUBLE EPS = 1e-9;
struct LPSolver {
int m, n;
VI B, N;
VVD D;

LPSolver(const VVD &A, const VD &b, const VD &c)
  : m(sz(b)), n(sz(c)), N(n + 1), B(m), D(m + 2, VD(n + 2)) {
  forn (i, m) forn (j, n) D[i][j] = A[i][j];
  forn (i, m) { B[i] = n + i; D[i][n] = -1; D[i][n + 1] = b[i]; }
  forn (j, n) { N[j] = j; D[m][j] = -c[j]; }
  N[n] = -1; D[m + 1][n] = 1;
}

void Pivot(int r, int s) {
  forn (i, m + 1) if (i != r)
    forn (j, n + 2) if (j != s)
      D[i][j] -= D[r][j] * D[i][s] / D[r][s];
  forn (j, n + 2) if (j != s) D[r][j] /= D[r][s];
  forn (i, m + 2) if (i != r) D[i][s] /= -D[r][s];
  D[r][s] = 1.0 / D[r][s];
  swap(B[r], N[s]);
}

bool Simplex(int phase) {
  int x = phase == 1 ? m + 1 : m;
  while (true) {
    int s = -1;
    forn (j, n + 1) {
      if (phase == 2 && N[j] == -1) continue;
      if (s == -1 || D[x][j] < D[x][s] || 
          D[x][j] == D[x][s] && N[j] < N[s]) s = j;
    }
    if (D[x][s] >= -EPS) return true;
    int r = -1;
    forn (i, m) {
      if (D[i][s] <= 0) continue;
      if (r == -1 || D[i][n+1] / D[i][s] < D[r][n+1] / D[r][s] ||
   D[i][n+1]/D[i][s] == D[r][n+1]/D[r][s] && B[i] < B[r]) r = i;
    }
    if (r == -1) return false;
    Pivot(r, s);
  }
}

DOUBLE Solve(VD &x) {
  int r = 0;
  for (int i = 1; i < m; i++) if (D[i][n + 1] < D[r][n + 1]) r = i;
  if (D[r][n + 1] <= -EPS) {
    Pivot(r, n);
    if (!Simplex(1) || D[m + 1][n + 1] < -EPS) 
      return -numeric_limits<DOUBLE>::infinity();
    forn (i, m) if (B[i] == -1) {
      int s = -1;
      forn (j, n + 1)  
        if (s == -1 || D[i][j] < D[i][s] || 
            D[i][j] == D[i][s] && N[j] < N[s]) s = j;
      Pivot(i, s);
    }
  }
  if (!Simplex(2)) return numeric_limits<DOUBLE>::infinity();
  x = VD(n);
  forn (i, m) if (B[i] < n) x[B[i]] = D[i][n + 1];
  return D[m][n+1];
}};

void solve() {
  const int m = 4;
  const int n = 3;
  DOUBLE _A[m][n] = {
    {  6, -1,  0 },
    { -1, -5,  0 },
    {  1,  5,  1 },
    { -1, -5, -1 }
  };
  DOUBLE _b[m] = { 10, -4, 5, -5 }; 
  DOUBLE _c[n] = { 1, -1, 0 };
  VVD A(m);
  VD b(_b, _b + m);
  VD c(_c, _c + n);
  forn (i, m) A[i] = VD(_A[i], _A[i] + n);
  LPSolver solver(A, b, c);
  VD x;
  DOUBLE value = solver.Solve(x);
  cout << "VALUE: "<< value << endl;
  cout << "SOLUTION:";
  forn (i, sz(x)) cout << " " << x[i];
}
 
