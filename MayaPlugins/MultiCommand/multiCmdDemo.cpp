#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MObject.h>

#include "third_class.h"

#define __VENDOR "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat"
#define __VERSION "2018.07.25.01"

namespace NeoBards
{
	class FirstClass : public MPxCommand
	{
	public:
		FirstClass();
		~FirstClass();

		MStatus doIt(const MArgList&);

		static void* creator();

	private:

	};

	FirstClass::FirstClass()
	{
		MGlobal::displayInfo("constructor");
	}

	FirstClass::~FirstClass()
	{
		MGlobal::displayInfo("deConstructor");
	}

	MStatus FirstClass::doIt(const MArgList &)
	{
		MGlobal::displayInfo("doIt");
		return MStatus();
	}

	void * FirstClass::creator()
	{
		return new FirstClass();
	}

	//////////////////////////////////////////////////////////////////////////
	class SecondClass : public MPxCommand
	{
	public:
		SecondClass() {};
		~SecondClass() {};
		MStatus doIt(const MArgList& args);
		static void* creator();
	protected:
	private:
	};

	MStatus SecondClass::doIt(const MArgList& args)
	{
		MGlobal::displayInfo("second class doIt");
		return MS::kSuccess;
	}

	void* SecondClass::creator()
	{
		return new SecondClass();
	}

	//////////////////////////////////////////////////////////////////////////
	
}

MStatus NeoBards::ThirdClass::doIt(const MArgList& args)
{
	MGlobal::displayInfo("third class doIt");
	return MS::kSuccess;
}

void* NeoBards::ThirdClass::creator() { return new ThirdClass(); }


MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, __VENDOR, __VERSION);

	MStatus status;

	status = plugin.registerCommand("mCmd1", NeoBards::FirstClass::creator);
	if (!status)
	{
		status.perror("register mCmd1 failed");
	}

	status = plugin.registerCommand("mCmd2", NeoBards::SecondClass::creator);
	if (!status)
	{
		status.perror("register mCmd2 failed");
	}
	
	status = plugin.registerCommand("mCmd3", NeoBards::ThirdClass::creator);
	if (!status)
	{
		status.perror("register mCmd3 failed");
	}

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, __VENDOR, __VERSION);

	MStatus status;

	plugin.deregisterCommand("mCmd1");
	
	plugin.deregisterCommand("mCmd2");

	status = plugin.deregisterCommand("mCmd3");
	if (!status)
	{
		status.perror("deregister mCmd3 failed");
	}

	return MS::kSuccess;
}