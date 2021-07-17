#include "SyntaxAnalyzer.h"
#include <iostream>
#include <map>
using namespace std;

const char* VN_names[]{
	"emptypro",
	"A",
	"P",
	"M",
	"N",
	"add_expression",
	"argument_list",
	"assign_sentence",
	"declare",
	"declare_list",
	"expression",
	"factor",
	"function_declare",
	"if_sentence",
	"inner_declare",
	"inner_var_declare",
	"item",
	"param",
	"parameter",
	"parameter_list",
	"return_sentence",
	"sentence",
	"sentence_block",
	"sentence_list",
	"var_declare",
	"while_sentence",
};


const char* PATTERNS[] = {
	"P,N,declare_list",
	"declare_list,declare,declare_list",
	"declare_list,declare",
	"declare,'int','Identifier',M,A,function_declare",
	"declare,'int','Identifier',var_declare",
    //"declare,'int','Identifier',array_declare",//数组声明
	"declare,'void','Identifier',M,A,function_declare",
	"A,emptypro",
	"var_declare,';'",
	"function_declare,'(',parameter,')',sentence_block",
    //"array_declare,'[',Integer,",
	"parameter,parameter_list",
	"parameter,'void'",
	"parameter_list,param",
	"parameter_list,param,',',parameter_list",
	"param,'int','Identifier'",
	"sentence_block,'{',inner_declare,sentence_list,'}'",
	"inner_declare,emptypro",
	"inner_declare,inner_var_declare,';',inner_declare",
	"inner_var_declare,'int','Identifier'",
	"sentence_list,sentence,M,sentence_list",
	"sentence_list,sentence",
	"sentence,if_sentence",
	"sentence,while_sentence",
	"sentence,return_sentence",
	"sentence,assign_sentence",
	"assign_sentence,'Identifier','=',expression,';'",
	"return_sentence,'return',';'",
	"return_sentence,'return',expression,';'",
	"while_sentence,'while',M,'(',expression,')',A,sentence_block",
	"if_sentence,'if','(',expression,')',A,sentence_block",
	"if_sentence,'if','(',expression,')',A,sentence_block,N,'else',M,A,sentence_block",
	"N,emptypro",
	"M,emptypro",
	"expression,add_expression",
	"expression,add_expression,'>',add_expression",
	"expression,add_expression,'>=',add_expression",
	"expression,add_expression,'<',add_expression",
	"expression,add_expression,'<=',add_expression",
	"expression,add_expression,'==',add_expression",
	"expression,add_expression,'!=',add_expression",
	"add_expression,item",
	"add_expression,item,'+',add_expression",
	"add_expression,item,'-',add_expression",
	"item,factor",
	"item,factor,'*',item",
	"item,factor,'/',item",
	"factor,'Integer'",
    "factor,'Floating'",
	"factor,'(',expression,')'",
	"factor,'Identifier','(',argument_list,')'",
	"factor,'Identifier'",
	"argument_list,emptypro",
	"argument_list,expression",
	"argument_list,expression,',',argument_list",
	"NULL"
};




string VN_to_string(int i) {
	if (i<0 || i>VN_MAX - VN_MIN)
		return "";
	else
		return VN_names[i];
}

int VNStr_to_int(const char* p) {
	for (int i = 0;i <= VN_MAX - VN_MIN;i++) {
		if (_strcmpi(p, VN_names[i]) == 0) {
			return i + VN_MIN;
		}
	}
	cerr << "无法将" << p << "转化为既存变元" << endl;
	return 0;//这里默认变元的范围是<0，所以返回0的话实际上是没找出来。
}

static inline void thr(const string& message) {
	cerr << message << endl;
	throw message;
}

void SyntaxAnalyzer::InitVTs() {
	for (int i = 0;i <= VT_MAX - VT_MIN;i++) {
		pair<int, string> tmp;
		tmp.first = VT_MIN + i;
		tmp.second = VT_to_string(i);
		VTs.push_back(tmp);
	}
}

