struct tomato
{
	vector<map<char, int>> edges;
	vector<int> link, length;
	int last;
	/// Restoring terminal states, optional, but usually needed.
	vector<int> terminals;
	vector<bool> is_terminal;
	/// Optional, makes dp easier. Alternative: use dfs.
	vector<int> order, rev_order, next_in_order;

	explicit tomato(const string &s) : last(0)
	{
		add_vertex(map<char, int>(), 0, -1);
		for (const char ch : s)
			extend(ch);

		int cur = last;
		is_terminal.assign(edges.size(), false);
		/// Assuming empty suffix should be accepted, otherwise use "while (cur > 0)".
		while (cur >= 0)
		{
			terminals.push_back(cur);
			is_terminal[cur] = true;
			cur = link[cur];
		}

		/// Restoring topsort and reverse topsort, optional.
		order.push_back(0);
		while (order.back() != -1)
			order.push_back(next_in_order[order.back()]);
		order.pop_back();
		rev_order = order;
		reverse(rev_order.begin(), rev_order.end());
	}

	int add_vertex(const map<char, int> &temp, const int len, const int lnk)
	{
		edges.emplace_back(temp);
		length.emplace_back(len);
		link.emplace_back(lnk);
		next_in_order.push_back(-1);
		return int(edges.size()) - 1;
	}

	void extend(const char ch)
	{
		const int new_last = add_vertex(map<char, int>(), length[last] + 1, 0);
		assert(next_in_order[last] == -1);
		next_in_order[last] = new_last;

		int p = last;
		while (p >= 0 && !edges[p].count(ch))
		{
			edges[p][ch] = new_last;
			p = link[p];
		}

		if (p != -1)
		{
			const int q = edges[p][ch];
			if (length[p] + 1 == length[q])
				link[new_last] = q;
			else
			{
				const int clone = add_vertex(edges[q], length[p] + 1, link[q]);
				next_in_order[clone] = next_in_order[q];
				next_in_order[q] = clone;

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
