vector<int> suffix_array(string_view str)
{
	vector<int> p(str.size());

	iota(p.begin(), p.end(), 0);
	vector<ll> group(p.size());

	for (int i = 0; i < ssize(group); i++)
		group[i] = ll(str[i]);

	auto cmp = [&](int a, int b)
	{
		return group[a] <=> group[b];
	};

	auto cmp_bool = [&](int a, int b)
	{
		return cmp(a, b) == strong_ordering::less;
	};

	ranges::sort(p, cmp_bool);

	vector<pair<int, int>> blocks;

	auto gen_blocks = [&]()
	{
		blocks.clear();

		for (int l = 0, r; l < (int) str.size(); l = r)
		{
			for (r = l; r < (int) str.size() && cmp(p[l], p[r]) == strong_ordering::equal; r++);

			for (int i = l; i < r; i++)
				group[p[i]] = (int) blocks.size();

			blocks.emplace_back(l, r);
		}
	};

	gen_blocks();

	for (auto len = 1; len < (int) str.size(); len *= 2)
	{
		{
			vector<ll> tmp(group.size());

			for (auto i: ranges::iota_view(0, ssize(tmp)))
				tmp[i] = group[i] * ssize(blocks) + group[(i + len) % str.size()];

			group = std::move(tmp);
		}

		for (auto [l, r]: blocks)
			sort(p.begin() + l, p.begin() + r, cmp_bool);

		gen_blocks();
	}

	return p;
}


vector<int> kasai_lcp(const vector<int> &sa, string_view sv)
{
	vector<int> lcp(sa.size() - 1), pos(sa.size());

	for (int i = 0; i < ssize(sa); i++)
		pos[sa[i]] = i;

	int ans = 0;

	for (auto p: pos)
	{
		if (p != lcp.size())
		{
			auto i = sa[p];
			auto j = sa[p + 1];

			while (i + ans < ssize(sv) && j + ans < ssize(sv) && sv[i + ans] == sv[j + ans])
				ans++;

			lcp[p] = ans;
		}

		ans = max(0, ans - 1);
	}

	return lcp;
}
