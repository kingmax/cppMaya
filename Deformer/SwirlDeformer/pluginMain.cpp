#include "SwirlDeformer.h"
#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>

MStatus initializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin pluginFn(obj, "jason.li", "1.0", "Any");
	stat = pluginFn.registerNode(SwirlDeformer::typeName,
		SwirlDeformer::typeId, 
		SwirlDeformer::creator,
		SwirlDeformer::initialize,
		MPxNode::kDeformerNode);

	if (!stat)
	{
		stat.perror("registerNode failed");
		return stat;
	}

	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat;
	MFnPlugin pluginFn(obj, "jason.li", "1.0", "Any");
	stat = pluginFn.deregisterNode(SwirlDeformer::typeId);

	if (!stat)
	{
		stat.perror("deregisterNode failed");
		return stat;
	}

	return stat;
}