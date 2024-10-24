#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<stdio.h>
#include"LinkedList.h"
#include"CycleLinkedList.h"
using std::string;
using std::cout;
using std::endl;
class Node {
private:
	//������ӡ�ڵ�ķ���
	void printLevel() {
		if (parient) {
			parient->printLevel();
		}
		cout << "�� ";
	}
public:
	enum class Types
	{
		CLASS=0,TEACHER=1, GRADUATE_STUDENT=2, UNGRADUATE_STUDENT=3
	};
	string name;
	string info;
	Types type;
	Node* parient;
	LinkedList<Node*> child;
	Node(string name,Types type):name(name),type(type),parient(nullptr){}
	Node(string name,Types type, Node* parient) :name(name), type(type), parient(parient) {
		parient->child.add(this);
	}
	//��ӡ�ڵ�
	void printNode() {
		cout << name << endl;
		for (auto i = child.getHead(); i; i = i->getNext()) {
			if (parient)parient->printLevel();
			cout << "�� ";
			i->data->printNode();
		}
	}
	//���һ���ӽڵ�
	void addChild(Node* child) {
		this->child.add(child);
		child->parient = this;
	}
	Node* search(string s) {
		if (name == s)return this;
		else {
			for (auto i = child.getHead(); i; i = i->getNext()) {
				if (i->data->name == s)return i->data;
				else {
					auto ret = i->data->search(s);
					if (ret)return ret;
				}
			}
		}
		return nullptr;
	}
	//���һ���ӽڵ㲢���Խ��ӽڵ��̯���о����ӽڵ���
	void addChild(LinkedList<Node*> child) {
		CycleLinkedList<Node*> gs;
		for (auto i = this->child.getHead(); i; i = i->getNext()) {
			if ((this->type==Node::Types::TEACHER&&i->data->type == Node::Types::GRADUATE_STUDENT)
				||(this->type==Node::Types::CLASS))gs.add(i->data);
		}
		if (!gs.isEmpty()) {
			auto temp = gs.getHead();
			for (auto i = child.getHead(); i; i = i->getNext(),temp=temp->getNext()) {
				temp->data->addChild(i->data);
			}
		}
		else {
			this->child.addAll(child);
		}
	}
	void changeParient(Node *n) {
		remove();
		n->addChild(this);
	}
	void moveChild(Node* n) {
		n->addChild(child);
		child.clear();
	}
	void remove() {
		if (parient) {
			for (auto i = parient->child.getHead(); i; i = i->getNext()) {
				if (i->data == this) {
					parient->child.remove(i);
					break;
				}
			}
			parient->addChild(child);
		}
		child.clear();
	}
	int size() {
		int total = 0;
		for (auto i = child.getHead(); i; i = i->getNext()) {
			total += i->data->size();
		}
		return child.size() + total;
	}
};
int main() {
	//����
	Node classes("�༶1", Node::Types::CLASS);
	//����
	Node teacher("��ʦ1", Node::Types::TEACHER,&classes);
	Node ung1("������1", Node::Types::UNGRADUATE_STUDENT,&teacher);
	Node ung2("������2", Node::Types::UNGRADUATE_STUDENT, &teacher);
	Node g1("�о���1", Node::Types::GRADUATE_STUDENT, &teacher);
	Node ung3("������3", Node::Types::UNGRADUATE_STUDENT, &g1);
	Node ung4("������4", Node::Types::UNGRADUATE_STUDENT, &g1);
	Node g2("�о���2", Node::Types::GRADUATE_STUDENT, &teacher);
	Node teacher2("��ʦ2", Node::Types::TEACHER, &classes);
	Node ung5("������5", Node::Types::UNGRADUATE_STUDENT, &teacher2);
	Node ung6("������6", Node::Types::UNGRADUATE_STUDENT, &teacher2);
	Node teacher3("��ʦ3", Node::Types::TEACHER, &classes);
	Node g3("�о���3", Node::Types::GRADUATE_STUDENT, &teacher);
	Node g4("�о���4", Node::Types::GRADUATE_STUDENT, &teacher);
	Node ung7("������7", Node::Types::UNGRADUATE_STUDENT, &g3);
	Node ung8("������8", Node::Types::UNGRADUATE_STUDENT, &g3);
	//��ѯ
	auto ans = classes.search("������3");
	printf("��ѯ���:%s\n", ans->name.c_str());
	//���
	classes.printNode();
	//ɾ��+˼��3
	printf("������:%d\n", classes.size());
	printf("ɾ���о���1�ڵ�\n");
	g1.remove();
	classes.printNode();
	//ͳ��
	printf("������:%d\n", classes.size());
	printf("ɾ���о���2�ڵ�\n");
	g2.remove();
	classes.printNode();
	printf("������:%d\n", classes.size());
	//˼��4
	printf("ɾ����ʦ2\n");
	teacher2.remove();
	classes.printNode();
	//˼��1
	printf("��������4ת�Ƶ���ʦ3����\n");
	ung4.changeParient(&teacher3);
	classes.printNode();
	//˼��2
	printf("���о���3��ѧ��ת�Ƶ��о���4����\n");
	g3.moveChild(&g4);
	classes.printNode();
	return 0;
}