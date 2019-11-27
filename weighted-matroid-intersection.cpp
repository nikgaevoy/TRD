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

auto expand = [&] (T cur_set) -> pair<T, int>
{
    vector<int> in(ver);
    for (int i = 0; i < ver; i++)
        in[i] = ((cur_set >> i) & 1);

    const int red = 1;
    const int blue = 2;

    vector<vector<int>> g(ver);
    for (int i = 0; i < ver; i++)
    for (int j = 0; j < ver; j++)
    {
        T swp_mask = (cur_set ^ (T(1) << i) ^ (T(1) << j));
        if (!in[i] && in[j])
        {
            if (oracle(swp_mask, red))
                g[i].push_back(j);
            if (oracle(swp_mask, blue))
                g[j].push_back(i);
        }
    }

    vector<int> from, to;
    for (int i = 0; i < ver; i++) if (!in[i])
    {
        T add_mask = cur_set ^ (T(1) << i);
        if (oracle(add_mask, blue))
            from.push_back(i);
        if (oracle(add_mask, red))
            to.push_back(i);
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
