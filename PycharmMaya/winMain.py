import sys
from PySide2.QtWidgets import *
from shiboken2 import wrapInstance
from maya import OpenMayaUI


def getMayaWindow():
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    return wrapInstance(long(ptr), QMainWindow)


class Win(QMainWindow):
    def __int__(self):
        super(Win, self).__int__()
        self.init_ui()

    def init_ui(self):
        txtEdit = QTextEdit()
        self.setCentralWidget(txtEdit)

        exitAct = QAction('Exit', self)
        exitAct.setShortcut('Ctrl+Q')
        exitAct.setStatusTip('exit application')
        exitAct.triggered.connect(self.close)

        self.statusBar()

        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(exitAct)

        toolbar = self.addToolBar('Exit')
        toolbar.addAction(exitAct)

        self.setGeometry(300, 300, 350, 250)
        self.setWindowTitle('Main Window')

        layer = QHBoxLayout(self)
        txtEdit.setLayout(layer)


class Win2(QWidget):
    def __init__(self, parent=None):
        super(Win2, self).__init__(parent=parent)

        self.setGeometry(400, 300, 800, 600)
        self.setWindowTitle('QWidget window')

        self.btn = QPushButton()
        self.btn.move(100, 100)
        self.btn.setText('click me')
        self.btn.clicked.connect(self.btn_click)

    def btn_click(self):
        print('clicked..')


def main():
    #app = QApplication(sys.argv)
    w = Win2(getMayaWindow)
    w.show()
    #sys.exit(app.exec_())

if __name__ == '__main__':
    main()