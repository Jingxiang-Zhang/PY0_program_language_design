#pragma once


#define PHRASEBASIC 0



namespace phrase {
	enum block_type {
		loop,
		function,
		if_block
	};

	struct block {
		lexer* lex;
		block_type type;
		int block_start_location;
		int running_table_location;
	};

	struct return_value {
		bool hasvalue = true;
		running::name_type type;  //����з���ֵ�����ҵ�����ֵ����
		void* value;  //��ʾ����ֵ��ָ��
		std::string name;

		bool error = false;
		std::string error_type;
	};

	enum bracket_type {
		bracket,
		para
	};

	struct infunction_type {
		std::vector<running::name_type>* type ;
		std::vector<void*>* parameter ;
	};

}
