#ifndef FAST_HASH_H
#define FAST_HASH_H

#include<cstddef>
#include<cstdint>
#include<string>
#include<utility>
#include<type_traits>
#include<vector>
#include<tuple>
#include<array>

#define ll long long
#define u64 unsigned long long

inline u64 mix64(u64 x){
	x+=0x9e3779b97f4a7c15ULL;
	x=(x^(x>>30))*0xbf58476d1ce4e5b9ULL;
	x=(x^(x>>27))*0x94d049bb133111ebULL;
	return x^(x>>31);
}

inline u64 mix64_combine(u64 a,u64 b){
	return mix64(a^(mix64(b)+0x9e3779b97f4a7c15ULL+(a<<6)+(a>>2)));
}

inline u64 hash_ull(u64 x){
	return mix64(x);
}

inline u64 hash_ll(ll x){
	return mix64(static_cast<u64>(x));
}

inline u64 hash_uint(uint32_t x){
	return mix64(static_cast<u64>(x));
}

inline u64 hash_int(int32_t x){
	return mix64(static_cast<u64>(static_cast<int64_t>(x)));
}

constexpr u64 FNV_OFFSET=1469598103934665603ULL;
constexpr u64 FNV_PRIME=1099511628211ULL;

inline u64 hash_str(const char* s){
	if(!s)return 0;
	u64 h=FNV_OFFSET;
	while(*s){
		h^=static_cast<unsigned char>(*s++);
		h*=FNV_PRIME;
	}
	return h;
}

inline u64 hash_str(const char* s,size_t len){
	if(!s)return 0;
	u64 h=FNV_OFFSET;
	for(size_t i=0;i<len;i++){
		h^=static_cast<unsigned char>(s[i]);
		h*=FNV_PRIME;
	}
	return h;
}

inline u64 hash_str(const std::string& s){
	return hash_str(s.data(),s.size());
}

inline u64 hash_str_init(){
	return FNV_OFFSET;
}

inline u64 hash_str_append(u64 h,char c){
	h^=static_cast<unsigned char>(c);
	h*=FNV_PRIME;
	return h;
}

inline u64 hash_bytes(const void* data,size_t len){
	if(!data)return 0;
	const unsigned char* p=static_cast<const unsigned char*>(data);
	u64 h=FNV_OFFSET;
	for(size_t i=0;i<len;i++){
		h^=p[i];
		h*=FNV_PRIME;
	}
	return h;
}

template<typename T>
inline u64 hash_ptr(T* p){
	return mix64(static_cast<u64>(reinterpret_cast<uintptr_t>(p)));
}

inline u64 hash_combine(u64 a,u64 b){
	return mix64_combine(a,b);
}

template<typename T>
struct fast_hash;

template<> struct fast_hash<int>{
	u64 operator()(int x)const{
		return hash_int(static_cast<int32_t>(x));
	}
};

template<> struct fast_hash<unsigned int>{
	u64 operator()(unsigned int x)const{
		return hash_uint(static_cast<uint32_t>(x));
	}
};

template<> struct fast_hash<short>{
	u64 operator()(short x)const{
		return hash_int(static_cast<int32_t>(x));
	}
};

template<> struct fast_hash<unsigned short>{
	u64 operator()(unsigned short x)const{
		return hash_uint(static_cast<uint32_t>(x));
	}
};

template<> struct fast_hash<long>{
	u64 operator()(long x)const{
		return hash_ll(static_cast<ll>(x));
	}
};

template<> struct fast_hash<unsigned long>{
	u64 operator()(unsigned long x)const{
		return hash_ull(static_cast<u64>(x));
	}
};

template<> struct fast_hash<long long>{
	u64 operator()(long long x)const{
		return hash_ll(x);
	}
};

template<> struct fast_hash<unsigned long long>{
	u64 operator()(unsigned long long x)const{
		return hash_ull(x);
	}
};

template<> struct fast_hash<char>{
	u64 operator()(char c)const{
		return hash_uint(static_cast<uint32_t>(static_cast<unsigned char>(c)));
	}
};

template<> struct fast_hash<signed char>{
	u64 operator()(signed char c)const{
		return hash_int(static_cast<int32_t>(c));
	}
};

template<> struct fast_hash<unsigned char>{
	u64 operator()(unsigned char c)const{
		return hash_uint(static_cast<uint32_t>(c));
	}
};

template<> struct fast_hash<bool>{
	u64 operator()(bool b)const{
		return hash_uint(b?1u:0u);
	}
};

template<> struct fast_hash<std::string>{
	u64 operator()(const std::string& s)const{
		return hash_str(s);
	}
};

template<> struct fast_hash<const char*>{
	u64 operator()(const char* s)const{
		return hash_str(s);
	}
};

template<> struct fast_hash<std::nullptr_t>{
	u64 operator()(std::nullptr_t)const{
		return 0;
	}
};

template<typename T>
struct fast_hash<T*>{
	u64 operator()(T* p)const{
		return hash_ptr(p);
	}
};

template<typename A,typename B>
struct fast_hash<std::pair<A,B>>{
	u64 operator()(const std::pair<A,B>& p)const{
		u64 ha=fast_hash<A>{}(p.first);
		u64 hb=fast_hash<B>{}(p.second);
		return mix64_combine(ha,hb);
	}
};

template<typename T>
struct fast_hash<std::vector<T>>{
	u64 operator()(const std::vector<T>& v)const{
		u64 h=FNV_OFFSET;
		fast_hash<T> hasher;
		for(const auto& x:v){
			h=mix64_combine(h,hasher(x));
		}
		return h;
	}
};

template<>
struct fast_hash<std::vector<bool>>{
	u64 operator()(const std::vector<bool>& v)const{
		u64 h=FNV_OFFSET;
		for(size_t i=0;i<v.size();i++){
			h=mix64_combine(h,hash_uint(v[i]?1u:0u));
		}
		return h;
	}
};

template<typename T,size_t N>
struct fast_hash<std::array<T,N>>{
	u64 operator()(const std::array<T,N>& a)const{
		u64 h=FNV_OFFSET;
		fast_hash<T> hasher;
		for(size_t i=0;i<N;i++){
			h=mix64_combine(h,hasher(a[i]));
		}
		return h;
	}
};

template<size_t I,typename... Ts>
struct tuple_hasher{
	static u64 apply(const std::tuple<Ts...>& t,u64 h){
		using elem=typename std::tuple_element<I,std::tuple<Ts...>>::type;
		h=mix64_combine(h,fast_hash<elem>{}(std::get<I>(t)));
		return tuple_hasher<I-1,Ts...>::apply(t,h);
	}
};

template<typename... Ts>
struct tuple_hasher<0,Ts...>{
	static u64 apply(const std::tuple<Ts...>& t,u64 h){
		using elem=typename std::tuple_element<0,std::tuple<Ts...>>::type;
		return mix64_combine(h,fast_hash<elem>{}(std::get<0>(t)));
	}
};

template<>
struct fast_hash<std::tuple<>>{
	u64 operator()(const std::tuple<>&)const{
		return FNV_OFFSET;
	}
};

template<typename T0,typename... Ts>
struct fast_hash<std::tuple<T0,Ts...>>{
	u64 operator()(const std::tuple<T0,Ts...>& t)const{
		return tuple_hasher<sizeof...(Ts),T0,Ts...>::apply(t,FNV_OFFSET);
	}
};

#endif //FAST_HASH_H
