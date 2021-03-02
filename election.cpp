#include "electionHeader.h"
#include "districtLoaderHeader.h"
#include "splitDistrictHeader.h"
#include <typeinfo>
#include <algorithm>

using namespace std;

namespace elc {
	bool elections::election_in_district(district* dis)
	{
		int civBookInd = 0;
		map<int, int> &disVotesMap = dis->getVotesMap();
		disVotesMap.clear();																	//empty all parties and votes if there were elections before

		int countOfTotalVotes = 0;

		//run while civilians are from district dis
		while (civBookInd < civBook.size() && civBook[civBookInd]->getHomeDistrict() != dis)
			++civBookInd;
		while (civBookInd < civBook.size() && civBook[civBookInd]->getHomeDistrict() == dis)
		{
			if (civBook[civBookInd]->did_vote())
			{
				totalVotes[civBook[civBookInd]->getVoteParty()->getSerial()] += 1;				//add a vote for the right party in total votes
				disVotesMap[civBook[civBookInd]->getVoteParty()->getSerial()] += 1;				//and in disVotesArr
				++countOfTotalVotes;
			}
			++civBookInd;
		}

		if (countOfTotalVotes == 0)																//in case there is no civilian in the district that has voted
		{
			string what_arg = dis->getName();
			what_arg += " district didn't vote at all.";
			throw elections_exception(what_arg);		
		}
		else
		{
			const int disRepNum = static_cast<int>(dis->getRepNum());
			dynamicArray<civilian*>& disElectArr = dis->getElcRes();							//get the elected civilian array from within the district
			disElectArr.clear();																//empty all representatives if there were elections before

			
			for (auto& itr : disVotesMap)
			{
				//get the array of potential representatives from the party
				dynamicArray<civilian*>* potentialRepArr = find_party_by_serial(itr.first)->get_potential_representors_of_district(dis);
				
				//calculate the percent of the votes that this party got
				const double percentOfVotes = static_cast<double>(itr.second) / static_cast<double>(countOfTotalVotes);
	
				//calculate the number of representatives the party is getting
				int numOfRepForParty = percentOfVotes * disRepNum;
				
				if ((percentOfVotes * disRepNum) - static_cast<double>(numOfRepForParty) >= 0.5)
					numOfRepForParty++;
				
				for (int i = 0; i < numOfRepForParty; ++i)
				{
					if (potentialRepArr != nullptr && i < potentialRepArr->size())
						disElectArr.push_back((*potentialRepArr)[i]);
					else
						throw elections_exception("One or more of the parties doesn't have enough representatives in one or more of the districts.");
				}
			}
			//deletes the unnecessary representatives.
			while (disRepNum < dis->getElcRes().size())
				dis->getElcRes().pop_back();

			disElectArr.shrink_to_fit();


			bool first = true;
			int maxSerial, maxVotes;										//TODO: should we conduct a check?

			for (auto& itr : disVotesMap)
			{
				if (first)
				{
					maxSerial = itr.first;
					maxVotes = itr.second;
					first = false;
				}
				else if (maxVotes > itr.second)
				{
					maxSerial = itr.first;
					maxVotes = itr.second;
				}
			}

			dis->setWinningParty(find_party_by_serial(maxSerial));
			if (dis->getWinningParty() == nullptr)
				throw item_not_found("Party wasn't found");

			return true;
		}
	}


