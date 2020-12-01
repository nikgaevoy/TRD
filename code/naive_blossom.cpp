struct blossom {
    int n;
    vector<vector<int>> g;
    vector<int> match, parent, blossom_root;
    /// match[x] is the vertex x is matched with (or -1 if x is not matched)
    /// parent[x] allows to restore the augmenting path
    /// normally it would be a forest, but our reality is more complicated:
    /// even the case parent[x] = y, parent[y] = x would be quite a common occurence
    /// even then, parent[x] "kind of" form a forest, but with two copies of each vertex
    /// and values for one of the copies being occassionally overwritten
    /// with the values for another, if that makes sense
    /// blossom_root[x] is the highest vertex (in the sense of the bfs/dfs forest)
    /// in the current largest blossom, containing x;
    /// because blossoms are actually vertices in the compressed graph,
    /// then, originally, blossom_root[x] = x, because no blossoms were found yet
    /// and the graph has not been compressed at all

    blossom (const vector<vector<int>> &g_) {
        n = int(g_.size());
        g = g_;
        match.assign(n, -1);
        parent.assign(n, -1);
        blossom_root.resize(n);
        /// not filling the array here, because we should
        /// do that on every iteration
    }

    void restore_augmenting_path (int from, int to) {
        assert(match[from] == -1 && match[to] == -1);
        vector<int> path;

        int cur = to;
        while (cur != -1) {
            path.push_back(cur);
            cur = parent[cur];
            assert(match[cur] != -1 || cur == from);
            path.push_back(cur);
            cur = match[cur];
        }

        assert(path.size() % 2 == 0);
        assert(path.back() == from && path.front() == to);
        for (int i = 1; i < int(path.size()) - 1; i += 2)
            assert(match[path[i]] == path[i + 1]);

        for (int i = 0; i < int(path.size()); i += 2) {
            match[path[i]] = path[i + 1];
            match[path[i + 1]] = path[i];
        }
    }

    int blossoms_lca (int v, int u) {
        /// finds lca in O(length of the path in the tree) time

        static vector<int> used;
        if (used.size() != n)
            used.resize(n);

        static int tmr = 0;
        tmr++;

        while (true) {
            if (v != -1) {
                if (used[v] == tmr) {
                    return v;
                }

                used[v] = tmr;
                if (match[v] != -1 && parent[match[v]] != -1)
                    v = blossom_root[parent[match[v]]];
                else
                    v = -1;
            }

            assert(v != -1 || u != -1);
            swap(v, u);
        }

        assert(false);
        return -1;
    }

    void reorder_parents_within_blossom (int cur_vertex, int new_parent, int root, queue<int> &q) {
        while (blossom_root[cur_vertex] != root) {
            parent[cur_vertex] = new_parent;
            blossom_root[cur_vertex] = root;

            new_parent = match[cur_vertex];
            if (new_parent == -1)
                break;
            if (color[new_parent] == 1) {
                /// now this vertex is even as well
                color[new_parent] = 0;
                q.push(new_parent);
            }
            blossom_root[new_parent] = root;
            cur_vertex = parent[new_parent];
        }
    }

    vector<int> color;
    /// the vertex is either not visited yet (-1), or it is of the color 1
    /// (odd distance from the start so far), or it has the color 0
    /// (even distance from the start so far OR
    /// it was of color 1 originally, but we found some blossom and
    /// going along the blossom in the opposite direction makes this vertex
    /// be the SECOND vertex of the matching edge
    /// (if we count along the current path)

    void try_augment (int start) {
        queue<int> q;
        color.assign(n, -1);
        iota(blossom_root.begin(), blossom_root.end(), 0);
        q.push(start);
        color[start] = 0;

        while (!q.empty()) {
            const int cur = q.front(); q.pop();

            for (int dest : g[cur]) {
                if (color[dest] == -1) {
                    parent[dest] = cur;

                    if (match[dest] == -1) {
                        /// we found an augmenting path, now we need to restore it
                        restore_augmenting_path(start, dest);
                        return;
                    }

                    color[dest] = 1; /// odd distanse
                    color[match[dest]] = 0; /// even distance
                    q.push(match[dest]); /// add the vertex with the even distance to the queue
                } else if (color[dest] == 0 && blossom_root[dest] != blossom_root[cur]) {
                    /// we ignore the case blossom_root[dest] == blossom_root[cur],
                    /// because it corresponds to the same vertex in the corresponding graph
                    const int root = blossoms_lca(blossom_root[dest], blossom_root[cur]);
                    reorder_parents_within_blossom(cur, dest, root, q);
                    reorder_parents_within_blossom(dest, cur, root, q);
                }
            }
        }
    }

    void max_matching () {
        for (int i = 0; i < n; i++) {
            if (match[i] == -1) {
                try_augment(i);
            }
        }
    }
};