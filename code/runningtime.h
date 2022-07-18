#pragma once
#define RUNNINGTIME 0
#include <iostream>
#include <string>
#include <vector>
#include <stack>

#ifndef SYMBOLTABLE
#include "symboltable.h"
#endif 

#ifndef LEXER
#include "lexer.h"
#endif 


#ifndef RUNNINGBASIC
#include "runningbasic.h"
#endif // !RUNNINGBASIC


class running_table_class {

public:

	running_table_class();

	
	//���һ���ֶε����ֱ���value����Ҫ��ֵ��ָ�룬�������͡����ֲ���
	void append_table(running::name_type type, std::string name, void* value);


	//��ȡ���ֱ��һ������
	running::name_table* get_items(std::string name);
	int get_items_location(std::string name);


	std::vector<running::name_table*>* get_table();  //���������ֱ�

	int running_levelup();    //running_level++��������if while�ȴ������������ֱ����������ֵΪ���λ��
	int running_leveldown();  //running_level--��������е���ر���ݹ����������ֵΪ���λ��
	void running_delete_from(int); //��intλ�õ�ȫ�����

	int in_function(std::string name);   //����ĺ�������
	//���뺯����ʱ��������иú�������ĺ�����running_table��ָ�룬����ָ��
	//����Ϊvector<name_table*>��Ȼ����ջ��ջ���999�����ݹ�999�㣩
	//����ֵΪ����ʱ���ݱ�ĺ�������λ�ã��Ա��˳����ʹ��
	void out_function(int location);
	//���������location���µ����н��������ݹ������Ȼ���ջһ��vector������
	//���еĽ���������ʱ�������
	
	void del_function(std::string name);  //ɾ��һ������


	bool in_import(std::string);  //import�������ƣ��ж��Ƿ�import�ɹ�
	//������������
	void out_import(std::string); //ȡ��import
	
	void show_table();   //չʾ�б����ڲ���ʹ��
	void save_table_at(std::string); //���б��浽�ļ�
	//�˹��ܴ���

private:
	int running_level;  //��ʾ��������ڲ㼶
	const int max_level = 9999;  //��ʾ�����㼶����   
	const int max_function_level = 999;
	std::vector<running::name_table*> running_table;
	std::stack<std::stack<running::name_table*> > save_table;

	std::vector<std::string> import_list;  //��ʾimport����Щ����
};


void running_table_class::show_table() {
	printf("\n\n");
	printf("name table: \n  tpye  name  level   value\n");

	for (int i = 0; i < (int)running_table.size(); i++) {
		running::name_table* temp = running_table[i];
		if (temp->exist == false) {
			continue;
		}

		std::vector<int>* nums = (std::vector<int>*)(temp->pointer);
		std::vector<double>* floats = (std::vector<double>*)(temp->pointer);
		std::vector<std::string>* strings = (std::vector<std::string>*)(temp->pointer);
		running::function_table* functions = (running::function_table*)(temp->pointer);

		switch (temp->ntype) {
		case running::name_type::INT:
			printf("  int  %s  %d  %d\n", temp->name.c_str(), temp->level,
				*(int*)(temp->pointer));
			break;
		case running::name_type::FLOAT:
			printf("  float  %s  %d  %f\n", temp->name.c_str(), temp->level,
				*(double*)(temp->pointer));
			break;
		case running::name_type::STRING:
			printf("  string  %s  %d  %s\n", temp->name.c_str(), temp->level, 
				((std::string*)(temp->pointer))->c_str());
			break;
		case running::name_type::ARRAY_INT:
			printf("  array int  %s  %d\n", temp->name.c_str(), temp->level);
			for (int i = 0; i < (int)nums->size(); i++) {
				printf("      %d", (*nums)[i]);
			}
			printf("\n");
			break;
		case running::name_type::ARRAY_FLOAT:
			printf("  array float  %s  %d\n", temp->name.c_str(), temp->level);
			for (int i = 0; i < (int)floats->size(); i++) {
				printf("      %f", (*floats)[i]);
			}
			printf("\n");
			break;
		case running::name_type::ARRAY_STRING:
			printf("  array int  %s  %d\n", temp->name.c_str(), temp->level);
			for (int i = 0; i < (int)strings->size(); i++) {
				printf("      %s", (*strings)[i].c_str() );
			}
			printf("\n");
			break;
		case running::name_type::FUNCTION:
			printf("  function  %s  %d\n", temp->name.c_str(), temp->level);
			running::function_table* fun = (running::function_table*)temp->pointer;
			if (fun->insert_function == true) {
				printf("    function type: insert function\n");
			}
			else {
				printf("     function type: user function");
				printf("     lexer file: %s\n", functions->lex_table->get_lexer_name());
				printf("     start position: %d\n", functions->start_position);
				printf("     parameter nums: %d\n", functions->parameter_num);
			}
			
			break;
		}
	}
	printf("\n\n");
}

