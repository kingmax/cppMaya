#include "hi_maya.h"
#include <maya/MGlobal.h>

void* HiMaya::creator()
{
	return new HiMaya;
}

MStatus HiMaya::doIt(const MArgList& args)
{
	MGlobal::displayInfo("‡À£¨Maya Plugin");

	return MS::kSuccess;
}