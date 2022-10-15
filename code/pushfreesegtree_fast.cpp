template<class Val, class Change, Val zero = Val{}, Change one = Change{}>
class pushfreesegtree
{
	vector<pair<Val, Change>> arr;

	void upd(int v)
	{
		if (0 < v && v < arr.size() / 2)
			arr[v].first = arr[2 * v].first * arr[2 * v].second + arr[2 * v + 1].first * arr[2 * v + 1].second;
	}

public:
	explicit pushfreesegtree(size_t n = 0) : arr(2 * up(n), {zero, one})
	{}

	template<class It>
	explicit pushfreesegtree(It be, It en) : arr(2 * up(distance(be, en)), {zero, one})
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

		while (r > 0)
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
	}

	auto segsum(size_t l, size_t r)
	{
		l += arr.size() / 2;
		r += arr.size() / 2;

		Val ansl = zero, ansr = zero;

		while (r > 0)
		{
			if (l > 1)
				ansl *= arr[l - 1].second;
			if (r < ssize(arr))
				ansr *= arr[r].second;

			if (l < r)
			{
				if (l & 1u)
				{
					ansl += arr[l].first * arr[l].second;
					l++;
				}
				if (r & 1u)
				{
					r--;
					ansr += arr[r].first * arr[r].second;
				}
			}
			else
			{
				if (l & 1u)
					l++;
				if (r & 1u)
					r--;
			}

			l /= 2;
			r /= 2;
		}

		return ansl + ansr;
	}
};
