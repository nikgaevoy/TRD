// here we use T = __int128 to store the independent set
// calling expand k times to an empty set finds the maximum
// cost of the set with size exactly k,
// that is independent in blue and red matroids
// ver is the number of the elements in the matroid,
// e[i].w is the cost of the i-th element
// first return value is new independent set
// second return value is difference between
// new and old costs
// oracle(set, red) and oracle(set, blue) check whether
// or not the set lies in red or blue matroid respectively
auto expand = [&](T in) -> T
{
    vector<int> ids;
    for (int i = 0; i < int(es.size()); i++)
        if (in[i])
            ids.push_back(i);

    vector<int> from, to;
    /// Given a set that is independent in both matroids, answers
    /// queries "If we add i-th element to the set, will it still be
    /// independent in red/blue matroid?". Usually can be done quickly.
    can_extend full_can(ids, n, es);

    for (int i = 0; i < int(es.size()); i++)
        if (!in[i])
        {
            auto new_ids = ids;
            new_ids.push_back(i);

            auto is_red = full_can.extend_red(i, es);
            auto is_blue = full_can.extend_blue(i, es);

            if (is_blue)
                from.push_back(i);
            if (is_red)
                to.push_back(i);

            if (is_red && is_blue)
            {
                T swp_mask = in;
                swp_mask.flip(i);
                return swp_mask;
            }
    }

    vector<vector<int>> g(es.size());
    for (int j = 0; j < int(es.size()); j++)
        if (in[j])
        {
            auto new_ids = ids;
            auto p = find(new_ids.begin(), new_ids.end(), j);
            assert(p != new_ids.end());
            new_ids.erase(p);

            can_extend cur(new_ids, n, es);

            for (int i = 0; i < int(es.size()); i++)
                if (!in[i])
                {
                    if (cur.extend_red(i, es))
                        g[i].push_back(j);
                    if (cur.extend_blue(i, es))
                        g[j].push_back(i);
                }
        }

    auto get_cost = [&] (int x)
    {
        const int cost = (!in[x] ? e[x].w : -e[x].w);
        return (ver + 1) * cost - 1;
    };

    const int inf = int(1e9);
    vector<int> dist(ver, -inf), prev(ver, -1);
    for (int x : from)
        dist[x] = get_cost(x);

    queue<int> q;

    vector<int> used(ver);
    for (int x : from)
    {
        q.push(x);
        used[x] = 1;
    }

    while (!q.empty())
    {
        int cur = q.front(); used[cur] = 0; q.pop();

        for (int to : g[cur])
        {
            int cost = get_cost(to);
            if (dist[to] < dist[cur] + cost)
            {
                dist[to] = dist[cur] + cost;
                prev[to] = cur;
                if (!used[to])
                {
                    used[to] = 1;
                    q.push(to);
                }
            }
        }
    }

    int best = -inf, where = -1;
    for (int x : to)
    {
        if (dist[x] > best)
        {
            best = dist[x];
            where = x;
        }
    }

    if (best == -inf)
        return pair<T, int>(cur_set, best);

    while (where != -1)
    {
        cur_set ^= (T(1) << where);
        where = prev[where];
    }

    while (best % (ver + 1))
        best++;
    best /= (ver + 1);

    assert(oracle(cur_set, red) && oracle(cur_set, blue));
    return pair<T, int>(cur_set, best);
};
