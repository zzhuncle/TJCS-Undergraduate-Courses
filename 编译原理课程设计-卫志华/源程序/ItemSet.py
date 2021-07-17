import re

# 最终项目族的DFA
class ItemSetSpecificationFamily():
    def __init__(self, cfg):
        self.TerminalSymbols = cfg.TerminalSymbols
        self.NonTerminalSymbols = cfg.NonTerminalSymbols
        self.StartSymbol = cfg.StartSymbol
        self.EndSymbol = cfg.EndSymbol
        self.Epsilon = cfg.Epsilon

        self.symbols = self.TerminalSymbols + self.NonTerminalSymbols
        self.itemPool = cfg.items  # itemPool：由产生式加点后的项目池
        self.itemSets = []         # 从pool中用GO函数划分
        self.edges = []            # 项目集之间的转移
        self.firstSet = cfg.firstSet
        return

    # 获取产生式，用于计算闭包
    def getLeftNT(self, NT):
        rst = []
        for item in self.itemPool:
            if item.left == NT and item.dotPos == 0:
                rst.append(item)
        return rst

    # 获取字符串的first
    def getFirstSet(self, symbols):
        rst = []
        hasEpsAllBefore = 0

        for s in symbols:
            tempSet = [i for i in self.firstSet[s]]
            if self.Epsilon in tempSet:
                if hasEpsAllBefore == 0:
                    hasEpsAllBefore = 1
                rst.extend([i for i in tempSet if i != self.Epsilon])
            else:
                hasEpsAllBefore = -1
                rst.extend(tempSet)
                break

        if hasEpsAllBefore == 1:
            rst.append(self.Epsilon)
        return rst

    def extendItem(self, item):
        rst = []
        if item.right[item.dotPos]['class'] != 'NT':
            return rst

        str2BgetFirstSet = []
        for rightIdx in range(item.dotPos + 1, len(item.right)):
            str2BgetFirstSet.append(item.right[rightIdx]['type'])
        nextItem = self.getLeftNT(item.right[item.dotPos]['type'])
        str2BgetFirstSet.append(item.terms[0])
        tempFirsts = self.getFirstSet(str2BgetFirstSet)
        for i in nextItem:
            for j in tempFirsts:
                rst.append(Item(i.left, i.right, 0, [j]))

        return rst

    # 计算闭包
    def getLR1Closure(self, I):
        rst = []
        rst.extend(I)

        # toString 已包含terms信息
        rstStr = [item.toString() for item in rst]  # 作为key值进行比对是否已有了
        while(True):
            isAddItem = 0
            for item in rst:
                right = item.right  # 引用 为了缩短变量长度

                for i in range(len(right) + 1):
                    if item.dotPos == len(right):
                        continue
                    if right[item.dotPos]['class'] == 'T':
                        continue
                    tempRst = self.extendItem(item)
                    for i in tempRst:
                        tempStr = i.toString()
                        if tempStr not in rstStr:
                            rstStr.append(tempStr)
                            rst.append(i)
                            isAddItem = 1
            if isAddItem == 0:
                break

        return rst

    # 状态转换函数GO GO(I，X) ＝ CLOSURE(J)
    def GO(self, I, X):
        J = []

        # 求GOTO函数的时候不要把ε当终结符/非终结符处理，不要引出ε边
        if len(I.items) == 0 or X == self.Epsilon:
            return J

        for item in I.items:
            if item.dotPos == len(item.right):
                continue
            # 空产生式
            if len(item.right) == 1 and item.right[0] == self.Epsilon:
                continue

            if item.right[item.dotPos]['type'] == X:
                temp = item.nextItem()
                if temp != None:
                    J.append(temp)
        return self.getLR1Closure(J)

    # 构造项目集规范族
    def buildFamily(self):
        iS = self.itemSets
        startI = []
        startI.append(self.itemPool[0])
        iS.append(ItemSet('s0', self.getLR1Closure([startI[0]] + self.extendItem(startI[0]))))

        setCnt = 1
        setStrings = {}
        setStrings['s0'] = iS[0].toString()
        edgeStrings = []

        while(True):
            isBigger = 0  # 是否有加新边或者状态
            for I in iS:
                for X in self.symbols:
                    rstGO = self.GO(I, X)
                    if len(rstGO) == 0:
                        continue
                    tempItemSet = ItemSet('s' + str(setCnt), rstGO)

                    if tempItemSet.toString() in setStrings.values():
                        tempItemSet.name = list(setStrings.keys())[list(setStrings.values()).index(tempItemSet.toString())]
                    else:
                        setStrings[tempItemSet.name] = tempItemSet.toString()
                        iS.append(tempItemSet)
                        isBigger = 1
                        setCnt = setCnt + 1

                    tempEdge = {'start' : I.name, 'symbol' : X, 'end' : tempItemSet.name}
                    tempEdgeStr = tempEdge['start'] + '->' + tempEdge['symbol'] + '->' + tempEdge['end']

                    if tempEdgeStr not in edgeStrings:
                        self.edges.append(tempEdge)
                        edgeStrings.append(tempEdgeStr)
                        isBigger = 1
            if isBigger == 0:
                break
        return

    def prtFamily(self):
        print(' ----------- 打印项目集族 --------------')
        for itemSet in self.itemSets:
            print(itemSet.name)
            for item in itemSet.items:
                rightList = [r['type'] for r in item.right]
                print(item.left, rightList, item.dotPos, item.terms)
        print('\n')
        for edge in self.edges:
            print(edge['start'], edge['symbol'], edge['end'])
        return

