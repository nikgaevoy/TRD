template<ull mod = (1ull << 61) - 1, ull mul = 736>
class hasher
{
	vector<ull> ph, pw;

	constexpr static ull prod(ull a, ull b)
	{
		return a * (unsigned __int128) b % mod;
	}

	constexpr static ull sub(ull a, ull b)
	{
		if (b > a)
			a += mod;

		return a - b;
	}

public:
	explicit hasher(string_view sv = "") : ph(sv.size() + 1), pw(sv.size() + 1)
	{
		pw[0] = 1;
		for (int i = 1; i < ssize(pw); i++)
		{
			pw[i] = prod(pw[i - 1], mul);
			ph[i] = prod(ph[i - 1], mul) + sv[i - 1];

			if (ph[i] >= mod)
				ph[i] -= mod;
		}
	}

	ull operator()(size_t l, size_t r) const
	{
		return sub(ph[r], prod(ph[l], pw[r - l]));
	}
};
