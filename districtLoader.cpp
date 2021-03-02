#include "districtLoaderHeader.h"

using namespace std;
namespace elc {



	bool districtLoader::save(std::ostream& os, const district* dis)
	{
		disType districtType;
		if (dis->getType() == 0)
			districtType = disType::normal;
		else
			districtType = disType::split;

		saveNload::save_basic_type(os, districtType);

		return dis->save(os);
	}


	district* districtLoader::load(std::istream& is)
	{
		disType districtType;

		saveNload::load_basic_type(is, districtType);

		district* newDis = nullptr;
		if (districtType == disType::normal)
		{
			try {
				newDis = new district();
			}
			catch (bad_alloc& ex)
			{
				throw allocation_failed(ex.what(), "Memory allocation for the district failed.");
			}
		}
		else
		{
			try {
				newDis = new splitDistrict();
			}
			catch (bad_alloc& ex)
			{
				throw allocation_failed(ex.what(), "Memory allocation for the district failed.");
			}
		}

		try {
			newDis->load(is);
		}
		catch (bad_alloc& ex)
		{
			delete newDis;
			throw allocation_failed(ex.what(), "Memory allocation for the district failed.");
		}
			
		return newDis;
	}

}