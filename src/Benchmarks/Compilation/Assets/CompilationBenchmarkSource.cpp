// Elpida C++ compilation benchmark source (compiled by the C++ Compilation benchmark, not part of the build).
//
// A self-contained translation unit (no #include: the benchmark has no system headers) shaped like typical modern
// C++: a small standard-library-like core (type traits, utilities, tuple, smart pointers, containers, optional,
// variant, function, algorithms, hash and ordered maps, formatting) followed by application code that instantiates
// it: template geometry (vectors, matrices, inverse, rays, bounding boxes) and text processing (splitting, word
// counting in a hash map, stable sort, formatting). The application part is sized so that one -O3 compile takes about
// 0.25 s on a Ryzen 9 7950X3D (about 2.5 s expected on a Pentium 4 Northwood).
//
// Copyright (c) 2026  Ioannis Panagiotopoulos. Licensed under the GNU General Public License v3 or later.

typedef decltype(sizeof(0)) size_t;
typedef decltype((char*)0 - (char*)0) ptrdiff_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef decltype(nullptr) nullptr_t;

extern "C"
{
	void* malloc(size_t size);
	void free(void* ptr);
	void* memcpy(void* dst, const void* src, size_t n);
	void* memmove(void* dst, const void* src, size_t n);
	void* memset(void* dst, int c, size_t n);
	int memcmp(const void* a, const void* b, size_t n);
	size_t strlen(const char* s);
	double sqrt(double x);
	double sin(double x);
	double cos(double x);
	double exp(double x);
	double log(double x);
	double pow(double x, double y);
	double floor(double x);
	double fabs(double x);
	[[noreturn]] void abort();
}

inline void* operator new(size_t, void* where) noexcept { return where; }
void* operator new(size_t size);
void operator delete(void* ptr) noexcept;
void operator delete(void* ptr, size_t) noexcept;
void* operator new[](size_t size);
void operator delete[](void* ptr) noexcept;

namespace std
{
	// The compiler requires this exact shape for braced initializer lists.
	template<class T>
	class initializer_list
	{
	public:
		typedef T value_type;
		typedef const T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef const T* iterator;
		typedef const T* const_iterator;

		constexpr initializer_list() noexcept : _begin(nullptr), _size(0) {}
		constexpr size_t size() const noexcept { return _size; }
		constexpr const T* begin() const noexcept { return _begin; }
		constexpr const T* end() const noexcept { return _begin + _size; }
	private:
		const T* _begin;
		size_t _size;
		constexpr initializer_list(const T* b, size_t s) noexcept : _begin(b), _size(s) {}
	};
}

namespace ebs
{
	using std::initializer_list;

	// ---------------------------------------------------------------------------------------------- type traits

	template<class T, T V>
	struct integral_constant
	{
		static constexpr T value = V;
		typedef T value_type;
		typedef integral_constant type;
		constexpr operator value_type() const noexcept { return value; }
		constexpr value_type operator()() const noexcept { return value; }
	};

	template<bool B> using bool_constant = integral_constant<bool, B>;
	typedef bool_constant<true> true_type;
	typedef bool_constant<false> false_type;

	template<class T> struct type_identity { typedef T type; };

	template<bool B, class T = void> struct enable_if {};
	template<class T> struct enable_if<true, T> { typedef T type; };
	template<bool B, class T = void> using enable_if_t = typename enable_if<B, T>::type;

	template<bool B, class T, class F> struct conditional { typedef T type; };
	template<class T, class F> struct conditional<false, T, F> { typedef F type; };
	template<bool B, class T, class F> using conditional_t = typename conditional<B, T, F>::type;

	template<class A, class B> struct is_same : false_type {};
	template<class A> struct is_same<A, A> : true_type {};
	template<class A, class B> inline constexpr bool is_same_v = is_same<A, B>::value;

	template<class T> struct remove_const { typedef T type; };
	template<class T> struct remove_const<const T> { typedef T type; };
	template<class T> struct remove_volatile { typedef T type; };
	template<class T> struct remove_volatile<volatile T> { typedef T type; };
	template<class T> struct remove_cv { typedef typename remove_volatile<typename remove_const<T>::type>::type type; };
	template<class T> using remove_cv_t = typename remove_cv<T>::type;

	template<class T> struct remove_reference { typedef T type; };
	template<class T> struct remove_reference<T&> { typedef T type; };
	template<class T> struct remove_reference<T&&> { typedef T type; };
	template<class T> using remove_reference_t = typename remove_reference<T>::type;
	template<class T> using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

	template<class T> struct is_lvalue_reference : false_type {};
	template<class T> struct is_lvalue_reference<T&> : true_type {};
	template<class T> struct is_reference : false_type {};
	template<class T> struct is_reference<T&> : true_type {};
	template<class T> struct is_reference<T&&> : true_type {};

	template<class T> struct is_pointer_helper : false_type {};
	template<class T> struct is_pointer_helper<T*> : true_type {};
	template<class T> struct is_pointer : is_pointer_helper<remove_cv_t<T>> {};
	template<class T> inline constexpr bool is_pointer_v = is_pointer<T>::value;

	template<class T> struct is_array : false_type {};
	template<class T> struct is_array<T[]> : true_type {};
	template<class T, size_t N> struct is_array<T[N]> : true_type {};

	template<class T> struct is_integral_helper : false_type {};
	template<> struct is_integral_helper<bool> : true_type {};
	template<> struct is_integral_helper<char> : true_type {};
	template<> struct is_integral_helper<signed char> : true_type {};
	template<> struct is_integral_helper<unsigned char> : true_type {};
	template<> struct is_integral_helper<short> : true_type {};
	template<> struct is_integral_helper<unsigned short> : true_type {};
	template<> struct is_integral_helper<int> : true_type {};
	template<> struct is_integral_helper<unsigned int> : true_type {};
	template<> struct is_integral_helper<long> : true_type {};
	template<> struct is_integral_helper<unsigned long> : true_type {};
	template<> struct is_integral_helper<long long> : true_type {};
	template<> struct is_integral_helper<unsigned long long> : true_type {};
	template<class T> struct is_integral : is_integral_helper<remove_cv_t<T>> {};
	template<class T> inline constexpr bool is_integral_v = is_integral<T>::value;

	template<class T> struct is_floating_point_helper : false_type {};
	template<> struct is_floating_point_helper<float> : true_type {};
	template<> struct is_floating_point_helper<double> : true_type {};
	template<> struct is_floating_point_helper<long double> : true_type {};
	template<class T> struct is_floating_point : is_floating_point_helper<remove_cv_t<T>> {};
	template<class T> inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

	template<class T> struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};
	template<class T> inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

	template<class T> struct is_signed : bool_constant<is_arithmetic_v<T> && (T(-1) < T(0))> {};
	template<class T> struct is_unsigned : bool_constant<is_arithmetic_v<T> && !(T(-1) < T(0))> {};

	template<class T> struct is_enum : bool_constant<__is_enum(T)> {};
	template<class T> struct is_class : bool_constant<__is_class(T)> {};
	template<class T> struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};
	template<class T> inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;
	template<class T> struct is_trivially_destructible : bool_constant<__is_trivially_destructible(T)> {};
	template<class T> inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;
	template<class B, class D> struct is_base_of : bool_constant<__is_base_of(B, D)> {};
	template<class T, class... A> struct is_constructible : bool_constant<__is_constructible(T, A...)> {};
	template<class T, class... A> inline constexpr bool is_constructible_v = is_constructible<T, A...>::value;
	template<class T> struct is_default_constructible : is_constructible<T> {};
	template<class T> struct is_copy_constructible : is_constructible<T, const T&> {};
	template<class T> struct is_move_constructible : is_constructible<T, T&&> {};
	template<class T, class... A> struct is_nothrow_constructible : bool_constant<__is_nothrow_constructible(T, A...)> {};
	template<class T> inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_constructible<T, T&&>::value;
	template<class T> struct underlying_type { typedef __underlying_type(T) type; };

	template<class T> struct add_lvalue_reference { typedef T& type; };
	template<> struct add_lvalue_reference<void> { typedef void type; };
	template<class T> struct add_rvalue_reference { typedef T&& type; };
	template<> struct add_rvalue_reference<void> { typedef void type; };
	template<class T> typename add_rvalue_reference<T>::type declval() noexcept;

	template<class T> struct remove_extent { typedef T type; };
	template<class T> struct remove_extent<T[]> { typedef T type; };
	template<class T, size_t N> struct remove_extent<T[N]> { typedef T type; };

	template<class T> struct decay
	{
	private:
		typedef remove_reference_t<T> U;
	public:
		typedef conditional_t<is_array<U>::value, typename remove_extent<U>::type*, remove_cv_t<U>> type;
	};
	template<class T> using decay_t = typename decay<T>::type;

	template<class...> using void_t = void;

	template<class From, class To, class = void> struct is_convertible : false_type {};
	template<class From, class To>
	struct is_convertible<From, To, void_t<decltype(static_cast<void (*)(To)>(nullptr)(declval<From>()))>> : true_type {};
	template<class From, class To> inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

	template<class F, class... A> using invoke_result_t = decltype(declval<F>()(declval<A>()...));

	template<class... B> struct conjunction : true_type {};
	template<class B1> struct conjunction<B1> : B1 {};
	template<class B1, class... Bn> struct conjunction<B1, Bn...> : conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};
	template<class... B> struct disjunction : false_type {};
	template<class B1> struct disjunction<B1> : B1 {};
	template<class B1, class... Bn> struct disjunction<B1, Bn...> : conditional_t<bool(B1::value), B1, disjunction<Bn...>> {};
	template<class B> struct negation : bool_constant<!bool(B::value)> {};

	template<class T> struct make_unsigned_helper { typedef T type; };
	template<> struct make_unsigned_helper<char> { typedef unsigned char type; };
	template<> struct make_unsigned_helper<signed char> { typedef unsigned char type; };
	template<> struct make_unsigned_helper<short> { typedef unsigned short type; };
	template<> struct make_unsigned_helper<int> { typedef unsigned int type; };
	template<> struct make_unsigned_helper<long> { typedef unsigned long type; };
	template<> struct make_unsigned_helper<long long> { typedef unsigned long long type; };
	template<class T> using make_unsigned_t = typename make_unsigned_helper<remove_cv_t<T>>::type;

	template<class T, class... Ts> struct index_of;
	template<class T, class... Ts> struct index_of<T, T, Ts...> : integral_constant<size_t, 0> {};
	template<class T, class U, class... Ts> struct index_of<T, U, Ts...> : integral_constant<size_t, 1 + index_of<T, Ts...>::value> {};

	template<size_t I, class... Ts> struct type_at;
	template<class T, class... Ts> struct type_at<0, T, Ts...> { typedef T type; };
	template<size_t I, class T, class... Ts> struct type_at<I, T, Ts...> { typedef typename type_at<I - 1, Ts...>::type type; };
	template<size_t I, class... Ts> using type_at_t = typename type_at<I, Ts...>::type;

	template<class T, class... Ts> struct contains : disjunction<is_same<T, Ts>...> {};

	template<size_t... Ts> struct static_max;
	template<size_t A> struct static_max<A> : integral_constant<size_t, A> {};
	template<size_t A, size_t B, size_t... R>
	struct static_max<A, B, R...> : static_max<(A > B ? A : B), R...> {};

	template<class T, T... Is> struct integer_sequence { static constexpr size_t size() noexcept { return sizeof...(Is); } };
	template<size_t... Is> using index_sequence = integer_sequence<size_t, Is...>;
	template<class T, T N> using make_integer_sequence = __make_integer_seq<integer_sequence, T, N>;
	template<size_t N> using make_index_sequence = make_integer_sequence<size_t, N>;
	template<class... T> using index_sequence_for = make_index_sequence<sizeof...(T)>;

	// ---------------------------------------------------------------------------------------------- numeric limits

	template<class T> struct numeric_limits;
	template<> struct numeric_limits<int>
	{
		static constexpr int min() noexcept { return -2147483647 - 1; }
		static constexpr int max() noexcept { return 2147483647; }
		static constexpr int digits10 = 9;
	};
	template<> struct numeric_limits<unsigned int>
	{
		static constexpr unsigned int min() noexcept { return 0; }
		static constexpr unsigned int max() noexcept { return 0xffffffffu; }
		static constexpr int digits10 = 9;
	};
	template<> struct numeric_limits<long long>
	{
		static constexpr long long min() noexcept { return -9223372036854775807ll - 1; }
		static constexpr long long max() noexcept { return 9223372036854775807ll; }
		static constexpr int digits10 = 18;
	};
	template<> struct numeric_limits<unsigned long long>
	{
		static constexpr unsigned long long min() noexcept { return 0; }
		static constexpr unsigned long long max() noexcept { return 0xffffffffffffffffull; }
		static constexpr int digits10 = 19;
	};
	template<> struct numeric_limits<double>
	{
		static constexpr double min() noexcept { return 2.2250738585072014e-308; }
		static constexpr double max() noexcept { return 1.7976931348623157e+308; }
		static constexpr double lowest() noexcept { return -1.7976931348623157e+308; }
		static constexpr double epsilon() noexcept { return 2.220446049250313e-16; }
		static constexpr double infinity() noexcept { return __builtin_huge_val(); }
		static constexpr int digits10 = 15;
	};
	template<> struct numeric_limits<float>
	{
		static constexpr float min() noexcept { return 1.17549435e-38f; }
		static constexpr float max() noexcept { return 3.40282347e+38f; }
		static constexpr float lowest() noexcept { return -3.40282347e+38f; }
		static constexpr float epsilon() noexcept { return 1.19209290e-7f; }
		static constexpr float infinity() noexcept { return __builtin_huge_valf(); }
		static constexpr int digits10 = 6;
	};

	// ---------------------------------------------------------------------------------------------- utility

	template<class T> constexpr remove_reference_t<T>&& move(T&& t) noexcept { return static_cast<remove_reference_t<T>&&>(t); }
	template<class T> constexpr T&& forward(remove_reference_t<T>& t) noexcept { return static_cast<T&&>(t); }
	template<class T> constexpr T&& forward(remove_reference_t<T>&& t) noexcept { return static_cast<T&&>(t); }

	template<class T>
	constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T>)
	{
		T tmp = move(a);
		a = move(b);
		b = move(tmp);
	}

	template<class T, class U = T>
	constexpr T exchange(T& obj, U&& value)
	{
		T old = move(obj);
		obj = forward<U>(value);
		return old;
	}

	template<class T> constexpr const T& min(const T& a, const T& b) { return b < a ? b : a; }
	template<class T> constexpr const T& max(const T& a, const T& b) { return a < b ? b : a; }
	template<class T, class C> constexpr const T& min(const T& a, const T& b, C comp) { return comp(b, a) ? b : a; }
	template<class T, class C> constexpr const T& max(const T& a, const T& b, C comp) { return comp(a, b) ? b : a; }
	template<class T> constexpr const T& clamp(const T& v, const T& lo, const T& hi) { return v < lo ? lo : (hi < v ? hi : v); }
	template<class T> constexpr T abs(T v) { return v < T(0) ? -v : v; }

	template<class T> constexpr T* addressof(T& v) noexcept { return __builtin_addressof(v); }

	struct in_place_t { explicit in_place_t() = default; };
	inline constexpr in_place_t in_place{};
	template<class T> struct in_place_type_t { explicit in_place_type_t() = default; };
	template<class T> inline constexpr in_place_type_t<T> in_place_type{};
	template<size_t I> struct in_place_index_t { explicit in_place_index_t() = default; };
	template<size_t I> inline constexpr in_place_index_t<I> in_place_index{};

	[[noreturn]] inline void panic(const char* message)
	{
		(void) message;
		abort();
	}

	template<class T1, class T2>
	struct pair
	{
		T1 first;
		T2 second;

		constexpr pair() : first(), second() {}
		constexpr pair(const T1& a, const T2& b) : first(a), second(b) {}
		template<class U1, class U2, enable_if_t<is_constructible_v<T1, U1&&> && is_constructible_v<T2, U2&&>, int> = 0>
		constexpr pair(U1&& a, U2&& b) : first(forward<U1>(a)), second(forward<U2>(b)) {}
		template<class U1, class U2>
		constexpr pair(const pair<U1, U2>& o) : first(o.first), second(o.second) {}
		pair(const pair&) = default;
		pair(pair&&) = default;
		pair& operator=(const pair&) = default;
		pair& operator=(pair&&) = default;

		constexpr bool operator==(const pair& o) const { return first == o.first && second == o.second; }
		constexpr bool operator!=(const pair& o) const { return !(*this == o); }
		constexpr bool operator<(const pair& o) const { return first < o.first || (!(o.first < first) && second < o.second); }
	};

	template<class T1, class T2>
	constexpr pair<decay_t<T1>, decay_t<T2>> make_pair(T1&& a, T2&& b)
	{
		return pair<decay_t<T1>, decay_t<T2>>(forward<T1>(a), forward<T2>(b));
	}

	// ---------------------------------------------------------------------------------------------- tuple

	template<size_t I, class T, bool Empty = __is_empty(T) && !__is_final(T)>
	struct tuple_leaf
	{
		T value;
		constexpr tuple_leaf() : value() {}
		template<class U> constexpr explicit tuple_leaf(U&& u) : value(forward<U>(u)) {}
		constexpr T& get() noexcept { return value; }
		constexpr const T& get() const noexcept { return value; }
	};

	template<size_t I, class T>
	struct tuple_leaf<I, T, true> : private T
	{
		constexpr tuple_leaf() : T() {}
		template<class U> constexpr explicit tuple_leaf(U&& u) : T(forward<U>(u)) {}
		constexpr T& get() noexcept { return *this; }
		constexpr const T& get() const noexcept { return *this; }
	};

	template<class Seq, class... Ts> struct tuple_impl;
	template<size_t... Is, class... Ts>
	struct tuple_impl<index_sequence<Is...>, Ts...> : tuple_leaf<Is, Ts>...
	{
		constexpr tuple_impl() = default;
		template<class... Us>
		constexpr explicit tuple_impl(int, Us&&... us) : tuple_leaf<Is, Ts>(forward<Us>(us))... {}
	};

	template<class... Ts>
	class tuple : public tuple_impl<index_sequence_for<Ts...>, Ts...>
	{
		typedef tuple_impl<index_sequence_for<Ts...>, Ts...> base;
	public:
		constexpr tuple() = default;
		template<class... Us, enable_if_t<sizeof...(Us) == sizeof...(Ts) && sizeof...(Ts) != 0, int> = 0>
		constexpr tuple(Us&&... us) : base(0, forward<Us>(us)...) {}
	};

	template<class T> struct tuple_size;
	template<class... Ts> struct tuple_size<tuple<Ts...>> : integral_constant<size_t, sizeof...(Ts)> {};
	template<size_t I, class T> struct tuple_element;
	template<size_t I, class... Ts> struct tuple_element<I, tuple<Ts...>> { typedef type_at_t<I, Ts...> type; };

	template<size_t I, class... Ts>
	constexpr type_at_t<I, Ts...>& get(tuple<Ts...>& t) noexcept
	{
		return static_cast<tuple_leaf<I, type_at_t<I, Ts...>>&>(t).get();
	}
	template<size_t I, class... Ts>
	constexpr const type_at_t<I, Ts...>& get(const tuple<Ts...>& t) noexcept
	{
		return static_cast<const tuple_leaf<I, type_at_t<I, Ts...>>&>(t).get();
	}
	template<class T, class... Ts>
	constexpr T& get(tuple<Ts...>& t) noexcept { return get<index_of<T, Ts...>::value>(t); }

	template<class... Ts>
	constexpr tuple<decay_t<Ts>...> make_tuple(Ts&&... ts) { return tuple<decay_t<Ts>...>(forward<Ts>(ts)...); }
	template<class... Ts>
	constexpr tuple<Ts&...> tie(Ts&... ts) noexcept { return tuple<Ts&...>(ts...); }

	template<class F, class Tuple, size_t... Is>
	constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, index_sequence<Is...>)
	{
		return forward<F>(f)(get<Is>(forward<Tuple>(t))...);
	}
	template<class F, class Tuple>
	constexpr decltype(auto) apply(F&& f, Tuple&& t)
	{
		return apply_impl(forward<F>(f), forward<Tuple>(t), make_index_sequence<tuple_size<remove_cvref_t<Tuple>>::value>{});
	}

	template<class F, class... Ts, size_t... Is>
	constexpr void for_each_in_tuple_impl(tuple<Ts...>& t, F&& f, index_sequence<Is...>)
	{
		(f(get<Is>(t)), ...);
	}
	template<class F, class... Ts>
	constexpr void for_each_in_tuple(tuple<Ts...>& t, F&& f)
	{
		for_each_in_tuple_impl(t, forward<F>(f), index_sequence_for<Ts...>{});
	}

	// ---------------------------------------------------------------------------------------------- hashing

	inline constexpr uint64_t hash_mix(uint64_t h)
	{
		h ^= h >> 33;
		h *= 0xff51afd7ed558ccdull;
		h ^= h >> 33;
		h *= 0xc4ceb9fe1a85ec53ull;
		h ^= h >> 33;
		return h;
	}

	inline uint64_t hash_bytes(const void* data, size_t size)
	{
		const uint8_t* p = static_cast<const uint8_t*>(data);
		uint64_t h = 0xcbf29ce484222325ull;
		for (size_t i = 0; i < size; ++i)
		{
			h ^= p[i];
			h *= 0x100000001b3ull;
		}
		return hash_mix(h);
	}

	template<class T, class = void> struct hash;
	template<class T>
	struct hash<T, enable_if_t<is_integral_v<T> || is_enum<T>::value>>
	{
		constexpr size_t operator()(T v) const noexcept { return size_t(hash_mix(uint64_t(v))); }
	};
	template<class T>
	struct hash<T*, void>
	{
		size_t operator()(T* p) const noexcept { return size_t(hash_mix(uint64_t(size_t(p)))); }
	};
	template<>
	struct hash<double, void>
	{
		size_t operator()(double v) const noexcept { return size_t(hash_bytes(&v, sizeof(v))); }
	};

	inline constexpr size_t hash_combine(size_t seed, size_t v)
	{
		return seed ^ (v + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2));
	}

	template<class T1, class T2>
	struct hash<pair<T1, T2>, void>
	{
		size_t operator()(const pair<T1, T2>& p) const { return hash_combine(hash<T1>()(p.first), hash<T2>()(p.second)); }
	};

	// ---------------------------------------------------------------------------------------------- function objects

	template<class T = void> struct less { constexpr bool operator()(const T& a, const T& b) const { return a < b; } };
	template<> struct less<void>
	{
		template<class A, class B> constexpr bool operator()(A&& a, B&& b) const { return forward<A>(a) < forward<B>(b); }
	};
	template<class T = void> struct greater { constexpr bool operator()(const T& a, const T& b) const { return b < a; } };
	template<class T = void> struct equal_to { constexpr bool operator()(const T& a, const T& b) const { return a == b; } };
	template<> struct equal_to<void>
	{
		template<class A, class B> constexpr bool operator()(A&& a, B&& b) const { return forward<A>(a) == forward<B>(b); }
	};
	template<class T = void> struct plus { constexpr T operator()(const T& a, const T& b) const { return a + b; } };
	template<> struct plus<void>
	{
		template<class A, class B> constexpr auto operator()(A&& a, B&& b) const { return forward<A>(a) + forward<B>(b); }
	};
	template<class T = void> struct multiplies { constexpr T operator()(const T& a, const T& b) const { return a * b; } };
	struct identity { template<class T> constexpr T&& operator()(T&& t) const noexcept { return forward<T>(t); } };

	template<class F, class... A>
	constexpr decltype(auto) invoke(F&& f, A&&... a) { return forward<F>(f)(forward<A>(a)...); }
	template<class R, class C, class Obj>
	constexpr decltype(auto) invoke(R C::* member, Obj&& obj) { return forward<Obj>(obj).*member; }
	template<class R, class C, class Obj, class... A>
	constexpr decltype(auto) invoke(R (C::* member)(A...), Obj&& obj, A... a) { return (forward<Obj>(obj).*member)(a...); }
	template<class R, class C, class Obj, class... A>
	constexpr decltype(auto) invoke(R (C::* member)(A...) const, Obj&& obj, A... a) { return (forward<Obj>(obj).*member)(a...); }
}

