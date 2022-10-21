template<class T, class Add>
class skew_heap
{
	struct node
	{
		using nodeptr = unique_ptr<node>;

		nodeptr l = nullptr, r = nullptr;
		T x;

		explicit node(T x = {}) : x(x)
		{}
	};

	using nodeptr = typename node::nodeptr;

	static nodeptr merge(nodeptr &&a, nodeptr &&b)
	{
		if (a == nullptr)
			return std::move(b);
		if (b == nullptr)
			return std::move(a);
		if (b->x < a->x)
			return merge(std::move(b), std::move(a));

		auto tmp = merge(std::move(a->r), std::move(b));
		a->r = std::move(a->l);
		a->l = std::move(tmp);

		return std::move(a);
	}

	void add_to_all(nodeptr &a, Add x)
	{
		if (a == nullptr)
			return;

		a->x += x;
		add_to_all(a->l, x);
		add_to_all(a->r, x);
	}

	nodeptr root = nullptr;
	size_t siz = 0;
	Add to_add{};

public:
	void add(Add x)
	{
		to_add += x;
	}

	[[nodiscard]] T top() const
	{
		return root->x + to_add;
	}

	[[nodiscard]] auto size() const
	{
		return siz;
	}

	[[nodiscard]] auto empty() const
	{
		return size() == 0;
	}

	void pop()
	{
		auto q = merge(std::move(root->l), std::move(root->r));
		siz--;
		root = std::move(q);
	}

	void merge(skew_heap &&rhs)
	{
		if (size() < rhs.size())
			swap(*this, rhs);

		siz += rhs.siz;
		rhs.siz = 0;
		rhs.add_to_all(rhs.root, rhs.to_add - to_add);
		auto q = merge(std::move(root), std::move(rhs.root));
		root = std::move(q);
	}

	void push(T x)
	{
		skew_heap sh;
		sh.root = make_unique<node>(x);
		sh.siz = 1;

		merge(std::move(sh));
	}
};

struct edge
{
	ll w;
	int to;

	strong_ordering operator<=>(const edge &rhs) const
	{
		return w <=> rhs.w;
	}

	edge &operator+=(ll rhs)
	{
		w += rhs;

		return *this;
	}

	edge operator+(ll rhs) const
	{
		return edge{w + rhs, to};
	}
};


enum color_t
{
	White = 0, Grey, Black, Cycle
};


ll solve(const vector<vector<pair<int, int>>> &gr, int root = 0)
{
	vector<skew_heap<edge, ll>> rev(gr.size());

	for (int v = 0; v < (int) gr.size(); v++)
		for (auto [u, w]: gr[v])
			if (u != root)
				rev[u].push(edge{w, v});

	auto mrg = [&](int a, int b)
	{
		rev[a].merge(std::move(rev[b]));
	};

	dsu cc(gr.size(), mrg);

	vector<color_t> color(gr.size());
	color[root] = Black;

	ll ans = 0;

	function<bool(int)> dfs = [&](int v) -> bool
	{
		v = cc.get_class(v);

		if (color[v] == Black)
			return false;

		if (color[v] == Grey)
		{
			color[v] = Cycle;

			return true;
		}
		color[v] = Grey;

		while (true)
		{
			while (!rev[v].empty() && cc.get_class(rev[v].top().to) == v)
				rev[v].pop();

			assert(!rev[v].empty()); // assume that the answer exists
			auto [w, to] = rev[v].top();

			ans += w;
			rev[v].add(-w);

			if (dfs(to))
			{
				if (color[v] != Cycle)
				{
					cc.unite(v, to);
					color[cc.get_class(v)] = Cycle;

					return true;
				}
				else
				{
					v = cc.get_class(v);

					color[v] = Grey;
				}
			}
			else
			{
				color[v] = Black;

				return false;
			}
		}
	};

	for (int i = 0; i < (int) gr.size(); i++)
		dfs(i);

	return ans;
}


void dfs(const vector<vector<pair<int, int>>> &gr, vector<bool> &used, int v)
{
	if (used[v])
		return;
	used[v] = true;

	for (auto [u, w]: gr[v])
		dfs(gr, used, u);
}


void solve(istream &cin = std::cin, ostream &cout = std::cout)
{
	int n, m;

	cin >> n >> m;

	vector<vector<pair<int, int>>> gr(n);

	for (int i = 0; i < m; i++)
	{
		int a, b, w;

		cin >> a >> b >> w;
		a--;
		b--;

		gr[a].emplace_back(b, w);
	}

	vector<bool> used(gr.size());

	dfs(gr, used, 0);

	if (ranges::count(used, false))
	{
		cout << "NO" << endl;

		return;
	}

	cout << "YES" << endl;

	cout << solve(gr) << endl;
}
