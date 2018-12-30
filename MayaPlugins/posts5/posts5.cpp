#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>
#include <maya/MFn.h>
#include <maya/MFnPlugin.h>
#include <maya/MPoint.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MObject.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MArgDatabase.h>
#include <maya/MDGModifier.h>


const char *cmdName = "posts5";

//Syntax string definitions
const char *numberFlag = "-n", *numberLongFlag = "-number";
const char *radiusFlag = "-r", *radiusLongFlag = "-radius";
const char *heightFlag = "-he", *heightLongFlag = "-height";
const char *helpFlag = "-h", *helpLongFlag = "-help";
const char *helpText =
"\nThe posts4 command is used to create a series of posts"
"(c y l i n d e r s) along all the selected curves."
"\n l t is possible to set the number of posts, as well as"
"t h e i r width and h e i g h t ."
"\nFor f u r t h e r d e t a i l s consult the help documentation."
"\nFor quick help use: help posts4";


class Posts5Cmd : public MPxCommand
{
public:
	static void * creator() { return new Posts5Cmd; }
	virtual MStatus doIt(const MArgList & args);
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return true; }
	virtual bool hasSyntax() const { return true; }
	static MSyntax newSyntax();

private:
	MDGModifier dgMod;
};

MStatus Posts5Cmd::doIt(const MArgList & args)
{
	int nPosts = 5;
	double radius = 0.5;
	double height = 5.0;

	MArgDatabase argData(syntax(), args);
	if (argData.isFlagSet(numberFlag))
	{
		argData.getFlagArgument(numberFlag, 0, nPosts);
	}
	if (argData.isFlagSet(radiusFlag))
	{
		argData.getFlagArgument(radiusFlag, 0, radius);
	}
	if (argData.isFlagSet(heightFlag))
	{
		argData.getFlagArgument(heightFlag, 0, height);
	}
	if (argData.isFlagSet(helpFlag))
	{
		setResult(helpText);
		return MS::kSuccess;
	}

	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);

	MDagPath dagPath;
	MFnNurbsCurve curveFn;
	double heightRatio = height / radius;

	MItSelectionList iter(selection, MFn::kNurbsCurve);
	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		curveFn.setObject(dagPath);

		double tStart, tEnd;
		curveFn.getKnotDomain(tStart, tEnd);

		MPoint pt;
		int i;
		double t;
		double tIncr = (tEnd - tStart) / (nPosts - 1);
		for (i = 0, t = tStart; i < nPosts; i++, t += tIncr)
		{
			curveFn.getPointAtParam(t, pt, MSpace::kWorld);
			pt.y += 0.5 * height;

			//MGlobal::executeCommand(MString("cylinder -pivot ") +
			dgMod.commandToExecute(MString("cylinder -pivot ") +
				pt.x + " " + pt.y + " " + pt.z + " -radius 0.5 -axis 0 1 0 -heightRatio " +
				heightRatio);
		}
	}

	//return MS::kSuccess;
	return redoIt();
}

MStatus Posts5Cmd::undoIt()
{
	//return MStatus();
	return dgMod.undoIt();
}

MStatus Posts5Cmd::redoIt()
{
	//return MStatus();
	return dgMod.doIt();
}

MSyntax Posts5Cmd::newSyntax()
{
	MSyntax syntax;
	syntax.addFlag(numberFlag, numberLongFlag, MSyntax::kLong);
	syntax.addFlag(radiusFlag, radiusLongFlag, MSyntax::kDouble);
	syntax.addFlag(heightFlag, heightLongFlag, MSyntax::kDouble);
	syntax.addFlag(helpFlag, helpLongFlag, MSyntax::kString);
	return syntax;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj, "jason.li", "0.3");
	MStatus stat;
	stat = pluginFn.registerCommand(cmdName, Posts5Cmd::creator, Posts5Cmd::newSyntax);
	if (!stat)
	{
		stat.perror("registerCommand failed.");
	}
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj);
	MStatus stat;
	stat = pluginFn.deregisterCommand(cmdName);
	if (!stat)
	{
		stat.perror("deregisterCommand failed.");
	}
	return stat;
}