# coding:utf-8
# this testing is ok for maya2019

import sys
from PySide2.QtWidgets import *
import MaxPlus


class _GCProtector(object):
    widgets = []


def getMaxWindow():
    return MaxPlus.GetQMaxMainWindow()


class Win(QMainWindow):
    def __init__(self, parent=None):
        super(Win, self).__init__(parent)
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


def main():
    # app = QApplication(sys.argv)
    w = Win(getMaxWindow())
    _GCProtector.widgets.append(w)
    w.show()
    # sys.exit(app.exec_())


if __name__ == '__main__':
    main()
    # w = Win()
    # w.show()
