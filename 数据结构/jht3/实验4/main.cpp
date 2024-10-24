#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<algorithm>
#include<stdint.h>
#include<string>
#include<utility>
#include"BinaryTree.h"
#include"Pair.h"
#include"LinkedList.h"
using std::string;
using std::move;
void compress(const char* dir, const char* name);
void decompress(const char* dir, const char* name);
Pair<uint16_t, uint8_t>* serializate(Pair<uint16_t,uint8_t>* pair, BinaryTree<Pair<uint8_t, int>>* tree, uint16_t parient,uint16_t &now);
void treeToList(LinkedList<Pair<uint8_t, string>>& list, BinaryTree<Pair<uint8_t, int>>* tree, string s);
class BitWrite {
private:
	FILE* file;
	int flag;
	uint8_t bits;
	void writeBuffer() {
		if (flag != 7) {
			fwrite(&bits, sizeof(uint8_t), 1, file);
			bits = 0;
			flag = 7;
		}
	}
public:
	BitWrite(char* file) :file(fopen(file,"wb")),flag(7),bits(0) {};
	~BitWrite() {
		close();
	}
	void close() {
		if (file) {
			writeBuffer();
			fclose(file);
			file = nullptr;
		}
	}
	void write(const void* buff,size_t size,size_t count) {
		fwrite(buff, size, count, file);
	}
	void writeBit(uint8_t bit) {
		bit &= 1;
		bit=bit << flag;
		bits |= bit;
		--flag;
		if (flag == -1)
			writeBuffer();
	}

};
class BitRead {
private:
	FILE* file;
	int flag;
	uint8_t bits;
	void readBuffer() {
		flag = 7;
		fread(&bits, sizeof(uint8_t), 1, file);
	}
public:
	BitRead(char* file) :file(fopen(file, "rb")), flag(-1), bits(0) {};
	~BitRead() {
		close();
	}
	void close() {
		if (file) {
			fclose(file);
			file = nullptr;
		}
	}
	void read(void* buff, size_t size, size_t count) {
		fread(buff, size, count, file);
	}
	int readBit() {
		if (flag == -1)
			readBuffer();
		uint8_t bit = bits >> flag;
		bit &= 1;
		--flag;
		return bit;
	}

};
int main() {
	compress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(���ݽṹʵ��ָ����.pdf)");
	compress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(����.txt)");
	compress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(test1.txt)");
	compress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(test2.txt)");
	
	decompress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(���ݽṹʵ��ָ����.pdf.codefile.dat)");
	decompress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(����.txt.codefile.dat)");
	decompress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(test1.txt.codefile.dat)");
	decompress(R"(I:\Demo\ʵ��4\Project1\ѹ���ļ���\)", R"(test2.txt.codefile.dat)");
	
	return 0;
}
void decompress(const char* dir, const char* name) {
	printf("���ڽ�ѹ�ļ�:%s\n", name);
	//�ָ���������
	char filePath[512] = { 0 };
	strcat(filePath, dir);
	strcat(filePath, name);
	auto read = BitRead(filePath);
	BinaryTree<uint8_t>* trees[511] = {0};
	for (int i = 0; i < 511; ++i) {
		uint16_t parient;
		uint8_t c;
		read.read(&parient, sizeof(uint16_t), 1);
		read.read(&c, sizeof(uint8_t), 1);
		trees[i] = new BinaryTree<uint8_t>(c);
		if (parient != UINT16_MAX) {
			if (trees[parient]->lChild == nullptr)trees[parient]->lChild = trees[i];
			else trees[parient]->rChild = trees[i];
		}
	}
	//��ȡ���ֽ���
	uint32_t total;
	read.read(&total, sizeof(total), 1);

	//�ָ��ļ�
	char outFilePath[512] = { 0 };
	strcat(outFilePath, dir);
	strcat(outFilePath, "re_");
	strncat(outFilePath, name, strlen(name) - 13);
	FILE* fOut = fopen(outFilePath, "wb");
	for (int i = 0; i < total; ++i) {
		uint8_t c;
		BinaryTree<uint8_t>* node = trees[0];
		while (1) {
			if (node->lChild == nullptr) {
				c = node->data;
				break;
			}
			if (read.readBit() == 0) {
				node = node->lChild;
			}
			else {
				node = node->rChild;
			}
		}
		fwrite(&c, sizeof(c), 1, fOut);
	}
	fclose(fOut);
	delete trees[0];
}
void compress(const char* dir,const char* name) {
	//��ȡ�ļ�
	printf("����ѹ���ļ�:%s\n", name);
	printf("��ȡ�ļ���\n");
	char filePath[512] = { 0 };
	strcat(filePath, dir);
	strcat(filePath, name);
	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	uint32_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	int* fileContext=new int[fileSize];
	for (int i = 0; i < fileSize; ++i) {
		fileContext[i] = fgetc(file);
	}
	fclose(file);
	//ͳ�ƴ�Ƶ
	printf("��Ƶͳ����\n");
	BinaryTree<Pair<uint8_t, int>>* trees[256];
	for (int i = 0; i < 256; ++i) {
		trees[i] = new BinaryTree<Pair<uint8_t, int>>(Pair<uint8_t, int>(i, 0));
	}
	for (int i = 0; i < fileSize; ++i) {
		++(trees[fileContext[i]]->data.second);
	}
	//����
	printf("��������������\n");
	int total = 256;
	while (total > 1) {
		std::sort(trees, trees+total,
			[](BinaryTree<Pair<uint8_t, int>>* a,const BinaryTree<Pair<uint8_t, int>>* b)->bool {
				return a->data.second > b->data.second; });
		BinaryTree<Pair<uint8_t, int>>* temp = new BinaryTree<Pair<uint8_t, int>>(Pair<uint8_t,int>(0, trees[total-1]->data.second + trees[total-2]->data.second));
		temp->lChild = trees[total - 1];
		temp->rChild = trees[total - 2];
		--total;
		trees[total - 1] = temp;
	}
	//�������������л�
	//����ڵ�ĸ��ڵ�ͽ���ֵ
	//��Ϊ����ȫ������ ��256����Ҷ ��˽ڵ�����Ϊ511
	Pair<uint16_t,uint8_t> serializationTree[511];
	uint16_t now = 0;
	serializate(serializationTree, trees[0], UINT16_MAX,now);

	//�����ֵ�(.huftree.dat)
 	printf("�����ֵ���");
	char hufTreeData[512] = { 0 };
	strcat(hufTreeData, filePath);
	strcat(hufTreeData, R"(.huftree.dat)");
	FILE* data=fopen(hufTreeData,"wb");
	for (int i = 0; i < 511; ++i) {
		fwrite(&serializationTree[i].first, sizeof(serializationTree[i].first), 1, data);
		fwrite(&serializationTree[i].second, sizeof(serializationTree[i].second), 1, data);
	}
	fclose(data);

	//��������(.hufcode.txt)
	printf("��������\n");
	LinkedList<Pair<uint8_t, string>> list;
	treeToList(list, trees[0],"");
	list.sort([](const Pair<uint8_t, string>& a, const Pair<uint8_t, string>& b)->bool {return a.first < b.first; });
	char hufCode[512] = { 0 };
	strcat(hufCode, filePath);
	strcat(hufCode, R"(.hufcode.txt)");
	FILE* code = fopen(hufCode, "w");
	for (auto i = list.getHead(); i; i = i->getNext()) {
		fprintf(code, "%02X %s\n", i->data.first, i->data.second.c_str());
	}
	fclose(data);

	//����ת����
	Pair<uint8_t, string>* dirt[256];
	int j = 0;
	for (auto i = list.getHead(); i; i = i->getNext(),++j) {
		dirt[j]=&i->data;
	}
	
	//����ѹ���ļ�(.codefile.dat)
	printf("����ѹ���ļ�\n");
	char codeFile[512] = { 0 };
	strcat(codeFile, filePath);
	strcat(codeFile, R"(.codefile.dat)");
	auto write=BitWrite(codeFile);
	//�����ֵ�
	for (int i = 0; i < 511; ++i) {
		write.write(&serializationTree[i].first, sizeof(serializationTree[i].first), 1);
		write.write(&serializationTree[i].second, sizeof(serializationTree[i].second), 1);
	}
	//����Դ�ļ���С
	write.write(&fileSize, sizeof(uint32_t), 1);
	//����ѹ���ļ�
	for (int i = 0; i < fileSize; ++i) {
		string &s=dirt[fileContext[i]]->second;
		int len = s.length();
		for (int j = 0; j < len; ++j) {
			if (s[j] == '0') {
				write.writeBit(0);
			}
			else {
				write.writeBit(1);
			}
		}
	}
	write.close();
	
	//�����ڴ�
	delete trees[0];
	delete[] fileContext;
}
void treeToList(LinkedList<Pair<uint8_t, string>>& list, BinaryTree<Pair<uint8_t, int>>* tree, string s){
	if (tree->lChild) {
		treeToList(list, tree->lChild, s + "0");
		treeToList(list, tree->rChild, s + "1");
	}
	else {
		list.add(Pair<uint8_t,string>(tree->data.first, s));
	}
}
Pair<uint16_t, uint8_t>* serializate(Pair<uint16_t, uint8_t>* pair, BinaryTree<Pair<uint8_t, int>>* tree, uint16_t parient,uint16_t& now) {
	if (!tree)return pair;
	pair->first = parient;
	pair->second = tree->data.first;
	++pair;
	parient = now;
	++now;
	pair = serializate(pair, tree->lChild, parient,now);
	pair = serializate(pair, tree->rChild, parient,now);
	return pair;
}