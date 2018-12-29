#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MItSelectionList.h>
#include <maya/MFn.h>
#include <maya/MPoint.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

class PostsCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList&);
	static void *creator()
	{
		return new PostsCmd;
	}
};

MStatus PostsCmd::doIt(const MArgList &)
{
	const int nPosts = 5;
	const double radius = 0.5;
	const double height = 5.0;

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


MStatus initializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj, "jason.li", "1.0");
	MStatus stat;
	stat = pluginFn.registerCommand("posts1", PostsCmd::creator);
	if (!stat)
	{
		stat.perror("registerCommand failed.");
	}
	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plgFn(obj);
	MStatus stat;
	stat = plgFn.deregisterCommand("posts1");
	if (!stat)
	{
		stat.perror("deregisterCommand failed");
	}
	return stat;
}