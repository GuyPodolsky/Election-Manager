#pragma once
#include "splitDistrictHeader.h"
#include "ProgramExceptions.h"
#include "saveNload.h"
#include <iostream>

namespace elc {

	class districtLoader {
	private:

		enum class disType { normal, split };

	public:
		static bool save(std::ostream& os, const district* dis);
		static district* load(std::istream& is);
	};

}