#pragma once
#ifndef __LIST_H__
#define __LIST_H__

#include "alloctor.h"		//�ռ����������
#include "construct.h"		//�ڴ����Ĺ��졢�������ع���
#include "algobase.h"		//�����㷨���

__mySTL_BEGIN__

/*	����ڵ�ṹ	*/
	template<class T>
	struct __list_node {
		typedef __list_node<T>* link_type;
		link_type prev;		//�ͱ�Ϊ__list_node<T>*��Ҳ����Ϊ void*
		link_type next;
		T data;
	};

/*	������ list ��������ƣ��ͱ�Ϊ˫�������	*/
	template<class T, class Ref = T&, class Ptr = T*>
	struct __list_iterator {
	public:
		typedef __list_iterator<T, T&, T*>		iterator;
		typedef __list_iterator<T, Ref, Ptr>	self;

		typedef bidirectional_iterator_tag		iterator_category;		//˫�������
		typedef T								value_type;
		typedef Ptr								pointer;
		typedef Ref								reference;
		typedef size_t							size_type;
		typedef ptrdiff_t						difference_type;
		typedef __list_node<T>*					link_type;

		link_type node;		//�������ڲ�ӵ��һ��ָ�룬ָ��list�Ľڵ�

		//constructor
		__list_iterator() { }
		__list_iterator(link_type x) : node(x) { }
		__list_iterator(const iterator& x) : node(x.node) { }

		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
		//���¶Ե�����ȡֵ��ȡ���ǽڵ����ֵ
		reference operator*() const { return (*node).data; }
		//�����ǵ������ĳ�Ա��ȡ�����ӵı�׼����
		pointer operator->() const { return &(operator*()); }		//����*�����ȡ��node�����ݣ�����&ȡ�õ�ַ
		//�Ե�������1����ǰ��һ���ڵ�
		self& operator++() {
			node = (*node).next;
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}
		//�Ե�������1���Ǻ���һ���ڵ�
		self& operator--() {
			node = (*node).prev;
			return *this;
		}
		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}
	};


