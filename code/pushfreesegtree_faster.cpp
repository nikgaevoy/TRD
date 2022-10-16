#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

mt19937 mt(736);


template<class Val, class Change, Change one = Change{}>
class pushfreesegtree
{
	vector<pair<Val, Change>> arr;

	void upd(size_t v)
	{
		arr[v].first = (arr[2 * v].first + arr[2 * v + 1].first) * arr[v].second;
	}

public:
	explicit pushfreesegtree(size_t n = 0) : arr(2 * n + 2, {Val{}, one})
	{}

	template<class It>
	explicit pushfreesegtree(It be, It en) : arr(2 * distance(be, en) + 2, {Val{}, one})
	{
		transform(be, en, arr.begin() + ssize(arr) / 2, [](auto x)
		{
			return pair{Val{x}, one};
		});

		for (int i = ssize(arr) / 2 - 1; i > 0; i--)
			upd(i);
	}

	auto segmult(const Change &x, size_t l, size_t r)
	{
		l += arr.size() / 2;
		r += arr.size() / 2;

		while (true)
		{
			if (l < r)
			{
				if (l & 1u)
				{
					arr[l].first *= x;
					arr[l].second *= x;
				}
				if (r & 1u)
				{
					arr[r - 1].first *= x;
					arr[r - 1].second *= x;
				}
			}

			l = (l + 1) / 2;
			r /= 2;

			if (r == 0)
				break;

			upd(l - 1);
			upd(r);
		}
	}

	[[nodiscard]] Val segsum(size_t l, size_t r) const
	{
		l += arr.size() / 2;
		r += arr.size() / 2;

		Val ansl{}, ansr{};

		while (true)
		{
			if (l < r)
			{
				if (l & 1u)
					ansl = ansl + arr[l].first;
				if (r & 1u)
					ansr = arr[r - 1].first + ansr;
			}

			l = (l + 1) / 2;
			r /= 2;

			if (r == 0)
				break;

			ansl *= arr[l - 1].second;
			ansr *= arr[r].second;
		}

		return ansl + ansr;
	}
};


template<class Val, class Change>
class naive
{
	vector<Val> arr;

public:
	explicit naive(size_t n = 0) : arr(n)
	{}

	template<class It>
	explicit naive(It be, It en) : arr(be, en)
	{
		transform(be, en, arr.begin(), [](auto x)
		{
			return Val{x};
		});
	}

	void segmult(const Change &x, size_t l, size_t r)
	{
		for (auto i = l; i < r; i++)
			arr[i] *= x;
	}

	[[nodiscard]] Val segsum(size_t l, size_t r) const
	{
		Val ans{};

		for (auto i = l; i < r; i++)
			ans += arr[i];

		return ans;
	}
};

struct xor_int
{
	unsigned x;

	constexpr explicit xor_int(unsigned x = 0) : x(x)
	{}

	xor_int operator*(xor_int rhs) const
	{
		return xor_int{x ^ rhs.x};
	}

	xor_int &operator*=(xor_int rhs)
	{
		x ^= rhs.x;

		return *this;
	}
};

string &operator*=(string &str, xor_int x)
{
	for (auto &it: str)
		it ^= x.x;

	return str;
}

string operator*(string str, xor_int x)
{
	str *= x;

	return str;
}

template<template<class Val, class Change> class ds = pushfreesegtree>
void solve(istream &cin = std::cin, ostream &cout = std::cout)
{
	int n, q;

	cin >> n >> q;

	vector<string> arr(n);

	for (auto &it: arr)
		cin >> it;

	ds<string, xor_int> st(arr.begin(), arr.end());

	for (int i = 0; i < q; i++)
	{
		int l, r;
		unsigned x;

		cin >> l >> r >> x;
		l--;

		st.segmult(xor_int(x), l, r);

		cout << st.segsum(l, r) << endl;
	}
}


void gen(ostream &cout = std::cout)
{
	uniform_int_distribution<int> len(1, 10);

	const int n = len(mt), q = 10;

	uniform_int_distribution<char> ch('a', 'z');
	uniform_int_distribution<int> uid(1, n), x(0, 10);

	cout << n << ' ' << q << endl;

	for (int i = 0; i < n; i++)
		cout << ch(mt) << ' ';
	cout << endl;

	for (int i = 0; i < q; i++)
	{
		int l = uid(mt), r = uid(mt);

		if (r < l)
			swap(l, r);

		cout << l << ' ' << r << ' ' << x(mt) << endl;
	}
}


int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	cout << fixed;

#ifdef LOCAL
	auto st = chrono::steady_clock::now();

	ifstream fin("../input.txt");

	do
	{
		solve(fin);

		cout << "===" << endl;

		string str;
		while (getline(fin, str) && str != string(max(1, (int) str.size()), '='));
	} while (fin);

#ifdef STRESS
	for (int cnt = 1;; cnt++)
	{
		stringstream ss, in1, out1, in2, out2;

		gen(ss);

		in1 << ss.str();
		in2 << ss.str();

		solve(in1, out1);
		solve<naive>(in2, out2);

		if (out1.str() != out2.str())
		{
			cout << "fail: " << cnt << endl;
			cout << ss.str();
			cout << "solve:" << endl;
			cout << out1.str();
			cout << "stress:" << endl;
			cout << out2.str();

			break;
		}
		else if (cnt % 100 == 0)
			cout << "ok: " << cnt << endl;
	}
#endif

	cout << setprecision((int) floor(log10(chrono::steady_clock::duration::period::den)));
	cout << "clock: " << chrono::duration<double>(chrono::steady_clock::now() - st).count() << endl;
#else
	solve();
#endif

	return 0;
}
