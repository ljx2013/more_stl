#ifndef MATH_SET_H
#define MATH_SET_H

#include<set>
#include<vector>
#include<utility>
#include<initializer_list>
#include<algorithm>
#include<iterator>
#include<cstddef>
#include<stdexcept>
#include<limits>

template<typename T>
class MathSet{
private:
	std::set<T>s;
public:
	using value_type=T;
	using size_type=std::size_t;
	using iterator=typename std::set<T>::iterator;
	using const_iterator=typename std::set<T>::const_iterator;
	
	MathSet()=default;
	MathSet(std::initializer_list<T> init):s(init){}
	
	template<typename InputIt>
	MathSet(InputIt first,InputIt last):s(first,last){}
	
	size_type size()const noexcept{ return s.size(); }
	bool empty()const noexcept{ return s.empty(); }
	bool is_empty()const noexcept{ return s.empty(); }
	void clear()noexcept{ s.clear(); }
	
	iterator begin()noexcept{ return s.begin(); }
	iterator end()noexcept{ return s.end(); }
	const_iterator begin()const noexcept{ return s.begin(); }
	const_iterator end()const noexcept{ return s.end(); }
	const_iterator cbegin()const noexcept{ return s.cbegin(); }
	const_iterator cend()const noexcept{ return s.cend(); }
	
	bool contains(const T& x)const{ return s.find(x)!=s.end(); }
	size_type count(const T& x)const{ return s.count(x); }
	
	iterator find(const T& x){ return s.find(x); }
	const_iterator find(const T& x)const{ return s.find(x); }
	iterator lower_bound(const T& x){ return s.lower_bound(x); }
	const_iterator lower_bound(const T& x)const{ return s.lower_bound(x); }
	iterator upper_bound(const T& x){ return s.upper_bound(x); }
	const_iterator upper_bound(const T& x)const{ return s.upper_bound(x); }
	
	std::pair<iterator,bool> insert(const T& x){ return s.insert(x); }
	
	template<typename InputIt>
	void insert(InputIt first,InputIt last){ s.insert(first,last); }
	
	template<typename... Args>
	std::pair<iterator,bool> emplace(Args&&... args){
		return s.emplace(std::forward<Args>(args)...);
	}
	
	size_type erase(const T& x){ return s.erase(x); }
	iterator erase(const_iterator pos){ return s.erase(pos); }
	iterator erase(const_iterator first,const_iterator last){ return s.erase(first,last); }
	
	void swap(MathSet& o)noexcept{ s.swap(o.s); }
	
	bool operator==(const MathSet& o)const{ return s==o.s; }
	bool operator!=(const MathSet& o)const{ return s!=o.s; }
	
	bool operator<(const MathSet& o)const{
		return std::lexicographical_compare(
											s.begin(),s.end(),o.s.begin(),o.s.end());
	}
	bool operator>(const MathSet& o)const{ return o<*this; }
	bool operator<=(const MathSet& o)const{ return !(o<*this); }
	bool operator>=(const MathSet& o)const{ return !(*this<o); }
	
	bool is_subset_of(const MathSet& o)const{
		if(s.size()>o.s.size())return false;
		return std::includes(o.s.begin(),o.s.end(),s.begin(),s.end());
	}
	bool is_proper_subset_of(const MathSet& o)const{
		return s.size()<o.s.size()&&is_subset_of(o);
	}
	bool is_superset_of(const MathSet& o)const{ return o.is_subset_of(*this); }
	bool is_proper_superset_of(const MathSet& o)const{ return o.is_proper_subset_of(*this); }
	
	bool is_disjoint_with(const MathSet& o)const{
		auto ia=s.begin();
		auto ib=o.s.begin();
		while(ia!=s.end()&&ib!=o.s.end()){
			if(*ia<*ib)++ia;
			else if(*ib<*ia)++ib;
			else return false;
		}
		return true;
	}
	
