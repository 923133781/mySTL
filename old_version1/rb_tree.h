#pragma once
#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include "alloctor.h"		//�ռ����������
#include "construct.h"		//�ڴ����Ĺ��졢�������ع���
#include "algobase.h"		//�����㷨���
#include "queue.h"			//queue ������պ����
#include <utility>			//���� pair


__mySTL_BEGIN__

typedef bool	__rb_tree_color_type;
#define RED		false		//��ɫΪ0
#define BLACK	true		//��ɫΪ1


/* rb_tree �����ڵ�ṹ */
	struct __rb_tree_node_base {
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;		//�ڵ���ɫ
		base_ptr parent;		//ָ�򸸽ڵ�
		base_ptr left;
		base_ptr right;

		static base_ptr get_min_node(base_ptr x) {
			if (!x)
				return nullptr;

			while (x->left != nullptr)
				x = x->left;
			return x;
		}
		static base_ptr get_max_node(base_ptr x) {
			if (!x)
				return nullptr;

			while (x->right != nullptr)
				x = x->right;
			return x;
		}
	};

/* rb_tree �ڵ�ṹ */
	template<class Value>
	struct __rb_tree_node : public __rb_tree_node_base {
		typedef __rb_tree_node<Value>* link_type;
		Value value_field;		//�ڵ�ֵ
	};


