#include "groundShadowCmd.h"
#include "groundShadowNode.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MString errMsg;
	MFnPlugin pluginFn(obj, "jason.li", "0.1", "Any");
	stat = pluginFn.registerCommand("groundShadow", GroundShadowCmd::creator);
	if (!stat)
	{
		errMsg = "registerCommand failed";
		goto error;
	}
	stat = pluginFn.registerNode(
		"GroundShadow",
		GroundShadowNode::id,
		GroundShadowNode::creator,
		GroundShadowNode::initialize
	);
	if (!stat)
	{
		errMsg = "registerNode failed";
		goto error;
	}
	return stat;

error:
	stat.perror(errMsg);
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MString errMsg;
	MFnPlugin pluginFn(obj);
	stat = pluginFn.deregisterCommand("groundShadow");
	if (!stat)
	{
		errMsg = "deregisterCommand failed";
		goto error;
	}
	stat = pluginFn.deregisterNode(GroundShadowNode::id);
	if (!stat)
	{
		errMsg = "deregisterNode failed";
		goto error;
	}
	return stat;

error:
	stat.perror(errMsg);
	return stat;
}