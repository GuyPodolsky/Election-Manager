#include "districtHeader.h"
#include "partyHeader.h"
#include "saveNload.h"

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>


namespace elc {


	unsigned int district::serial_generator = 0;
	

	district::district() : district("", 1)
	{
	}


	district::district(const std::string& _name, unsigned int _repNum) : 
		serial(++serial_generator),
		name(_name),
		civOfDis(0),
		elcRes(0),
		votesMap(),
		winningParty(nullptr)
	{
		if (_repNum > 0)
			repNum = _repNum;
		else
			throw invalid_input("Invalid number of representatives.");
	}


	district::~district()
	{
	}


	void district::addCiv(civilian* civ)  //here?
	{
		civOfDis.push_back(civ);
	}


	void district::reset_serial_generator()
	{
		serial_generator = 0;
	}


	double district::percentOfVoters() const
	{
		
		int countVoters = 0;
		int numOfCiv = civOfDis.size();

		if (numOfCiv == 0)
			return 0;
		else
		{
			for (int i = 0; i < numOfCiv; ++i)
				if (civOfDis[i]->getVoteParty() != nullptr)
					++countVoters;

			return ((static_cast<double>(countVoters) / static_cast<double>(numOfCiv)) * 100.00);
		}
	}


	int district::get_total_votes() const
	{
		int total = 0;
		for (auto& itr : votesMap)
			total += itr.second;

		return total;
	}


	std::ostream& operator<<(std::ostream& os, const district& dis)
	{
		os << "District serial number: " << dis.serial << ", district type: " << (dis.getType() == 0 ? "normal" : "split")
			<< ", District's name is: \"" << dis.name << "\", The number of representatives is: " << dis.repNum;

		return os;
	}


	bool district::save(std::ostream& os) const
	{

		saveNload::save_basic_type(os, serial_generator);

		saveNload::save_basic_type(os, serial);

		saveNload::save_string(os, name);

		civOfDis.save(os);

		elcRes.save(os);

		saveNload::save_votes_per_party_map(os, votesMap);

		saveNload::save_basic_type(os, repNum);

		return true;
	}


	bool district::load(std::istream& is)
	{

		saveNload::load_basic_type(is, serial_generator);

		unsigned int& loadSerial = const_cast<unsigned int&>(serial);

		saveNload::load_basic_type(is, loadSerial);

		saveNload::load_string(is, name);

		civOfDis.load(is);

		elcRes.load(is);

		saveNload::load_votes_per_party_map(is, votesMap);

		saveNload::load_basic_type(is, repNum);

		return true;
	}

}