namespace ebs
{
	// ---------------------------------------------------------------------------------------------- memory

	template<class T>
	struct allocator
	{
		typedef T value_type;
		constexpr allocator() noexcept = default;
		template<class U> constexpr allocator(const allocator<U>&) noexcept {}
		T* allocate(size_t n)
		{
			if (n > size_t(-1) / sizeof(T)) panic("allocation size overflow");
			return static_cast<T*>(::operator new(n * sizeof(T)));
		}
		void deallocate(T* p, size_t) noexcept { ::operator delete(p); }
		template<class U> bool operator==(const allocator<U>&) const noexcept { return true; }
		template<class U> bool operator!=(const allocator<U>&) const noexcept { return false; }
	};

	template<class T, class... A>
	constexpr T* construct_at(T* p, A&&... a) { return ::new (static_cast<void*>(p)) T(forward<A>(a)...); }
	template<class T>
	constexpr void destroy_at(T* p) noexcept { p->~T(); }
	template<class It>
	constexpr void destroy(It first, It last) noexcept
	{
		typedef remove_cvref_t<decltype(*first)> T;
		if constexpr (!is_trivially_destructible_v<T>)
		{
			for (; first != last; ++first) destroy_at(addressof(*first));
		}
	}

	template<class InIt, class T>
	T* uninitialized_move(InIt first, InIt last, T* out)
	{
		if constexpr (is_trivially_copyable_v<T> && is_pointer_v<InIt>)
		{
			const size_t n = size_t(last - first);
			if (n) memcpy(out, first, n * sizeof(T));
			return out + n;
		}
		else
		{
			for (; first != last; ++first, ++out) construct_at(out, move(*first));
			return out;
		}
	}

	template<class InIt, class T>
	T* uninitialized_copy(InIt first, InIt last, T* out)
	{
		if constexpr (is_trivially_copyable_v<T> && is_pointer_v<InIt>)
		{
			const size_t n = size_t(last - first);
			if (n) memcpy(out, first, n * sizeof(T));
			return out + n;
		}
		else
		{
			for (; first != last; ++first, ++out) construct_at(out, *first);
			return out;
		}
	}

	template<class T>
	struct default_delete
	{
		constexpr default_delete() noexcept = default;
		template<class U, enable_if_t<is_convertible_v<U*, T*>, int> = 0> default_delete(const default_delete<U>&) noexcept {}
		void operator()(T* p) const noexcept { delete p; }
	};
	template<class T>
	struct default_delete<T[]>
	{
		void operator()(T* p) const noexcept { delete[] p; }
	};

	template<class T, class D = default_delete<T>>
	class unique_ptr
	{
	public:
		typedef T element_type;
		typedef T* pointer;

		constexpr unique_ptr() noexcept : _ptr(nullptr), _deleter() {}
		constexpr unique_ptr(nullptr_t) noexcept : _ptr(nullptr), _deleter() {}
		explicit unique_ptr(T* p) noexcept : _ptr(p), _deleter() {}
		unique_ptr(T* p, D d) noexcept : _ptr(p), _deleter(move(d)) {}
		unique_ptr(unique_ptr&& o) noexcept : _ptr(o.release()), _deleter(move(o._deleter)) {}
		template<class U, class E, enable_if_t<is_convertible_v<U*, T*>, int> = 0>
		unique_ptr(unique_ptr<U, E>&& o) noexcept : _ptr(o.release()), _deleter(move(o.get_deleter())) {}
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;
		unique_ptr& operator=(unique_ptr&& o) noexcept
		{
			reset(o.release());
			_deleter = move(o._deleter);
			return *this;
		}
		template<class U, class E>
		unique_ptr& operator=(unique_ptr<U, E>&& o) noexcept
		{
			reset(o.release());
			return *this;
		}
		unique_ptr& operator=(nullptr_t) noexcept
		{
			reset();
			return *this;
		}
		~unique_ptr() { reset(); }

		T* get() const noexcept { return _ptr; }
		T& operator*() const noexcept { return *_ptr; }
		T* operator->() const noexcept { return _ptr; }
		explicit operator bool() const noexcept { return _ptr != nullptr; }
		D& get_deleter() noexcept { return _deleter; }
		T* release() noexcept { return exchange(_ptr, nullptr); }
		void reset(T* p = nullptr) noexcept
		{
			T* old = exchange(_ptr, p);
			if (old) _deleter(old);
		}
		bool operator==(nullptr_t) const noexcept { return _ptr == nullptr; }
		bool operator!=(nullptr_t) const noexcept { return _ptr != nullptr; }
	private:
		T* _ptr;
		[[no_unique_address]] D _deleter;
	};

	template<class T, class... A>
	unique_ptr<T> make_unique(A&&... a) { return unique_ptr<T>(new T(forward<A>(a)...)); }

	struct shared_control_block
	{
		long strong = 1;
		long weak = 1;
		virtual void destroy_object() noexcept = 0;
		virtual void destroy_self() noexcept = 0;
		virtual ~shared_control_block() = default;

		void add_strong() noexcept { ++strong; }
		void add_weak() noexcept { ++weak; }
		void release_strong() noexcept
		{
			if (--strong == 0)
			{
				destroy_object();
				release_weak();
			}
		}
		void release_weak() noexcept
		{
			if (--weak == 0) destroy_self();
		}
	};

	template<class T>
	struct shared_inplace_block final : shared_control_block
	{
		alignas(T) unsigned char storage[sizeof(T)];
		template<class... A> explicit shared_inplace_block(A&&... a) { construct_at(object(), forward<A>(a)...); }
		T* object() noexcept { return reinterpret_cast<T*>(storage); }
		void destroy_object() noexcept override { destroy_at(object()); }
		void destroy_self() noexcept override { delete this; }
	};

	template<class T>
	class weak_ptr;

	template<class T>
	class shared_ptr
	{
	public:
		constexpr shared_ptr() noexcept : _ptr(nullptr), _block(nullptr) {}
		constexpr shared_ptr(nullptr_t) noexcept : _ptr(nullptr), _block(nullptr) {}
		shared_ptr(const shared_ptr& o) noexcept : _ptr(o._ptr), _block(o._block) { if (_block) _block->add_strong(); }
		shared_ptr(shared_ptr&& o) noexcept : _ptr(exchange(o._ptr, nullptr)), _block(exchange(o._block, nullptr)) {}
		template<class U, enable_if_t<is_convertible_v<U*, T*>, int> = 0>
		shared_ptr(const shared_ptr<U>& o) noexcept : _ptr(o._ptr), _block(o._block) { if (_block) _block->add_strong(); }
		shared_ptr& operator=(shared_ptr o) noexcept
		{
			swap(_ptr, o._ptr);
			swap(_block, o._block);
			return *this;
		}
		~shared_ptr() { if (_block) _block->release_strong(); }

		T* get() const noexcept { return _ptr; }
		T& operator*() const noexcept { return *_ptr; }
		T* operator->() const noexcept { return _ptr; }
		explicit operator bool() const noexcept { return _ptr != nullptr; }
		long use_count() const noexcept { return _block ? _block->strong : 0; }
		void reset() noexcept { shared_ptr().swap_with(*this); }
		void swap_with(shared_ptr& o) noexcept
		{
			swap(_ptr, o._ptr);
			swap(_block, o._block);
		}
		bool operator==(const shared_ptr& o) const noexcept { return _ptr == o._ptr; }
		bool operator!=(const shared_ptr& o) const noexcept { return _ptr != o._ptr; }
	private:
		T* _ptr;
		shared_control_block* _block;
		shared_ptr(T* p, shared_control_block* b) noexcept : _ptr(p), _block(b) {}
		template<class U> friend class shared_ptr;
		template<class U> friend class weak_ptr;
		template<class U, class... A> friend shared_ptr<U> make_shared(A&&... a);
	};

	template<class T>
	class weak_ptr
	{
	public:
		constexpr weak_ptr() noexcept : _ptr(nullptr), _block(nullptr) {}
		weak_ptr(const shared_ptr<T>& s) noexcept : _ptr(s._ptr), _block(s._block) { if (_block) _block->add_weak(); }
		weak_ptr(const weak_ptr& o) noexcept : _ptr(o._ptr), _block(o._block) { if (_block) _block->add_weak(); }
		weak_ptr& operator=(weak_ptr o) noexcept
		{
			swap(_ptr, o._ptr);
			swap(_block, o._block);
			return *this;
		}
		~weak_ptr() { if (_block) _block->release_weak(); }
		bool expired() const noexcept { return !_block || _block->strong == 0; }
		shared_ptr<T> lock() const noexcept
		{
			if (expired()) return shared_ptr<T>();
			_block->add_strong();
			return shared_ptr<T>(_ptr, _block);
		}
	private:
		T* _ptr;
		shared_control_block* _block;
	};

