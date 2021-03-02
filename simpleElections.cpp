#include "simpleElectionsHeader.h"

namespace elc {

	/*************************** SIMPLE ELECTIONS ****************************/


	simpleElections::simpleElections(int representativesNumber, unsigned int _day, unsigned int _month, unsigned int _year) :
		elections(_day, _month, _year)
	{
		elections::set_new_district("simple elections hidden district", representativesNumber, elections::district_type_split);
	}


	bool simpleElections::set_new_civilian(const std::string& civName, unsigned int civId, unsigned int civBirthYear, district* civHomeDis)
	{
		civilian* newCiv;
		try
		{
			newCiv = new civilian(civName, civId, civBirthYear, districts[0]);
		}
		catch (bad_alloc& ex)
		{
			throw  allocation_failed(ex.what(), "Memory allocation for the civilian failed.");
		}

		try {
			civBook.push_back(newCiv);						//updates civBook
		}
		catch (allocation_failed &ex)
		{
			delete[] newCiv;
			throw allocation_failed(ex.what(), ex.Message());
		}
		try
		{
			newCiv->getHomeDistrict()->addCiv(newCiv);		//updates districts
		}
		catch (allocation_failed& ex)
		{
			delete[] newCiv;
			throw allocation_failed(ex.what(), ex.Message());
		}
		return true;
	}


	bool simpleElections::set_new_district(const std::string& disName, unsigned int disRepNum, int type)
	{
		return true;
	}


	bool simpleElections::set_civilian_as_representative(civilian* civ, party* pty, district* dis)
	{
		civ->setRepresentive(pty, districts[0]);		//updates civBook
		pty->addRep(civ, districts[0]);					//updates the party
		return true;
	}


	void simpleElections::print_districts()
	{
		std::cout << "The whole state is a one big district."<< std::endl;
		return;
	}


	void simpleElections::print_parties()
	{
		if (parties.size() == 0)
			std::cout << "There are no parties!" << std::endl;
		for (int i = 0; i < parties.size(); ++i)
		{
			std::cout << "Party serial number is: " << parties[i]->getSerial() << ", Party's name: \"" << parties[i]->getName()
				<< "\", The head of the party is: " << parties[i]->getHead()->getName() << std::endl;
			
			std::map<int, dynamicArray<civilian*>>& repOfHiddenDis = parties[i]->getDisRepresentors();

			if (!repOfHiddenDis.empty())
			{
				std::cout << "The representatives are: " << std::endl;

				for (auto& itr : repOfHiddenDis.begin()->second)
					std::cout << *(itr) << std::endl;
			}
			else
				std::cout << "There are no representatives." << std::endl;
		}
	}


	void simpleElections::print_elections()
	{
		std::cout << "The election results are: " << std::endl;
		std::cout << "The number of representatives: " << districts[0]->getRepNum() << std::endl;

		int totalVotesOfElection = districts[0]->get_total_votes();

		std::cout << "The total number of votes is: " << totalVotesOfElection << std::endl;
		std::cout << "The percentage of votes is: " <<
			(static_cast<double>(totalVotesOfElection) / static_cast<double>(districts[0]->getCivOfDis().size())) * 100<< "\%" << std::endl;

		elections::print_district_elected_reps(districts[0]);

		elections::print_finale_results();
	}

}