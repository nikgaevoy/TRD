namespace std
{
	template<>
	struct hash<pnt>
	{
		std::size_t operator()(pnt const &s) const noexcept
		{
			return std::hash<ll>{}(s.first * ll(1ull << 32u) + s.second);
		}
	};
}
