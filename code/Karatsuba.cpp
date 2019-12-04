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

	fill(ans, ans + 2 * siz, 0);

	if (siz < 60)
	{
		stupid(siz, abegin, bbegin, ans);

		return;
	}

	auto amid = abegin + siz / 2, aend = abegin + siz;
	auto bmid = bbegin + siz / 2, bend = bbegin + siz;
	auto smid = sum + siz / 2, send = sum + siz;

	Karatsuba(siz / 2, abegin, bbegin, small, small + siz, big + siz, sum);
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
	a.resize(up(max(a.size(), b.size())));
	b.resize(a.size());

	c.resize(a.size() * 2);

	auto small = c;
	auto big = c;
	auto sum = c;

	Karatsuba(a.size(), a.begin(), b.begin(), c.begin(), small.begin(), big.begin(), sum.begin());
}
