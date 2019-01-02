#pragma once

#include <maya/MPxCommand.h>

class TestCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList&);
	static void *creator();
};