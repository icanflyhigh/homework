#pragma once
// author: M@
// #define _DEBUG_

#include<iostream>
#include<cstdlib>
#include<fstream>
#include<istream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<string.h>
#include<queue>
#include<vector>
#include<map>
#include<stack>
#include<list>
#include<set>
#include<string>
#include <utility>
#include "Type.h"
#include "AST_node.h"
#include "symbol.h"

using namespace std;
typedef pair<string, node_type> s2n;
/*
语义分析器
TODO: 1.建立语法分析之后的AST 2.分类optimizer和sematicparser
*/
const set<string> qualifier = { "const", "static" };
const vector<string> atom_op = { "print", "+", "-", "*", "/", ":=" };
const vector<string> property = { "val", "type" };
const map<string, node_type>  node_type_map = {
	{{"nop"}, node_nop},
	{{"assign"}, node_assign},
	{{"add"}, node_add},
	{{"sub"}, node_sub},
	{{"mul"}, node_mul},
	{{"div"}, node_div},
	{{"if"}, node_if},
	{{"while"}, node_while},
	{{"func"}, node_func},
	{{"greater"}, node_greater},
	{{"declare"}, node_declare,},
	{{"call"}, node_call},
	{{"return"}, node_return},
	{{"and"}, node_and},
};




class sematic_parser
{
public:
	sematic_parser(string syntax_path, string code_path);

	//struct operand {
	//	int v, property; //v指向产生式的第几个V -1 表示左式
	//	operand(int a = 0, int b = 0) : v(a), property(b) {
	//	}
	//};

	struct form
	{
		int left;// 产生式左部
		vector<int> right;// 产生式右部
		vector<node_type> ops;// node操作的型号
		vector<int> var;// 通过位置指示对应的元素在表达式的什么位置, -1 表示左式，自然数表示右式
		form(int a, vector<int> b) :left(a), right(b) {}
	};

	struct item
	{
		int f; // 产生式
		int p = 0;// 产生式的状态
		int fs; // 前向搜索符
		item(int form_idx, int pos, int fs_idx) :f(form_idx), p(pos), fs(fs_idx)
		{}
		bool operator==(const item & a)const
		{
			return f == a.f && p == a.p && fs == a.fs;
		}
		bool operator<(const item & a)const
		{
			if (f != a.f)
			{
				return f < a.f;
			}
			else if (p != a.p)
			{
				return p < a.p;
			}
			return fs < a.fs;
		}
	};


	// 参数区
	ifstream f1;
	ifstream f2;
	vector<string> V_list;// 记录V的列表
	vector<bool> is_VT;// 是否为终结符
	set<int> to_epsilon;// 会产生epsilon的集合
	vector<form> form_list; // 产生式列表
	map<int, vector<int>> form_map;// 每一个左部为idx的form
	string start_s = "<Start>";// 开始符号
	string end_s = "endC"; // 结束符号
	vector<vector<item>> item_clan; // 项目集族
	vector<map<int, int>> go; // 转移 第一个int表示符号，第二个表示转移到的状态
	vector<map<int, int>> reverse;// 规约，第一个int表示符号，第二个表示规约的状态数
	map<int, set<int>> first_set; // first集

	vector<int> s_stack; // 状态栈
	vector<int> V_stack; // 符号栈
	vector<AST_node> a_stack;

	//___________new______________
	symtab symbolTable;
	AST_tree ASTTree;




	void output();

	void test();


	static node_type check_node_type(const string &);



	// 搜索s是否在V_LIST中，若不在则加入，返回s在V_LSIT中的编号
	int V_idx(const string & s, bool is_push = true);

	// 寻找产生空的Vn
	void find_epsilon();

	// p 指向字符串读取数字的第一个位置
	inline int read_int(const string & s, int & p);

	inline int chk_dot(const string & s);


	// p 指向字符串读取数字的第一个位置
	inline double read_double(const string & s, int p);

	// 读取语法
	void read_syntax();

	set<int> union_first(const set<int> & a, const set<int> & b);

	// 存在迭代，高危代码段
	set<int> form_first_set(int idx, set<int> & vis);

	// 求FIRST
	void gen_first();

	// 求项目集
	int closure(vector<item> & vi, int fa);

	// action——goto表与项目集族一起产生
	void generate_clan();

	// 读取常量
	symbol read_COSNT(const string & s);
	
	symbol read_VARIABLE(const string & s);
	

	void parse_begin();

	// id_stack留着在原来函数里面pop
	// 这里只处理表达式
	AST_node deal_expression(int id_num, int form_idx);

	// TODO处理parse，1.产生参数表 2.语法树 3.特殊处理某些输入
	// 分析序列 返回错误类型，或者接受 -1程序错误 0接受 1不识别的Vt 2拒绝 3处理中 4标识符类型错误
	int parse_Vt(string  s, string Vt, string type);

	int str2Num(const string  s);

	// TODO给出错误的原因
	int parse_code();// 读取TASK1的输出

	int AST2TAC();// 将抽象语法树转化为四元式

	int dfs_AST(int idx); // 遍历AST
		
};
