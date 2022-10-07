template<class T>
class segtree
{
	vector<pair<T, T>> arr;
	size_t sz;

	void upd(int v)
	{
		if (0 < v && v < arr.size() / 2)
			arr[v].first = arr[2 * v].first * arr[2 * v].second + arr[2 * v + 1].first * arr[2 * v + 1].second;
	}

public:
	explicit segtree(size_t n) : arr(2 * up(n)), sz(n)
	{}

	auto segmult(const T &x, size_t l, size_t r)
	{
		l += arr.size() / 2;
		r += arr.size() / 2;

		while (l > 0)
		{
			upd(l - 1);
			upd(r);

			if (l < r)
			{
				if (l & 1u)
					arr[l++].second *= x;
				if (r & 1u)
					arr[--r].second *= x;
			}

			l /= 2;
			r /= 2;
		}

		assert(r == 0);
	}

	auto segmult(const T &x, size_t l = 0)
	{
		return segmult(x, l, sz);
	}

	auto segsum(size_t l, size_t r)
	{
		l += arr.size() / 2;
		r += arr.size() / 2;

		T ansl{}, ansr{};

		while (l > 0)
		{
			if (l < r)
			{
				if (l & 1u)
				{
					ansl *= arr[l - 1].second;
					ansl += arr[l].first * arr[l].second;
					l++;
				}
				if (r & 1u)
				{
					ansr *= arr[r].second;
					r--;
					ansr += arr[r].first * arr[r].second;
				}
			}

			l /= 2;
			r /= 2;
		}

		assert(r == 0);

		return ansl + ansr;
	}

	auto segsum(size_t l = 0)
	{
		return segsum(l, sz);
	}
};
