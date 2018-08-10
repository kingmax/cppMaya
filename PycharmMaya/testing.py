# coding:utf-8
# this testing is ok for maya2018

import sys
from PySide2.QtWidgets import *
from shiboken2 import wrapInstance
from maya import OpenMayaUI


def getMayaWindow():
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    return wrapInstance(long(ptr), QWidget)


class Win(QMainWindow):
    def __init__(self, parent=None):
        super(Win, self).__init__(parent)
        if parent:
            self.setWindowFlags(parent.windowFlags())
        self.init_ui()

    def init_ui(self):
        self.resize(400, 300)
        self.setWindowTitle('testing')
        # create menu
        menu = self.menuBar()
        filemenu = menu.addMenu('&File')
        filemenu.setStatusTip('file menu')

        # action (menu item)
        act = QAction('Exit', self)
        act.setStatusTip('exit application')
        act.triggered.connect(self.close)
        filemenu.addAction(act)

        # toolBar
        tb = self.addToolBar('Exit')
        tb.addAction(act)

        # button test
        btn = QPushButton('click me', self)
        btn.clicked.connect(self.btn_click)
        btn.move(50, 50)
        btn.setStatusTip('click me button')

        te = QTextEdit()
        self.setCentralWidget(te)
        
        self.statusBar()

    def btn_click(self):
        print('clicked')


class MyWidget(QWidget):
    def __init__(self, parent=None):
        super(MyWidget, self).__init__(parent)
        if parent:
            self.setWindowFlags(parent.windowFlags())

        self.setGeometry(800, 600, 400, 300)
        self.setWindowTitle('my widget')

        self.btn = QPushButton('close', self)
        self.btn.move(50, 20)
        self.btn.clicked.connect(self.close)


def main():
    try:
        app = QApplication(sys.argv)
        w = Win()
        w.show()
        sys.exit(app.exec_())
    except:
        w = Win(getMayaWindow())
        w.show()

def main2():
    try:
        app = QApplication(sys.argv)
        w = MyWidget()
        w.show()
        sys.exit(app.exec_())
    except:
        w = MyWidget(getMayaWindow())
        w.show()


if __name__ == '__main__':
    # main()
    #app = QApplication(sys.argv)
    # w = MyWidget(getMayaWindow())
    main2()
    #sys.exit(app.exec_())
