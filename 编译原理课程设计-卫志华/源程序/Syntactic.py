from Semantic import *

# 语法分析器
class SyntacticAnalyzer():
    # ACTION[s, a]：当状态s面临输入符号a时，应采取什么动作.
    # GOTO[s, X]：状态s面对文法符号X时，下一状态是什么
    def __init__(self, lex, cfg, family):
        self.lex = lex                # 词法分析结果
        self.cfg = cfg                # 产生式
        self.family = family          # 项目集族
        self.EndSymbol = cfg.EndSymbol
        self.OriginStartSymbol = cfg.OriginStartSymbol
        self.StartSymbol = cfg.StartSymbol
        self.TerminalSymbols = cfg.TerminalSymbols
        self.NonTerminalSymbols = cfg.NonTerminalSymbols

        self.itemSets = family.itemSets  # 状态集合
        self.edges = family.edges
        self.numSet = len(self.itemSets)

        ACTIONTitle = self.TerminalSymbols
        ACTIONTitle.append(self.EndSymbol)

        self.ACTION = {y.name : {x :  ' ' for x in ACTIONTitle} for y in self.itemSets}           # ACTION表
        self.GOTO = {y.name : {x :  ' ' for x in self.NonTerminalSymbols} for y in self.itemSets} # GOTO表

        self.prods = cfg.prods
        self.prodStrs = [i.toString() for i in self.prods]

        self.MTitle = self.TerminalSymbols + self.NonTerminalSymbols
        self.M = {y.name: {x: ' ' for x in self.MTitle} for y in self.itemSets}

        self.syntacticRst = True
        self.syntacticErrMsg = "语法分析成功！"
        self.semantic = SemanticAnalyser() # 语法分析的同时进行语义分析

        return

    # 给出一个产生式，返回这个产生式是原文法的第几个产生式
    def item2prodIdx(self, item):
        tempStr = item.left + '->@' # 代替点
        for right in item.right:
            tempStr += (right['type'] + ' ')
        tempStr += '# '
        return self.prodStrs.index(tempStr)

    # 输入字符串分析 S05 P92
    def getParseRst(self):
        return self.parseRst

    # 得到ACTION表和GOTO表
    def getTables(self):
        self.rst = []
        for e in self.edges:
            if e['symbol'] in self.TerminalSymbols:
                self.M[e['start']][e['symbol']] = 'shift ' + e['end']

            if e['symbol'] in self.NonTerminalSymbols:
                self.M[e['start']][e['symbol']] = 'goto ' + e['end']

        for I in self.itemSets:
            for item in I.items:
                if item.dotPos == len(item.right):
                    if item.left == self.OriginStartSymbol and item.terms[0] == '#':
                        if self.M[I.name][item.terms[0]] != ' ':
                            print('LR(1)分析表有多重入口！')
                        self.M[I.name][item.terms[0]] = 'acc'
                    else:
                        if self.M[I.name][item.terms[0]] != ' ':
                            print('LR(1)分析表有多重入口！')
                        self.M[I.name][item.terms[0]] = 'reduce ' + str(self.item2prodIdx(item))
                    continue

                if len(item.right) == 1 and item.right[0]['type'] == '$':
                    if item.left == self.OriginStartSymbol and item.terms[0] == '#':
                        if self.M[I.name][item.terms[0]] != ' ':
                            print('LR(1)分析表有多重入口！')
                        self.M[I.name][item.terms[0]] = 'acc'
                    else:
                        if self.M[I.name][item.terms[0]] != ' ':
                            print('LR(1)分析表有多重入口！')
                        self.M[I.name][item.terms[0]] = 'reduce ' + str(self.item2prodIdx(item))
                    continue
        return

    # 分析语句
    def isRecognizable(self, originCode):
        inputStr = []      # 输入串
        inputStr += self.lex.getTokensOfOneLine(originCode)
        sys.stdout.flush()
        stateStack = []    # 栈内状态序列
        shiftStr = []      # 移进规约串
        self.parseRst = [] # 记录步骤

        # 开始
        wallSymbol = {'class': 'T', 'type': '#'}
        shiftStr.append(wallSymbol)
        stateStack.append('s0')
        X = inputStr[0]    # X为当前单词
        while (True):
            if len(inputStr) <= 2:
                tmpInputStr = self.lex.getTokensOfOneLine(originCode)
                if len(tmpInputStr) == 0:
                    inputStr.append(wallSymbol)
                else:
                    inputStr += tmpInputStr

            self.parseRst.append({'stateStack': copy.deepcopy(stateStack),
                                  'shiftStr': copy.deepcopy(shiftStr),
                                  'inputStr': copy.deepcopy(inputStr)})

            act = self.M[stateStack[-1]][X['type']].split(' ')[0]
            target = self.M[stateStack[-1]][X['type']].split(' ')[1] if len(self.M[stateStack[-1]][X['type']].split(' ')) == 2 else None

            if act == 'shift':      # 移进操作
                stateStack.append(target)
                inputStr.pop(0)
                shiftStr.append(X)
                X = inputStr[0]

            elif act == 'goto':     # 转移操作
                stateStack.append(target)
                shiftStr.append(X)
                X = inputStr[0]

            elif act == 'reduce':   # 规约操作
                prodIdx = int(target)
                prod = self.prods[prodIdx]
                self.semantic.semanticAnalyze(prod, shiftStr)
                if False == self.semantic.semanticRst:
                    return False

                rightLen = len(prod.right)
                stateLen = len(stateStack)

                if rightLen == 1 and prod.right[0]['type'] == '$':
                    # 是空串, 有问题
                    dst = self.M[stateStack[-1]][prod.left].split(' ')[1]
                    stateStack.append(dst)
                    shiftStr.append({'class': 'NT', 'type': prod.left})
                    X = inputStr[0]

                else:# 不是空串
                    stateStack = stateStack[0 : stateLen - rightLen]
                    shiftStr = shiftStr[0 : stateLen - rightLen]
                    X = {'class': 'NT', 'type': prod.left}

            elif act == 'acc':      # 接受操作
                print('语法、语义分析成功!')
                self.semantic.semanticAnalyze(self.prods[1], shiftStr)
                return True

            else:
                print('语法、语义分析出错!')
                self.syntacticRst = False
                sys.stdout.flush()
                self.syntacticErrMsg = "语法分析错误：" + str(X['row']) + "行" + str(X['colum']) + "列"
                return False

    def prtMTables(self):
        print('----------- 打印 M 表 --------------')
        for i in self.M.keys():
            print(i, self.M[i])
        return

    def prtTables(self):
        print('-------------- 打印ACTION表 --------------')
        for i in self.ACTION.keys():
            print(i, self.ACTION[i])

        print('----------- 打印GOTO表 --------------')
        for i in self.GOTO.keys():
            print(i, self.GOTO[i])
        return

    def prtStep(self, stateStack, shiftStr, inputStr):
        print('----------- 打印规约过程 --------------')
        shiftList = []
        inputList = []
        for s in shiftStr:
            shiftList.append(s['type'])
        for s in inputStr:
            inputList.append(s['type'])
        print(stateStack, shiftList, inputList, '\n')
        return
