#include "meltNode.h"

#include <maya/MGlobal.h>
#include <maya/MTypeId.h>
#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MPointArray.h>

const MTypeId MeltNode::id(0x00334);
MObject MeltNode::inputSurface;
MObject MeltNode::outputSurface;
MObject MeltNode::amount;

MStatus MeltNode::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus stat;
	if (plug == outputSurface)
	{
		MDataHandle amountHnd = data.inputValue(amount);
		MDataHandle inputSurfaceHnd = data.inputValue(inputSurface);
		MDataHandle outputSurfaceHnd = data.outputValue(outputSurface);
		double amt = amountHnd.asDouble();
		MObject inputSurfaceObj = inputSurfaceHnd.asNurbsSurface();
		MFnNurbsSurfaceData surfaceDataFn;
		MObject newSurfaceData = surfaceDataFn.create();
		MFnNurbsSurface surfaceFn;
		surfaceFn.copy(inputSurfaceObj, newSurfaceData);
		surfaceFn.setObject(newSurfaceData);
		MPointArray pts;
		surfaceFn.getCVs(pts);
	}
}