class hld
{
	struct node
	{
		int p = -1, l = 0, sz = 1;
	};

	vector<node> mem;
	vector<int> wh;
	segtree st;


	int build(const vector<vector<int>> &gr, int &ind, int v, int p = -1, int l = 0)
	{
		int me = ind;
		ind++;

		wh[v] = me;

		mem[me].p = p;
		mem[me].l = l;

		auto rec = [&](int u, int nl)
		{
			build(gr, ind, u, me, nl);
		};

		if (!gr[v].empty())
		{
			rec(gr[v].front(), l);

			for (auto it: gr[v] | views::drop(1))
				rec(it, ind);
		}

		return me;
	}

public:
	explicit hld(const vector<vector<int>> &gr = {}) : mem(gr.size()), wh(gr.size(), -1), st(gr.size())
	{
		int q = 0;

		build(gr, q, 0);

		assert(q == ssize(gr));
		assert(ranges::count(wh, -1) == 0);

		for (const auto &it: mem | views::drop(1) | views::reverse)
			mem[it.p].sz += it.sz;
	}
};
