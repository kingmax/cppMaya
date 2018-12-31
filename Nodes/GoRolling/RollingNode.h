#pragma once

#include <maya/MPxNode.h>

class RollingNode : public MPxNode
{
public:
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

private:

};