	template<class T, class... A>
	shared_ptr<T> make_shared(A&&... a)
	{
		auto* block = new shared_inplace_block<T>(forward<A>(a)...);
		return shared_ptr<T>(block->object(), block);
	}

	// ---------------------------------------------------------------------------------------------- iterators

	struct input_iterator_tag {};
	struct forward_iterator_tag : input_iterator_tag {};
	struct bidirectional_iterator_tag : forward_iterator_tag {};
	struct random_access_iterator_tag : bidirectional_iterator_tag {};

	template<class It>
	struct iterator_traits
	{
		typedef typename It::value_type value_type;
		typedef typename It::difference_type difference_type;
		typedef typename It::iterator_category iterator_category;
		typedef typename It::reference reference;
	};
	template<class T>
	struct iterator_traits<T*>
	{
		typedef remove_cv_t<T> value_type;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;
		typedef T& reference;
	};

	template<class It>
	class reverse_iterator
	{
	public:
		typedef typename iterator_traits<It>::value_type value_type;
		typedef typename iterator_traits<It>::difference_type difference_type;
		typedef typename iterator_traits<It>::iterator_category iterator_category;
		typedef typename iterator_traits<It>::reference reference;

		constexpr reverse_iterator() = default;
		constexpr explicit reverse_iterator(It it) : _it(it) {}
		constexpr reference operator*() const { It tmp = _it; return *--tmp; }
		constexpr reverse_iterator& operator++() { --_it; return *this; }
		constexpr reverse_iterator operator++(int) { reverse_iterator t = *this; --_it; return t; }
		constexpr reverse_iterator& operator--() { ++_it; return *this; }
		constexpr bool operator==(const reverse_iterator& o) const { return _it == o._it; }
		constexpr bool operator!=(const reverse_iterator& o) const { return _it != o._it; }
		constexpr It base() const { return _it; }
	private:
		It _it{};
	};

	template<class It>
	constexpr typename iterator_traits<It>::difference_type distance(It first, It last)
	{
		if constexpr (is_base_of<random_access_iterator_tag, typename iterator_traits<It>::iterator_category>::value)
		{
			return last - first;
		}
		else
		{
			typename iterator_traits<It>::difference_type n = 0;
			for (; first != last; ++first) ++n;
			return n;
		}
	}

	template<class It, class D>
	constexpr void advance(It& it, D n)
	{
		if constexpr (is_base_of<random_access_iterator_tag, typename iterator_traits<It>::iterator_category>::value)
		{
			it += n;
		}
		else
		{
			for (; n > 0; --n) ++it;
		}
	}

	template<class It> constexpr It next(It it, typename iterator_traits<It>::difference_type n = 1) { advance(it, n); return it; }

	template<class C> constexpr auto begin(C& c) -> decltype(c.begin()) { return c.begin(); }
	template<class C> constexpr auto begin(const C& c) -> decltype(c.begin()) { return c.begin(); }
	template<class C> constexpr auto end(C& c) -> decltype(c.end()) { return c.end(); }
	template<class C> constexpr auto end(const C& c) -> decltype(c.end()) { return c.end(); }
	template<class T, size_t N> constexpr T* begin(T (&a)[N]) noexcept { return a; }
	template<class T, size_t N> constexpr T* end(T (&a)[N]) noexcept { return a + N; }
	template<class C> constexpr auto size(const C& c) -> decltype(c.size()) { return c.size(); }
	template<class T, size_t N> constexpr size_t size(const T (&)[N]) noexcept { return N; }

	// ---------------------------------------------------------------------------------------------- span / array

	template<class T>
	class span
	{
	public:
		typedef T element_type;
		typedef remove_cv_t<T> value_type;
		typedef T* iterator;

		constexpr span() noexcept : _data(nullptr), _size(0) {}
		constexpr span(T* data, size_t size) noexcept : _data(data), _size(size) {}
		template<size_t N> constexpr span(T (&a)[N]) noexcept : _data(a), _size(N) {}
		template<class C, enable_if_t<is_convertible_v<decltype(declval<C&>().data()), T*>, int> = 0>
		constexpr span(C& c) noexcept : _data(c.data()), _size(c.size()) {}

		constexpr T* data() const noexcept { return _data; }
		constexpr size_t size() const noexcept { return _size; }
		constexpr bool empty() const noexcept { return _size == 0; }
		constexpr T& operator[](size_t i) const noexcept { return _data[i]; }
		constexpr T* begin() const noexcept { return _data; }
		constexpr T* end() const noexcept { return _data + _size; }
		constexpr T& front() const noexcept { return _data[0]; }
		constexpr T& back() const noexcept { return _data[_size - 1]; }
		constexpr span subspan(size_t offset, size_t count = size_t(-1)) const noexcept
		{
			if (offset > _size) offset = _size;
			if (count > _size - offset) count = _size - offset;
			return span(_data + offset, count);
		}
		constexpr span first(size_t n) const noexcept { return subspan(0, n); }
		constexpr span last(size_t n) const noexcept { return subspan(_size - min(n, _size)); }
	private:
		T* _data;
		size_t _size;
	};

	template<class T, size_t N>
	struct array
	{
		T elements[N];

		typedef T value_type;
		typedef T* iterator;
		typedef const T* const_iterator;

		constexpr T& operator[](size_t i) noexcept { return elements[i]; }
		constexpr const T& operator[](size_t i) const noexcept { return elements[i]; }
		constexpr T* data() noexcept { return elements; }
		constexpr const T* data() const noexcept { return elements; }
		constexpr size_t size() const noexcept { return N; }
		constexpr T* begin() noexcept { return elements; }
		constexpr T* end() noexcept { return elements + N; }
		constexpr const T* begin() const noexcept { return elements; }
		constexpr const T* end() const noexcept { return elements + N; }
		constexpr void fill(const T& v) { for (auto& e : elements) e = v; }
		constexpr bool operator==(const array& o) const
		{
			for (size_t i = 0; i < N; ++i) if (!(elements[i] == o.elements[i])) return false;
			return true;
		}
	};

	// ---------------------------------------------------------------------------------------------- vector

	template<class T, class Alloc = allocator<T>>
	class vector
	{
	public:
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef reverse_iterator<T*> reverse_iterator_type;
		typedef size_t size_type;

		vector() noexcept = default;
		explicit vector(size_t n) { resize(n); }
		vector(size_t n, const T& v) { assign(n, v); }
		vector(initializer_list<T> list) { append(list.begin(), list.end()); }
		template<class It, enable_if_t<!is_integral_v<It>, int> = 0>
		vector(It first, It last) { append(first, last); }
		vector(const vector& o) { append(o.begin(), o.end()); }
		vector(vector&& o) noexcept
			: _data(exchange(o._data, nullptr)), _size(exchange(o._size, 0)), _capacity(exchange(o._capacity, 0)) {}
		vector& operator=(const vector& o)
		{
			if (this != &o)
			{
				clear();
				append(o.begin(), o.end());
			}
			return *this;
		}
		vector& operator=(vector&& o) noexcept
		{
			if (this != &o)
			{
				release();
				_data = exchange(o._data, nullptr);
				_size = exchange(o._size, 0);
				_capacity = exchange(o._capacity, 0);
			}
			return *this;
		}
		vector& operator=(initializer_list<T> list)
		{
			clear();
			append(list.begin(), list.end());
			return *this;
		}
		~vector() { release(); }

		T* data() noexcept { return _data; }
		const T* data() const noexcept { return _data; }
		size_t size() const noexcept { return _size; }
		size_t capacity() const noexcept { return _capacity; }
		bool empty() const noexcept { return _size == 0; }
		T& operator[](size_t i) noexcept { return _data[i]; }
		const T& operator[](size_t i) const noexcept { return _data[i]; }
		T& at(size_t i)
		{
			if (i >= _size) panic("vector index out of range");
			return _data[i];
		}
		const T& at(size_t i) const
		{
			if (i >= _size) panic("vector index out of range");
			return _data[i];
		}
		T& front() noexcept { return _data[0]; }
		const T& front() const noexcept { return _data[0]; }
		T& back() noexcept { return _data[_size - 1]; }
		const T& back() const noexcept { return _data[_size - 1]; }
		T* begin() noexcept { return _data; }
		T* end() noexcept { return _data + _size; }
		const T* begin() const noexcept { return _data; }
		const T* end() const noexcept { return _data + _size; }
		reverse_iterator<T*> rbegin() noexcept { return reverse_iterator<T*>(end()); }
		reverse_iterator<T*> rend() noexcept { return reverse_iterator<T*>(begin()); }

		void reserve(size_t n)
		{
			if (n <= _capacity) return;
			Alloc alloc;
			T* fresh = alloc.allocate(n);
			uninitialized_move(_data, _data + _size, fresh);
			destroy(_data, _data + _size);
			if (_data) alloc.deallocate(_data, _capacity);
			_data = fresh;
			_capacity = n;
		}
		void shrink_to_fit()
		{
			if (_size == _capacity) return;
			vector tmp;
			tmp.reserve(_size);
			tmp._size = size_t(uninitialized_move(_data, _data + _size, tmp._data) - tmp._data);
			*this = move(tmp);
		}
		void clear() noexcept
		{
			destroy(_data, _data + _size);
			_size = 0;
		}
		void resize(size_t n)
		{
			if (n < _size)
			{
				destroy(_data + n, _data + _size);
			}
			else
			{
				reserve(n);
				for (size_t i = _size; i < n; ++i) construct_at(_data + i);
			}
			_size = n;
		}
		void resize(size_t n, const T& v)
		{
			if (n < _size)
			{
				destroy(_data + n, _data + _size);
			}
			else
			{
				reserve(n);
				for (size_t i = _size; i < n; ++i) construct_at(_data + i, v);
			}
			_size = n;
		}
		void assign(size_t n, const T& v)
		{
			clear();
			resize(n, v);
		}
		template<class It>
		void append(It first, It last)
		{
			if constexpr (is_base_of<random_access_iterator_tag, typename iterator_traits<It>::iterator_category>::value)
			{
				const size_t n = size_t(last - first);
				reserve(_size + n);
				uninitialized_copy(first, last, _data + _size);
				_size += n;
			}
			else
			{
				for (; first != last; ++first) push_back(*first);
			}
		}
		void push_back(const T& v) { emplace_back(v); }
		void push_back(T&& v) { emplace_back(move(v)); }
		template<class... A>
		T& emplace_back(A&&... a)
		{
			if (_size == _capacity) grow(_size + 1);
			T* p = construct_at(_data + _size, forward<A>(a)...);
			++_size;
			return *p;
		}
		void pop_back() noexcept
		{
			--_size;
			destroy_at(_data + _size);
		}
		T* insert(const T* where, const T& v) { return emplace(where, v); }
		T* insert(const T* where, T&& v) { return emplace(where, move(v)); }
		template<class... A>
		T* emplace(const T* where, A&&... a)
		{
			const size_t index = size_t(where - _data);
			if (index == _size)
			{
				emplace_back(forward<A>(a)...);
				return _data + index;
			}
			T tmp(forward<A>(a)...);
			if (_size == _capacity) grow(_size + 1);
			construct_at(_data + _size, move(_data[_size - 1]));
			for (size_t i = _size - 1; i > index; --i) _data[i] = move(_data[i - 1]);
			_data[index] = move(tmp);
			++_size;
			return _data + index;
		}
		T* erase(const T* where) { return erase(where, where + 1); }
		T* erase(const T* first, const T* last)
		{
			T* f = _data + (first - _data);
			T* l = _data + (last - _data);
			if (f == l) return f;
			T* out = f;
			for (T* in = l; in != end(); ++in, ++out) *out = move(*in);
			destroy(out, end());
			_size = size_t(out - _data);
			return f;
		}
		void swap(vector& o) noexcept
		{
			ebs::swap(_data, o._data);
			ebs::swap(_size, o._size);
			ebs::swap(_capacity, o._capacity);
		}

		bool operator==(const vector& o) const
		{
			if (_size != o._size) return false;
			for (size_t i = 0; i < _size; ++i) if (!(_data[i] == o._data[i])) return false;
			return true;
		}
		bool operator!=(const vector& o) const { return !(*this == o); }
		bool operator<(const vector& o) const
		{
			const size_t n = min(_size, o._size);
			for (size_t i = 0; i < n; ++i)
			{
				if (_data[i] < o._data[i]) return true;
				if (o._data[i] < _data[i]) return false;
			}
			return _size < o._size;
		}
	private:
		T* _data = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;

		void grow(size_t minimum)
		{
			size_t n = _capacity ? _capacity * 2 : 4;
			if (n < minimum) n = minimum;
			reserve(n);
		}
		void release() noexcept
		{
			destroy(_data, _data + _size);
			if (_data) Alloc().deallocate(_data, _capacity);
			_data = nullptr;
			_size = _capacity = 0;
		}
	};

	// ---------------------------------------------------------------------------------------------- small vector

	template<class T, size_t N>
	class small_vector
	{
	public:
		typedef T value_type;
		typedef T* iterator;
		typedef const T* const_iterator;

		small_vector() noexcept : _data(inline_data()), _size(0), _capacity(N) {}
		small_vector(initializer_list<T> list) : small_vector()
		{
			for (auto& v : list) push_back(v);
		}
		small_vector(const small_vector& o) : small_vector()
		{
			reserve(o._size);
			uninitialized_copy(o.begin(), o.end(), _data);
			_size = o._size;
		}
		small_vector(small_vector&& o) noexcept : small_vector()
		{
			if (o.is_inline())
			{
				uninitialized_move(o.begin(), o.end(), _data);
				_size = o._size;
				o.clear();
			}
			else
			{
				_data = exchange(o._data, o.inline_data());
				_size = exchange(o._size, 0);
				_capacity = exchange(o._capacity, N);
			}
		}
		small_vector& operator=(small_vector o) noexcept
		{
			this->~small_vector();
			new (this) small_vector(move(o));
			return *this;
		}
		~small_vector()
		{
			destroy(begin(), end());
			if (!is_inline()) ::operator delete(_data);
		}

		size_t size() const noexcept { return _size; }
		bool empty() const noexcept { return _size == 0; }
		T* data() noexcept { return _data; }
		T* begin() noexcept { return _data; }
		T* end() noexcept { return _data + _size; }
		const T* begin() const noexcept { return _data; }
		const T* end() const noexcept { return _data + _size; }
		T& operator[](size_t i) noexcept { return _data[i]; }
		const T& operator[](size_t i) const noexcept { return _data[i]; }
		T& back() noexcept { return _data[_size - 1]; }
		void clear() noexcept
		{
			destroy(begin(), end());
			_size = 0;
		}
		void reserve(size_t n)
		{
			if (n <= _capacity) return;
			T* fresh = static_cast<T*>(::operator new(n * sizeof(T)));
			uninitialized_move(begin(), end(), fresh);
			destroy(begin(), end());
			if (!is_inline()) ::operator delete(_data);
			_data = fresh;
			_capacity = n;
		}
		template<class... A>
		T& emplace_back(A&&... a)
		{
			if (_size == _capacity) reserve(_capacity * 2);
			T* p = construct_at(_data + _size, forward<A>(a)...);
			++_size;
			return *p;
		}
		void push_back(const T& v) { emplace_back(v); }
		void push_back(T&& v) { emplace_back(move(v)); }
		void pop_back() noexcept { destroy_at(_data + --_size); }
	private:
		T* _data;
		size_t _size;
		size_t _capacity;
		alignas(T) unsigned char _storage[N * sizeof(T)];

		T* inline_data() noexcept { return reinterpret_cast<T*>(_storage); }
		bool is_inline() noexcept { return _data == inline_data(); }
	};

	// ---------------------------------------------------------------------------------------------- string

	template<class C>
	struct char_traits
	{
		static constexpr size_t length(const C* s) noexcept
		{
			size_t n = 0;
			while (s[n]) ++n;
			return n;
		}
		static constexpr int compare(const C* a, const C* b, size_t n) noexcept
		{
			for (size_t i = 0; i < n; ++i)
			{
				if (a[i] < b[i]) return -1;
				if (b[i] < a[i]) return 1;
			}
			return 0;
		}
		static constexpr const C* find(const C* s, size_t n, C c) noexcept
		{
			for (size_t i = 0; i < n; ++i) if (s[i] == c) return s + i;
			return nullptr;
		}
		static C* copy(C* d, const C* s, size_t n) noexcept { return static_cast<C*>(memcpy(d, s, n * sizeof(C))); }
		static C* move(C* d, const C* s, size_t n) noexcept { return static_cast<C*>(memmove(d, s, n * sizeof(C))); }
	};

