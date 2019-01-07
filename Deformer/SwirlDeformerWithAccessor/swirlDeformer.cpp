#include "swirlDeformer.h"
#include <maya/MMatrix.h>
#include <maya/MPoint.h>
#include <maya/MItGeometry.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <maya/MDagModifier.h>

MTypeId SwirlDeformer::typeId(0x00339);
MString SwirlDeformer::typeName("swirl");

MObject SwirlDeformer::deformSpace;
MObject SwirlDeformer::startDist;
MObject SwirlDeformer::endDist;

void *SwirlDeformer::creator()
{
	return new SwirlDeformer();
}

MStatus SwirlDeformer::deform(MDataBlock &block,
	MItGeometry &iter,
	const MMatrix &localToWorld,
	unsigned int geomIndex)
{
	MStatus stat;

	MDataHandle envData = block.inputValue(envelope);
	float env = envData.asFloat();
	if (env == 0.0)
	{
		return MS::kSuccess;
	}

	MDataHandle matData = block.inputValue(deformSpace);
	MMatrix mat = matData.asMatrix();
	MMatrix invMat = mat.inverse();

	MDataHandle startDistHnd = block.inputValue(startDist);
	double startDist = startDistHnd.asDouble();

	MDataHandle endDistHnd = block.inputValue(endDist);
	double endDist = endDistHnd.asDouble();

	MPoint pt;
	float weight;
	double dist, ang, cosAng, sinAng, x, distFactor;
	for (iter.reset(); !iter.isDone(); iter.next())
	{
		weight = weightValue(block, geomIndex, iter.index());
		if (weight == 0.0f)
		{
			continue;
		}
		pt = iter.position();
		pt *= invMat;

		dist = sqrt(pt.x * pt.x + pt.z * pt.z);
		if (dist < startDist || dist > endDist)
		{
			continue;
		}

		distFactor = 1 - ((dist-startDist) / (endDist - startDist));
		ang = distFactor * M_PI * 2.0 * env * weight;
		if (ang == 0.0)
		{
			continue;
		}

		cosAng = cos(ang);
		sinAng = sin(ang);
		x = pt.x * cosAng - pt.z * sinAng;
		pt.z = pt.x * sinAng + pt.z * cosAng;
		pt.x = x;

		pt *= mat;
		
		iter.setPosition(pt);
	}

	return stat;
}

MObject &SwirlDeformer::accessoryAttribute() const
{
	return deformSpace;
}

MStatus SwirlDeformer::accessoryNodeSetup(MDagModifier &dagMod)
{
	MStatus stat;

	MObject locObj = dagMod.createNode("locator", MObject::kNullObj, &stat);
	if (!stat)
	{
		return stat;
	}

	dagMod.renameNode(locObj, "swirlHandle");

	MFnDependencyNode locFn(locObj);
	MObject attrMat = locFn.attribute("matrix");
	stat = dagMod.connect(locObj, attrMat, thisMObject(), deformSpace);

	return stat;
}

