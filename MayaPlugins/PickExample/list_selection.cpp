#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>

DeclareSimpleCommand(pickExample, "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat", "2018.07.24.01");

MStatus pickExample::doIt(const MArgList& args)
{
	MDagPath node;
	MObject component;
	MSelectionList list;
	MFnDagNode nodeFn;
	
	MGlobal::getActiveSelectionList(list);
	for (unsigned int i = 0; i < list.length(); i++)
	{
		list.getDagPath(i, node, component);
		nodeFn.setObject(node);
		//cout << nodeFn.name().asChar() << " is selected." << endl;
		MString msg = nodeFn.name() + " is selected.";
		MGlobal::displayInfo(msg);
		cerr << msg << endl;
	}

	return MS::kSuccess;
}