#pragma once

#include <maya/MPxCommand.h>

namespace NeoBards
{
	class ThirdClass : public MPxCommand
	{
	public:
		ThirdClass();
		~ThirdClass();

		MStatus doIt(const MArgList& args);
		static void* creator();

	private:

	};

	ThirdClass::ThirdClass()
	{
	}

	ThirdClass::~ThirdClass()
	{
	}
}
