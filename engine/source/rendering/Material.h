#pragma once
#include "Program.h"

struct Material
{
	std::unique_ptr<Program>& program;
};
