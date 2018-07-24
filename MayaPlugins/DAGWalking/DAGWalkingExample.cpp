////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// Produces the MEL command "scanDagSyntax". 
//
// This command plug-in provides the same functionality as scanDagCmd except
// that the syntax parsing is performed using syntax objects.
//
// The command accepts several flags:
//
//	-b/-breadthFirst  : Perform breadth first search 
//	-d/-depthFirst    : Perform depth first search 
//	-c/-cameras       : Limit the scan to cameras
//	-l/-lights        : Limit the scan to lights
//	-n/-nurbsSurfaces : Limit the scan to NURBS surfaces
//
////////////////////////////////////////////////////////////////////////

#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MFnCamera.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnLight.h>
#include <maya/MColor.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MIOStream.h>

#define kBreadthFlag			"-b"
#define kBreadthFlagLong		"-breadthFirst"
#define kDepthFlag				"-d"
#define kDepthFlagLong			"-depthFirst"
#define kCameraFlag				"-c"
#define kCameraFlagLong			"-cameras"
#define kLightFlag				"-l"
#define kLightFlagLong			"-lights"
#define kNurbsSurfaceFlag		"-n"
#define kNurbsSurfaceFlagLong	"-nurbsSurfaces"
#define kQuietFlag				"-q"
#define kQuietFlagLong			"-quiet"

class scanDagSyntax : public MPxCommand
{
public:
	scanDagSyntax() {}
	virtual ~scanDagSyntax();
	
	static void* creator();
	static MSyntax newSyntax();
	virtual MStatus doIt(const MArgList&);

private:
	MStatus parseArgs(const MArgList& args, MItDag::TraversalType& traversalType, MFn::Type& filter, bool& quiet);
	MStatus doScan(const MItDag::TraversalType traversalType, MFn::Type filter, bool quiet);
	void printTransformData(const MDagPath& dagPath, bool quiet);
};

scanDagSyntax::~scanDagSyntax() {}

void* scanDagSyntax::creator()
{
	return new scanDagSyntax();
}

MSyntax scanDagSyntax::newSyntax()
{
	MSyntax syntax;
	syntax.addFlag(kBreadthFlag, kBreadthFlagLong);
	syntax.addFlag(kDepthFlag, kDepthFlagLong);
	syntax.addFlag(kCameraFlag, kCameraFlagLong);
	syntax.addFlag(kLightFlag, kLightFlagLong);
	syntax.addFlag(kNurbsSurfaceFlag, kNurbsSurfaceFlagLong);
	syntax.addFlag(kQuietFlag, kQuietFlagLong);
	return syntax;
}

MStatus scanDagSyntax::doIt(const MArgList& args)
{
	MItDag::TraversalType traversalType = MItDag::kDepthFirst;
	MFn::Type filter = MFn::kInvalid;
	MStatus status;
	bool quiet = false;
	status = parseArgs(args, traversalType, filter, quiet);
	if (!status)
	{
		return status;
	}
	return doScan(traversalType, filter, quiet);
}

MStatus scanDagSyntax::parseArgs(const MArgList & args, MItDag::TraversalType & traversalType, MFn::Type & filter, bool & quiet)
{
	MStatus status;

	MArgDatabase argData(syntax(), args);
	//MString arg;

	if (argData.isFlagSet(kBreadthFlag))
	{
		traversalType = MItDag::kBreadthFirst;
	}
	else if (argData.isFlagSet(kDepthFlag))
	{
		traversalType = MItDag::kDepthFirst;
	}

	if (argData.isFlagSet(kCameraFlag))
	{
		filter = MFn::kCamera;
	}
	else if (argData.isFlagSet(kLightFlag))
	{
		filter = MFn::kLight;
	}
	else if (argData.isFlagSet(kNurbsSurfaceFlag))
	{
		filter = MFn::kNurbsSurface;
	}

	if (argData.isFlagSet(kQuietFlag))
	{
		quiet = true;
	}

	return status;
}

