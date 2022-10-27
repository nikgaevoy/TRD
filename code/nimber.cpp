template<class T, int lvl>
pair<T, T> split(T x)
{
	return {x >> lvl, x & ((T{1} << lvl) - 1)};
}

template<class T, int lvl>
T combine(T a, T b)
{
	return (a << lvl) | b;
}

template<class T, int lvl = 8 * sizeof(T)>
T nim_hmul(T x)
{
	constexpr int half = lvl / 2;
	if constexpr (lvl == 1)
		return x;

	auto [a, b] = split<T, half>(x);

	return combine<T, half>(nim_hmul<T, half>(a ^ b), nim_hmul<T, half>(nim_hmul<T, half>(a)));
}

template<class T, int lvl = 8 * sizeof(T)>
T nim_mul(T x, T y)
{
	constexpr int half = lvl / 2;
	if constexpr (lvl == 1)
		return x & y;

	auto [a, b] = split<T, half>(x);
	auto [c, d] = split<T, half>(y);

	auto ac = nim_mul<T, half>(a, c);
	auto bd = nim_mul<T, half>(b, d);
	auto hp = nim_mul<T, half>(a ^ b, c ^ d) ^ bd;

	return combine<T, half>(hp, bd ^ nim_hmul<T, half>(ac));
}

template<class T, int lvl = 8 * sizeof(T)>
T nim_sqr(T x)
{
	return nim_mul<T, lvl>(x, x);
}

template<class T, int lvl = 8 * sizeof(T)>
T nim_sqrt(T x)
{
	constexpr int half = lvl / 2;
	if constexpr (lvl == 1)
		return x;

	auto [a, b] = split<T, half>(x);

	return combine<T, half>(nim_sqrt<T, half>(a), nim_sqrt<T, half>(nim_hmul<T, half>(a) ^ b));
}

template<class T, int lvl = 8 * sizeof(T)>
T nim_recip(T x)
{
	constexpr int half = lvl / 2;
	if constexpr (lvl == 1)
		return x;

	auto [a, b] = split<T, half>(x);

	auto ad = nim_mul<T, half>(a ^ b, b);
	auto bc = nim_hmul<T, half>(nim_sqr<T, half>(a));
	auto det_recip = nim_recip<T, half>(ad ^ bc);

	return combine<T, half>(nim_mul(a, det_recip), nim_mul(a ^ b, det_recip));
}
