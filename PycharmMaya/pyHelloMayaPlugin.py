#coding:utf-8
import sys

import maya.api.OpenMaya as OpenMaya
import maya.api.OpenMayaUI as OpenMayaUI
import maya.api.OpenMayaAnim as OpenMayaAnim
import maya.api.OpenMayaRender as OpenMayaRender

def maya_useNewAPI():
    pass

class MyCommandClass(OpenMaya.MPxCommand):
    kPluginCmdName = 'pyHelloMaya'

    def __init__(self):
        super(MyCommandClass, self).__init__()

    @staticmethod
    def cmdCreator():
        return MyCommandClass()

    def doIt(self, args):
        OpenMaya.MGlobal.displayInfo("好啊，hi, maya, this message from python plugin using API2.0")


def initializePlugin(mobj):
    mplugin = OpenMaya.MFnPlugin(mobj)
    try:
        mplugin.registerCommand(MyCommandClass.kPluginCmdName, MyCommandClass.cmdCreator)
    except:
        sys.stderr.write('Failed to register command: %s'%MyCommandClass.kPluginCmdName)

def uninitializePlugin(mobj):
    mplugin = OpenMaya.MFnPlugin(mobj)
    try:
        mplugin.deregisterCommand(MyCommandClass.kPluginCmdName)
    except:
        sys.stderr.write('Failed to unregister command: %s'%MyCommandClass.kPluginCmdName)



#using
'''
import maya.cmds as cmds
cmds.loadPlugin('pyHelloMayaPlugin.py')
cmds.pyHelloMaya()
'''