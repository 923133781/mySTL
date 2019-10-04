#pragma once
#ifndef __HASH_TABLE__
#define __HASH_TABLE__


#include "alloctor.h"		//�ռ����������
#include "construct.h"		//�ڴ����Ĺ��졢�������ع���
#include "algobase.h"		//�����㷨���
#include "iterator.h"		//���������
#include <utility>			//���� pair

#include "vector.h"			//����ʵ�־ۺ���

__mySTL_BEGIN__

/* hash table �ڵ�ṹ */
	template<class Value>
	struct __hashtable_node {
		Value val;
		__hashtable_node* next;
	};


/* hash table �ṹ���� */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
	struct hashtable;


/* hash table ���������� */
	//ע�⣺hashtable �ĵ�����û�к��˲�����Ҳû��reverse iterator
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = alloc>
	struct __hashtable_iterator {
	public:
		typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>	hashtable;
		typedef __hashtable_iterator<Value, Key,
									 HashFunc, ExtractKey, EqualKey, Alloc>		iterator;
		typedef __hashtable_iterator<const Value, const Key,
									 HashFunc, ExtractKey, EqualKey, Alloc>		const_iterator;
		typedef Value					value_type;
		typedef size_t					size_type;
		typedef Value&					reference;
		typedef Value*					pointer;
		typedef ptrdiff_t				difference_type;
		typedef forward_iterator_tag	iterator_category;
		typedef __hashtable_node<Value> node_type;

		node_type* cur;		//������Ŀǰ��ָ���
		hashtable* htb;		//ָ��hash table ��bucket������������֮��������ϵ

	public:
		__hashtable_iterator() { }
		__hashtable_iterator(node_type* _cur, hashtable* _htb) : cur(_cur), htb(_htb) { }

		reference operator*() const { return cur->val; }
		pointer operator->() const { return &(operator*()); }

		iterator& operator++() {
			const node_type* old = cur;
			cur = cur->next;		//������ھ������������������ if ����
			if (cur == nullptr) {
				size_type bucket = htb->bkt_num(old->val);		//�ҳ���ǰԪ�����ڵ�Ͱ

				/* Ѱ�ҵ�һ������Ԫ�ص�Ͱ�����һ��Ԫ�ؼ�Ϊ���� */
				while (!cur && ++bucket < htb->buckets.size()) {
					cur = htb->buckets[bucket];
				}
			}
			return *this;
		}
		//����++�������Լ�ǰ�Ķ���
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;		//����operator++
			return tmp;
		}

		bool operator==(const iterator& it) { return (cur == it.cur); }
		bool operator!=(const iterator& it) { return (cur != it.cur); }
	};



