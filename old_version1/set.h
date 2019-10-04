#pragma once
#ifndef __SET_H__
#define __SET_H__

#include "rb_tree.h"		//�ײ�ʵ�ֻ���


__mySTL_BEGIN__

/* ע�⣺setʹ��RB_tree�� insert_unique ������Ԫ�أ�multisetʹ�� insert_equal */

	//ʡȱʹ�õ�������(less)
	template<class Key, class Compare = std::less<Key>, class Alloc = alloc>
	class set {
	public:
		typedef Key key_type;
		typedef Key value_type;		//key_type �� value_type��ͬ
		typedef Compare key_compare;
		typedef Compare value_compare;
	private:
		//ע�⣺���µ�һ���ڶ������ֱ�Ϊ��ֵ��ʵֵ�ͱ𣬶���set��ֵ��ʵֵ�ͱ���ͬ
		typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;

		rep_type C;		//�ײ��������� RB_tree ʵ��set

	public:
		/* ע�⣺���½�set��iterator����Ϊrb_tree��const_iterator����ʾ set ��Ԫ��ֵ�����Ըı� */
		typedef typename rep_type::const_iterator		iterator;
		typedef typename rep_type::const_iterator		const_iterator;
		typedef typename rep_type::pointer				pointer;
		typedef typename rep_type::const_pointer		const_pointer;
		typedef typename rep_type::reference			reference;
		typedef typename rep_type::const_reference		const_reference;
		typedef typename rep_type::size_type			size_type;
		typedef typename rep_type::difference_type		difference_type;
		//typedef typename rep_type::reverse_iterator	reverse_iterator;

		set() : C(Compare()) { }
		explicit set(const Compare& comp) : C(comp) { }
		template<class InputIterator>
		set(InputIterator first, InputIterator last) : C(Compare()) {
			C.insert_unique(first, last);
		}
		template<class InputIterator>
		set(InputIterator first, InputIterator last, const Compare& comp) : C(comp) {
			C.insert_unique(first, last);
		}
		set(const set<Key, Compare, Alloc>& x) : C(x.C) { }

		set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
			C = x.C;
			return *this;
		}

		/* ���µ����в���RB_tree�Ѿ��ṩ��ֻ��Ҫ���ݵ��ü��� */

		//accessors:
		iterator begin() { return C.begin(); }
		iterator end() { return C.end(); }
		key_compare key_comp() const { return C.key_comp(); }
		value_compare value_comp() const { return C.key_comp(); }
		/*reverse_iterator rbegin() const { return C.rbegin(); }
		reverse_iterator rend() const { return C.rend(); }*/
		bool empty() const { return C.empty(); }
		size_type size() const { return C.size(); }
		size_type max_size() const { return C.max_size(); }
		void swap(set<Key, Compare, Alloc>& x) { C.swap(x.C); }

		//insert
		std::pair<iterator, bool> insert(const value_type& x) {
			std::pair<typename rep_type::iterator, bool> p = C.insert_unique(x);
			return p;
			//return std::pair<iterator, bool>(p.first, p.second);
		}
		/*iterator insert(iterator position, const value_type& x) {
			return C.insert_unique(position, x);
		}*/
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			C.insert_unique(first, last);
		}

		//erase
		void erase(iterator position) {
			C.erase(position);
		}
		size_type erase(const key_type& x) {
			return C.erase(x);
		}
		void erase(iterator first, iterator last) {
			C.erase(first, last);
		}
		void clear() { C.clear(); }

		//set operations
		iterator find(const key_type& x) { return C.find(x); }
		size_type count(const key_type& x) { return C.count(x); }
		//iterator lower_bound(const key_type& x) const { return C.lower_bound(x); }
		//iterator upper_bound(const key_type& x) const { return C.upper_bound(x); }
		/* ����Ԫ�� x ������ */
		//std::pair<iterator, iterator> equal_range(const key_type& x) const { return C.equal_range(x); }

		friend bool operator==(const set& x, const set& y);
		friend bool operator< (const set& x, const set& y);
	};

	template<class Key, class Compare, class Alloc>
	inline bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return (x.C == y.C);
	}
	template<class Key, class Compare, class Alloc>
	inline bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
		return (x.C < y.C);
	}


__mySTL_END__

#endif //end of __SET_H__
