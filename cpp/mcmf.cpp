template<typename TCap, typename TCost, typename TCapLarge = TCap, typename TCostLarge = TCost>
struct Graph
{
    typedef function<TCost(int, int, int)> CostFunction;

    // nodes
    vector<int> head;
    vector<int> from;
    vector<int> q;
    vector<char> used;
    vector<TCost> phi;
    vector<TCost> dist;

    // edges
    vector<int> dest;
    vector<int> next;
    vector<TCap> cap;
    vector<TCost> cost;

    int n, s, t, mask;

    CostFunction simple;
    CostFunction potential;

    Graph(int n, int capacity)
        : head(n, -1)
        , from(n)
        , used(n)
        , phi(n)
        , dist(n)
        , n(n)
        , s(n - 2)
        , t(n - 1)
        , simple([&](int arc, int u, int v) { return cost[arc]; })
        , potential([&](int arc, int u, int v) { return cost[arc] + phi[u] - phi[v]; })
    {
        dest.reserve(capacity);
        next.reserve(capacity);
        cap.reserve(capacity);
        cost.reserve(capacity);
        int mask = 1;
        while (mask <= n) {
            mask <<= 1;
        }
        mask--;
        this->mask = mask;
        q.resize(mask + 1);
    }

    inline void addArc(int u, int v, TCap cap, TCost cost)
    {
        this->dest.pb(v);
        this->next.pb(head[u]);
        this->cap.pb(cap);
        this->cost.pb(cost);
        head[u] = sz(dest) - 1;
    }

    inline void addEdge(int u, int v, TCap cap, TCost cost)
    {
        addArc(u, v, cap, cost);
        addArc(v, u, 0, -cost);
    }

    inline bool bf(vector<TCost>& dist, CostFunction cost)
    {
        fill(all(dist), numeric_limits<TCost>::max());
        fill(all(used), false);
        fill(all(from), -1);
        int qh = 0, qt = 0;

        dist[s] = 0;
        used[s] = true;
        q[qt] = s; qt = (qt + 1) & mask;
        while (qh != qt) {
            int u = q[qh]; qh = (qh + 1) & mask;
            TCost du = dist[u];
            used[u] = false;
            for (int arc = head[u]; arc != -1; arc = next[arc]) {
                int v = dest[arc];
                TCost dv = du + cost(arc, u, v);
                if (cap[arc] > 0 && dv < dist[v]) {
                    dist[v] = dv;
                    from[v] = arc;
                    if (!used[v]) {
                        q[qt] = v; qt = (qt + 1) & mask;
                        used[v] = true;
                    }
                }
            }
        }
        return dist[t] < numeric_limits<TCost>::max();
    }

    inline bool dijkstra_dense(std::vector<TCost>& dist, CostFunction cost) {
        std::fill(all(dist), std::numeric_limits<TCost>::max());
        std::fill(all(used), false);
        std::fill(all(from), -1);

        int blockPower = 0;
        while ((1 << blockPower) * (1 << blockPower) < n) {
            blockPower++;
        }
        int blockSize = (1 << blockPower);
        int blockCount =  (n + blockSize + 1) >> blockPower;
        std::vector<int> distBlock(blockCount, -1);
        dist[s] = 0;
        distBlock[s >> blockPower] = s;

        forn (i, n) {
            // find
            int u = -1;
            for (int x : distBlock) {
                if (x != -1 && (u == -1 || dist[x] < dist[u])) {
                    u = x;
                }
            }
            if (u == -1) {
                break;
            }
            // update current block
            used[u] = true;
            int block = u >> blockPower;
            int l = block << blockPower;
            int r = min(n, l + blockSize);
            distBlock[block] = -1;
            for (int j = l; j < r; ++j) {
                if (!used[j] && (distBlock[block] == -1 || dist[j] < dist[distBlock[block]])) {
                    distBlock[block] = j;
                }
            }
            if (dist[distBlock[block]] == numeric_limits<TCost>::max()) {
                distBlock[block] = -1;
            }
            // update new nodes
            TCost du = dist[u];
            for (int arc = head[u]; arc != -1; arc = next[arc]) {
                int v = dest[arc];
                TCost dv = du + cost(arc, u, v);
                if (cap[arc] > 0 && dv < dist[v]) {
                    dist[v] = dv;
                    from[v] = arc;
                    block = v >> blockPower;
                    if (!used[v] && (distBlock[block] == -1 || dv < dist[distBlock[block]])) {
                        distBlock[block] = v;
                    }
                }
            }
        }
        return used[t];
    }

    inline bool dijkstra_sparse(vector<TCost>& dist, CostFunction cost)
    {
        fill(all(dist), numeric_limits<TCost>::max());
        fill(all(from), -1);
        typedef pair<TCost, int> T;
        priority_queue<T, vector<T>, greater<T>> pq;
        dist[s] = 0;
        pq.push(mp(dist[s], s));
        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            int u = top.second;
            TCost du = top.first;
            if (dist[u] != du) {
                continue;
            }
            for (int arc = head[u]; arc != -1; arc = next[arc]) {
                int v = dest[arc];
                TCost dv = du + cost(arc, u, v);
                if (cap[arc] > 0 && dv < dist[v]) {
                    dist[v] = dv;
                    pq.push(mp(dist[v], v));
                    from[v] = arc;
                }
            }
        }
        return dist[t] < numeric_limits<TCost>::max();
    }

    inline pair<TCapLarge, TCostLarge> maxPath(
        TCapLarge minCap = numeric_limits<TCapLarge>::max())
    {
        TCostLarge sumCost = 0;
        for (int v = t; v != s; v = dest[from[v] ^ 1]) {
            // debug(v);
            mn(minCap, cap[from[v]]);
            sumCost += cost[from[v]];
        }
        for (int v = t; v != s; v = dest[from[v] ^ 1]) {
            cap[from[v]] -= minCap;
            cap[from[v] ^ 1] += minCap;
        }
        return mp(minCap, sumCost * minCap);
    }

    pair<TCapLarge, TCostLarge> mcmf()
    {
        pair<TCapLarge, TCostLarge> result = mp(0, 0);
        // Case 1: only bf
        // while (bf(dist, simple)) {
            // auto path = maxPath();
            // result.first += path.first;
            // result.second += path.second;
        // }

        // Case 2: bf + dijkstra
        bf(phi, simple);
        // or
        // fill(all(phi), 0);
        while (dijkstra_sparse(dist, potential)) {
            forn (i, n) {
                if (dist[i] != numeric_limits<TCost>::max()) {
                    phi[i] += dist[i];
                }
            }
            auto path = maxPath();
            result.first += path.first;
            result.second += path.second;
        }
        return result;
    }
};

void solve() {
  int k;
  scanf("%d", &k);
  vector<vector<int>> a(k, vector<int>(k));
  forn (i, k) forn (j, k) scanf("%d", &a[i][j]);

  Graph<int, int> g(1 + k + k + 1, 2 * (k + k * k + k));
  forn (i, k) g.addEdge(g.s, i, 1, 0);
  forn (i, k) g.addEdge(i + k, g.t, 1, 0);
  forn (i, k) forn (j, k) g.addEdge(i, j + k, 1, a[i][j]);
  auto mcmf = g.mcmf();
  printf("cap: %d, cost: %d\n", mcmf.first, mcmf.second);
}