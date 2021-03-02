#pragma once
#include "simpleElectionsHeader.h"
#include <iostream>

namespace elc {

	class electionsLoader {
	private:

		enum class elctType { normal, simple };

	public:

		static bool save(std::ostream& os, const elections* elc);
		static elections* load(std::istream& is);

	};

}