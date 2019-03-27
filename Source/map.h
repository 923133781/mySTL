#pragma once
#ifndef __MAP_H__
#define __MAP_H__

#include "rb_tree.h"		//�ײ�ʵ�ֻ���


__mySTL_BEGIN__

/* ע�⣺mapʹ��RB_tree�� insert_unique ������Ԫ�أ�multimapʹ�� insert_equal */

	//����KeyΪ��ֵ�ͱ�TΪʵֵ�ͱ�ʡȱʹ�õ�������(less)
	template<class Key, class T, class Compare = std::less<Key>, class Alloc = alloc>
	class map {
	public:
		typedef Key							key_type;		//��ֵ�ͱ�
		typedef T							data_type;		//ʵֵ�ͱ�
		typedef T							mapped_type;
		typedef std::pair<const Key, T>		value_type;		//map ��Ԫ���ͱ�
		typedef Compare						key_compare;	//��ֵ�Ƚ��ͱ�

		//���¶���һ���º����������þ��ǵ��ü�ֵ�ȽϺ���
		class value_compare : public binary_function<value_type, value_type, bool> {
			friend class map<Key, T, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) { }
		public:
			bool operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};
	private:
		//ע�⣺���µ�һ���ڶ������ֱ�Ϊ��ֵ��ʵֵ�ͱ�
		//��ֵ�ͱ�Ϊ pair ��һ�ͱ�ʵֵ�ͱ�Ϊ pair
		typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;

		rep_type C;
	public:
		/* ���²�����set��iterator����Ϊrb_tree��const_iterator����Ϊmap��ͨ��iterator�ı�Ԫ��ʵֵ */
		typedef typename rep_type::iterator				iterator;
		typedef typename rep_type::const_iterator		const_iterator;
		typedef typename rep_type::pointer				pointer;
		typedef typename rep_type::const_pointer		const_pointer;
		typedef typename rep_type::reference			reference;
		typedef typename rep_type::const_reference		const_reference;
		typedef typename rep_type::size_type			size_type;
		typedef typename rep_type::difference_type		difference_type;
		//typedef typename rep_type::reverse_iterator	reverse_iterator;

		map() : C(Compare()) { }
		explicit map(const Compare& comp) : C(comp) { }
		template<class InputIterator>
		map(InputIterator first, InputIterator last) : C(Compare()) {
			C.insert_unique(first, last);
		}
		template<class InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp) : C(comp) {
			C.insert_unique(first, last);
		}
		map(const map<Key, T, Compare, Alloc>& x) : C(x.C) { }

		map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& x) {
			C = x.C;
			return *this;
		}

		//�������е���Ϊ��ת���� rb_tree ����Ϊ
		key_compare key_comp() const { return C.key_comp(); }
		value_compare value_comp() const { return value_compare(C.key_comp()); }
		iterator begin() { return C.begin(); }
		const_iterator begin() const { return C.begin(); }
		iterator end() { return C.end(); }
		const_iterator end() const { return C.end(); }
		bool empty() const { return C.empty(); }
		size_type size() const { return C.size(); }
		size_type max_size() const { return C.max_size(); }

		//ע������ �±��������д����Ȼ��Щ���ӣ����Ǻ�����
		T& operator[](const key_type& k) {
			return (*((insert(value_type(k, T()))).first)).second;

			/* ע�⣺insert(value_type(k, T()))���ص���һ�� pair<iterator, bool> */
			/*       ���һԪ��ָ��ɹ����������ͻ�� map ��Ԫ�� pair<Key, T>  */
			/*       ���� *((insert(value_type(k, T()))).first) �������һԪ�أ��õ�һ��mapԪ�� */
			/*       ���� (*((insert(value_type(k, T()))).first)).second �õ� map Ԫ�ص�ʵֵ    */
		}
		void swap(map<Key, T, Compare, Alloc>& x) { C.swap(x.C); }

		//insert/erase
		std::pair<iterator, bool> insert(const value_type& x) {
			return C.insert_unique(x);
		}
		/*iterator insert(iterator pos, const value_type& x) {
			return C.insert_unique(pos, x);
		}*/
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			C.insert_unique(first, last);
		}
	
		void erase(iterator pos) { C.erase(pos); }
		void erase(const key_type& x) { C.erase(x); }
		void erase(iterator first, iterator last) { C.erase(first, last); }
		void clear() { C.clear(); }

		//map operations
		iterator find(const key_type& k) { return C.find(k); }
		const_iterator find(const key_type& k) const { return C.find(k); }
		size_type count(const key_type& k) const { return C.count(k); }

		friend bool operator==(const map&, const map&);
		friend bool operator< (const map&, const map&);
	};

	template<class Key, class T, class Compare, class Alloc>
	inline bool operator==(const map<Key, T, Compare, Alloc>& x,
						   const map<Key, T, Compare, Alloc>& y)
	{
		return x.C == y.C;
	}
	template<class Key, class T, class Compare, class Alloc>
	inline bool operator<(const map<Key, T, Compare, Alloc>& x,
		const map<Key, T, Compare, Alloc>& y)
	{
		return x.C < y.C;
	}


__mySTL_END__

#endif //end of __MAP_H__
