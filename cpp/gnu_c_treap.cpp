#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds; // pb_ds

//typedef tree<KEY, VAL(null_type/null_mapped_type),
//CMP, rb(splay/ov)_tree_tag,
//tree_order_statistics_node_update> TREE;

typedef tree<int,null_type,less<int>, rb_tree_tag,
tree_order_statistics_node_update> TREE;

void solve(TREE m, int k) {
  cout << m.order_of_key(k); // # of keys < k
  cout << *m.find_by_order(k); // k-th element
}
