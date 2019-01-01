#include "meltCmd.h"
#include "meltNode.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MString errStr;

	MFnPlugin pluginFn(obj, "jason.li", "1.0", "any");
	stat = pluginFn.registerCommand("melt", MeltCmd::creator);
	if (!stat)
	{
		errStr = "registerCommand failed";
		goto error;
	}
	stat = pluginFn.registerNode
	(
		"MeltNode",
		MeltNode::id,
		MeltNode::creator,
		MeltNode::initialize
	);
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
	stat = pluginFn.deregisterCommand("melt");
	if (!stat)
	{
		errStr = "deregisterCommand failed";
		goto error;
	}
	stat = pluginFn.deregisterNode(MeltNode::id);
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