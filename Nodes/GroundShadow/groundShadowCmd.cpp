#include "groundShadowCmd.h"

#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MFn.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>

MStatus GroundShadowCmd::doIt(const MArgList& args)
{
	MStatus stat;
	MSelectionList selection;
	MObject shadingGroupObj;

	selection.clear();
	MGlobal::getSelectionListByName("initialShadingGroup", selection);
	MGlobal::displayInfo("initialShadingGroup count: " + selection.length());
	selection.getDependNode(0, shadingGroupObj);

	MFnSet shadingGroupFn(shadingGroupObj);
	selection.clear();
	MGlobal::getActiveSelectionList(selection);
	displayInfo("active selection count: " + selection.length());
	
	MItSelectionList iter(selection);
	MDagPath lightTransformPath;
	MPlug pointLightTranslatePlug;
	iter.setFilter(MFn::kPointLight);
	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(lightTransformPath);
		// pop the shape node, only need transform
		lightTransformPath.pop();

		MFnDagNode dagNodeFn(lightTransformPath);
		pointLightTranslatePlug = dagNodeFn.findPlug("translate");
		// only the first selected light is taken into consideration
		break;
	}

	if (!lightTransformPath.isValid())
	{
		MGlobal::displayError("\nSelect a point light.");
		return MS::kFailure;
	}


}