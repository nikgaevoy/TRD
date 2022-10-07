template<int mod>
class modulo
{
	int x;

public:
	constexpr modulo(int x = 0) : x(x)
	{}

	constexpr bool operator==(const modulo &rhs) const = default;

	constexpr bool operator!=(const modulo &rhs) const = default;

	constexpr modulo &operator+=(modulo rhs)
	{
		x += rhs.x;
		if (x >= mod)
			x -= mod;

		return *this;
	}

	constexpr modulo operator+(modulo rhs) const
	{
		auto q = *this;
		q += rhs;

		return q;
	}

	constexpr modulo &operator-=(modulo rhs)
	{
		x -= rhs.x;
		if (x < 0)
			x += mod;

		return *this;
	}

	constexpr modulo operator-(modulo rhs) const
	{
		auto q = *this;
		q -= rhs;

		return q;
	}

	constexpr modulo operator*(modulo rhs) const
	{
		return {decltype(x)(x * (ll) rhs.x % mod)};
	}

	constexpr modulo &operator*=(modulo rhs)
	{
		return *this = (*this * rhs);
	}

	constexpr modulo operator/(modulo rhs) const
	{
		return *this * rhs.recip();
	}

	constexpr modulo &operator/=(modulo rhs)
	{
		return *this = (*this / rhs);
	}

	[[nodiscard]] constexpr modulo sqr() const
	{
		return *this * *this;
	}

	[[nodiscard]] constexpr modulo pow(unsigned y) const
	{
		return y == 0 ? modulo{1 % mod} : y & 1u ? *this * pow(y - 1) : pow(y / 2).sqr();
	}

	[[nodiscard]] constexpr modulo recip() const
	{
		return pow(mod - 2);
	}

	constexpr explicit operator int() const
	{
		return x;
	}

	template<int val>
	friend ostream &operator<<(ostream &cout, const modulo<val> &);
};

template<int mod>
ostream &operator<<(ostream &cout, const modulo<mod> &x)
{
	return cout << x.x;
}
