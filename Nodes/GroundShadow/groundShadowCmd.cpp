#include "groundShadowCmd.h"
#include "groundShadowNode.h"

#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MFn.h>
#include <maya/MFnSet.h>
#include <maya/MDagPath.h>
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>

#include <cassert>

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

	iter.setFilter(MFn::kGeometric);
	unsigned count;
	for (iter.reset(), count=0; !iter.isDone(); iter.next(), count++)
	{
		MDagPath geomShapePath;
		iter.getDagPath(geomShapePath);
		MDagPath geomTransformPath(geomShapePath);
		geomTransformPath.pop();

		MFnDagNode geomShapeFn(geomShapePath);
		MObject newGeomTransformObj = geomShapeFn.duplicate(false, false);

		MFnDagNode newGeomShapeFn(newGeomTransformObj);
		newGeomShapeFn.setObject(newGeomShapeFn.child(0));

		dagMod.reparentNode(newGeomShapeFn.object(), geomTransformPath.node());
		shadingGroupFn.addMember(newGeomShapeFn.object());

		MObject shadowNode = dagMod.createNode(GroundShadowNode::id);
		//assert(!shadowNode.isNull());

		MFnDependencyNode shadowNodeFn(shadowNode);
		MPlug castingSurfacePlug = shadowNodeFn.findPlug("castingSurface");
		MPlug shadowSurfacePlug = shadowNodeFn.findPlug("shadowSurface");
		MPlug lightPositionPlug = shadowNodeFn.findPlug("lightPosition");

		MString outGeomPlugName, inGeomPlugName;
		switch (geomShapePath.apiType())
		{
		case MFn::kMesh:
			outGeomPlugName = "worldMesh";
			inGeomPlugName = "inMesh";
			break;
		case MFn::kNurbsSurface:
			outGeomPlugName = "worldSpace";
			inGeomPlugName = "create";
			break;
		default:
			break;
		}
		MPlug outGeomPlug = geomShapeFn.findPlug(outGeomPlugName);
		
		unsigned instanceNum = geomShapePath.instanceNumber();
		outGeomPlug.selectAncestorLogicalIndex(instanceNum);

		MPlug inGeomPlug = newGeomShapeFn.findPlug(inGeomPlugName);

		dagMod.connect(pointLightTranslatePlug, lightPositionPlug);
		dagMod.connect(outGeomPlug, castingSurfacePlug);
		dagMod.connect(shadowSurfacePlug, inGeomPlug);
	}
	if (count == 0)
	{
		MGlobal::displayError("\nSelect one or more geometric objects.");
		return MS::kFailure;
	}

	return redoIt();
}

MStatus GroundShadowCmd::redoIt()
{
	return dagMod.doIt();
}

MStatus GroundShadowCmd::undoIt()
{
	return dagMod.undoIt();
}