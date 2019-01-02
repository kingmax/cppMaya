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

void BasicLocator::draw(M3dView &view, const MDagPath &path,
	M3dView::DisplayStyle style,
	M3dView::DisplayStatus status)
{
	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);
	
	MPointArray pts;
	getCirclePoints(pts);

	glBegin(GL_LINE_STRIP);
		for (size_t i = 0; i < pts.length(); i++)
		{
			glVertex3f(float(pts[i].x), float(pts[i].y), float(pts[i].z));
		}
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(-0.5f, 0.0f, 0.0f);
		glVertex3f(0.5f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -0.5f);
		glVertex3f(0.0f, 0.0f, 0.5f);
	glEnd();

	glPopAttrib();

	view.endGL();
}

bool BasicLocator::isBounded() const
{
	return true;
}