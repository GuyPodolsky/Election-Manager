#include "splitDistrictHeader.h"

namespace elc {

	/*********************** SIMPLE DISTRICT *************************/


	splitDistrict::splitDistrict() : district()
	{
	}


	splitDistrict::splitDistrict(const std::string& _name, unsigned int _repNum) : district(_name, _repNum)
	{
	}

}