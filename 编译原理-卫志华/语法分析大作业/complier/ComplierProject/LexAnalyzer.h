#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

/* 组成元素枚举 */


/*
int void if while else return + - * / = == > < != >= <= ; , ( ) { } ID NUM
*/

//为了扩展性，将词法分析的表格写的很多，但实际分析的时候没有用到这么多
enum VT {
    //0 ~ 9
    VTNull,
    VTInteger,
    VTString,
    VTBoolean,
    VTFloating,
    VTCharacter,
    VTComment,
    VTMacro,
    VTWhiteSpace,
    VTEndLine,
    //界符 括号 10 ~ 15
    VTLeftBrace,
    VTRightBrace,
    VTLeftSquareBracket,
    VTRightSquareBracket,
    VTLeftBracket,
    VTRightBracket,
    //运算符 16 ~ 50
    VTPlus,
    VTMinus,
    VTMultiply,
    VTDivide,
    VTModulus,
    VTIncrement,
    VTDecrement,
    VTEqual,
    VTNotEqual,
    VTGreater,
    VTLess,
    VTGE,
    VTLE,
    VTLogicalAnd,
    VTLogicalOr,
    VTLogicalNot,
    VTBinaryAnd,
    VTBinaryOr,
    VTBinaryNot,
    VTXor,
    VTLeftShift,
    VTRightShift,
    VTAssignment,
    VTAddAssignment,
    VTSubtractAssignment,
    VTMultiplyAssignment,
    VTDivideAssignment,
    VTModulusAssignment,
    VTLeftShiftAssignment,
    VTRightShiftAssignment,
    VTBinaryAndAssignment,
    VTBinaryOrAssignment,
    VTBinaryXorAssignment,
    VTComma,
    VTSemicolon,
    //关键字 51 ~ 82
    VTAuto,
    VTBreak,
    VTCase,
    VTChar,
    VTConst,
    VTContinue,
    VTDefault,
    VTDo,
    VTDouble,
    VTElse,
    VTEnum,
    VTExtern,
    VTFloat,
    VTFor,
    VTGoto,
    VTIf,
    VTInt,
    VTLong,
    VTRegister,
    VTReturn,
    VTShort,
    VTSigned,
    VTSizeof,
    VTStatic,
    VTStruct,
    VTSwitch,
    VTTypedef,
    VTUnion,
    VTUnsigned,
    VTVoid,
    VTVolatile,
    VTWhile,
    //words 标识符 82
    VTIdentifier,
    VTWord
};
//这两个值分别记录词汇表的开始和结尾，用于寻找某一个终结符的位置
//此后可以用map来实现上述的结构。就不必要来回的折腾
const int VT_MIN = (int)VTNull;
const int VT_MAX = (int)VTWord;

//这三个函数分别是将一个数字转化为字符串string的函数，用于转换的时候利用
string VT_to_string(VT component);
string VT_to_string(int i);
int VTStr_to_int(const char* p);


//这是语法分析的时候用于判断分类点的函数，具体的实现有一点瑕疵
//直接将位置确定。不利于扩展
bool is_keyword(VT component);
bool is_operator(VT component);
bool is_bracket(VT component);
bool is_value(VT component);

/* 分割组成类 */
class LexSegment {
public:
    int left_index = -1;
    int right_index = -1;
    //这两个位置是相对于文件开头的部分，利用left_index和right_index可以定位到这个原字符串
    VT component_type = VT::VTNull;

public:
    LexSegment();
    LexSegment(int li, int ri, VT ctype) :
        left_index(li), right_index(ri), component_type(ctype) {};
};

/* 字典树结果结构体 */
struct _Res {
    VT val;
    int begin;
    int end;
};

// 字典树类
//方便查找关键字和运算符
class Trie {
private:
    bool isLeaf;
    Trie* next[128];
    VT type;
public:
    Trie() {
        isLeaf = false;
        memset(next, NULL, sizeof(next));
    }

    void insert(const string& word, const VT& t) {
        Trie* node = this;
        for (char c : word) {
            if (node->next[c - '\0'] == NULL)
                node->next[c - '\0'] = new Trie();
            node = node->next[c - '\0'];
        }
        node->isLeaf = true;
        node->type = t;
    }

    vector<_Res> match(const string& word) {
        Trie* node = this,* parent = this;
        int len = word.length();
        vector<_Res> res;
        for (int start = 0, i = 0; i < len;) {
            node = this;
            for (i = start; i < len;i++) {
                const char& c = word[i];
                parent = node;
                node = node->next[c - '\0'];
                if (node == NULL) {
                    if (parent->isLeaf) {//防止过早匹配
                        if ((parent->type == VTPlus || parent->type == VTMinus)
                            && (i >= 2 && (word[i - 2] == 'e' || word[i - 2] == 'E'))) {//防止匹配到浮点数中的-、+
                            start = start + 1;
                            break;
                        }
                        res.emplace_back(_Res{ parent->type, start, i});
                        start = i;
                    }
                    else
                        start = start + 1;
                    break;
                }
            }
        }
        return res;
    }
};


/* 词法分析类 */
class LexAnalyzer {
private:
    char* original_code = NULL;         //原始字符串
    int code_length = -1;               //字符串长度
    vector<LexSegment> inner_result;    //词法分析结果
    vector<VT> component_type;//
    Trie trie;                          //方便查找
private:
    void init();
    void preAnalyze();
    void analyzeSpliters();
    void analyzeKeywordsAndOperators();
    void analyzeWords();
    void setComponent(int start_index, int end_index, VT component);
public:
    LexAnalyzer();
    void Analyze(const char* code);
    void Analyze(const string& code);
    void GetResult(vector<LexSegment>& result);
};

#endif
