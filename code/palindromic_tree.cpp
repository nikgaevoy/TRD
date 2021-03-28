class treert
{
	struct node
	{
		array<int, 26> nxt;
		int par, link, siz;

		node(int siz, int par, int link) : par(par), link(link == -1 ? 1 : link), siz(siz)
		{
			fill(nxt.begin(), nxt.end(), -1);
		}
	};

	vector<node> mem;
	vector<int> suff; // longest palindromic suffix

public:
	treert(const string &str) : suff(str.size())
	{
		mem.emplace_back(-1, -1, 0);
		mem.emplace_back(0, 0, 0);
		mem[0].link = mem[1].link = 0;

		auto link_walk = [&](int st, int pos)
		{
			while (pos - 1 - mem[st].siz < 0 || str[pos] != str[pos - 1 - mem[st].siz])
				st = mem[st].link;

			return st;
		};

		for (int i = 0, last = 1; i < str.size(); i++)
		{
			last = link_walk(last, i);
			auto ind = str[i] - 'a';

			if (mem[last].nxt[ind] == -1)
			{
				// order is important
				mem.emplace_back(mem[last].siz + 2, last, mem[link_walk(mem[last].link, i)].nxt[ind]);
				mem[last].nxt[ind] = (int)mem.size() - 1;
			}

			last = mem[last].nxt[ind];

			suff[i] = last;
		}
	}
};
