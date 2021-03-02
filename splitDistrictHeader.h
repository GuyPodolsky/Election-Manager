#pragma once
#include "districtHeader.h"
#include <iostream>

namespace elc {

	class splitDistrict : public district {
	public:
		splitDistrict();
		splitDistrict(const std::string& _name, unsigned int _repNum);
		virtual ~splitDistrict() = default;

		splitDistrict(const splitDistrict&) = delete;
		splitDistrict& operator=(const splitDistrict&) = delete;
		const int getType() const override { return 1; }
	};

}