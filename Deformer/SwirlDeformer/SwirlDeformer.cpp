#include "SwirlDeformer.h"
#include <maya/MItGeometry.h>
#include <maya/MPoint.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MDistance.h>

MTypeId SwirlDeformer::typeId(0x0033A);
MString SwirlDeformer::typeName("swirl");

MObject SwirlDeformer::startDist;
MObject SwirlDeformer::endDist;

MStatus SwirlDeformer::initialize()
{
	MFnUnitAttribute unitFn;
	startDist = unitFn.create("startDist", "sd", MFnUnitAttribute::kDistance);
	unitFn.setDefault(MDistance(0.0, MDistance::uiUnit()));
	unitFn.setMin(MDistance(0.0, MDistance::uiUnit()));
	unitFn.setKeyable(true);

	endDist = unitFn.create("endDist", "ed", MFnUnitAttribute::kDistance);
	unitFn.setDefault(MDistance(3.0, MDistance::uiUnit()));
	unitFn.setMin(MDistance(0.0, MDistance::uiUnit()));
	unitFn.setKeyable(true);

	addAttribute(startDist);
	addAttribute(endDist);

	attributeAffects(startDist, outputGeom);
	attributeAffects(endDist, outputGeom);

	return MS::kSuccess;
}

MStatus SwirlDeformer::deform(MDataBlock &data, MItGeometry &iter, const MMatrix &localToWorld, unsigned int geomIndex)
{
	MStatus stat;

	MDataHandle envData = data.inputValue(envelope);
	float env = envData.asFloat();
	if (env == 0.0)
	{
		return MS::kSuccess;
	}

	MDataHandle startDistHnd = data.inputValue(startDist);
	double startDist = startDistHnd.asDouble();
	MDataHandle endDistHnd = data.inputValue(endDist);
	double endDist = endDistHnd.asDouble();

	float weight;
	MPoint pt;
	double dist;
	double distFactor;
	double ang;
	double cosAng, sinAng;
	double x;

	for (iter.reset(); !iter.isDone(); iter.next())
	{
		weight = weightValue(data, geomIndex, iter.index());
		if (weight == 0.0)
		{
			continue;
		}
		pt = iter.position();
		dist = sqrt(pt.x * pt.x + pt.z * pt.z);
		if (dist < startDist || dist > endDist)
		{
			continue;
		}
		distFactor = 1 - ((dist - startDist) / (endDist - startDist));
		ang = distFactor * M_PI * 2.0 * env * weight;
		if (ang == 0.0)
		{;
			continue;
		}
		cosAng = cos(ang);
		sinAng = sin(ang);
		x = pt.x * cosAng - pt.z * sinAng;
		pt.z = pt.x * sinAng + pt.z * cosAng;
		pt.x = x;

		iter.setPosition(pt);
	}

	return stat;
}
