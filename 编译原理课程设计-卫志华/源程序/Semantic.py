import copy
import sys

# 语句块
class Node():
    def __init__(self):
        self.place = None # 语句块入口的中间变量
        self.code = []    # 传递而来的或者生成的*中间代码*
        self.stack = []   # 翻译闭包表达式所用的临时栈

        self.name = None      # 语句块的标识符
        self.arrname = None   # 数组的名字
        self.type = None      # 结点的数据类型
        self.data = None      # 结点携带的数据
        self.dims = []        # 数组结点的维度
        self.position = []    # 符号元素在数组的位置

        self.begin = None # 循环入口
        self.end = None   # 循环出口
        self.true = None  # 为真时的跳转位置
        self.false = None # 为假时的跳转位置

    def prtNode(self):
        print('Node name:', self.name, ',type:', self.type, ',data:', self.data, ',code:',self.code)
        return

# 符号表
class Symbol:
    def __init__(self):
        self.name = None     # 符号的标识符
        self.type = None     # 类型

        self.size = None     # 占用字节数
        self.offset = None   # 内存偏移量

        self.place = None    # 对应的中间变量
        self.function = None # 所在函数

        self.dims = []       # 数组的维度

# 函数表
class FunctionSymbol:
    def __init__(self):
        self.name = None     # 函数的标识符
        self.type = None     # 返回值类型
        self.label = None    # 入口处的标签
        self.params = []     # 形参列表

