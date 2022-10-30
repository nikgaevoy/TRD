int StoerWagner(vector<vector<int>> matr)
{
	int ans = numeric_limits<int>::max();

	auto work = [&]() -> pair<int, int>
	{
		vector<int> d(matr.size());

		int q;

		for (int i = 0; i + 1 < int(matr.size()); i++)
		{
			q = int(max_element(d.begin(), d.end()) - d.begin());
			d[q] = numeric_limits<int>::lowest();

			for (int j = 0; j < int(matr.size()); j++)
				d[j] += matr[q][j];
		}

		auto w = int(max_element(d.begin(), d.end()) - d.begin());

		ans = min(ans, d[w]);

		return {q, w};
	};

	while (matr.size() > 1)
	{
		int a, b;

		tie(a, b) = work();

		if (b < a)
			swap(a, b);

		for (int i = 0; i < int(matr.size()); i++)
			if (i != a && i != b)
			{
				matr[i][a] += matr[i][b];
				matr[a][i] += matr[b][i];
			}

		for (auto &row: matr)
			row.erase(row.begin() + b);
		matr.erase(matr.begin() + b);
	}

	return ans;
}
