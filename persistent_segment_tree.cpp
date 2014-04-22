const int N = 1000000;

struct Tree {
  int s;
  Tree *l, *r;
  Tree() : s(0), l(NULL), r(NULL) { }
  Tree(int s, Tree* l, Tree* r) : s(s), l(l), r(r) { }
};

Tree* roots[N + 1];

Tree* EMPTY_TREE() { 
  Tree* v = new Tree(); 
  v->l = v->r = v; 
  return v; 
}

Tree* add(int x, int l, int r, Tree* t) {
  Tree* node = new Tree(t->s + 1, t->l, t->r);
  if (l + 1 != r) {
    int m = (l + r) / 2;
    if (x < m) node->l = add(x, l, m, t->l);
    else node->r = add(x, m, r, t->r);
  }
  return node;
}

int tree_sum(int ql, int qr, int l, int r, Tree* t1, Tree* t2) {
  if (ql <= l && r <= qr) return t2->s - t1->s;
  if (r <= ql || qr <= l) return 0;
  int m = (l + r) / 2;
  return tree_sum(ql, qr, l, m, t1->l, t2->l)
       + tree_sum(ql, qr, m, r, t1->r, t2->r);
}

// 1 <= l <= r <= n
inline int sum(int l1, int r1, int l2, int r2, int n) {
  return tree_sum(l2-1, r2, 0, n, roots[l1-1], roots[r2]); 
}
