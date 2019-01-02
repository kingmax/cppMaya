#pragma once
#include <maya/MPxNode.h>

class GroundShadowNode : public MPxNode
{
public:
	static const MTypeId id;

	static void *creator();
	static MStatus initialize();

	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	static MObject lightPosition;
	static MObject castingSurface;
	static MObject shadowSurface;
	static MObject groundHeight;
};