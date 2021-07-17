#pragma once
#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "LexAnalyzer.h"
#include <unordered_map>
#include <vector>

/* 组成元素枚举 */
enum VN {
	emptypro = -1000,
	A,
	M,
	N,
	P,
	add_expression,
	argument_list,
	assign_sentence,
	declare,
	declare_list,
	expression,
	factor,
	function_declare,
	if_sentence,
	inner_declare,
	inner_var_declare,
	item,
	param,
	parameter,
	parameter_list,
	return_sentence,
	sentence,
	sentence_block,
	sentence_list,
	var_declare,
	while_sentence
};

const int VN_MIN = (int)emptypro;
const int VN_MAX = (int)while_sentence;



string VN_to_string(int i);  //将下标号i的具体名称给取出来
int VNStr_to_int(const char* p);	//例如将return所对应的标号给找出来

class SyntaxAnalyzer {
private:
	vector<pair<int,string>> VTs;	//记录终结符的向量，其中元素为pair<int,string>,
									//第1个元素为该终结符的标号，第2个元素为该终结符的实际字符，例如，pair<52,"break">
	vector<pair<int, string>> VNs;	//记录中间变元的向量，其中元素为pair<int,string>,
									//first:该变元的标号，second:该变元的实际字符，例如，pair<-200,"VN_Program">
	vector<vector<int>> PATs;				//记录生成规则的向量,其中元素为int,
									//first:该变元的标号，second:该变元推出的表达式，例如，[ -200,-100,45,12],代表 -200 -> -100,45,12
	void InitVTs();
	void InitVNs();
	void InitPATs();
	vector<int> InitPATs_SingleLine(const char * p);	//分析某一行内的数据信息，可能要增加多个生成式



public:
	SyntaxAnalyzer();
	~SyntaxAnalyzer();
	void VectorClear();
	void PrintVectorVTs();
	void PrintVectorVNs();
	void PrintVectorPATS();
	void LR1_Analyze_file(ofstream& out);

};

#endif

