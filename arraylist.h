#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include<vector>
#include<cstddef>
#include<stdexcept>
#include<iterator>
#include<utility>
#include<algorithm>

using ll = long long;

struct Node{
	ll prev;
	ll next;
	ll data;
	bool alive;
};

class ArrayList{
private:
	std::vector<Node>nodes;
	ll head;
	ll tail;
	ll cnt;
	ll free_head;
	
	ll newnode(ll value){
		ll idx;
		if(free_head!=-1){
			idx=free_head;
			free_head=nodes[idx].next;
			nodes[idx].data=value;
			nodes[idx].prev=-1;
			nodes[idx].next=-1;
			nodes[idx].alive=true;
		}else{
			Node n;
			n.data=value;
			n.prev=-1;
			n.next=-1;
			n.alive=true;
			nodes.push_back(n);
			idx=nodes.size()-1;
		}
		return idx;
	}
	
public:
	ArrayList()noexcept:head(-1),tail(-1),cnt(0),free_head(-1){}
	
	ArrayList(const ArrayList& other):head(-1),tail(-1),cnt(0),free_head(-1){
		for(auto it=other.begin();it!=other.end();++it)push_back(*it);
	}
	
	ArrayList& operator=(const ArrayList& other){
		if(this==&other)return *this;
		clear();
		for(auto it=other.begin();it!=other.end();++it)push_back(*it);
		return *this;
	}
	
	ArrayList(ArrayList&& other)noexcept
	:nodes(std::move(other.nodes)),
	head(other.head),tail(other.tail),
	cnt(other.cnt),free_head(other.free_head){
		other.head=-1;
		other.tail=-1;
		other.cnt=0;
		other.free_head=-1;
	}
	
	ArrayList& operator=(ArrayList&& other)noexcept{
		if(this!=&other){
			nodes=std::move(other.nodes);
			head=other.head;
			tail=other.tail;
			cnt=other.cnt;
			free_head=other.free_head;
			other.head=-1;
			other.tail=-1;
			other.cnt=0;
			other.free_head=-1;
		}
		return *this;
	}
	
	class const_it;
	
	class it{
	private:
		ArrayList* list;
		ll idx;
		friend class ArrayList;
		friend class const_it;
	public:
		using iterator_category=std::bidirectional_iterator_tag;
		using value_type=ll;
		using difference_type=std::ptrdiff_t;
		using pointer=ll*;
		using reference=ll&;
		
		it(ArrayList* l,ll i):list(l),idx(i){}
		
		ArrayList* owner()const noexcept{ return list; }
		ll index()const noexcept{ return idx; }
		
		ll& operator*()const{
			return list->nodes[idx].data;
		}
		ll* operator->()const{
			return &list->nodes[idx].data;
		}
		it& operator++(){
			idx=list->nodes[idx].next;
			return *this;
		}
		it operator++(int){
			it tmp=*this;
			++(*this);
			return tmp;
		}
		it& operator--(){
			if(idx==-1)idx=list->tail;
			else idx=list->nodes[idx].prev;
			return *this;
		}
		it operator--(int){
			it tmp=*this;
			--(*this);
			return tmp;
		}
		bool operator==(const it& o)const{
			return list==o.list&&idx==o.idx;
		}
		bool operator!=(const it& o)const{
			return !(*this==o);
		}
		bool is_valid()const{
			if(list==nullptr)return false;
			if(idx==-1)return true;
			return idx>=0&&idx<(ll)list->nodes.size()&&list->nodes[idx].alive;
		}
	};
	
	class const_it{
	private:
		const ArrayList* list;
		ll idx;
		friend class ArrayList;
	public:
		using iterator_category=std::bidirectional_iterator_tag;
		using value_type=ll;
		using difference_type=std::ptrdiff_t;
		using pointer=const ll*;
		using reference=const ll&;
		
		const_it(const ArrayList* l,ll i):list(l),idx(i){}
		const_it(const it& o):list(o.list),idx(o.idx){}
		
		const ArrayList* owner()const noexcept{ return list; }
		ll index()const noexcept{ return idx; }
		
		const ll& operator*()const{
			return list->nodes[idx].data;
		}
		const ll* operator->()const{
			return &list->nodes[idx].data;
		}
		const_it& operator++(){
			idx=list->nodes[idx].next;
			return *this;
		}
		const_it operator++(int){
			const_it tmp=*this;
			++(*this);
			return tmp;
		}
		const_it& operator--(){
			if(idx==-1)idx=list->tail;
			else idx=list->nodes[idx].prev;
			return *this;
		}
		const_it operator--(int){
			const_it tmp=*this;
			--(*this);
			return tmp;
		}
		bool operator==(const const_it& o)const{
			return list==o.list&&idx==o.idx;
		}
		bool operator!=(const const_it& o)const{
			return !(*this==o);
		}
		bool is_valid()const{
			if(list==nullptr)return false;
			if(idx==-1)return true;
			return idx>=0&&idx<(ll)list->nodes.size()&&list->nodes[idx].alive;
		}
	};
	
