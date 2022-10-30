vector<int> suffix_array(string_view str)
{
	vector<int> p(str.size());

	iota(p.begin(), p.end(), 0);
	vector<pair<int, int>> group(p.size());

	for (int i = 0; i < ssize(group); i++)
		group[i] = {int(str[i]), -1};

	auto compress = [&](int len)
	{
		for (int l = 0, r, val = 0; l < ssize(p); val++, l = r)
		{
			for (r = l; r < ssize(p) && group[p[l]] == group[p[r]]; r++);

			for (int i = l; i < r; i++)
				group[p[i]].first = val;
		}

		for (auto i: ranges::iota_view(0, ssize(group)))
			group[i].second = group[(i + len) % str.size()].first;
	};

	auto cmp = [&](int a, int b)
	{
		return group[a] < group[b];
	};

	ranges::sort(p, cmp);

	for (auto len = 1; len < (int) str.size(); len *= 2)
	{
		compress(len);

		for (int l = 0, r, val = 0; l < ssize(p); val++, l = r)
		{
			for (r = l; r < ssize(p) && group[p[l]].first == group[p[r]].first; r++);

			sort(p.begin() + l, p.begin() + r, cmp);
		}
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