/* hash table �ṹ���� */
	/*
	 *	Value:		����ֵ�ĺ������Value
	 *	Key:		key�ڵ�������
	 *	HashFcn:	hash����
	 *	ExtractKey:	��δ�Value������ȡ��Key(�������߷º���)
	 *	EqualKey:	��αȽ�key(�������߷º���)
	 *	Alloc:		�ڴ����
	 */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	struct hashtable {
	public:
		typedef typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey,
											  EqualKey, Alloc>::iterator			iterator;
		typedef typename __hashtable_iterator<Value, Key, HashFunc, ExtractKey,
											  EqualKey, Alloc>::const_iterator		const_iterator;
		typedef Value			value_type;
		typedef Key				key_type;
		typedef size_t			size_type;
		typedef HashFunc		hasher;
		typedef EqualKey		key_equal;
		typedef ptrdiff_t		difference_type;
		typedef Value*			pointer;
		typedef const Value*	const_pointer;
		typedef Value&			reference;
		typedef const Value&			const_reference;
		typedef __hashtable_node<Value> node_type;
		typedef simple_alloc<node_type, Alloc> node_allocater;

		/* bucket �ۺ��壬��vectorʵ�ֱ�����չ */
		vector<node_type*, Alloc> buckets;

	private:
		hasher		hash;
		key_equal	equals;
		ExtractKey	get_key;
		size_type	num_elements;	//Ԫ�ظ���
	protected:
		/* ����һ���½ڵ� */
		node_type* create_node(const value_type& x) {
			node_type* node = node_allocater::allocate();
			node->next = nullptr;
			try {
				construct(&node->val, x);
				return node;
			}
			catch (...) {
				node_allocater::deallocate(node);
				throw;
			}
		}
		/* ����һ���ڵ� */
		void delete_node(node_type* node) {
			destroy(&node->val);
			node_allocater::deallocate(node);
		}

		/* ��ʼ������ buckets */
		void initialize_buckets(size_type n) {
			const size_type bucket_size = next_size(n);
			buckets.resize(bucket_size, nullptr);		//�޸� buckets ��С������ʼ���¿ռ�Ϊ��
			num_elements = 0;
		}

		/* �ж��Ƿ���Ҫ�ؽ�����������Ҫ�����̷��أ������ؽ�һ������ı�� */
		void resize(size_type num_elements_hint);
		/* �ڲ��ؽ���������²���ڵ㡣��ֵ�������ظ� */
		std::pair<iterator, bool> insert_unique_noresize(const value_type& x);
		/* �ڲ��ؽ���������²���ڵ㡣��ֵ�������ظ� */
		iterator insert_equal_noresize(const value_type& x);
		/* ʵ��ִ��ɾ��Ԫ�صĲ��� */
		void __erase(const value_type& x);
		/* nest_size ������ӽ� n �Ҳ�С�� n ������ */
		size_type next_size(size_type n) const { return __stl_next_prime(n); }

	public:
		/* ����Ԫ�� x ��ŵ�ĺ��� */

		//�汾1������ʵֵ(value)�� buckets ����
		size_type bkt_num(const value_type& obj, size_t n) const {
			return bkt_num_key(get_key(obj), n);		//���ð汾4
		}
		//�汾2��ֻ����ʵֵ
		size_type bkt_num(const value_type& obj) const {
			return bkt_num_key(get_key(obj));			//���ð汾3
		}
		//�汾3��ֻ���ܼ�ֵ
		size_type bkt_num_key(const key_type& key) const {
			return bkt_num_key(key, buckets.size());		//���ð汾4
		}
		//�汾4�����ܼ�ֵ�� buckets ����
		size_type bkt_num_key(const key_type& key, size_t n) const {
			//return (key % n);
			return hash(key) % n;
		}

	public:
		hashtable(size_type n, const HashFunc& hf, const EqualKey& eql)
		: hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0){
			initialize_buckets(n);
		}

		iterator begin() {
			for (size_type i = 0; i < buckets.size(); ++i) {
				if (buckets[i])
					return iterator(buckets[i], this);
			}
			return end();
		}
		iterator end() { return iterator(nullptr, this); }
		size_type size() const { return num_elements; }					//����Ԫ�ظ���
		bool empty() { return num_elements == 0; }
		size_type bucket_count() const { return buckets.size(); }	//���ؾۺ���vector�Ĵ�С
		size_type max_bucket_count() const {
			return __stl_prime_list[__stl_nun_primes - 1];
			//��ֵ������ 4294967291ul
		}
		/* ���� n ��Ͱ��Ԫ�ظ��� */
		int elemts_in_buckets(size_type n) {
			int count = 0;
			for (node_type* first = buckets[n]; first; first = first->next)
				++count;
			return count;
		}

		/* ���Ҽ�ֵΪ key ��Ԫ�� */
		iterator find(const key_type& key) {
			size_type n = bkt_num_key(key);		//�ҵ� key �����ĸ�Ͱ��

			node_type* cur = buckets[n];
			while (cur) {
				if (equals(get_key(cur->val), key))
					return iterator(cur, this);
				cur = cur->next;
			}
			return end();
		}
		const_iterator find(const key_type& key) const {
			size_type n = bkt_num_key(key);		//�ҵ� key �����ĸ�Ͱ��

			node_type* cur = buckets[n];
			while (cur) {
				if (equals(get_key(cur->val), key))
					return iterator(cur, this);
				cur = cur->next;
			}
			return end();
		}
		size_type count(const key_type& key) {
			int result = 0;
			iterator it = find(key);
			if (it != end()) {
				node_type* cur = it.cur;
				while (cur) {
					if (equals(get_key(cur->val), key))
						++result;
					cur = cur->next;
				}
			}
			return result;
		}

		/* ����Ԫ�أ��������ظ� */
		std::pair<iterator, bool> insert_unique(const value_type& x) {
			resize(num_elements + 1);		//�ж��Ƿ���Ҫ�ؽ����
			return insert_unique_noresize(x);
		}
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_unique(*first);
		}
		/* ����Ԫ�أ������ظ� */
		iterator insert_equal(const value_type& x) {
			resize(num_elements + 1);		//�ж��Ƿ���Ҫ�ؽ����
			return insert_equal_noresize(x);
		}
		template<class InputIterator>
		void insert_equal(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_equal(*first);
		}

		/* ɾ��Ԫ�� */
		void erase(iterator it) { __erase(*it); }
		void erase(node_type* node) { __erase(node->val); }
		void erase(const value_type& x) { __erase(x); }
		template<class InputIterator>
		void erase(InputIterator first, InputIterator last) {
			while (first != last) {
				InputIterator tmp = first++;
				__erase(*tmp);		//ע�ⲻ��ֱ��ʹ�� first ����ɾ�������ʹ first ʧЧ
			}
		}

		/* ��� hash table */
		void clear();

		/* ���� ht ����hashtable */
		void copy_from(const hashtable& ht);

		void swap(hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& ht) {
			size_type size1 = buckets.size();
			size_type size2 = ht.buckets.size();
			size_type min_size = size1 < size2 ? size1 : size2;

			int i = 0;
			for (; i < min_size; ++i) {
				std::swap(buckets[i], ht.buckets[i]);
			}

			if (size1 < size2) {
				while (i < size2) 
					buckets.push_back(ht.buckets[i++]);
				ht.buckets.erase(ht.buckets.begin() + size1, ht.buckets.end());
			}
			else {
				while (i < size1)
					ht.buckets.push_back(buckets[i++]);
				buckets.erase(buckets.begin() + size2, buckets.end());
			}
			std::swap(num_elements, ht.num_elements);
		}
	};


	/* �ڲ��ؽ���������²���ڵ㡣��ֵ�������ظ� */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool>
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);		//�ҵ� obj Ӧ�ò���Ĵ���
		node_type* first = buckets[n];			//�� first ָ���Ӧ���е�ͷ��

		/* �鿴��ǰ�������Ƿ���ں� obj ��ͬ��Ԫ�� */
		for (node_type* cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj)))
				return std::pair<iterator, bool>(iterator(cur, this), false);
			//�������ͬ��Ԫ������������
		}

		/* ���е���˵����ǰ����Ϊ�ջ����ظ�Ԫ�أ��ɽ��в��� */
		node_type* tmp = create_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return std::pair<iterator, bool>(iterator(tmp, this), true);
	}

	/* �ڲ��ؽ���������²���ڵ㡣��ֵ�����ظ� */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);		//�ҵ� obj Ӧ�ò���Ĵ���
		node_type* first = buckets[n];			//�� first ָ���Ӧ���е�ͷ��

		/* ���Ҵ������Ƿ������ obj ��ͬ��Ԫ�أ���������������� */
		for (node_type* cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				node_type* tmp = create_node(obj);
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		}

		/* ���е���˵����ǰ����Ϊ�ջ����ظ�Ԫ�� */
		node_type* tmp = create_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return iterator(tmp, this);
	}


	/* ���º����ж��Ƿ���Ҫ�ؽ�����������Ҫ�����̷��أ������ؽ�һ������ı�� */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
	{	
		const size_type bucket_size = buckets.size();
		
		/* ע����ͨ���ȽϾۺ����С��Ԫ�ظ����ж��Ƿ���Ҫ�ؽ� */
		if (num_elements_hint > bucket_size) {
			const size_type new_size = next_size(bucket_size);		//�ҳ���һ������
			vector<node_type*, Alloc> tmp_buckets(buckets);			//����һ����ʱ�� buckets ���ڱ���ԭʼ buckets ������

			//��ԭ buckets ��գ�ע��ֻ�������buckets��ָ�룬ԭ���������ڣ�
			for (int i = 0; i < bucket_size; ++i)
				buckets[i] = nullptr;

			buckets.resize(new_size);		//��ԭ buckets ����Ϊ�� buckets

			//���´���ÿ���� buckets ��ÿ������
			for (size_type i = 0; i < bucket_size; ++i) {
				node_type* first = tmp_buckets[i];			//ָ��ڵ������Ӧ�ĵ�һ���ڵ�

				/* ���´��� first ��Ӧ�����ÿһ���ڵ� */
				while (first != nullptr) {
					/* �����ҳ��ڵ������� buckets ���ĸ�λ�� */
					size_type pos = bkt_num(first->val, new_size);

					/* ����4��ʵ�ֽڵ㽻�� */
					tmp_buckets[i] = first->next;	//���ڵ� first �Ӿ� buckets ��������ɾ��
					first->next = buckets[pos];
					buckets[pos] = first;			//��first������buckets�Ĵ���
					first = tmp_buckets[i];			//�� first ����ָ���buckets�ĵ�һ���ڵ㣬�Ա㴦�����ڵ�
				}
			}
		}
	}

	/* ��� hash table */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
		for (size_type i = 0; i < buckets.size(); ++i) {		//�� buckets ��ÿһ��Ԫ�ؽ��д���
			node_type* cur = buckets[i];
			while (cur) {
				node_type* next = cur->next;
				delete_node(cur);
				cur = next;
			}
			buckets[i] = nullptr;
		}
		num_elements = 0;
		//ע�⣺clear������δ�ͷ�buckets vector �Ŀռ�
	}

	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht) {
		//�Ƚ��Լ��� buckets ���
		clear();
		buckets.size(ht.buckets.size());

		try {
			for (size_type i = 0; i < ht.buckets.size(); ++i) {
				//����ÿһ�����ڵĴ���
				node_type* cur = ht.buckets[i];
				if (cur != nullptr) {
					node_type* copy = create_node(cur->val);
					buckets[i] = copy;

					/* ���Ƶ�ǰ���е�ÿһ���ڵ� */
					for (cur = cur->next; cur; cur = cur->next) {
						copy->next = create_node(cur->val);
						copy = copy->next;
					}
				}
			}
			num_elements = ht.num_elements;
		}
		catch (...) {
			clear();
		}
	}

	/* ʵ��ִ��ɾ��Ԫ�صĲ��� */
	template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
	void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::__erase(const value_type& x) {
		size_type n = bkt_num(x);
		node_type* cur = buckets[n];
		if (cur) {
			if (equals(get_key(cur->val), get_key(x))) {
				buckets[n] = cur->next;
				delete_node(cur);
				--num_elements;
				return;
			}

			while (cur->next) {
				if (equals(get_key(cur->next->val), get_key(x))) {
					node_type* tmp = cur->next;
					cur->next = tmp->next;
					delete_node(tmp);
					--num_elements;
					return;
				}
				cur = cur->next;
			}
		}
	}



__mySTL_END__

#endif //end of __HASH_TABLE__