	using value_type=ll;
	using size_type=std::size_t;
	using iterator=it;
	using const_iterator=const_it;
	using reverse_iterator=std::reverse_iterator<it>;
	using const_reverse_iterator=std::reverse_iterator<const_it>;
	
	it begin()noexcept{ return it(this,head); }
	it end()noexcept{ return it(this,-1); }
	const_it begin()const noexcept{ return const_it(this,head); }
	const_it end()const noexcept{ return const_it(this,-1); }
	const_it cbegin()const noexcept{ return const_it(this,head); }
	const_it cend()const noexcept{ return const_it(this,-1); }
	
	reverse_iterator rbegin()noexcept{ return reverse_iterator(end()); }
	reverse_iterator rend()noexcept{ return reverse_iterator(begin()); }
	const_reverse_iterator rbegin()const noexcept{ return const_reverse_iterator(end()); }
	const_reverse_iterator rend()const noexcept{ return const_reverse_iterator(begin()); }
	const_reverse_iterator crbegin()const noexcept{ return const_reverse_iterator(cend()); }
	const_reverse_iterator crend()const noexcept{ return const_reverse_iterator(cbegin()); }
	
	ll size()const noexcept{ return cnt; }
	bool empty()const noexcept{ return head==-1; }
	std::size_t capacity()const noexcept{ return nodes.capacity(); }
	void reserve(std::size_t n){ nodes.reserve(n); }
	
	void push_back(ll value){
		ll idx=newnode(value);
		if(head==-1){
			head=tail=idx;
			++cnt;
			return ;
		}
		nodes[tail].next=idx;
		nodes[idx].prev=tail;
		tail=idx;
		++cnt;
	}
	
	void push_front(ll value){
		ll idx=newnode(value);
		if(head==-1){
			head=tail=idx;
			++cnt;
			return ;
		}
		nodes[idx].next=head;
		nodes[head].prev=idx;
		head=idx;
		++cnt;
	}
	
	template<typename... Args>
	void emplace_back(Args&&... args){
		push_back(ll(std::forward<Args>(args)...));
	}
	
	template<typename... Args>
	void emplace_front(Args&&... args){
		push_front(ll(std::forward<Args>(args)...));
	}
	
	template<typename... Args>
	it emplace(it pos,Args&&... args){
		return insert(pos,ll(std::forward<Args>(args)...));
	}
	
	it erase(it pos){
		if(pos.list!=this)throw std::invalid_argument("erase: iterator from another list");
		if(pos.idx==-1)return it(this,-1);
		if(!pos.is_valid())throw std::invalid_argument("erase: invalid iterator");
		
		ll idx=pos.idx;
		ll p=nodes[idx].prev;
		ll n=nodes[idx].next;
		if(p!=-1)nodes[p].next=n;
		else head=n;
		if(n!=-1)nodes[n].prev=p;
		else tail=p;
		
		nodes[idx].alive=false;
		nodes[idx].prev=-1;
		nodes[idx].next=free_head;
		nodes[idx].data=ll{};
		free_head=idx;
		
		--cnt;
		return it(this,n);
	}
	
	it insert(it pos,ll value){
		if(pos.list!=this)throw std::invalid_argument("insert: iterator from another list");
		if(pos.idx!=-1&&!pos.is_valid())throw std::invalid_argument("insert: invalid iterator");
		if(head==-1||pos.idx==head){
			push_front(value);
			return begin();
		}
		if(pos.idx==-1){
			push_back(value);
			return it(this,tail);
		}
		ll cur=pos.idx;
		ll p=nodes[cur].prev;
		ll idx=newnode(value);
		nodes[idx].prev=p;
		nodes[idx].next=cur;
		nodes[p].next=idx;
		nodes[cur].prev=idx;
		++cnt;
		return it(this,idx);
	}
	
	template<typename InputIt>
	it insert(it pos,InputIt first,InputIt last){
		if(first==last)return pos;
		it ret=insert(pos,*first);
		++first;
		for(;first!=last;++first){
			insert(pos,*first);
		}
		return ret;
	}
	
	void assign(std::size_t n,ll value){
		clear();
		for(std::size_t i=0;i<n;++i)push_back(value);
	}
	
	template<typename InputIt>
	void assign(InputIt first,InputIt last){
		ArrayList tmp;
		for(;first!=last;++first)tmp.push_back(*first);
		std::swap(nodes,tmp.nodes);
		std::swap(head,tmp.head);
		std::swap(tail,tmp.tail);
		std::swap(cnt,tmp.cnt);
		std::swap(free_head,tmp.free_head);
	}
	
