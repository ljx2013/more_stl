#ifndef LOGIC_H
#define LOGIC_H

#include<vector>
#include<utility>
#include<cstddef>
#include<cstdint>
#include<stdexcept>
#include<algorithm>
#include<ostream>

enum class TriBool:std::int8_t{ False=0,True=1,Unknown=2 };

inline TriBool tri_not(TriBool a)noexcept{
	switch(a){
		case TriBool::True:  return TriBool::False;
		case TriBool::False: return TriBool::True;
		default:             return TriBool::Unknown;
	}
}

inline TriBool tri_and(TriBool a,TriBool b)noexcept{
	if(a==TriBool::False||b==TriBool::False)return TriBool::False;
	if(a==TriBool::Unknown||b==TriBool::Unknown)return TriBool::Unknown;
	return TriBool::True;
}

inline TriBool tri_or(TriBool a,TriBool b)noexcept{
	if(a==TriBool::True||b==TriBool::True)return TriBool::True;
	if(a==TriBool::Unknown||b==TriBool::Unknown)return TriBool::Unknown;
	return TriBool::False;
}

inline TriBool tri_xor(TriBool a,TriBool b)noexcept{
	if(a==TriBool::Unknown||b==TriBool::Unknown)return TriBool::Unknown;
	return (a==b)?TriBool::False:TriBool::True;
}

inline TriBool tri_implies(TriBool a,TriBool b)noexcept{
	return tri_or(tri_not(a),b);
}

inline TriBool tri_equiv(TriBool a,TriBool b)noexcept{
	return tri_and(tri_implies(a,b),tri_implies(b,a));
}

inline TriBool tri_nxor(TriBool a,TriBool b)noexcept{
	return tri_equiv(a,b);
}

inline TriBool tri_nand(TriBool a,TriBool b)noexcept{
	return tri_not(tri_and(a,b));
}

inline TriBool tri_nor(TriBool a,TriBool b)noexcept{
	return tri_not(tri_or(a,b));
}

inline bool tri_to_bool(TriBool t,bool unknown_value=false)noexcept{
	switch(t){
		case TriBool::True:  return true;
		case TriBool::False: return false;
		default:             return unknown_value;
	}
}

inline const char* tri_to_string(TriBool t)noexcept{
	switch(t){
		case TriBool::True:  return "T";
		case TriBool::False: return "F";
		default:             return "?";
	}
}

inline std::ostream& operator<<(std::ostream& os,TriBool t){
	return os<<tri_to_string(t);
}

inline bool logic_and(bool a,bool b)noexcept{ return a&&b; }
inline bool logic_or (bool a,bool b)noexcept{ return a||b; }
inline bool logic_not(bool a)noexcept{ return !a; }
inline bool logic_xor(bool a,bool b)noexcept{ return a!=b; }
inline bool logic_nand(bool a,bool b)noexcept{ return !(a&&b); }
inline bool logic_nor(bool a,bool b)noexcept{ return !(a||b); }
inline bool logic_implies(bool a,bool b)noexcept{ return !a||b; }
inline bool logic_equiv(bool a,bool b)noexcept{ return a==b; }

enum class LogicOp{ And,Or,Not,Xor,Nand,Nor,Implies,Equiv };

inline bool is_unary(LogicOp op)noexcept{
	return op==LogicOp::Not;
}

inline bool is_binary(LogicOp op)noexcept{
	return !is_unary(op);
}

inline bool apply_op(LogicOp op,bool a,bool b=false)noexcept{
	switch(op){
		case LogicOp::And:     return logic_and(a,b);
		case LogicOp::Or:      return logic_or(a,b);
		case LogicOp::Not:     return logic_not(a);
		case LogicOp::Xor:     return logic_xor(a,b);
		case LogicOp::Nand:    return logic_nand(a,b);
		case LogicOp::Nor:     return logic_nor(a,b);
		case LogicOp::Implies: return logic_implies(a,b);
		case LogicOp::Equiv:   return logic_equiv(a,b);
	}
	return false;
}

inline TriBool apply_tri_op(LogicOp op,TriBool a,TriBool b=TriBool::False)noexcept{
	switch(op){
		case LogicOp::And:     return tri_and(a,b);
		case LogicOp::Or:      return tri_or(a,b);
		case LogicOp::Not:     return tri_not(a);
		case LogicOp::Xor:     return tri_xor(a,b);
		case LogicOp::Nand:    return tri_nand(a,b);
		case LogicOp::Nor:     return tri_nor(a,b);
		case LogicOp::Implies: return tri_implies(a,b);
		case LogicOp::Equiv:   return tri_equiv(a,b);
	}
	return TriBool::Unknown;
}

constexpr std::size_t MAX_TRUTH_TABLE_VARS=30;

inline std::size_t truth_table_size(std::size_t n){
	if(n>MAX_TRUTH_TABLE_VARS)
		throw std::length_error("truth_table: n > 30");
	return std::size_t(1)<<n;
}

template<typename Func>
std::vector<std::pair<std::vector<bool>,bool>> truth_table(std::size_t n,Func f){
	std::size_t total=truth_table_size(n);
	std::vector<std::pair<std::vector<bool>,bool>> r;
	r.reserve(total);
	for(std::size_t mask=0;mask<total;++mask){
		std::vector<bool> vars(n);
		for(std::size_t i=0;i<n;++i)vars[i]=(mask>>i)&1u;
		r.emplace_back(vars,f(vars));
	}
	return r;
}

