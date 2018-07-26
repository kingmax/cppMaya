#include <string>
//#include <iostream>
#include <maya/MIOStream.h>
#include <math.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>

class sine : public MPxNode
{
public:
	sine();
	virtual ~sine();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	static void* creator();
	static MStatus initialize();

	static MObject input;
	static MObject output;
	static MTypeId id;
};

MTypeId sine::id(0x80000);
MObject sine::input;
MObject sine::output;

sine::sine() {}
sine::~sine() {}

MStatus sine::compute(const MPlug & plug, MDataBlock & data)
{
	MStatus status;
	if (plug == output)
	{
		MDataHandle inputData = data.inputValue(input, &status);
		if (!status)
		{
			//std::cerr << "Error getting data" << std::endl;
			cerr << "Error getting data" << endl;
		}
		else
		{
			float result = sin(inputData.asFloat());
			MDataHandle outputHandle = data.outputValue(output);
			outputHandle.set(result);
			data.setClean(plug);
		}
	}
	return MS::kSuccess;
}

void* sine::creator()
{
	return new sine;
}

MStatus sine::initialize()
{
	MFnNumericAttribute nAttr;
	output = nAttr.create("output", "out", MFnNumericData::kFloat, 0.0);
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	input = nAttr.create("input", "in", MFnNumericData::kFloat, 0.0);
	nAttr.setStorable(true);
	addAttribute(input);
	attributeAffects(input, output);
	addAttribute(output);
	return MS::kSuccess;
}

//////////////////////////////////////////////////////////////////////////
MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "Simple DG Demo", "kingmax_res@163.com", "Any");
	status = plugin.registerNode("sine", sine::id, sine::creator, sine::initialize);
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);
	status = plugin.deregisterNode(sine::id);
	return status;
}