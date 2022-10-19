template<class Val, class Change, Change one = Change{}>
class pushfreesegtree
{
	vector<pair<Val, Change>> arr;

	void upd(size_t v)
	{
		arr[v].first = (arr[2 * v].first + arr[2 * v + 1].first) * arr[v].second;
	}

public:
	explicit pushfreesegtree(size_t n = 0) : arr(2 * n + 2, {Val{}, one})
	{}

	template<class It>
	explicit pushfreesegtree(It be, It en) : arr(2 * distance(be, en) + 2, {Val{}, one})
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

		while (true)
		{
			if (l < r)
			{
				if (l & 1u)
				{
					arr[l].first *= x;
					arr[l].second *= x;
				}
				if (r & 1u)
				{
					arr[r - 1].first *= x;
					arr[r - 1].second *= x;
				}
			}

			l = (l + 1) / 2;
			r /= 2;

			if (r == 0)
				break;

			upd(l - 1);
			upd(r);
		}
	}

	[[nodiscard]] Val segsum(size_t l, size_t r) const
	{
		l += arr.size() / 2;
		r += arr.size() / 2;

		Val ansl{}, ansr{};

		while (true)
		{
			if (l < r)
			{
				if (l & 1u)
					ansl = ansl + arr[l].first;
				if (r & 1u)
					ansr = arr[r - 1].first + ansr;
			}

			l = (l + 1) / 2;
			r /= 2;

			if (r == 0)
				break;

			ansl *= arr[l - 1].second;
			ansr *= arr[r].second;
		}

		return ansl + ansr;
	}
};
