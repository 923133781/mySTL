/*
 *vectorʵ�֣�
 *
 *����ȱʡʹ���ڲ��ռ�������alloc�����1stl_alloctor.h����ʹ����Ҳ�������д���ռ�������
 */

#ifndef _STL_VECTOR_H_
#define _STL_VECTOR_H_

#include "1stl_alloctor.h"		//�ռ�������
#include "1stl_construct.h"		//������������
#include "1stl_iterator.h"		//for function distance
#include "1stl_uninitialized.h"	//for function uninitialized_*
#include "2stl_algobase.h"		//for diatance
#include <initializer_list>		//for �б��ʼ��
#include <cstddef>				//for ptrdiff_t

namespace lfp {

	//ʡȱʹ�õڶ����ռ�������
	template<class T, class Alloc = alloc>
	class vector {
	public:
		//vector��Ƕ���ͱ���
		/* ��Ϊvector�����������ռ��ڴ�ţ�����ԭ��ָ��Ϳ���Ϊ������� */
		typedef T				value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	private:
		//ר���Ŀռ�������������ʱ�������ֽ�Ϊ��λ��������Ԫ�ظ���Ϊ��λ
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				//Ŀǰʹ�ÿռ��ͷ
		iterator finish;			//Ŀǰʹ�ÿռ��β
		iterator end_of_storage;	//Ŀǰ���ÿռ��β

		//�ڲ����������ù������ͷ�vectorռ�õ����пռ�
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		//����n��Ԫ�صĿռ䲢�������
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		//�ڵ���������pos������n��Ԫ��x�����ռ䲻���������
		void insert_aux(iterator pos, size_type n, const T& x);

	public:		//vector��6�ֳ�ʼ����ʽ
		vector() : start(0), finish(0), end_of_storage(0) { }	//1 Ĭ�Ϲ���
		explicit vector(size_type n) { fill_initialize(n, T()); }	//2 ָ����С
		vector(int n, const T& value) { fill_initialize(n, value); }	//3	����n��value
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		template<class Iterator>
		vector(Iterator first, Iterator last) {		//4 ������[first, last)��ʼ��
			size_type n = distance(first, last);
			start = data_allocator::allocate(n);
			uninitialized_copy(first, last, start);
			finish = start + n;
			end_of_storage = finish;
		}
		vector(const vector<T>& rhs) {		//5 ��������
			start = data_allocator::allocate(rhs.size());
			uninitialized_copy(rhs.start, rhs.finish, start);
			finish = start + rhs.size();
			end_of_storage = finish;
		}
		vector(std::initializer_list<T> init_list) {		//6 �б��ʼ��
			start = data_allocator::allocate(init_list.size());
			uninitialized_copy(init_list.begin(), init_list.end(), start);
			finish = start + init_list.size();
			end_of_storage = finish;
		}
		~vector() {
			if (start)
				destroy(start, finish);		//����������
			deallocate();			//�ͷ��ڴ�
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return finish - start; }
		size_type capacity() const { return end_of_storage - start; }
		reference operator[](size_type n) { return *(start + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		/* ��posλ�ò���n��Ԫ��x */
		void insert(iterator pos, size_type n, const T& x) {
			insert_aux(pos, n, x);
		}
		/* ��posλ�ò���һ��Ԫ��x */
		void insert(iterator pos, const T& x) {
			insert_aux(pos, 1, x);
		}

		void push_front(const T& x) {
			insert_aux(begin(), 1, x);
		}

		void push_back(const T& x) {
			if (finish != end_of_storage) {	  //���δ������ֱ����ĩβ����Ԫ��
				construct(finish, x);
				++finish;
			}
			else {		//������Ҫ����
				insert_aux(end(), 1, x);
			}
		}

		void pop_front() {
			erase(begin());
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}

		iterator erase(iterator pos) {		
			if (pos + 1 != end()) {		//�������ɾ�����һ��Ԫ�أ���Ҫ������Ԫ����ǰ�ƶ�
				copy(pos + 1, finish, pos);
			}
			--finish;
			destroy(finish);
			return pos;
		}

		//ɾ��[first, last)������Ԫ��
		iterator erase(iterator first, iterator last) {
			//��[last, finish)�������ƶ�����first��ʼλ�ã���������Ƶ���һλ��
			iterator it = copy(last, finish, first);

			/* ����[it, finish) �ڵ��������� */
			destroy(it, finish);
			finish = it;		//���� finish ָ���µĽ�β
			return first;
		}

		void resize(size_type new_sz, const T& x) {
			if (new_sz < size()) {
				erase(begin() + new_sz, end());
			}
			else {
				insert_aux(end(), new_sz - size(), x);
			}
		}

		void resize(size_type new_sz) {
			resize(new_sz, T());
		}

		void swap(vector<T>& rhs) {
			swap(start, rhs.start);
			swap(end, rhs.end);
			swap(end_of_storage, rhs.end_of_storage);
		}

		void clear() {
			erase(begin(), end());
		}

	protected:
		//����n��Ԫ�ؿռ䣬���������Ϊvalue
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);		//����n��Ԫ�صĿռ�
			uninitialized_fill_n(result, n, value);		//���Ԫ��
			return result;
		}
	};	//end of class vector



	/* �ڵ���������pos������Ԫ�� x�����ռ䲻��������� */
	//template<class T, class Alloc>
	//void vector<T, Alloc>::insert_aux(iterator pos, const T& x) {
	//	if (finish != end_of_storage) {		//���б��ÿռ�
	//		//�ڱ��ÿռ���ʼ(finish)������һ��Ԫ�أ�����vector���һ��Ԫ��(finish - 1)Ϊ��ʼֵ
	//		construct(finish, *(finish - 1));
	//		++finish;		//����ˮλ
	//		T x_copy = x;

