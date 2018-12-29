import sys

from PySide2 import QtGui, QtCore, QtWidgets
#from demo_ui import Ui_Form as _ui
from test_ui import Ui_MainWindow as _ui

from shiboken2 import wrapInstance
from maya import OpenMayaUI
from maya import cmds

from maya.api import OpenMaya

def maya_useNewAPI():
    print('maya use new API')
    pass

def getMayaMainWindow():
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    return wrapInstance(long(ptr), QtWidgets.QWidget)

#class Win(QtWidgets.QWidget, _ui):
class Win(QtWidgets.QMainWindow, _ui):
    def __init__(self, parent=None):
        super(Win, self).__init__(parent=parent)
        self.setupUi(self)
        #self.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint)
        if parent:
            # self.setParent(parent)
            # ref: http://help.autodesk.com/view/MAYAUL/2018/ENU/?guid=__files_GUID_3F96AF53_A47E_4351_A86A_396E7BFD6665_htm
            self.setWindowFlags(QtCore.Qt.Window)
            # self.setWindowFlags(parent.windowFlags() | QtCore.Qt.WindowStaysOnTopHint)

        #self.btnA.clicked.connect(self.btnA_click)
        #self.btnB.clicked.connect(self.btnB_click)
        #self.btnC.clicked.connect(self.btnC_click)
        try:
            cmds.loadPlugin('MultiCommand')
        except:
            pass

    def btnA_click(self):
        print('btnA_click')
        #cmds.mCmd1()
        sel = OpenMaya.MSelectionList()
        # OpenMaya.MGlobal.getSelectionListByName('persp', sel) # API 1.0
        sel = OpenMaya.MGlobal.getSelectionListByName('persp')
        dagPath = OpenMaya.MDagPath()
        # sel.getDagPath(0, dagPath)
        dagPath = sel.getDagPath(0) # API 2.0
        print(dagPath.fullPathName())

        myStrings = []
        # sel.getSelectionStrings(myStrings)
        myStrings = sel.getSelectionStrings()
        print(myStrings)

    def btnB_click(self):
        print('btnB_click')
        # cmds.mCmd2()

        from maya import OpenMaya
        types = OpenMaya.MStringArray() # error, python api removed MString, MStringArray!!!
        io = OpenMaya.MFileIO()
        io.getFileTypes(types)
        # types = OpenMaya.MGlobal.MFileIO.getFileTypes()
        print(types)

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