	elections::elections(unsigned int _day, unsigned int _month, unsigned int _year) : civBook(0), districts(0), parties(0), totalVotes()
	{
        if (_year <= 0)									//checks the validness of the year.
            throw invalid_input("Invalid year.");

        if (_month >= 1 && _month <= 12)					//checks the validness of the month.
        {												//in case the month contains 31 days.
            if (static_cast<months>(_month) == months::Jan || static_cast<months>(_month) == months::Mar || static_cast<months>(_month) == months::May || static_cast<months>(_month) == months::Jul|| static_cast<months>(_month) == months::Aug|| static_cast<months>(_month) == months::Oct|| static_cast<months>(_month) == months::Dec)
            {
                if (_day < 1 || _day > 31)				//checks the validness of the day.
                    throw invalid_input("Invalid days in the month (should be between 1 to 31).");
            }											//in case the month contains 30 days.
            else if (static_cast<months>(_month) == months::Apr || static_cast<months>(_month) == months::Jun || static_cast<months>(_month) == months::Sep || static_cast<months>(_month) == months::Nov)
            {
                if (_day < 1 || _day > 30)				//checks the validness of the day.
                    throw invalid_input("Invalid day in the month (should be between 1 to 30).");
            }
            else										//in case the month is February.
            {
                if (_year % 4 == 0)						//in case this is a leap year.
                {
                    if (_day < 1 || _day > 29)			//checks the validness of the day.
                        throw invalid_input("Invalid day in the month (should between 1 to 29).");
                }
                else									//in case this isn't a leap year.
                {
                    if (_day < 1 || _day > 28)			//checks the validness of the day.
                        throw invalid_input("Invalid day in the month (should be between 1 to 28).");
                }
            }
        }
        else
            throw invalid_input("Invalid month (should be between 1 to 12).");

        year=_year;
        month=_month;
        day=_day;
	}


	elections::~elections()
	{
		for (int i = 0; i < civBook.size(); ++i)
			delete civBook[i];

		for (int i = 0; i < districts.size(); ++i)
			delete districts[i];

		for (int i = 0; i < parties.size(); ++i)
			delete parties[i];
	}


	bool elections::set_new_civilian(const std::string& civName, unsigned int civId, unsigned int civBirthYear, district* civHomeDis)
	{
		civilian* newCiv;
		try {
			newCiv = new civilian(civName, civId, civBirthYear, civHomeDis);
		}
		catch (bad_alloc& ex)
		{
			throw allocation_failed(ex.what(), "Memory allocation for the civilian failed.");
		}

		try
		{
			civBook.push_back(newCiv);						//update civBook
		}
		catch (allocation_failed& ex)
		{
			delete[] newCiv;
			throw allocation_failed(ex.what(), ex.Message());
		}

		try {
			newCiv->getHomeDistrict()->addCiv(newCiv);		//update districts
		}
		catch (allocation_failed& ex)
		{
			delete[] newCiv;
			throw allocation_failed(ex.what(), ex.Message());
		}

		return true;
	}


	bool elections::set_new_district(const std::string& disName, unsigned int disRepNum, int type)
	{	
		district* newDis;

		if (type == district_type_normal)
		{
			try {
				newDis = new district(disName, disRepNum);
			}
			catch (bad_alloc& ex)
			{
				throw allocation_failed(ex.what(),"Memory allocation for the district failed.");
			}
		}
		else {
			try{
				newDis = new splitDistrict(disName, disRepNum);
			}
			catch (bad_alloc& ex)
			{
				cout << "Memory allocation for the district failed." << endl;
				exit(EXIT_FAILURE);
			}
		}
		
		districts.push_back(newDis);				//updates districts
		return true;
	}


	bool elections::set_new_party(const std::string& ptyName, civilian* ptyHead)
	{
		party* newPty;
		try {
			newPty = new party(ptyName, ptyHead);
		}
		catch (bad_alloc& ex)
		{
			throw allocation_failed(ex.what(),"Memory allocation for the party failed.");
		}

		parties.push_back(newPty);					//update parties
		newPty->getHead()->setRepresentive(newPty);		//update the head of the party

		return true;
	}


	bool elections::set_civilian_as_representative(civilian* civ, party* pty, district* dis)
	{
		civ->setRepresentive(pty, dis);		//update civBook
		pty->addRep(civ, dis);				//update the party
		return true;
	}


	void elections::print_civilians()
	{
		if (civBook.empty())
			throw invalid_command("There are no civilians.");

		for(civilian* civ : civBook)
			cout << *civ << endl;
	}


