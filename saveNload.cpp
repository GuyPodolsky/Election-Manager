#include "saveNload.h"
#include "dynamicArray.h"

namespace elc {

	void saveNload::save_string(std::ostream& os, const std::string& str)
	{
		size_t sizeOfString = str.size();

		os.write(reinterpret_cast<const char*>(&sizeOfString), sizeof(sizeOfString));
		if (os.fail())
			throw filesystem_exception("Write failure.");

		for (auto i = str.begin(); i < str.end(); ++i)
		{
			os.write(reinterpret_cast<const char*>(&(*i)), sizeof(*i));
			if (os.fail())
				throw filesystem_exception("Write failure.");
		}
	}


	void saveNload::load_string(std::istream& is, std::string& str)
	{
		size_t sizeOfString;
		is.read(reinterpret_cast<char*>(&sizeOfString), sizeof(sizeOfString));
		if(is.fail())
			throw filesystem_exception("Read failure.");

		str.reserve(sizeOfString);

		for (int i = 0; i < sizeOfString; ++i)
		{
			char nextCh;
			is.read(reinterpret_cast<char*>(&nextCh), sizeof(nextCh));
			if (is.fail())
				throw filesystem_exception("Read failure.");

			str.push_back(nextCh);
		}
	}


	void saveNload::save_representatives_per_district_map(std::ostream& os, const std::map<int, dynamicArray<civilian*>>& districtReps)
	{
		saveNload::save_basic_type<size_t>(os, districtReps.size());

		for (auto itr : districtReps)		//save the size and length of the arrays and the sirial of the district they are attached to
		{
			saveNload::save_basic_type<int>(os, itr.first);
			itr.second.save(os);
		}
	}


	void saveNload::load_representatives_per_district_map(std::istream& is, std::map<int, dynamicArray<civilian*>>& districtReps)
	{
		size_t size;
		saveNload::load_basic_type<size_t>(is, size);

		for (size_t i = 0; i < size; ++i)
		{
			int serial;

			saveNload::load_basic_type(is, serial);
			dynamicArray<civilian*> tempArr;
			tempArr.load(is);

			districtReps.insert({ serial,tempArr });
		}
	}


	void saveNload::save_votes_per_party_map(std::ostream& os, const std::map<int, int>& votesPerParty)
	{
		saveNload::save_basic_type<size_t>(os, votesPerParty.size());

		for (auto& itr : votesPerParty)				//save the sirial of the party and the number of votes
		{
			saveNload::save_basic_type<int>(os, itr.first);
			saveNload::save_basic_type<int>(os, itr.second);
		}
	}


	void saveNload::load_votes_per_party_map(std::istream& is, std::map<int, int>& votesPerParty)
	{
		size_t size;
		saveNload::load_basic_type<size_t>(is, size);

		for (size_t i = 0; i < size; ++i)
		{
			int serial, votes;

			saveNload::load_basic_type(is, serial);
			saveNload::load_basic_type(is, votes);

			votesPerParty.insert({ serial,votes });
		}
	}

}