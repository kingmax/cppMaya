#include "meltCmd.h"
#include "meltNode.h"
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>


MStatus MeltCmd::doIt(const MArgList &args)
{
	MStatus stat;

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MTime startTime = MAnimControl::minTime();
	MTime endTime = MAnimControl::maxTime();

	MItSelectionList iter(selection, MFn::kNurbsSurface);
	for (; !iter.isDone(); iter.next())
	{
		MObject shapeNode;
		iter.getDependNode(shapeNode);
		MFnDependencyNode shapeFn(shapeNode);
		MPlug createPlug = shapeFn.findPlug("create");
		MPlugArray srcPlugs;
		createPlug.connectedTo(srcPlugs, true, false);
		if (srcPlugs.length() == 1)
		{
			MPlug srcPlug = srcPlugs[0];
			MObject meltNode = dgMod.createNode(MeltNode::id);
			MFnDependencyNode meltFn(meltNode);
			MPlug outputSurfacePlug = meltFn.findPlug("outputSurface");
			MPlug inputSurfacePlug = meltFn.findPlug("inputSurface");
			// break existing connection and establish new connections
			dgMod.disconnect(srcPlug, createPlug);
			dgMod.connect(srcPlug, inputSurfacePlug);
			dgMod.connect(outputSurfacePlug, createPlug);

			static unsigned i = 0;
			MString name = MString("melting") + i++;
			dgMod.renameNode(meltNode, name);

			MString cmd;
			cmd = MString("setKeyframe -at amount -t ")
				+ startTime.value()
				+ " -v " + 0.0 + " " + name;
			dgMod.commandToExecute(cmd);
			cmd = MString("setKeyframe -at amount -t ")
				+ endTime.value()
				+ " -v " + 1.0 + " " + name;
			dgMod.commandToExecute(cmd);
		}
	}
	return redoIt();
}

MStatus MeltCmd::redoIt()
{
	return dgMod.doIt();
}

MStatus MeltCmd::undoIt()
{
	return dgMod.undoIt();
}