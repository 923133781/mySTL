#pragma once
#ifndef __SLIST_H__
#define __SLIST_H__

#include "alloctor.h"		//�ռ����������
#include "construct.h"		//�ڴ����Ĺ��졢�������ع���
#include "algobase.h"		//�����㷨���

__mySTL_BEGIN__

	//������Ļ����ڵ�ṹ
	struct __slist_node_base {
		__slist_node_base* next;
	};
	//��������Ľڵ�ṹ
	template<class T>
	struct __slist_node : public __slist_node_base {
		T data;
	};

	//ȫ�ֺ�������֪ĳһ�ڵ㣬����һ�½ڵ������
	inline __slist_node_base* __slist_make_link(
		__slist_node_base* prev_node,
		__slist_node_base* next_node)
	{
		next_node->next = prev_node->next;
		prev_node->next = next_node;
		return next_node;
	}

	//ȫ�ֺ��������㵥�����Ԫ�ظ���
	inline size_t __slist_size(__slist_node_base* node) {
		size_t result = 0;
		for (; node != 0; node = node->next) {
			++result;
		}
		return result;
	}


	/* ��������������Ļ����ṹ */
	struct __slist_iterator_base {
	public:
		typedef size_t size_type;
		typedef ptrdiff_t defference_type;
		typedef forward_iterator_tag iterator_category;

		__slist_node_base* node;		//ָ��ڵ�����ṹ

		__slist_iterator_base(__slist_node_base* x) : node(x) { }

		void incr() { node = node->next; }		//ǰ��һ���ڵ�
		bool operator==(const __slist_iterator_base& x) const {
			return node == x.node;
		}
		bool operator!=(const __slist_iterator_base& x) const {
			return node != x.node;
		}
	};

	//��������ĵ������ṹ
	template<class T, class Ref = T&, class Ptr = T*>
	struct __slist_iterator : public __slist_iterator_base {
		typedef __slist_iterator<T, T&, T*>  iterator;
		typedef __slist_iterator<T, const T&, const T*>  const_iterator;
		typedef __slist_iterator<T, Ref, Ptr>  self;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef __slist_node<T> slist_node;

		__slist_iterator() : __slist_iterator_base(0) { }
		__slist_iterator(slist_node* x) : __slist_iterator_base(x) { }
		__slist_iterator(const iterator& x) : __slist_iterator_base(x.node) { }

		reference operator*() const {
			return ((slist_node*)node)->data;
		}
		pointer operator->() const {
			return &(operator*());
		}
		self& operator++() {
			incr();
			return *this;
		}
		self operator++(int) {
			self tmp = *this;
			incr();
			return tmp;
		}
	};


/* slist�ṹ��ʵ�� */
	template<class T, class Alloc = alloc>
	struct slist {
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			defference_type;

		typedef __slist_iterator<T, T&, T*> iterator;
		typedef __slist_iterator<T, const T&, const T*> const_iterator;

	private:
		typedef __slist_node<T> list_node;
		typedef __slist_node_base list_node_base;
		typedef __slist_iterator_base list_iterator_base;

		//�Խڵ��СΪ��λ��ר���ռ�������
		typedef simple_alloc<list_node, Alloc> list_node_allocater;

		static list_node* create_node(const value_type& x) {
			list_node* node = list_node_allocater::allocate();		//����һ���ڵ�Ŀռ�
			try {
				construct(&node->data, x);
				node->next = 0;
			}
			catch (...) {
				list_node_allocater::deallocate(node);
				throw;
			}
			return node;
		}
		static void destroy_node(list_node* node) {
			destroy(&node->data);		//����Ԫ��
			list_node_allocater::deallocate(node);		//�ͷſռ�
		}
		void clear() {
			list_node* node = (list_node*)head.next;
			while (node != 0) {
				list_node* tmp = node;
				node = (list_node*)node->next;
				destroy_node(tmp);
			}
		}

	private:
		list_node_base head;	//ͷ����ע������ʵ�壬������ָ��
	public:
		slist()  { head.next = 0; }
		~slist() { clear(); }
	public:
		iterator begin() { return iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		size_type size() const { return __slist_size(head.next); }
		reference front() { return ((list_node*)head.next)->data; }
		void push_front(const value_type& x) {
			__slist_make_link(&head, create_node(x));
		}
		void pop_front() {
			list_node* node = (list_node*)head.next;
			head.next = node->next;
			destroy_node(node);
		}
		void insert(iterator ite, const value_type& x) {
			__slist_make_link(ite.node, create_node(x));
		}

		//���� slist ������ֻҪ�� head ������ָ����
		void swap(slist& L) {
			list_node_base* tmp = head.next;
			head.next = L.head.next;
			L.head.next = tmp;
		}
	};


__mySTL_END__


#endif		//end of __SLIST_H__
