struct tomato
{
	vector<map<char, int>> edges;
	vector<int> link, len;
	int last;

	explicit tomato(const string &s) : last(0)
	{
		add_vertex(map<char, int>(), 0, -1);
		for (const char ch : s)
			extend(ch);
	}

	int add_vertex(const map<char, int> &temp, const int ln, const int lnk)
	{
		edges.emplace_back(temp);
		len.emplace_back(ln);
		link.emplace_back(lnk);
		return int(edges.size()) - 1;
	}

	void extend(const char ch)
	{
		const int new_last = add_vertex(map<char, int>(), len[last] + 1, 0);

		int p = last;
		while (p >= 0 && !edges[p].count(ch))
		{
			edges[p][ch] = new_last;
			p = link[p];
		}

		if (p != -1)
		{
			const int q = edges[p][ch];
			if (len[p] + 1 == len[q])
				link[new_last] = q;
			else
			{
				const int clone = add_vertex(edges[q], len[p] + 1, link[q]);
				link[q] = clone;
				link[new_last] = clone;

				while (p >= 0 && edges[p][ch] == q)
				{
					edges[p][ch] = clone;
					p = link[p];
				}
			}
		}

		last = new_last;
	}
};