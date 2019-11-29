struct dom_tree {
  vvi g, rg, tree, bucket;
  vi sdom, par, dom, dsu, label, in, order, tin, tout;
  int T = 0, root = 0, n = 0;

  void dfs_tm (int x) {
    in[x] = T;
    order[T] = x;
    label[T] = T, sdom[T] = T, dsu[T] = T, dom[T] = T;
    T++;
    for (int to : g[x]) {
      if (in[to] == -1) {
        dfs_tm(to);
        par[in[to]] = in[x];
      }
      rg[in[to]].pb(in[x]);
    }
  }

  void dfs_tree (int v, int p) {
    tin[v] = T++;
    for (int dest : tree[v]) {
      if (dest != p) {
        dfs_tree(dest, v);
      }
    }
    tout[v] = T;
  }

  dom_tree (const vvi &g_, int root_) {
    g = g_;
    n = sz(g);
    assert(0 <= root && root < n);
    in.assign(n, -1);
    rg.resize(n);
    order = sdom = par = dom = dsu = label = vi(n);
    root = root_;
    bucket.resize(n);
    tree.resize(n);

    dfs_tm(root);

    for (int i = n - 1; i >= 0; i--) {
      for (int j : rg[i])
        sdom[i] = min(sdom[i], sdom[find(j)]);
      if (i > 0)
        bucket[sdom[i]].pb(i);

      for (int w : bucket[i]) {
        int v = find(w);
        dom[w] = (sdom[v] == sdom[w] ? sdom[w] : v);
      }

      if (i > 0)
        unite(par[i], i);
    }

    for (int i = 1; i < n; i++) {
      if (dom[i] != sdom[i])
        dom[i] = dom[dom[i]];
      tree[order[i]].pb(order[dom[i]]);
      tree[order[dom[i]]].pb(order[i]);
    }

    T = 0;
    tin = tout = vi(n);
    dfs_tree(root, -1);
  }

  void unite (int u, int v) {
    dsu[v] = u;
  }

  int find (int u, int x = 0) {
    if (u == dsu[u])
      return (x ? -1 : u);
    int v = find(dsu[u], x + 1);
    if (v == -1)
      return u;
    if (sdom[label[dsu[u]]] < sdom[label[u]])
      label[u] = label[dsu[u]];
    dsu[u] = v;
    return (x ? v : label[u]);
  }

  bool dominated_by (int v, int by_what) {
    return tin[by_what] <= tin[v] && tout[v] <= tout[by_what];
  }
};
