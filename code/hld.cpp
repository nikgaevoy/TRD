class hld
{
	struct node
	{
		int p = -1, l = 0, sz = 1;
	};

	vector<node> gr;
	vector<int> wh;
	segtree st;


	int build(const vector<vector<int>> &tr, int &ind, int v, int p = -1, int l = 0)
	{
		int me = ind;
		ind++;

		wh[v] = me;

		gr[me].p = p;
		gr[me].l = l;

		auto rec = [&](int u, int nl)
		{
			build(tr, ind, u, me, nl);
		};

		if (!tr[v].empty())
		{
			rec(tr[v].front(), l);

			for (auto it: tr[v] | views::drop(1))
				rec(it, ind);
		}

		return me;
	}

public:
	explicit hld(const vector<vector<int>> &tr = {}) : gr(tr.size()), wh(gr.size(), -1), st(gr.size())
	{
		int q = 0;

		build(tr, q, 0);

		assert(q == tr.size());
		assert(ranges::count(wh, -1) == 0);

		for (const auto &it: gr | views::drop(1) | views::reverse)
			gr[it.p].sz += it.sz;
	}
};