/*	������list�ṹ��ʵ��	*/
/*	list Ϊ˫��ѭ������ṹ	*/
	template<class T, class Alloc = alloc>		//ȱʡʹ�� alloc Ϊ�ռ�������
	struct list {
	protected:
		typedef __list_node<T> list_node;
	public:
		typedef list_node* link_type;
		typedef typename __list_iterator<T, T&, T*>::iterator			iterator;
		typedef typename __list_iterator<T, T&, T*>::iterator_category	category;
		typedef typename __list_iterator<T, T&, T*>::value_type			value_type;
		typedef typename __list_iterator<T, T&, T*>::pointer			pointer;
		typedef typename __list_iterator<T, T&, T*>::reference			reference;
		typedef typename __list_iterator<T, T&, T*>::size_type			size_type;
		typedef typename __list_iterator<T, T&, T*>::difference_type	difference_type;
	protected:
		link_type node;
		/* ֻ��Ҫһ��ָ�뼴��ʵ��˫��ѭ������Ϊ��ʵ�ֱ�׼��ǰ�պ�	*/
		/* �����ʾ����node ָ��β�˺����һ���հ׽ڵ㣬������������	*/

		//ר���ռ���������ÿ������һ���ڵ��С
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
	
		/* ���ǣ�list_node_allocator(n)��ʾ���� n ���ڵ�ռ䡣���º����ֱ��������á��ͷš����졢����һ���ڵ� */
		//����һ���ڵ㲢����
		link_type get_node() { return list_node_allocator::allocate(); }
		//�ͷ�һ���ڵ�
		void put_node(link_type p) { list_node_allocator::deallocate(p); }
		//���ò�����һ���ڵ�
		link_type create_node(const T& x) {
			link_type p = get_node();
			construct(&p->data, x);		//�� p �� data ��Ա�ĵ�ַ������һ������
			return p;
		}
		//�������ͷ�һ���ڵ�
		void destroy_node(link_type p) {
			destroy(&p->data);		//���� data ��������
			put_node(p);		//�ͷ��ڴ�
		}
		//����һ��������
		void empty_initialize() {
			node = get_node();		//����һ���ڵ㣬�� node ָ����
			node->next = node;
			node->prev = node;		//node������ָ���ָ���Լ�������Ԫ��ֵ
		}

		/* list �ڲ�������Ϊ splice sort merge �ȵ춨���� */
		//�� [first, last) �ڵ�����Ԫ�ؽӺ��� position ��ָλ��֮ǰ��position ��
		//[first, last)����ָ��ͬһ��list���� position ����λ�� [first, last) ֮��
		void transfer(iterator first, iterator last, iterator position);

		//sort �ڵ���������
		iterator QSort(iterator low, iterator high);
		void QuickSort(iterator low, iterator high);
	public:
		/* ���캯��������һ�������� */
		list() { empty_initialize(); }
		~list() {
			clear();
			destroy_node(node);
		}

		iterator begin() { return (*node).next; }
		iterator end() { return node; }
		bool empty() const { return node->next == node;	}
		size_type size() {
			return distance(begin(), end(), category());		//ȫ�ֺ������� iterator.h
		}
		//ȡͷ�������
		reference front() { return *begin(); }
		//ȡβ�������
		reference back() { return *(--end()); }

		//�ڵ����� position ��ָλ�ò���һ���ڵ�
		iterator insert(iterator position, const T& x) {
			link_type tmp = create_node(x);
			
			//����ָ�룬�� tmp ����
			tmp->next = position.node;
			tmp->prev = position.node->prev;
			(position.node->prev)->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}

		//�Ƴ������� position ��ָ���
		iterator erase(iterator position) {
			link_type next_node = position.node->next;
			link_type prev_node = position.node->prev;
			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(position.node);
			return iterator(next_node);
		}

		void push_front(const T& x) { insert(begin(), x); }
		void push_back(const T& x) { insert(end(), x); }
		void pop_front() { erase(begin()); }
		void pop_back() { erase(--end()); }

		//�����������
		void clear();
		//��ֵΪ value ������Ԫ���Ƴ�
		void remove(const T& value);
		//�Ƴ���ֵ��ͬ������Ԫ��Ϊֻʣһ��
		void unique();
		
		//�� x �Ӻ��� position ��ָλ��֮ǰ��x ���벻ͬ�� *this
		void splice(iterator position, list& x);
		//�� i ��ָ��Ԫ�ؽӺ��� position ��ָλ��֮ǰ��position �� i ����ָ��ͬһ�� list 
		void splice(iterator position, iterator i);
		//�� [first, last) �ڵ�����Ԫ�ؽӺ��� position ��ָλ��֮ǰ��position ��
		//[first, last)����ָ��ͬһ��list���� position ����λ�� [first, last) ֮��
		void splice(iterator position, iterator first, iterator last);

		//merge ��x�ϲ���this���ϣ�ǰ��������list�Ѿ������������
		void merge(list<T, Alloc>& x);
		//reserve ��this��������������
		void reverse();

		//����ʹ�ÿ������򰴷ǵݼ���ʽ����
		//list ����ʹ��STL�������㷨����ΪSTL��sortֻ�������������
		void sort();
	};

	//�����������
	template<class T, class Alloc>
	void list<T, Alloc>::clear() {
		link_type cur = node->next;		//begin()
		while (cur != node) {		//����ÿһ���ڵ�
			link_type tmp = cur;
			cur = cur->next;
			destroy_node(tmp);		//���ٽڵ�
		}
		//������ָ���������״̬
		node->next = node;
		node->prev = node;
	}

	//��ֵΪ value ������Ԫ���Ƴ�
	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator tmp = first++;
			if (*tmp == value) {
				erase(tmp);
			}
		}
	}

	//�Ƴ���ֵ��ͬ��������Ԫ��Ϊֻʣһ��
	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last)		//������
			return;
		iterator next = first;
		while (++next != last) {
			if (*first == *next) {	//����������ͬ��Ԫ��
				erase(next);		//ɾ��һ��
			}
			else {
				first = next;		//����ָ��
			}
			next = first;			//�������η�Χ
		}
	}

	/* list �ڲ�������Ϊ splice��sort��merge �ȵ춨���� */
	//�� [first, last) �ڵ�����Ԫ�ؽӺ��� position ��ָλ��֮ǰ��position ��
	//[first, last)����ָ��ͬһ��list���� position ����λ�� [first, last) ֮��
	template<class T, class Alloc>
	void list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
		if (position != last) {
			(*((*last.node).prev)).next = position.node;
			(*((*first.node).prev)).next = last.node;
			(*((*position.node).prev)).next = first.node;
			link_type tmp = (*position.node).prev;
			(*position.node).prev = (*last.node).prev;
			(*last.node).prev = (*first.node).prev;
			(*first.node).prev = tmp;
		}
	}

	//������ x �Ӻ��� position ��ָλ��֮ǰ��x���벻ͬ�� *this
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& x) {
		if (!x.empty()) {
			transfer(position, x.begin(), x.end());
		}
	}
	
	//�� i ��ָ��Ԫ�ؽӺ��� position ��ָλ��֮ǰ��position �� i ����ָ��ͬһ�� list 
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j)
			return;
		transfer(position, i, j);
	}

	//�� [first, last) �ڵ�����Ԫ�ؽӺ��� position ��ָλ��֮ǰ��position ��
	//[first, last)����ָ��ͬһ��list���� position ����λ�� [first, last) ֮��
	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, iterator first, iterator last) {
		if (first != last) {
			tranfer(position, first, last);
		}
	}

	//merge ��x�ϲ���this���ϣ�ǰ��������list�Ѿ������������
	template<class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		//ע�⣺ǰ�������� list �Ѿ�������������
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else {
				++first1;
			}
		}
		if (first2 != last2) {
			transfer(last1, first2, last2);
		}
	}

	//reserve ��this��������������
	template<class T, class Alloc>
	void list<T, Alloc>::reverse() {
		//�����ж�����Ϊ�ջ�ֻ��һ��Ԫ���򲻽����κβ�����ʹ��size()�ж�Ҳ���ԣ����ٶȽ���
		if (node->next == node || (node->next)->next == node)
			return;

		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	//����ʹ�ÿ�������
	//list ����ʹ��STL�������㷨����ΪSTL��sortֻ�������������
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
	list<T, Alloc>::QSort(iterator low, iterator high) {
		value_type pivotkey = (low.node)->data;
		while (low != high) {
			while (low != high && (high.node)->data >= pivotkey)
				--high;
			(low.node)->data = (high.node)->data;
			while (low != high && (low.node)->data <= pivotkey)
				++low;
			(high.node)->data = (low.node)->data;
		}
		(low.node)->data = pivotkey;
		return low;		//������������λ��
	}
	template<class T, class Alloc>
	void list<T, Alloc>::QuickSort(iterator low, iterator high) {
		if (low != high) {
			iterator pivot = QSort(low, high);
			if (pivot == low) {
				QuickSort(++pivot, high);
			}
			else if (pivot == high) {
				QuickSort(low, --high);
			}
			else {
				iterator pivot2 = pivot;
				QuickSort(low, --pivot2);		//��ǰ�벿�ֽ�������
				QuickSort(++pivot, high);		//�Ժ�벿�ֽ�������
			}
		}
	}
	//����ӿ�
	template<class T, class Alloc>
	void list<T, Alloc>::sort() {
		//�����ж�����Ϊ�ջ�ֻ��һ��Ԫ���򲻽����κβ�����ʹ��size()�ж�Ҳ���ԣ����ٶȽ���
		if (node->next == node || (node->next)->next == node)
			return;
		QuickSort(begin(), --end());
	}


__mySTL_END__


#endif		//end of __LIST_H__