	template<class C, class Traits = char_traits<C>>
	class basic_string_view
	{
	public:
		typedef C value_type;
		typedef const C* iterator;
		static constexpr size_t npos = size_t(-1);

		constexpr basic_string_view() noexcept : _data(nullptr), _size(0) {}
		constexpr basic_string_view(const C* s) noexcept : _data(s), _size(Traits::length(s)) {}
		constexpr basic_string_view(const C* s, size_t n) noexcept : _data(s), _size(n) {}

		constexpr const C* data() const noexcept { return _data; }
		constexpr size_t size() const noexcept { return _size; }
		constexpr size_t length() const noexcept { return _size; }
		constexpr bool empty() const noexcept { return _size == 0; }
		constexpr const C* begin() const noexcept { return _data; }
		constexpr const C* end() const noexcept { return _data + _size; }
		constexpr C operator[](size_t i) const noexcept { return _data[i]; }
		constexpr C front() const noexcept { return _data[0]; }
		constexpr C back() const noexcept { return _data[_size - 1]; }
		constexpr void remove_prefix(size_t n) noexcept { _data += n; _size -= n; }
		constexpr void remove_suffix(size_t n) noexcept { _size -= n; }
		constexpr basic_string_view substr(size_t pos, size_t n = npos) const noexcept
		{
			if (pos > _size) pos = _size;
			return basic_string_view(_data + pos, min(n, _size - pos));
		}
		constexpr int compare(basic_string_view o) const noexcept
		{
			int r = Traits::compare(_data, o._data, min(_size, o._size));
			if (r != 0) return r;
			return _size < o._size ? -1 : (_size > o._size ? 1 : 0);
		}
		constexpr size_t find(C c, size_t pos = 0) const noexcept
		{
			for (size_t i = pos; i < _size; ++i) if (_data[i] == c) return i;
			return npos;
		}
		constexpr size_t find(basic_string_view s, size_t pos = 0) const noexcept
		{
			if (s._size == 0) return pos <= _size ? pos : npos;
			for (size_t i = pos; i + s._size <= _size; ++i)
			{
				if (Traits::compare(_data + i, s._data, s._size) == 0) return i;
			}
			return npos;
		}
		constexpr size_t rfind(C c) const noexcept
		{
			for (size_t i = _size; i > 0; --i) if (_data[i - 1] == c) return i - 1;
			return npos;
		}
		constexpr size_t find_first_of(basic_string_view set, size_t pos = 0) const noexcept
		{
			for (size_t i = pos; i < _size; ++i) if (Traits::find(set._data, set._size, _data[i])) return i;
			return npos;
		}
		constexpr size_t find_first_not_of(basic_string_view set, size_t pos = 0) const noexcept
		{
			for (size_t i = pos; i < _size; ++i) if (!Traits::find(set._data, set._size, _data[i])) return i;
			return npos;
		}
		constexpr bool starts_with(basic_string_view p) const noexcept
		{
			return _size >= p._size && Traits::compare(_data, p._data, p._size) == 0;
		}
		constexpr bool ends_with(basic_string_view p) const noexcept
		{
			return _size >= p._size && Traits::compare(_data + _size - p._size, p._data, p._size) == 0;
		}
		constexpr bool operator==(basic_string_view o) const noexcept { return _size == o._size && compare(o) == 0; }
		constexpr bool operator!=(basic_string_view o) const noexcept { return !(*this == o); }
		constexpr bool operator<(basic_string_view o) const noexcept { return compare(o) < 0; }
	private:
		const C* _data;
		size_t _size;
	};

	typedef basic_string_view<char> string_view;

	template<class C, class Traits = char_traits<C>>
	class basic_string
	{
	public:
		typedef C value_type;
		typedef C* iterator;
		typedef const C* const_iterator;
		static constexpr size_t npos = size_t(-1);

		basic_string() noexcept { set_inline_size(0); }
		basic_string(const C* s) : basic_string(s, Traits::length(s)) {}
		basic_string(const C* s, size_t n) { init(s, n); }
		basic_string(size_t n, C c)
		{
			init(nullptr, n);
			for (size_t i = 0; i < n; ++i) data()[i] = c;
		}
		explicit basic_string(basic_string_view<C, Traits> v) { init(v.data(), v.size()); }
		basic_string(const basic_string& o) { init(o.data(), o.size()); }
		basic_string(basic_string&& o) noexcept
		{
			memcpy(static_cast<void*>(this), &o, sizeof(*this));
			o.set_inline_size(0);
		}
		basic_string& operator=(const basic_string& o)
		{
			if (this != &o) assign(o.data(), o.size());
			return *this;
		}
		basic_string& operator=(basic_string&& o) noexcept
		{
			if (this != &o)
			{
				release();
				memcpy(static_cast<void*>(this), &o, sizeof(*this));
				o.set_inline_size(0);
			}
			return *this;
		}
		basic_string& operator=(const C* s) { return assign(s, Traits::length(s)); }
		~basic_string() { release(); }

		const C* data() const noexcept { return is_heap() ? _heap.data : _inline.data; }
		C* data() noexcept { return is_heap() ? _heap.data : _inline.data; }
		const C* c_str() const noexcept { return data(); }
		size_t size() const noexcept { return is_heap() ? _heap.size : size_t(_inlineSize); }
		size_t length() const noexcept { return size(); }
		size_t capacity() const noexcept { return is_heap() ? _heap.capacity : InlineCapacity; }
		bool empty() const noexcept { return size() == 0; }
		C* begin() noexcept { return data(); }
		C* end() noexcept { return data() + size(); }
		const C* begin() const noexcept { return data(); }
		const C* end() const noexcept { return data() + size(); }
		C& operator[](size_t i) noexcept { return data()[i]; }
		const C& operator[](size_t i) const noexcept { return data()[i]; }
		C& back() noexcept { return data()[size() - 1]; }
		C back() const noexcept { return data()[size() - 1]; }
		operator basic_string_view<C, Traits>() const noexcept { return basic_string_view<C, Traits>(data(), size()); }
		basic_string_view<C, Traits> view() const noexcept { return basic_string_view<C, Traits>(data(), size()); }

		void reserve(size_t n)
		{
			if (n <= capacity()) return;
			C* fresh = static_cast<C*>(::operator new((n + 1) * sizeof(C)));
			const size_t s = size();
			Traits::copy(fresh, data(), s + 1);
			release();
			_heap.data = fresh;
			_heap.size = s;
			_heap.capacity = n;
			_inlineSize = HeapMarker;
		}
		void clear() noexcept { set_size(0); }
		void resize(size_t n, C c = C())
		{
			const size_t s = size();
			if (n > s)
			{
				reserve(n);
				for (size_t i = s; i < n; ++i) data()[i] = c;
			}
			set_size(n);
		}
		basic_string& assign(const C* s, size_t n)
		{
			if (n > capacity()) reserve(n);
			Traits::move(data(), s, n);
			set_size(n);
			return *this;
		}
		basic_string& append(const C* s, size_t n)
		{
			const size_t old = size();
			if (old + n > capacity()) reserve(max(old + n, capacity() * 2));
			Traits::copy(data() + old, s, n);
			set_size(old + n);
			return *this;
		}
		basic_string& append(basic_string_view<C, Traits> v) { return append(v.data(), v.size()); }
		basic_string& append(size_t n, C c)
		{
			const size_t old = size();
			if (old + n > capacity()) reserve(max(old + n, capacity() * 2));
			for (size_t i = 0; i < n; ++i) data()[old + i] = c;
			set_size(old + n);
			return *this;
		}
		void push_back(C c)
		{
			const size_t old = size();
			if (old + 1 > capacity()) reserve(capacity() * 2 + 1);
			data()[old] = c;
			set_size(old + 1);
		}
		void pop_back() noexcept { set_size(size() - 1); }
		basic_string& operator+=(const basic_string& o) { return append(o.data(), o.size()); }
		basic_string& operator+=(basic_string_view<C, Traits> v) { return append(v.data(), v.size()); }
		basic_string& operator+=(const C* s) { return append(s, Traits::length(s)); }
		basic_string& operator+=(C c)
		{
			push_back(c);
			return *this;
		}
		basic_string substr(size_t pos, size_t n = npos) const { return basic_string(view().substr(pos, n)); }
		size_t find(C c, size_t pos = 0) const noexcept { return view().find(c, pos); }
		size_t find(basic_string_view<C, Traits> s, size_t pos = 0) const noexcept { return view().find(s, pos); }
		bool starts_with(basic_string_view<C, Traits> p) const noexcept { return view().starts_with(p); }
		bool ends_with(basic_string_view<C, Traits> p) const noexcept { return view().ends_with(p); }
		int compare(basic_string_view<C, Traits> o) const noexcept { return view().compare(o); }

		friend bool operator==(const basic_string& a, const basic_string& b) noexcept { return a.view() == b.view(); }
		friend bool operator!=(const basic_string& a, const basic_string& b) noexcept { return a.view() != b.view(); }
		friend bool operator<(const basic_string& a, const basic_string& b) noexcept { return a.view() < b.view(); }
		friend bool operator==(const basic_string& a, const C* b) noexcept { return a.view() == basic_string_view<C, Traits>(b); }
		friend basic_string operator+(basic_string a, basic_string_view<C, Traits> b)
		{
			a.append(b);
			return a;
		}
		friend basic_string operator+(basic_string a, const C* b)
		{
			a += b;
			return a;
		}
		friend basic_string operator+(basic_string a, C b)
		{
			a.push_back(b);
			return a;
		}
	private:
		static constexpr size_t InlineCapacity = (3 * sizeof(void*)) / sizeof(C) - 1;
		static constexpr unsigned char HeapMarker = 0xff;

		struct Heap
		{
			C* data;
			size_t size;
			size_t capacity;
		};
		struct Inline
		{
			C data[InlineCapacity + 1];
		};
		union
		{
			Heap _heap;
			Inline _inline;
		};
		unsigned char _inlineSize;    // HeapMarker when the characters are on the heap

		bool is_heap() const noexcept { return _inlineSize == HeapMarker; }
		void set_inline_size(size_t n) noexcept
		{
			_inlineSize = (unsigned char) n;
			_inline.data[n] = C();
		}
		void set_size(size_t n) noexcept
		{
			if (is_heap())
			{
				_heap.size = n;
				_heap.data[n] = C();
			}
			else
			{
				set_inline_size(n);
			}
		}
		void init(const C* s, size_t n)
		{
			if (n <= InlineCapacity)
			{
				if (s) Traits::copy(_inline.data, s, n);
				set_inline_size(n);
			}
			else
			{
				_heap.data = static_cast<C*>(::operator new((n + 1) * sizeof(C)));
				if (s) Traits::copy(_heap.data, s, n);
				_heap.size = n;
				_heap.capacity = n;
				_heap.data[n] = C();
				_inlineSize = HeapMarker;
			}
		}
		void release() noexcept
		{
			if (is_heap()) ::operator delete(_heap.data);
		}
	};

	typedef basic_string<char> string;

	template<class C, class Traits>
	struct hash<basic_string_view<C, Traits>, void>
	{
		size_t operator()(basic_string_view<C, Traits> s) const noexcept { return size_t(hash_bytes(s.data(), s.size() * sizeof(C))); }
	};
	template<class C, class Traits>
	struct hash<basic_string<C, Traits>, void>
	{
		size_t operator()(const basic_string<C, Traits>& s) const noexcept { return size_t(hash_bytes(s.data(), s.size() * sizeof(C))); }
	};

	inline namespace literals
	{
		constexpr string_view operator""_sv(const char* s, size_t n) noexcept { return string_view(s, n); }
	}
}

namespace ebs
{
	// ---------------------------------------------------------------------------------------------- optional

	struct nullopt_t { explicit constexpr nullopt_t(int) {} };
	inline constexpr nullopt_t nullopt{ 0 };

	template<class T, bool Trivial = is_trivially_destructible_v<T>>
	struct optional_storage
	{
		union { char _empty; T _value; };
		bool _engaged;
		constexpr optional_storage() noexcept : _empty(), _engaged(false) {}
		~optional_storage() { if (_engaged) _value.~T(); }
		void reset() noexcept
		{
			if (_engaged) _value.~T();
			_engaged = false;
		}
	};
	template<class T>
	struct optional_storage<T, true>
	{
		union { char _empty; T _value; };
		bool _engaged;
		constexpr optional_storage() noexcept : _empty(), _engaged(false) {}
		void reset() noexcept { _engaged = false; }
	};

	template<class T>
	class optional : private optional_storage<T>
	{
		typedef optional_storage<T> base;
	public:
		typedef T value_type;

		constexpr optional() noexcept = default;
		constexpr optional(nullopt_t) noexcept {}
		optional(const optional& o) : base() { if (o._engaged) emplace(o._value); }
		optional(optional&& o) noexcept(is_nothrow_move_constructible_v<T>) : base() { if (o._engaged) emplace(move(o._value)); }
		template<class U = T, enable_if_t<is_constructible_v<T, U&&> && !is_same_v<remove_cvref_t<U>, optional>
			&& !is_same_v<remove_cvref_t<U>, nullopt_t>, int> = 0>
		optional(U&& v) : base() { emplace(forward<U>(v)); }
		template<class... A>
		explicit optional(in_place_t, A&&... a) : base() { emplace(forward<A>(a)...); }

		optional& operator=(nullopt_t) noexcept
		{
			this->reset();
			return *this;
		}
		optional& operator=(const optional& o)
		{
			if (o._engaged)
			{
				if (this->_engaged) this->_value = o._value;
				else emplace(o._value);
			}
			else
			{
				this->reset();
			}
			return *this;
		}
		optional& operator=(optional&& o) noexcept
		{
			if (o._engaged)
			{
				if (this->_engaged) this->_value = move(o._value);
				else emplace(move(o._value));
			}
			else
			{
				this->reset();
			}
			return *this;
		}
		template<class U = T, enable_if_t<!is_same_v<remove_cvref_t<U>, optional>, int> = 0>
		optional& operator=(U&& v)
		{
			if (this->_engaged) this->_value = forward<U>(v);
			else emplace(forward<U>(v));
			return *this;
		}

		template<class... A>
		T& emplace(A&&... a)
		{
			this->reset();
			construct_at(addressof(this->_value), forward<A>(a)...);
			this->_engaged = true;
			return this->_value;
		}

		using base::reset;
		constexpr bool has_value() const noexcept { return this->_engaged; }
		constexpr explicit operator bool() const noexcept { return this->_engaged; }
		T& value() &
		{
			if (!this->_engaged) panic("bad optional access");
			return this->_value;
		}
		const T& value() const&
		{
			if (!this->_engaged) panic("bad optional access");
			return this->_value;
		}
		T&& value() &&
		{
			if (!this->_engaged) panic("bad optional access");
			return move(this->_value);
		}
		T& operator*() & noexcept { return this->_value; }
		const T& operator*() const& noexcept { return this->_value; }
		T* operator->() noexcept { return addressof(this->_value); }
		const T* operator->() const noexcept { return addressof(this->_value); }
		template<class U> T value_or(U&& def) const& { return this->_engaged ? this->_value : static_cast<T>(forward<U>(def)); }

		template<class F>
		auto and_then(F&& f) const& -> remove_cvref_t<invoke_result_t<F, const T&>>
		{
			if (this->_engaged) return forward<F>(f)(this->_value);
			return {};
		}
		template<class F>
		auto transform(F&& f) const& -> optional<remove_cvref_t<invoke_result_t<F, const T&>>>
		{
			if (this->_engaged) return optional<remove_cvref_t<invoke_result_t<F, const T&>>>(forward<F>(f)(this->_value));
			return nullopt;
		}

		friend bool operator==(const optional& a, const optional& b)
		{
			if (a._engaged != b._engaged) return false;
			return !a._engaged || a._value == b._value;
		}
		friend bool operator!=(const optional& a, const optional& b) { return !(a == b); }
	};

	template<class T> optional<decay_t<T>> make_optional(T&& v) { return optional<decay_t<T>>(forward<T>(v)); }

	// ---------------------------------------------------------------------------------------------- variant

