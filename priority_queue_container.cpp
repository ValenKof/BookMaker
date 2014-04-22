template <typename T, int N>
struct MyAllocator : public array<T, N> {
  int pos; //default 0
  void push_back(T value) { *(array<T,N>::begin()+pos++)=value; }
  T* end() { return array<T,N>::begin() + pos;}
  void pop_back() { --pos; }
  bool empty() const { return pos == 0; }
};
priority_queue<int, MyAllocator<int, 5000>, greater<int> > q;
