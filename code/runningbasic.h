#pragma once
#define RUNNINGBASIC 0



namespace running {
	enum name_type {
		INT,
		FLOAT,
		STRING,
		ARRAY_INT,
		ARRAY_FLOAT,
		ARRAY_STRING,
		FUNCTION,
		LIST
	};
	//ע�����е�array����vector����,list��vector<list_items>����



	struct function_table {
		lexer* lex_table;   //ִ�в�����
		int start_position; //��ʼ��λ��
		int parameter_num;  //��������
		bool non_def_num;   //�Ƿ��в�ȷ�������Ĳ���
		std::vector<std::string> *name_list;

		bool insert_function;
		void* function_pointer;
		//phrase::return_value(init_function::* f_max) (phrase::infunction_type* infunction) ;

	};

	struct name_table {
		name_type ntype;
		std::string name;
		int level;     //��ʾ���������Ĳ㼶
		void* pointer;  //ָ����ؽṹ���ָ��
		bool exist;     //��ʾ�Ƿ�ɾ��
		int reftimes;   //��ʾ���ô���
	};
}

