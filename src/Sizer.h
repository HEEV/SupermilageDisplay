#pragma once

#include "ISizeable.h"
#include "Constants.h"

class Sizer {
public:
	Sizer(uint width, uint height);


	ISizeable* resize(ISizeable& input, ISizeable* output);

private:
	uint _width;
	uint _height;
};