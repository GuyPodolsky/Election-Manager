#include "civilianHeader.h"
#include "districtHeader.h"
#include "partyHeader.h"
#include "saveNload.h"
#include <iostream>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>


namespace elc {

	civilian::civilian() : civilian("", 100000000, 0, nullptr)
	{ 
	}

	civilian::civilian(const std::string& _name, unsigned int _id, unsigned int _birthYear, district* _dis) :
		name(_name),
		birthYear(_birthYear),
		homeDis(_dis),
		repParty(nullptr),
		repDis(nullptr),
		voteParty(nullptr)
	{
		id = _id;
		int numOfDigits = 0;
		while (_id != 0)
		{
			_id /= 10;
			++numOfDigits;
		}
		if (numOfDigits != validIDLength)
			throw invalid_input("ID number is to short.");
	}


	civilian::~civilian()
	{
	}


	std::ostream& operator<<(std::ostream& os, const civilian& civ)
	{
		os << "Name: " << civ.name << ", ID: " << civ.id << ", Year of birth: " << civ.birthYear;

		return os;
	}


	bool civilian::save(std::ostream& os) const
	{
		saveNload::save_string(os, name);

		saveNload::save_basic_type(os, id);

		saveNload::save_basic_type(os, birthYear);

		return true;
	}


	bool civilian::load(std::istream& is)
	{
		saveNload::load_string(is, name);

		saveNload::load_basic_type(is, id);

		saveNload::load_basic_type(is, birthYear);

		return true;
	}


	const district* civilian::getHomeDistrict() const { return homeDis; }
	district* civilian::getHomeDistrict() { return homeDis; }

	const party* civilian::getRepParty() const { return repParty; }
	party* civilian::getRepParty() { return repParty; }

	const district* civilian::getRepDis() const { return repDis; }
	district* civilian::getRepDis() { return repDis; }
	
	const party* civilian::getVoteParty() const { return voteParty; }
	party* civilian::getVoteParty() { return voteParty; }

	bool civilian::setRepresentive(party* _repParty, district* _repDis)
	{
		repParty = _repParty;
		repDis = _repDis;

		return true;
	}


	bool civilian::setVote(party* _voteParty)
	{
		voteParty = _voteParty;

		return true;
	}
	
}