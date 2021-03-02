#pragma once
#include <iostream>
#include <string>
#include <map>
#include "ProgramExceptions.h"

namespace elc {

	class civilian;
	class district;
	class party;
	template<typename T> class dynamicArray;

	class saveNload {
	public:

		static void save_string(std::ostream& os, const std::string& str);
		static void load_string(std::istream& is, std::string& str);


		static void save_representatives_per_district_map(std::ostream& os, const std::map<int, dynamicArray<civilian*>>& districtReps);
		static void load_representatives_per_district_map(std::istream& is, std::map<int, dynamicArray<civilian*>>& districtReps);


		static void save_votes_per_party_map(std::ostream& os, const std::map<int, int>& votesPerParty);
		static void load_votes_per_party_map(std::istream& is, std::map<int, int>& votesPerParty);


		template<typename T>
		static void save_basic_type(std::ostream& os, T basic_type)						//does not need ref and const because its meant to be used on
		{																				//basic variables, like int, double, bool...
			os.write(reinterpret_cast<const char*>(&basic_type), sizeof(basic_type));
			if (os.fail())
				throw filesystem_exception("Write failure.");
		}

		template<typename T>
		static void load_basic_type(std::istream& is, T& basic_type)					//meant to be used on basic variables, like int, double, bool...
		{
			is.read(reinterpret_cast<char*>(&basic_type), sizeof(basic_type));
			if (is.fail())
				throw filesystem_exception("Read failure.");
		}
		
	};

}

