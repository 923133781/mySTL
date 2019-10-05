#pragma once
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "config.h"		//�������ͷ�ļ�
#include <cstddef>		//ʹ�ú� ptrdiff_t


__mySTL_BEGIN__

/*	���ֵ���������	*/
	/* ֻд������ */
	struct input_iterator_tag { };
	/* ֻ�������� */
	struct output_iterator_tag { };
	/* ��д������ */
	struct forward_iterator_tag : public input_iterator_tag { };
	/* ˫������� */
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	/* ��������� */
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };


/*	Ϊ�˱���д����ʱ��һ©�����п����Ĵ�����ü̳��������������	*/
	template<class Category, class T, class Distance = ptrdiff_t,
			 class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category		iterator_category;
		typedef T				value_type;
		typedef Distance		difference_type;
		typedef Pointer			pointer;
		typedef Reference		reference;
	};


/*	ե֭�� traits	*/
/*	iterator_traits ������ȡ������������	*/
	template<class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};


/*	���ԭ��ָ�����Ƶ�ƫ�ػ��� traits	*/
	template<class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	};


/*	��� const ԭ��ָ�����Ƶ�ƫ�ػ��� traits	*/
	template<class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T								value_type;
		typedef ptrdiff_t						difference_type;
		typedef const T*						pointer;
		typedef const T&						reference;
	};


/*	����������Ժܷ���ľ���ĳ�������������� (category)	*/	
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
	iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}


/*	����������Ժܷ���ľ���ĳ���������� distance type	*/
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
	distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


/*	����������Ժܷ���ľ���ĳ���������� value type	*/
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


/*	���������� distance ����	*/
	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last) {
			++first;
			++n;
		}
		return n;
	}

	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}

	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
	distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return __distance(first, last, category());
	}


/*	���������� advance ����	*/
	template<class InputIterator, class Distance>
	inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
		while (n--)
			++i;
	}

	template<class BidirectionalIterator, class Distance>
	inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--) ++i;
		else
			while (n++) --i;
	}

	template<class RandomAccessIterator, class Distance>
	inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
		i += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n) {
		__advance(i, n, iterator_category(i));
	}


/*	SGI STL ���ͱ������Ƶ�����	*/

/* ������������������ʵĶ���ʹ����������Ӧ������в����Ƶ� */
	struct __true_type	{	};
	struct __false_type {	};

/*	ʵ���� SGI STL �� type_traits.h �ļ�	*/
/*	__type_traits ������ȡ�ͱ������	*/

	template<class type>
	struct __type_traits {
		typedef __true_type this_dummy_member_must_be_first;
			/* ��Ҫ�Ƴ������Ա����֪ͨ���������� __type_traits �ػ����ı�����˵���������������������		*/
			/* __type_traits template ������ġ�����Ϊ��ȷ����һ������Ҳʹ��һ����Ϊ __type_traits ����		*/
			/* ʵ��˴����岢���κι����� template���������鶼�Խ�˳��������								*/

		/* ��������Ӧ�ñ����أ���Ϊ�������п����Զ�Ϊ���ͱ����ר���� __type_traits �ػ��汾�� */
			/* ����������������³�Ա�Ĵ���										*/
			/* ������Ƴ�һ���κγ�Ա											*/
			/* ���Բ����Խ����³�Ա��������ȴû�иı�������еĶ�Ӧ����			*/
			/* �¼���ĳ�Ա�ᱻ��Ϊһ���Ա���������ڱ������м����ʵ���֧��		*/

		typedef __false_type	has_trivial_default_constructor;
		typedef __false_type	has_trivial_copy_constructor;
		typedef __false_type	has_trivial_assignment_operator;
		typedef __false_type	has_trivial_destructor;
		typedef __false_type	is_POD_type;
	};


/* ������ type_traits.h �ж�����c++�����ͱ�������� __type_traits �ػ��汾����Щ��������ڽ���	__type_traits */
/* ֧�������ı����������˺��������޸õ�֧�������ı����������ڱ�Ҫ��		����ΪC++�����ͱ��ṩ�ػ��汾��ÿһ�� */
/* ��Աֵ���� __true_type����ʾ��Щ�ͱ𶼿��Բ�������ٷ�ʽ(����memcpy)���п�������ֵ�Ȳ�����				  */

	__STL_TEMPLATE_NULL struct __type_traits<char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<signed char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<short> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<int> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<float> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<double> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	__STL_TEMPLATE_NULL struct __type_traits<long double> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};

	//���������ԭ��ָ����Ƶ�ƫ�ػ��汾��ԭ��ָ��Ҳ����Ϊһ�ֱ����ͱ�
	template<class T>
	struct __type_traits<T*> {
		typedef __true_type		has_trivial_default_constructor;
		typedef __true_type		has_trivial_copy_constructor;
		typedef __true_type		has_trivial_assignment_operator;
		typedef __true_type		has_trivial_destructor;
		typedef __true_type		is_POD_type;
	};


__mySTL_END__

#endif	//end of __ITERATOR_H__