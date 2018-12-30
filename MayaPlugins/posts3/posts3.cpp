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

//Syntax string definitions
const char *numberFlag = "-n", *numberLongFlag = "-number";
const char *radiusFlag = "-r", *radiusLongFlag = "-radius";
const char *heightFlag = "-h", *heightLongFlag = "-height";

class Posts3Cmd : public MPxCommand
{
public:
	static void * creator()
	{
		return new Posts3Cmd;
	}

	virtual MStatus doIt(const MArgList &args);

	static MSyntax newSyntax();
private:

};

MStatus Posts3Cmd::doIt(const MArgList & args)
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

			MGlobal::executeCommand(MString("cylinder -pivot ") +
				pt.x + " " + pt.y + " " + pt.z + " -radius 0.5 -axis 0 1 0 -heightRatio " +
				heightRatio);
		}
	}

	return MS::kSuccess;
}

MSyntax Posts3Cmd::newSyntax()
{
	MSyntax syntax;
	syntax.addFlag(numberFlag, numberLongFlag, MSyntax::kLong);
	syntax.addFlag(radiusFlag, radiusLongFlag, MSyntax::kDouble);
	syntax.addFlag(heightFlag, heightLongFlag, MSyntax::kDouble);
	return syntax;
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj, "jason.li", "0.3");
	MStatus stat;
	stat = pluginFn.registerCommand("posts3", Posts3Cmd::creator, Posts3Cmd::newSyntax);
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
	stat = pluginFn.deregisterCommand("posts3");
	if (!stat)
	{
		stat.perror("deregisterCommand failed.");
	}
	return stat;
}