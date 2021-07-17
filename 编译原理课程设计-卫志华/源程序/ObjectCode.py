# This Python file uses the following encoding: utf-8

import copy
import sys

# 目标代码生成器
class ObjectCodeGenerator():
    def __init__(self, middleCode, symbolTable, funcTable):
        self.middleCode = copy.deepcopy(middleCode)
        self.symbolTable = copy.deepcopy(symbolTable)
        self.funcNameTable = []
        for f in funcTable:
            self.funcNameTable.append(f.name)

        self.mipsCode = []
        self.regTable = { '$' + str(i): '' for i in range(7, 26) }
        self.varStatus = {}  # 记录变量是在寄存器当中还是内存当中

        self.DATA_SEGMENT = 10010000
        self.STACK_OFFSET = 8000
        return

    def getRegister(self, identifier, codes):
        if identifier[0] != 't':
            return identifier
        if identifier in self.varStatus and self.varStatus[identifier] == 'reg':
            for key in self.regTable:
                if self.regTable[key] == identifier:
                    return key

        while True:
            for key in self.regTable:
                if self.regTable[key] == '':
                    self.regTable[key] = identifier
                    self.varStatus[identifier] = 'reg'
                    return key
            self.freeRegister(codes)

    # 释放一个寄存器
    def freeRegister(self, codes):
        # 提取出使用了 reg 的变量, 形式如t1, t2, ...
        varRegUsed = list(filter(lambda x: x != '', self.regTable.values()))

        # 统计这些变量后续的使用情况
        varUsageCnts = {}
        for code in codes:
            for item in code:
                tmp = str(item)
                if tmp[0] == 't':  # 是个变量
                    if tmp in varRegUsed:
                        if tmp in varUsageCnts:
                            varUsageCnts[tmp] += 1
                        else:
                            varUsageCnts[tmp] = 1

        sys.stdout.flush()
        flag = False

        # 找出之后不会使用的变量所在的寄存器
        for var in varRegUsed:
            if var not in varUsageCnts:
                for reg in self.regTable:
                    if self.regTable[reg] == var:
                        self.regTable[reg] = ''
                        self.varStatus[var] = 'memory'
                        flag = True
        if flag:
            return

        # 释放最少使用的寄存器，
        sorted(varUsageCnts.items(), key=lambda x: x[1])
        varFreed = list(varUsageCnts.keys())[0]
        for reg in self.regTable:
            if self.regTable[reg] == varFreed:
                for item in self.symbolTable:
                    if item.place == varFreed:  # t1, t2, ...
                        self.mipsCode.append('addi $at, $zero, 0x{}'.format(self.DATA_SEGMENT))
                        self.mipsCode.append('sw {}, {}($at)'.format(reg, item.offset))
                        self.regTable[reg] = ''
                        self.varStatus[varFreed] = 'memory'
                        return

        return

    def genMips(self):
        mc = self.mipsCode
        dc = self.middleCode

        dc.insert(0, ('call', '_', '_', 'programEnd'))
        dc.insert(0, ('call', '_', '_', 'main'))

        mc.append('.data    # data section') # 数据段 存放数组
        for s in self.symbolTable:
            if s.type == 'int array':
                size = 4 # 单位字节
                for dim in s.dims:
                    size *= int(dim)
                mc.append('    ' + s.place + ': .space ' + str(size))

        mc.append('')
        mc.append('.text    # code section') # 代码段
        mc.append('    addiu $sp, $zero, 0x{}'.format(self.DATA_SEGMENT + self.STACK_OFFSET))
        mc.append('    or $fp, $sp, $zero')

        while dc:
            code = dc.pop(0)
            tmp = []
            for item in code:
                if item == 'v0':
                    tmp.append('$v0')
                else:
                    tmp.append(item)
            code = tmp

            if code[0] == ':=':
                src = self.getRegister(code[1], dc)
                dst = self.getRegister(code[3], dc)
                mc.append('    add {},$zero,{}'.format(dst, src))

            elif code[0] == '[]=': # []=, t21, _, t17[t22]
                src = self.getRegister(code[1], dc)
                base = code[3][ : code[3].index('[')]
                offset = code[3][code[3].index('[') + 1 : -1]
                dst_offset = self.getRegister(offset, dc)
                mc.append('    la $v1,{}'.format(base))
                mc.append('    mul {},{},4'.format(dst_offset,dst_offset))
                mc.append('    addu {},{},$v1'.format(dst_offset, dst_offset))
                mc.append('    sw {},'.format(src) + '0({})'.format(dst_offset))

            elif code[0] == '=[]': # =[], t17[t23], -, t24
                dst = self.getRegister(code[3], dc)
                base = code[1][ : code[1].index('[')]
                offset = code[1][code[1].index('[') + 1 : -1]
                src_offset = self.getRegister(offset, dc)
                mc.append('    la $v1,{}'.format(base))
                mc.append('    mul {},{},4'.format(src_offset,src_offset))
                mc.append('    addu {},{},$v1'.format(src_offset, src_offset))
                mc.append('    lw {},'.format(dst) + '0({})'.format(src_offset))

            # function or label
            elif code[1] == ':':
                if code[0] in self.funcNameTable or code[0][0] == 'f':  # is a function definition
                    mc.append('')  # empty line
                mc.append('{}:'.format(code[0]))

            # 跳转到函数的label处
            elif code[0] == 'call':
                mc.append('    jal  {}'.format(code[3]))

            # actual arg of a function call
            elif code[0] == 'push':
                if code[3] == 'ra':  # return addr
                    mc.append('    sw $ra, {}($fp)'.format(code[2]))
                else:
                    register = self.getRegister(code[3], dc)
                    if str(register)[0] != '$':
                        mc.append("    add $a0, $zero, {}".format(register))
                        register = '$a0'
                    mc.append('    sw {}, {}($fp)'.format(register, code[2]))

            # get args inside the function
            elif code[0] == 'pop':
                if code[3] == 'ra':
                    mc.append('    lw $ra, {}($fp)'.format(code[2]))
                else:
                    register = self.getRegister(code[3], dc)
                    mc.append('    lw {}, {}($fp)'.format(register, code[2]))

            # store var from reg to memory
            elif code[0] == 'store':
                if code[3] == 'ra':
                    mc.append('    sw $ra, {}($sp)'.format(code[2]))
                else:
                    register = self.getRegister(code[3], dc)
                    if str(register)[0] != '$':
                        mc.append("    add $a0,$zero,{}".format(register))
                        register = '$a0'
                    mc.append('    sw {}, {}($sp)'.format(register, code[2]))

            # load var from memory to reg
            elif code[0] == 'load':
                if code[3] == 'ra':
                    mc.append('    lw $ra, {}($sp)'.format(code[2]))
                else:
                    register = self.getRegister(code[3], dc)
                    mc.append('    lw {}, {}($sp)'.format(register, code[2]))

            # jump instruction
            elif code[0] == 'j':
                mc.append('    j {}'.format(code[3]))

            elif code[0] == 'j>':
                arg1 = self.getRegister(code[1], dc)
                mc.append('    bgt {},$zero,{}'.format(arg1, code[3]))

            elif code[0] == 'return':
                mc.append('    jr $ra')

            # algorithm operations, has 3 oprand
            else:
                if code[0] == '+':
                    if code[1] == 'fp':
                        mc.append("    add $fp,$fp,{}".format(code[2]))
                    elif code[1] == 'sp':
                        mc.append("    add $sp,$sp,{}".format(code[2]))
                    else:
                        arg1 = self.getRegister(code[1], dc)
                        arg2 = self.getRegister(code[2], dc)
                        arg3 = self.getRegister(code[3], dc)
                        if str(arg1)[0] != '$':
                            mc.append("    add $a1,$zero,{}".format(arg1))
                            arg1 = '$a1'
                        mc.append("    add {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '-':
                    if code[1] == 'fp':
                        mc.append("    sub $fp,$fp,{}".format(code[2]))
                    elif code[1] == 'sp':
                        mc.append("    sub $sp,$sp,{}".format(code[2]))
                    else:
                        arg1 = self.getRegister(code[1], dc)
                        arg2 = self.getRegister(code[2], dc)
                        arg3 = self.getRegister(code[3], dc)
                        if str(arg1)[0] != '$':
                            mc.append("    add $a1,$zero,{}".format(arg1))
                            arg1 = '$a1'
                        if str(arg2)[0] != '$':
                            mc.append("    add $a2,$zero,{}".format(arg2))
                            arg2 = '$a2'
                        mc.append("    sub {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '*':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    mul {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '/':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    div {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '%':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    div {},{},{}".format(arg3, arg1, arg2))
                    mc.append("    mfhi {}".format(arg3))

                elif code[0] == '<':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    slt {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '>':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    sgt {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '!=':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    sne {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '==':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    seq {},{},{}".format(arg3, arg1, arg2))

                elif code[0] == '<=':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    sgt {},{},{}".format(arg3, arg1, arg2))
                    mc.append("    xori {},{},1".format(arg3, arg3))

                elif code[0] == '>=':
                    arg1 = self.getRegister(code[1], dc)
                    arg2 = self.getRegister(code[2], dc)
                    arg3 = self.getRegister(code[3], dc)
                    if str(arg1)[0] != '$':
                        mc.append("    add $a1,$zero,{}".format(arg1))
                        arg1 = '$a1'
                    if str(arg2)[0] != '$':
                        mc.append("    add $a2,$zero,{}".format(arg2))
                        arg2 = '$a2'
                    mc.append("    slt {},{},{}".format(arg3, arg1, arg2))
                    mc.append("    xori {},{},1".format(arg3, arg3))

        mc.append('')
        mc.append('programEnd:')
        mc.append('    nop')

        sys.stdout.flush()
        return

    def prtMips(self):
        for code in self.mipsCode:
            print(code)
        return
