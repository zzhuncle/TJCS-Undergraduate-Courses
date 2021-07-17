#include "Lex_main.h"

string folder = "./MyStoreFolder";
string LexOutputFilename0 = "0.LexSynCheckList.txt";
string LexOutputFilename1 = "1.LexResultIndex.txt";
string LexOutputFilename2 = "2.LexResultList.txt";
string LexOutputFilename3 = "3.LexResultWord.txt";
string LexOutputFilename4 = "4.SyntaxGrammar.txt";


string src;
string lex;
bool show_highlights = true;
LexAnalyzer analyzer;


void boundaryprint(const char* promt) {
	//仅仅是为了好看而打印的分割线，实质上无任何含义
	const char tmp[] = "-------------------------------------------------";
	const char left[] = "-----";
	cout << tmp << endl;
	cout << left;
	unsigned short a = (unsigned short)(strlen(tmp) - 2 * strlen(left)-strlen(promt))/2;
	unsigned short b = (unsigned short)(strlen(tmp) - 2 * strlen(left) -strlen(promt)- a);
	char blank[100]="";
	for (unsigned short i = 0;i < a;i++) {
		strcat(blank, " ");
	}
	cout << blank;
	cout << promt;
	if (a < b)
		strcat(blank, " ");
	cout << blank;

	cout << left << endl;
	cout << tmp << endl;

}

string read(ifstream& f) {
	stringstream ssr;
	ssr << f.rdbuf();
	return ssr.str();
}

bool cmp(LexSegment l, LexSegment r) {
	return l.left_index < r.left_index;
}

bool complex(const LexSegment& a, const LexSegment& b) {
	return a.left_index < b.right_index;
}


int Lex_main(string src) {

    bool isLexAnalyzePrint = false;

    ofstream output1((folder+'/'+LexOutputFilename1), ios::out);
    ofstream output2((folder+'/'+LexOutputFilename2), ios::out);
    ofstream output3((folder+'/'+LexOutputFilename3), ios::out);
	if (!output1.is_open()|| !output2.is_open()) {
		cout << "打开 " << LexOutputFilename1 << " 文件失败" << endl;
		return -1;
	}
    //boundaryprint("词法分析开始");
	analyzer.Analyze(src.c_str());
	vector<LexSegment> res;
	analyzer.GetResult(res);
	sort(res.begin(), res.end(), cmp);
	for (auto& seg : res)
		if (seg.component_type != VTWhiteSpace && seg.component_type != VTEndLine) {
			//上面是输出数字标号的，仅是为了下面处理语法分析来使用的
			output1 << seg.left_index << " " << seg.right_index << " " << (int)(seg.component_type) << endl;
            if(seg.component_type!=VT::VTComment&&seg.component_type!=VT::VTMacro)
                output2 << (int)(seg.component_type) << endl;
			//下面是输出分析结果的，是为了我们自己检查而展示的
            output3 << string(src.substr(seg.left_index, seg.right_index - seg.left_index)) << " " << VT_to_string(seg.component_type) << endl;
			if (isLexAnalyzePrint) {
                cout << string(src.substr(seg.left_index, seg.right_index - seg.left_index)) << " " << VT_to_string(seg.component_type) << endl;
			}
		}
    output2<<-1<<endl;
	output1.close();
	output2.close();
	output3.close();
    //boundaryprint("词法分析结束");
	
	//以上是词法分析的部分，输出的结果是一个文件夹和屏幕输出

    //boundaryprint("语法分析开始");

    ifstream lexinput((folder+'/'+LexOutputFilename1), ios::in);
	if (!lexinput.is_open()) {
		cerr << "打开 " << LexOutputFilename1 << " 文件失败" << endl;
		return -1;
	}
	lex = read(lexinput);
	lexinput.close();

	stringstream ssr(lex);
	vector<LexSegment> segs;
	LexSegment lex_seg(-1, -1, VT::VTNull);
	int comp;
	while (ssr >> lex_seg.left_index >> lex_seg.right_index >> comp) {
		lex_seg.component_type = (VT)comp;
		if (lex_seg.component_type != VT::VTWhiteSpace &&
			lex_seg.component_type != VT::VTEndLine)
			segs.push_back(lex_seg);
	}
	sort(segs.begin(), segs.end(), &complex);
	//这上面的模块已经读取结束

	SyntaxAnalyzer SynTax;
    //SynTax.PrintVectorVTs();
    //SynTax.PrintVectorVNs();
    //SynTax.PrintVectorPATS();
    ofstream myoutput((folder+'/'+LexOutputFilename4), ios::out);
	SynTax.LR1_Analyze_file(myoutput);

    ofstream output0((folder+'/'+LexOutputFilename0), ios::out);
    if (!output0.is_open()) {
        cout << "打开 " << LexOutputFilename0 << " 文件失败" << endl;
        return -1;
    }
    for(int i=0;i<=VT_MAX-VT_MIN;i++){
        output0<<VT_to_string(i)<<' '<<i+VT_MIN<<endl;
    }
    for(int i=1;i<VN_MAX-VN_MIN+1;i++){
        output0<<VN_to_string(i)<<" "<<i+VN_MIN<<endl;
    }
    output0.close();

	return 0;
}
