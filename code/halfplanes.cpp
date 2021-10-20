#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;
using ld = long double;

using pnti = complex<int>;
using pntd = complex<ld>;


ll operator%(const pnti &a, const pnti &b)
{
	return a.real() * (ll) b.imag() - a.imag() * (ll) b.real();
}


ld operator%(const pntd &a, const pntd &b)
{
	return a.real() * b.imag() - a.imag() * b.real();
}


namespace std
{
	template<class T>
	bool operator<(const complex<T> &a, const complex<T> &b)
	{
		if (a.real() == b.real())
			return a.imag() < b.imag();
		return a.real() < b.real();
	}
}


pntd line_intersect(const pnti &a, const pnti &b, const pnti &c, const pnti &d)
{
	auto ab = a - b;
	auto cd = c - d;

	auto det = ab % cd;

	assert(det != 0);

	pntd ans{ld(a % b * cd.real() - c % d * ab.real()),
	         ld(a % b * cd.imag() - c % d * ab.imag())};

	return ans / (ld) det;
}


pntd conv(const pnti &x)
{
	return pntd{(ld) x.real(), (ld) x.imag()};
}


auto halfplane_intesection(const vector<pair<pnti, pnti>> &hp)
{
	vector<int> ind(hp.size());

	iota(ind.begin(), ind.end(), 0);

	auto type = [&hp](int j) -> bool
	{
		return hp[j].first < pnti{0, 0};
	};

	sort(ind.begin(), ind.end(), [&](int a, int b)
	{
		if (type(a) != type(b))
			return type(a) < type(b);
		return hp[a].first % hp[b].first > 0;
	});

	ind.resize(ind.size() * 2);
	copy(ind.begin(), ind.begin() + ind.size() / 2, ind.begin() + ind.size() / 2);

	auto parallel = [&hp](int a, int b)
	{
		return hp[a].first % hp[b].first == 0;
	};

	vector<int> ans;

	auto hp_intersect = [&hp](int a, int b)
	{
		return line_intersect(hp[a].second, hp[a].second + hp[a].first,
		                      hp[b].second, hp[b].second + hp[b].first);
	};

	for (auto nxt: ind)
	{
		while (!ans.empty())
		{
			auto lst = ans.back();

			if (parallel(lst, nxt))
			{
				if (hp[nxt].first % (hp[nxt].second - hp[lst].second) > 0)
					ans.pop_back();
				else
					break;
			}
			else if (ans.size() > 1)
			{
				auto prv = ans[ans.size() - 2];

				auto inter = hp_intersect(nxt, lst);

				if (conv(hp[prv].first) % inter <= ld(hp[prv].first % hp[prv].second))
					ans.pop_back();
				else
					break;
			}
			else
				break;
		}

		if (ans.empty() || !parallel(nxt, ans.back()))
			ans.push_back(nxt);
	}

	auto st = find_if(ans.begin(), ans.end(), [&type](int q)
	{
		return type(q);
	});

	if (st == ans.end())
		return vector<int>{};

	auto en = find(st + 1, ans.end(), *st);

	if (en == ans.end() || en - st < 3)
		return vector<int>{};

	return vector<int>(st, en);
}