	void elections::print_districts()
	{
		if (districts.empty())
			throw invalid_command("There are no district.");

		for (district* dis : districts)
			cout << *dis << endl;
	}
	

	void elections::print_parties()
	{
		if (parties.empty())
			throw invalid_command("There are no parties.");

		for (party* pty : parties)
		{
			cout << *pty;

			for (auto& itr : pty->getDisRepresentors())
			{
				cout << "representatives of district " << find_district_by_serial(itr.first)->getName() << ":" << endl;

				for (auto& civItr : itr.second)
					std::cout << *civItr << std::endl;

				cout << endl;
			}
			cout << endl;
		}
	}

	
	bool elections::save(std::ostream& os) const
	{

		//save the date and the size of the arrays

		saveNload::save_basic_type<unsigned int>(os, day);
		saveNload::save_basic_type<unsigned int>(os, month);
		saveNload::save_basic_type<unsigned int>(os, year);

		//save the "hard" values of the classes - without pointers

		civBook.save(os);

		districts.save(os);

		parties.save(os);

		saveNload::save_votes_per_party_map(os, totalVotes);

		//save the "hard" values of the civilians, districts and parties which are inside the arrays

		for (int i = 0; i < civBook.size(); ++i)
			if (!civBook[i]->save(os))
				throw "An unknown error occurred in civilian save.";

		for (int i = 0; i < districts.size(); ++i)
			if (!districtLoader::save(os, districts[i]))
				throw "An unknown error occurred in district save.";
			
		for (int i = 0; i < parties.size(); ++i)
			if (!parties[i]->save(os))
				throw "An unknown error occurred in party save.";

		//save the indexes of everthing inside the civilians, districts and parties, so that when we call load we can connect them back together

		for (int i = 0; i < civBook.size(); ++i)
			if (!save_civilian_indexes(os, *civBook[i]))
				throw "An unknown error occurred in civilians' indexes save.";

		for (int i = 0; i < districts.size(); ++i)
			if (!save_district_indexes(os, *districts[i]))
				throw "An unknown error occurred in districts' indexes save.";

		for (int i = 0; i < parties.size(); ++i)
			if (!save_party_indexes(os, *parties[i]))
				throw "An unknown error occurred in parties' indexes save.";

		return true;
	}


	bool elections::load(std::istream& is)
	{

		saveNload::load_basic_type<unsigned int>(is, day);
		saveNload::load_basic_type<unsigned int>(is, month);
		saveNload::load_basic_type<unsigned int>(is, year);

		civBook.load(is);

		districts.load(is);

		parties.load(is);

		
		saveNload::load_votes_per_party_map(is, totalVotes);		//loads the total votes per party

		try {
			for (int i = 0; i < civBook.size(); ++i)					//loads all the civilians to the civilians array
			{
				try {
					civBook[i] = new civilian();
				}
				catch (bad_alloc& ex)
				{
					throw allocation_failed(ex.what(), "Memory allocation for the civilian failed.");
				}

				if (!civBook[i]->load(is))
				{
					for (civilian* civ : civBook)
						delete civ;
					civBook.clear();
					throw "An unknown error occurred in civilian load.";
				}
			}
		}
		catch (allocation_failed& ex)
		{
			for (civilian* civ : civBook)
				delete civ;
			civBook.clear();
			throw allocation_failed(ex.what(), ex.Message());
		}

		try {
			for (int i = 0; i < districts.size(); ++i)				//loads all the districts to the districts array
			{
				try {
					districts[i] = districtLoader::load(is);
				}
				catch (bad_alloc& ex)
				{
					throw allocation_failed(ex.what(), "Memory allocation for the district failed.");
				}
			}
		}
		catch (allocation_failed& ex)
		{
			for (civilian* civ : civBook)
				delete civ;
			civBook.clear();
			for (district* dis : districts)
				delete dis;
			districts.clear();
			throw allocation_failed(ex.what(), ex.Message());
		}
		
		try {
			for (int i = 0; i < parties.size(); ++i)					//loads all the parties to the parties array
			{
				try {
					parties[i] = new party("", nullptr);
				}
				catch (bad_alloc& ex)
				{
					throw allocation_failed(ex.what(), "Memory allocation for the party failed.");
				}

				if (!parties[i]->load(is))
					throw "An unknown error occurred in party load.";
			}
		}
		catch (allocation_failed& ex)
		{
			for (civilian* civ : civBook)
				delete civ;
			civBook.clear();
			for (district* dis : districts)
				delete dis;
			districts.clear();
			for(party* pty : parties)
				delete pty;
			parties.clear();
			throw allocation_failed(ex.what(), ex.Message());
		}


		for (int i = 0; i < civBook.size(); ++i)					//reconnects all the connections from each of the civilians
			if (!load_civilian_indexes(is, *civBook[i]))
				throw "An unknown error occurred in civilians' indexes load.";

		for (int i = 0; i < districts.size(); ++i)				//reconnects all the connections from each of the districts
			if (!load_district_indexes(is, *districts[i]))
				throw "An unknown error occurred in districts' indexes load.";

		for (int i = 0; i < parties.size(); ++i)					//reconnects all the connections from each of the parties
			if (!load_party_indexes(is, *parties[i]))
				throw "An unknown error occurred in parties' indexes load.";

		return true;
	}


