#include "basicLocator.h"
#define _USE_MATH_DEFINES //for M_PI
//#include <cmath>
#include <math.h>

#include <maya/MPointArray.h>
#include <maya/MFnDagNode.h>


const double M_2PI = M_PI * 2.0;

bool BasicLocator::getCirclePoints(MPointArray &pts) const
{
	MStatus stat;
	MObject thisNode = thisMObject();
	MFnDagNode dagFn(thisNode);

	MPlug xWidthPlug = dagFn.findPlug(xWidth, &stat);
	float xWidthValue;
	xWidthPlug.getValue(xWidthValue);

	MPlug zWidthPlug = dagFn.findPlug(zWidth, &stat);
	float zWidthValue;
	zWidthPlug.getValue(zWidthValue);

	MPlug typePlug = dagFn.findPlug(dispType, &stat);
	short typeValue;
	typePlug.getValue(typeValue);

	unsigned nCirclePts;

	switch (typeValue)
	{
		case 0:
			nCirclePts = 4;
			break;
		case 1:
			nCirclePts = 5;
			break;
		default:
			nCirclePts = 20;
			break;
	}

	pts.clear();
	pts.setSizeIncrement(nCirclePts);

	MPoint pt;
	pt.y = 0;
	const double angleIncr = M_2PI / (nCirclePts - 1);
	double angle = 0.0;
	unsigned i = 0;
	for (; i < nCirclePts; i++, angle += angleIncr)
	{
		pt.x = xWidthValue * cos(angle);
		pt.z = zWidthValue * sin(angle);
		pts.append(pt);
	}

	return true;
}