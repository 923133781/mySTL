#pragma once
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "alloctor.h"		//�ռ����������
#include "construct.h"		//�ڴ����Ĺ��졢�������ع���
#include "algobase.h"		//�����㷨���

//#include <initializer_list>


__mySTL_BEGIN__

	//ʡȱʹ�õڶ����ռ�������
	template<class T, class Alloc = alloc>
	class vector {
	public:
		//vector��Ƕ���ͱ���
		typedef T				value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;

	private:
		//simple_alloc��SGI STL�Ŀռ�������
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;				//Ŀǰʹ�ÿռ��ͷ
		iterator finish;			//Ŀǰʹ�ÿռ��β
		iterator end_of_storage;	//Ŀǰ���ÿռ��β

		//insert�ڵ���������position������Ԫ�� x�����ռ䲻����пռ�����
		void insert_aux(iterator position, const T& x);
		//�ڲ����������ù������ͷ�vectorռ�õ����пռ�
		void deallocate() {
			if (start) {
				data_allocator::deallocate(start, end_of_storage - start);
			}
		}

		//ROUND_UP()��byte�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + __ALIGN - 1) & ~(__ALIGN - 1));
		}
		//�����ڴ�ռ䲢�������
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			
			//end_of_storage = finish;		//����д��

			/* ����д������ȷ��ʵ�ʵõ��Ŀռ��СΪ: n * sizeof(value_type)����ȡ 8 ���������ֽ��� */
			end_of_storage = start + ROUND_UP(n * sizeof(value_type)) / sizeof(value_type);
		}

	public:
		vector() : start(0), finish(0), end_of_storage(0) { }
		vector(int n, const T& value) { fill_initialize(n, value); }
		vector(size_type n, const T& value) { fill_initialize(n, value); }
		explicit vector(size_type n) { fill_initialize(n, T()); }
		vector(const vector<T>& x) {				//�������캯��
			fill_initialize(x.size(), T());
			for (int i = 0; i < size(); ++i)
				*(start + i) = *(x.start + i);
		}
		/* �б��ʼ�� */
		vector(std::initializer_list<T> init_list) {
			fill_initialize(init_list.size(), T());
			auto beg = init_list.begin();
			for (int i = 0; i < size(); ++i)
				*(start + i) = *(beg + i);
		}
		~vector() {
			destroy(start, finish);		//����������
			deallocate();			//�ͷ��ڴ�
		}

		iterator begin() { return start; }
		iterator end() { return finish; }
		size_type size() const { return size_type(finish - start); }
		size_type capacity() const { return size_type(end_of_storage - start); }
		reference operator[](size_type n) { return *(begin() + n); }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }

		/* ��positionλ��֮ǰ������ n ��Ԫ�� x  */
		void insert(iterator position, size_type n, const T& x);
		/* ��positionλ��֮ǰ������Ԫ�� x  */
		void insert(iterator position, const T& x) {
			insert(position, 1, x);
		}

		//��Ԫ�ز�����β��
		void push_back(const T& x) {
			if (finish != end_of_storage) {
				construct(finish, x);		//ȫ�ֺ�������β�ι���һ��Ԫ��
				++finish;
			}
			else {
				insert_aux(end(), x);		//����ռ䲢����Ԫ��
			}
		}

		//����β��Ԫ��
		void pop_back() {
			--finish;
			destroy(finish);
		}

		//ɾ��λ�� position �ϵ�Ԫ��
		iterator erase(iterator position) {		
			if (position + 1 != end()) {		//�������ɾ�����һ��Ԫ��
				copy(position + 1, finish, position);	//����Ԫ����ǰ�ƶ������ɸ߽�STL�㷨����ʵ�ּ� algobase.h
			}
			--finish;
			destroy(finish);
			return position;
		}

		//ɾ��[first, last)������Ԫ��
		iterator erase(iterator first, iterator last) {
			//��[last, finish)�������ƶ�����first��ʼλ�ã���������Ƶ���һλ��
			iterator i = copy(last, finish, first);

			/* ���� [i, finish) �ڵ��������� */
			destroy(i, finish);
			finish = i;		//���� finish ָ���µĽ�β
			return first;
		}

		void resize(size_type new_sz, const T& x) {
			if (new_sz < size()) {
				erase(begin() + new_sz, end());
			}
			else {
				insert(end(), new_sz - size(), x);
			}
		}

		void resize(size_type new_sz) {
			resize(new_sz, T());
		}

		void swap(vector<T, Alloc>& vec) {

		}

		void clear() {
			destroy(begin(), end());
		}

	protected:
		//���� n ���ռ䣬�������� value ����
		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}
	};	//end of class vector


	/* insert�ڵ���������position������Ԫ�� x�����ռ䲻����пռ����� */
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {		//���б��ÿռ�
			//�ڱ��ÿռ���ʼ(finish)������һ��Ԫ�أ�����vector���һ��Ԫ��(finish - 1)Ϊ��ʼֵ
			construct(finish, *(finish - 1));
			++finish;		//����ˮλ
			T x_copy = x;

			//�� [position, finish - 2) �ڵ�Ԫ������ƶ�һ��
			copy_backward(position, finish - 2, finish - 1);	//���ɸ߽�STL�㷨����ʵ�ּ� algobase.h
			*position = x_copy;		//�� position ��������Ԫ��
		}
		else {		//û�б��ÿռ�
			const size_type old_size = size();
			const size_type new_size = (old_size == 0) ? 10 : 2 * old_size;
			/* ����Ϊ�ڴ�ռ�����ԭ��: ���ԭ��СΪ0��������10����������ԭ�ռ��С�� 2 �� */
			/* ǰ�����������ԭ���ݣ������������������� */

			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try {
				/* �Ƚ�ԭvector�� [start, position) �����ݿ����� new_start ��ʼ��λ�� */
				new_finish = uninitialized_copy(start, position, new_start);
				
				/* �� new_finish(��position) ����������Ԫ�� x */
				construct(new_finish, x);
				++new_finish;		//����ˮλ

				/* �ٽ�ԭvector�� [position, finish) �����ݿ����� new_finish ��ʼ��λ�� */
				new_finish = uninitialized_copy(position, finish, new_finish);

				//Ԫ�ز�����ɣ��� vector �������
			} catch (...) {
				//�����쳣��������Ӧ�����ٹ���
				destroy(new_start, new_finish);		//�����¿ռ��ϵĶ���
				data_allocator::deallocate(new_start, new_size);		//�ͷſռ�
				throw;
			}

			//�������ͷ�ԭ vector
			destroy(begin(), end());
			deallocate();

			//������������ָ���µ� vector
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}
	}


	/* ��positionλ�ÿ�ʼ������ n ��Ԫ�� x  */
	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
		if (n > 0) {
			if (size_type(end_of_storage - finish) >= n) {
				//���ÿռ���ڵ�������Ԫ�ظ���
				T x_copy = x;
				
				//���¼�������֮�������Ԫ�ظ���
				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n) {
					//�����֮���Ԫ�ظ�����������Ԫ�ظ���
					uninitialized_copy(finish - n, finish, finish);		//�ڱ��ÿռ俪ʼ���ٹ��� n ��Ԫ��
					finish += n;		//��vectorβ�˱�Ǻ���
					copy_backward(position, old_finish - n, old_finish);	//���ɸ߽�STL�㷨����ʵ�ּ� algobase.h
					//�Ӳ���㿪ʼ������ֵ
					fill(position, position + n, x_copy);
				}
				else {
					//�����֮���Ԫ�ظ���С�ڵ�������Ԫ�ظ���
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish = finish + n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else {		//���ÿռ�С������Ԫ�ظ������������ö�����ڴ�
				//���Ⱦ����³��ȣ��ɳ��ȵ��������²���Ԫ�س��ȵ�����
				const size_type old_size = size();
				const size_type new_size = (old_size == 0) ? n : 2 * max(old_size, n);

				//���������µ� vector �ռ�
				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;
				try{
					//�������Ƚ��� vector �в����֮ǰ��Ԫ�ظ��Ƶ��¿ռ���
					new_finish = uninitialized_copy(start, position, new_start);
					//�ٽ�����Ԫ�� n �����¿ռ�
					new_finish = uninitialized_fill_n(new_finish, n, x);
					//��󽫾� vector �в����֮���Ԫ���ƶ����¿ռ�
					new_finish = uninitialized_copy(position, finish, new_finish);
				} catch (...) {
					//�����쳣��ִ����Ӧ�����ٹ���
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, new_size);
					throw;
				}

				//����������ͷž� vector
				destroy(start, finish);
				deallocate();

				//���µ���ˮλ���
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_size;
			}
		}
	}

__mySTL_END__

#endif	//end of __VECTOR_H__