	bool elections::set_vote(civilian* votingCiv, party* votedParty)
	{
		votingCiv->setVote(votedParty);
		return true;
	}


	bool elections::do_elections()
	{
		if (districts.empty())
			throw elections_exception("There are no districts.");
		if (parties.empty())
			throw elections_exception("There are no parties.");
		if (civBook.empty())
			throw elections_exception("There are no civilians.");
		
													//sorts by home district serial number
		civBook.sort([](civilian* civ1, civilian* civ2)->bool
			{ return civ1->getHomeDistrict()->getSerial() < civ2->getHomeDistrict()->getSerial(); });

		totalVotes.clear();							//initialize totalVotes
		for (party* pty : parties)
			totalVotes[pty->getSerial()] = 0;

		for (district* dis : districts)
			election_in_district(dis);

		return true;
	}

	void elections::print_elections()
	{
		for (district* dis : districts)
			print_district_results(dis);

		print_finale_results();
	}


	void elections::get_total_electors(std::map<int, int>& electorMap)
	{
		for (district* dis:districts)
		{
			if ((*dis).getType() == 0)												//normal district
				electorMap[dis->getWinningParty()->getSerial()] += dis->getRepNum();
			else																				//split district
			{
				dynamicArray<civilian*>& electedArray = dis->getElcRes();
				
				for (civilian* elected:electedArray)
					electorMap[elected->getRepParty()->getSerial()] += 1;

			}
		}
	}


	void elections::print_finale_results()
	{
		std::map<int, int> electorMap;
		for(party* pty: parties)
			electorMap[pty->getSerial()] = 0;

		get_total_electors(electorMap);							//get the total amount of electors for each party

		std::multimap<int, int, std::greater<int>> electorMapByElectors;
		for (auto& itr : electorMap)
			electorMapByElectors.insert({ itr.second,itr.first });

		int place = 1;

		std::cout << "In conclusion:" << std::endl;
		for (auto& itr : electorMapByElectors)
		{
			party* thisParty = find_party_by_serial(itr.second);
			if (thisParty == nullptr)
				throw item_not_found("Party wasn't found.");

			std::cout << "" << thisParty->getHead()->getName() << " ,the head of \"" << thisParty->getName() <<
				"\" party, got to the " << place++ << " place with " <<
				itr.first << " electors." << std::endl;
			std::cout << "His party was elected by " << totalVotes[itr.second] << " civilians.\n" << std::endl << std::endl;
		}
	}


