#include "RollingNode.h"

#include <maya/MGlobal.h>
#include <maya/MTypeId.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>

MTypeId RollingNode::id(0x00333);

MObject RollingNode::distance;
MObject RollingNode::radius;
MObject RollingNode::rotation;

const double PI = 3.1415926535;
const double TWOPI = 2.0 * PI;

MStatus RollingNode::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus stat;
	if (plug == rotation)
	{
		MDataHandle distData = data.inputValue(distance);
		MDataHandle radData = data.inputValue(radius);
		double dist = distData.asDouble();
		double rad = radData.asDouble();

		MDataHandle rotData = data.outputValue(rotation);
		rotData.set( -dist / rad );
		data.setClean(plug);
	}
	else
	{
		stat = MS::kUnknownParameter;
	}
	return stat;
}

void *RollingNode::creator()
{
	return new RollingNode();
}

MStatus RollingNode::initialize()
{
	MFnNumericAttribute nAttr;
	distance = nAttr.create("distance", "dist", MFnNumericData::kDouble, 0.0);
	radius = nAttr.create("radius", "rad", MFnNumericData::kDouble, 0.0);
	
	MFnUnitAttribute uAttr;
	rotation = uAttr.create("rotation", "rot", MFnUnitAttribute::kAngle, 0.0);

	addAttribute(distance);
	addAttribute(radius);
	addAttribute(rotation);

	attributeAffects(distance, rotation);
	attributeAffects(radius, rotation);

	return MS::kSuccess;
}