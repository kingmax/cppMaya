#include "basicLocator.h"
#include <maya/MPxNode.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin pluginFn(obj, "jason.li", "0.1");
	stat = pluginFn.registerNode(BasicLocator::typeName,
		BasicLocator::typeId,
		BasicLocator::creator,
		BasicLocator::initialize,
		MPxNode::kLocatorNode);
	if (!stat)
	{
		stat.perror("registerNode failed");
		return MS::kFailure;
	}
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin pluginFn(obj);
	stat = pluginFn.deregisterNode(BasicLocator::typeId);
	if (!stat)
	{
		stat.perror("deregisterNode failed");
		return MS::kFailure;
	}
	return stat;
}