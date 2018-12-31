#pragma once

#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>

class GoRollingCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList&);
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return true; }

	static void *creator() { return new GoRollingCmd; }
	static MSyntax newSyntax();
private:
	MDGModifier dgMod;
};

