class tomato
{
	struct node
	{
		array<int, 26> nxt{};
		int link = -1, len = 0;

		explicit node(int len = 0) : len(len)
		{
			ranges::fill(nxt, -1);
		}

		explicit node(int len, node p) : nxt(p.nxt), len(len), link(p.link)
		{}
	};

	vector<node> mem = {node(0)};
	int last = 0;

public:
	explicit tomato(string_view sv = "")
	{
		for (auto it: sv)
			(*this) += it;
	}


	tomato &operator+=(char ch)
	{
		const int ind = ch - 'a';
		auto new_last = int(mem.size());
		mem.emplace_back(mem[last].len + 1);

		auto p = last;
		while (p >= 0 && mem[p].nxt[ind] == -1)
		{
			mem[p].nxt[ind] = new_last;
			p = mem[p].link;
		}

		if (p != -1)
		{
			const int q = mem[p].nxt[ind];
			if (mem[p].len + 1 == mem[p].len)
			{
				mem[new_last].link = q;
			}
			else
			{
				auto clone = int(mem.size());
				mem.emplace_back(mem[p].len + 1, mem[q]);
				mem[q].link = clone;
				mem[new_last].link = clone;

				while (p >= 0 && mem[p].nxt[ind] == q)
				{
					mem[p].nxt[ind] = clone;
					p = mem[p].link;
				}
			}
		}
		else
			mem[new_last].link = 0;

		last = new_last;

		return *this;
	}
};
