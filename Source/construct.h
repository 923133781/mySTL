/*�������ͷ�ļ���
 *
 *�����ڴ�ռ����Ĺ��졢�������ƶ��͸��ƵȲ���
 *
 */


#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__

#include "config.h"		//�������ͷ�ļ�
#include "iterator.h"	//������ȡ���ͷ�ļ�
#include "algobase.h"	//�����㷨ͷ�ļ�
#include <new.h>		//��ʹ��placement new�����Ȱ�����ͷ�ļ�
#include <string.h>		//memmove() ����


__mySTL_BEGIN__

/* ����ͽ����Ļ������ߣ�construct() �� destroy()	*/
	template<typename T1, typename T2>
	inline void construct(T1* p, const T2& value) {
		new (p) T1(value);		//placement new������T1::T1(value)
	}

	//������destroy��һ�汾������һ��ָ��
	template<typename T>
	inline void destroy(T* p) {
		p->~T();
	}


/*	������destroy�ڶ��汾�������������������˺����跨�ҳ�Ԫ�ص���ֵ�ͱ� */
/*	��������__type_traits<>��ȡ��ǡ���Ĵ�ʩ */
	template<typename ForwardIterator, typename T>		//��������
	inline void __destroy(ForwardIterator, ForwardIterator, T*);		

	template<typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		__destroy(first, last, value_type(first));
	}

	//�ж�Ԫ�ص���ֵ�ͱ�(value type)�Ƿ���trivial destructor
	template<typename ForwardIterator, typename T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	//���Ԫ����ֵ�ͱ���non-trivial destructor
	template<typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
		for (; first < last; ++first)
			destroy(&*first);
	}

	//���Ԫ����ֵ�ͱ���trivial destructor
	template<typename ForwardIterator>
	inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) { }

	//������destroy�ڶ��汾��Ե�����Ϊchar*��wchar_t*���ػ���
	inline void destroy(char*, char*) { }
	inline void destroy(wchar_t*, wchar_t*) { }



/*	�ڴ����������	*/

/* uninitialized_copy: ��Ӧ�߲�� copy  */
	/*	first:	����˿ռ���ʼλ��		*/
	/*	last:	����˿ռ����λ��		*/
	/*	result:	����ʼ���ռ���ʼλ��	*/
	template<typename InputIterator, typename ForwardIterator, typename T>		//��������
	inline ForwardIterator __uninitialized_copy(InputIterator, InputIterator, ForwardIterator, T*);

	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(result));
		//�������� value_type ȡ�� result �� value type
	}

	/* ����������߼���������ȡ�� result �� value type��Ȼ���жϸ��ͱ��Ƿ�ΪPOD�ͱ� */
	template<typename InputIterator, typename ForwardIterator, typename T>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type is_POD;			//__type_traits<>����ʵ�ּ�3.7
		return __uninitialized_copy_aux(first, last, result, is_POD());
	}

	/* ���copy construct��ͬ��assignment������destruct��trivial�����¾���Ч */

	/* �����POD�ͱ�ִ�����̾ͻ�ת�����溯�������ǽ���function template�Ĳ����Ƶ����ƶ��� */
	template<typename InputIterator, typename ForwardIterator>
	inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		return copy(first, last, result);			//���ɸ߽�STL�㷨����ʵ�ּ� algobase.h
	}

	/* �������POD�ͱ�ִ�����̾ͻ�ת�����溯�������ǽ���function template�Ĳ����Ƶ����ƶ��� */
	template<typename InputIterator, typename ForwardIterator>
	ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
		ForwardIterator cur = result;
		try {
			for (; first != last; ++first, ++cur) {
				construct(&*cur, *first);		//����һ��һ���Ĺ��죬�޷���������
			}
		}
		catch (...) {	//�����쳣��[result,cur)�ڵ�Ԫ������ɹ��죬���뽫��ȫ������
			ForwardIterator p = result;
			for (; p != cur; ++p) {
				destroy(&*p);
			}
			throw;
		}
		return cur;
	}

	/* ��� const char* ���ػ��汾 */
	inline char* uninitialized_copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);		//ֱ���ƶ��ڴ����ݽ��п���
		return result + (last - first);
	}

	/* ��� const wchar_t* ���ػ��汾 */
	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));		//ֱ���ƶ��ڴ����ݽ��п���
		return result + (last - first);
	}