	template<class... Ts> class variant;

	template<class T> struct variant_size;
	template<class... Ts> struct variant_size<variant<Ts...>> : integral_constant<size_t, sizeof...(Ts)> {};

	inline constexpr size_t variant_npos = size_t(-1);

	template<class T, class... Ts>
	struct best_alternative
	{
		// Exact match first, then the first alternative constructible from T.
		static constexpr size_t exact()
		{
			constexpr bool matches[] = { is_same_v<remove_cvref_t<T>, Ts>... };
			for (size_t i = 0; i < sizeof...(Ts); ++i) if (matches[i]) return i;
			return variant_npos;
		}
		static constexpr size_t convertible()
		{
			constexpr bool matches[] = { is_constructible_v<Ts, T>... };
			for (size_t i = 0; i < sizeof...(Ts); ++i) if (matches[i]) return i;
			return variant_npos;
		}
		static constexpr size_t value = exact() != variant_npos ? exact() : convertible();
	};

	template<class... Ts>
	class variant
	{
		static_assert(sizeof...(Ts) > 0, "variant must have alternatives");
		static constexpr size_t Size = static_max<sizeof(Ts)...>::value;
		static constexpr size_t Align = static_max<alignof(Ts)...>::value;
	public:
		variant() noexcept(is_nothrow_constructible<type_at_t<0, Ts...>>::value) { construct<0>(); }
		variant(const variant& o) { o.visit_index([&](auto i) { this->construct<decltype(i)::value>(o.template get_unchecked<decltype(i)::value>()); }); }
		variant(variant&& o) noexcept
		{
			o.visit_index([&](auto i) { this->construct<decltype(i)::value>(move(o.template get_unchecked<decltype(i)::value>())); });
		}
		template<class T, size_t I = best_alternative<T, Ts...>::value,
			enable_if_t<!is_same_v<remove_cvref_t<T>, variant> && I != variant_npos, int> = 0>
		variant(T&& v) { construct<I>(forward<T>(v)); }
		template<class T, class... A>
		explicit variant(in_place_type_t<T>, A&&... a) { construct<index_of<T, Ts...>::value>(forward<A>(a)...); }
		template<size_t I, class... A>
		explicit variant(in_place_index_t<I>, A&&... a) { construct<I>(forward<A>(a)...); }

		variant& operator=(const variant& o)
		{
			if (this != &o)
			{
				destroy_current();
				o.visit_index([&](auto i) { this->construct<decltype(i)::value>(o.template get_unchecked<decltype(i)::value>()); });
			}
			return *this;
		}
		variant& operator=(variant&& o) noexcept
		{
			if (this != &o)
			{
				destroy_current();
				o.visit_index([&](auto i) { this->construct<decltype(i)::value>(move(o.template get_unchecked<decltype(i)::value>())); });
			}
			return *this;
		}
		template<class T, size_t I = best_alternative<T, Ts...>::value,
			enable_if_t<!is_same_v<remove_cvref_t<T>, variant> && I != variant_npos, int> = 0>
		variant& operator=(T&& v)
		{
			if (_index == I)
			{
				get_unchecked<I>() = forward<T>(v);
			}
			else
			{
				destroy_current();
				construct<I>(forward<T>(v));
			}
			return *this;
		}
		~variant() { destroy_current(); }

		constexpr size_t index() const noexcept { return _index; }
		template<class T> constexpr bool holds() const noexcept { return _index == index_of<T, Ts...>::value; }

		template<class T, class... A>
		T& emplace(A&&... a)
		{
			destroy_current();
			construct<index_of<T, Ts...>::value>(forward<A>(a)...);
			return get_unchecked<index_of<T, Ts...>::value>();
		}

		template<size_t I> type_at_t<I, Ts...>& get_unchecked() noexcept { return *reinterpret_cast<type_at_t<I, Ts...>*>(_storage); }
		template<size_t I> const type_at_t<I, Ts...>& get_unchecked() const noexcept
		{
			return *reinterpret_cast<const type_at_t<I, Ts...>*>(_storage);
		}

		template<class F>
		void visit_index(F&& f) const { visit_index_impl(f, index_sequence_for<Ts...>{}); }

		bool operator==(const variant& o) const
		{
			if (_index != o._index) return false;
			bool equal = false;
			visit_index([&](auto i) { equal = get_unchecked<decltype(i)::value>() == o.template get_unchecked<decltype(i)::value>(); });
			return equal;
		}
		bool operator!=(const variant& o) const { return !(*this == o); }
	private:
		alignas(Align) unsigned char _storage[Size];
		size_t _index = variant_npos;

		template<size_t I, class... A>
		void construct(A&&... a)
		{
			construct_at(reinterpret_cast<type_at_t<I, Ts...>*>(_storage), forward<A>(a)...);
			_index = I;
		}
		void destroy_current() noexcept
		{
			if (_index == variant_npos) return;
			visit_index([&](auto i) { destroy_at(&this->get_unchecked<decltype(i)::value>()); });
			_index = variant_npos;
		}
		template<class F, size_t... Is>
		void visit_index_impl(F& f, index_sequence<Is...>) const
		{
			(void) ((_index == Is ? (f(integral_constant<size_t, Is>{}), true) : false) || ...);
		}
	};

	template<class T, class... Ts>
	T& get(variant<Ts...>& v)
	{
		if (!v.template holds<T>()) panic("bad variant access");
		return v.template get_unchecked<index_of<T, Ts...>::value>();
	}
	template<class T, class... Ts>
	const T& get(const variant<Ts...>& v)
	{
		if (!v.template holds<T>()) panic("bad variant access");
		return v.template get_unchecked<index_of<T, Ts...>::value>();
	}
	template<class T, class... Ts>
	T* get_if(variant<Ts...>* v) noexcept
	{
		return v && v->template holds<T>() ? &v->template get_unchecked<index_of<T, Ts...>::value>() : nullptr;
	}
	template<class T, class... Ts>
	const T* get_if(const variant<Ts...>* v) noexcept
	{
		return v && v->template holds<T>() ? &v->template get_unchecked<index_of<T, Ts...>::value>() : nullptr;
	}
	template<class T, class... Ts>
	constexpr bool holds_alternative(const variant<Ts...>& v) noexcept { return v.template holds<T>(); }

	template<class F, class... Ts>
	decltype(auto) visit(F&& f, variant<Ts...>& v)
	{
		typedef invoke_result_t<F, type_at_t<0, Ts...>&> R;
		if constexpr (is_same_v<R, void>)
		{
			v.visit_index([&](auto i) { f(v.template get_unchecked<decltype(i)::value>()); });
		}
		else
		{
			optional<R> result;
			v.visit_index([&](auto i) { result.emplace(f(v.template get_unchecked<decltype(i)::value>())); });
			return move(*result);
		}
	}
	template<class F, class... Ts>
	decltype(auto) visit(F&& f, const variant<Ts...>& v)
	{
		typedef invoke_result_t<F, const type_at_t<0, Ts...>&> R;
		if constexpr (is_same_v<R, void>)
		{
			v.visit_index([&](auto i) { f(v.template get_unchecked<decltype(i)::value>()); });
		}
		else
		{
			optional<R> result;
			v.visit_index([&](auto i) { result.emplace(f(v.template get_unchecked<decltype(i)::value>())); });
			return move(*result);
		}
	}

	template<class... Fs> struct overloaded : Fs... { using Fs::operator()...; };
	template<class... Fs> overloaded(Fs...) -> overloaded<Fs...>;

	struct monostate
	{
		constexpr bool operator==(monostate) const noexcept { return true; }
	};

	// ---------------------------------------------------------------------------------------------- function

	template<class Sig> class function;

	template<class R, class... A>
	class function<R(A...)>
	{
		struct callable_base
		{
			virtual R invoke(A... a) = 0;
			virtual callable_base* clone_into(void* buffer) const = 0;
			virtual callable_base* clone() const = 0;
			virtual ~callable_base() = default;
		};
		template<class F>
		struct callable final : callable_base
		{
			F f;
			template<class G> explicit callable(G&& g) : f(forward<G>(g)) {}
			R invoke(A... a) override { return f(forward<A>(a)...); }
			callable_base* clone_into(void* buffer) const override { return new (buffer) callable(f); }
			callable_base* clone() const override { return new callable(f); }
		};
		static constexpr size_t InlineSize = 4 * sizeof(void*);
	public:
		function() noexcept : _callable(nullptr) {}
		function(nullptr_t) noexcept : _callable(nullptr) {}
		template<class F, enable_if_t<!is_same_v<decay_t<F>, function>, int> = 0>
		function(F&& f)
		{
			typedef callable<decay_t<F>> C;
			if constexpr (sizeof(C) <= InlineSize && alignof(C) <= alignof(void*))
			{
				_callable = new (_buffer) C(forward<F>(f));
			}
			else
			{
				_callable = new C(forward<F>(f));
			}
		}
		function(const function& o) : _callable(nullptr)
		{
			if (o._callable) _callable = o.is_inline() ? o._callable->clone_into(_buffer) : o._callable->clone();
		}
		function(function&& o) noexcept : _callable(nullptr)
		{
			if (!o._callable) return;
			if (o.is_inline())
			{
				_callable = o._callable->clone_into(_buffer);
				o.reset();
			}
			else
			{
				_callable = exchange(o._callable, nullptr);
			}
		}
		function& operator=(function o) noexcept
		{
			reset();
			new (this) function(move(o));
			return *this;
		}
		~function() { reset(); }

		explicit operator bool() const noexcept { return _callable != nullptr; }
		R operator()(A... a) const
		{
			if (!_callable) panic("bad function call");
			return _callable->invoke(forward<A>(a)...);
		}
	private:
		alignas(void*) unsigned char _buffer[InlineSize];
		callable_base* _callable;

		bool is_inline() const noexcept { return static_cast<const void*>(_callable) == static_cast<const void*>(_buffer); }
		void reset() noexcept
		{
			if (!_callable) return;
			if (is_inline()) _callable->~callable_base();
			else delete _callable;
			_callable = nullptr;
		}
	};

	template<class Sig> class function_ref;
	template<class R, class... A>
	class function_ref<R(A...)>
	{
	public:
		template<class F, enable_if_t<!is_same_v<decay_t<F>, function_ref>, int> = 0>
		function_ref(F&& f) noexcept
			: _object(const_cast<void*>(static_cast<const void*>(addressof(f)))),
			  _call([](void* o, A... a) -> R { return (*static_cast<remove_reference_t<F>*>(o))(forward<A>(a)...); }) {}
		R operator()(A... a) const { return _call(_object, forward<A>(a)...); }
	private:
		void* _object;
		R (*_call)(void*, A...);
	};

	// ---------------------------------------------------------------------------------------------- algorithms

	template<class It, class F> constexpr F for_each(It first, It last, F f)
	{
		for (; first != last; ++first) f(*first);
		return f;
	}
	template<class It, class T> constexpr It find(It first, It last, const T& v)
	{
		for (; first != last; ++first) if (*first == v) return first;
		return last;
	}
	template<class It, class P> constexpr It find_if(It first, It last, P p)
	{
		for (; first != last; ++first) if (p(*first)) return first;
		return last;
	}
	template<class It, class P> constexpr It find_if_not(It first, It last, P p)
	{
		for (; first != last; ++first) if (!p(*first)) return first;
		return last;
	}
	template<class It, class P> constexpr bool all_of(It first, It last, P p) { return find_if_not(first, last, p) == last; }
	template<class It, class P> constexpr bool any_of(It first, It last, P p) { return find_if(first, last, p) != last; }
	template<class It, class P> constexpr bool none_of(It first, It last, P p) { return find_if(first, last, p) == last; }
	template<class It, class P> constexpr size_t count_if(It first, It last, P p)
	{
		size_t n = 0;
		for (; first != last; ++first) if (p(*first)) ++n;
		return n;
	}
	template<class It, class T> constexpr size_t count(It first, It last, const T& v)
	{
		return count_if(first, last, [&](const auto& e) { return e == v; });
	}
	template<class It, class Out, class F> constexpr Out transform(It first, It last, Out out, F f)
	{
		for (; first != last; ++first, ++out) *out = f(*first);
		return out;
	}
	template<class It1, class It2, class Out, class F> constexpr Out transform(It1 first1, It1 last1, It2 first2, Out out, F f)
	{
		for (; first1 != last1; ++first1, ++first2, ++out) *out = f(*first1, *first2);
		return out;
	}
	template<class It, class Out> constexpr Out copy(It first, It last, Out out)
	{
		for (; first != last; ++first, ++out) *out = *first;
		return out;
	}
	template<class It, class Out, class P> constexpr Out copy_if(It first, It last, Out out, P p)
	{
		for (; first != last; ++first) if (p(*first)) { *out = *first; ++out; }
		return out;
	}
	template<class It, class T> constexpr void fill(It first, It last, const T& v)
	{
		for (; first != last; ++first) *first = v;
	}
	template<class It, class T> constexpr void iota(It first, It last, T v)
	{
		for (; first != last; ++first, ++v) *first = v;
	}
	template<class It, class T, class Op = plus<>> constexpr T accumulate(It first, It last, T init, Op op = Op())
	{
		for (; first != last; ++first) init = op(move(init), *first);
		return init;
	}
	template<class It1, class It2, class T> constexpr T inner_product(It1 first1, It1 last1, It2 first2, T init)
	{
		for (; first1 != last1; ++first1, ++first2) init = init + *first1 * *first2;
		return init;
	}
	template<class It, class Out> constexpr Out partial_sum(It first, It last, Out out)
	{
		if (first == last) return out;
		auto sum = *first;
		*out = sum;
		for (++first, ++out; first != last; ++first, ++out)
		{
			sum = sum + *first;
			*out = sum;
		}
		return out;
	}
	template<class It, class P> constexpr It remove_if(It first, It last, P p)
	{
		first = find_if(first, last, p);
		if (first == last) return first;
		for (It i = first; ++i != last;)
		{
			if (!p(*i))
			{
				*first = move(*i);
				++first;
			}
		}
		return first;
	}
	template<class It> constexpr void reverse(It first, It last)
	{
		while (first != last && first != --last)
		{
			swap(*first, *last);
			++first;
		}
	}
	template<class It, class C = less<>> constexpr It unique(It first, It last, C eq = C())
	{
		if (first == last) return last;
		It result = first;
		while (++first != last)
		{
			if (!(*result == *first)) *++result = move(*first);
		}
		(void) eq;
		return ++result;
	}
	template<class It, class C = less<>> constexpr It min_element(It first, It last, C comp = C())
	{
		if (first == last) return last;
		It best = first;
		for (++first; first != last; ++first) if (comp(*first, *best)) best = first;
		return best;
	}
	template<class It, class C = less<>> constexpr It max_element(It first, It last, C comp = C())
	{
		if (first == last) return last;
		It best = first;
		for (++first; first != last; ++first) if (comp(*best, *first)) best = first;
		return best;
	}
	template<class It, class T, class C = less<>> constexpr It lower_bound(It first, It last, const T& v, C comp = C())
	{
		auto n = distance(first, last);
		while (n > 0)
		{
			auto half = n / 2;
			It mid = next(first, half);
			if (comp(*mid, v))
			{
				first = ++mid;
				n -= half + 1;
			}
			else
			{
				n = half;
			}
		}
		return first;
	}
	template<class It, class T, class C = less<>> constexpr It upper_bound(It first, It last, const T& v, C comp = C())
	{
		auto n = distance(first, last);
		while (n > 0)
		{
			auto half = n / 2;
			It mid = next(first, half);
			if (!comp(v, *mid))
			{
				first = ++mid;
				n -= half + 1;
			}
			else
			{
				n = half;
			}
		}
		return first;
	}
	template<class It, class T, class C = less<>> constexpr bool binary_search(It first, It last, const T& v, C comp = C())
	{
		first = lower_bound(first, last, v, comp);
		return first != last && !comp(v, *first);
	}
	template<class It1, class It2> constexpr bool equal(It1 first1, It1 last1, It2 first2)
	{
		for (; first1 != last1; ++first1, ++first2) if (!(*first1 == *first2)) return false;
		return true;
	}
	template<class It, class P> constexpr It partition(It first, It last, P p)
	{
		first = find_if_not(first, last, p);
		if (first == last) return first;
		for (It i = next(first); i != last; ++i)
		{
			if (p(*i))
			{
				swap(*i, *first);
				++first;
			}
		}
		return first;
	}

