#ifndef LEX_MAIN_H
#define LEX_MAIN_H

#define _CRT_SECURE_NO_WARNINGS
#include "LexAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include <algorithm>

int Lex_main(string src);

string read(ifstream& f);

bool cmp(LexSegment l, LexSegment r);

bool complex(const LexSegment& a, const LexSegment& b);


#endif // LEX_MAIN_H
