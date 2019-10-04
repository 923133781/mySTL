#pragma once
#ifndef __HEAP_H__
#define __HEAP_H__

#include "iterator.h"		//�������ͱ����
#include "vector.h"

__mySTL_BEGIN__

	//ʡȱ��vector��Ϊ�ײ�������Ҳ��ָ��Ϊ����
	template<class T, class Sequence = vector<T>>
	class heap {
	public:
		typedef typename Sequence::iterator			iterator;
		typedef typename Sequence::value_type		value_type;
		typedef typename Sequence::size_type		size_type;
		typedef typename Sequence::difference_type	Distance;
		typedef typename Sequence::reference		reference;
	
		Sequence C;		//�ײ�����

	protected:
		/* ���ݳ��� */
		/* first:�ײ�������ʼλ��  topIndex:�Ѷ�λ��  holeIndex:����  value:��Ҫ���ݵ�Ԫ��ֵ */
		void percolate_up(iterator first, Distance topIndex, Distance holeIndex, value_type value);

		/* ���ݳ��� */
		/* first:�ײ�������ʼλ��  holeIndex:����  len:�ѵ��ܳ���  value:��Ҫ���ݵ�Ԫ��ֵ */
		void percolate_down(iterator first, Distance holeIndex, Distance len, value_type value);

		/* �� heap ���Ƴ��������ӵײ��������Ƴ� */
		void pop_heap(iterator first, iterator last) {
			value_type value = *(last - 1);
			*(last - 1) = *first;			//���Ѷ��ƶ����ײ�������β��

			percolate_down(first, Distance(0), Distance(last - first - 1), value);
			/* �������δ��룺�ײ�������ʼλ�� ���� �ѵ��ܳ��� ��Ҫ���ݵ�Ԫ��ֵ */
		}

		/* �������õײ������Ĳ���ʵ��queue�Ķ���ӿ� */
	public:
		heap() : C() { }
		heap(const Sequence& x) : C(x) { make_heap(); }
		heap(std::initializer_list<T> init_list) : C(init_list) { make_heap(); }

		bool empty() const { return C.empty(); }
		size_type size() const { return C.size(); }

		/* top���ضѶ������������/��Сֵ */
		reference top() { return C.front(); }

		/* �����ѹ��Ԫ�� */
		void push(const value_type& x) {
			C.push_back(x);
			percolate_up(C.begin(), Distance(0), Distance(C.size() - 1), C.back());
			/* �������δ��룺�ײ�������ʼλ�� �Ѷ�λ�� ���� �²���Ԫ��ֵ */
		}

		/* �Ӷ��е���Ԫ�� */
		void pop() {
			pop_heap(C.begin(), C.end());	//�ȵ�����
			C.pop_back();	//�ٵ����ײ�����
		}

		/* �Եײ��������� */
		void sort() {
			iterator first = C.begin();
			iterator last = C.end();
			while (last - first > 1) {
				pop_heap(first, last--);	//ÿ�ο۳�һ��β��Ԫ��
			}
		}

		/* ����ʹ�ѵĵײ���������ѵ�Ҫ�� */
		/* �䱾�ʹ���Ϊ�������ķ�Ҷ�ڵ㿪ʼ����ÿ����Ҷ�ڵ�ִ��һ�����ݳ��� */
		void make_heap() {
			iterator first = C.begin();
			iterator last = C.end();

			Distance len = last - first;
			if (len < 2) return;		//�������Ϊ0��1���򲻱�����Ԫ��

			/* �ҳ���һ����Ҫ���ŵĽڵ㣬��holeIndex������ӵ�һ����Ҷ�ڵ㿪ʼ */
			Distance holeIndex = (len - 2) / 2;
			while (holeIndex >= 0) {
				percolate_down(first, holeIndex, len, *(first + holeIndex));
				--holeIndex;
			}
		}

		bool operator==(const heap<T, Sequence>& x) {
			return (C == x.C);
		}
		bool operator<(const heap<T, Sequence>& x) {
			return (C < x.C);
		}
	};


	/* ���ݳ��� */
	/* first:�ײ�������ʼλ��  topIndex:�Ѷ�λ��  holeIndex:����  value:��Ҫ���ݵ�Ԫ��ֵ */
	template<class T, class Sequence>
	void heap<T, Sequence>::percolate_up(iterator first, Distance topIndex, Distance holeIndex, value_type value)
	{
		Distance parent = (holeIndex - 1) / 2;		//�ҵ����ŵĸ��ڵ�

		/* ����δ�ﵽ�������²����ֵ���ڶ��Ÿ��ڵ� */
		while (holeIndex > topIndex && value > *(first + parent))
		{
			*(first + holeIndex) = *(first + parent);		//�ֵΪ���ڵ�ֵ
			holeIndex = parent;					//�������ţ��������������ڵ�
			parent = (holeIndex - 1) / 2;		//�ҵ��¶��ŵĸ��ڵ�
		}
		/* �ֵΪ��ֵ������������ */
		*(first + holeIndex) = value;
	}


	/* ���ݳ��� */
	/* first:�ײ�������ʼλ��  holeIndex:����  len:�ѵ��ܳ���  value:��Ҫ���ݵ�Ԫ��ֵ */
	template<class T, class Sequence>
	void heap<T, Sequence>::percolate_down(iterator first, Distance holeIndex, Distance len, value_type value)
	{
		Distance maxChild = 2 * (holeIndex + 1);	//�ҵ����ŵ����ӽڵ�
		while (maxChild < len)			//�����������Һ��Ӷ��еĽڵ㣨maxChild == len��ʾ����ֻ������û���Һ��ӣ�
		{
			/* �ҵ����Žϴ���ӽڵ㣬�� maxChild ��� */
			if (*(first + maxChild) < *(first + maxChild - 1)) {
				--maxChild;
			}

			if (value >= *(first + maxChild))		//��� value ��С�ڶ��Žϴ��ӽڵ�ֵ�������ҵ��� lalue Ӧ�����λ��
				break;

			*(first + holeIndex) = *(first + maxChild);		//�ֵΪ�ϴ��ӽڵ�ֵ
			holeIndex = maxChild;				//�����������ϴ��ӽڵ㴦
			maxChild = 2 * (holeIndex + 1);		//�ҵ��¶��ŵ��Һ���
		}
		if (maxChild == len) {			//maxChild == len��ʾ����ֻ������û���Һ���
			if (*(first + maxChild - 1) > value) {				//�������ֵ���ڲ���ֵ
				*(first + holeIndex) = *(first + maxChild - 1);	//������ֵ��䵽����
				holeIndex = maxChild - 1;		//�������Ƶ����Ӵ�
			}
		}
		*(first + holeIndex) = value;		//��䶴��
	}


__mySTL_END__

#endif	//end of __HEAP_H__