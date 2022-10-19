class lct
{
	struct node
	{
		using nodeptr = node *;

		array<nodeptr, 2> ch{};
		nodeptr par = nullptr;
		size_t siz = 1;
		bool rev = false;
	};

	using nodeptr = node::nodeptr;

	static void reverse(const nodeptr &h)
	{
		if (h != nullptr)
			h->rev = !h->rev;
	}

	static void push(node &h)
	{
		if (h.rev)
		{
			swap(h.ch.front(), h.ch.back());
			h.rev = false;

			for (auto it: h.ch)
				reverse(it);
		}
	}

	static auto size(const nodeptr &h)
	{
		return h == nullptr ? 0 : h->siz;
	}

	static void upd(node &h)
	{
		h.siz = 1;

		for (auto it: h.ch)
		{
			h.siz += size(it);

			if (it != nullptr)
				it->par = &h;
		}
	}

	static bool is_root(const node &h)
	{
		return h.par == nullptr || find(h.par->ch.begin(), h.par->ch.end(), &h) == h.par->ch.end();
	}

	static bool is_right(const node &h)
	{
		assert(!is_root(h));
		push(*h.par);
		return get<1>(h.par->ch) == &h;
	}

	static void zig(node &h)
	{
		assert(!is_root(h));

		auto &p = *h.par;
		push(p);
		push(h);
		auto pp = p.par;
		bool ind = is_right(h);
		auto &x = p.ch[ind];
		auto &b = h.ch[!ind];

		x = b;
		b = &p;
		h.par = pp;

		upd(p);
		upd(h);

		if (pp != nullptr)
			for (auto &it: pp->ch)
				if (it == &p)
					it = &h;
	}

	static void splay(node &h)
	{
		push(h);
		while (!is_root(h))
		{
			auto &p = *h.par;

			if (is_root(p))
			{
				zig(h);
			}
			else if (is_right(h) == is_right(p))
			{
				zig(p);
				zig(h);
			}
			else
			{
				zig(h);
				zig(h);
			}
		}
	}

	static void expose(node &h)
	{
		splay(h);

		while (h.par != nullptr)
		{
			auto &p = *h.par;
			splay(p);
			get<1>(p.ch) = &h;
			upd(p);
			splay(h);
		}
	}

	static node &lca(node &a, node &b)
	{
		expose(a);
		expose(b);
		splay(a);

		if (a.par != nullptr)
			return *a.par;

		auto sz_a = size(get<0>(a.ch));
		splay(b);
		auto sz_b = size(get<0>(b.ch));

		return sz_a < sz_b ? a : b;
	}

	template<bool right>
	static node &find_extreme(node &h)
	{
		push(h);
		return get<right>(h.ch) == nullptr ? h : find_extreme<right>(*get<right>(h.ch));
	}

	static void cut_suffix(node &a)
	{
		splay(a);
		get<1>(a.ch) = nullptr;
		upd(a);
	}

	static void cut_prefix(node &a)
	{
		splay(a);
		if (get<0>(a.ch) != nullptr)
			cut_suffix(find_extreme<true>(*get<0>(a.ch)));
	}

	static node &find_root(node &a)
	{
		expose(a);
		auto &q = find_extreme<false>(a);
		splay(q);
		return q;
	}

	static void reroot(node &a)
	{
		expose(a);
		cut_suffix(a);
		splay(a);
		reverse(&a);
		splay(a);
	}

	unique_ptr<node[]> mem;

public:
	explicit lct(size_t n = 0) : mem(make_unique<node[]>(n))
	{}

	void link(int a, int b)
	{
		expose(mem[a]);
		mem[a].par = &mem[b];
	}

	void cut(int b)
	{
		expose(mem[b]);
		cut_prefix(mem[b]);
		splay(mem[b]);
		mem[b].par = nullptr;
	}

	auto lca(int a, int b)
	{
		return &lca(mem[a], mem[b]) - mem.get();
	}
};
