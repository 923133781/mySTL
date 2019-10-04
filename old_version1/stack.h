#pragma once
#ifndef __STACK_H__
#define __STACK_H__

#include "deque.h"		//�� deque ��Ϊ��ײ�����
#include "list.h"		//Ҳ���� list ��Ϊ��ײ�����


__mySTL_BEGIN__

	//ʡȱ�� deque ��Ϊ�ײ�������Ҳ��ָ��Ϊ list
	template<class T, class Sequence = deque<T>>
	class stack {
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::const_reference	const_reference;
	protected:
		Sequence c;		//�ײ�����

		/* �������õײ������Ĳ���ʵ��stack����ӿ� */
	public:
		stack() : c() {}
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference top() { return c.back(); }
		const_reference top() const { return c.back(); }
		void push(const value_type& x) { c.push_back(x); }
		void pop() { c.pop_back(); }

		bool operator==(const stack<T, Sequence>& x) {
			return (c == x.c);
		}
		bool operator<(const stack<T, Sequence>& x) {
			return (c < x.c);
		}
	};


__mySTL_END__

#endif	//end of __STACK_H__