	void elections::print_district_results(district* dis)
	{
		cout << "The " << dis->getName() << " district election results: " << endl;
		cout << "The number of representatives: " << dis->getRepNum() << endl;

		if ((*dis).getType() == 0)
			cout << "The Winning party is: \"" << dis->getWinningParty()->getName() << "\"" << endl;
		else
			print_split_district_electors(dis);

		int totalVotesInDis = dis->get_total_votes();

		cout << "The total number of votes is: " << totalVotesInDis << endl;
		cout << "The percentage of votes in the district is: " <<
			(static_cast<double>(totalVotesInDis) / static_cast<double>(dis->getCivOfDis().size())) * 100 << "\%" << endl;

		print_district_elected_reps(dis);
	}


	void elections::print_split_district_electors(district* dis)
	{
		std::map<int, int> elctorsPerParty;											//we need to print the amount of electors each partyHead got
				
		dynamicArray<civilian*>& elcArr = dis->getElcRes();

		for (int i = 0; i < elcArr.size(); ++i)
			elctorsPerParty[elcArr[i]->getRepParty()->getSerial()] += 1;

		std::multimap<int, int, std::greater<int>> elctorsPerPartyByElectors;
		for (auto& itr : elctorsPerParty)
			elctorsPerPartyByElectors.insert({ itr.second,itr.first });

		for (auto& itr : elctorsPerPartyByElectors)
		{
			party* partyOfTheSerial = find_party_by_serial(itr.second);
			if (partyOfTheSerial == nullptr)
				throw item_not_found("Party wasn't found.");

			std::cout << "" << partyOfTheSerial->getHead()->getName() << " from party: " << "\"" << partyOfTheSerial->getName() << "\"," <<
				" has got " << itr.first << " electors." << std::endl;
		}
	}


	bool elections::save_civilianArray_indexes(std::ostream& os, const dynamicArray<civilian*>& civArr) const //saves the indexes of each of the civilians in comparison to the indexes of the civilians in civBook (the array in election).
	{
		for (int i = 0; i < civArr.size(); ++i)
		{
			int indexOfCivInCivBook = get_index_of_civilian(civArr[i]);

			saveNload::save_basic_type(os, indexOfCivInCivBook);
		}

		return true;
	}


	bool elections::save_districtArray_indexes(std::ostream& os, const dynamicArray<district*>& disArr) const //saves the indexes of each of the districts in comparison to the indexes of the districts in districts (the array in election).
	{
		for (int i = 0; i < disArr.size(); ++i)
		{
			int indexOfDisInDistricts = get_index_of_district(disArr[i]);

			saveNload::save_basic_type(os, indexOfDisInDistricts);
		}

		return true;
	}


	bool elections::save_civilian_indexes(std::ostream& os, const civilian& civ) const				//saves the indexes of each of the civilian's pointers (according to their indexes in districts and parties (the arrays in election)).
	{
		int homeDisInd, repPartyInd, repDisInd, votePartyInd;

		homeDisInd = get_index_of_district(civ.getHomeDistrict());		//there has to be homeDistrict

		if (civ.getRepParty() != nullptr)
			repPartyInd = get_index_of_party(civ.getRepParty());
		else
			repPartyInd = point_to_nullptr;

		if (civ.getRepDis() != nullptr)
			repDisInd = get_index_of_district(civ.getRepDis());
		else
			repDisInd = point_to_nullptr;

		if (civ.getVoteParty() != nullptr)
			votePartyInd = get_index_of_party(civ.getVoteParty());
		else
			votePartyInd = point_to_nullptr;

		saveNload::save_basic_type(os, homeDisInd);
		saveNload::save_basic_type(os, repPartyInd);
		saveNload::save_basic_type(os, repDisInd);
		saveNload::save_basic_type(os, votePartyInd);

		return true;
	}


