template<typename T>
vector<T> berlekamp(const vector<T> &s)
{
	vector<T> c, oldC;
	int f = -1;
	for (int i = 0; i < (int) s.size(); i++)
	{
		T delta = s[i];
		for (int j = 1; j <= (int) c.size(); j++)
			delta -= c[j - 1] * s[i - j];
		if (delta == 0)
			continue;

		if (f == -1)
		{
			c.resize(i + 1);
			f = i;
		}
		else
		{
			vector<T> d = oldC;
			for (T &x : d)
				x = -x;
			d.insert(d.begin(), 1);
			T df1 = 0;
			for (int j = 1; j <= (int) d.size(); j++)
				df1 += d[j - 1] * s[f + 1 - j];
			assert(df1 != 0);
			T coef = delta / df1;
			for (T &x : d)
				x *= coef;

			vector<T> zeros(i - f - 1);
			zeros.insert(zeros.end(), d.begin(), d.end());
			d = zeros;
			vector<T> temp = c;
			c.resize(max(c.size(), d.size()));
			for (int j = 0; j < (int) d.size(); j++)
				c[j] += d[j];

			if (i - (int) temp.size() > f - (int) oldC.size())
			{
				oldC = temp;
				f = i;
			}
		}
	}

	return c;
}