/* �������� */
	void __rb_tree_rotate_left(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rotate_right(__rb_tree_node_base*, __rb_tree_node_base*&);
	void __rb_tree_rebalance(__rb_tree_node_base*, __rb_tree_node_base*&);


/* RB_tree ����������ṹ */
/* ע�⣺����rb_tree��begin()ָ�� left_most��end()ָ�� header */
/*		 --begin() �� ++right_most �õ� header��--end() �õ� right_most */
	struct __rb_tree_iterator_base {
		typedef __rb_tree_node_base::base_ptr	base_ptr;
		typedef bidirectional_iterator_tag		iterator_category;		//˫�������
		typedef ptrdiff_t						difference_type;

		base_ptr node;		//���ڲ���������֮��������ϵ

		// operator ++
		void incrment() {
			if (node->right != nullptr) {		//���node�����ӽڵ㣬���ҵ�node������������ڵ㼴Ϊ���
				node = node->right;
				while (node->left != nullptr) {
					node = node->left;
				}
			}
			else {			//node û�����ӽڵ�
				base_ptr _parent = node->parent;	//�ҵ����ڵ�
				while (_parent->right == node) {	//������нڵ㱾�������ӽڵ�
					node = _parent;					//��һֱ���ݣ�ֱ����Ϊ���ӽڵ�Ϊֹ
					_parent = node->parent;
				}
				if (node->right != _parent) {		//����ʱ�����ӽڵ㲻���ڸ��ڵ�
					node = _parent;					//�򸸽ڵ㼴Ϊ��𣬷����ʱ��nodeΪ���
				}
			}
		}

		// operator --
		void decrment() {
			if (node->color == RED &&		//����Ǻ�ڵ㣬�Ҹ��ڵ�ĸ��ڵ�����Լ�
				node->parent->parent == node)	//��ʾ��ʱ node ָ�� header����end()��
			{
				node = node->right;				//�൱��ִ�� --end()��right_most ��Ϊ���
			}
			else if (node->left != nullptr) {		//��������ӽڵ�
				node = node->left;			//�� node ָ�������ӽڵ�

				while (node->right != nullptr)		//�ҵ����ӽڵ�����ҽڵ㼴Ϊ���
					node = node->right;					
			}
			else {			//�ȷǸ��ڵ�Ҳ�����ӽڵ�
				base_ptr _parent = node->parent;		//�ҵ� node �ĸ��ڵ�

				while (node == _parent->left) {			//�����нڵ���Ϊ���ӽڵ�
					node = _parent;						//һֱ�����ߣ�ֱ����Ϊ���ӽڵ�
					_parent = node->parent;
				}
				node = _parent;			//��ʱ�ĸ��ڵ㼴Ϊ���
			}
		}
	};

/* RB_tree ����������ṹ */
	template<class Value, class Ref, class Ptr>
	struct __rb_tree_iterator : public __rb_tree_iterator_base {
		typedef Value	value_type;
		typedef Ref		reference;
		typedef Ptr		pointer;	
		typedef __rb_tree_node<Value>*	link_type;
		typedef __rb_tree_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;		//�����Ƕ����const����ʾ�����޸�Ԫ�ص�ֵ

		__rb_tree_iterator() : node(nullptr) { }
		__rb_tree_iterator(link_type x) { node = x; }
		__rb_tree_iterator(const iterator& x) { node = x.node; }

		reference operator*() const { return link_type(node)->value_field; }
#ifndef __SGI_STL_NO_ARROW_OPERATOR
		pointer operator->() const { return &(operator*()); }
#endif
		/* ǰ��++�������ԼӺ��this */
		self& operator++() { incrment(); return *this; }
		/* ����++�������Լ�ǰ��this */
		self operator++(int) {
			self tmp = *this;
			incrment();
			return tmp;
		}
		/* ǰ��--�������Լ����this */
		self& operator--() { decrment(); return *this; }
		/* ����--�������Լ�ǰ��this */
		self operator--(int) {
			self tmp = *this;
			decrment();
			return tmp;
		}

		bool operator==(const iterator& x) const { return (node == x.node); }
		bool operator!=(const iterator& x) const { return (node != x.node); }
		bool operator==(const const_iterator& x) const { return (node == x.node); }
		bool operator!=(const const_iterator& x) const { return (node != x.node); }
	};




/* RB_tree �ṹ�Ķ��� */
	/* Key:			rb_tree��ֵ�ͱ� */
	/* Value:		rb_treeʵ�ʴ洢��Ԫ���ͱ�ͨ����Ӧ��ֵ�ĳ�����˳�� */
	/* KeyOfValue:	��ֵ��ȡ�������ó�ĳ��Ԫ��ʵֵ����Ӧ�ļ�ֵ */
	/* Compare:		��ֵ�ȽϷ��� */
	template<class Key, class Value, class KeyOfValue, class Compare = std::less<Key>, class Alloc = alloc>
	struct rb_tree {
	protected:
		typedef void* void_pointer;
		typedef __rb_tree_node_base*	base_ptr;
		typedef __rb_tree_color_type	color_type;
		typedef __rb_tree_node<Value>	rb_tree_node;

		/* ����ר���ռ���������ÿ������һ���ڵ�Ŀռ� */
		typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	public:
		typedef Key					key_type;
		typedef Value				value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef rb_tree_node*		link_type;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef typename __rb_tree_iterator<value_type, reference, pointer>::iterator iterator;
		typedef typename __rb_tree_iterator<value_type, reference, pointer>::const_iterator const_iterator;
	protected:
		link_type get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

		/* ����һ���ڵ� */
		link_type create_node(const value_type& x) {
			link_type tmp = get_node();
			try {
				construct(&(tmp->value_field), x);
			}
			catch (...) {
				put_node(tmp);
			}
			return tmp;
		}
		/* ����һ���ڵ㣨��ɫ��ֵ�� */
		link_type clone_node(const link_type& x) {
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = nullptr;
			tmp->right = nullptr;
			return tmp;
		}
		/* ����һ���ڵ� */
		void destroy_node(link_type x) {
			destroy(&x->value_field);
			put_node(x);
		}

	protected:
		/* RB_tree ֻ���������ݱ��� */
		link_type header;			//ͷָ�룬��parent���root����ָ��Է�����left��right�ֱ�ָ����С�����ڵ�
		size_type node_count;		//�ڵ�����
		Compare key_compare;		//�ڵ��ֵ��С�Ƚ�׼��

		/* ��������������������ȡ�� header �ĳ�Ա */
		link_type& root() const { return (link_type&) header->parent; }
		link_type& leftmost() const { return (link_type&) header->left; }
		link_type& rightmost() const { return (link_type&) header->right; }

		/* ��������������������ȡ�ýڵ� x �ĳ�Ա */
		static link_type& left(link_type x) { return (link_type&) (x->left); }
		static link_type& right(link_type x) { return (link_type&) (x->right); }
		static link_type& parent(link_type x) { return (link_type&) (x->parent); }
		static reference value(link_type x) { return x->value_field; }
		static const Key& key(link_type x) { return KeyOfValue() (value(x)); }
		static color_type& color(link_type x) { return (color_type&) (x->color); }

		/* ��������������������ȡ�ýڵ� x �ĳ�Ա */
		static link_type& left(base_ptr x) { return (link_type&) (x->left); }
		static link_type& right(base_ptr x) { return (link_type&) (x->right); }
		static link_type& parent(base_ptr x) { return (link_type&) (x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value_field; }
		static const Key& key(base_ptr x) { return KeyOfValue() (value(link_type(x))); }
		static color_type& color(base_ptr x) { return (color_type&) (link_type(x)->color); }

		//��ȡ�� x �����ֵ����Сֵ
		static link_type get_min_node(link_type x) {
			return (link_type)__rb_tree_node_base::get_min_node(x);
		}
		static link_type get_max_node(link_type x) {
			return (link_type)__rb_tree_node_base::get_max_node(x);
		}
	private:
		/* �����Ĳ������: xΪ��ֵ����㣬yΪ�����ĸ��ڵ㣬vΪ������ֵ������ָ���²���ڵ�ĵ����� */
		iterator __insert(base_ptr x, base_ptr y, const value_type& v);
		/* ���������� x */
		link_type __copy(link_type x);
		/* ɾ���ڵ� x����������Ӧ�ĵ��� */
		void __erase(link_type x);

		void init() {
			header = get_node();	//����һ���ڵ㣬�� header ָ����
			color(header) = RED;	//��headerΪ��ɫ����������header��root����iterator.operator--��ʹ��
			root() = nullptr;		//root Ϊ�գ��� header->parent
			leftmost() = header;	//�� header �������ӽڵ�Ϊ�Լ�
			rightmost() = header;		
		}
	public:
		rb_tree(const Compare& comp = Compare()) : node_count(0), key_compare(comp) {
			init();
		}
		rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {		//�������캯��
			init();
			*this = x;		//���� operator=
		}
		~rb_tree() {
			clear();
			put_node(header);
		}

		/* ���������� x */
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
		operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

		/* ��� rb_tree(��header) */
		void clear();

		/* ���� *this �� x ���� */
		void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
			link_type tmp = header;
			*(&header) = x.header;
			*(&(x.header)) = tmp;
		}

		/* Ѱ��RB_tree���Ƿ���ڼ�ֵΪ k �Ľڵ� */
		iterator find(const Key& k);

		/* ͳ��Ԫ�� x �ĸ��� */
		size_type count(const key_type& x) {
			size_type x_count = 0;
			iterator it = find(x);		//���ж����ͬԪ��ʱ��find ���ص�һ��Ԫ�صĵ�����
			if (it != end()) {
				++x_count;
										//�������鿴�Ƿ񻹴���Ԫ�� x
				for (++it; !key_compare(key(it.node), x) && !key_compare(x, key(it.node)); ++it)
					++x_count;
			}
			return x_count;
		}

		iterator begin() { return leftmost(); }		//����RB_tree��С�ڵ�
		iterator end() { return header; }			//RB_tree���յ�Ϊ header
		bool empty() const { return (node_count == 0); }
		size_type size() const { return node_count; }
		Compare key_comp() const { return key_compare; }
		size_type max_size() const { return size_type(-1); }

		/* ������ֵ���ڵ�ֵ�������ظ����ظ������ʧ�� */
		std::pair<iterator, bool> insert_unique(const value_type& v);
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				insert_unique(*first);
		}

		/* ������ֵ���ڵ�ֵ�����ظ� */
		iterator insert_equal(const value_type& v);

		/* ɾ���ڵ� x */
		void erase(link_type x) { __erase(x); }
		void erase(iterator x) { __erase(link_type(x.node)); }
		void erase(const value_type& x) {
			iterator it = find(x);
			if (it != end())
				__erase(link_type(it.node));
		}
		template<class InputIterator>
		void erase(InputIterator first, InputIterator last) {
			for (; first != last; ++first)
				erase(first);
		}
	};



	/* ���������� x */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) {
		clear();		//�Ƚ�ԭʼ�������

		key_compare = x.key_compare;
		header->parent = __copy(x.root());
		if (header->parent != nullptr) {
			root()->parent = header;
			node_count = x.node_count;

			leftmost() = get_min_node(root());
			rightmost() = get_max_node(root());
		}

		return *this;
	}


	/* ��� rb_tree(��header) */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
	{
		if (header->parent != nullptr) {
			queue<base_ptr> node_queue;

			node_queue.push(header->parent);
			while (!node_queue.empty()) {
				base_ptr cur = node_queue.front(); node_queue.pop();
				
				if (cur->left != nullptr)
					node_queue.push(cur->left);
				if (cur->right != nullptr)
					node_queue.push(cur->right);
				destroy_node(link_type(cur));
			}

			/* �� rb_tree �ָ�����״̬ */
			node_count = 0;
			root() = nullptr;		//root Ϊ�գ��� header->parent
			leftmost() = header;	//�� header �������ӽڵ�Ϊ�Լ�
			rightmost() = header;
		}
	}


	/* Ѱ��RB_tree���Ƿ���ڼ�ֵΪ k �Ľڵ� */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
		link_type y = header;
		link_type x = root();

	//ע�⣺��������ͬԪ��ʱ��find ���ص�һ��Ԫ�صĵ������������������ɷ���������ͬԪ�أ�

		/* ����д���ȽϷ��£����� k �����ĸ�λ�ö������ߵ�Ҷ�ڵ��ֹͣ����ΪC++��׼�� */
		/* map ֻҪ�� key ���붨��С�ڹ�ϵ��û��Ҫ������ڹ�ϵ������ֻ������д */
		while (x != nullptr) {
			if (key_compare(key(x), k))
				x = right(x);			//���е������ʾ x ��ֵС�� k��x ������
			else
				y = x, x = left(y);		//���е������ʾ x ��ֵ���ڻ���� k��x ������
		}
		iterator j = iterator(y);
		return ( j == end() || key_compare(k, key(j.node)) ) ? end() : j;
	}


	/* ������ֵ���ڵ�ֵ�������ظ����ظ��������Ч */
	/* ����һ��pair����һ��Ԫ��ָ��RB_tree�������ڵ㣬�ڶ���Ԫ�ر�ʾ�Ƿ����ɹ� */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v) {
		link_type y = header;
		link_type x = root();
		bool comp = true;

		while (x != nullptr) {		//�Ӹ��ڵ㿪ʼ������Ѱ���ʵ��Ĳ����
			y = x;

			comp = key_compare(KeyOfValue()(v), key(x));	//less
			x = comp ? left(x) : right(x);
		}//������xָ�����㣬yָ��x�ĸ��ڵ�

		iterator j = iterator(y);
		if (comp) {		//�������ʱ comp Ϊ�棬��ʾ���������
			if (j == begin())	//�������ڵ�ĸ��ڵ�Ϊ left_most����ֱ�Ӳ���
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			else		//���򽫵��� j ���ں������
				--j;
		}
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			//�¼�ֵ������м�ֵ�ظ������в������
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		}

		/* ���е������ֵһ���ظ�������ʧ�� */
		/* ע�⣺�����и������Ƿ���һ����һԪ��ָ������ͻ */
		/* �Ľڵ�ĵ���������map���±������������Ҫ������ */
		return std::pair<iterator, bool>(j, false);
	}

	/* ������ֵ���ڵ�ֵ�����ظ� */
	/* ����ָ���²���ڵ�ĵ����� */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v) {

		/* ע�⣺��������ͬԪ��ʱ����Ԫ�س�Ϊ����Ԫ�صĺ�̣�������Ԫ���Һ��ӵ�����ڵ㣩 */

		link_type y = header;
		link_type x = root();		//�Ӹ��ڵ㿪ʼ����Ѱ���ʵ��Ĳ����
		while (x != nullptr) {
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);		//less
		}
		return __insert(x, y, v);
		// xΪ��ֵ����㣬yΪ�����ĸ��ڵ㣬vΪ������ֵ
	}

	/* �����Ĳ������: xΪ��ֵ����㣬yΪ�����ĸ��ڵ㣬vΪ������ֵ������ָ���²���ڵ�ĵ����� */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr _x, base_ptr _y, const value_type& v) {
		link_type x = (link_type)_x;
		link_type y = (link_type)_y;

		link_type new_node = create_node(v);			//����һ���½ڵ�
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			left(y) = new_node;		//��ʹ�õ�yΪheaderʱ��leftmost() = new_node
			if (y == header) {
				root() = new_node;
				rightmost() = new_node;
			}
			else if (y == leftmost()) {
				leftmost() = new_node;
			}
		}
		else {
			right(y) = new_node;		//���½ڵ�Ϊ����㸸�ڵ�����ӽڵ�
			if (y == rightmost()) {
				rightmost() = new_node;		//ά�� rightmost ��Զָ�����ҽڵ�
			}
		}
		parent(new_node) = y;		//�趨�½ڵ�ĸ��ڵ�
		left(new_node) = 0;
		right(new_node) = 0;		//�趨�½ڵ�������ӽڵ�

		//�ڵ���ɫһ��Ϊ�죬����ȫ�ֺ���__rb_tree_rebalance() ���趨������
		__rb_tree_rebalance(new_node, header->parent);		//����һΪ�����ڵ㣬������Ϊ root
		++node_count;
		return iterator(new_node);		//����ָ�������ڵ�ĵ�����
	}

	/* ���������� x */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__copy(link_type x) {
		if (x == nullptr)
			return nullptr;

		link_type x_copy = clone_node(x);
		x_copy->left = __copy(left(x));
		x_copy->right = __copy(right(x));

		if (x_copy->left != nullptr)
			x_copy->left->parent = x_copy;
		if (x_copy->right != nullptr)
			x_copy->right->parent = x_copy;

		return x_copy;
	}

	/* ����д�����Լ�д�ļ򵥵ķ�������Ч�ʺܵͣ��д��Ľ� */

	/* ɾ���ڵ� x����������Ӧ�ĵ��� */
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type x)
	{
		if (x->left == nullptr && x->right == nullptr)			//��� x �Ѿ���Ҷ�ڵ㣬�ͽ� x ɾ���������е���
		{
			if (x == x->parent->left)
				x->parent->left = nullptr;
			else
				x->parent->right = nullptr;

			if (x->color == RED) {		//��� x Ϊ��ɫ��Ҷ�ڵ㣬��ֱ�ӽ���ɾ������
				destroy_node(x);
			}
			else {			//������Ҫ������Ӧ����
				
				//�����������������е���

			}
			--node_count;
		}
		else if (x->left != nullptr && x->right == nullptr) {		//x ֻ�����������ҵ� x ��ǰ���滻 x��ת��Ϊɾ��x��ǰ��
			link_type prev = x->left;
			x->value_field = prev->value_field;
			__erase(prev);
		}
		else if (x->left == nullptr && x->right != nullptr) {		//x ֻ�����������ҵ� x �ĺ���滻 x��ת��Ϊɾ��x�ĺ��
			link_type next = x->right;
			x->value_field = next->value_field;
			__erase(next);
		}
		else {			//x ӵ�������������ҵ� x �ĺ���滻 x��ת��Ϊɾ��x�ĺ��
			link_type next = x->right;
			x->value_field = next->value_field;
			__erase(next);
		}
	}




