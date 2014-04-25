// HEAD-HEAD1-HEAD2-...-HEADM
//        |     |    :    |
// R_C  R1_C1-R1_C2-...-R1_CM
//  =     :     :    :    :
//  1   RN_C1-RN_C2-...-RN_CM
				
struct LIST {
  LIST *u, *d, *l, *r, *h;
  int size, index;
  LIST() : size(0), index(-1) { u = d = l = r = h = NULL; }
};
 
inline void remove(LIST* p, vector<LIST*>& backup) {
  if (p->l) p->l->r = p->r; if (p->u) p->u->d = p->d;
  if (p->r) p->r->l = p->l; if (p->d) p->d->u = p->u;
  if (p->h) p->h->size--;
  backup.push_back(p);
}
 
inline void restore(vector<LIST*>& backup) {
	reverse(backup.begin(), backup.end());
	for (LIST* p : backup) {
    if (p->l) p->l->r = p; if (p->u) p->u->d = p;
    if (p->r) p->r->l = p; if (p->d) p->d->u = p;
    if (p->h) p->h->size++;
  }
	backup.resize(0);
}
 
inline void remove_common(LIST* p, vector<LIST*>& backup) {
  remove(p, backup);
  for (LIST* l = p->l; l; l = l->l) remove(l, backup);
  for (LIST* r = p->r; r; r = r->r) remove(r, backup);
}
 
inline void remove_head(LIST* p, vector<LIST*>& backup) {
  for (LIST* h = p->h; h; h = h->d)
    if (h->index >= 0) remove_common(h, backup);
    else remove(h, backup);
}
 
inline void remove_heads(LIST* p, vector<LIST*>& backup) {
  for (LIST* l = p->l; l; l = l->l) remove_head(l, backup);       
  for (LIST* r = p->r; r; r = r->r) remove_head(r, backup);   
  remove_head(p, backup);
}
 
vector<int> ANSWER;
 
bool exact_cover(LIST* HEAD) {
  if (!HEAD->r) return true;    
  LIST* START = HEAD->r;
  for (LIST* TRY = START; TRY; TRY = TRY->r)
    if (TRY->size < START->size) START = TRY;   
  if (START->size == 0) return false;
  vector<LIST*> column, backup;
  for (LIST* p = START->d; p; p = p->d) {
    remove_heads(p, backup);
    if (exact_cover(HEAD)) {
      ANSWER.push_back(p->index);
      restore(backup);
      return true;
    }
    restore(backup);
  }
  return false;
}