# 最终项目族的DFA的状态, 一个状态可能含有多个项目
class ItemSet():
    def __init__(self, name, items):
        self.name = name      # s0,s1...
        self.items = items    # 复数个项目 Production

        # 将所有item用string连起来, 方便比较
        self.string = []

        for item in self.items:
            itemStr = item.toString()
            if itemStr not in self.string:
                self.string.append(itemStr)

        self.string = sorted(self.string)
        return

    def toString(self):
        return "\n".join(self.string)

# 项目(带点产生式)
class Item():
    def __init__(self, left, right, dotPos=0, terms=['#']):  # terms的默认是[],不是None
        self.right = right  # Node的集合
        self.left = left
        self.dotPos = dotPos  # 用点所在的位置表示同一产生式的不同项目
        self.terms = terms  # LR(1)
        return

    def nextItem(self):
        return Item(self.left, self.right, self.dotPos + 1, self.terms) if self.dotPos <= len(self.right) else None

    def toString(self):
        rst = self.left + '->'
        pos = 0
        for right in self.right:
            if pos == self.dotPos:
                rst += '@' # 代替点
            rst += right['type'] + ' '
            pos += 1

        if pos == self.dotPos:
            rst += '@'

        for term in self.terms:
            rst += term + ' '
        return rst

# 穿线表节点
class CFG():
    def __init__(self):
        self.left = []
        self.prods = []        # 产生式
        self.items = []        # 元素的FIRST集
        self.startProd = None  # 可能需要转成广义G’，改写原来的起始符号
        self.firstSet = {}
        # 保留字
        self.reserved = {
            'if': 'IF',
            'else': 'ELSE',
            'while': 'WHILE',
            'int': 'INT',
            'return': 'RETURN',
            'void': 'VOID'
        }
        # 类别
        self.type = ['seperator', 'operator', 'identifier', 'int']
        # 词法分析所使用的正则表达式
        self.regexs = [
            '\{|\}|\[|\]|\(|\)|,|;'          # 界符
            , '\+|-|\*|/|==|!=|>=|<=|>|<|='  # 操作符
            , '[a-zA-Z][a-zA-Z0-9]*'         # 标识符
            , '\d+'                          # 整数
        ]

        self.TerminalSymbols = []
        self.NonTerminalSymbols = []
        self.StartSymbol = None
        self.OriginStartSymbol = None
        self.EndSymbol = None
        self.Epsilon = None
        return

    # 读取语法表达式文件
    def readGrammerFile(self, path):
        # 拓展成广义语法
        # 更改语法时需要修改此函数
        self.StartSymbol = 'program_'
        self.OriginStartSymbol = 'program'
        self.prods.append(Item(self.StartSymbol, [{'type': self.OriginStartSymbol, 'class': 'NT', 'name': ''}]))
        self.NonTerminalSymbols.append(self.StartSymbol)

        fd = open(path, 'r')
        self.cntProd = 0

        tokens = []
        while 1:
            line = fd.readline().replace('\n', '')
            if not line:
                break
            token1 = [] # 产生式左边
            token3 = [] # 产生式右边(合并左边相同的产生式，因此可能有很多右边)
            token1.append({'type': line, 'class': 'NT', 'name': line})
            while 1:
                token2 = [] # 一条产生式右边
                line = fd.readline().replace('\n', '')
                if not line:
                    break
                if (line[0] == '\t'):
                    line = line.strip('\t').split(' ')
                    if (line[0] == '#'):
                        tokens.append({'left': token1, 'right': token3})
                        break

                    self.cntProd = self.cntProd + 1
                    for item in line:
                        match = 0
                        for regex in self.regexs[0 : 2]:
                            result = re.match(regex, item)
                            if result:
                                match = 1
                                break

                        if (match == 1):              # 界符 操作符
                            tempToken2 = {'type': item, 'class': 'T', 'name': self.type[self.regexs.index(regex)].upper()}
                        elif (item in self.reserved): # 保留字
                            tempToken2 = {'type': item, 'class': 'T', 'name': item}
                        elif (item == 'id'):          # 标识符
                            tempToken2 = {'type': 'IDENTIFIER', 'class': 'T', 'name': 'IDENTIFIER'}
                        elif (item == '$'):           # 空
                            tempToken2 = {'type': item, 'class': 'T', 'name': item}
                        elif (item == 'num'):         # 整数
                            tempToken2 = {'type': 'INT', 'class': 'T', 'name': 'INT'}
                        else:                         # 变元
                            tempToken2 = {'type': item, 'class': 'NT', 'name': item}
                        token2.append(tempToken2)
                    token3.append(token2)

        for t in tokens:
            if t['left'][0]['type'] not in self.NonTerminalSymbols: # 添加变元
                self.NonTerminalSymbols.append(t['left'][0]['type'])

            for rightIdx in range(len(t['right'])):
                self.prods.append(Item(t['left'][0]['type'], t['right'][rightIdx]))
                for rightIdx2 in range(len(t['right'][rightIdx])): # 添加非变元
                    if t['right'][rightIdx][rightIdx2]['class'] == 'T' and t['right'][rightIdx][rightIdx2]['type'] not in self.TerminalSymbols:
                        self.TerminalSymbols.append(t['right'][rightIdx][rightIdx2]['type'])

        self.EndSymbol = '#'  # 终止符
        self.TerminalSymbols.append(self.EndSymbol)
        self.Epsilon = '$'
        return

    # 计算所有T和NT的first集
    def calFirstSet(self):
        for symbol in self.TerminalSymbols:  # 包括空串
            self.firstSet[symbol] = [symbol]
        for symbol in self.NonTerminalSymbols:
            self.firstSet[symbol] = []
        for symbol in self.NonTerminalSymbols:
            self.calNTFirstSet(symbol)
        return

    # 计算单个NT的first集
    def calNTFirstSet(self, symbol):
        eps = {'class': 'T', 'name': '', 'type': self.Epsilon}
        hasEpsAllBefore = -1
        prods = [prod for prod in self.prods if prod.left == symbol]
        if len(prods) == 0:
            return

        is_add = 1
        while (is_add):
            is_add = 0
            for prod in prods:
                hasEpsAllBefore = 0

                for right in prod.right:
                    # 2. 若X∈VN，且有产生式X->a…，a∈VT，则 a∈FIRST(X)
                    #    X->ε,则ε∈FIRST(X)
                    if right['class'] == 'T' or (right['type'] == self.Epsilon and len(prod.right) == 1):  # A->epsilon
                        # 有就加
                        if right['type'] not in self.firstSet[symbol]:
                            self.firstSet[symbol].append(right['type'])
                            is_add = 1
                        break

                    # 3. 对NT, 之前已算出来过, 但有可能是算到一半的
                    if len(self.firstSet[right['type']]) == 0:
                        if right['type'] != symbol:  # 防止陷入死循环
                            self.calNTFirstSet(right['type'])

                    # X->Y…是一个产生式且Y ∈VN  则把FIRST(Y)中的所有非空符号串ε元素都加入到FIRST(X)中。
                    if self.Epsilon in self.firstSet[right['type']]:
                        hasEpsAllBefore = 1

                    for f in self.firstSet[right['type']]:
                        if f != self.Epsilon and f not in self.firstSet[symbol]:
                            self.firstSet[symbol].append(f)
                            is_add = 1

                # 到这里说明整个产生式已遍历完毕 看是否有始终能推出eps
                # 中途不能退出eps的已经break了
                # 所有right(即Yi) 能够推导出ε,(i=1,2,…n)，则
                if hasEpsAllBefore == 1:
                    if self.Epsilon not in self.firstSet[symbol]:
                        self.firstSet[symbol].append(self.Epsilon)
                        is_add = 1

        return

    # 给产生式加点，转为项目item
    def getDotItems(self):
        for prod in self.prods:
            if len(prod.right) == 1 and prod.right[0]['type'] == self.Epsilon:
                self.items.append(Item(prod.left, prod.right, 0, ['#']))
                continue
            for i in range(len(prod.right) + 1):
                self.items.append(Item(prod.left, prod.right, i, ['#']))
        return

    # 输出测试
    def prtGrammer(self):
        print('------------ 打印语法详情 --------------')
        print("产生式个数", len(self.prods), self.cntProd)

        print('非终结符个数:', len(self.NonTerminalSymbols), '终结符个数:', len(self.TerminalSymbols))
        print(self.NonTerminalSymbols)
        print(self.TerminalSymbols)

        for item in self.items:
            rightList = [r['type'] for r in item.right]
            print(item.left, rightList, item.dotPos)  #
        return

    # 输出测试
    def prtFirstSet(self):
        print('---------- 打印First集 --------------')
        for key in self.firstSet.keys():
            prtList = [value for value in self.firstSet[key]]
            print(key, prtList)
        return
