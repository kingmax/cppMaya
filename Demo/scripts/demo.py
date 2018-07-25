import sys

from PySide2 import QtGui, QtCore, QtWidgets
from demo_ui import Ui_Form as _ui

from shiboken2 import wrapInstance
from maya import OpenMayaUI
from maya import cmds

##########################################################################
def getMayaMainWindow():
    ptr = OpenMayaUI.MQtUtil.mainWindow()
    return wrapInstance(long(ptr), QtCore.QObject)

class win(QtWidgets.QWidget, _ui):
    def __init__(self, parent=None):
        super(win, self).__init__()
        #self.ui = _ui()
        #self.ui.setupUi(self)
        self.setupUi(self)
        
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
        
##########################################################################
def main():
    p = getMayaMainWindow()
    w = win(p)
    w.show()

##########################################################################
if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv) #if in maya, comment this line
    
    p = None
    try:
        p = getMayaMainWindow()
    except Exception as ex:
        print(ex.message)
        print('[Warning] Maybe in standalone dev/debug mode, not in Maya\n')
        
    w = win(p)
    w.show()
    
    sys.exit(app.exec_())  #if in maya, comment this line

##########################################################################
#test in Maya2018 Win10
#usage:
#place Demo.mod into Maya modules folder
#launch maya
#import demo
#reload(demo)
#w = demo.win(demo.getMayaMainWindow())
#w.show()