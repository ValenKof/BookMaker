typedef double T; 
const T EPS = 1e-7; 
T sqr(T x) { return x * x; }

int cmp(T a, T b) {
  if (a > b + EPS) return +1;
  if (a + EPS < b) return -1;
  return 0;
}

int sign(T x) { return cmp(x, 0); }

struct PT { 
  T x, y; 
  PT() { } 
  PT(T x, T y) : x(x), y(y) { } 
  PT RotateCCW90() { return PT(-y, x); }
  PT RotateCW90() { return PT(y, -x); }
  PT RotateCCW(T a) {
    T cosa = cos(a), sina = sin(a);
    return PT(x*cosa-y*sina, x*sina+y*cosa);
  }
  PT operator+(PT p) {return PT(x+p.x,y+p.y);}
  PT operator-(PT p) {return PT(x-p.x,y-p.y);}
  PT operator*(T d) {return PT(x*d,y*d);}
  PT operator/(T d) {return PT(x/d,y/d);}
};
typedef vector<PT> PTS;

bool operator<(PT a, PT b)
{ return (cmp(a.x,b.x)?cmp(a.x,b.x):cmp(a.y,b.y)) < 0; } 

bool operator==(PT a, PT b)
{ return !cmp(a.x, b.x) && !cmp(a.y, b.y); }

T dot(PT a, PT b) {return a.x*b.x + a.y*b.y;}
T dist2(PT a, PT b) {return sqr(a.x-b.x)+sqr(a.y-b.y);}  
T cross(PT p, PT q) { return p.x * q.y - p.y * q.x; } 
T area2(PT a, PT b, PT c) {return cross(a,b)+cross(b,c)+cross(c,a);} 
 
/// CONVEX HULL
PTS ConvexHull(PTS& pts) {
  sort(all(pts));
  PTS up, dn; 
  for (PT p : pts) {
    while(sz(up)>1 && area2(up[sz(up)-2],up.back(),p)>=0)up.pop_back();
    while(sz(dn)>1 && area2(dn[sz(dn)-2],dn.back(),p)<=0)dn.pop_back();
    up.pb(p), dn.pb(p); 
  }
  copy(dn.rbegin() + 1, dn.rend() - 1, back_inserter(up));
  return up;
}

/// POINT IN POLYGON, +1 outside, 0 border, -1 inside
int PointInPolygon(const PTS& p, PT q) { 
  int cnt[3] = { 0, 0, 0 };
  for (int i = 0; i < sz(p); i++) { 
    int j = (i + 1) % sz(p); 
    cnt[sign(area2(p[i], q, p[j])) + 1]++;
  } 
  return (cnt[0] && cnt[2]) ? +1 : (cnt[1] ? 0 : -1);
}

/// INTERSECTION OF CIRCLES (a, r) and (b, R)
PTS CircleCircleIntersection(PT a, PT b, T r, T R) { 
  PTS ret; 
  T d = sqrt(dist2(a, b)); 
  if (d > r+R || d+min(r, R) < max(r, R)) return ret; 
  T x = (sqr(d)-sqr(R)+sqr(r))/(2*d); 
  T y = sqrt(sqr(r)-sqr(x)); 
  PT v = (b-a)/d; 
  ret.push_back(a+v*x + v.RotateCCW90()*y); 
  if (y > 0) ret.push_back(a+v*x - v.RotateCCW90()*y); 
  return ret; 
}

/// INTERSECTION OF CIRCLE (c, r) and line (a, b)
PTS CircleLineIntersection(PT a, PT b, PT c, T r) { 
  PTS ret; 
  b = b - a; 
  a = a - c; 
  T A = dot(b, b);
  T B = dot(a, b);
  T C = dot(a, a) - sqr(r);
  T D = sqr(B) - A * C; 
  if (D < -EPS) return ret;
  ret.push_back(c+a+b*(-B+sqrt(D+EPS))/A); 
  if (D > EPS) ret.push_back(c+a+b*(-B-sqrt(D))/A); 
  return ret; 
}

/// SPHERE SEGMENT AREA. H � SEGMENT HEIGHT
T SphereSegmentArea(T H, T R) {return 2*PI*R*H; }

/// SPHERICAL DISTANCE.THETA(T)�LATITUDE(V^OXY), PHI(P)�LONGITUDE
T SphericalDistance(T t1, T p1, T t2, T p2, T R)
{return R*acos(sin(t1)*sin(t2) + cos(t1)*cos(t2)*cos(p1 - p2));}
