mt19937 mt(736);

using ld = double;
constexpr ld eps = 1e-9;

vector <ld> &operator+=(vector <ld> &lhs, const vector <ld> &rhs)
{
	assert(ssize(lhs) == ssize(rhs));

	for (auto i: ranges::iota_view(0, ssize(lhs)))
		lhs[i] += rhs[i];

	return lhs;
}

vector <ld> operator*(vector <ld> lhs, ld x)
{
	for (auto &it: lhs)
		it *= x;

	return lhs;
}

vector <ld> &operator/=(vector <ld> &lhs, ld x)
{
	for (auto &it: lhs)
		it /= x;

	return lhs;
}

bool eps_nonneg(ld x)
{
	return x >= -eps;
}

bool eps_zero(ld x)
{
	return abs(x) <= eps;
}

bool cmp_abs(ld a, ld b)
{
	return abs(a) < abs(b);
}


void basis_change(vector <ld> &row, vector <ld> &nd, int b)
{
	auto mult = row[b];

	row += nd * mult;

	row[b] = 0;
}


void pivot(vector <vector<ld>> &a, vector<int> &b, vector <ld> &func, int wh, int x)
{
	a[wh][b[wh]] = -1;
	b[wh] = x;
	auto den = -a[wh][x];
	a[wh][x] = 0;
	a[wh] /= den;

	for (auto i: ranges::iota_view(0, ssize(a)))
		if (i != wh)
			basis_change(a[i], a[wh], b[wh]);
	basis_change(func, a[wh], b[wh]);
}

bool simplex(vector <vector<ld>> &a, vector<int> &b, vector <ld> &func)
{
	while (true)
	{
		vector<int> cand;

		for (auto i: ranges::iota_view(0, ssize(func) - 1))
			if (func[i] > eps)
				cand.push_back(i);

		if (cand.empty())
			return true;

		auto x = cand[uniform_int_distribution < int > {0, (int) cand.size() - 1}(mt)];

		vector <ld> len(a.size(), numeric_limits<ld>::max());

		for (auto i: ranges::iota_view(0, ssize(len)))
			if (a[i][x] < -eps)
				len[i] = a[i].back() / -a[i][x];

		auto wh = int(ranges::min_element(len) - len.begin());

		if (len[wh] == numeric_limits<ld>::max())
			return false;

		pivot(a, b, func, wh, x);
	}
}

enum results
{
	NO_SOLUTION, UNBOUNDED, BOUNDED
};

/*
 * Solving system of linear inequalities in the form
 * $a * x <= rhs$
 * $x >= 0$
 * $costs * x -> max$
 * assumes at least one inequality and at least one variable
 * */
results global_solve(vector <vector<ld>> a, const vector <ld> &rhs, const vector <ld> &costs, vector <ld> &ans)
{
	assert(!a.empty() && a.size() == rhs.size() && !costs.empty() && ans.size() == costs.size());
	const auto m = costs.size() + a.size() + 2;

	for (auto i: ranges::iota_view(0, ssize(a)))
	{
		auto &row = a[i];

		row /= -1; // just finding inverse
		row.resize(m);
		row.back() = rhs[i];
		row.rbegin()[1] = 1;
	}

	vector <ld> func(m), lambda(m);
	vector<int> b(a.size());

	iota(b.begin(), b.end(), (int) costs.size());

	lambda.rbegin()[1] = -1;
	for (auto j: ranges::iota_view(0, ssize(costs)))
		func[j] = costs[j];

	auto wh = int(ranges::min_element(rhs) - rhs.begin());

	if (rhs[wh] < 0)
	{
		pivot(a, b, lambda, wh, (int) lambda.size() - 2);

		auto q = simplex(a, b, lambda);

		assert(q);
	}

	wh = int(ranges::find(b, (int) lambda.size() - 2) - b.begin());

	if (!eps_zero(lambda.back()))
		return NO_SOLUTION;

	if (wh != size(b))
	{
		if (!eps_zero(a[wh].back()))
			return NO_SOLUTION;

		auto q = int(ranges::find_if(a[wh], eps_nonneg) - a[wh].begin());

		if (q != ssize(a[wh]))
		{
			pivot(a, b, lambda, wh, q);
		}
		else
		{
			q = int(ranges::max_element(a[wh], cmp_abs) - a[wh].begin());

			if (!eps_zero(a[wh][q]))
				pivot(a, b, lambda, wh, q);
		}
	}

	for (auto &row: a)
		row.rbegin()[1] = 0;

	for (auto i: ranges::iota_view(0, ssize(b)))
		basis_change(func, a[i], b[i]);

	if (!simplex(a, b, func))
		return UNBOUNDED;

	for (auto i: ranges::iota_view(0, ssize(a)))
		if (b[i] < ssize(ans))
			ans[b[i]] = a[i].back();

	return BOUNDED;
}
