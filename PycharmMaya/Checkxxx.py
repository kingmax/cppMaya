# -*- coding: utf-8 -*-
# module desc: check xxx
# date (version): 2018.08.x.x
# author: jason.li@neobards.cn | 184327932@qq.com | kingmax_res@163.com

import os
import sys
from maya import mel
from maya import cmds


########################################################################
class Check2Side(AbstractNode):
    """check maya file format"""

    # ----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        super(Check2Side, self).__init__()

        self.Name = u'渲染双面'
        self.Desc = u'检查物体渲染统计信息中是不是没有勾选双面'
        self.UIName = u'渲染双面'
        self.IsFixable = True
        # if need params
        # param1 = NodeParam('ext', '.mb', u'格式', EnumParamUICtrl.ComboBox,
        # cbValues=['.mb', '.ma']
        # )
        # self.ParamList = [param1]
        self.ErrorList = []

    # ----------------------------------------------------------------------
    def doCheck(self, *args, **kwargs):
        """do checking and
        set the node status,
        update the errorList
        """
        super(Check2Side, self).doCheck(*args, **kwargs)

        ##if the node has ParamList, get it to internal_kwargs
        # internal_kwargs = self.get_kwargs_from_ParamList()
        # print('internal_kwargs: %s'%str(internal_kwargs))

        ##then new variable (same as dict key), assign value to it
        # unit = internal_kwargs.get('unit')
        # print('arg: unit = %s'%unit)

        # start checking here...
        # _unit = cmds.currentUnit(q=True, linear=True)
        # if _unit != unit:
        # print('in CheckUnit.doCheck')
        # msg = u'场景单位是%s'%_unit
        # print(u'error msg:%s'%msg)
        # self.ErrorList = [msg]
        # self.Status = EnumNodeStatus.Failure
        # else:
        # self.Status = EnumNodeStatus.Success

        objs = cmds.ls(g=True)
        for obj in objs:
            if not cmds.getAttr('%s.doubleSided' % obj):
                self.ErrorList.append(obj)
        if self.ErrorList:
            self.Status = EnumNodeStatus.Failure
        else:
            self.Status = EnumNodeStatus.Success

    # ----------------------------------------------------------------------
    def doFix(self, selObjs=None):
        """fixed the selected or all,
        @param selObjs = [selected error objs or all error objs]
        update the errorList,
        set the node status
        """
        super(Check2Side, self).doFix(selObjs)

        # star fix here...
        # internal_kwargs = self.get_kwargs_from_ParamList()
        # unit = internal_kwargs.get('unit')
        # if unit:
        # cmds.currentUnit(linear=unit)
        # self.ErrorList = []
        # self.Status = EnumNodeStatus.Success
        if selObjs:
            for obj in selObjs:
                # print(obj)
                if obj in self.ErrorList:
                    cmds.setAttr('%s.doubleSided' % obj, True)
                    self.ErrorList.remove(obj)

        if len(self.ErrorList) == 0:
            self.Status = EnumNodeStatus.Success


# ----------------------------------------------------------------------
def newNode():
    """new a CheckNode instance object, every CheckNode must be has this function"""
    return Check2Side()


########################################################################
def main():
    checkNode = newNode()
    print(checkNode.__str__())

    checkNode.doCheck()
    print('\nAfter doCheck ----------------------------')
    print(checkNode.__str__())

    checkNode.doFix()
    print('\nAfter doFix ----------------------------')
    print(checkNode.__str__())


if __name__ == '__main__':
    main()
