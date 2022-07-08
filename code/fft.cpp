auto gen_perm(size_t n)
{
	vector<unsigned> perm(n);

	auto lg = __builtin_ctz((int) perm.size());

	for (unsigned i = 0; i < perm.size(); i++)
		perm[i] = (perm[i / 2] >> 1u) ^ ((i & 1u) << (lg - 1));

	return perm;
}


template<bool inv = false>
void fft(vector<mod_t> &arr, const vector<unsigned> &perm, const vector<mod_t> &z)
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

	for (int k = 0, w_exp = root_exp / 2; (1u << k) < arr.size(); k++, w_exp /= 2)
	{
		const int step = int(1u << k);

		for (int i = 0; i < arr.size(); i += 2 * step)
			for (int j = 0; j < step; j++)
			{
				auto &a = arr[i + j];
				auto &b = arr[i + j + step];

				mod_t x = a + b * get_z(j * w_exp);
				mod_t y = a + b * get_z(j * w_exp + root_exp / 2);

				a = x;
				b = y;
			}
	}

	if constexpr (inv)
	{
		auto mul = mod_t{int(arr.size())}.recip();

		for (auto &it: arr)
			it *= mul;
	}
}


void mult(vector<mod_t> &a, vector<mod_t> &&b)
{
	auto sz = up(max(a.size(), b.size()));

	a.resize(sz);
	b.resize(sz);

	auto perm = gen_perm(sz);

	auto w = mod_t{31}.pow((1u << 23u) / sz);

	vector<mod_t> z(sz, mod_t{1});

	for (int j = 1; j < z.size(); j++)
		if (j % 2 == 0)
			z[j] = z[j / 2].sqr();
		else
			z[j] = z[j - 1] * w;

	fft(a, perm, z);
	fft(b, perm, z);

	for (int i = 0; i < a.size(); i++)
		a[i] *= b[i];

	fft<true>(a, perm, z);
}