	void clear(){
		nodes.clear();
		head=-1;
		tail=-1;
		cnt=0;
		free_head=-1;
	}
	
	ll& front(){
		if(head==-1)throw std::out_of_range("front() on empty list");
		return nodes[head].data;
	}
	const ll& front()const{
		if(head==-1)throw std::out_of_range("front() on empty list");
		return nodes[head].data;
	}
	ll& back(){
		if(tail==-1)throw std::out_of_range("back() on empty list");
		return nodes[tail].data;
	}
	const ll& back()const{
		if(tail==-1)throw std::out_of_range("back() on empty list");
		return nodes[tail].data;
	}
	
	void pop_front(){ if(head!=-1)erase(it(this,head)); }
	void pop_back(){ if(tail!=-1)erase(it(this,tail)); }
	
	ll& at(std::size_t k){
		if(k>=(std::size_t)cnt)throw std::out_of_range("at() out of range");
		ll cur=head;
		for(std::size_t i=0;i<k;++i)cur=nodes[cur].next;
		return nodes[cur].data;
	}
	const ll& at(std::size_t k)const{
		if(k>=(std::size_t)cnt)throw std::out_of_range("at() out of range");
		ll cur=head;
		for(std::size_t i=0;i<k;++i)cur=nodes[cur].next;
		return nodes[cur].data;
	}
	
	void reverse(){
		ll cur=head;
		while(cur!=-1){
			ll n=nodes[cur].next;
			std::swap(nodes[cur].prev,nodes[cur].next);
			cur=n;
		}
		std::swap(head,tail);
	}
	
	void remove(const ll& value){
		auto i=begin();
		while(i!=end()){
			if(*i==value)i=erase(i);
			else ++i;
		}
	}
	
	template<typename Pred>
	void remove_if(Pred pred){
		auto i=begin();
		while(i!=end()){
			if(pred(*i))i=erase(i);
			else ++i;
		}
	}
	
	void unique(){
		if(empty())return;
		auto i=begin();
		auto p=i++;
		while(i!=end()){
			if(*i==*p)i=erase(i);
			else{ p=i; ++i; }
		}
	}

	void splice(it pos,ArrayList& other){
		if(pos.list!=this)throw std::invalid_argument("splice: iterator from another list");
		if(this==&other)return;
		if(other.empty())return;
		for(auto i=other.begin();i!=other.end();++i){
			insert(pos,*i);
		}
		other.clear();
	}
	
	void swap(ArrayList& other)noexcept{
		std::swap(nodes,other.nodes);
		std::swap(head,other.head);
		std::swap(tail,other.tail);
		std::swap(cnt,other.cnt);
		std::swap(free_head,other.free_head);
	}
	
	const std::vector<Node>& raw()const noexcept{ return nodes; }
};

inline bool operator==(const ArrayList::it& a,const ArrayList::const_it& b){
	return a.owner()==b.owner()&&a.index()==b.index();
}
inline bool operator==(const ArrayList::const_it& a,const ArrayList::it& b){
	return a.owner()==b.owner()&&a.index()==b.index();
}
inline bool operator!=(const ArrayList::it& a,const ArrayList::const_it& b){
	return !(a==b);
}
inline bool operator!=(const ArrayList::const_it& a,const ArrayList::it& b){
	return !(a==b);
}

inline bool operator==(const ArrayList& a,const ArrayList& b){
	if(a.size()!=b.size())return false;
	auto ia=a.begin();
	auto ib=b.begin();
	auto ea=a.end();
	while(ia!=ea){
		if(*ia!=*ib)return false;
		++ia;
		++ib;
	}
	return true;
}

inline bool operator!=(const ArrayList& a,const ArrayList& b){
	return !(a==b);
}

inline bool operator<(const ArrayList& a,const ArrayList& b){
	auto ia=a.begin();
	auto ib=b.begin();
	auto ea=a.end();
	auto eb=b.end();
	while(ia!=ea&&ib!=eb){
		if(*ia<*ib)return true;
		if(*ib<*ia)return false;
		++ia;
		++ib;
	}
	return (ia==ea)&&(ib!=eb);
}

inline bool operator>(const ArrayList& a,const ArrayList& b){ return b<a; }
inline bool operator<=(const ArrayList& a,const ArrayList& b){ return !(b<a); }
inline bool operator>=(const ArrayList& a,const ArrayList& b){ return !(a<b); }

inline void swap(ArrayList& a,ArrayList& b)noexcept{
	a.swap(b);
}

#endif //ARRAYLIST_H
