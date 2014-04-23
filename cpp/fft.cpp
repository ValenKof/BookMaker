typedef complex<double> base; //struct { double re, im } better
//const int MOD=1051721729, GEN=330, REV=rev(GEN,MOD), POWER=1<<20;
//binPow(GEN,POWER,MOD) = 1, binPow(GEN,x,MOD) != 1  0<x<POWER
void fft(vector<base>& a, bool invert) {
  int n = sz(a);  
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    while (j & bit) {
      j ^= bit;
      bit >>= 1;
    }
    j |= bit;
    if (i < j) swap(a[i], a[j]);
  }
 
  for (int len = 1; len < n; len <<= 1) {
    double ang = (2*PI) / (2*len) * (invert ? -1 : 1);
    base wlen(cos(ang), sin(ang)); 
    //int wlen = binPow(invert?REV:GEN,POWER/(len*2), MOD);
    for (int i = 0; i < n; i += 2 * len) {
      base w(1, 0); // 1
      forn (j, len) {
        base u = a[i + j];
        base v = a[i + j + len] * w; // � %MOD
        a[i + j] = u + v; // � %MOD
        a[i + j + len] = u - v; // � %MOD
        w = w * wlen; // � %MOD
      }
    }
  }
  if (invert) forn (i, n) a[i] /= n; // a[i]=(a[i]*rev(n,MOD))%MOD;
}

void tr(vector<vector<base> >& a) {
  forn (i, sz(a)) forn (j, i) swap(a[i][j], a[j][i]);
}

void fft(vector<vector<base> >& a, bool invert) {
  if (invert) tr(a);
  forn (i, sz(a)) fft(a[i], invert);
  tr(a);
  forn (i, sz(a)) fft(a[i], invert); 
  if (!invert) tr(a);
}
