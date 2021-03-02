#include "partyHeader.h"
#include "districtHeader.h"
#include "civilianHeader.h"
#include "saveNload.h"

namespace elc {


	unsigned int party::serial_generator = 0;


	party::party(const std::string& _name, civilian* _head) :
		serial(++serial_generator),
		name(_name),
		head(_head),
		representors()
	{
	}


	party::~party()
	{
	}


	void party::addRep(civilian* rep, district* repDis)		
	{
		auto found = representors.find(repDis->getSerial());

		if (found == representors.end())			
		{
			dynamicArray<civilian*> tempArr;
			tempArr.push_back(rep);

			representors.insert({ repDis->getSerial(),tempArr });
		}
		else
			found->second.push_back(rep);
	}


	void party::reset_serial_generator()
	{
		serial_generator = 0;
	}


	dynamicArray<civilian*>* party::get_potential_representors_of_district(const district* dis)			//CHANGE
	{
		/*int ind = representors.find_dis(*dis);

		if (ind == disRepsArray::not_found)
			return nullptr;
		else
			return &representors[ind]->getRepArray();*/

		auto itr = representors.find(dis->getSerial());

		if (itr == representors.end())
			return nullptr;
		else
			return &(itr->second);
	}


	std::ostream& operator<<(std::ostream& os, const party& pty)		//CHANGE
	{
		os << "Party serial number is: " << pty.serial << ", Party's name: \"" << pty.name << "\", The head of the party is: " << pty.head->getName() << std::endl;
		return os;
	}


	bool party::save(std::ostream& os) const
	{
		os.write(reinterpret_cast<const char*>(&serial_generator), sizeof(serial_generator));
		if (os.fail())
			throw filesystem_exception("Write failure.");

		os.write(reinterpret_cast<const char*>(&serial), sizeof(serial));
		if (os.fail())
			throw filesystem_exception("Write failure.");

		saveNload::save_string(os, name);

		saveNload::save_representatives_per_district_map(os, representors);

		return true;
	}


	bool party::load(std::istream& is)
	{
		is.read(reinterpret_cast<char*>(&serial_generator), sizeof(serial_generator));
		if (is.fail())
			throw filesystem_exception("Read failure.");

		is.read(reinterpret_cast<char*>(&serial), sizeof(serial));
		if (is.fail())
			throw filesystem_exception("Read failure.");

		saveNload::load_string(is, name);

		saveNload::load_representatives_per_district_map(is, representors);

		return true;
	}



}