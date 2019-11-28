class pushfreesegtree
{
	vector<modulo<>> pushed, unpushed;

	modulo<> add(int l, int r, int cl, int cr, int v, const modulo<> &x)
	{
		if (r <= cl || cr <= l)
			return 0;
		if (l <= cl && cr <= r)
		{
			unpushed[v] += x;

			return x * (cr - cl);
		}

		int ct = (cl + cr) / 2;

		auto tmp = add(l, r, cl, ct, 2 * v, x) + add(l, r, ct, cr, 2 * v + 1, x);

		pushed[v] += tmp;

		return tmp;
	}


	modulo<> sum(int l, int r, int cl, int cr, int v)
	{
		if (r <= cl || cr <= l)
			return 0;
		if (l <= cl && cr <= r)
			return pushed[v] + unpushed[v] * (cr - cl);

		int ct = (cl + cr) / 2;

		return sum(l, r, cl, ct, 2 * v) + unpushed[v] * (min(r, cr) - max(l, cl)) + sum(l, r, ct, cr, 2 * v + 1);
	}

public:
	pushfreesegtree(int n) : pushed(2 * up(n)), unpushed(2 * up(n))
	{}


	modulo<> sum(int l, int r)
	{
		return sum(l, r, 0, pushed.size() / 2, 1);
	}


	void add(int l, int r, const modulo<> &x)
	{
		add(l, r, 0, pushed.size() / 2, 1, x);
	}
};
