# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'D:\git\cppMaya\demo.ui'
#
# Created: Wed Jul 25 19:08:31 2018
#      by: pyside2-uic  running on PySide2 2.0.0~alpha0
#
# WARNING! All changes made in this file will be lost!

from PySide2 import QtCore, QtGui, QtWidgets

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(228, 238)
        self.gridLayout = QtWidgets.QGridLayout(Form)
        self.gridLayout.setObjectName("gridLayout")
        self.btnA = QtWidgets.QPushButton(Form)
        self.btnA.setObjectName("btnA")
        self.gridLayout.addWidget(self.btnA, 0, 0, 1, 1)
        self.btnB = QtWidgets.QPushButton(Form)
        self.btnB.setObjectName("btnB")
        self.gridLayout.addWidget(self.btnB, 1, 0, 1, 1)
        self.btnC = QtWidgets.QPushButton(Form)
        self.btnC.setObjectName("btnC")
        self.gridLayout.addWidget(self.btnC, 2, 0, 1, 1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtWidgets.QApplication.translate("Form", "Form", None, -1))
        self.btnA.setText(QtWidgets.QApplication.translate("Form", "PushButton A", None, -1))
        self.btnB.setText(QtWidgets.QApplication.translate("Form", "PushButton B", None, -1))
        self.btnC.setText(QtWidgets.QApplication.translate("Form", "PushButton C", None, -1))

