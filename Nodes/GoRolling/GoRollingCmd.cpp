#include "GoRollingCmd.h"
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnTransform.h>
#include <maya/MString.h>
#include <maya/MItSelectionList.h>
#include <maya/MFn.h>
#include <maya/MSyntax.h>
#include <maya/MPlug.h>
//#include <maya/MFnPlugin.h>

MStatus GoRollingCmd::doIt(const MArgList &args)
{
	MStatus stat;

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MFnTransform transformFn;
	MString name;
	for (MItSelectionList iter(selection, MFn::kTransform); !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		transformFn.setObject(dagPath);
		MObject rollNodeObj = dgMod.createNode("RollingNode");
		MFnDependencyNode depNodeFn(rollNodeObj);
		dgMod.connect(transformFn.findPlug("translateX"), depNodeFn.findPlug("distance"));
		dgMod.connect(transformFn.findPlug("translateY"), depNodeFn.findPlug("radius"));
		dgMod.connect(depNodeFn.findPlug("rotation"), transformFn.findPlug("rotateZ"));
	}

	return redoIt();
}

MStatus GoRollingCmd::redoIt()
{
	return dgMod.doIt();
}

MStatus GoRollingCmd::undoIt()
{
	return dgMod.undoIt();
}

MSyntax GoRollingCmd::newSyntax()
{
	MSyntax syntax;
	return syntax;
}

//
//MStatus initializePlugin(MObject obj)
//{
//	MFnPlugin pluginFn(obj, "jason.li", "0.1");
//	MStatus stat;
//	stat = pluginFn.registerCommand("goRolling", GoRollingCmd::creator, GoRollingCmd::newSyntax);
//	if (!stat)
//	{
//		stat.perror("registerCommand failed");
//	}
//	return stat;
//}
//
//MStatus uninitializePlugin(MObject obj)
//{
//	MFnPlugin pluginFn(obj);
//	MStatus stat;
//	stat = pluginFn.deregisterCommand("goRolling");
//	if (!stat)
//	{
//		stat.perror("deregisterCommand failed");
//	}
//	return stat;
//}
//
