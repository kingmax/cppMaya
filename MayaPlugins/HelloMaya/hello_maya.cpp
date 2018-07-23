#include <maya/MSimple.h>
#include <maya/MGlobal.h>

DeclareSimpleCommand(HelloMaya, "kingmax_res@163.com | 184327932@qq.com | iJasonLee@WeChat", "2018.07.23.01");

MStatus HelloMaya::doIt(const MArgList& args)
{
	MGlobal::displayInfo("Hello Maya, the message from my first plugin. ÄãºÃ£¡");
	return MS::kSuccess;
}