/* uninitialized_fill: ��Ӧ�߲�� fill */
	/*	first: ����ʼ���ռ���ʼλ��	   */
	/*	last:  ����ʼ���ռ����λ��	   */
	/*	x:	   ��ʼ����ֵ			   */
	template<typename ForwardIterator, typename T, typename T1>		//��������
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*);

	template<typename ForwardIterator, typename T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		__uninitialized_fill(first, last, x, value_type(first));
		//�������� value_type ȡ�� first �� value type
	}

	/* ����������߼���������ȡ�� first �� value type��Ȼ���жϸ��ͱ��Ƿ�ΪPOD�ͱ� */
	template<typename ForwardIterator, typename T, typename T1>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;		//__type_traits<>����ʵ�ּ�3.7
		__uninitialized_fill_aux(first, last, x, is_POD());
	}

	/* ���copy construct��ͬ��assignment������destruct��trivial�����¾���Ч */

	/* �����POD�ͱ�ִ�����̾ͻ�ת�����溯�������ǽ���function template�Ĳ����Ƶ����ƶ��� */
	template<typename ForwardIterator, typename T>
	inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type) {
		fill(first, last, x);			//���ɸ߽�STL�㷨����ʵ�ּ� algobase.h
	}

	/* �������POD�ͱ�ִ�����̾ͻ�ת�����溯�������ǽ���function template�Ĳ����Ƶ����ƶ��� */
	template<typename ForwardIterator, typename T>
	void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
		ForwardIterator cur = first;
		try {
			for (; cur != last; ++cur) {
				construct(&*cur, x);		//����һ��һ���Ĺ��죬�޷���������
			}
		}
		catch (...) {	//�����쳣��[first,cur)�ڵ�Ԫ������ɹ��죬���뽫��ȫ������
			ForwardIterator p = first;
			for (; p != cur; ++p) {
				destroy(&*p);
			}
			throw;
		}
	}


/* uninitialized_fill_n: ��Ӧ�߲�� fill_n */
	/*	first: ����ʼ���ռ���ʼ��	*/
	/*	n:	   ����ʼ���ռ��С		*/
	/*	x:	   ��ʼ����ֵ			*/
	template<typename ForwardIterator, typename Size, typename T, typename T1>		//��������
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator, Size, const T&, T1*);

	template<typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return __uninitialized_fill_n(first, n, x, value_type(first));
		//�������� value_type ȡ�� first �� value type
	}

	/* ����������߼���������ȡ�� first �� value type��Ȼ���жϸ��ͱ��Ƿ�ΪPOD�ͱ� */
	template<typename ForwardIterator, typename Size, typename T, typename T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		typedef typename __type_traits<T1>::is_POD_type is_POD;		//__type_traits<>����ʵ�ּ�3.7
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	/* ���copy construct��ͬ��assignment������destruct��trivial�����¾���Ч */

	/* �����POD�ͱ�ִ�����̾ͻ�ת�����溯�������ǽ���function template�Ĳ����Ƶ����ƶ��� */
	template<typename ForwardIterator, typename Size, typename T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
		return fill_n(first, n, x);		//���ɸ߽�STL�㷨����ʵ�ּ� algobase.h
	}

	/* �������POD�ͱ�ִ�����̾ͻ�ת�����溯�������ǽ���function template�Ĳ����Ƶ����ƶ��� */
	template<typename ForwardIterator, typename Size, typename T>
	ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
		ForwardIterator cur = first;
		try {
			for (; n > 0; --n, ++cur) {
				construct(&*cur, x);		//����һ��һ���Ĺ��죬�޷���������
			}
		}
		catch (...) {	//�����쳣��[first,cur)�ڵ�Ԫ������ɹ��죬���뽫��ȫ������
			ForwardIterator p = first;
			for (; p != cur; ++p) {
				destroy(&*p);
			}
			throw;
		}
		return cur;
	}


__mySTL_END__

#endif	//end of __CONSTRUCT_H__