	MathSet union_with(const MathSet& o)const{
		MathSet r;
		std::set_union(s.begin(),s.end(),o.s.begin(),o.s.end(),
					   std::inserter(r.s,r.s.begin()));
		return r;
	}
	MathSet intersection(const MathSet& o)const{
		MathSet r;
		std::set_intersection(s.begin(),s.end(),o.s.begin(),o.s.end(),
							  std::inserter(r.s,r.s.begin()));
		return r;
	}
	MathSet difference(const MathSet& o)const{
		MathSet r;
		std::set_difference(s.begin(),s.end(),o.s.begin(),o.s.end(),
							std::inserter(r.s,r.s.begin()));
		return r;
	}
	MathSet symmetric_difference(const MathSet& o)const{
		MathSet r;
		std::set_symmetric_difference(s.begin(),s.end(),o.s.begin(),o.s.end(),
									  std::inserter(r.s,r.s.begin()));
		return r;
	}
	
	MathSet complement_in(const MathSet& universe)const{
		return universe.difference(*this);
	}
	
	MathSet operator|(const MathSet& o)const{ return union_with(o); }
	MathSet operator&(const MathSet& o)const{ return intersection(o); }
	MathSet operator-(const MathSet& o)const{ return difference(o); }
	MathSet operator^(const MathSet& o)const{ return symmetric_difference(o); }
	
	MathSet& operator|=(const MathSet& o){
		s.insert(o.s.begin(),o.s.end());
		return *this;
	}
	MathSet& operator&=(const MathSet& o){
		for(auto it=s.begin();it!=s.end();){
			if(!o.s.count(*it))it=s.erase(it);
			else ++it;
		}
		return *this;
	}
	MathSet& operator-=(const MathSet& o){
		for(const auto& x:o.s)s.erase(x);
		return *this;
	}
	MathSet& operator^=(const MathSet& o){
		MathSet tmp=symmetric_difference(o);
		s=std::move(tmp.s);
		return *this;
	}
	
	std::vector<std::pair<T,T>> cartesian_product(const MathSet& o)const{
		size_type a=s.size();
		size_type b=o.s.size();
		if(b!=0&&a>std::numeric_limits<size_type>::max()/b)
			throw std::length_error("cartesian_product: size overflow");
		std::vector<std::pair<T,T>> r;
		r.reserve(a*b);
		for(const auto& x:s)
			for(const auto& y:o.s)
				r.emplace_back(x,y);
		return r;
	}
	
	std::vector<MathSet> power_set()const{
		if(s.size()>20)throw std::length_error("power_set: |S| > 20");
		std::vector<T> v(s.begin(),s.end());
		size_type n=v.size();
		size_type total=size_type(1)<<n;
		std::vector<MathSet> r;
		r.reserve(total);
		for(size_type mask=0;mask<total;++mask){
			MathSet sub;
			for(size_type i=0;i<n;++i){
				if(mask&(size_type(1)<<i))sub.s.insert(v[i]);
			}
			r.push_back(std::move(sub));
		}
		return r;
	}
	
	template<typename Func>
	MathSet filter(Func pred)const{
		MathSet r;
		for(const auto& x:s){
			if(pred(x))r.s.insert(x);
		}
		return r;
	}
	
	template<typename Func>
	auto map(Func f)const
	-> MathSet<typename std::decay<decltype(f(std::declval<T>()))>::type>
	{
		using R=typename std::decay<decltype(f(std::declval<T>()))>::type;
		MathSet<R> r;
		for(const auto& x:s)r.insert(f(x));
		return r;
	}
	
	const std::set<T>& raw()const noexcept{ return s; }
};

template<typename T>
void swap(MathSet<T>& a,MathSet<T>& b)noexcept{
	a.swap(b);
}

template<typename T>
MathSet<T> make_mathset(std::initializer_list<T> init){
	return MathSet<T>(init);
}

template<typename T>
MathSet<T> set_union(const MathSet<T>& a,const MathSet<T>& b){ return a|b; }

template<typename T>
MathSet<T> set_intersection(const MathSet<T>& a,const MathSet<T>& b){ return a&b; }

template<typename T>
MathSet<T> set_difference(const MathSet<T>& a,const MathSet<T>& b){ return a-b; }

template<typename T>
MathSet<T> set_symmetric_difference(const MathSet<T>& a,const MathSet<T>& b){ return a^b; }

template<typename T>
MathSet<T> set_complement(const MathSet<T>& universe,const MathSet<T>& a){
	return universe-a;
}

#endif //MATH_SET_H