template<std::size_t N,typename Func,std::size_t... Is>
bool call_with_vars(Func& f,const std::vector<bool>& v,std::index_sequence<Is...>){
	return f(v[Is]...);
}

template<std::size_t N,typename Func>
std::vector<std::pair<std::vector<bool>,bool>> truth_table_n(Func f){
	return truth_table(N,[&](const std::vector<bool>& v){
		return call_with_vars<N>(f,v,std::make_index_sequence<N>{});
	});
}

template<typename Func>
auto truth_table_1(Func f){
	return truth_table_n<1>(f);
}

template<typename Func>
auto truth_table_2(Func f){
	return truth_table_n<2>(f);
}

template<typename Func>
auto truth_table_3(Func f){
	return truth_table_n<3>(f);
}

constexpr std::size_t MAX_TRUTH_TABLE_TRI_VARS=12;

inline std::size_t truth_table_tri_size(std::size_t n){
	if(n>MAX_TRUTH_TABLE_TRI_VARS)
		throw std::length_error("truth_table_tri: n > 12");
	std::size_t total=1;
	for(std::size_t i=0;i<n;++i)total*=3;
	return total;
}

template<typename Func>
std::vector<std::pair<std::vector<TriBool>,TriBool>> truth_table_tri(std::size_t n,Func f){
	std::size_t total=truth_table_tri_size(n);
	std::vector<std::pair<std::vector<TriBool>,TriBool>> r;
	r.reserve(total);
	for(std::size_t mask=0;mask<total;++mask){
		std::size_t m=mask;
		std::vector<TriBool> vars(n);
		for(std::size_t i=0;i<n;++i){
			vars[i]=static_cast<TriBool>(m%3);
			m/=3;
		}
		r.emplace_back(vars,f(vars));
	}
	return r;
}

inline bool is_tautology(const std::vector<bool>& col)noexcept{
	return std::all_of(col.begin(),col.end(),[](bool b){ return b; });
}

inline bool is_contradiction(const std::vector<bool>& col)noexcept{
	return std::none_of(col.begin(),col.end(),[](bool b){ return b; });
}

inline bool is_contingent(const std::vector<bool>& col)noexcept{
	return !is_tautology(col)&&!is_contradiction(col);
}

inline bool is_tautology(const std::vector<TriBool>& col)noexcept{
	return std::all_of(col.begin(),col.end(),[](TriBool b){ return b==TriBool::True; });
}

inline bool is_contradiction(const std::vector<TriBool>& col)noexcept{
	return std::none_of(col.begin(),col.end(),[](TriBool b){ return b==TriBool::True; });
}

inline bool is_contingent(const std::vector<TriBool>& col)noexcept{
	return !is_tautology(col)&&!is_contradiction(col);
}

inline std::vector<bool> eval_column(LogicOp op,std::size_t n){
	if(n<1)throw std::invalid_argument("eval_column: n must be >= 1");
	if(n>2)throw std::invalid_argument("eval_column: n must be <= 2");
	std::size_t total=truth_table_size(n);
	std::vector<bool> col;
	col.reserve(total);
	for(std::size_t mask=0;mask<total;++mask){
		std::vector<bool> vars(n);
		for(std::size_t i=0;i<n;++i)vars[i]=(mask>>i)&1u;
		bool r;
		if(is_unary(op))r=apply_op(op,vars[0]);
		else             r=apply_op(op,vars[0],vars[1]);
		col.push_back(r);
	}
	return col;
}

inline std::vector<TriBool> eval_tri_column(LogicOp op,std::size_t n){
	if(n<1)throw std::invalid_argument("eval_tri_column: n must be >= 1");
	if(n>2)throw std::invalid_argument("eval_tri_column: n must be <= 2");
	std::size_t total=truth_table_tri_size(n);
	std::vector<TriBool> col;
	col.reserve(total);
	for(std::size_t mask=0;mask<total;++mask){
		std::size_t m=mask;
		std::vector<TriBool> vars(n);
		for(std::size_t i=0;i<n;++i){
			vars[i]=static_cast<TriBool>(m%3);
			m/=3;
		}
		TriBool r;
		if(is_unary(op))r=apply_tri_op(op,vars[0]);
		else             r=apply_tri_op(op,vars[0],vars[1]);
		col.push_back(r);
	}
	return col;
}

inline void print_truth_table(
							  const std::vector<std::pair<std::vector<bool>,bool>>& table,
							  std::ostream& os)
{
	std::size_t n=table.empty()?0:table[0].first.size();
	for(std::size_t i=0;i<n;++i)os<<"v"<<i<<" ";
	os<<"| result\n";
	for(const auto& row:table){
		for(bool b:row.first)os<<(b?"T ":"F ");
		os<<"| "<<(row.second?"T":"F")<<"\n";
	}
}

inline void print_truth_table_tri(
								  const std::vector<std::pair<std::vector<TriBool>,TriBool>>& table,
								  std::ostream& os)
{
	std::size_t n=table.empty()?0:table[0].first.size();
	for(std::size_t i=0;i<n;++i)os<<"v"<<i<<" ";
	os<<"| result\n";
	for(const auto& row:table){
		for(TriBool b:row.first)os<<tri_to_string(b)<<" ";
		os<<"| "<<tri_to_string(row.second)<<"\n";
	}
}

#endif //LOGIC_H