	bool elections::save_district_indexes(std::ostream& os, const district& dis) const	///saves the indexes of each of the districts' pointers (according to civBook and parties (the arrays in election))
	{
		if (!save_civilianArray_indexes(os, dis.getCivOfDis()))
			throw "An unknown error occurred in civilianArray indexes save.";

		if (!save_civilianArray_indexes(os, dis.getElcRes()))
			throw "An unknown error occurred in civilianArray indexes save.";

		int winningPartyInd;

		if (dis.getWinningParty() != nullptr)
			winningPartyInd = get_index_of_party(dis.getWinningParty());
		else
			winningPartyInd = point_to_nullptr;

		saveNload::save_basic_type(os, winningPartyInd);

		return true;
	}


	bool elections::save_party_indexes(std::ostream& os, const party& pty) const	// saves the indexes of each of the parties' pointers (according to districts and civBook (the arrays in election)) 
	{
		int headInd = get_index_of_civilian(pty.getHead());							//has to have head

		saveNload::save_basic_type(os, headInd);

		for (auto& itr : pty.getDisRepresentors())
			save_civilianArray_indexes(os, itr.second);

		return true;
	}


	bool elections::load_civilianArray_indexes(std::istream& is, dynamicArray<civilian*>& civArr)		//connects all the pointers with the indexes of each of the civilians in comparison to the indexes of the civilians in civBook (the array in election).
	{

		for (int i = 0; i < civArr.size(); ++i)
		{
			int indexOfCivInCivBook;

			saveNload::load_basic_type(is, indexOfCivInCivBook);

			civArr[i] = civBook[indexOfCivInCivBook];
		}

		return true;
	}


	bool elections::load_districtArray_indexes(std::istream& is, dynamicArray<district*>& disArr)	//connects all the pointers with the indexes of each of the districts in comparison to the indexes of the districts in districts (the array in election).
	{

		for (int i = 0; i < disArr.size(); ++i)
		{
			int indexOfDisInDistricts;

			saveNload::load_basic_type(is, indexOfDisInDistricts);

			disArr[i] = districts[indexOfDisInDistricts];
		}

		return true;
	}


	bool elections::load_civilian_indexes(std::istream& is, civilian& civ)	//connects the pointers with the indexes of each of the civilian's pointers (according to their indexes in districts and parties (the arrays in election)).
	{

		int homeDisInd, repPartyInd, repDisInd, votePartyInd;

		saveNload::load_basic_type(is, homeDisInd);
		saveNload::load_basic_type(is, repPartyInd);
		saveNload::load_basic_type(is, repDisInd);
		saveNload::load_basic_type(is, votePartyInd);

		civ.setHomeDistrict(districts[homeDisInd]);			//has to have home district

		if (repPartyInd == point_to_nullptr)
			civ.setRepresentive(nullptr, nullptr);
		else if (repPartyInd != point_to_nullptr && repDisInd == point_to_nullptr)
			civ.setRepresentive(parties[repPartyInd]);
		else
			civ.setRepresentive(parties[repPartyInd], districts[repDisInd]);

		if (votePartyInd == point_to_nullptr)
			civ.setVote(nullptr);
		else
			civ.setVote(parties[votePartyInd]);

		return true;
	}


	bool elections::load_district_indexes(std::istream& is, district& dis) //connects the pointers with the indexes of each of the districts' pointers (according to civBook and parties (the arrays in election))
	{

		if (!load_civilianArray_indexes(is, dis.getCivOfDis()))
			throw "An unknown error occurred in civilianArray indexes load.";

		if (!load_civilianArray_indexes(is, dis.getElcRes()))
			throw "An unknown error occurred in civilianArray indexes load.";

		int winningPartyInd;

		saveNload::load_basic_type(is, winningPartyInd);

		if (winningPartyInd == point_to_nullptr)
			dis.setWinningParty(nullptr);
		else
			dis.setWinningParty(parties[winningPartyInd]);

		return true;
	}