	template<class It, class C>
	constexpr void insertion_sort(It first, It last, C comp)
	{
		if (first == last) return;
		for (It i = first + 1; i != last; ++i)
		{
			auto v = move(*i);
			It j = i;
			while (j != first && comp(v, *(j - 1)))
			{
				*j = move(*(j - 1));
				--j;
			}
			*j = move(v);
		}
	}

	template<class It, class C>
	constexpr void sift_down(It first, ptrdiff_t start, ptrdiff_t n, C comp)
	{
		ptrdiff_t root = start;
		while (2 * root + 1 < n)
		{
			ptrdiff_t child = 2 * root + 1;
			if (child + 1 < n && comp(first[child], first[child + 1])) ++child;
			if (!comp(first[root], first[child])) return;
			swap(first[root], first[child]);
			root = child;
		}
	}

	template<class It, class C = less<>>
	constexpr void make_heap(It first, It last, C comp = C())
	{
		const ptrdiff_t n = last - first;
		for (ptrdiff_t start = n / 2 - 1; start >= 0; --start) sift_down(first, start, n, comp);
	}

	template<class It, class C = less<>>
	constexpr void sort_heap(It first, It last, C comp = C())
	{
		for (ptrdiff_t n = last - first; n > 1; --n)
		{
			swap(first[0], first[n - 1]);
			sift_down(first, 0, n - 1, comp);
		}
	}

	template<class It, class C>
	constexpr It median_of_three(It a, It b, It c, C comp)
	{
		if (comp(*a, *b))
		{
			if (comp(*b, *c)) return b;
			return comp(*a, *c) ? c : a;
		}
		if (comp(*a, *c)) return a;
		return comp(*b, *c) ? c : b;
	}

	template<class It, class C>
	constexpr void introsort(It first, It last, int depth, C comp)
	{
		while (last - first > 16)
		{
			if (depth-- == 0)
			{
				make_heap(first, last, comp);
				sort_heap(first, last, comp);
				return;
			}
			It mid = first + (last - first) / 2;
			auto pivot = *median_of_three(first, mid, last - 1, comp);
			It l = first, r = last - 1;
			while (true)
			{
				while (comp(*l, pivot)) ++l;
				while (comp(pivot, *r)) --r;
				if (!(l < r)) break;
				swap(*l, *r);
				++l;
				--r;
			}
			It split = r + 1;
			if (split - first < last - split)
			{
				introsort(first, split, depth, comp);
				first = split;
			}
			else
			{
				introsort(split, last, depth, comp);
				last = split;
			}
		}
		insertion_sort(first, last, comp);
	}

	template<class It, class C = less<>>
	constexpr void sort(It first, It last, C comp = C())
	{
		int depth = 0;
		for (auto n = last - first; n > 1; n >>= 1) depth += 2;
		introsort(first, last, depth, comp);
	}

	template<class It, class C = less<>>
	void stable_sort(It first, It last, C comp = C())
	{
		typedef typename iterator_traits<It>::value_type T;
		const ptrdiff_t n = last - first;
		if (n < 2) return;
		vector<T> buffer;
		buffer.reserve(size_t(n));
		for (ptrdiff_t width = 1; width < n; width *= 2)
		{
			for (ptrdiff_t lo = 0; lo < n - width; lo += 2 * width)
			{
				const ptrdiff_t mid = lo + width;
				const ptrdiff_t hi = min(lo + 2 * width, n);
				buffer.clear();
				ptrdiff_t i = lo, j = mid;
				while (i < mid && j < hi) buffer.push_back(comp(first[j], first[i]) ? move(first[j++]) : move(first[i++]));
				while (i < mid) buffer.push_back(move(first[i++]));
				while (j < hi) buffer.push_back(move(first[j++]));
				for (ptrdiff_t k = 0; k < hi - lo; ++k) first[lo + k] = move(buffer[size_t(k)]);
			}
		}
	}

	template<class It, class C = less<>>
	constexpr void nth_element(It first, It nth, It last, C comp = C())
	{
		while (last - first > 3)
		{
			It mid = first + (last - first) / 2;
			auto pivot = *median_of_three(first, mid, last - 1, comp);
			It split = partition(first, last, [&](const auto& v) { return comp(v, pivot); });
			if (split == first) split = partition(first, last, [&](const auto& v) { return !comp(pivot, v); });
			if (nth < split) last = split;
			else first = split;
		}
		insertion_sort(first, last, comp);
	}

	// ---------------------------------------------------------------------------------------------- hash map

	template<class K, class V, class H = hash<K>, class E = equal_to<>>
	class hash_map
	{
		enum class slot_state : uint8_t { empty, full, deleted };
	public:
		typedef pair<const K, V> value_type;

		class iterator
		{
		public:
			typedef pair<const K, V> value_type;
			typedef ptrdiff_t difference_type;
			typedef forward_iterator_tag iterator_category;
			typedef value_type& reference;

			iterator(const hash_map* m, size_t i) : _map(m), _index(i) { skip(); }
			value_type& operator*() const { return *_map->slot(_index); }
			value_type* operator->() const { return _map->slot(_index); }
			iterator& operator++()
			{
				++_index;
				skip();
				return *this;
			}
			bool operator==(const iterator& o) const { return _index == o._index; }
			bool operator!=(const iterator& o) const { return _index != o._index; }
		private:
			const hash_map* _map;
			size_t _index;
			void skip()
			{
				while (_index < _map->_capacity && _map->_states[_index] != slot_state::full) ++_index;
			}
		};

		hash_map() = default;
		hash_map(initializer_list<pair<K, V>> list)
		{
			for (auto& kv : list) insert_or_assign(kv.first, kv.second);
		}
		hash_map(const hash_map& o)
		{
			reserve(o._size);
			for (auto& kv : o) insert_or_assign(kv.first, kv.second);
		}
		hash_map(hash_map&& o) noexcept
			: _slots(exchange(o._slots, nullptr)), _states(exchange(o._states, nullptr)), _capacity(exchange(o._capacity, 0)),
			  _size(exchange(o._size, 0)), _tombstones(exchange(o._tombstones, 0)) {}
		hash_map& operator=(hash_map o) noexcept
		{
			swap(_slots, o._slots);
			swap(_states, o._states);
			swap(_capacity, o._capacity);
			swap(_size, o._size);
			swap(_tombstones, o._tombstones);
			return *this;
		}
		~hash_map() { release(); }

		size_t size() const noexcept { return _size; }
		bool empty() const noexcept { return _size == 0; }
		iterator begin() const { return iterator(this, 0); }
		iterator end() const { return iterator(this, _capacity); }

		void reserve(size_t n)
		{
			size_t wanted = 8;
			while (wanted * 7 / 8 < n) wanted *= 2;
			if (wanted > _capacity) rehash(wanted);
		}

		template<class Key>
		V* find(const Key& key) const
		{
			if (_capacity == 0) return nullptr;
			const size_t mask = _capacity - 1;
			for (size_t i = H()(key) & mask, probe = 0; probe < _capacity; i = (i + ++probe) & mask)
			{
				if (_states[i] == slot_state::empty) return nullptr;
				if (_states[i] == slot_state::full && E()(slot(i)->first, key)) return &slot(i)->second;
			}
			return nullptr;
		}
		template<class Key> bool contains(const Key& key) const { return find(key) != nullptr; }

		template<class Key, class... A>
		pair<V*, bool> try_emplace(Key&& key, A&&... a)
		{
			if (V* existing = find(key)) return pair<V*, bool>(existing, false);
			if ((_size + _tombstones + 1) * 8 > _capacity * 7) rehash(_capacity ? (_size * 2 > _capacity ? _capacity * 2 : _capacity) : 8);
			const size_t mask = _capacity - 1;
			size_t i = H()(key) & mask;
			for (size_t probe = 0; _states[i] == slot_state::full; i = (i + ++probe) & mask) {}
			if (_states[i] == slot_state::deleted) --_tombstones;
			construct_at(slot(i), K(forward<Key>(key)), V(forward<A>(a)...));
			_states[i] = slot_state::full;
			++_size;
			return pair<V*, bool>(&slot(i)->second, true);
		}
		template<class Key, class T>
		V& insert_or_assign(Key&& key, T&& v)
		{
			auto r = try_emplace(forward<Key>(key), forward<T>(v));
			if (!r.second) *r.first = forward<T>(v);
			return *r.first;
		}
		template<class Key>
		V& operator[](Key&& key) { return *try_emplace(forward<Key>(key)).first; }

		template<class Key>
		bool erase(const Key& key)
		{
			if (_capacity == 0) return false;
			const size_t mask = _capacity - 1;
			for (size_t i = H()(key) & mask, probe = 0; probe < _capacity; i = (i + ++probe) & mask)
			{
				if (_states[i] == slot_state::empty) return false;
				if (_states[i] == slot_state::full && E()(slot(i)->first, key))
				{
					destroy_at(slot(i));
					_states[i] = slot_state::deleted;
					--_size;
					++_tombstones;
					return true;
				}
			}
			return false;
		}
		void clear()
		{
			for (size_t i = 0; i < _capacity; ++i)
			{
				if (_states[i] == slot_state::full) destroy_at(slot(i));
				_states[i] = slot_state::empty;
			}
			_size = _tombstones = 0;
		}
	private:
		value_type* _slots = nullptr;
		slot_state* _states = nullptr;
		size_t _capacity = 0;
		size_t _size = 0;
		size_t _tombstones = 0;

		value_type* slot(size_t i) const noexcept { return _slots + i; }

		void rehash(size_t capacity)
		{
			value_type* oldSlots = _slots;
			slot_state* oldStates = _states;
			const size_t oldCapacity = _capacity;
			_slots = static_cast<value_type*>(::operator new(capacity * sizeof(value_type)));
			_states = static_cast<slot_state*>(::operator new(capacity));
			for (size_t i = 0; i < capacity; ++i) _states[i] = slot_state::empty;
			_capacity = capacity;
			_size = _tombstones = 0;
			for (size_t i = 0; i < oldCapacity; ++i)
			{
				if (oldStates[i] != slot_state::full) continue;
				try_emplace(move(const_cast<K&>(oldSlots[i].first)), move(oldSlots[i].second));
				destroy_at(oldSlots + i);
			}
			::operator delete(oldSlots);
			::operator delete(oldStates);
		}
		void release() noexcept
		{
			for (size_t i = 0; i < _capacity; ++i) if (_states[i] == slot_state::full) destroy_at(slot(i));
			::operator delete(_slots);
			::operator delete(_states);
			_slots = nullptr;
			_states = nullptr;
			_capacity = _size = _tombstones = 0;
		}
	};

	template<class T, class H = hash<T>, class E = equal_to<>>
	class hash_set
	{
	public:
		bool insert(const T& v) { return _map.try_emplace(v).second; }
		bool contains(const T& v) const { return _map.contains(v); }
		bool erase(const T& v) { return _map.erase(v); }
		size_t size() const noexcept { return _map.size(); }
		template<class F> void for_each(F&& f) const { for (auto& kv : _map) f(kv.first); }
	private:
		hash_map<T, monostate, H, E> _map;
	};

	// ---------------------------------------------------------------------------------------------- ordered map (AVL)

	template<class K, class V, class C = less<>>
	class ordered_map
	{
		struct node
		{
			pair<const K, V> kv;
			node* left = nullptr;
			node* right = nullptr;
			node* parent = nullptr;
			int height = 1;
			template<class Key, class... A> node(Key&& k, A&&... a) : kv(forward<Key>(k), V(forward<A>(a)...)) {}
		};
	public:
		class iterator
		{
		public:
			typedef pair<const K, V> value_type;
			typedef ptrdiff_t difference_type;
			typedef bidirectional_iterator_tag iterator_category;
			typedef value_type& reference;

			explicit iterator(node* n = nullptr) : _node(n) {}
			value_type& operator*() const { return _node->kv; }
			value_type* operator->() const { return &_node->kv; }
			iterator& operator++()
			{
				if (_node->right)
				{
					_node = _node->right;
					while (_node->left) _node = _node->left;
				}
				else
				{
					node* p = _node->parent;
					while (p && _node == p->right)
					{
						_node = p;
						p = p->parent;
					}
					_node = p;
				}
				return *this;
			}
			bool operator==(const iterator& o) const { return _node == o._node; }
			bool operator!=(const iterator& o) const { return _node != o._node; }
		private:
			node* _node;
			friend class ordered_map;
		};

		ordered_map() = default;
		ordered_map(initializer_list<pair<K, V>> list)
		{
			for (auto& kv : list) insert_or_assign(kv.first, kv.second);
		}
		ordered_map(const ordered_map& o)
		{
			for (auto& kv : o) insert_or_assign(kv.first, kv.second);
		}
		ordered_map(ordered_map&& o) noexcept : _root(exchange(o._root, nullptr)), _size(exchange(o._size, 0)) {}
		ordered_map& operator=(ordered_map o) noexcept
		{
			swap(_root, o._root);
			swap(_size, o._size);
			return *this;
		}
		~ordered_map() { destroy_tree(_root); }

		size_t size() const noexcept { return _size; }
		bool empty() const noexcept { return _size == 0; }
		iterator begin() const
		{
			node* n = _root;
			if (n) while (n->left) n = n->left;
			return iterator(n);
		}
		iterator end() const { return iterator(nullptr); }

		template<class Key>
		iterator find(const Key& key) const
		{
			node* n = _root;
			while (n)
			{
				if (C()(key, n->kv.first)) n = n->left;
				else if (C()(n->kv.first, key)) n = n->right;
				else return iterator(n);
			}
			return end();
		}
		template<class Key>
		iterator lower_bound(const Key& key) const
		{
			node* n = _root;
			node* best = nullptr;
			while (n)
			{
				if (C()(n->kv.first, key)) n = n->right;
				else
				{
					best = n;
					n = n->left;
				}
			}
			return iterator(best);
		}
		template<class Key> bool contains(const Key& key) const { return find(key) != end(); }

		template<class Key, class... A>
		pair<iterator, bool> try_emplace(Key&& key, A&&... a)
		{
			node* parent = nullptr;
			node** link = &_root;
			while (*link)
			{
				parent = *link;
				if (C()(key, parent->kv.first)) link = &parent->left;
				else if (C()(parent->kv.first, key)) link = &parent->right;
				else return pair<iterator, bool>(iterator(parent), false);
			}
			node* n = new node(forward<Key>(key), forward<A>(a)...);
			n->parent = parent;
			*link = n;
			++_size;
			rebalance_up(parent);
			return pair<iterator, bool>(iterator(n), true);
		}
		template<class Key, class T>
		V& insert_or_assign(Key&& key, T&& v)
		{
			auto r = try_emplace(forward<Key>(key), forward<T>(v));
			if (!r.second) r.first->second = forward<T>(v);
			return r.first->second;
		}
		template<class Key>
		V& operator[](Key&& key) { return try_emplace(forward<Key>(key)).first->second; }

		template<class F>
		void for_each_in_range(const K& lo, const K& hi, F&& f) const
		{
			for (auto it = lower_bound(lo); it != end() && C()(it->first, hi); ++it) f(it->first, it->second);
		}
	private:
		node* _root = nullptr;
		size_t _size = 0;

		static int height(node* n) noexcept { return n ? n->height : 0; }
		static void update(node* n) noexcept { n->height = 1 + max(height(n->left), height(n->right)); }
		static int balance(node* n) noexcept { return height(n->left) - height(n->right); }

		void replace_child(node* parent, node* old, node* fresh) noexcept
		{
			if (!parent) _root = fresh;
			else if (parent->left == old) parent->left = fresh;
			else parent->right = fresh;
			if (fresh) fresh->parent = parent;
		}
		node* rotate_left(node* x) noexcept
		{
			node* y = x->right;
			replace_child(x->parent, x, y);
			x->right = y->left;
			if (y->left) y->left->parent = x;
			y->left = x;
			x->parent = y;
			update(x);
			update(y);
			return y;
		}
		node* rotate_right(node* x) noexcept
		{
			node* y = x->left;
			replace_child(x->parent, x, y);
			x->left = y->right;
			if (y->right) y->right->parent = x;
			y->right = x;
			x->parent = y;
			update(x);
			update(y);
			return y;
		}
		void rebalance_up(node* n) noexcept
		{
			while (n)
			{
				update(n);
				const int b = balance(n);
				if (b > 1)
				{
					if (balance(n->left) < 0) rotate_left(n->left);
					n = rotate_right(n);
				}
				else if (b < -1)
				{
					if (balance(n->right) > 0) rotate_right(n->right);
					n = rotate_left(n);
				}
				n = n->parent;
			}
		}
		static void destroy_tree(node* n) noexcept
		{
			while (n)
			{
				destroy_tree(n->right);
				node* left = n->left;
				delete n;
				n = left;
			}
		}
	};

