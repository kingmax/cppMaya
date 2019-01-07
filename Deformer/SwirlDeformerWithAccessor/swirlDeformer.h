#pragma once
#include <maya/MPxDeformerNode.h>

class SwirlDeformer : public MPxDeformerNode
{
public:
	static void *creator();
	static MStatus initialize();
	
	virtual MStatus deform(MDataBlock &block,
		MItGeometry &iter,
		const MMatrix &mat,
		unsigned int multiIndex);

	virtual MObject &accessoryAttribute() const;
	virtual MStatus accessoryNodeSetup(MDagModifier &cmd);

	static MTypeId typeId;
	static MString typeName;

private:
	static MObject deformSpace;
	static MObject startDist;
	static MObject endDist;
};