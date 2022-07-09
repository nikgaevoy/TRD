#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

mt19937 mt(736);


auto gen_perm(size_t n)
{
	vector<unsigned> perm(n);

	auto lg = __builtin_ctz((int) perm.size());

	for (unsigned i = 0; i < perm.size(); i++)
		perm[i] = (perm[i / 2] >> 1u) ^ ((i & 1u) << (lg - 1));

	return perm;
}


template<bool inv = false, class T>
void fft(vector<T> &arr, const vector<T> &z, const vector<unsigned> &perm)
{
	assert(arr.size() == perm.size() && arr.size() <= z.size());

	auto get_z = [&z](int ind)
	{
		if constexpr(inv)
			return z[(z.size() - ind) & (z.size() - 1)];
		else
			return z[ind];
	};

	for (unsigned i = 0; i < arr.size(); i++)
		if (perm[i] < i)
			swap(arr[i], arr[perm[i]]);

	const auto root_exp = (int) z.size();

	for (int step = 1, w_exp = root_exp / 2; step < arr.size(); step *= 2, w_exp /= 2)
		for (int i = 0; i < arr.size(); i += 2 * step)
			for (int j = 0; j < step; j++)
			{
				auto &a = arr[i + j];
				auto &b = arr[i + j + step];

				auto x = a + b * get_z(j * w_exp);
				auto y = a + b * get_z(j * w_exp + root_exp / 2);

				a = x;
				b = y;
			}

	if constexpr(inv)
	{
		auto mul = T(1) / T(arr.size());

		for (auto &it: arr)
			it *= mul;
	}
}


template<class T>
auto up(const T &n)
{
	T r = 1;

	while (r < n)
		r *= 2;

	return r;
}


template<class T>
void mult(vector<T> &a, vector<T> &&b, T w)
{
	auto sz = 2 * up(max(a.size(), b.size()));

	a.resize(sz);
	b.resize(sz);

	auto perm = gen_perm(sz);

	vector<T> z(sz, T(1));

	for (int j = 1; j < z.size(); j++)
		if (j % 2 == 0)
			z[j] = z[j / 2] * z[j / 2];
		else
			z[j] = z[j - 1] * w;

	fft(a, z, perm);
	fft(b, z, perm);

	for (int i = 0; i < a.size(); i++)
		a[i] *= b[i];

	fft<true>(a, z, perm);
}


constexpr int mod = (119 << 23) + 1;


struct mod_t
{
	int x;

	constexpr explicit mod_t(int x = 0) : x(x)
	{}

	constexpr mod_t operator+(const mod_t &rhs) const
	{
		return mod_t((x + rhs.x) % mod);
	}

	constexpr mod_t operator*(const mod_t &rhs) const
	{
		return mod_t(int((x * (ll) rhs.x) % mod));
	}

	constexpr mod_t &operator*=(const mod_t &rhs)
	{
		*this = *this * rhs;

		return *this;
	}

	[[nodiscard]] constexpr mod_t sqr() const
	{
		return *this * *this;
	}

	[[nodiscard]] constexpr mod_t pow(unsigned y) const
	{
		return y == 0 ? mod_t(1) : y % 2 == 0 ? pow(y / 2).sqr() : pow(y - 1) * *this;
	}

	[[nodiscard]] constexpr mod_t recip() const
	{
		return pow(mod - 2);
	}

	constexpr mod_t operator/(const mod_t &rhs) const
	{
		return *this * rhs.recip();
	}
};


void mult(vector<mod_t> &a, vector<mod_t> &&b)
{
	auto root_exp = 2 * up(max(a.size(), b.size()));

	auto w = mod_t(31).pow((1u << 23u) / root_exp);

	mult(a, move(b), w);
}


using ld = double;
using pnt_t = complex<ld>;


void mult(vector<pnt_t> &a, vector<pnt_t> &&b)
{
	auto root_exp = ld(2 * up(max(a.size(), b.size())));

	auto w = pnt_t(cos(2 * M_PI / root_exp), sin(2 * M_PI / root_exp));

	mult(a, move(b), w);
}


template<class T>
auto conv(const vector<int> &arr)
{
	vector<T> ans(arr.size());

	for (int i = 0; i < ans.size(); i++)
		ans[i] = T(arr[i]);

	return ans;
}


void solve_test(istream &cin = std::cin, ostream &cout = std::cout)
{
	int n;

	cin >> n;

	vector<int> arr(n);

	for (auto &it: arr)
		cin >> it;

	int m;

	cin >> m;

	vector<int> brr(m);

	for (auto &it: brr)
		cin >> it;

	auto a_mod = conv<mod_t>(arr);
	auto a_pnt = conv<pnt_t>(arr);

	mult(a_mod, conv<mod_t>(brr));
	mult(a_pnt, conv<pnt_t>(brr));

	for (int i = 0; i < a_mod.size(); i++)
		assert(a_mod[i].x == round(a_pnt[i].real()));

	while (a_mod.size() > 1 && a_mod.back().x == 0)
		a_mod.pop_back();

	for (auto it: a_mod)
		cout << it.x << ' ';

	cout << endl;
}


void solve(istream &cin = std::cin, ostream &cout = std::cout)
{
	int t = 1;

	for (int i = 0; i < t; i++)
		solve_test(cin, cout);
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

	cout << setprecision((int) floor(log10(chrono::steady_clock::duration::period::den)));
	cout << "clock: " << chrono::duration<double>(chrono::steady_clock::now() - st).count() << endl;
#else
	solve();
#endif

	return 0;
}