/* ȫ�ֺ�������������ʹ��ƽ�⣨�ı���ɫ����ת���Σ� */
/* ����һΪ�����ڵ㣬������Ϊ root */
	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x->color = RED;		//�½ڵ��Ϊ�죨���ݹ���4��

		while (x != root && x->parent->color == RED)	//���ڵ�ҲΪ��ɫ
		{
			if (x->parent == x->parent->parent->left) {			//���ڵ�Ϊ�游�ڵ�����ӽڵ�
				__rb_tree_node_base* y = x->parent->parent->right;	//�� y Ϊ�����ڵ�
				if (y && y->color == RED) {			//�����ڵ������Ϊ��ɫ
					y->color = BLACK;				//���Ĳ����ڵ�Ϊ��
					x->parent->color = BLACK;		//���ĸ��ڵ�Ϊ��
					x->parent->parent->color = RED;	//�����游�ڵ�Ϊ��

					/* x �������游�ڵ㣬�´μ����ж��游�ڵ��̫�游�ڵ��Ƿ�ͬʱΪ�� */
					x = x->parent->parent;
				}
				else {			//�޲����ڵ㣬�򲮸��ڵ�Ϊ��
					if (x == x->parent->right) {		//����½ڵ�Ϊ���ڵ�����ӽڵ�
						x = x->parent;					//�޸� x Ϊ�丸�ڵ㣬�ȶ��丸�ڵ����һ����������
						__rb_tree_rotate_left(x, root);		//��һ����Ϊ������
					}
					x->parent->color = BLACK;			//�����ڵ��Ϊ��
					x->parent->parent->color = RED;		//���游�ڵ��Ϊ��
					__rb_tree_rotate_right(x->parent->parent, root);	//���游�ڵ�ִ������

					/* ��ʱx���ֵܽڵ�(ԭ�游�ڵ�)Ϊ�죬x���ڵ�Ϊ�ڣ�������rb_treeҪ���´�ѭ�����˳�while */
				}
			}
			else {			//���ڵ�Ϊ�游�ڵ�����ӽڵ�
				__rb_tree_node_base* y = x->parent->parent->left;		//�� y Ϊ�����ڵ�
				if (y && y->color == RED) {			//�����ڵ������Ϊ��
					y->color = BLACK;				//���Ĳ����ڵ�Ϊ��
					x->parent->color = BLACK;		//���ĸ��ڵ�Ϊ��
					x->parent->parent->color = RED;	//�����游�ڵ�Ϊ��

					/* x �������游�ڵ㣬�´μ����ж��游�ڵ��̫�游�ڵ��Ƿ�ͬʱΪ�� */
					x = x->parent->parent;
				}
				else {		//�����ڵ㲻���ڻ򲮸��ڵ�Ϊ��
					if (x == x->parent->left) {			//����½ڵ�Ϊ���ڵ�����ӽڵ�
						x = x->parent;					//�޸� x Ϊ�丸�ڵ㣬�ȶ��丸�ڵ����һ����������
						__rb_tree_rotate_right(x, root);	//��һ����Ϊ������
					}
					x->parent->color = BLACK;			//�����ڵ��Ϊ��
					x->parent->parent->color = RED;		//���游�ڵ��Ϊ��
					__rb_tree_rotate_left(x->parent->parent, root);		//���游�ڵ�ִ������

					/* ��ʱx���ֵܽڵ�(ԭ�游�ڵ�)Ϊ�죬x���ڵ�Ϊ�ڣ�������rb_treeҪ���´�ѭ�����˳�while */
				}
			}
		}		//while ����
		root->color = BLACK;		//���ڵ���ԶΪ��
	}

