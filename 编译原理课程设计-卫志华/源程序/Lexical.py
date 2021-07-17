import re
import sys

class LexicalAnalyzer():
    def __init__(self):
        self.CURRENT_LINE = 0
        self.pInputStr = 0
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

    def remove_comments(self, text):
        comments = re.findall('//.*?\n', text, flags=re.DOTALL)
        if (len(comments) > 0):
            text = text.replace(comments[0], "")
        comments = re.findall('/\*.*?\*/', text, flags=re.DOTALL)
        if (len(comments) > 0):
            text = text.replace(comments[0], "")
        return text.strip()

    def scan(self, line):  # 经行一次扫描，返回得到的token以及剩余的字符串
        max = ''
        target_regex = self.regexs[0]
        index_sub = 0
        match = False
        for regex in self.regexs:
            result = re.match(regex, line)
            if (result):
                result = result.group(0)
                if (len(result) > len(max)):
                    match = True
                    max = result
                    target_regex = regex
        # 出错处理
        if (match == False):
            print(u"非法字符：" + line[0])
            return {"data": line[0], "regex": None, "remain": line[1 : ]}
        else:
            return {"data": max, "regex": target_regex, "remain": line[index_sub + len(max):]}

    def scan_line(self, line):  # 对一行进行重复扫描，获得一组token
        tokens = []
        result = line.strip().strip('\t')
        origin = result
        while True:
            if result == "":
                break
            before = result
            result = self.scan(result)
            if result['regex']:
                token = {}
                token['class'] = "T"
                token['row'] = self.CURRENT_LINE
                token['colum'] = origin.find(before) + 1
                token['name'] = self.type[self.regexs.index(result['regex'])].upper()
                token['data'] = result['data']
                token['type'] = token['name']

                # 保留字，对应文法中->不加引号，认定为终结符
                if result['data'] in self.reserved:
                    token['name'] = self.reserved[result['data']].lower()
                    token['type'] = token['name']

                # 操作符或者界符，对应文法中->加引号，认定为终结符
                if token['name'] == "operator".upper() or token['name'] == "seperator".upper():
                    token['type'] = token['data']

                if token['name'] == "INT":
                    token['data'] = token['data']  # 如果是int(token['data'])，词法分析表就显示不出来，因为要str
                tokens.append(token)

            result = result['remain'].strip().strip('\t')
            if (result == ""):
                return tokens
        return tokens

    def genTokensFromInputBox(self, inputStr):
        lines = self.remove_comments(inputStr).split('\n')
        tokens = []
        for line in lines:
            tokens_temp = self.scan_line(line)
            tokens += tokens_temp
            self.CURRENT_LINE += 1
        return tokens

    def getTokensOfOneLine(self, inputStr):
        if self.pInputStr >= len(inputStr):
            return []
        while True:
            idx = inputStr.find('\n', self.pInputStr)
            if idx == -1:
                idx = len(inputStr) - 1
            line = inputStr[self.pInputStr:idx + 1].strip()
            self.pInputStr = idx + 1
            if line == '':
                continue
            else:
                break

        tokens = self.scan_line(line)
        sys.stdout.flush()
        return tokens
