#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

#include "hi_maya.h"

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat", "2018.07.23.01");
	MStatus status;

	status = plugin.registerCommand("HiMaya", HiMaya::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status;

	status = plugin.deregisterCommand("HiMaya");
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}