#pragma once
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "deque.h"		//�� deque ��Ϊ��ײ�����
#include "list.h"		//Ҳ���� list ��Ϊ��ײ�����


__mySTL_BEGIN__

	//ʡȱ��deque��Ϊ�ײ�������Ҳ��ָ��Ϊ list
	template<class T, class Sequence = deque<T>>
	class queue {
	public:
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::reference		reference;
		typedef typename Sequence::const_reference	const_reference;
	protected:
		Sequence c;		//�ײ�����

		/* �������õײ������Ĳ���ʵ��queue�Ķ���ӿ� */
	public:
		queue() : c() {}
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const value_type& x) { c.push_back(x); }
		void pop() { c.pop_front(); }

		bool operator==(const queue<T, Sequence>& x) {
			return (c == x.c);
		}
		bool operator<(const queue<T, Sequence>& x) {
			return (c < x.c);
		}
	};


__mySTL_END__

#endif	//end of __QUEUE_H__