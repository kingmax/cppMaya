import sys

from PySide2 import QtGui, QtCore, QtWidgets
from demo_ui import Ui_Form as _ui

from shiboken2 import wrapInstance
from maya import OpenMayaUI
from maya import cmds


def getMayaMainWindow():
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    return wrapInstance(long(ptr), QtWidgets.QWidget)

class Win(QtWidgets.QWidget, _ui):
    def __init__(self, parent=None):
        super(Win, self).__init__(parent=parent)
        self.setupUi(self)
        #self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)
        if parent:
            #self.setParent(parent)
            #ref: http://help.autodesk.com/view/MAYAUL/2018/ENU/?guid=__files_GUID_3F96AF53_A47E_4351_A86A_396E7BFD6665_htm
            self.setWindowFlags(QtCore.Qt.Window)
            #self.setWindowFlags(parent.windowFlags() | QtCore.Qt.WindowStaysOnTopHint)

        self.btnA.clicked.connect(self.btnA_click)
        self.btnB.clicked.connect(self.btnB_click)
        self.btnC.clicked.connect(self.btnC_click)
        try:
            cmds.loadPlugin('MultiCommand')
        except:
            pass

    def btnA_click(self):
        print('btnA_click')
        cmds.mCmd1()

    def btnB_click(self):
        print('btnB_click')
        cmds.mCmd2()

    def btnC_click(self):
        print('btnC_click')
        cmds.mCmd3()


def main():
    p = getMayaMainWindow()
    w = Win(p)
    w.show()

if __name__ == '__main__':
    #app = QtWidgets.QApplication(sys.argv)

    p = None
    try:
        p = getMayaMainWindow()
    except Exception as ex:
        print(ex.message)
        print('[Warning] Maybe in standalone dev/debug mode, not in Maya\n')

    w = Win(p)
    w.show()

    #sys.exit(app.exec_())
