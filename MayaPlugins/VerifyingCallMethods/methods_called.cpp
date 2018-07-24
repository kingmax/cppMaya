#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>

#define print(_msg) MGlobal::displayInfo(_msg)

class VerifyingCallMethods : public MPxCommand
{
public:
	VerifyingCallMethods();
	virtual ~VerifyingCallMethods();

	MStatus doIt(const MArgList&);
	MStatus redoIt();
	MStatus undoIt();
	bool isUndoable() const;

	static void* creator();
};

//constructor
VerifyingCallMethods::VerifyingCallMethods()
{
	//MGlobal::displayInfo("In VerifyingCallMethods::VerifyingCallMethods() constructor method.");
	print("In VerifyingCallMethods::VerifyingCallMethods() constructor method.");
}

//deconstructor
VerifyingCallMethods::~VerifyingCallMethods()
{
	print("In VerifyingCallMethods::~VerifyingCallMethods() deconstructor method.");
}

MStatus VerifyingCallMethods::doIt(const MArgList& args)
{
	print("In doIt()");
	return MS::kSuccess;
}

MStatus VerifyingCallMethods::redoIt() 
{ 
	print("in redoIt()"); 
	return MS::kSuccess;
}

MStatus VerifyingCallMethods::undoIt()
{
	print("in undoIt()");
	return MS::kSuccess;
}

bool VerifyingCallMethods::isUndoable() const
{
	print("in isUndoable()");
	return true;
}

//return a instance
void* VerifyingCallMethods::creator()
{
	cout << "in creator()";
	return new VerifyingCallMethods();
}

//register
MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat", "2018.07.24.01", "Any");
	plugin.registerCommand("commandDemo", VerifyingCallMethods::creator);
	print("in initializePlugin(MObject)");
	return MS::kSuccess;
}

//deregister
MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat", "2018.07.24.01", "Any");
	plugin.deregisterCommand("commandDemo");
	print("in uninitializePlugin(MObject)");
	return MS::kSuccess;
}