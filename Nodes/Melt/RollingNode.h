//#pragma once

#ifndef __ROLLING_NODE_H__
#define __ROLLING_NODE_H__

#include <maya/MPxNode.h>

class RollingNode : public MPxNode
{
public:
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

	static void *creator();
	static MStatus initialize();

	static MObject distance;
	static MObject radius;
	static MObject rotation;

	static MTypeId id;
private:

};

#endif // !__ROLLING_NODE_H__
