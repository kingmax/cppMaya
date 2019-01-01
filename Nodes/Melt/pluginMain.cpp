#include "RollingNode.h"
#include "GoRollingCmd.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MString errStr;
	MFnPlugin pluginFn(obj, "Jason Li", "1.0", "Any");

	stat = pluginFn.registerCommand("goRolling", GoRollingCmd::creator);
	if (!stat)
	{
		errStr = "registerCommand failed";
		goto error;
	}

	stat = pluginFn.registerNode(
		"RollingNode",
		RollingNode::id,
		RollingNode::creator,
		RollingNode::initialize);
	if (!stat)
	{
		errStr = "registerNode failed";
		goto error;
	}

	return stat;

error:
	stat.perror(errStr);
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MString errStr;
	MFnPlugin pluginFn(obj);

	stat = pluginFn.deregisterCommand("goRolling");
	if (!stat)
	{
		errStr = "deregisterCommand failed";
		goto error;
	}

	stat = pluginFn.deregisterNode(RollingNode::id);
	if (!stat)
	{
		errStr = "deregisterNode failed";
		goto error;
	}

	return stat;

error:
	stat.perror(errStr);
	return stat;
}
