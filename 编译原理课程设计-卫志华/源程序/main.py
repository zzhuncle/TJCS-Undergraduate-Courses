from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
import qtawesome
import os, time, sys
from Lexical import *
from ItemSet import *
from Syntactic import *
from ObjectCode import *

class SplashScreen(QSplashScreen):
    def __init__(self):
        super(SplashScreen, self).__init__(QtGui.QPixmap("splash.png"))  # 启动程序的图片

    # 动画效果设置
    def effect(self):
        self.setWindowOpacity(0)
        while True:
            newOpacity = self.windowOpacity() + 0.02  # 设置窗口透明度
            if newOpacity > 1:
                break
            self.setWindowOpacity(newOpacity)
            self.show()
            time.sleep(0.01)

class MainUi(object):
    def setupUi(self, MainWindow, cfg, family, parseRst, tokens):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(920, 620)
        MainWindow.setAttribute(QtCore.Qt.WA_TranslucentBackground)  # 设置窗口背景透明

        self.MainWindow = MainWindow
        self.main_widget = QtWidgets.QWidget()  # 创建窗口主部件
        self.main_layout = QtWidgets.QGridLayout()  # 创建主部件的网格布局
        self.main_widget.setLayout(self.main_layout)  # 设置窗口主部件布局为网格布局
        self.main_layout.setSpacing(0)

        self.top_widget = QtWidgets.QWidget()  # 创建头部部件
        self.top_widget.setObjectName('top_widget')
        self.top_layout = QtWidgets.QGridLayout()  # 创建头部部件的网格布局层
        self.top_widget.setLayout(self.top_layout)  # 设置头部部件布局为网格
        self.top_layout.setSpacing(0)

        self.left_widget = QtWidgets.QWidget()  # 创建左侧部件
        self.left_widget.setObjectName('left_widget')
        self.left_layout = QtWidgets.QGridLayout()  # 创建左侧部件的网格布局层
        self.left_widget.setLayout(self.left_layout)  # 设置左侧部件布局为网格

        self.left_widget1 = QtWidgets.QWidget()  # 创建左上侧部件
        self.left_widget1.setObjectName('left_widget1')
        self.left_layout1 = QtWidgets.QGridLayout()  # 创建左上侧部件的网格布局层
        self.left_widget1.setLayout(self.left_layout1)  # 设置左上侧部件布局为网格

        self.left_widget2 = QtWidgets.QWidget()  # 创建左中侧部件
        self.left_widget2.setObjectName('left_widget2')
        self.left_layout2 = QtWidgets.QGridLayout()  # 创建左中侧部件的网格布局层
        self.left_widget2.setLayout(self.left_layout2)  # 设置左中侧部件布局为网格

        self.left_widget3 = QtWidgets.QWidget()  # 创建左下侧部件-占位
        self.left_widget3.setObjectName('left_widget3')
        self.left_layout3 = QtWidgets.QGridLayout()  # 创建左下侧部件的网格布局层-占位
        self.left_widget3.setLayout(self.left_layout3)  # 设置左下侧部件布局为网格-占位

        self.left_layout.addWidget(self.left_widget1, 0, 0, 3, 3)  # 左侧部件在第0行第0列，占3行3列
        self.left_layout.addWidget(self.left_widget2, 3, 0, 6, 3)  # 左侧部件在第3行第0列，占6行3列
        self.left_layout.addWidget(self.left_widget3, 9, 0, 2, 3)  # 左侧部件在第9行第0列，占2行3列

        self.right_widget = QtWidgets.QWidget()  # 创建右侧部件
        self.right_widget.setObjectName('right_widget')
        self.right_layout = QtWidgets.QStackedLayout()
        self.right_widget.setLayout(self.right_layout)  # 设置右侧部件布局为堆叠

        '''#######################################################################################'''
        # UI 1
        self.ui1 = QtWidgets.QMainWindow()
        self.ui1_main_widget = QtWidgets.QWidget()  # 创建UI1窗口主部件
        # 输入代码
        self.label_code = QtWidgets.QLabel("输入代码：")

        # 写代码的框
        self.textCode = QtWidgets.QTextEdit()
        self.textCode.setObjectName("textCode")
        self.label_rowIndex = QtWidgets.QLabel()
        self.label_rowIndex.setText("0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24")
        index_code_Widget = QtWidgets.QWidget()  # 创建部件
        hhbox1 = QHBoxLayout()                    # 横向布局
        hhbox1.addWidget(self.label_rowIndex)     # 把行号加入布局
        hhbox1.addWidget(self.textCode)           # 把文本框加入布局
        index_code_Widget.setLayout(hhbox1)       # 把布局加入部件

        # 按钮
        self.Button = QPushButton()
        self.Button.setText('编译源代码')

        # 所有按钮的布局
        self.vboxcode = QVBoxLayout()
        self.vboxcode.addWidget(self.label_code)
        self.vboxcode.addWidget(index_code_Widget)
        self.vboxcode.addWidget(self.Button)
        self.ui1_main_widget.setLayout(self.vboxcode)
        self.ui1.setCentralWidget(self.ui1_main_widget)  # 设置窗口主部件
        self.ui1_main_widget.setStyleSheet(
        '''
            QLabel
            {
                font-size:20px;
                font-family:'隶书';
             }
        ''')
        self.Button.setStyleSheet(
        '''
            QPushButton
            {
                background:#F8F8FF;
                border:none;
                font-weight:600;
                font-size:20px;
                font-family:'隶书';
             }
        ''')

        '''#######################################################################################'''
        # UI 2
        self.ui2 = QtWidgets.QMainWindow()
        self.ui2_main_widget = QtWidgets.QWidget()  # 创建UI2窗口主部件

        self.table2 = QTableWidget()
        self.table2.setColumnCount(4)
        self.table2.setColumnWidth(0, 160)
        self.table2.setColumnWidth(1, 160)
        self.table2.setColumnWidth(2, 160)
        self.table2.setColumnWidth(3, 160)
        self.table2.setRowCount(16)
        self.table2.setHorizontalHeaderLabels(['类型', '值', '行', '列'])
        self.table2.horizontalScrollBar().setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                  "QScrollBar::handle:hover{background:gray;}"
                                                  "QScrollBar::sub-line{background:transparent;}"
                                                  "QScrollBar::add-line{background:transparent;}")
        self.table2.verticalScrollBar().setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                "QScrollBar::handle:hover{background:gray;}"
                                                "QScrollBar::sub-line{background:transparent;}"
                                                "QScrollBar::add-line{background:transparent;}")
        self.hhbox2 = QHBoxLayout()      # 横向布局
        self.hhbox2.addWidget(self.table2)     # 把表格加入布局
        self.ui2_main_widget.setLayout(self.hhbox2)
        self.ui2.setCentralWidget(self.ui2_main_widget)  # 设置窗口主部件
        self.table2.setFont(QtGui.QFont('隶书', 12))
        # 循环横向设置表头颜色和字体
        for index in range(self.table2.columnCount()):
            headItem = self.table2.horizontalHeaderItem(index)
            headItem.setFont(QtGui.QFont('隶书', 12))

        '''#######################################################################################'''
        # UI 3
        self.ui3 = QtWidgets.QMainWindow()
        self.ui3_main_widget = QtWidgets.QWidget()  # 创建UI3窗口主部件

        self.table3 = QTableWidget()
        self.table3.setColumnCount(3)
        self.table3.setColumnWidth(0, 214)
        self.table3.setColumnWidth(1, 214)
        self.table3.setColumnWidth(2, 214)
        self.table3.setRowCount(16)
        self.table3.setHorizontalHeaderLabels(['状态栈', '移动栈', '输入栈'])
        self.table3.horizontalScrollBar().setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                  "QScrollBar::handle:hover{background:gray;}"
                                                  "QScrollBar::sub-line{background:transparent;}"
                                                  "QScrollBar::add-line{background:transparent;}")
        self.table3.verticalScrollBar().setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                "QScrollBar::handle:hover{background:gray;}"
                                                "QScrollBar::sub-line{background:transparent;}"
                                                "QScrollBar::add-line{background:transparent;}")
        self.hhbox3 = QHBoxLayout()      # 横向布局
        self.hhbox3.addWidget(self.table3)     # 把表格加入布局
        self.ui3_main_widget.setLayout(self.hhbox3)
        self.ui3.setCentralWidget(self.ui3_main_widget)  # 设置窗口主部件
        self.table3.setFont(QtGui.QFont('隶书', 12))
        # 循环横向设置表头颜色和字体
        for index in range(self.table3.columnCount()):
            headItem = self.table3.horizontalHeaderItem(index)
            headItem.setFont(QtGui.QFont('隶书', 12))

        '''#######################################################################################'''
        # UI 4
        self.ui4 = QtWidgets.QMainWindow()
        self.ui4_main_widget = QtWidgets.QWidget()  # 创建UI4窗口主部件

        self.table4 = QTableWidget()
        self.table4.setColumnCount(4)
        self.table4.setColumnWidth(0, 160)
        self.table4.setColumnWidth(1, 160)
        self.table4.setColumnWidth(2, 160)
        self.table4.setColumnWidth(3, 160)
        self.table4.setRowCount(16)
        self.table4.setHorizontalHeaderLabels(['operation', 'arg1', 'arg2', 'result'])
        self.table4.horizontalScrollBar().setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                  "QScrollBar::handle:hover{background:gray;}"
                                                  "QScrollBar::sub-line{background:transparent;}"
                                                  "QScrollBar::add-line{background:transparent;}")
        self.table4.verticalScrollBar().setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                "QScrollBar::handle:hover{background:gray;}"
                                                "QScrollBar::sub-line{background:transparent;}"
                                                "QScrollBar::add-line{background:transparent;}")
        self.hhbox4 = QHBoxLayout()      # 横向布局
        self.hhbox4.addWidget(self.table4)     # 把表格加入布局
        self.ui4_main_widget.setLayout(self.hhbox4)
        self.ui4.setCentralWidget(self.ui4_main_widget)  # 设置窗口主部件
        self.table4.setFont(QtGui.QFont('隶书', 12))
        # 循环横向设置表头颜色和字体
        for index in range(self.table4.columnCount()):
            headItem = self.table4.horizontalHeaderItem(index)
            headItem.setFont(QtGui.QFont('隶书', 12))

        '''#######################################################################################'''
        # UI 5
        self.ui5 = QtWidgets.QMainWindow()
        self.ui5_main_widget = QtWidgets.QWidget()  # 创建UI5窗口主部件

        self.table5 = QTableWidget()
        self.table5.setColumnCount(4)
        self.table5.setColumnWidth(0, 160)
        self.table5.setColumnWidth(1, 160)
        self.table5.setColumnWidth(2, 160)
        self.table5.setColumnWidth(3, 160)
        self.table5.setRowCount(16)
        self.table5.setHorizontalHeaderLabels(['函数的标识符', '返回值类型', '入口处的标签', '形参列表'])
        self.table5.horizontalScrollBar().setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                  "QScrollBar::handle:hover{background:gray;}"
                                                  "QScrollBar::sub-line{background:transparent;}"
                                                  "QScrollBar::add-line{background:transparent;}")
        self.table5.verticalScrollBar().setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                "QScrollBar::handle:hover{background:gray;}"
                                                "QScrollBar::sub-line{background:transparent;}"
                                                "QScrollBar::add-line{background:transparent;}")
        self.hhbox5 = QHBoxLayout()      # 横向布局
        self.hhbox5.addWidget(self.table5)     # 把表格加入布局
        self.ui5_main_widget.setLayout(self.hhbox5)
        self.ui5.setCentralWidget(self.ui5_main_widget)  # 设置窗口主部件
        self.table5.setFont(QtGui.QFont('隶书', 12))
        # 循环横向设置表头颜色和字体
        for index in range(self.table5.columnCount()):
            headItem = self.table5.horizontalHeaderItem(index)
            headItem.setFont(QtGui.QFont('隶书', 12))

        '''#######################################################################################'''
        # UI 6
        self.ui6 = QtWidgets.QMainWindow()
        self.ui6_main_widget = QtWidgets.QWidget()  # 创建UI6窗口主部件

        self.table6 = QTableWidget()
        self.table6.setColumnCount(6)
        self.table6.setRowCount(16)
        self.table6.setHorizontalHeaderLabels(['符号的标识符', '类型', '占用字节数', '内存偏移量', '对应的中间变量', '所在函数'])
        self.table6.horizontalScrollBar().setStyleSheet("QScrollBar{background:transparent; height:10px;}"
                                                  "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                  "QScrollBar::handle:hover{background:gray;}"
                                                  "QScrollBar::sub-line{background:transparent;}"
                                                  "QScrollBar::add-line{background:transparent;}")
        self.table6.verticalScrollBar().setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                                "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                                "QScrollBar::handle:hover{background:gray;}"
                                                "QScrollBar::sub-line{background:transparent;}"
                                                "QScrollBar::add-line{background:transparent;}")
        self.hhbox6 = QHBoxLayout()            # 横向布局
        self.hhbox6.addWidget(self.table6)     # 把表格加入布局
        self.ui6_main_widget.setLayout(self.hhbox6)
        self.ui6.setCentralWidget(self.ui6_main_widget)  # 设置窗口主部件
        self.table6.setFont(QtGui.QFont('隶书', 12))
        # 循环横向设置表头颜色和字体
        for index in range(self.table6.columnCount()):
            headItem = self.table6.horizontalHeaderItem(index)
            headItem.setFont(QtGui.QFont('隶书', 12))

        '''#######################################################################################'''
        # UI 7
        self.ui7 = QtWidgets.QMainWindow()
        self.ui7_main_widget = QtWidgets.QWidget()  # 创建UI7窗口主部件

        self.hhbox7 = QHBoxLayout()
        self.objCodeTextBox = QTextEdit()
        self.hhbox7.addWidget(self.objCodeTextBox)
        self.ui7_main_widget.setLayout(self.hhbox7)
        self.ui7.setCentralWidget(self.ui7_main_widget)  # 设置窗口主部件
        self.objCodeTextBox.setFont(QtGui.QFont('隶书', 12))

        self.right_layout.addWidget(self.ui1)
        self.right_layout.addWidget(self.ui2)
        self.right_layout.addWidget(self.ui3)
        self.right_layout.addWidget(self.ui4)
        self.right_layout.addWidget(self.ui5)
        self.right_layout.addWidget(self.ui6)
        self.right_layout.addWidget(self.ui7)


        self.main_layout.addWidget(self.top_widget, 0, 0, 1, 13)  # 右侧部件在第0行第0列，占1行13列
        self.main_layout.addWidget(self.left_widget, 1, 0, 12, 3)  # 左侧部件在第1行第0列，占12行3列
        self.main_layout.addWidget(self.right_widget, 1, 3, 12, 10)  # 右侧部件在第1行第3列，占12行10列
        MainWindow.setCentralWidget(self.main_widget)  # 设置窗口主部件

        self.left_layout.setVerticalSpacing(0)
        self.left_layout.setSpacing(0)
        self.left_layout2.setSpacing(0)
        self.left_layout3.setSpacing(0)

        self.left_out = QtWidgets.QPushButton(qtawesome.icon('fa.sign-out', color='#808080'), "退出")
        self.left_out.setObjectName('left_out')
        self.left_out.clicked.connect(MainWindow.close)  # 点击按钮之后关闭窗口

        self.left_username = QtWidgets.QLabel("    编译原理课程设计")
        self.left_username.setObjectName('username')

        self.left_close = QtWidgets.QPushButton("")  # 占位
        self.left_visit = QtWidgets.QPushButton("")  # 占位
        self.left_mini = QtWidgets.QPushButton("")  # 占位
        self.zw = QtWidgets.QLabel("")  # 占位
        self.zw2 = QtWidgets.QLabel("")  # 占位

        self.left_label_1 = QtWidgets.QLabel("Admin")
        self.left_label_1.setObjectName('left_label')

        self.left_img_1 = QtWidgets.QLabel("图")
        self.left_img_1.setObjectName('left_img')

        # 按钮
        self.left_button_1 = QtWidgets.QPushButton(qtawesome.icon('fa.home', color='#2c3a45'), "编译源代码")
        self.left_button_1.setObjectName('left_button1')
        self.left_button_1.clicked.connect(lambda : self.right_layout.setCurrentIndex(0))
        self.left_button_2 = QtWidgets.QPushButton(qtawesome.icon('fa.file-picture-o', color='#2c3a45'), "词法分析结果")
        self.left_button_2.setObjectName('left_button2')
        self.left_button_2.clicked.connect(lambda : self.right_layout.setCurrentIndex(1))
        self.left_button_3 = QtWidgets.QPushButton(qtawesome.icon('fa.desktop', color='#2c3a45'), "语法分析结果")
        self.left_button_3.setObjectName('left_button3')
        self.left_button_3.clicked.connect(lambda : self.right_layout.setCurrentIndex(2))
        self.left_button_4 = QtWidgets.QPushButton(qtawesome.icon('fa.key', color='#2c3a45'), "中间代码")
        self.left_button_4.setObjectName('left_button4')
        self.left_button_4.clicked.connect(lambda : self.right_layout.setCurrentIndex(3))
        self.left_button_5 = QtWidgets.QPushButton(qtawesome.icon('fa.user', color='#2c3a45'), "函数表")
        self.left_button_5.setObjectName('left_button5')
        self.left_button_5.clicked.connect(lambda : self.right_layout.setCurrentIndex(4))
        self.left_button_6 = QtWidgets.QPushButton(qtawesome.icon('fa.user', color='#2c3a45'), "符号表")
        self.left_button_6.setObjectName('left_button6')
        self.left_button_6.clicked.connect(lambda : self.right_layout.setCurrentIndex(5))
        self.left_button_7 = QtWidgets.QPushButton(qtawesome.icon('fa.heart', color='#2c3a45'), "目标代码")
        self.left_button_7.setObjectName('left_button7')
        self.left_button_7.clicked.connect(lambda : self.right_layout.setCurrentIndex(6))

        self.recommend_button_1 = QtWidgets.QToolButton()
        self.recommend_button_1.setText("")  # 设置按钮文本
        self.recommend_button_1.setStyleSheet("font: 14pt \"隶书\";")  # 设置按钮文本
        self.recommend_button_1.setIcon(QtGui.QIcon(r"./logo.png"))  # 设置按钮图标
        self.recommend_button_1.setIconSize(QtCore.QSize(100, 100))  # 设置图标大小
        self.recommend_button_1.setToolButtonStyle(QtCore.Qt.ToolButtonTextUnderIcon)

        self.left_layout1.addWidget(self.recommend_button_1, 0, 0, 1, 1)
        self.left_layout2.addWidget(self.left_button_1, 0, 0, 1, 3)
        self.left_layout2.addWidget(self.left_button_2, 1, 0, 1, 3)
        self.left_layout2.addWidget(self.left_button_3, 2, 0, 1, 3)
        self.left_layout2.addWidget(self.left_button_4, 3, 0, 1, 3)
        self.left_layout2.addWidget(self.left_button_5, 4, 0, 1, 3)
        self.left_layout2.addWidget(self.left_button_6, 5, 0, 1, 3)
        self.left_layout2.addWidget(self.left_button_7, 6, 0, 1, 3)

        self.top_layout.addWidget(self.left_username, 0, 0, 1, 15)
        self.top_layout.addWidget(self.left_out, 0, 16, 1, 1)

        self.main_layout.setContentsMargins(0, 0, 0, 0)
        self.left_layout.setContentsMargins(0, 0, 0, 0)
        self.left_layout1.setContentsMargins(0, 0, 0, 0)
        self.left_layout2.setContentsMargins(0, 0, 0, 0)
        self.left_layout3.setContentsMargins(0, 0, 0, 0)
        self.right_layout.setContentsMargins(0, 0, 0, 0)
        MainWindow.setWindowFlag(QtCore.Qt.FramelessWindowHint)  # 隐藏边框
        self.Button.clicked.connect(self.TotalTest)

        self.top_widget.setStyleSheet(
        '''
            *{background-color:#303030;}
            QLabel
            {
                color:#ffffff;
                border:none;
                font-weight:600;
                font-size:25px;
                font-family:'隶书';
             }
            QPushButton
            {
                color:#ffffff;
                border:none;
                font-weight:600;
                font-size:20px;
                font-family:'隶书';
             }
            QWidget#top_widget
            {
                border-top-left-radius: 20px;
                border-top-right-radius: 20px;
            }
        ''')
        # 系统标记
        self.left_username.setStyleSheet(
        '''
            QPushButton
            { 
                text-align:left;
                padding-left:30px;
                font-size:24px;
            }
        ''')
        self.left_out.setStyleSheet(
        '''
            QPushButton
            { 
                text-align:right;
                padding-right:30px;
                color:#fcfcfc;
                font-size:25px;
            }
        ''')

        self.left_widget.setStyleSheet(
        '''
            QPushButton
            {
                border:none;
                font-size:20px;
                text-align:left;
                padding-left:30px;
                height:70px;
                font-family:'隶书';
            }
            QWidget#left_widget
            {
                background:#F8F8FF;
            }
        ''')
        self.left_widget1.setStyleSheet(
        '''
            *{color:#2c3a45;}
            QToolButton
            {
                border:none;
                font-weight:600;
                font-size:14px;
                font-family:'隶书';
            }
        ''')
        self.left_widget2.setStyleSheet(
        '''
            QPushButton:hover
            {
                background:#ffffff;
            }
        ''')
        self.right_widget.setStyleSheet(
        '''
            QWidget#right_widget
            {
                background:#F8F8FF;
            }
        ''')

    # 词法分析 + 语法分析 + 语义分析 + 中间代码 + 目标代码 生成
    def TotalTest(self):
        os.chdir(os.path.dirname(sys.argv[0]))  # 将工作路径改为该文件目录
        self.lex = LexicalAnalyzer()
        self.cfg = CFG()
        self.cfg.readGrammerFile('.\\grammer.txt')
        self.cfg.getDotItems()
        self.cfg.calFirstSet()
        self.family = ItemSetSpecificationFamily(self.cfg)
        self.family.buildFamily()
        self.ana = SyntacticAnalyzer(self.lex, self.cfg, self.family)
        self.ana.getTables()
        self.originCode = self.textCode.toPlainText()
        self.ana.isRecognizable(self.originCode)
        # 语法分析结果
        if self.ana.syntacticRst == False:
            ErrorDialog = QMessageBox.question(self.MainWindow,
                                               "语法分析出错",
                                               self.ana.syntacticErrMsg,
                                               QMessageBox.Yes)
            return
        # 语义分析结果
        elif self.ana.semantic.semanticRst == False:
            ErrorDialog = QMessageBox.question(self.MainWindow,
                                               "语义分析出错",
                                               self.ana.semantic.semanticErrMsg,
                                               QMessageBox.Yes)
            return
        # 语法分析成功后结果
        self.parseRst = self.ana.getParseRst()
        self.ana.semantic.saveMidCodeToFile()
        # 目标代码结果
        self.ocg = ObjectCodeGenerator(self.ana.semantic.middleCode, self.ana.semantic.symbolTable, self.ana.semantic.funcTable)
        self.ocg.genMips()
        self.mipsText = ''
        for code in self.ocg.mipsCode:
            self.mipsText += code + '\n'

        objCodeFile = open("objCodeFile.txt", "w")
        objCodeFile.write(self.mipsText)
        objCodeFile.close()

        ErrorDialog = QMessageBox.information(self.MainWindow,
                                              "成功",
                                              "生成目标代码成功！",
                                              QMessageBox.Yes)



        # 词法分析结果
        inputProgram = self.textCode.toPlainText()
        self.originCode = inputProgram
        self.tokens = self.lex.genTokensFromInputBox(inputProgram)
        self.table2.setRowCount(len(self.tokens))
        cnt_row = 0
        for dir in self.tokens:
            cnt_col = 0
            self.table2.setItem(cnt_row, cnt_col, QTableWidgetItem(dir['type']))
            cnt_col = cnt_col + 1
            self.table2.setItem(cnt_row, cnt_col, QTableWidgetItem(dir['data']))
            cnt_col = cnt_col + 1
            self.table2.setItem(cnt_row, cnt_col, QTableWidgetItem(str(dir['row'])))
            cnt_col = cnt_col + 1
            self.table2.setItem(cnt_row, cnt_col, QTableWidgetItem(str(dir['colum'])))
            cnt_row = cnt_row + 1

        # 语法分析结果
        self.table3.setRowCount(len(self.parseRst))
        cnt_row = 0
        for dir in self.parseRst:
            cnt_col = 0
            li = dir['stateStack']
            s = ""
            for item in li:
                s = s + str(item) + " "
            self.table3.setItem(cnt_row, cnt_col, QTableWidgetItem(s.strip("->")))
            cnt_col += 1

            li = dir['shiftStr']
            s = ""
            for item in li:
                s = s + str(item['type']) + " "
            self.table3.setItem(cnt_row, cnt_col, QTableWidgetItem(s))
            cnt_col += 1

            li = dir['inputStr']
            s = ""
            for item in li:
                s = s + str(item['type']) + " "
            self.table3.setItem(cnt_row, cnt_col, QTableWidgetItem(s))
            cnt_row += 1

        # 中间代码
        self.table4.setRowCount(len(self.ana.semantic.middleCode))
        cnt_row = 0
        for dir in self.ana.semantic.middleCode:
            cnt_col = 0
            self.table4.setItem(cnt_row, cnt_col, QTableWidgetItem(dir[cnt_col]))
            cnt_col += 1
            self.table4.setItem(cnt_row, cnt_col, QTableWidgetItem(dir[cnt_col]))
            cnt_col += 1
            self.table4.setItem(cnt_row, cnt_col, QTableWidgetItem(dir[cnt_col]))
            cnt_col += 1
            self.table4.setItem(cnt_row, cnt_col, QTableWidgetItem(dir[cnt_col]))
            cnt_row += 1

        # 函数表
        self.table5.setRowCount(len(self.ana.semantic.funcTable))
        cnt_row = 0
        for dir in self.ana.semantic.funcTable:
            cnt_col = 0
            self.table5.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.name))
            cnt_col += 1
            self.table5.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.type))
            cnt_col += 1
            self.table5.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.label))
            cnt_col += 1
            self.table5.setItem(cnt_row, cnt_col, QTableWidgetItem(str(dir.params)))
            cnt_row += 1

        # 符号表
        self.table6.setRowCount(len(self.ana.semantic.symbolTable))
        cnt_row = 0
        for dir in self.ana.semantic.symbolTable:
            cnt_col = 0
            self.table6.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.name))
            cnt_col += 1
            self.table6.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.type))
            cnt_col += 1
            self.table6.setItem(cnt_row, cnt_col, QTableWidgetItem(str(dir.size)))
            cnt_col += 1
            self.table6.setItem(cnt_row, cnt_col, QTableWidgetItem(str(dir.offset)))
            cnt_col += 1
            self.table6.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.place))
            cnt_col += 1
            self.table6.setItem(cnt_row, cnt_col, QTableWidgetItem(dir.function))
            cnt_row += 1

        # 目标代码
        self.objCodeTextBox.setText(self.mipsText)

def main():
    app = QtWidgets.QApplication(sys.argv)
    splash = SplashScreen()
    splash.effect()
    app.processEvents() # 设置启动画面不影响其他效果

    mainWindow = QtWidgets.QMainWindow()
    ui = MainUi()
    ui.setupUi(mainWindow, None, None, None, None)
    mainWindow.show()
    splash.finish(mainWindow)
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
