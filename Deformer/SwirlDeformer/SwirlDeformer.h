#pragma once
#include <maya/MPxDeformerNode.h>


class SwirlDeformer : public MPxDeformerNode
{
public:
	static void *creator() { return new SwirlDeformer; }
	static MStatus initialize();

	virtual MStatus deform(MDataBlock&, MItGeometry&, const MMatrix&, unsigned int);

	static MTypeId typeId;
	static MString typeName;


private:
	static MObject startDist;
	static MObject endDist;


};