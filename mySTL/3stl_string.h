/*
 *ʵ���˼򵥵�string��ͳ��ò���
 */

#ifndef _STL_STRING_H_
#define _STL_STRING_H_

#include "1stl_alloctor.h"
#include <cstddef>		//for ptrdiff_t

namespace lfp {
	class string {
	public:
		typedef char			value_type;
		typedef value_type*		pointer;
		typedef value_type*		iterator;
		typedef value_type&		reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;
		typedef string			self;
	private:
		size_t size_;
		char* buffer_;

		//ר���Ŀռ�������������ʱ�������ֽ�Ϊ��λ��������Ԫ�ظ���Ϊ��λ
		typedef simple_alloc<value_type, alloc> data_allocator;

		//�ڲ����������ù������ͷ�vectorռ�õ����пռ�
		void deallocate() {
			if (buffer_) {
				data_allocator::deallocate(buffer_, size_ + 1);
			}
			buffer_ = 0;
			size_ = 0;
		}
	public:
		//5�ֹ��췽ʽ
		string();	//1 Ĭ�Ϲ���
		string(const string& str);	//2 ��������
		string(const string& str, size_t pos, size_t len = 0);
		string(const char* s);		//3 const char* ����
		string(const char* s, size_t n);
		string(iterator first, iterator last);	//4 ���乹��
		string(size_t n, char c);	//5 n char����
		~string();

		const char* c_str() const;
		size_t length() const;
		size_t size() const;
		iterator begin();
		iterator end();

		char& operator[](size_t pos);
		const char& operator[](size_t pos) const;
		string& operator=(const string& str);
		string& operator=(const char* s);
		string& operator=(char c);
		string& operator+=(const string& str);
		string& operator+=(const char* s);
		string& operator+=(char c);

		string& append(const string& str);
		string& append(const char* s);

		string& assign(const string& str);
		string& assign(const char* s);

		char& at(size_t pos);
		const char& at(size_t pos) const;

		void clear();

		int compare(const string& str) const;
		int compare(const char* s) const;

		const char* data() const;
		bool empty() const;
	};


/* ȫ�ֵ�string�������� */
	string operator+(const string& lhs, const string& rhs);

	string operator+(const string& lhs, const char* rhs);

	string operator+(const char* lhs, const string& rhs);

	string operator+(const string& lhs, char rhs);

	string operator+(char lhs, const string& rhs);

	bool operator==(const string& lhs, const string& rhs);

	bool operator==(const char* lhs, const string& rhs);

	bool operator==(const string& lhs, const char* rhs);

	bool operator!=(const string& lhs, const string& rhs);

	bool operator!=(const char* lhs, const string& rhs);

	bool operator!=(const string& lhs, const char* rhs);

	bool operator<(const string& lhs, const string& rhs);

	bool operator<(const char* lhs, const string& rhs);

	bool operator<(const string& lhs, const char* rhs);

	bool operator<=(const string& lhs, const string& rhs);

	bool operator<=(const char* lhs, const string& rhs);

	bool operator<=(const string& lhs, const char* rhs);

	bool operator>(const string& lhs, const string& rhs);

	bool operator>(const char* lhs, const string& rhs);

	bool operator>(const string& lhs, const char* rhs);

	bool operator>=(const string& lhs, const string& rhs);

	bool operator>=(const char* lhs, const string& rhs);

	bool operator>=(const string& lhs, const char* rhs);


}// end of namespace lfp

#endif // !_STL_STRING_H_