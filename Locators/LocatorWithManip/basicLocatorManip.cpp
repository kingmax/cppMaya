#include "basicLocatorManip.h"
#include <maya/MFnDistanceManip.h>
#include <maya/MFnStateManip.h>
#include <maya/MFnDependencyNode.h>

const MTypeId BasicLocatorManip::typeId(0x00338);

const MString BasicLocatorManip::typeName("basicLocatorManip");

MStatus BasicLocatorManip::createChildren()
{
	MStatus stat = MStatus::kSuccess;

	xWidthDagPath = addDistanceManip("xWidthManip", "xW");
	MFnDistanceManip xWidthFn(xWidthDagPath);
	xWidthFn.setStartPoint(MVector(0.0, 0.0, 0.0));
	xWidthFn.setDirection(MVector(1.0, 0.0, 0.0));

	zWidthDagPath = addDistanceManip("zWidthManip", "zW");
	MFnDistanceManip zWidthFn(zWidthDagPath);
	zWidthFn.setStartPoint(MVector(0.0, 0.0, 0.0));
	zWidthFn.setDirection(MVector(0.0, 0.0, 1.0));

	typeDagPath = addStateManip("typeManip", "tM");
	MFnStateManip typeFn(typeDagPath);
	typeFn.setMaxStates(3);

	return stat;
}

MStatus BasicLocatorManip::connectToDependNode(const MObject & node)
{
	MStatus stat;

	targetObj = node;
	MFnDependencyNode nodeFn(targetObj);

	MFnDistanceManip xWidthFn(xWidthDagPath);
	MPlug xWidthPlug = nodeFn.findPlug("xWidth", &stat);
	xWidthFn.connectToDistancePlug(xWidthPlug);
	addPlugToManipConversionCallback(xWidthFn.startPointIndex(), (plugToManipConversionCallback)centerPointCallback);
	addPlugToManipConversionCallback(xWidthFn.directionIndex(), (plugToManipConversionCallback)sideDirectionCallback);

	MFnDistanceManip zWidthFn(zWidthDagPath);
	MPlug zWidthPlug = nodeFn.findPlug("zWidth", &stat);
	zWidthFn.connectToDistancePlug(zWidthPlug);
	addPlugToManipConversionCallback(zWidthFn.startPointIndex(), (plugToManipConversionCallback)centerPointCallback);
	addPlugToManipConversionCallback(zWidthFn.directionIndex(), (plugToManipConversionCallback)backDirectionCallback);

	MFnStateManip typeFn(typeDagPath);
	MPlug typePlug = nodeFn.findPlug("dispType", &stat);
	typeFn.connectToStatePlug(typePlug);
	addPlugToManipConversionCallback(typeFn.positionIndex(), (plugToManipConversionCallback)centerPointCallback);

	finishAddingManips();
	MPxManipContainer::connectToDependNode(node);
	return MS::kSuccess;
}

void BasicLocatorManip::draw(M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status)
{
	__super::draw(view, path, style, status);
	//MPxManipContainer::draw(view, path, style, status);
	
	MFnDependencyNode nodeFn(targetObj);
	MPlug xWidthPlug = nodeFn.findPlug("xWidth");
	float xWidth;
	xWidthPlug.getValue(xWidth);
	MPlug zWidthPlug = nodeFn.findPlug("zWidth");
	float zWidth;
	zWidthPlug.getValue(zWidth);

	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);

	char str[100];
	MVector TextVector;
	MString distanceText;
	strcpy(str, "XWidth");
	distanceText = str;
	
	MVector xWidthTrans = nodeTranslation();
	TextVector = xWidthTrans;
	TextVector += worldOffset(MVector(xWidth, 0, 0));
	
	view.drawText(distanceText, TextVector, M3dView::kLeft);

	strcpy(str, "ZWidth");
	distanceText = str;
	MVector zWidthTrans = nodeTranslation();
	TextVector = zWidthTrans;
	TextVector += worldOffset(MVector(0, 0, zWidth));
	view.drawText(distanceText, TextVector, M3dView::kLeft);

	strcpy(str, "Type");
	distanceText = str;
	TextVector = nodeTranslation();
	TextVector += worldOffset(MVector(0, 0.1, 0));
	view.drawText(distanceText, TextVector, M3dView::kLeft);

	glPopAttrib();
	view.endGL();
}

MManipData BasicLocatorManip::centerPointCallback(unsigned index) const
{
	MFnNumericData numData;
	MObject numDataObj = numData.create(MFnNumericData::k3Double);
	MVector vec = nodeTranslation();
	numData.setData(vec.x, vec.y, vec.z);
	return MManipData(numDataObj);
}
