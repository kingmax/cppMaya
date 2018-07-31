# MayaCharm testing

import sys

print(sys.executable)

from maya import cmds

#print(cmds.polyCube())

from maya import OpenMaya

selObjs = OpenMaya.MSelectionList()
OpenMaya.MGlobal.getActiveSelectionList(selObjs)
print(selObjs.length())