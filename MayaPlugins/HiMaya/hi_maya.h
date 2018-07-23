#pragma once

#ifndef __HI_MAYA_H__
#define __HI_MAYA_H__

#include <maya/MPxCommand.h>

//a very very basic Maya command;

class HiMaya : public MPxCommand
{
public:
	HiMaya() = default;

	//when call "HiMaya;" from mel or "Maya.cmds.HiMaya()" from python
	virtual MStatus doIt(const MArgList&);

	//called when the command is registered, return a pointer to a new instance of the class
	static void* creator();
};

#endif