# 语义分析类
class SemanticAnalyser():
    def __init__(self):
        self.sStack = []           # 语义分析栈
        self.symbolTable = []      # 符号表    [s { name, function } ]
        self.funcTable = []        # 函数表

        self.curTempId = 0         # 中间变量名序号
        self.curLabelId = 0        # 当前 label 入口序号
        self.curFuncId = 0         # 当前 function 序号
        self.curOffset = 0         # 当前偏移量
        self.curFuncSymbol = None  # 当前函数

        # 把全局当作一个函数
        f = FunctionSymbol()
        f.name = 'global'
        f.label = 'global'
        self.updateFuncTable(f)
        self.curFuncSymbol = f
        self.middleCode = []

        self.semanticRst = True
        self.semanticErrMsg = "语义分析成功！"
        return

    # prod: 规约时运用的产生式, shiftStr: 移进规约串
    def semanticAnalyze(self, prod, shiftStr):
        nt = prod.left
        r = prod.right

        # 主要分为说明、赋值、数组、布尔表达式、条件控制
        # 注意实现数组的扁平化

        sys.stdout.flush()

        # arrayDeclaration -> [ num ] | [ num ] arrayDeclaration
        if nt == 'arrayDeclaration':
            if len(r) == 3:
                n = Node()
                n.name = nt
                n.type = 'int array'
                n.dims = [ int(shiftStr[-2]['data']) ]      # 数组维度
                self.sStack.append(n)
            elif len(r) == 4:
                n = self.sStack.pop(-1)
                n.name = nt
                n.type = 'int array'
                n.dims.insert(0, int(shiftStr[-3]['data']) ) # 数组维度
                self.sStack.append(n)

        # array -> id [ expression ] | array [ expression ]
        if nt == 'array':
            expression_n = self.sStack.pop(-1)
            self.calExpression(expression_n)         # 计算表达式的值
            array_n = copy.deepcopy(self.sStack[-1]) # 深拷贝

            if shiftStr[-4]['type'] == 'array': # -> array [ expression ]
                self.sStack.pop(-1)
                expression_n.name = 'array'
                expression_n.arrname = array_n.arrname # 数组原始变量名字
                expression_n.position = copy.deepcopy(array_n.position)
                expression_n.position.append(expression_n.place if expression_n.place else expression_n.data)
                expression_n.place = array_n.place    # 数组对应的中间变量名字
                expression_n.type = 'array'
                self.sStack.append(expression_n)

            else: # -> id [ expression ]
                expression_n.name = 'array'
                expression_n.arrname = shiftStr[-4]['data']
                s = shiftStr[-4]['data']  # 拿到变量名称
                nTmp = self.findSymbol(s, self.curFuncSymbol.label) # python 可变对象传引用
                if nTmp == None:
                    print('使用未定义的数组变量!')
                    self.semanticRst = False
                    self.semanticErrMsg = "未定义的数组变量：" + shiftStr[-4]['data'] + "，在" + str(shiftStr[-4]['row']) + "行" + str(shiftStr[-4]['colum']) + "列"
                    return
                expression_n.position.append(expression_n.place if expression_n.place else expression_n.data)
                expression_n.place = s            # 数组对应的中间变量
                expression_n.type = 'array'
                self.sStack.append(expression_n)

        # program -> declarationChain
        if nt == 'program':
            n = self.sStack.pop(-1)
            n.name = nt

            for node in n.stack:
                for code in node.code:
                    n.code.append(code)

            self.middleCode = copy.deepcopy(n.code)
            self.sStack.append(n)

        # block -> { statementChain }
        # statement -> declaration | ifStatement | iterStatement | returnStatement | assignStatement
        elif nt in ['statement', 'block']:
            n = self.sStack.pop(-1)
            n.name = nt
            self.sStack.append(n)

        # declarationChain -> $ | declaration declarationChain
        elif nt == 'declarationChain':
            n = Node()
            if len(r) == 2:  # declaration declarationChain
                n = self.sStack.pop(-1)
                n.stack.insert(0, self.sStack.pop(-1))
            n.name = nt
            self.sStack.append(n)

        # typeSpecifier -> int | void
        elif nt == 'typeSpecifier':
            n = Node()
            n.name = nt
            n.type = shiftStr[-1]['type']  # 拿到类型名
            self.sStack.append(n)

        # declaration -> typeSpecifier id ; | completeFunction | typeSpecifier id arrayDeclaration ;
        elif nt == 'declaration':  # variable or function
            if len(r) == 3:  # -> typeSpecifier id ;
                n = self.sStack.pop(-1)
                n.name = nt
                defType = n.type
                defName = shiftStr[-2]['data']  # 变量名
                s = self.findSymbol(defName, self.curFuncSymbol.label)
                if s != None:
                    print("变量重定义！")
                    self.semanticRst = False
                    self.semanticErrMsg = "变量重定义: " + str(shiftStr[-2]['row']) + "行" + str(shiftStr[-2]['colum']) + "列"
                    return
                else:
                    s = Symbol()

                if n.place == None:
                    s.name = defName
                    s.place = self.getNewTemp()
                    s.type = defType
                    s.function = self.curFuncSymbol.label
                    s.size = 4
                    s.offset = self.curOffset
                    self.curOffset += s.size
                    self.updateSymbolTable(s)

                    if n.data != None:  # 不是常数
                        code = (':=', n.data, '_', s.place)
                        n.code.append(code)
                else:
                    s.name = defName
                    s.place = n.place
                    s.type = defType
                    s.function = self.curFuncId
                    s.size = 4
                    s.offset = self.curOffset
                    self.curOffset += s.size
                    self.updateSymbolTable(s)
                    for code in n.code:
                        n.code.stack.insert(0, code)

            if len(r) == 4:  # -> typeSpecifier id arrayDeclaration ;
                array_n = self.sStack.pop(-1)
                n = self.sStack.pop(-1)
                n.name = nt
                defType = array_n.type
                defName = shiftStr[-3]['data']  # 数组变量名
                s = self.findSymbol(defName, self.curFuncSymbol.label)
                if s != None:
                    print("数组变量重定义！")
                    self.semanticRst = False
                    self.semanticErrMsg = "数组变量重定义: " + str(shiftStr[-2]['row']) + "行" + str(shiftStr[-2]['colum']) + "列"
                    return
                else:
                    s = Symbol()

                if n.place == None:# 没有分配中间变量
                    s.name = defName
                    s.place = self.getNewTemp()
                    s.type = defType
                    s.function = self.curFuncSymbol.label
                    s.size = 4
                    for ndim in array_n.dims:
                        s.size *= ndim
                    s.dims = copy.deepcopy(array_n.dims)
                    s.offset = self.curOffset
                    self.curOffset += s.size
                    self.updateSymbolTable(s)

                    if n.data != None:  # 不是常数
                        code = (':=', n.data, '_', s.place)
                        n.code.append(code)

                else:
                    s.name = defName
                    s.place = n.place
                    s.type = defType
                    s.function = self.curFuncId
                    s.size = 4
                    for ndim in array_n.dims:
                        s.size *= ndim
                    s.dims = copy.deepcopy(array_n.dims)
                    s.offset = self.curOffset
                    self.curOffset += s.size
                    self.updateSymbolTable(s)

                    for code in n.code:
                        n.code.stack.insert(0, code)

            if len(r) == 1:  # -> completeFunction
                n = self.sStack.pop(-1)
                n.name = nt

            self.sStack.append(n)

        # completeFunction -> declareFunction block
        elif nt == 'completeFunction':
            n = self.sStack.pop(-1)  # block
            nDefine = self.sStack.pop(-1)  # declareFunction

            n.name = nt
            codeTmp = []
            codeTmp.append((nDefine.data, ':', '_', '_'))  # 函数名

            for node in nDefine.stack:  # para
                codeTmp.append(('pop', '_', 4 * nDefine.stack.index(node), node.place))

            if len(nDefine.stack) > 0:
                codeTmp.append(('-', 'fp', 4 * len(nDefine.stack), 'fp'))

            for code in reversed(codeTmp):
                n.code.insert(0, code)

            code_end = n.code[-1]
            if code_end[0][0] == 'l':  # 非main函数
                label = code_end[0]
                n.code.remove(code_end)
                for code in n.code:
                    if code[3] == label:
                        n.code.remove(code)

            self.sStack.append(n)

        # declareFunction -> typeSpecifier id ( formalParaList )
        elif nt == 'declareFunction':
            n = self.sStack.pop(-1)  # formalParaList
            n.name = nt
            nFuncReturnType = self.sStack.pop(-1)  # typeSpecifier
            f = FunctionSymbol()  # 登记函数
            f.name = shiftStr[-4]['data']
            f.type = nFuncReturnType.type
            if f.name == 'main':
                f.label = 'main'
            else:
                f.label = self.getNewFuncLabel()

            # 搜索formalParaList表，记录参数列表
            for arg in n.stack:
                s = Symbol()
                s.name = arg.data    # 处理para时，变量名放data
                s.place = arg.place  # 此时是None

                s.type = arg.type
                s.function = f.label
                s.size = 4
                s.offset = self.curOffset
                self.curOffset += s.size
                self.updateSymbolTable(s)

                f.params.append((arg.data, arg.type, arg.place))

            n.data = f.label
            self.updateFuncTable(f)
            self.stack = []  # 清空
            self.curFuncSymbol = f
            self.sStack.append(n)

        # formalParaList -> $ | para | para, formalParaList | void
        elif nt == 'formalParaList':
            n = Node()
            if len(r) == 3:  # formalParaList -> para , formalParaList
                n = self.sStack.pop(-1)
                n.name = nt
                n.stack.insert(0, self.sStack.pop(-1))

            elif len(r) == 1 and (r[0]['type'] in ['$', 'void']):  # $ | void
                n.name = nt

            elif len(r) == 1 and r[0]['type'] == 'para':  # para
                n.stack.insert(0, self.sStack.pop(-1))
                n.name = nt

            self.sStack.append(n)

        # para -> typeSpecifier id
        elif nt == 'para':
            n = self.sStack.pop(-1)  # typeSpecifier node
            n.name = nt
            n.place = self.getNewTemp()    # 形参不在符号表里登记
            n.data = shiftStr[-1]['data']  # id在para node的data里
            self.sStack.append(n)

        # statementChain -> $ | statement statementChain
        elif nt == 'statementChain':
            if len(r) == 1:  # $
                n = Node()
                n.name = nt
                self.sStack.append(n)
            elif len(r) == 2:  # statement statementChain
                n = self.sStack.pop(-1)
                n.stack.insert(0, self.sStack.pop(-1))
                n.name = nt

                # statement.code，statementChain.code是顺序的, 但前者要在后者前面
                for code in reversed(n.stack[0].code):
                    n.code.insert(0, code)

                self.sStack.append(n)

        # assignStatement -> id = expression ; | array = expression ;
        elif nt == 'assignStatement':
            if shiftStr[-4]['type'] != 'array': # id = expression ;
                id = shiftStr[-4]['data']       # 取id的名字
                n = copy.deepcopy(self.sStack.pop(-1))  # expression
                n.name = nt
                self.calExpression(n)

                s = self.findSymbol(id, self.curFuncSymbol.label)
                if s == None:
                    print("使用未定义变量！")
                    self.semanticRst = False
                    self.semanticErrMsg = "使用未定义变量：" + str(shiftStr[-4]['row']) + "行" + str(shiftStr[-4]['colum']) + "列"
                    return

                if s.type != n.type:
                    token = shiftStr[-4]
                    self.semanticRst = False
                    self.semanticErrMsg = "赋值时变量类型错误：" + token['data'] + '，在' + str(token['row']) + "行" + str(token['colum']) + "列"
                    return

                sys.stdout.flush()
                code = None
                if n.place != None:  # a = 1;不存在n.place
                    code = (':=', n.place, '_', s.place)
                else:
                    code = (':=', n.data, '_', s.place)
                n.code.append(code)
                sys.stdout.flush()
                self.sStack.append(n)
            else: # -> array = expression ;
                n = copy.deepcopy(self.sStack.pop(-1))  # expression
                n.name = nt

                array_n = self.sStack.pop(-1)
                array_name = array_n.arrname # 取到数组的名字

                self.calExpression(n) # 计算表达式的值

                s = self.findSymbol(array_name, self.curFuncSymbol.label)
                if s == None:
                    print("使用未定义的数组变量！")
                    self.semanticRst = False
                    self.semanticErrMsg = "使用未定义的数组变量！" + array_name
                    return
                sys.stdout.flush()

                t_offset = self.getNewTemp() # 偏移地址临时变量
                if len(array_n.position) == 1: # 一维数组
                    n.code.append((':=', str(array_n.position[0]), '-', t_offset))
                elif len(array_n.position) == 2: # 二维数组
                    n.code.append(('*', str(array_n.position[0]), str(s.dims[1]), t_offset))
                    n.code.append(('+', t_offset, str(array_n.position[1]), t_offset))

                if n.place != None:
                    code = ('[]=', n.place, '_', s.place + '[' + t_offset + ']')
                else:
                    t = self.getNewTemp()
                    code = (':=', n.data, '_', t)
                    n.code.append(code)
                    code = ('[]=', t, '_', s.place + '[' + t_offset + ']')
                n.code.append(code)
                sys.stdout.flush()
                self.sStack.append(n)

        # returnStatement -> return expression; | return;
        elif nt == 'returnStatement':
            n = None
            if len(r) == 3:  # return expression
                n = self.sStack.pop(-1)  # expression

                # 计算返回值
                self.calExpression(n)
                n.type = r[0]['type']  # == return

                nRst = None
                if n.place != None:
                    nRst = n.place  # 返回存放expression的变量
                else:
                    nRst = n.data   # 返回expression的值(可能就等于一个常量)
                n.code.append((':=', nRst, '_', 'v0'))  # 返回地址
            elif len(r) == 2:  # return
                n = Node()
                n.type = r[0]['type']

            n.code.append((n.type, '_', '_', '_'))
            n.name = nt
            self.sStack.append(n)

        # expression -> primaryExpression | primaryExpression operator expression
        elif nt == 'expression':
            n = None
            if len(r) == 1:  # expression -> primaryExpression
                n = copy.deepcopy(self.sStack[-1])
                sys.stdout.flush()
                n.stack.insert(0, self.sStack.pop(-1))

            elif len(r) == 3:  # expression -> primaryExpression operator expression
                n = copy.deepcopy(self.sStack.pop(-1))  # expression
                n.stack.insert(0, self.sStack.pop(-1))  # operator
                n.stack.insert(0, self.sStack.pop(-1))  # primaryExpression

            n.name = nt
            self.sStack.append(n)
            sys.stdout.flush()

        # primaryExpression -> num | ( expression ) | id ( actualParaList ) | id | array
        elif nt == 'primaryExpression':
            n = Node()
            if len(r) == 1 and r[0]['type'] == 'INT':
                n.data = shiftStr[-1]['data']  # 具体数字
                n.type = shiftStr[-1]['type'].lower()

            # id ( actualParaList )
            elif len(r) == 4 and r[0]['type'] == 'IDENTIFIER':# 找定义过的
                function = self.findFuncSymbolByName(shiftStr[-4]['data'])
                n = self.sStack.pop(-1)  # actualParaList
                n.name = nt
                if function == None:
                    print('使用未定义的函数!')
                    self.semanticRst = False
                    self.semanticErrMsg = "未定义的函数：" + shiftStr[-4]['data'] + "，在" + str(shiftStr[-4]['row']) + "行" + str(shiftStr[-4]['colum']) + "列"
                    return

                if len(function.params) != len(n.stack):
                    print('实参和形参个数不匹配!')
                    sys.stdout.flush()
                    self.semanticRst = False
                    self.semanticErrMsg = "实参和形参个数不匹配：" + shiftStr[-4]['data'] + "，在" + str(shiftStr[-4]['row']) + "行" + str(shiftStr[-4]['colum']) + "列"
                    return

                code_temp = []
                symbol_temp_list = copy.deepcopy(self.curFuncSymbol.params)  # 获取调用函数参数
                code_temp.append(('-', 'sp', 4 * len(symbol_temp_list) + 4, 'sp'))  # 注意+4给ra留空间
                code_temp.append(('store', '_', 4 * len(symbol_temp_list), 'ra'))  # 保存ra的值避免复写
                for symbol in symbol_temp_list:  # 保存变量 因为是值传递，这里都还是t1
                    code_temp.append(('store', '_', 4 * symbol_temp_list.index(symbol), symbol[2]))
                for code in reversed(code_temp):
                    n.code.insert(0, code)

                if len(function.params) > 0:
                    n.code.append(('+', 'fp', 4 * len(function.params), 'fp'))  # 被调用函数

                for node in n.stack:  # 实参列表
                    if node.place != None:
                        node_result = node.place
                    else:
                        node_result = node.data
                    n.code.append(('push', '_', 4 * n.stack.index(node), node_result))
                n.code.append(('call', '_', '_', function.label))

                symbol_temp_list.reverse()
                for symbol in symbol_temp_list:
                    n.code.append(('load', '_', 4 * symbol_temp_list.index(symbol), symbol[2]))  # n.place = symbol[2]
                n.code.append(('load', '_', 4 * len(symbol_temp_list), 'ra'))
                n.code.append(('+', 'sp', 4 * len(self.curFuncSymbol.params) + 4, 'sp'))

                n.place = self.getNewTemp()
                n.code.append((':=', 'v0', '_', n.place))
                # ! 关键！保证primary的stack是空的 express里的stack的node个数必为2n + 1
                n.stack = []

            elif len(r) == 1 and r[0]['type'] == 'IDENTIFIER':
                n.data = shiftStr[-1]['data']  # 拿到变量名称
                nTmp = self.findSymbol(n.data, self.curFuncSymbol.label)
                if nTmp == None:
                    print('使用未定义变量!')
                    self.semanticRst = False
                    self.semanticErrMsg = "未定义的变量：" + shiftStr[-1]['data'] + "，在" + str(shiftStr[-1]['row']) + "行" + str(shiftStr[-1]['colum']) + "列"
                    return
                n.type = nTmp.type
                n.place = nTmp.place

            # ( expression )
            elif len(r) == 3 and r[1]['type'] == 'expression':
                n = self.sStack.pop(-1)
                self.calExpression(n)  # 有优先级

            else: # -> array
                n = self.sStack.pop(-1)
                nTmp = self.findSymbol(n.arrname, self.curFuncSymbol.label)
                if nTmp == None:
                    print('使用未定义数组变量!')
                    self.semanticRst = False
                    self.semanticErrMsg = "未定义的数组变量！" + n.arrname
                    return

                t_offset = self.getNewTemp() # 偏移地址临时变量
                if len(n.position) == 1: # 一维数组
                    n.code.append((':=', str(n.position[0]), '-', t_offset))
                elif len(n.position) == 2: # 二维数组
                    n.code.append(('*', str(n.position[0]), str(nTmp.dims[1]), t_offset))
                    n.code.append(('+', t_offset, str(n.position[1]), t_offset))

                t = self.getNewTemp()  # 临时变量
                n.code.append(('=[]', nTmp.place + '[' + t_offset + ']', '-', t))
                n.type = 'int'
                n.place = t
                n.stack = []

            n.name = nt
            self.sStack.append(n)
            sys.stdout.flush()

        # operator -> + | - | * | / | < | <= | >= | > | == | !=
        elif nt == 'operator':
            n = Node()
            n.name = 'operator'
            n.type = ''
            for i in range(len(r)):
                token = shiftStr[-(len(r) - i)]
                n.type += token['type']
            self.sStack.append(n)

        # actualParaList -> $ | expression | expression, actualParaList
        elif nt == 'actualParaList':
            n = None
            if len(r) == 3:  # formalParaList -> expression , formalParaList
                n = self.sStack.pop(-1)
                nExp = self.sStack.pop(-1)
                self.calExpression(nExp)
                n.stack.insert(0, nExp)

            elif len(r) == 1 and (r[0]['type'] in ['$']):  # $
                n = Node()

            elif len(r) == 1 and r[0]['type'] == 'expression':  # expression
                n = copy.deepcopy(self.sStack.pop(-1))
                self.calExpression(n)

            n.name = nt
            self.sStack.append(n)
            sys.stdout.flush()

        # ifStatement | if ( expression ) block | if ( expression ) block else block
        elif nt == 'ifStatement':
            n = Node()
            n.name = nt

            # if ( expression ) block
            if len(r) == 5:
                n.true = self.getNewLabel()
                n.end = self.getNewLabel()
                nT = self.sStack.pop(-1)  # True
                nExp = self.sStack.pop(-1)
                self.calExpression(nExp)
                n.code.extend(nExp.code)
                n.code.append(('j>', nExp.place, '0', n.true))
                n.code.append(('j', '_', '_', n.end))
                n.code.append((n.true, ':', '_', '_'))
                for code in nT.code:
                    n.code.append(code)
                n.code.append((n.end, ':', '_', '_'))

            # if ( expression ) block else block
            elif len(r) == 7:
                n.true = self.getNewLabel()
                n.false = self.getNewLabel()
                n.end = self.getNewLabel()
                nF = self.sStack.pop(-1)  # False
                nT = self.sStack.pop(-1)  # True
                nExp = self.sStack.pop(-1)
                self.calExpression(nExp)

                n.code.extend(nExp.code)

                n.code.append(('j>', nExp.place, '0', n.true))
                n.code.append(('j', '_', '_', n.false))
                n.code.append((n.true, ':', '_', '_'))
                for code in nT.code:
                    n.code.append(code)
                n.code.append(('j', '_', '_', n.end))
                n.code.append((n.false, ':', '_', '_'))
                for code in nF.code:
                    n.code.append(code)
                n.code.append((n.end, ':', '_', '_'))

            self.sStack.append(n)

        # iterStatement -> while ( expression ) block
        elif nt == 'iterStatement':
            n = Node()  # 生成新节点
            n.name = nt
            n.true = self.getNewLabel()  # 四个分支的入口
            n.false = self.getNewLabel()
            n.begin = self.getNewLabel()
            n.end = self.getNewLabel()

            if r[0]['type'] == 'while':
                statement = self.sStack.pop(-1)
                expression = self.sStack.pop(-1)
                self.calExpression(expression)
                n.code.append((n.begin, ':', '_', '_'))
                for code in expression.code:
                    n.code.append(code)
                n.code.append(('j>', expression.place, '0', n.true))
                n.code.append(('j', '_', '_', n.false))
                n.code.append((n.true, ':', '_', '_'))
                for code in statement.code:
                    if code[0] == 'break':
                        n.code.append(('j', '_', '_', n.false))
                    elif code[0] == 'continue':
                        n.code.append(('j', '_', '_', n.begin))
                    else:
                        n.code.append(code)
                n.code.append(('j', '_', '_', n.begin))
                n.code.append((n.false, ':', '_', '_'))

            self.sStack.append(n)
        sys.stdout.flush()
        return

    # 在符号表里查找符号
    def findSymbol(self, name, function):
        for s in self.symbolTable:
            if s.name == name and s.function == function:
                return s
        return None

    # 更新符号表
    def updateSymbolTable(self, symbol):
        for s in self.symbolTable:
            if s.name == symbol.name and s.function == symbol.function:
                self.symbolTable.remove(s)
                break
        self.symbolTable.append(symbol)
        return

    # 在函数表里查找函数
    def findFuncSymbolByName(self, name):
        for f in self.funcTable:
            if f.name == name:
                return f
        return None

    # 更新函数表
    def updateFuncTable(self, functionSymbol):
        for f in self.funcTable:
            if f.name == functionSymbol.name:
                self.funcTable.remove(f)
                break
        self.funcTable.append(functionSymbol)
        return

    def getNewTemp(self):
        self.curTempId += 1
        return "t" + str(self.curTempId)

    def getNewLabel(self):
        self.curLabelId += 1
        return 'l' + str(self.curLabelId)

    def getNewFuncLabel(self):
        self.curFuncId += 1
        return 'f' + str(self.curFuncId)

    # 对于 expression 节点 n, 生成对应代码, 放在属性 code 里
    def calExpression(self, n):
        if len(n.stack) == 1:  # 只有一个值 不用三元计算
            n = copy.deepcopy(n.stack[0])
            n.stack = []
            return True

        # 所有要计算的节点都已经在stack里了，其实相当于生成一个新节点
        n.code = []
        sys.stdout.flush()

        nLeft = n.stack.pop(0)
        while len(n.stack) > 0:
            nOp = n.stack.pop(0)
            nRight = n.stack.pop(0)

            if nLeft.place == None:  # 说明是一个常数
                arg1 = nLeft.data
            else:
                arg1 = nLeft.place

            if nRight.place == None:
                arg2 = nRight.data
            else:
                arg2 = nRight.place

            if len(nLeft.code) > 0:
                for code in nLeft.code:
                    n.code.append(code)

            if len(nRight.code) > 0:
                for code in nRight.code:
                    n.code.append(code)

            nRst = Node()
            nRst.name = None  # 不需要名字
            nRst.place = self.getNewTemp()  # 中间变量
            nRst.type = nRight.type
            code = (nOp.type, arg1, arg2, nRst.place)
            n.code.append(code)

            nLeft = nRst
            n.type = nRight.type
        n.place = n.code[-1][3]  # code的place位置
        return True

    def saveMidCodeToFile(self):
        text = ''
        for code in self.middleCode:
            text += '{}, {}, {}, {}\n'.format(code[0], code[1], code[2], code[3])
        middleCodeObj = open("middleCodeFile.txt", 'w+')
        middleCodeObj.write(text)
        middleCodeObj.close()
        return True

    def prtSemanticStack(self):
        print("------- 打印语义分析栈 -----------")
        for s in self.sStack:
            print(s.name)
        return

    def prtFuncTable(self):
        print("------- 打印函数表 -----------")
        for f in self.funcTable:
            paraList = []
            for para in f.params:
                paraList.append(para[0])
            print('函数名:', f.name, '形参列表:', paraList)
        return

    def prtSymbolTable(self):
        print("------- 打印符号表 -----------")
        for s in self.symbolTable:
            print(s.name, s.type, s.place, s.function)
        return

    def prtNodeCode(self, node):
        print('------ 打印语法树节点的中间代码 -----------' + node.name)
        for code in node.code:
            print(code)
        return

    def prtNodeStack(self, node):
        print('------ 打印语法树节点栈 -----------' + node.name)
        nameList = []
        for node in node.stack:
            nameList.append(node.name)
        print(nameList)
        return