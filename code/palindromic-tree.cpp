auto pal_tree(const string &str)
{
	const int rooteven = str.size();
	const int rootodd = str.size() + 1;

	vector<int> siz(str.size() + 2, 1), link(str.size() + 2, rooteven), type(str.size());

	vector<vector<int>> mem(str.size() + 2, vector<int>(26, -1));
	siz[rooteven] = 0;
	siz[rootodd] = -1;
	link[rooteven] = rootodd;
	link[rootodd] = rootodd;

	for (int i = 0; i < str.size(); i++)
	{
		auto it = str[i] - 'a';

		auto get = [&](int p) {
			while (!(i - siz[p] - 1 >= 0 && str[i - siz[p] - 1] == str[i]))
				p = link[p];

			return p;
		};

		auto prev = get(i == 0 ? rootodd : type[i - 1]);

		if (mem[prev][it] == -1)
		{
			mem[prev][it] = i;
			siz[i] = siz[prev] + 2;

			auto l = get(link[prev]);

			link[i] = mem[l][it];
			if (link[i] == i)
				link[i] = rooteven;
		}

		type[i] = mem[prev][it];
	}

	return make_tuple(type, link, siz, mem);
}
