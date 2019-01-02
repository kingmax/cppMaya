#pragma once
#include <maya/MPxLocatorNode.h>

class BasicLocator : public MPxLocatorNode
{
public:
	virtual void draw(M3dView&, const MDagPath&, M3dView::DisplayStyle, M3dView::DisplayStatus);
	virtual bool isBounded() const;
	virtual MBoundingBox boundingBox() const;

	static void *creator();
	static MStatus initialize();

	static const MTypeId typeId;
	static const MString typeName;

	// Attributes
	static MObject xWidth;
	static MObject zWidth;
	static MObject dispType;

private:
	bool getCirclePoints(MPointArray&) const;
};