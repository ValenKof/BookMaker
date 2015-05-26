template<typename I, typename T>
inline T kahan(const I& begin, const I& end, T s = 0) {
  T c = 0;
  for (I it = begin; it != end; ++it) {
    T y = *it - c;
    T t = s + y;
    c = (t - s) - y;
    s = t;
  }
  return s;
}