void SyntaxAnalyzer::InitVNs() {
	for (int i = 0;i <= VN_MAX - VN_MIN;i++) {
		pair<int, string> tmp;
		tmp.first = VN_MIN + i;
		tmp.second = VN_to_string(i);
		VNs.push_back(tmp);
	}
}

void SyntaxAnalyzer::InitPATs() {
	for (int i = 0;strcmp(PATTERNS[i],"NULL")!=0;i++) {
		vector<int> tmp = InitPATs_SingleLine(PATTERNS[i]);//先不检查错误回头再看
		PATs.push_back(tmp);
	}
}

vector<int> SyntaxAnalyzer::InitPATs_SingleLine(const char* p) {

	//  "VN,{'while','(',VN_Expression,')',VN_SentenceBlock}"
	vector<int> tmp;
	char copy[500] = { 0 };
	char word[100] = { 0 };
	int i = 0, y = 0;
	//扫描第一遍，去除空格
	while (p[i]) {
		if (p[i] == ' ') {
			i++;
			continue;
		}
		else {
			copy[y] = p[i];
			y++, i++;
		}
	}

	i=y = 0;
	bool VTflag = false;
	while (p[i]) {
		y = 0;
		if (p[i] == ',')
			i++;
		if (p[i] == '\'') {
			VTflag = true;
			i++;	//跳过该引号
		}
		else
			VTflag = false;
		while (p[i]) {
			if (VTflag && p[i] != '\'') {
				word[y++] = p[i++];
			}
			else if (VTflag && p[i] == '\'') {
				break;
			}
			else if (!VTflag && p[i] != ',') {
				word[y++] = p[i++];
			}
			else
				break;
			
		}
		word[y] = 0;
		if (VTflag) {
			i = i + 1;//跳过后面的引号
			tmp.push_back(VTStr_to_int(word));
		}
		else {
			tmp.push_back(VNStr_to_int(word));
		}
	}
	return tmp;
}

SyntaxAnalyzer::SyntaxAnalyzer() {
	//这个函数应该在一生成就将语法实例化

	InitVTs();
	InitVNs();
	InitPATs();

}

SyntaxAnalyzer::~SyntaxAnalyzer() {
	VectorClear();
}

void SyntaxAnalyzer::VectorClear()
{
	VTs.clear();
	VNs.clear();
	PATs.clear();
}

void SyntaxAnalyzer::PrintVectorVTs() {
	cout << "Here are contents of VTs\n";
	for (auto it = VTs.begin();it != VTs.end();it++) {
		cout << "< " << it->first << " , " << it->second << " >" << endl;
	}
}

void SyntaxAnalyzer::PrintVectorVNs()
{
	cout << "Here are contents of VNs\n";
	for (auto it = VNs.begin();it != VNs.end();it++) {
		cout << "< " << it->first << " , " << it->second << " >" << endl;
	}
}

void SyntaxAnalyzer::PrintVectorPATS(){
	cout << "Here are contents of PATs\n";
	for (auto it = PATs.begin();it != PATs.end();it++) {
		for (auto it2 = it->begin();it2 != it->end();it2++) {
			if (it2 == it->begin())
				cout << *it2 << " --> [ ";
			else if (it2 + 1 == it->end()) {
				cout << *it2 << " ";
			}
			else
				cout << *it2 << " , ";

		}
		cout << "]\n";
	}
}

void SyntaxAnalyzer::LR1_Analyze_file(ofstream& out){

	//第一行写出终结符 ，变元 ，产生式
	out << VT_MAX - VT_MIN + 1 << " " << VN_MAX - VN_MIN << " " << PATs.size()<<endl;

	//第二行输出终结符的
	for (auto it = VTs.begin();it != VTs.end();it++) {
		out << it->first << " ";
	}
	out << endl;

	//第三行输出非终结符的
	for (auto it = VNs.begin();it != VNs.end();it++) {
		if(it->first!= emptypro)
			out << it->first << " ";
	}
	out << endl;

	for (auto it1 = PATs.begin();it1 != PATs.end();it1++) {
		out << it1->size() << " ";
		for (auto it2 = it1->begin();it2 != it1->end();it2++) {
			out << *it2 << " ";
		}
		out << endl;
	}

}

