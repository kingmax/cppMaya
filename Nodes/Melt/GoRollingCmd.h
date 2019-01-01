//#pragma once

#ifndef __GO_ROLLING_CMD_H__
#define __GO_ROLLING_CMD_H__

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

#endif // !__GO_ROLLING_CMD_H__