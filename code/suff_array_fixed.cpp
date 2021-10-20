/// Actually sorts suffixes now.
vector<int> suff_array(string s)
{
	/// Add something that is less than all of symbols from s
	s += 'a' - 1;
	/// Only now we can compute the length of the string.
	const int n = int(s.size());
	vector<int> p(n);

	const int bound = 260;
	vector<int> c(max(n, bound));
	auto cnt = c, np = c, nc = c;

	for (char ch : s)
		cnt[ch]++;

	for (int i = 1; i < 256; i++)
		cnt[i] += cnt[i - 1];
	for (int i = 0; i < n; i++)
		p[--cnt[s[i]]] = i;

	int cls = 1;
	c[p[0]] = cls - 1;

	for (int i = 1; i < n; i++)
	{
		if (s[p[i]] != s[p[i - 1]])
			++cls;
		c[p[i]] = cls - 1;
	}

	for (int len = 1; len <= n; len *= 2)
	{
		fill(cnt.begin(), cnt.begin() + cls, 0);
		for (int i = 0; i < n; i++)
			cnt[c[i]]++;

		for (int i = 0; i < cls - 1; i++)
			cnt[i + 1] += cnt[i];


		for (int i = n - 1; i >= 0; i--)
		{
			const int j = p[i];
			int j2 = (j - len + n) % n;
			np[--cnt[c[j2]]] = j2;
		}
		copy(np.begin(), np.begin() + n, p.begin());

		cls = 1;
		nc[p[0]] = cls - 1;
		for (int i = 1; i < n; i++)
		{
			if (c[p[i]] != c[p[i - 1]] || c[(p[i] + len) % n] != c[(p[i - 1] + len) % n])
				++cls;
			nc[p[i]] = cls - 1;
		}

		copy(nc.begin(), nc.begin() + n, c.begin());
	}

	/// Removing the extra symbol.
	assert(p.front() == int(p.size()) - 1);
	p.erase(p.begin());
	return p;
}

vector<int> kasai_lcp(const vector<int> &p, const string &s)
{
	const int n = int(p.size());
	vector<int> lcp(n - 1);

	vector<int> pos(n);
	for (int i = 0; i < n; i++)
		pos[p[i]] = i;

	int pref = 0;
	for (int i = 0; i < n; i++)
	{
		const int pi = pos[i];
		if (pi == n - 1)
			continue;

		const int j = p[pi + 1];
		while (i + pref < n && j + pref < n && s[i + pref] == s[j + pref])
			++pref;

		lcp[pi] = pref;
		pref = max(0, pref - 1);
	}

	return lcp;
}