	// ---------------------------------------------------------------------------------------------- deque-like ring buffer

	template<class T>
	class ring_buffer
	{
	public:
		ring_buffer() = default;
		ring_buffer(const ring_buffer&) = delete;
		ring_buffer& operator=(const ring_buffer&) = delete;
		~ring_buffer()
		{
			while (!empty()) pop_front();
			::operator delete(_data);
		}
		size_t size() const noexcept { return _size; }
		bool empty() const noexcept { return _size == 0; }
		T& front() noexcept { return _data[_head]; }
		T& back() noexcept { return _data[(_head + _size - 1) & (_capacity - 1)]; }
		T& operator[](size_t i) noexcept { return _data[(_head + i) & (_capacity - 1)]; }
		template<class... A>
		T& emplace_back(A&&... a)
		{
			if (_size == _capacity) grow();
			T* p = construct_at(_data + ((_head + _size) & (_capacity - 1)), forward<A>(a)...);
			++_size;
			return *p;
		}
		template<class... A>
		T& emplace_front(A&&... a)
		{
			if (_size == _capacity) grow();
			_head = (_head + _capacity - 1) & (_capacity - 1);
			T* p = construct_at(_data + _head, forward<A>(a)...);
			++_size;
			return *p;
		}
		void push_back(T v) { emplace_back(move(v)); }
		void push_front(T v) { emplace_front(move(v)); }
		void pop_front() noexcept
		{
			destroy_at(_data + _head);
			_head = (_head + 1) & (_capacity - 1);
			--_size;
		}
		void pop_back() noexcept
		{
			destroy_at(&back());
			--_size;
		}
	private:
		T* _data = nullptr;
		size_t _head = 0;
		size_t _size = 0;
		size_t _capacity = 0;

		void grow()
		{
			const size_t capacity = _capacity ? _capacity * 2 : 8;
			T* fresh = static_cast<T*>(::operator new(capacity * sizeof(T)));
			for (size_t i = 0; i < _size; ++i)
			{
				T* old = _data + ((_head + i) & (_capacity - 1));
				construct_at(fresh + i, move(*old));
				destroy_at(old);
			}
			::operator delete(_data);
			_data = fresh;
			_head = 0;
			_capacity = capacity;
		}
	};

	template<class T, class C = less<>>
	class priority_queue
	{
	public:
		bool empty() const noexcept { return _heap.empty(); }
		size_t size() const noexcept { return _heap.size(); }
		const T& top() const noexcept { return _heap.front(); }
		void push(T v)
		{
			_heap.push_back(move(v));
			size_t i = _heap.size() - 1;
			while (i > 0)
			{
				size_t parent = (i - 1) / 2;
				if (!C()(_heap[parent], _heap[i])) break;
				swap(_heap[parent], _heap[i]);
				i = parent;
			}
		}
		T pop()
		{
			T result = move(_heap.front());
			_heap.front() = move(_heap.back());
			_heap.pop_back();
			sift_down(_heap.begin(), 0, ptrdiff_t(_heap.size()), C());
			return result;
		}
	private:
		vector<T> _heap;
	};
}

namespace ebs
{
	// ---------------------------------------------------------------------------------------------- number formatting

	struct to_chars_result
	{
		char* ptr;
		bool ok;
	};

	template<class T, enable_if_t<is_integral_v<T>, int> = 0>
	to_chars_result to_chars(char* first, char* last, T value, int base = 10)
	{
		typedef make_unsigned_t<T> U;
		U u = U(value);
		if constexpr (is_signed<T>::value)
		{
			if (value < 0)
			{
				if (first == last) return { last, false };
				*first++ = '-';
				u = U(0) - u;
			}
		}
		char buffer[72];
		char* p = buffer + sizeof(buffer);
		do
		{
			const unsigned digit = unsigned(u % U(base));
			*--p = char(digit < 10 ? '0' + digit : 'a' + digit - 10);
			u /= U(base);
		} while (u != 0);
		const size_t n = size_t(buffer + sizeof(buffer) - p);
		if (size_t(last - first) < n) return { last, false };
		memcpy(first, p, n);
		return { first + n, true };
	}

	inline to_chars_result to_chars(char* first, char* last, double value, int precision = 6)
	{
		if (value != value)
		{
			if (last - first < 3) return { last, false };
			memcpy(first, "nan", 3);
			return { first + 3, true };
		}
		if (value < 0)
		{
			if (first == last) return { last, false };
			*first++ = '-';
			value = -value;
		}
		if (value > 1e18)
		{
			int exponent = 0;
			while (value >= 10.0)
			{
				value /= 10.0;
				++exponent;
			}
			auto r = to_chars(first, last, value, precision);
			if (!r.ok || last - r.ptr < 2) return { last, false };
			*r.ptr++ = 'e';
			return to_chars(r.ptr, last, exponent);
		}
		double scale = 1.0;
		for (int i = 0; i < precision; ++i) scale *= 10.0;
		const uint64_t scaled = uint64_t(value * scale + 0.5);
		const uint64_t integral = scaled / uint64_t(scale);
		uint64_t fraction = scaled % uint64_t(scale);
		auto r = to_chars(first, last, integral);
		if (!r.ok || precision == 0) return r;
		char digits[32];
		for (int i = precision - 1; i >= 0; --i)
		{
			digits[i] = char('0' + fraction % 10);
			fraction /= 10;
		}
		int n = precision;
		while (n > 1 && digits[n - 1] == '0') --n;
		if (last - r.ptr < n + 1) return { last, false };
		*r.ptr++ = '.';
		memcpy(r.ptr, digits, size_t(n));
		return { r.ptr + n, true };
	}

	struct from_chars_result
	{
		const char* ptr;
		bool ok;
	};

	template<class T, enable_if_t<is_integral_v<T>, int> = 0>
	from_chars_result from_chars(const char* first, const char* last, T& value, int base = 10)
	{
		bool negative = false;
		if (first != last && *first == '-')
		{
			if constexpr (!is_signed<T>::value) return { first, false };
			negative = true;
			++first;
		}
		typedef make_unsigned_t<T> U;
		U result = 0;
		const char* start = first;
		for (; first != last; ++first)
		{
			int digit;
			const char c = *first;
			if (c >= '0' && c <= '9') digit = c - '0';
			else if (c >= 'a' && c <= 'z') digit = c - 'a' + 10;
			else if (c >= 'A' && c <= 'Z') digit = c - 'A' + 10;
			else break;
			if (digit >= base) break;
			result = U(result * U(base) + U(digit));
		}
		if (first == start) return { first, false };
		value = negative ? T(U(0) - result) : T(result);
		return { first, true };
	}

	inline from_chars_result from_chars(const char* first, const char* last, double& value)
	{
		const char* p = first;
		bool negative = false;
		if (p != last && (*p == '-' || *p == '+'))
		{
			negative = *p == '-';
			++p;
		}
		double result = 0.0;
		bool any = false;
		for (; p != last && *p >= '0' && *p <= '9'; ++p, any = true) result = result * 10.0 + (*p - '0');
		if (p != last && *p == '.')
		{
			double scale = 0.1;
			for (++p; p != last && *p >= '0' && *p <= '9'; ++p, any = true, scale *= 0.1) result += (*p - '0') * scale;
		}
		if (!any) return { first, false };
		if (p != last && (*p == 'e' || *p == 'E'))
		{
			int exponent = 0;
			auto r = from_chars(p + 1, last, exponent);
			if (r.ok)
			{
				p = r.ptr;
				result *= pow(10.0, exponent);
			}
		}
		value = negative ? -result : result;
		return { p, true };
	}

	// ---------------------------------------------------------------------------------------------- format

	struct format_spec
	{
		char fill = ' ';
		char align = 0;    // '<', '>', '^'
		bool plus = false;
		int width = 0;
		int precision = -1;
		char type = 0;     // 'd', 'x', 'b', 'f', 's', 'c'

		static format_spec parse(string_view s)
		{
			format_spec spec;
			size_t i = 0;
			if (s.size() >= 2 && (s[1] == '<' || s[1] == '>' || s[1] == '^'))
			{
				spec.fill = s[0];
				spec.align = s[1];
				i = 2;
			}
			else if (!s.empty() && (s[0] == '<' || s[0] == '>' || s[0] == '^'))
			{
				spec.align = s[0];
				i = 1;
			}
			if (i < s.size() && s[i] == '+')
			{
				spec.plus = true;
				++i;
			}
			while (i < s.size() && s[i] >= '0' && s[i] <= '9') spec.width = spec.width * 10 + (s[i++] - '0');
			if (i < s.size() && s[i] == '.')
			{
				spec.precision = 0;
				for (++i; i < s.size() && s[i] >= '0' && s[i] <= '9'; ++i) spec.precision = spec.precision * 10 + (s[i] - '0');
			}
			if (i < s.size()) spec.type = s[i];
			return spec;
		}
	};

	class format_buffer
	{
	public:
		void append(string_view s) { _text.append(s); }
		void append(char c) { _text.push_back(c); }
		void append_padded(string_view s, const format_spec& spec, char defaultAlign)
		{
			const size_t width = size_t(spec.width);
			if (s.size() >= width)
			{
				append(s);
				return;
			}
			const size_t pad = width - s.size();
			const char align = spec.align ? spec.align : defaultAlign;
			const size_t left = align == '>' ? pad : (align == '^' ? pad / 2 : 0);
			_text.append(left, spec.fill);
			append(s);
			_text.append(pad - left, spec.fill);
		}
		string& text() noexcept { return _text; }
	private:
		string _text;
	};

	template<class T, class = void> struct formatter;

	template<class T>
	struct formatter<T, enable_if_t<is_integral_v<T> && !is_same_v<T, bool> && !is_same_v<T, char>>>
	{
		static void format(format_buffer& out, T value, const format_spec& spec)
		{
			char buffer[80];
			char* p = buffer;
			if (spec.plus && value >= 0) *p++ = '+';
			const int base = spec.type == 'x' ? 16 : (spec.type == 'b' ? 2 : (spec.type == 'o' ? 8 : 10));
			auto r = to_chars(p, buffer + sizeof(buffer), value, base);
			out.append_padded(string_view(buffer, size_t(r.ptr - buffer)), spec, '>');
		}
	};
	template<>
	struct formatter<bool, void>
	{
		static void format(format_buffer& out, bool value, const format_spec& spec)
		{
			out.append_padded(value ? "true" : "false", spec, '<');
		}
	};
	template<>
	struct formatter<char, void>
	{
		static void format(format_buffer& out, char value, const format_spec& spec)
		{
			out.append_padded(string_view(&value, 1), spec, '<');
		}
	};
	template<class T>
	struct formatter<T, enable_if_t<is_floating_point_v<T>>>
	{
		static void format(format_buffer& out, T value, const format_spec& spec)
		{
			char buffer[96];
			char* p = buffer;
			if (spec.plus && value >= 0) *p++ = '+';
			auto r = to_chars(p, buffer + sizeof(buffer), double(value), spec.precision >= 0 ? spec.precision : 6);
			out.append_padded(string_view(buffer, size_t(r.ptr - buffer)), spec, '>');
		}
	};
	template<>
	struct formatter<string_view, void>
	{
		static void format(format_buffer& out, string_view value, const format_spec& spec)
		{
			if (spec.precision >= 0) value = value.substr(0, size_t(spec.precision));
			out.append_padded(value, spec, '<');
		}
	};
	template<>
	struct formatter<string, void>
	{
		static void format(format_buffer& out, const string& value, const format_spec& spec)
		{
			formatter<string_view>::format(out, value.view(), spec);
		}
	};
	template<>
	struct formatter<const char*, void>
	{
		static void format(format_buffer& out, const char* value, const format_spec& spec)
		{
			formatter<string_view>::format(out, string_view(value), spec);
		}
	};
	template<class T>
	struct formatter<T*, enable_if_t<!is_same_v<remove_cv_t<T>, char>>>
	{
		static void format(format_buffer& out, T* value, const format_spec& spec)
		{
			format_spec hex = spec;
			hex.type = 'x';
			out.append("0x");
			formatter<uint64_t>::format(out, uint64_t(size_t(value)), hex);
		}
	};
	template<class T>
	struct formatter<vector<T>, void>
	{
		static void format(format_buffer& out, const vector<T>& value, const format_spec& spec)
		{
			out.append('[');
			bool first = true;
			for (auto& e : value)
			{
				if (!first) out.append(", ");
				first = false;
				formatter<remove_cvref_t<T>>::format(out, e, spec);
			}
			out.append(']');
		}
	};
	template<class A, class B>
	struct formatter<pair<A, B>, void>
	{
		static void format(format_buffer& out, const pair<A, B>& value, const format_spec& spec)
		{
			out.append('(');
			formatter<A>::format(out, value.first, spec);
			out.append(", ");
			formatter<B>::format(out, value.second, spec);
			out.append(')');
		}
	};
	template<class T>
	struct formatter<optional<T>, void>
	{
		static void format(format_buffer& out, const optional<T>& value, const format_spec& spec)
		{
			if (value) formatter<T>::format(out, *value, spec);
			else out.append("none");
		}
	};

	namespace detail
	{
		struct format_arg
		{
			const void* value;
			void (*format)(format_buffer&, const void*, const format_spec&);
		};

		template<class T>
		format_arg make_arg(const T& value)
		{
			typedef conditional_t<is_array<T>::value, const char*, T> U;
			return { &value, [](format_buffer& out, const void* v, const format_spec& spec)
			{
				if constexpr (is_array<T>::value) formatter<const char*>::format(out, static_cast<const char*>(v), spec);
				else formatter<U>::format(out, *static_cast<const U*>(v), spec);
			} };
		}

		inline void vformat(format_buffer& out, string_view fmt, span<const format_arg> args)
		{
			size_t next = 0;
			for (size_t i = 0; i < fmt.size(); ++i)
			{
				const char c = fmt[i];
				if (c == '{')
				{
					if (i + 1 < fmt.size() && fmt[i + 1] == '{')
					{
						out.append('{');
						++i;
						continue;
					}
					const size_t close = fmt.find('}', i);
					if (close == string_view::npos) panic("unterminated format field");
					string_view field = fmt.substr(i + 1, close - i - 1);
					size_t index = next++;
					const size_t colon = field.find(':');
					string_view id = field.substr(0, colon);
					if (!id.empty()) from_chars(id.begin(), id.end(), index);
					format_spec spec = colon == string_view::npos ? format_spec() : format_spec::parse(field.substr(colon + 1));
					if (index >= args.size()) panic("format argument index out of range");
					args[index].format(out, args[index].value, spec);
					i = close;
				}
				else if (c == '}' && i + 1 < fmt.size() && fmt[i + 1] == '}')
				{
					out.append('}');
					++i;
				}
				else
				{
					out.append(c);
				}
			}
		}
	}

	template<class... A>
	string format(string_view fmt, const A&... args)
	{
		format_buffer out;
		const detail::format_arg list[] = { detail::make_arg(args)..., detail::format_arg{ nullptr, nullptr } };
		detail::vformat(out, fmt, span<const detail::format_arg>(list, sizeof...(A)));
		return move(out.text());
	}

	template<class... A>
	void format_to(string& out, string_view fmt, const A&... args)
	{
		out += format(fmt, args...);
	}

	template<class T>
	string to_string(const T& v) { return format("{}", v); }

	// ---------------------------------------------------------------------------------------------- string helpers

	inline vector<string_view> split(string_view s, char separator, bool skipEmpty = true)
	{
		vector<string_view> parts;
		size_t start = 0;
		while (start <= s.size())
		{
			size_t end = s.find(separator, start);
			if (end == string_view::npos) end = s.size();
			string_view part = s.substr(start, end - start);
			if (!skipEmpty || !part.empty()) parts.push_back(part);
			start = end + 1;
		}
		return parts;
	}

	inline string_view trim(string_view s)
	{
		const string_view ws = " \t\r\n";
		const size_t first = s.find_first_not_of(ws);
		if (first == string_view::npos) return string_view();
		size_t last = s.size();
		while (last > first && ws.find(s[last - 1]) != string_view::npos) --last;
		return s.substr(first, last - first);
	}

