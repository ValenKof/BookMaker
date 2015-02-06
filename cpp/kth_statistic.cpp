struct Node { Node *l, *r; int s; };

const int N = 100000;
Node nodes[20 * N];
int used = 0;
pair<int, int> a[N];
Node* roots[N + 1];
int pos[N];

Node* insert(Node* t, int l, int r, int x) {
  if (l + 1 == r) return nodes + 1; //NEW FILLED LEAF
  int m = (l + r) / 2;
  Node *newL = t->l, *newR = t->r;
  if (x < m) newL = insert(newL, l, m, x);
  else       newR = insert(newR, m, r, x);
  nodes[used] = {newL, newR, t->s + 1};
  return nodes + used++;
}

int find(Node* tl, Node* tr, int l, int r, int k) {
  if (l + 1 == r) return l;
  int m = (l + r) / 2;
  int sum = tr->l->s - tl->l->s;
  if (sum < k) return find(tl->r, tr->r, m, r, k - sum);
  else         return find(tl->l, tr->l, l, m, k);
}

int main() {
  int n, m;
  scanf("%d %d", &n, &m);
  forn (i, n) {
    scanf("%d", &a[i].first);
    a[i].second = i;
  }
  sort(a, a + n);
  forn (i, n) pos[a[i].second] = i;
  //BUILD
  nodes[used++] = { nodes, nodes, 0 }; //RECURSIVE EMPTY TREE
  nodes[used++] = { nodes, nodes, 1 }; //FILLED LEAF
  roots[0] = nodes;
  forn (i, n) roots[i+1] = insert(roots[i], 0, n, pos[i]);
  //ANSWER
  forn (i, m) {
    int l, r, k; // 1 <= l, r <= n
    scanf("%d %d %d", &l, &r, &k);
    printf("%d\n", a[find(roots[l-1], roots[r], 0, n, k)].first);
  }
  return 0;
}