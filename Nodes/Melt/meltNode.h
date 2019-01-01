#pragma once

#include <maya/MPxNode.h>

class MeltNode : public MPxNode
{
public:
	const static MTypeId id;
	// attributes
	static MObject inputSurface;
	static MObject outputSurface;
	static MObject amount;

	//
	virtual MStatus compute(const MPlug&, MDataBlock&);

	static void *creator();
	static MStatus initialize();
};