#include <iostream>
#include <tuple>

#define debug(x) std::cerr << "DEBUG: " << #x << " = " << (x) << std::endl

template<typename T> struct TotalOrder {
  #define CMP(S) \
  inline friend bool operator S(const T& a, const T& b) \
  { return a.tie() S b.tie(); }

  CMP(<); CMP(>); CMP(<=); CMP(>=); CMP(==); CMP(!=);
};

template<typename T> struct Point3D : public TotalOrder<Point3D<T>> {
  T x, y, z;

  Point3D(T x, T y, T z) : x(x), y(y), z(z) {}

  auto tie() const { return std::tie(x, y, z); }
};

int main() {
  Point3D<double> a = {1.0, 1.0, 1.0};
  Point3D<double> b = {1.0, 1.0, 2.0};

  debug(a < b);
  debug(a > b);
  debug(a <= b);
  debug(a >= b);
  debug(a == b);
  debug(a != b);

  return 0;
}
