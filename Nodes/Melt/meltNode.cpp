#include "meltNode.h"

#include <maya/MGlobal.h>
#include <maya/MTypeId.h>
#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MPointArray.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>

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
		
		double minHeight = DBL_MAX, maxHeight = DBL_MIN, y;
		unsigned i;
		for ( i = 0; i < pts.length(); i++)
		{
			y = pts[i].y;
			if (y < minHeight)
			{
				minHeight = y;
			}
			if (y > maxHeight)
			{
				maxHeight = y;
			}
		}

		double dist = amt * (maxHeight - minHeight);

		MVector vec;
		double d;
		for ( i = 0; i < pts.length(); i++)
		{
			MPoint &p = pts[i];
			p.y -= dist;
			if (p.y < minHeight)
			{
				d = minHeight - p.y;
				//obj's center to the CV, looking from the top viewport
				vec = MVector(p.x, 0.0, p.z);
				//how far the CV should spread out
				double len = vec.length();
				if (len > 1.0e-3)
				{
					double newLen = len + d;
					vec *= newLen / len;
					p.x = vec.x;
					p.z = vec.z;
				}
				p.y = minHeight;
			}
		}
		surfaceFn.setCVs(pts);
		surfaceFn.updateSurface();

		outputSurfaceHnd.set(newSurfaceData);
		data.setClean(plug);
	}
	else
	{
		stat = MS::kUnknownParameter;
	}

	return stat;
}

MStatus MeltNode::initialize()
{
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	amount = nAttr.create("amount", "amt", MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	
	inputSurface = tAttr.create("inputSurface", "is", MFnNurbsSurfaceData::kNurbsSurface);
	tAttr.setHidden(true);

	outputSurface = tAttr.create("outputSurface", "os", MFnNurbsSurfaceData::kNurbsSurface);
	tAttr.setStorable(false);
	tAttr.setHidden(true);

	addAttribute(amount);
	addAttribute(inputSurface);
	addAttribute(outputSurface);

	attributeAffects(amount, outputSurface);
	attributeAffects(inputSurface, outputSurface);

	return MS::kSuccess;
}

void *MeltNode::creator()
{
	return new MeltNode;
}