#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MFnTransform.h>
#include <maya/MItSelectionList.h>
#include <maya/MFn.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>

const char *timeFlag = "-t", *timeLongFlag = "-time";
const double TWOPI = 3.1415926 * 2;
const char *cmdName = "clock";

class ClockCmd : public MPxCommand
{
public:
	virtual MStatus doIt(const MArgList &args);
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable();
	
	static void * creator() { return new ClockCmd; }
	static MSyntax newSyntax();

private:
	bool isQuery;
	int prevTime, newTime;
	MDagPath hourHandPath, minuteHandPath;
	int getTime();
	void setTime(const int time);
};

MStatus ClockCmd::doIt(const MArgList & args)
{
	MStatus stat;
	MArgDatabase argData(syntax(), args, &stat);
	if (!stat)
	{
		return stat;
	}
	isQuery = argData.isQuery();

	if (argData.isFlagSet(timeFlag) && !isQuery)
	{
		argData.getFlagArgument(timeFlag, 0, newTime);
	}

	// get a list of currently selected objects
	MSelectionList selection;
	MGlobal::getActiveSelectionList(selection);
	
	MDagPath dagPath;
	MFnTransform transformFn;
	MString name;

	// Iterate over the transforms
	MItSelectionList iter(selection, MFn::kTransform);
	for (; !iter.isDone(); iter.next())
	{
		iter.getDagPath(dagPath);
		transformFn.setObject(dagPath);
		
		name = transformFn.name();
		if (name == MString("hour_hand"))
		{
			hourHandPath = dagPath;
		}
		else
		{
			if (name == MString("minute_hand"))
			{
				minuteHandPath = dagPath;
			}
		}
	}

	// Neither hour nor minute hand selected
	if (!hourHandPath.isValid() || !minuteHandPath.isValid())
	{
		MGlobal::displayError("Select hour and minute hands");
		return MS::kFailure;
	}

	prevTime = getTime();

	if (isQuery)
	{
		setResult(prevTime);
		return MS::kSuccess;
	}

	// if edit mode
	return redoIt();
}

MStatus ClockCmd::undoIt()
{
	//return MStatus();
	setTime(prevTime);
	return MS::kSuccess;
}

MStatus ClockCmd::redoIt()
{
	//return MStatus();
	setTime(newTime);
	return MS::kSuccess;
}

bool ClockCmd::isUndoable()
{
	//return true;
	return isQuery ? false : true;
}

MSyntax ClockCmd::newSyntax()
{
	MSyntax syntax;
	syntax.addFlag(timeFlag, timeLongFlag, MSyntax::kLong);
	syntax.enableQuery(true);
	syntax.enableEdit(true);
	return syntax;
}

int ClockCmd::getTime()
{
	// Get the time from the rotation
	MFnTransform transformFn;
	transformFn.setObject(hourHandPath);
	MEulerRotation rot;
	transformFn.getRotation(rot);

	// Determine the time and format it
	int a = int( -rot.y * (1200.0 / TWOPI));
	int time = (a / 100 * 100) + int(floor((a % 100) * (6.0 / 10.0) + 0.5)); ;
	return time;
}

void ClockCmd::setTime(const int time)
{
	MFnTransform transformFn;
	
	// Calculate the hour and minutes
	int hour = (time / 100) % 12;
	int minutes = time % 100;

	// Rotate the hour hand by the required amount
	transformFn.setObject(hourHandPath);
	transformFn.setRotation(MEulerRotation(MVector(0.0, hour * (-TWOPI / 12) + minutes * (-TWOPI / 720.0), 0)));

	// Rotate the minute hand by the required amount
	transformFn.setObject(minuteHandPath);
	transformFn.setRotation(MEulerRotation(MVector(0.0, minutes * (-TWOPI / 60.0), 0)));
}

MStatus initializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj, "jason.li", "0.1");
	MStatus stat;
	stat = pluginFn.registerCommand(cmdName, ClockCmd::creator, ClockCmd::newSyntax);
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