	template<class Range, class F>
	string join(const Range& range, string_view separator, F&& toString)
	{
		string out;
		bool first = true;
		for (auto& e : range)
		{
			if (!first) out.append(separator);
			first = false;
			out += toString(e);
		}
		return out;
	}

	inline constexpr char to_lower(char c) { return c >= 'A' && c <= 'Z' ? char(c - 'A' + 'a') : c; }
	inline constexpr char to_upper(char c) { return c >= 'a' && c <= 'z' ? char(c - 'a' + 'A') : c; }
	inline constexpr bool is_digit(char c) { return c >= '0' && c <= '9'; }
	inline constexpr bool is_alpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
	inline constexpr bool is_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

	inline string to_lower(string_view s)
	{
		string out(s.size(), ' ');
		transform(s.begin(), s.end(), out.begin(), [](char c) { return to_lower(c); });
		return out;
	}

	inline string replace_all(string_view s, string_view from, string_view to)
	{
		string out;
		size_t start = 0;
		while (true)
		{
			const size_t at = s.find(from, start);
			if (at == string_view::npos || from.empty()) break;
			out.append(s.substr(start, at - start));
			out.append(to);
			start = at + from.size();
		}
		out.append(s.substr(start));
		return out;
	}

	// ---------------------------------------------------------------------------------------------- result / expected

	template<class T, class E>
	class expected
	{
	public:
		expected(T value) : _state(in_place_index<0>, move(value)) {}
		static expected failure(E error) { return expected(in_place_index<1>, move(error)); }

		bool has_value() const noexcept { return _state.index() == 0; }
		explicit operator bool() const noexcept { return has_value(); }
		T& value() { return _state.template get_unchecked<0>(); }
		const T& value() const { return _state.template get_unchecked<0>(); }
		T& operator*() { return value(); }
		T* operator->() { return &value(); }
		const E& error() const { return _state.template get_unchecked<1>(); }

		template<class F>
		auto map(F&& f) const -> expected<remove_cvref_t<invoke_result_t<F, const T&>>, E>
		{
			typedef expected<remove_cvref_t<invoke_result_t<F, const T&>>, E> R;
			if (has_value()) return R(f(value()));
			return R::failure(error());
		}
		template<class F>
		auto and_then(F&& f) const -> remove_cvref_t<invoke_result_t<F, const T&>>
		{
			typedef remove_cvref_t<invoke_result_t<F, const T&>> R;
			if (has_value()) return f(value());
			return R::failure(error());
		}
	private:
		template<size_t I, class U>
		expected(in_place_index_t<I> i, U&& u) : _state(i, forward<U>(u)) {}
		variant<T, E> _state;
	};

	struct error_info
	{
		string message;
		size_t position = 0;
		bool operator==(const error_info& o) const { return message == o.message && position == o.position; }
	};

	template<>
	struct formatter<error_info, void>
	{
		static void format(format_buffer& out, const error_info& e, const format_spec&)
		{
			out.append("error at ");
			formatter<size_t>::format(out, e.position, format_spec());
			out.append(": ");
			out.append(e.message.view());
		}
	};

	// ---------------------------------------------------------------------------------------------- signals

	template<class... A>
	class signal
	{
	public:
		typedef size_t connection;

		template<class F>
		connection connect(F&& f)
		{
			const connection id = ++_nextId;
			_slots.emplace_back(id, function<void(A...)>(forward<F>(f)));
			return id;
		}
		bool disconnect(connection id)
		{
			auto it = find_if(_slots.begin(), _slots.end(), [&](const auto& s) { return s.first == id; });
			if (it == _slots.end()) return false;
			_slots.erase(it);
			return true;
		}
		void emit(const A&... a) const
		{
			for (auto& s : _slots) s.second(a...);
		}
		size_t size() const noexcept { return _slots.size(); }
	private:
		vector<pair<connection, function<void(A...)>>> _slots;
		connection _nextId = 0;
	};

	// ---------------------------------------------------------------------------------------------- arena

	class arena
	{
	public:
		explicit arena(size_t blockSize = 4096) : _blockSize(blockSize) {}
		arena(const arena&) = delete;
		arena& operator=(const arena&) = delete;
		~arena()
		{
			for (auto& f : _finalizers) f.first(f.second);
			for (void* b : _blocks) ::operator delete(b);
		}
		void* allocate(size_t size, size_t align)
		{
			size_t offset = (_offset + align - 1) & ~(align - 1);
			if (_blocks.empty() || offset + size > _blockSize)
			{
				_blocks.push_back(::operator new(max(size + align, _blockSize)));
				offset = 0;
			}
			_offset = offset + size;
			return static_cast<char*>(_blocks.back()) + offset;
		}
		template<class T, class... A>
		T* make(A&&... a)
		{
			T* p = construct_at(static_cast<T*>(allocate(sizeof(T), alignof(T))), forward<A>(a)...);
			if constexpr (!is_trivially_destructible_v<T>)
			{
				_finalizers.emplace_back([](void* o) { static_cast<T*>(o)->~T(); }, p);
			}
			return p;
		}
		size_t block_count() const noexcept { return _blocks.size(); }
	private:
		size_t _blockSize;
		size_t _offset = 0;
		vector<void*> _blocks;
		vector<pair<void (*)(void*), void*>> _finalizers;
	};
}

namespace app::geometry
{
	using namespace ebs;

	template<class T, size_t N>
	struct vec
	{
		array<T, N> v{};

		constexpr vec() = default;
		template<class... A, enable_if_t<sizeof...(A) == N && conjunction<is_convertible<A, T>...>::value, int> = 0>
		constexpr vec(A... a) : v{ { T(a)... } } {}

		constexpr T& operator[](size_t i) noexcept { return v[i]; }
		constexpr const T& operator[](size_t i) const noexcept { return v[i]; }

		template<class F>
		constexpr vec map(F f) const
		{
			vec r;
			for (size_t i = 0; i < N; ++i) r[i] = f(v[i]);
			return r;
		}
		template<class F>
		constexpr vec zip(const vec& o, F f) const
		{
			vec r;
			for (size_t i = 0; i < N; ++i) r[i] = f(v[i], o[i]);
			return r;
		}
		constexpr vec operator+(const vec& o) const { return zip(o, plus<T>()); }
		constexpr vec operator-(const vec& o) const { return zip(o, [](T a, T b) { return a - b; }); }
		constexpr vec operator*(T s) const { return map([s](T a) { return a * s; }); }
		constexpr vec operator/(T s) const { return map([s](T a) { return a / s; }); }
		constexpr vec operator-() const { return map([](T a) { return -a; }); }
		constexpr vec& operator+=(const vec& o) { return *this = *this + o; }
		constexpr vec& operator-=(const vec& o) { return *this = *this - o; }
		constexpr vec& operator*=(T s) { return *this = *this * s; }
		constexpr bool operator==(const vec& o) const { return v == o.v; }

		constexpr T dot(const vec& o) const
		{
			T sum{};
			for (size_t i = 0; i < N; ++i) sum += v[i] * o[i];
			return sum;
		}
		constexpr T length_squared() const { return dot(*this); }
		T length() const { return T(sqrt(double(length_squared()))); }
		vec normalized() const
		{
			const T l = length();
			return l == T(0) ? *this : *this / l;
		}
		constexpr T min_component() const { return *min_element(v.begin(), v.end()); }
		constexpr T max_component() const { return *max_element(v.begin(), v.end()); }
	};

	template<class T> constexpr vec<T, 3> cross(const vec<T, 3>& a, const vec<T, 3>& b)
	{
		return vec<T, 3>(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
	}

	template<class T, size_t R, size_t C>
	struct mat
	{
		array<vec<T, C>, R> rows{};

		static constexpr mat identity()
		{
			mat m;
			for (size_t i = 0; i < (R < C ? R : C); ++i) m.rows[i][i] = T(1);
			return m;
		}
		constexpr vec<T, C>& operator[](size_t r) noexcept { return rows[r]; }
		constexpr const vec<T, C>& operator[](size_t r) const noexcept { return rows[r]; }

		template<size_t K>
		constexpr mat<T, R, K> operator*(const mat<T, C, K>& o) const
		{
			mat<T, R, K> out;
			for (size_t r = 0; r < R; ++r)
			{
				for (size_t k = 0; k < K; ++k)
				{
					T sum{};
					for (size_t c = 0; c < C; ++c) sum += rows[r][c] * o[c][k];
					out[r][k] = sum;
				}
			}
			return out;
		}
		constexpr vec<T, R> operator*(const vec<T, C>& x) const
		{
			vec<T, R> out;
			for (size_t r = 0; r < R; ++r) out[r] = rows[r].dot(x);
			return out;
		}
		constexpr mat<T, C, R> transposed() const
		{
			mat<T, C, R> out;
			for (size_t r = 0; r < R; ++r)
				for (size_t c = 0; c < C; ++c) out[c][r] = rows[r][c];
			return out;
		}
	};

	template<class T, size_t N>
	optional<mat<T, N, N>> inverse(mat<T, N, N> m)
	{
		mat<T, N, N> inv = mat<T, N, N>::identity();
		for (size_t col = 0; col < N; ++col)
		{
			size_t pivot = col;
			for (size_t r = col + 1; r < N; ++r) if (abs(m[r][col]) > abs(m[pivot][col])) pivot = r;
			if (abs(m[pivot][col]) < T(1e-12)) return nullopt;
			swap(m[pivot], m[col]);
			swap(inv[pivot], inv[col]);
			const T scale = T(1) / m[col][col];
			m[col] *= scale;
			inv[col] *= scale;
			for (size_t r = 0; r < N; ++r)
			{
				if (r == col) continue;
				const T f = m[r][col];
				m[r] -= m[col] * f;
				inv[r] -= inv[col] * f;
			}
		}
		return inv;
	}

	template<class T> using vec2 = vec<T, 2>;
	template<class T> using vec3 = vec<T, 3>;
	template<class T> using vec4 = vec<T, 4>;
	template<class T> using mat4 = mat<T, 4, 4>;

	template<class T>
	mat4<T> translation(const vec3<T>& t)
	{
		mat4<T> m = mat4<T>::identity();
		for (size_t i = 0; i < 3; ++i) m[i][3] = t[i];
		return m;
	}
	template<class T>
	mat4<T> rotation_z(T angle)
	{
		mat4<T> m = mat4<T>::identity();
		const T c = T(cos(double(angle))), s = T(sin(double(angle)));
		m[0][0] = c;
		m[0][1] = -s;
		m[1][0] = s;
		m[1][1] = c;
		return m;
	}
	template<class T>
	mat4<T> perspective(T fov, T aspect, T nearPlane, T farPlane)
	{
		const T f = T(1) / T(sin(double(fov) / 2) / cos(double(fov) / 2));
		mat4<T> m;
		m[0][0] = f / aspect;
		m[1][1] = f;
		m[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
		m[2][3] = T(2) * farPlane * nearPlane / (nearPlane - farPlane);
		m[3][2] = T(-1);
		return m;
	}

	template<class T, size_t N>
	struct aabb
	{
		vec<T, N> lo, hi;

		static aabb empty()
		{
			aabb b;
			for (size_t i = 0; i < N; ++i)
			{
				b.lo[i] = numeric_limits<T>::max();
				b.hi[i] = numeric_limits<T>::lowest();
			}
			return b;
		}
		void extend(const vec<T, N>& p)
		{
			lo = lo.zip(p, [](T a, T b) { return min(a, b); });
			hi = hi.zip(p, [](T a, T b) { return max(a, b); });
		}
		bool contains(const vec<T, N>& p) const
		{
			for (size_t i = 0; i < N; ++i) if (p[i] < lo[i] || p[i] > hi[i]) return false;
			return true;
		}
		bool intersects(const aabb& o) const
		{
			for (size_t i = 0; i < N; ++i) if (o.hi[i] < lo[i] || o.lo[i] > hi[i]) return false;
			return true;
		}
		vec<T, N> center() const { return (lo + hi) / T(2); }
		T volume() const
		{
			T v = T(1);
			for (size_t i = 0; i < N; ++i) v *= max(T(0), hi[i] - lo[i]);
			return v;
		}
	};

	template<class T>
	struct ray
	{
		vec3<T> origin, direction;
		vec3<T> at(T t) const { return origin + direction * t; }
	};

	template<class T>
	optional<T> intersect_sphere(const ray<T>& r, const vec3<T>& center, T radius)
	{
		const vec3<T> oc = r.origin - center;
		const T b = oc.dot(r.direction);
		const T c = oc.length_squared() - radius * radius;
		const T disc = b * b - c;
		if (disc < T(0)) return nullopt;
		const T s = T(sqrt(double(disc)));
		const T t = -b - s;
		if (t > T(1e-4)) return t;
		const T t2 = -b + s;
		if (t2 > T(1e-4)) return t2;
		return nullopt;
	}

	template<class T>
	optional<T> intersect_box(const ray<T>& r, const aabb<T, 3>& box)
	{
		T tmin = T(0), tmax = numeric_limits<T>::max();
		for (size_t i = 0; i < 3; ++i)
		{
			const T inv = T(1) / r.direction[i];
			T t0 = (box.lo[i] - r.origin[i]) * inv;
			T t1 = (box.hi[i] - r.origin[i]) * inv;
			if (inv < T(0)) swap(t0, t1);
			tmin = max(tmin, t0);
			tmax = min(tmax, t1);
			if (tmax < tmin) return nullopt;
		}
		return tmin;
	}

}

namespace app::text
{
	using namespace ebs;

	struct word_stats
	{
		size_t words = 0;
		size_t unique = 0;
		size_t lines = 0;
		vector<pair<string, size_t>> top;
	};

	inline word_stats analyze(string_view text, size_t topCount)
	{
		word_stats stats;
		hash_map<string, size_t> counts;
		for (string_view line : split(text, '\n', false))
		{
			++stats.lines;
			size_t i = 0;
			while (i < line.size())
			{
				while (i < line.size() && !is_alpha(line[i])) ++i;
				const size_t start = i;
				while (i < line.size() && (is_alpha(line[i]) || is_digit(line[i]))) ++i;
				if (i > start)
				{
					++counts[to_lower(line.substr(start, i - start))];
					++stats.words;
				}
			}
		}
		stats.unique = counts.size();
		for (auto& kv : counts) stats.top.emplace_back(kv.first, kv.second);
		stable_sort(stats.top.begin(), stats.top.end(), [](const auto& a, const auto& b)
		{
			return a.second > b.second || (a.second == b.second && a.first < b.first);
		});
		if (stats.top.size() > topCount) stats.top.erase(stats.top.begin() + topCount, stats.top.end());
		return stats;
	}

}

namespace app
{
	using namespace ebs;

	struct result_sink
	{
		uint64_t checksum = 0;
		void add(uint64_t v) { checksum = hash_combine(size_t(checksum), size_t(v)); }
		void add(double v) { add(uint64_t(int64_t(v * 1000.0))); }
		void add(string_view s) { add(uint64_t(hash<string_view>()(s))); }
	};
}

extern "C" uint64_t RunGeometry(uint32_t seed, size_t rays)
{
	using namespace app;
	using namespace app::geometry;
	result_sink sink;
	uint32_t s = seed;
	auto rnd = [&]()
	{
		s = s * 1103515245u + 12345u;
		return double(s >> 8) / double(1u << 24);
	};
	const mat4<double> view = translation(vec3<double>(0, 0, -100)) * rotation_z(0.3);
	if (auto inv = inverse(view)) sink.add((*inv)[0][0] + (*inv)[1][3]);
	aabb<double, 3> box = aabb<double, 3>::empty();
	box.extend(vec3<double>(-10, -10, -10));
	box.extend(vec3<double>(10, 10, 10));
	for (size_t i = 0; i < rays; ++i)
	{
		ray<double> r{ vec3<double>(0, 0, -100), vec3<double>(rnd() - 0.5, rnd() - 0.5, 1.0).normalized() };
		if (auto t = intersect_sphere(r, vec3<double>(0, 0, 0), 5.0)) sink.add(*t);
		if (auto t = intersect_box(r, box)) sink.add(*t);
	}
	return sink.checksum;
}

extern "C" uint64_t RunText(const char* text, size_t size)
{
	using namespace app;
	result_sink sink;
	auto stats = text::analyze(ebs::string_view(text, size), 10);
	sink.add(uint64_t(stats.words));
	for (auto& w : stats.top) sink.add(ebs::format("{}={}", w.first, w.second).view());
	return sink.checksum;
}
