#include "hiCmd.h"

MStatus TestCmd::doIt(const MArgList& args)
{
	displayWarning("\nhi, from tempTestCmd plugin", true);
	return MS::kSuccess;
}

void *TestCmd::creator()
{
	return new TestCmd;
}