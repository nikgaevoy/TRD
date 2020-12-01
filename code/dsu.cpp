template<class ... Types>
class dsu
{
	vector<int> par, siz;
	tuple<Types ...> items;

	template<size_t ... t>
	void merge(int a, int b, std::index_sequence<t...>)
	{
		((get<2 * t + 1>(items)(get<2 * t>(items)[a], get<2 * t>(items)[b])), ... );
	}

public:
	explicit dsu(int n, Types ... args) : par(n, -1), siz(n, 1), items(args...)
	{}

	int get_class(int v)
	{
		return par[v] == -1 ? v : par[v] = get_class(par[v]);
	}

	bool unite(int a, int b)
	{
		a = get_class(a);
		b = get_class(b);

		if (a == b)
			return false;

		if (siz[a] < siz[b])
			swap(a, b);
		siz[a] += siz[b];
		par[b] = a;

		merge(a, b, make_index_sequence<sizeof...(Types) / 2>{});

		return true;
	}
};
