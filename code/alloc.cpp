template<size_t sz>
class chunk_alloc
{
public:
	static constexpr auto chunk_size = sz;

private:
	using chunk_t = array<byte, chunk_size>;

	deque<chunk_t> mem;
	stack<void *> emp;

public:
	void *allocate()
	{
		if (emp.empty())
			emp.push(reinterpret_cast<void *>(&mem.emplace_back()));

		auto ans = emp.top();
		emp.pop();

		return ans;
	}

	void deallocate(void *p) noexcept
	{
		emp.push(p);
	}
};


chunk_alloc<64> pool;


template<class T>
struct dummy_alloc
{
	using value_type = T;

	dummy_alloc() noexcept = default;

	template<class U>
	explicit dummy_alloc(const dummy_alloc<U> &) noexcept
	{}

	T *allocate(std::size_t n)
	{
		if constexpr (sizeof(value_type) == decltype(pool)::chunk_size)
			return static_cast<T *>(pool.allocate());
		else
			return static_cast<T *>(::operator new(n * sizeof(value_type)));
	}

	void deallocate(T *p, std::size_t n)
	{
		if constexpr (sizeof(value_type) == decltype(pool)::chunk_size)
			return pool.deallocate(p);
		else
			::delete (p);
	}
};

template<class T, class U>
constexpr bool operator==(const dummy_alloc<T> &, const dummy_alloc<U> &) noexcept
{ return true; }

template<class T, class U>
constexpr bool operator!=(const dummy_alloc<T> &, const dummy_alloc<U> &) noexcept
{ return false; }
