ld Hungarian(const vector<vector<ld>> &matr)
{
	vector<int> lb(matr.size(), -1), rb(matr[0].size(), -1);
	vector<ld> rows(matr.size()), cols(rb.size());

	for (int v = 0; v < ssize(matr); v++)
	{
		vector<bool> lused(lb.size()), rused(rb.size());
		vector<int> par(rb.size(), -1);
		vector<pair<ld, int>> w(rb.size(), {numeric_limits<ld>::max(), -1});

		auto add_row = [&](int i)
		{
			lused[i] = true;

			for (int j = 0; j < ssize(w); j++)
				remin(w[j], {matr[i][j] + rows[i] + cols[j], i});
		};

		add_row(v);

		while (true)
		{
			int j = -1;

			for (int k = 0; k < ssize(rb); k++)
				if (!rused[k] && (j == -1 || w[k] < w[j]))
					j = k;

			auto [x, i] = w[j];

			for (int k = 0; k < ssize(lused); k++)
				if (!lused[k])
					rows[k] += x;
			for (int k = 0; k < ssize(rused); k++)
				if (!rused[k])
				{
					cols[k] -= x;
					w[k].first -= x;
				}

			par[j] = i;
			rused[j] = true;

			if (rb[j] == -1)
			{
				while (j != -1)
				{
					rb[j] = par[j];
					auto nxt = lb[par[j]];
					lb[par[j]] = j;
					j = nxt;
				}

				break;
			}

			add_row(rb[j]);
		}
	}

	ld ans = 0;

	for (int i = 0; i < ssize(lb); i++)
		if (auto j = lb[i]; j != -1)
			ans += matr[i][j];

	return ans;
}
