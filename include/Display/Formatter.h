#pragma once
#include <string>

struct Formatter : public std::numpunct<char>
{
	std::string do_grouping() const override { return "\3"; }
};
