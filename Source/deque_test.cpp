#include "pch.h"
#include <iostream>
#include "deque.h"

using namespace mySTL;
using namespace std;


int main() {
	deque<int, alloc, 8> que(20, 9);		//����һ��ӵ��20��int��Ԫ�س�ֵΪ9��������Ϊ8��Ԫ�ص�deque
	//Ϊÿ��Ԫ���趨��ֵ
	for (int i = 0; i < que.size(); ++i) {
		que[i] = i;
	}

	cout << "1: que.size() = " << que.size() << endl;
	for (int i : que)
		cout << i << " ";
	cout << endl << endl;


	//��β��ѹ������Ԫ�أ���ʱ�����������պ�����������ҿ�ԭ�����һ���յ�λ�ã�
	for (int i = 0; i < 3; ++i) {
		que.push_back(20 + i);
	}
	cout << "2: que.size() = " << que.size() << endl;
	for (int i : que) {
		cout << i << " ";
	}
	cout << endl << endl;

	//��β������һ��Ԫ�� 23����ʱ��Ҫ�����µĻ�����
	que.push_back(23);
	cout << "3: que.size() = " << que.size() << endl;
	for (int i : que) {
		cout << i << " ";
	}
	cout << endl << endl;


	//��ǰ��ѹ������Ԫ��
	for (int i = 0; i < 3; ++i) {
		que.push_front(100 + i);
	}
	cout << "4: que.size() = " << que.size() << endl;
	for (int i : que) {
		cout << i << " ";
	}
	cout << endl << endl;


	que.erase(que.begin() + 4, que.begin() + 10);
	cout << "5: que.size() = " << que.size() << endl;
	for (deque<int, alloc, 8>::iterator it = que.begin(); it < que.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	que.insert(que.begin() + 10, 8888);
	cout << "6: que.size() = " << que.size() << endl;
	for (deque<int, alloc, 8>::iterator it = que.begin(); it < que.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;

	return 0;
}