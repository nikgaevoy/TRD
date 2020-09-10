#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

mt19937 mt(736);


auto foo(uintptr_t x)
{
	static uintptr_t add = uniform_int_distribution<uintptr_t>{}(mt);

	return x ^ add;
}


template<class T>
class treap
{
	struct node
	{
		using nodeptr = node *;

		T x, mn;
		nodeptr l, r;
		size_t siz;

		explicit node(const T &x, nodeptr l = nullptr, nodeptr r = nullptr) : x(x), l(l), r(r), mn(x), siz(1)
		{
			upd(this);
		}

		~node()
		{
			delete l;
			delete r;
		}
	};

public:
	using nodeptr = typename node::nodeptr;

private:
	static void upd(nodeptr h)
	{
		if (h == nullptr)
			return;

		h->mn = h->x;
		h->siz = get_siz(h->l) + 1 + get_siz(h->r);

		if (h->l != nullptr)
			h->mn = min(h->mn, h->l->mn);
		if (h->r != nullptr)
			h->mn = min(h->mn, h->r->mn);
	}

	static size_t get_siz(nodeptr h)
	{
		return h == nullptr ? 0 : h->siz;
	}

	static nodeptr merge(nodeptr le, nodeptr ri)
	{
		if (le == nullptr)
			return ri;
		if (ri == nullptr)
			return le;

		if (foo((uintptr_t) le) < foo((uintptr_t) ri))
		{
			le->r = merge(le->r, ri);

			upd(le);

			return le;
		}
		else
		{
			ri->l = merge(le, ri->l);

			upd(ri);

			return ri;
		}
	}


	static nodeptr erase_min(nodeptr h)
	{
		assert(h != nullptr);

		if (h->x == h->mn)
		{
			auto ret = merge(h->l, h->r);

			h->l = h->r = nullptr;

			delete h;

			return ret;
		}

		if (h->l != nullptr && h->l->mn == h->mn)
			h->l = erase_min(h->l);
		else
			h->r = erase_min(h->r);

		upd(h);

		return h;
	}

	nodeptr root = nullptr;

	void apply_to_all(const std::function<void(T &)> &q, nodeptr h)
	{
		if (h == nullptr)
			return;

		apply_to_all(q, h->l);
		apply_to_all(q, h->r);
		q(h->x);

		upd(h);
	}


public:
	treap() = default;

	treap(treap<T> &a, treap<T> &b) : root(merge(a.root, b.root))
	{
		a.root = nullptr;
		b.root = nullptr;
	}

	~treap()
	{
		delete root;
	}

	void swap(treap &b)
	{
		::swap(root, b.root);
	}

	void insert(const T &x)
	{
		root = merge(root, new node(x));
	}


	[[nodiscard]] T top() const
	{
		assert(root != nullptr);

		return root->mn;
	}


	void pop()
	{
		assert(root != nullptr);

		root = erase_min(root);
	}


	void apply_to_all(const std::function<void(T &)> &q)
	{
		apply_to_all(q, root);
	}

	[[nodiscard]] size_t size() const
	{
		return get_siz(root);
	}
};


class dsu
{
	vector<int> par, siz;

public:
	explicit dsu(int n) : par(n, -1), siz(n, 1)
	{}

	int get_par(int v)
	{
		return par[v] == -1 ? v : par[v] = get_par(par[v]);
	}

	bool unite(int a, int b)
	{
		a = get_par(a);
		b = get_par(b);

		if (a == b)
			return false;

		if (siz[a] < siz[b])
			swap(a, b);

		siz[a] += siz[b];
		par[b] = a;

		return true;
	}
};


void dfs(const vector<vector<pair<int, int>>> &gr, vector<bool> &used, int v)
{
	if (used[v])
		return;

	used[v] = true;

	for (auto it : gr[v])
		dfs(gr, used, it.first);
}


void solve(istream &cin = std::cin, ostream &cout = std::cout)
{
	ll ans = 0;

	int n, m, root = 0;

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

	vector<bool> used(n);

	dfs(gr, used, root);

	if (count(used.begin(), used.end(), false))
	{
		cout << "NO" << endl;

		return;
	}

	cout << "YES" << endl;

	vector<treap<pair<ll, int>>> rev(n);

	for (int v = 0; v < (int) gr.size(); v++)
		for (auto it : gr[v])
			if (it.first != root)
				rev[it.first].insert(pair{(ll) it.second, v});

	fill(used.begin(), used.end(), false);
	auto grey = used;
	used.front() = true;

	dsu group(n);

	vector<ll> add(n);

	auto merge_vert = [&](int a, int b, int to)
	{
		if (rev[a].size() < rev[b].size())
			swap(a, b);

		auto dif = add[b] - add[a];
		add[b] = add[a];

		rev[b].apply_to_all([dif](auto &x)
		                    {
			                    x.first += dif;
		                    });

		treap tmp(rev[a], rev[b]);

		rev[to].swap(tmp);
	};

	// returns whether we are on cycle
	std::function<bool(int)> ans_vert = [&](int v) -> bool
	{
		v = group.get_par(v);

		if (used[v])
			return false;

		if (grey[v])
		{
			grey[v] = false;

			return true;
		}
		grey[v] = true;

		while (true)
		{
			auto nxt = rev[v].top();

			while (group.get_par(nxt.second) == v)
			{
				rev[v].pop();
				nxt = rev[v].top();
			}

			ans += nxt.first + add[v];
			add[v] -= nxt.first + add[v];

			auto type = ans_vert(nxt.second);

			if (type)
			{
				if (grey[v])
				{
					nxt.second = group.get_par(nxt.second);

					group.unite(v, nxt.second);

					merge_vert(v, nxt.second, group.get_par(v));

					v = group.get_par(v);

					grey[v] = false;

					return true;
				}
				else
				{
					v = group.get_par(v);

					grey[v] = true;
				}
			}
			else
			{
				ans += nxt.first + add[v];
				add[v] -= nxt.first + add[v];
				used[v] = true;
				grey[v] = false;

				return false;
			}
		}
	};

	for (int i = 0; i < gr.size(); i++)
		if (group.get_par(i) == i)
			ans_vert(i);

	for (int v = 0; v < gr.size(); v++)
		assert(used[v] == (group.get_par(v) == v));

	cout << (long long) ans << endl;
}


int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	cout << fixed;

#ifdef LOCAL
	auto st = clock();

	ifstream fin("../input.txt");

	do
	{
		solve(fin);

		cout << "===" << endl;

		string str;
		while (getline(fin, str) && str != string(max(1, (int) str.size()), '='));
	} while (fin);

	cout << setprecision(6) << "clock: " << double(clock() - st) / CLOCKS_PER_SEC << endl;
#else
	solve();
#endif

	return 0;
}