MStatus scanDagSyntax::doScan(const MItDag::TraversalType traversalType, MFn::Type filter, bool quiet)
{
	MStatus status;
	MItDag dagIterator(traversalType, filter, &status);
	if (!status)
	{
		status.perror("MItDag constructor");
		return status;
	}

	if (traversalType == MItDag::kBreadthFirst)
	{
		if (!quiet)
		{
			displayInfo("Starting Breadth First scan of the DAG");
		}
	}
	else
	{
		if (!quiet)
		{
			displayInfo("Starting Depth First scan fo the DAG");
		}
	}

	switch (filter)
	{
		case MFn::kCamera:
			if (!quiet)
			{
				displayInfo(": Filtering for Cameras");
			}
			break;

		case MFn::kLight:
			if (!quiet)
			{
				displayInfo(": Filtering for Lights");
			}
			break;

		case MFn::kNurbsSurface:
			if (!quiet)
			{
				displayInfo(": Filtering for NurbsSurfaces");
			}
			break;
		default:
			break;
	}

	int objectCount = 0;
	for (; !dagIterator.isDone(); dagIterator.next())
	{
		MDagPath dagPath;
		status = dagIterator.getPath(dagPath);
		if (!status)
		{
			status.perror("MItDag::getPath");
			continue;
		}

		MFnDagNode dagNode(dagPath, &status);
		if (!status)
		{
			status.perror("MFnDagNode constructor");
			continue;
		}

		if (!quiet)
		{
			displayInfo(dagNode.name() + ": " + dagNode.typeName());
			displayInfo(" dagPath: " + dagPath.fullPathName());
		}
		objectCount++;

		//if the object is a camera
		if (dagPath.hasFn(MFn::kCamera))
		{
			MFnCamera camera(dagPath, &status);
			if (!status)
			{
				status.perror("MFnCamera constructor");
				continue;
			}
			//Get TRS
			printTransformData(dagPath, quiet);
			if (!quiet)
			{
				//displayInfo(" eyePoint: " + camera.eyePoint(MSpace::kWorld));
				cout << " eyePoint: "
					<< camera.eyePoint(MSpace::kWorld) << endl;
				cout << " upDirection: "
					<< camera.upDirection(MSpace::kWorld) << endl;
				cout << " viewDirection: "
					<< camera.viewDirection(MSpace::kWorld) << endl;
				cout << " aspectRatio: " << camera.aspectRatio() << endl;
				cout << " horizontalFilmAperture: "
					<< camera.horizontalFilmAperture() << endl;
				cout << " verticalFilmAperture: "
					<< camera.verticalFilmAperture() << endl;
			}
		}
		else if (dagPath.hasFn(MFn::kLight))
		{
			MFnLight light(dagPath, &status);
			if (!status) {
				status.perror("MFnLight constructor");
				continue;
			}
			// Get the translation/rotation/scale data
			printTransformData(dagPath, quiet);
			// Extract some interesting Light data
			MColor color;
			color = light.color();
			if (!quiet)
			{
				cout << " color: ["
					<< color.r << ", "
					<< color.g << ", "
					<< color.b << "]\n";
			}
			color = light.shadowColor();
			if (!quiet)
			{
				cout << " shadowColor: ["
					<< color.r << ", "
					<< color.g << ", "
					<< color.b << "]\n";
				cout << " intensity: " << light.intensity() << endl;
			}
		}
		else if (dagPath.hasFn(MFn::kNurbsSurface))
		{
			//Finally, if the object is a NURBS surface, surface specific information is output.
			MFnNurbsSurface surface(dagPath, &status);
			if (!status) {
				status.perror("MFnNurbsSurface constructor");
				continue;
			}
			// Get the translation/rotation/scale data
			printTransformData(dagPath, quiet);
			// Extract some interesting Surface data
			if (!quiet)
			{
				cout << " numCVs: "
					<< surface.numCVsInU()
					<< " * "
					<< surface.numCVsInV()
					<< endl;
				cout << " numKnots: "
					<< surface.numKnotsInU()
					<< " * "
					<< surface.numKnotsInV()
					<< endl;
				cout << " numSpans: "
					<< surface.numSpansInU()
					<< " * "
					<< surface.numSpansInV()
					<< endl;
			}
		}
		else
		{
			printTransformData(dagPath, quiet);
		}
	} //end for

	if (!quiet)
	{
		cout.flush();
	}

	setResult(objectCount); //return to mel

	return MS::kSuccess;
}

void scanDagSyntax::printTransformData(const MDagPath & dagPath, bool quiet)
{
	//This method simply determines the transformation information on the DAG node and prints it out.
	MStatus status;
	MObject transformNode = dagPath.transform(&status);
	if (!status && status.statusCode() == MS::kInvalidParameter)
	{
		return;
	}

	MFnDagNode transform(transformNode, &status);
	if (!status)
	{
		status.perror("MFnDagNode constructor");
		return;
	}

	MTransformationMatrix matrix(transform.transformationMatrix());
	if (!quiet)
	{
		cout << " translation: " << matrix.translation(MSpace::kWorld) << endl;
	}

	double threeDoubles[3];
	MTransformationMatrix::RotationOrder rOrder;
	matrix.getRotation(threeDoubles, rOrder, MSpace::kWorld);
	if (!quiet)
	{
		cout << " rotation: ["
			<< threeDoubles[0] << ", "
			<< threeDoubles[1] << ", "
			<< threeDoubles[2] << "]\n";
	}

	matrix.getScale(threeDoubles, MSpace::kWorld);
	if (!quiet)
	{
		cout << " scale: ["
			<< threeDoubles[0] << ", "
			<< threeDoubles[1] << ", "
			<< threeDoubles[2] << "]\n";
	}
}

///////////////////////////////////////////////////////
MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat", "2018.07.24.01", "Any");
	status = plugin.registerCommand("scanDagSyntax", scanDagSyntax::creator, scanDagSyntax::newSyntax);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);
	status = plugin.deregisterCommand("scanDagSyntax");
	return status;
}