int running_table_class::in_function(std::string name) {
	int loc = running_table_class::get_items_location(name);
	if (loc <= -1) {
		return -1;
	}
	if ((int)save_table.size() > max_function_level) {
		error::syntax_error(error::function_level_exceed);
		return -1;
	}
	std::stack<running::name_table*> temp_save;
	int items_num = 0;
	for(int i = running_table.size() - 1; i > loc; i--) {
		temp_save.push(running_table[i]);
		items_num++;
	}
	save_table.push(temp_save);
	for (int i = 0; i < items_num; i++) {
		running_table.pop_back();
	}
	return running_table.size();
}


void running_table_class::out_function(int location) {
	running_table_class::running_delete_from(location);
	std::stack<running::name_table*> temp_save = save_table.top();
	save_table.pop();
	while (temp_save.empty() != true) {
		running::name_table* temp = temp_save.top();
		temp_save.pop();
		running_table.push_back(temp);
	}
}



std::vector<running::name_table*>* running_table_class::get_table() {
	return &(running_table_class::running_table);
}

running_table_class::running_table_class() {
	running_level = 0;
}

bool running_table_class::in_import(std::string name) {
	int i;
	for (i = 0; i < (int)import_list.size(); i++) {
		if (import_list[i] == name) {
			return false;
		}
	}
	import_list.push_back(name);
	return true;
}

void running_table_class::out_import(std::string name) {
	std::vector<std::string>::iterator it;
	for (it=import_list.begin();it!=import_list.end();it++)	{
		if ((*it) == name) {
			import_list.erase(it);
			break;
		}
	}
}


int running_table_class::running_levelup() {
	running_table_class::running_level++;
	if (running_table_class::running_level >= running_table_class::max_level) {
		error::syntax_error(error::code_level_exceed);
		return -1;
	}
	return (int)running_table.size();
}


int running_table_class::running_leveldown() {
	int nowlevel = running_table_class::running_level;
	if (nowlevel > 0) {
		running_table_class::running_level--;
	}
	else {
		return -1;
	}
	int del_num = 0;
	for (int i = running_table.size() - 1; i >= 0; i--) {
		if (running_table[i]->level == nowlevel) {
			if (running_table[i]->exist == true) {
				//delete running_table[i]->pointer;
			}
			//delete running_table[i];
			del_num++;
		}
		else {
			break;
		}
	}
	for (int i = 0; i < del_num; i++) {
		running_table.pop_back();
	}
	return running_table.size();
}

void running_table_class::running_delete_from(int loc) {
	if (loc < 0) {
		return;
	}
	int del_num = 0;
	for (int i = running_table.size() - 1; i >= loc; i--) {
		if (running_table[i]->exist == true) {
			//delete running_table[i]->pointer;
	}
		//delete running_table[i];
		del_num++;
	}
	for (int i = 0; i < del_num; i++) {
		running_table.pop_back();
	}
}

void running_table_class::del_function(std::string name) { 
	for (int i = running_table.size() - 1; i >= 0; i--) {
		if (running_table[i]->name == name && running_table[i]->exist == true) {
			running_table[i]->exist = false;
			delete running_table[i]->pointer;			
		}
	}
}

running::name_table* running_table_class::get_items(std::string name) {
	for (int i = running_table.size() - 1; i >= 0; i--) {
		if (running_table[i]->name == name && running_table[i]->exist == true)
			return running_table[i];
	}
	return NULL;
}
	

int running_table_class::get_items_location(std::string name) {
	for (int i = running_table.size() - 1; i >= 0; i--) {
		if (running_table[i]->name == name && running_table[i]->exist == true)
			return i;
	}
	return -1;
}

void running_table_class::append_table(running::name_type type, std::string name, void* value) {
	running::name_table *tab = running_table_class::get_items(name);
	if (tab == NULL) {
		running::name_table* table = new running::name_table();
		table->level = running_table_class::running_level;
		table->ntype = type;
		table->name = name;
		table->exist = true;
		table->pointer = value;
		table->reftimes = 1;
		running_table_class::running_table.push_back(table);
	}
	else {
		tab->ntype = type;
		tab->pointer = value;
	}
}



