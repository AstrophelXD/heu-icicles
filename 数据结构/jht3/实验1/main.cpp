#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include"CycleLinkedList.h"
#include"Pair.h"

int main() {
	int n;
	printf("����ѧ������\n");
	scanf("%d", &n);
	printf("ÿ�������Ƿ���ͬ\n1:��\n2:��\n");
	int mode;
	scanf("%d", &mode);
	CycleLinkedList<Pair<int, int>> cl;
	if (mode==1) {
		int m;
		printf("������m��ֵ\n");
		scanf("%d", &m);
		for (int i = 0; i < n; ++i) {
			cl.add(Pair<int, int>(i + 1, m));
		}
	}else {
		for (int i = 0; i < n; ++i) {
			int m;
			printf("������m��ֵ\n");
			scanf("%d", &m);
			cl.add(Pair<int, int>(i + 1, m));
		}
	}
	auto data = cl.getHead();
	while (!cl.isEmpyt()) {
		int m = data->data.second;
		for (int i = 0; i < m-1; ++i) {
			data=data->getNext();
		}
		auto rm = data;
		data = data->getNext();
		printf("%d ", rm->data.first);
		cl.remove(rm);
	}
	return 0;
	
}