	//		//�� [pos, finish - 2) �ڵ�Ԫ������ƶ�һ��
	//		copy_backward(pos, finish - 2, finish - 1);		//���ɸ߽�STL�㷨����ʵ�ּ� 2stl_algobase.h
	//		*pos = x_copy;		//�� pos ��������Ԫ��
	//	}
	//	else {		//û�б��ÿռ�
	//		const size_type old_capacity = size();
	//		const size_type new_capacity = (old_capacity == 0) ? 8 : 2 * old_capacity;
	//		/* ����Ϊ�ڴ�ռ�����ԭ��: ���ԭ��СΪ0��������8����������ԭ�ռ��С�� 2 �� */
	//		/* ǰ�����������ԭ���ݣ������������������� */

	//		iterator new_start = data_allocator::allocate(new_capacity);
	//		iterator new_finish = new_start;
	//		try {
	//			/* �Ƚ�ԭvector�� [start, pos) �����ݿ����� new_start ��ʼ��λ�� */
	//			new_finish = uninitialized_copy(start, pos, new_start);
	//			
	//			/* �� new_finish(��pos) ����������Ԫ�� x */
	//			construct(new_finish, x);
	//			++new_finish;		//����ˮλ

	//			/* �ٽ�ԭvector�� [pos, finish) �����ݿ����� new_finish ��ʼ��λ�� */
	//			new_finish = uninitialized_copy(pos, finish, new_finish);

	//			//Ԫ�ز�����ɣ��� vector �������
	//		} catch (...) {
	//			//�����쳣��ִ����Ӧ�����ٹ�����ע�⣺��Ϊuninitialized_copy��ѭcommit or rollback
	//			//ԭ�����Թ���ʧ��ʱ����Ҫ�Լ�ִ�ж������ٹ�����ֻ��Ҫ�ͷ�������ڴ�ͺ�
	//			data_allocator::deallocate(new_start, new_capacity);		//�ͷſռ�
	//			throw;		//ע��Ҫ��߲��׳��쳣
	//		}

	//		//�������ͷ�ԭ vector
	//		destroy(begin(), end());
	//		deallocate();

	//		//������������ָ���µ� vector
	//		start = new_start;
	//		finish = new_finish;
	//		end_of_storage = new_start + new_capacity;
	//	}
	//}


	//�ڵ���������pos������n��Ԫ��x�����ռ䲻���������
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, size_type n, const T& x) {
		if (n > 0)
		{
			if (size_type(end_of_storage - finish) >= n) {
				//���ÿռ���ڵ�������Ԫ�ظ���
				T x_copy = x;
				
				//���¼�������֮�������Ԫ�ظ���
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after >= n) {		//�����֮���Ԫ�ظ������ڵ�������Ԫ�ظ���
					uninitialized_copy(finish - n, finish, finish);		//�ڱ��ÿռ俪ʼ���ٹ��� n ��Ԫ��
					finish += n;		//��vectorβ�˱�Ǻ���
					copy_backward(pos, old_finish - n, old_finish);		//��󿽱�ʣ��Ԫ�أ�ʵ�ּ� 2stl_algobase.h
					//�Ӳ���㿪ʼ������ֵ
					fill(pos, pos + n, x_copy);
				}
				else {		//�����֮���Ԫ�ظ���С������Ԫ�ظ���
					uninitialized_fill_n(finish, n - elems_after, x_copy);	//����finish֮����n-elems_after��x_copy����Ϊ����֮�������Ӧ����x_copy��
					finish = finish + n - elems_after;
					uninitialized_copy(pos, old_finish, finish);	//��ԭ��pos���Ԫ�ؿ���������֮����ȷ��λ��
					finish += elems_after;
					fill(pos, old_finish, x_copy);
				}
			}
			else {		//���ÿռ�С������Ԫ�ظ������������ö�����ڴ�
				//���Ⱦ����³��ȣ��ɳ��ȵ��������²���Ԫ�س��ȵ�����
				const size_type old_capacity = capacity();
				const size_type new_capacity = 2 * max(old_capacity, n);

				//���������µ� vector �ռ�
				iterator new_start = data_allocator::allocate(new_capacity);
				iterator new_finish = new_start;
				try{
					//�������Ƚ��� vector �в����֮ǰ��Ԫ�ظ��Ƶ��¿ռ���
					new_finish = uninitialized_copy(start, pos, new_start);
					//�ٽ�����Ԫ�� n �����¿ռ�
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//��󽫾� vector �в����֮���Ԫ���ƶ����¿ռ�
					new_finish = uninitialized_copy(pos, finish, new_finish);
				} catch (...) {
					//�����쳣��ִ����Ӧ�����ٹ�����ע�⣺��Ϊuninitialized_copy��uninitialized_fill_n����ѭ
					//commit or rollbackԭ�����Թ���ʧ��ʱ����Ҫ�Լ�ִ�ж������ٹ�����ֻ��Ҫ�ͷ�������ڴ�ͺ�
					data_allocator::deallocate(new_start, new_capacity);
					throw;		//ע��Ҫ��߲��׳��쳣
				}

				//�������ͷ�ԭ vector
				destroy(begin(), end());
				deallocate();

				//������������ָ���µ� vector
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_capacity;
			}
		}
	}

}// end of namespace lfp

#endif // !_STL_VECTOR_H_