/* ȫ�ֺ�����xΪ�����㣬rootΪ���ڵ� */
/* �½ڵ��Ϊ��ڵ㣬��������ĸ��ڵ�ҲΪ��ڵ㣬�������Ҫ����������ת����ɫ�ı� */
	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->right;		//�� y Ϊ��ת������ӽڵ�
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;		//�������趨���ڵ�
		}
		y->parent = x->parent;

		//�� y ��ȫ���� x �ĵ�λ�����뽫 x ���丸�ڵ�Ĺ�ϵ��ȫ���չ�����
		if (x == root)						//��� x Ϊ���ڵ㣬�� y ��Ϊ�µĸ��ڵ�
			root = y;
		else if (x == x->parent->left)		//x Ϊ�丸�ڵ�����ӽڵ�
			x->parent->left = y;
		else								//x Ϊ�丸�ڵ�����ӽڵ�
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

/* ȫ�ֺ�����xΪ�����㣬rootΪ���ڵ� */
/* �½ڵ��Ϊ��ڵ㣬��������ĸ��ڵ�ҲΪ��ڵ㣬�������Ҫ����������ת����ɫ�ı� */
	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x->left;		//�� y Ϊ��ת������ӽڵ�
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;		//�������趨���ڵ�
		}
		y->parent = x->parent;

		//�� y ��ȫ���� x �ĵ�λ�����뽫 x ���丸�ڵ�Ĺ�ϵ��ȫ���չ�����
		if (x == root)						//��� x Ϊ���ڵ㣬�� y ��Ϊ�µĸ��ڵ�
			root = y;
		else if (x == x->parent->right)		//x Ϊ�丸�ڵ�����ӽڵ�
			x->parent->right = y;
		else								//x Ϊ�丸�ڵ�����ӽڵ�
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}


__mySTL_END__

#endif	//end of __RB_TREE_H__