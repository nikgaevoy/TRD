#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

mt19937 mt(736);

using ld = double;
const ld eps = 1e-9;

void basis_change(valarray<ld> &row, valarray<ld> &nd, int b)
{
	auto mult = row[b];

	row += nd * mult;

	row[b] = 0;
}

void pivot(vector<valarray<ld>> &a, vector<int> &b, valarray<ld> &func, int wh, int x)
{
	a[wh][b[wh]] = -1;
	b[wh] = x;
	auto den = -a[wh][x];
	a[wh][x] = 0;
	a[wh] /= den;

	for (int i = 0; i < a.size(); i++)
		if (i != wh)
			basis_change(a[i], a[wh], b[wh]);
	basis_change(func, a[wh], b[wh]);
}


bool simplex(vector<valarray<ld>> &a, vector<int> &b, valarray<ld> &func)
{
	while (true)
	{
#ifdef LOCAL
		for (int i = 0; i < a.size(); i++)
		{
			cerr << "x[" << b[i] << "] =";

			for (auto it : a[i])
				cerr << ' ' << it;
			cerr << endl;
		}

		cerr << "max";
		for (auto it : func)
			cerr << ' ' << it;
		cerr << endl;
#endif

		vector<int> cand;

		for (int i = 0; i < func.size() - 1; i++)
			if (func[i] > eps)
				cand.push_back(i);

		if (cand.empty())
			return true;

		uniform_int_distribution<int> uid(0, (int) cand.size() - 1);

		int x = cand[uid(mt)];

		vector<double> len(a.size(), numeric_limits<ld>::max());

		for (int i = 0; i < len.size(); i++)
			if (a[i][x] < -eps)
				len[i] = a[i][a[i].size() - 1] / -a[i][x];

		auto wh = min_element(len.begin(), len.end()) - len.begin();

		if (len[wh] == numeric_limits<ld>::max())
			return false;

		pivot(a, b, func, wh, x);
	}
}


int global_solve(const vector<vector<ld>> &a, const vector<ld> &b, const vector<ld> &c, vector<ld> &ans)
{
	vector<valarray<ld>> can_a(a.size(), valarray<ld>(c.size() + a.size() + 2));
	valarray<ld> can_func(c.size() + a.size() + 2), can_lambda(c.size() + a.size() + 2);
	vector<int> can_b(a.size());
	int n = c.size();

	iota(can_b.begin(), can_b.end(), n);
	for (int i = 0; i < a.size(); i++)
	{
		for (int j = 0; j < a[i].size(); j++)
			can_a[i][j] = -a[i][j];
		can_a[i][can_a[i].size() - 1] = b[i];
		can_a[i][can_a[i].size() - 2] = 1;
	}

	can_lambda[can_lambda.size() - 2] = -1;
	for (int j = 0; j < c.size(); j++)
		can_func[j] = c[j];

	auto wh = min_element(b.begin(), b.end()) - b.begin();

	if (b[wh] < 0)
	{
		pivot(can_a, can_b, can_lambda, wh, can_lambda.size() - 2);

		assert (simplex(can_a, can_b, can_lambda));
	}

	wh = find(can_b.begin(), can_b.end(), can_lambda.size() - 2) - can_b.begin();

	if (abs(can_lambda[can_lambda.size() - 1]) > eps)
		return -1;

	if (wh != can_b.size())
	{
		if (abs(can_a[wh][can_a[wh].size() - 1]) > eps)
			return -1;

		auto q = 0;

		while (q < can_a[wh].size() && can_a[wh][q] >= -eps)
			q++;

		if (q != can_a[wh].size())
		{
			pivot(can_a, can_b, can_lambda, wh, q);
		}
		else
		{
			int q = 0;
			for (int i = 0; i < (int) can_a[wh].size(); i++)
				if (abs(can_a[wh][i]) > abs(can_a[wh][q]))
					q = i;

			if (abs(can_a[wh][q]) > eps)
				pivot(can_a, can_b, can_lambda, wh, q);

			//for (int i = 0; i < can_a[wh].size (); i++)
			//    if (abs (can_a[wh][i]) > eps)
			//    {
			//        pivot (can_a, can_b, can_lambda, wh, i);

			//        break;
			//}
		}
	}

	for (auto &row : can_a)
		row[row.size() - 2] = 0;

	for (int i = 0; i < can_b.size(); i++)
		basis_change(can_func, can_a[i], can_b[i]);

	if (!simplex(can_a, can_b, can_func))
		return 0;

	for (int i = 0; i < can_a.size(); i++)
		if (can_b[i] < ans.size())
			ans[can_b[i]] = can_a[i][can_a[i].size() - 1];

	return 1;
}


void solve(istream &cin = std::cin, ostream &cout = std::cout)
{
	int n, m;

	cin >> n >> m;

	vector<vector<ld>> a(m, vector<double>(n));
	vector<ld> b(m), c(n);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			cin >> a[i][j];
		cin >> b[i];
	}

	for (auto &it : c)
		cin >> it;

	vector<ld> ans(n);

	auto type = global_solve(a, b, c, ans);

	if (type == -1)
	{
		cout << "No solution" << endl;
	}
	else if (type == 0)
	{
		cout << "Unbounded" << endl;
	}
	else
	{
		cout << "Bounded" << endl;

		cout << fixed << setprecision(20);
		for (auto it : ans)
			cout << it << ' ';
		cout << endl;
	}
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
