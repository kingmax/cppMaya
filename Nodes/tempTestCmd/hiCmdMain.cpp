#include "hiCmd.h"
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin pluginFn(obj, "jason.li", "0.1", "Any");
	stat = pluginFn.registerCommand("hiCmd", TestCmd::creator);
	if (!stat)
	{
		stat.perror("registerCommand failed");
		return MS::kFailure;
	}
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin pluginFn(obj);
	stat = pluginFn.deregisterCommand("hiCmd");
	if (!stat)
	{
		stat.perror("deregisterCommand failed");
		return MS::kFailure;
	}
	return stat;
}