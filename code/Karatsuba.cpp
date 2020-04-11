// functon Karatsuba (and stupid as well) computes c += a * b, not c = a * b

using hvect = vector<modulo<>>::iterator;
using hcvect = vector<modulo<>>::const_iterator;


void add(hcvect abegin, hcvect aend, hvect ans)
{
	for (auto it = abegin; it != aend; ++it, ++ans)
		*ans += *it;
}


void sub(hcvect abegin, hcvect aend, hvect ans)
{
	for (auto it = abegin; it != aend; ++it, ++ans)
		*ans -= *it;
}


void stupid(int siz, hcvect abegin, hcvect bbegin, hvect ans)
{
	for (auto a = abegin; a != abegin + siz; ++a, ans -= (siz - 1))
		for (auto b = bbegin; b != bbegin + siz; ++b, ++ans)
			*ans += *a * *b;
}


void Karatsuba(size_t siz, hcvect abegin, hcvect bbegin, hvect ans, hvect small, hvect big, hvect sum)
{
	assert((siz & (siz - 1)) == 0);

	if (siz <= 32)
	{
		stupid(siz, abegin, bbegin, ans);

		return;
	}

	auto amid = abegin + siz / 2, aend = abegin + siz;
	auto bmid = bbegin + siz / 2, bend = bbegin + siz;
	auto smid = sum + siz / 2, send = sum + siz;

	fill(small, small + siz, 0);
	Karatsuba(siz / 2, abegin, bbegin, small, small + siz, big + siz, sum);
	fill(big, big + siz, 0);
	Karatsuba(siz / 2, amid, bmid, big, small + siz, big + siz, sum);

	copy(abegin, amid, sum);
	add(amid, aend, sum);
	copy(bbegin, bmid, sum + siz / 2);
	add(bmid, bend, sum + siz / 2);

	Karatsuba(siz / 2, sum, smid, ans + siz / 2, small + siz, big + siz, send);

	add(small, small + siz, ans);
	sub(small, small + siz, ans + siz / 2);
	add(big, big + siz, ans + siz);
	sub(big, big + siz, ans + siz / 2);
}


void mult(vector<modulo<>> a, vector<modulo<>> b, vector<modulo<>> &c)
{
	a.resize(up(max(a.size(), b.size())), 0);
	b.resize(a.size(), 0);

	c.resize(max(c.size(), a.size() * 2), 0);

	vector<modulo<>> small(2 * a.size());
	auto big = small;
	auto sum = small;

	Karatsuba(a.size(), a.begin(), b.begin(), c.begin(), small.begin(), big.begin(), sum.begin());
}
