// HEAD-HEAD1-HEAD2-...-HEADM
//        |     |    :    |
// R_C  R1_C1-R1_C2-...-R1_CM
//  =     :     :    :    :
//  1   RN_C1-RN_C2-...-RN_CM

#define FOR(p, init, dir) for(pList p = init; p; p = p->dir)
#define IF(p, l, r, q) if(p->l) p->l->r = q

struct List {
  List *u, *d, *l, *r, *head, *stack;
  int number;
  List() : number(0) { u = d = l = r = head = stack = NULL; }
};

typedef List* pList;

inline void remove(pList p, pList& backup) {
  IF(p, l, r, p->r); IF(p, u, d, p->d);
  IF(p, r, l, p->l); IF(p, d, u, p->u);
  p->head->number--;
  p->stack = backup; backup = p;
}

inline void restore(pList& p) {
  for (; p; p = p->stack) {
    IF(p, l, r, p); IF(p, u, d, p);
    IF(p, r, l, p); IF(p, d, u, p);
    p->head->number++;
  }
}

inline void remove_common(pList p, pList& backup) {
  remove(p, backup);
  FOR(l, p->l, l) remove(l, backup);
  FOR(r, p->r, r) remove(r, backup);
}

inline void remove_head(pList h, pList& backup) {
  h = h->head; remove(h, backup);
  FOR(p, h->d, d) remove_common(p, backup);
}

inline void remove_heads(pList p, pList& backup) {
  FOR(l, p->l, l) remove_head(l, backup);
  FOR(r, p->r, r) remove_head(r, backup);
}

vector<int> answer;

inline int exact_cover(pList head) {
  if (!head->r) return 1;
  pList cur = head->r;
  FOR(p, cur, r) if (p->number < cur->number) cur = p;
  if (cur->number == 0) return 0;
  pList col = NULL, backup = NULL;
  remove_head(cur->d, col);
  FOR(p, cur->d, d) {
    remove_heads(p, backup);
    if (exact_cover(head)) {
      answer.pb(p->number); restore(backup); restore(col);
      return 1;
    } restore(backup);
  } restore(col);
  return 0;
}

template<int NUM_BIT>
pair<bool, vector<int>> solve(const vector<bitset<NUM_BIT>>& have, const bitset<NUM_BIT>& need) {
  pList HEAD = new List();
  HEAD->number = -1;
  pList LAST_HEAD = HEAD;
  vector<pList> LAST_COL(NUM_BIT, NULL);
  vector<pList> HEADS(NUM_BIT, NULL);
  for (int i = 0; i < NUM_BIT; ++i) {
    if (need[i]) {
      pList LAST = new List();
      LAST->head = LAST;
      LAST->number = 0;
      LAST_HEAD->r = LAST;
      LAST->l = LAST_HEAD;
      LAST_HEAD = LAST;
      HEADS[i] = LAST;
      LAST_COL[i] = LAST;
    }
  }
  forn (i, sz(have)) {
    if ((have[i] & need) == have[i]) {
      pList CURR = NULL;
      forn (j, NUM_BIT) {
        if (have[i][j]) {
          pList LAST = new List();
          HEADS[j]->number++;
          LAST->head = HEADS[j];
          LAST->number = i;
          LAST->l = CURR;
          if (CURR) CURR->r = LAST;
          CURR = LAST;
          LAST->u = LAST_COL[j];
          LAST_COL[j]->d = LAST;
          LAST_COL[j] = LAST;
        }
      }
    }
  }
  answer.clear();
  bool ok = exact_cover(HEAD);
  while (HEAD != NULL) {
    pList CURR = HEAD;
    HEAD = HEAD->r;
    while (CURR != NULL) {
        pList NEXT = CURR->d;
        delete CURR;
        CURR = NEXT;
    }
  }
  return make_pair(ok, answer);
}
