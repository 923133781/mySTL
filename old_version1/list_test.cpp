#include "pch.h"
#include <iostream>
#include "list.h"
#include "hashtable.h"

using namespace mySTL;
using namespace std;

/* ע�⣺list �ĵ�����Ϊ˫�������û�� +n ������ֻ�� ++ -- */

int main() {
	list<int> ilist;
	list<int>::iterator it;

	ilist.push_back(9);
	ilist.push_back(1);
	ilist.push_back(5);
	ilist.push_back(8);
	ilist.push_back(3);
	ilist.push_back(7);
	ilist.push_back(5);
	ilist.push_back(4);
	ilist.push_back(8);
	ilist.push_back(6);
	ilist.push_back(2);
	ilist.push_back(5);
	cout << "ԭʼ״��: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.sort();
	cout << "�����: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	if ((it = find(ilist.begin(), ilist.end(), 5)) != ilist.end()) {
		ilist.insert(it, 99);
		cout << "��5֮ǰ����99: ilist.size() = " << ilist.size() << endl;
		for (it = ilist.begin(); it != ilist.end(); ++it) {
			cout << *it << " ";
		}
		cout << endl << endl;
	}


	ilist.unique();
	cout << "ȥ�������ظ���: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.reverse();
	cout << "��ת��: ilist.size() = " << ilist.size() << endl;
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	return 0;
}