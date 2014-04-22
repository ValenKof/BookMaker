pair<int, VI> GetMinCut(VVI& weights, int N) {
  VI used(N), cut, best_cut;
  int best_weight = -1;
  for (int phase = N - 1; phase >= 0; phase--) {
    VI w = weights[0];
    VI added = used;
    int prev, last = 0;
    forn (i, phase) {
      prev = last;
      last = -1;
      for (int j = 1; j < N; j++)
        if (!added[j] && (last == -1 || w[j] > w[last])) last = j;
      if (i == phase - 1) {
        forn (j, N) weights[prev][j] += weights[last][j];
        forn (j, N) weights[j][prev] = weights[prev][j];
        used[last] = true;
        cut.pb(last);
        if (best_weight == -1 || w[last] < best_weight) {
          best_cut = cut;
          best_weight = w[last];
        }
      } else {
        forn (j, N) w[j] += weights[last][j];
        added[last] = true;
      }
    }
  }
  return mp(best_weight, best_cut);
}
