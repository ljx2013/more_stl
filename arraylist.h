#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include<vector>
#include<cstddef>
#include<stdexcept>

#define MAX_NODE 11184810
#define SIZE_NODE 24
#define ll long long

struct Node{
	ll prev;
	ll next;
	ll data;
};
class ArrayList{
private:
	std::vector<Node>nodes;
	ll head;
	ll tail;
public:
	ArrayList():head(-1),tail(-1){}
	
	class it{
	private:
		ArrayList* list;
		ll idx;
		friend class ArrayList;
	public:
		it(ArrayList* l,ll i):list(l),idx(i){}
		ll& operator*(){
			return list->nodes[idx].data;
		}
		ll* operator->(){
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
			return idx==o.idx;
		}
		bool operator!=(const it& o)const{
			return idx!=o.idx;
		}
	};

	it begin(){
		return it(this,head);
	}
	it end(){
		return it(this,-1);
	}

	ll newnode(ll value){
		Node n;
		n.data=value;
		n.prev=-1;
		n.next=-1;
		nodes.push_back(n);
		return nodes.size()-1;
	}
	void push_back(ll value){
		ll idx=newnode(value);
		if(head==-1){
			head=tail=idx;
			return ;
		}
		nodes[tail].next=idx;
		nodes[idx].prev=tail;
		tail=idx;
	}
	void push_front(ll value){
		ll idx=newnode(value);
		if(head==-1){
			head=tail=idx;
			return ;
		}
		nodes[idx].next=head;
		nodes[head].prev=idx;
		head=idx;
	}
	it erase(it pos){
		ll idx=pos.idx;
		ll p=nodes[idx].prev;
		ll n=nodes[idx].next;
		if(p!=-1)nodes[p].next=n;
		else head=n;
		if(n!=-1)nodes[n].prev=p;
		else tail=p;
		nodes[idx].prev=-1;
		nodes[idx].next=-1;
		return it(this,n);
	}
	it insert(it pos,ll value){
		if(head==-1||pos.idx==head){
			push_front(value);
			return it(this,head);
		}
		ll cur=pos.idx;
		ll p=nodes[cur].prev;
		ll idx=newnode(value);
		nodes[idx].prev=p;
		nodes[idx].next=cur;
		nodes[p].next=idx;
		nodes[cur].prev=idx;
		return it(this,idx);
	}
	bool empty()const{
		return head==-1;
	}
	void clear(){
		nodes.clear();
		head=-1;
		tail=-1;
	}
	ll size()const{
		return nodes.size();
	}
	ll front()const{
		return nodes[head].data;
	}
	ll back()const{
		return nodes[tail].data;
	}
	void pop_front(){
		if(head!=-1) erase(it(this,head));
	}
	void pop_back(){
		if(tail!=-1) erase(it(this,tail));
	}
};

#endif //arraylist.h
