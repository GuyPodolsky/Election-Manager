#include "electionsLoaderHeader.h"

namespace elc {

	/*************************** LOADER ****************************/



	bool electionsLoader::save(std::ostream& os, const elections* elc)  //saves the election
	{
		elctType type;

		if (elc->getType() == 0)
			type = elctType::normal;
		else
			type = elctType::simple;

		os.write(reinterpret_cast<const char*>(&type), sizeof(type));
		if (os.fail())
			throw filesystem_exception("Write failure.");

		return elc->save(os);
	}


	elections* electionsLoader::load(std::istream& is)	//loads the election
	{
		elctType type;

		is.read(reinterpret_cast<char*>(&type), sizeof(type));
		if (is.fail())
			throw filesystem_exception("Read failure.");

		elections* newElc = nullptr;

		if (type == elctType::normal)
		{
			try {
				newElc = new elections();
			}
			catch (bad_alloc& ex)
			{
				throw allocation_failed(ex.what(), "Memory allocation for the election failed.");
			}
		}
		else
		{
			try {
				newElc = new simpleElections(1);	//value of repNum doesn't matter here because it is going to be loaded anyways...
			}
			catch (bad_alloc& ex)
			{
				throw allocation_failed(ex.what(), "Memory allocation for the election failed.");
			}
		}
		try {
			newElc->load(is);
		}
		catch (bad_alloc& ex)
		{
			delete newElc;
			throw allocation_failed(ex.what(), "Memory allocation for the election failed.");
		}
	
		return newElc;
	}

}