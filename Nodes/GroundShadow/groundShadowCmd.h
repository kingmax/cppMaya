#pragma once

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>

class GroundShadowCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList&);
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return true; }

	static void * creator() { return new GroundShadowCmd; }
	static MSyntax newSyntax();

private:
	MDGModifier dgMod;
};