	bool elections::load_party_indexes(std::istream& is, party& pty) //connects the pointers with the indexes of each of the parties' pointers (according to districts and civBook (the arrays in election)) 
	{

		int headInd;

		saveNload::load_basic_type(is, headInd);

		pty.setHead(civBook[headInd]);				//has to have head

		for (auto& itr : pty.getDisRepresentors())
			load_civilianArray_indexes(is, itr.second);

		return true;
	}


	int elections::get_index_of_civilian(const civilian* civ) const
	{
		for (int i = 0; i < civBook.size(); i++)
			if (civBook[i] == civ)
				return i;

		return -1;
	}


	int elections::get_index_of_district(const district* dis) const
	{
		for (int i = 0; i < districts.size(); i++)
			if (districts[i] == dis)
				return i;

		return -1;
	}


	int elections::get_index_of_party(const party* pty) const
	{
		for (int i = 0; i < parties.size(); i++)
			if (parties[i] == pty)
				return i;

		return -1;
	}


	void elections::print_district_elected_reps(district* dis)	//prints the elected representatives in the district.
	{
		auto& votesMapOfDis = dis->getVotesMap();

		for (auto& itr : votesMapOfDis)
		{
			int countOfVotes = itr.second;
			double percent = (static_cast<double>(countOfVotes) / static_cast<double>(dis->get_total_votes())) * 100.00;

			party* thisParty = find_party_by_serial(itr.first);
			if (thisParty == nullptr)
				throw item_not_found("Party wasn't found.");

			cout << "\nThe \"" << thisParty->getName() << "\" party has got: " << countOfVotes << " votes, which are " <<
				percent << "\% of the total number of votes." << endl;


			bool foundReps = false;
			int r = 1;
			for (int i = 0; i < dis->getElcRes().size(); ++i)
			{
				if (thisParty == dis->getElcRes()[i]->getRepParty())
				{
					if (!foundReps)
						cout << "The elected representatives of the party are: " << endl;
					cout << "" << r++ << "). " << *dis->getElcRes()[i] << endl;
					foundReps = true;
				}
			}
			if (!foundReps)
				cout << "There are no elected representatives from the party." << endl;

		}
		cout << endl;
	}


	district* elections::find_district_by_serial(int serialToFind)
	{
		for (int i = 0; i < districts.size(); ++i)
			if (serialToFind == districts[i]->getSerial())
				return districts[i];

		return nullptr;
	}


	civilian* elections::find_civilian_by_id(int idToFind)
	{
		for (int i = 0; i < civBook.size(); ++i)
			if (idToFind == civBook[i]->getId())
				return civBook[i];

		return nullptr;
	}


	party* elections::find_party_by_serial(int serialToFind)
	{
		for (int i = 0; i < parties.size(); ++i)
			if (serialToFind == parties[i]->getSerial())
				return parties[i];

		return nullptr;
	}




	//---------------------------  DEBUG  ---------------------------//


	bool elections::set_new_civilian_DEBUG(civilian* civ)		//exactly like the normal one but expects new from outside
	{															//only meant for debug!!
		civBook.push_back(civ);					
		civ->getHomeDistrict()->addCiv(civ);		
		return true;
	}


	bool elections::set_new_district_DEBUG(district* dis)
	{
		districts.push_back(dis);
		return true;
	}


	bool elections::set_new_party_DEBUG(party* pty)
	{
		parties.push_back(pty);						//update parties
		pty->getHead()->setRepresentive(pty);		//update the head of the party

		return true;
	}

	void elections::showVotes()														
	{
		for (int i = 0; i < civBook.size(); ++i)
		{
			if (civBook[i]->getVoteParty() == nullptr)
				std::cout << civBook[i]->getName() << ": did not vote" << std::endl;
			else
				std::cout << civBook[i]->getName() << ": " << civBook[i]->getVoteParty()->getName() << std::endl;
		}
	}

	//---------------------------  DEBUG  ---------------------------//

}