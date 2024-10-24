#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>
#include<math.h>
#include"LinkedList.h"
using namespace std;

void color() {
	int arr[10][10] = {0};
	FILE* f = fopen(R"(I:\Demo\ʵ��7\Project1\class.in)", "r");
	for (int i = 0; i < 10; ++i) {
		int a, b, c;
		fscanf(f,"%d%d%d", &a, &b, &c);
		arr[a][b] = 1; arr[b][a] = 1;
		arr[a][c] = 1; arr[c][a] = 1;
		arr[b][c] = 1; arr[c][b] = 1;
	}
	fclose(f);
	LinkedList<int> li[10];
	int day = 0;
	auto a_with_b = [&arr](int i,int j)->bool {

	};
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			int flag = 0;
			for (auto k = li[j].getHead(); k; k = k->getNext()) {
				if (arr[i][k->data]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				li[j].add(i);
				day = day > j ? day : j;
				break;
			}
		}
	}
	printf("������Ҫ������:%d\n", day+1);
	for (int i = 0; i <= day; ++i) {
		printf("�� %d ��: ", i);
		for (auto j = li[i].getHead(); j; j = j->getNext()) {
			printf("%d ", j->data);
		}
		putchar('\n');
	}
}

void student_rank() {
	FILE* f = fopen(R"(I:\Demo\ʵ��7\Project1\score.in)", "r");
	int total;
	fscanf(f, "%d", &total);

	typedef struct {
		int id;
		char name[10];
		int class_id;
		int score[10];
		int scoreTotal;
	}student;

	student *s = new student[total];
	for (int i = 0; i < total; ++i) {
		fscanf(f,"%d%s%d", &s[i].id, &s[i].name,&s[i].class_id);
		s[i].scoreTotal = 0;
		for (int j = 0; j < 10; ++j) {
			fscanf(f,"%d", &s[i].score[j]);
			s[i].scoreTotal += s[i].score[j];
		}
	}
	LinkedList<student*> li;
	for (int i = 0; i < total; ++i) {
		li.add(&s[i]);
	}
	fclose(f);
	auto print_head = []()->void {
		printf("ѧ��       ����  �༶ �ɼ�                                    �ܳɼ�\n");
	};
	auto print_student = [](const student* s)->void {
		printf("%d %s %-4d", s->id, s->name, s->class_id);
		for (int j = 0; j < 10; ++j) {
			printf(" %-3d", s->score[j]);
		}
		printf(" %-4d", s->scoreTotal);
		printf("\n");
	};
	//���ճɼ�����-1.1
	printf("���ճɼ�����\n");
	li.sort([](auto a, auto b)->bool {
		return a->scoreTotal > b->scoreTotal;
		});
	print_head();
	for (auto i = li.getHead(); i; i = i->getNext()) {
		print_student(i->data);
	}
	//���հ༶���� ���ѧ������-1.2
	printf("���հ༶���� ���ѧ������\n");
	li.sort([](auto a, auto b)->bool {
		if (a->class_id == b->class_id)
			return a->scoreTotal > b->scoreTotal;
		else return a->class_id < b->class_id;
		});
	print_head();
	for (auto i = li.getHead(); i; i = i->getNext()) {
		print_student(i->data);
	}
	//���ָ���༶-1.3
	printf("���յ�һ�� ѧ���ܳɼ�����\n");
	int tage_class = 1;
	int tage_course = 0;
	LinkedList<student*&> li_class1 = li.searchAll([&tage_class](auto a)->bool {
		if (a->class_id == tage_class)return true;
		return false;
		});
	li_class1.sort([](auto a, auto b)->bool {
		return a->scoreTotal > b->scoreTotal;
		});
	print_head();
	for (auto i = li_class1.getHead(); i; i = i->getNext()) {
		print_student(i->data);
	}
	//���շ�����ͳ�Ʋ����-2.1
	printf("���շ�����ͳ�Ʋ����\n");
	int ranks[5] = {};
	int temp = 0, temp2 = 0, k = 0;
	li.sort([](auto a, auto b)->bool {
		return a->scoreTotal > b->scoreTotal;
		});
	for (auto i = li.getHead(); i; i = i->getNext()) {
		if (i->data->scoreTotal >= 900)++ranks[0];
		else if (i->data->scoreTotal >= 800)++ranks[1];
		else if (i->data->scoreTotal >= 700)++ranks[2];
		else if (i->data->scoreTotal >= 600)++ranks[3];
		else ++ranks[4];
	}
	print_head();
	for (auto i = li.getHead(); i; i = i->getNext()) {
		for (int j = 0; j < 5; ++j) {
			if (temp2 == temp) {
				printf("���·�������%d ��:\n", ranks[k]);
				temp += ranks[k];
				++k;
			}
		}
		print_student(i->data);
		temp2++;
	}
	//ĳ�ſηֶβ�����-2.2
	printf("���յ�һ�ſη�����ͳ�Ʋ����\n");
	for (int i = 0; i < 5; ++i) {
		ranks[i] = 0;
	}
	temp = 0, temp2 = 0, k = 0;
	li.sort([&tage_course](auto a, auto b)->bool {
		return a->score[tage_course] > b->score[tage_course];
		});
	for (auto i = li.getHead(); i; i = i->getNext()) {
		if (i->data->score[tage_course] >= 90)++ranks[0];
		else if (i->data->score[tage_course] >= 80)++ranks[1];
		else if (i->data->score[tage_course] >= 70)++ranks[2];
		else if (i->data->score[tage_course] >= 60)++ranks[3];
		else ++ranks[4];
	}
	print_head();
	for (auto i = li.getHead(); i; i = i->getNext()) {
		for (int j = 0; j < 5; ++j) {
			if (temp2 == temp) {
				printf("���·�������%d ��:\n", ranks[k]);
				temp += ranks[k];
				++k;
			}
		}
		print_student(i->data);
		temp2++;
	}
	//��һ�༶��һ������-2.3
	printf("���յ�һ�༶��һ�ſη�����ͳ�Ʋ����\n");
	for (int i = 0; i < 5; ++i) {
		ranks[i] = 0;
	}
	temp = 0, temp2 = 0, k = 0;
	li_class1.sort([&tage_course](auto a, auto b)->bool {
		return a->score[tage_course] > b->score[tage_course];
		});
	for (auto i = li_class1.getHead(); i; i = i->getNext()) {
		if (i->data->class_id != 1)break;
		if (i->data->score[tage_course] >= 90)++ranks[0];
		else if (i->data->score[tage_course] >= 80)++ranks[1];
		else if (i->data->score[tage_course] >= 70)++ranks[2];
		else if (i->data->score[tage_course] >= 60)++ranks[3];
		else ++ranks[4];
	}
	print_head();
	for (auto i = li_class1.getHead(); i; i = i->getNext()) {
		for (int j = 0; j < 5; ++j) {
			if (temp2 == temp) {
				printf("���·�������%d ��:\n", ranks[k]);
				temp += ranks[k];
				++k;
			}
		}
		print_student(i->data);
		temp2++;
	}
	//���ҵ�һ�ſε�[40-60)�����ε���-3.1
	printf("���ҵ�һ�ſε�[40-60)�����ε��˲����\n");
	auto li_course1=li.searchAll([&tage_course](auto i)->bool {
		if (i->score[tage_course] >= 40 && i->score[tage_course] < 60)return true;
		else return false;
		});
	li_course1.sort([&tage_course](auto a, auto b)->bool {
		return a->score[tage_course] > b->score[tage_course];
		});
	print_head();
	for (auto i = li_course1.getHead(); i; i = i->getNext()) {
		print_student(i->data);
	}
	//���ҵ�һ���һ�ſε�[40-60)�����ε���-3.2
	printf("���ҵ�һ�ſε�[40-60)�����ε��˲����\n");
	auto li_class1_course1 = li_class1.searchAll([&tage_course](auto i)->bool {
		if (i->score[tage_course] >= 40 && i->score[tage_course] < 60)return true;
		else return false;
		});
	li_class1_course1.sort([&tage_course](auto a, auto b)->bool {
		return a->score[tage_course] > b->score[tage_course];
		});
	print_head();
	for (auto i = li_class1_course1.getHead(); i; i = i->getNext()) {
		print_student(i->data);
	}
	//����-3.3
	printf("����2020060024\n");
	int id = 2020060024;
	print_student(li.search([&id](auto a)->bool {
		if (a->id == id)return true;
		else return false;
		}));
	delete[] s;
}
int main(){
	printf("���԰���:\n");
	color();
	printf("�ɼ�����\n");
	student_rank();
	return 0;
}