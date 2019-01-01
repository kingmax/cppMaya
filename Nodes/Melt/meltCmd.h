#pragma once

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>

class MeltCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList&);
	virtual MStatus redoIt();
	virtual MStatus undoIt();

	static void *creator() { return new MeltCmd; }

private:
	MDGModifier dgMod;
};
