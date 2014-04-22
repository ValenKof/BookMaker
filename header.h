#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <complex>
#include <queue>

using namespace std;

#define debug(x) cerr << "DEBUG: " << #x << " = " << x << endl
#define forn(i, n) for(int i = 0; i < (n); ++i)
#define all(x) x.begin(), x.end()
#define mp make_pair
#define pb push_back

typedef long long ll;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int, int> PII;
typedef long long BigInt;
typedef long double DOUBLE;
typedef vector<DOUBLE> VD;
typedef vector<VD> VVD;

template <class T> inline void mn(T& x, T& y) { if (y < x) x = y; }
template <class T> inline void mx(T& x, T& y) { if (x < y) x = y; }
template <class T> inline int sz(const T& x) { return x.size(); }

const double PI = 2 * acos(0);