#include <mySTL/4stl_vector.h>
#include <iostream>
#include <stdio.h>

using namespace lfp;
using namespace std;

struct Test {
	Test(int n = 0) {
		++count_;
		if (n != 0)
			data_ = n;
		else
			data_ = count_;
	}

	int data_;
	static int count_;
};

int Test::count_ = 0;


int main() {
<<<<<<< HEAD
	//6�ֳ�ʼ����ʽ
=======
<<<<<<< HEAD
	//6�ֳ�ʼ����ʽ
=======
	//6
>>>>>>> update
>>>>>>> update
	vector<Test> vec1;
	vector<Test> vec2(10);
	vector<Test> vec3(10, Test(10));
	vector<Test> vec4(vec2.begin() + 5, vec2.end());
	vector<Test> vec5(vec3);
	vector<int> vec6 = { 10, 20, 30 };

	cout << "vec4.size() = " << vec4.size() << ", vec4.capacity() = " << vec4.capacity() << endl;
	cout << "vec5.size() = " << vec5.size() << ", vec5.capacity() = " << vec5.capacity() << endl << endl;
	
	vec5.push_front(Test(100));
	vec5.push_back(Test(100));
	for (int i = 0; i < vec5.size(); ++i) {
		printf("vec5[%d].data = %d\n", i, vec5[i].data_);
	}
	cout << "vec5.size() = " << vec5.size() << ", vec5.capacity() = " << vec5.capacity() << endl << endl;

	
<<<<<<< HEAD
	//�۲�vector���ݻ���
=======
<<<<<<< HEAD
	//�۲�vector���ݻ���
=======
	//vector
>>>>>>> update
>>>>>>> update
	cout << "vec6.size() = " << vec6.size() << "  vec6.capacity() = " << vec6.capacity() << endl;
	vec6.push_back(1);
	cout << "vec6.size() = " << vec6.size() << "  vec6.capacity() = " << vec6.capacity() << endl;
	vec6.push_back(2);
	cout << "vec6.size() = " << vec6.size() << "  vec6.capacity() = " << vec6.capacity() << endl;
	vec6.push_back(3);
	cout << "vec6.size() = " << vec6.size() << "  vec6.capacity() = " << vec6.capacity() << endl;
	vec6.push_back(4);
	cout << "vec6.size() = " << vec6.size() << "  vec6.capacity() = " << vec6.capacity() << endl;
	vec6.push_back(5);
	for (auto i : vec6) {
		printf("%d ", i);
	}
	cout << endl << endl;

<<<<<<< HEAD
	vector<int>::iterator it1 = find(vec6.begin(), vec6.end(), 1);	//�ҵ�1
	it1 = vec6.erase(it1, it1 + 3);		//ɾ��1 2 3��it1ָ��4
	vec6.insert(it1, 10, 1000);			//��4ǰ�����10��1000
=======
<<<<<<< HEAD
	vector<int>::iterator it1 = find(vec6.begin(), vec6.end(), 1);	//�ҵ�1
	it1 = vec6.erase(it1, it1 + 3);		//ɾ��1 2 3��it1ָ��4
	vec6.insert(it1, 10, 1000);			//��4ǰ�����10��1000
=======
	vector<int>::iterator it1 = find(vec6.begin(), vec6.end(), 1);	//1
	it1 = vec6.erase(it1, it1 + 3);		//1 2 3t14
	vec6.insert(it1, 10, 1000);			//4101000
>>>>>>> update
>>>>>>> update
	for (int i = 0; i < vec6.size(); ++i) {
		printf("vec6[%d] = %d\n", i, vec6[i]);
	}
	cout << "vec6.size() = " << vec6.size() << ", vec6.capacity() = " << vec6.capacity() << endl << endl;

	return 0;
}