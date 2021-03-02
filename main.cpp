#include "mainHeader.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <string>

using namespace std;
using namespace elc;

int main()
{ 
	try {
		mainClass::run();
	}
	catch (exception & ex) {
		cout << ex.what() << endl;
	}
	
	return 0;
}


int mainClass::getID()
{
	int res=0;
	string id;
	getline(cin, id);

	if (id.size() == validIDLength)
	{
		try
		{
			res = static_cast<int>(stol(id));
		}
		catch (invalid_argument& ex)					//in case one of the characters isn't a digit.
		{
			throw invalid_input("ID number should contain only digits (0-9).");
		}
		if (res<100000000)								//in case the ID number is short.
			throw invalid_input("The length of the ID number is invalid (A valid ID number contains 9 digits).");
	}
	else												//in case the ID number is short.
		throw invalid_input("The length of the ID number is invalid (A valid ID number contains 9 digits).");	
	
	return res;
}


void mainClass::add_district(elections& elc)								//gets election class and add a new district to the this election
{
	if (elc.getType() == 1)
		throw invalid_command("Due to the type of the election, additional districts are prohibited!");
	else
	{
		string newDisRepNum, newDisName, type;
		int nDRNum, tNum;

		cout << "Enter district's name: ";
		getline(cin, newDisName);											//gets an input of the new district's name.
		
		cout << "Enter the number of representatives: ";
		getline(cin, newDisRepNum);											//gets an input of the new district's number of representatives.
		try {
			nDRNum = static_cast<int>(stol(newDisRepNum));
		}
		catch (invalid_argument &ex)
		{
			throw invalid_input("Invalid input of district's number of representatives.");
		}
		
		cout << "Enter the districts type ( 0 for a normal district, 1 for a split district ): ";
		getline(cin, type);
		try {
			tNum = static_cast<int>(stol(type));
		}
		catch (invalid_argument& ex)
		{
			throw invalid_input("Invalid input of district's type.");
		}

		if (nDRNum <= 0)													//in case of invalid number of representatives
			throw invalid_input("Invalid number of representatives.");
		else if (tNum != 0 && tNum != 1)									//in case of invalid district type.
			throw invalid_input("Invalid district type.");
		else if (!elc.set_new_district(newDisName, nDRNum, tNum))			//sets the new district into the election.
			throw "The creation of a new district was failed.";	//in case of an error exits the program.
		
	}
}


void mainClass::add_civilian(elections& elc)
{
	int id, bYNum, dSNum;
	string birthYear, districtSerial;
	string newCivilName;
	district* homeDis = nullptr;

	cout << "Enter civilian's name: ";			
	getline(cin, newCivilName);												//gets an input of the new civilian's name.
	
	cout << "Enter civilian's ID number: ";
	id = getID();															//checks and gets an input of the new civilian's id number.
	
	cout << "Enter civilian's year of birth: ";
	getline(cin, birthYear);												//gets an input of the new civilian's year of birth.
	try {
		bYNum = static_cast<int>(stol(birthYear));
	}
	catch (invalid_argument &ex)
	{
		throw invalid_input("Invalid input of civilian's year of birth.");
	}

	if (elc.getType() == 0)									//in case that it is a normal elections.
	{
		cout << "Enter civilian's district number: ";
		getline(cin, districtSerial);										//gets an input of the new civilian's district.
		try {
			dSNum = static_cast<int>(stol(districtSerial));
		}
		catch (invalid_argument& ex)
		{
			throw invalid_input("Invalid input of civilian's home district serial number.");
		}

		if (dSNum > 0)
			homeDis = elc.find_district_by_serial(dSNum);					//conducts a search in the array of districts for the civilian's district.
		else
			throw invalid_input("Invalid district's serial number.");
	}

	if (homeDis == nullptr && elc.getType() == 0)				//in case that the civilian's district wasn't found. prints out a message about it and cancels the addition of the civilian.
		throw item_not_found("The civilian's district wasn't found.");
	else if (elc.find_civilian_by_id(id) != nullptr)						//checks that the civilian's id is unique
		throw invalid_input("The civilian's ID already exists.");
	else if (bYNum < 0)
		throw invalid_input("Invalid input of civilian's year of birth.");
	else if ((elc.getYear() - bYNum) < legalAgeToVote)						//checks that the civilian's age allows him to take part in the elections.
		throw invalid_input("The civilian is too young to take part in the elections.");
	else if (!elc.set_new_civilian(newCivilName, id, bYNum, homeDis))		//sets the new civilian into the election.
		throw "The creation of a new civilian was failed.";		//in case of an error exits the program.
}


void mainClass::add_party(elections& elc)									//gets election class and add a new district to the this election
{
	int partyHeadId;
	string newPartyName;
	civilian* ptyHead = nullptr;

	cout << "Enter party's name: "; 
	getline(cin, newPartyName);												//gets an input of the new party's name.
	cout << "Enter the ID number of the civilian that standing as the head of the party: ";
	partyHeadId = getID();													//gets an input of the new party's head's id.
	 
	if(partyHeadId > 0)														//checks that the part's head's id is valid.
		ptyHead = elc.find_civilian_by_id(partyHeadId);						//conducts a search in the civilians array for the civilian with that id number.

	if (ptyHead == nullptr)													//in case that the head of party wasn't found. prints out a message about it and cancels the addition of the party. 
		throw item_not_found("The ID number of the head of the party wasn't found.");
	else if (ptyHead->getRepParty() != nullptr)								//checks if the intended head of the party is already represents the following party or another party.
		throw invalid_command("The intended head of the party is already representing another party.");
	else if (!elc.set_new_party(newPartyName, ptyHead))						//sets the new party into the elections.
		throw "The creation of a new party was failed.";
}


void mainClass::set_civ_as_rep(elections& elc)											//sets a civilian as a representative of one of the parties in one of the districts of the election
{
	int civId,pSNum,dSNum;
	string ptySerial, disSerial;
	civilian* civ = nullptr;
	party* pty = nullptr;
	district* dis = nullptr;
	
	cout << "Enter civilian's ID number: ";
	civId = getID();																	//gets an input of the civilian's id number.
	cout << "Enter the number of the party that he represents is: ";	
	getline(cin,ptySerial);																//gets an input of the party's serial number.
	try
	{
		pSNum = static_cast<int>(stol(ptySerial));
	}
	catch (invalid_argument &ex)
	{
		throw invalid_input("Invalid input of party's serial number.");
	}

	if (elc.getType() == 0)												//only if normal elections
	{
		cout << "Enter the number of the district whither he is running in the election: ";
		getline(cin,disSerial);															//gets an input of the district's serial number.
		try
		{
			dSNum = static_cast<int>(stol(disSerial));
		}
		catch (invalid_argument& ex)
		{
			throw invalid_input("Invalid input of district's serial number.");
		}
		
		if (dSNum > 0)
			dis = elc.find_district_by_serial(dSNum);									//conducts a search for the district with that serial number.
		else
			throw invalid_input("Invalid district serial number.");
	}
	
	if (civId > 0)																		//checks that the civilian's id number is valid. 
		civ = elc.find_civilian_by_id(civId);											//conducts a search for the civilian with that id.
	else
		throw invalid_input("Invalid civilian ID.");
	if (pSNum > 0)																		//checks that the party's serial number is valid. 
		pty = elc.find_party_by_serial(pSNum);											//conducts a search for the party with that serial number.
	else
		throw invalid_input("Invalid party serial number.");

	if (civ == nullptr)																	//in case the civilian wasn't found.
		throw item_not_found("The civilian's ID wasn't found.");
	else if (pty == nullptr)															//in case the party wasn't found.
		throw item_not_found("The party's number wasn't found.");
	else if (dis == nullptr && elc.getType() == 0)							//in case the district wasn't found.
		throw item_not_found("The district's number wasn't found.");
	else if (civ->is_rep())																//check if the civilian already represents the following party or another party.
		throw invalid_command("The civilian already represents another party");
	else if (!elc.set_civilian_as_representative(civ, pty, dis))						//sets the civilian as a intended representative of the party in the district.
		throw "The setting of the civilian as a representative was failed.";	//in case of an error exits the program.
}


void mainClass::set_vote(elc::elections& elc)								//sets one vote from one of the civilians to one of the parties.
{
	int id, ptyNum;
	string partyNum;
	party* votedParty = nullptr;
	civilian* votingCiv = nullptr;

	cout << "Enter civilian's ID number: ";
	id = getID();															//gets an input of the civilians id.
	cout << "Enter the party's number that the civilian voted for: ";
	getline(cin, partyNum);													//gets the party serial that we want to vote to.
	try
	{
		ptyNum = static_cast<int>(stol(partyNum));
	}
	catch (invalid_argument& ex)
	{
		throw invalid_input("Invalid party serial number.");
	}

	if (id > 0)																//checks the validness of the civilian's id.
		votingCiv = elc.find_civilian_by_id(id);							//conducts a search for the civilian with that id number.
	else
		throw invalid_input("Invalid civilian's ID.");

	if (ptyNum > 0)															//checks the validness of the party's serial number.
		votedParty = elc.find_party_by_serial(ptyNum);						//conducts a search for the party with that serial number.
	else
		throw invalid_input("Invalid party's serial number.");

	if (votingCiv == nullptr)												//in case that the voting civilian wasn't found.
		throw item_not_found("The voting civilian wasn't found.");
	else if (votedParty == nullptr)											//in case that the voted party wasn't found.
		throw item_not_found("The voted for party wasn't found.");
	else if (votingCiv->did_vote())											//in case that the voting civilian already voted.
		throw invalid_command("The voting civilian already voted.");
	else if (!elc.set_vote(votingCiv, votedParty))							//sets the civilian's vote for the voted party.
		throw "The setting of the vote was failed.";				//in case of an error exits the program.}
}


void mainClass::handle_elections(elections& elc)	//calculates the elections result and prints it out.
{
		if(!elc.do_elections())						//calculates the election result.
			throw "An unknown error in the calculating of the election's results.";

		elc.print_elections();						//prints the election result.			
}


void mainClass::printElectionMenu()													//prints the menu of available commands options.
{
	cout << "\n1. Add district." << endl;
	cout << "2. Add civilian." << endl;
	cout << "3. Add party." << endl;
	cout << "4. Add civilian as party rep." << endl;
	cout << "5. Print all districts." << endl;
	cout << "6. Print all civilians." << endl;
	cout << "7. Print all parties." << endl;
	cout << "8. Add vote." << endl;
	cout << "9. Show election results." << endl;
	cout << "10. Exit." << endl;
	cout << "11. Save election." << endl;
	cout << "12. Load election.\n" << endl;
}


void mainClass::printTypeMenu()
{
	cout << "Choose a type of election: " << endl;
	cout << "1 for normal election" << endl;
	cout << "2 for Simple election" << endl;
}


void mainClass::printMainMenu()
{
	cout << "---Start Menu---" << endl;							
	cout << "1. Create a new election round" << endl;
	cout << "2. Load election round" << endl;
	cout << "0. Exit" << endl;
}


void mainClass::printHeader(elections& elc)									//prints the head of the menu.
{
	cout << "Election of " << elc.getDay() << "/" << elc.getMonth() << "/" << elc.getYear() << endl;
}


void mainClass::clr()														//clears the output screen.
{
	cout << "\033[2J\033[1;1H";
}


void mainClass::run()
{
	while (true)
	{
		elections* elc = nullptr;
		try
		{
			mainMenu(elc);				//prints the main menu and gets the user's option of choice
		}
		catch (invalid_argument& ex)
		{
			clr();
			cout << ex.what() << endl;
		}
		catch (invalid_input& ex)
		{
			clr();
			cout << ex.what() << endl;
		}
		catch (...)
		{
			clr();
			cout << "An unknown error was occurred in the option selection." << endl;
		}
	}
}


void mainClass::create_elections(elections** elc)
{
	try
	{
		typeMenu(elc);
	}
	catch (invalid_argument& ex)
	{
		clr();
		cout << ex.what() << endl;
		create_elections(elc);
	}
	catch (invalid_input& ex)
	{
		clr();
		cout << ex.what() << endl;
		create_elections(elc);
	}
	catch (...)
	{
		clr();
		cout << "An unknown error was occurred in the option selection." << endl;
		create_elections(elc);
	}
}


void mainClass::electionMenu(elections* elc)
{
	if (elc == nullptr)
		throw "The election is empty.";
	electionOptionsMenu option = electionOptionsMenu::do_nothing;
	string debugCommand;

	printHeader(*elc);										//prints the menu.
	printElectionMenu();

	try
	{
		option = get_command<electionOptionsMenu>(0, 13);										//gets a command input from the user.
	}
	catch (invalid_argument& ex)
	{
		clr();
		cout << ex.what() << endl;
		electionMenu(elc);
	}
	catch (invalid_input& ex)
	{
		clr();
		cout << ex.what() << endl;
		electionMenu(elc);
	}
	catch (...)
	{
		clr();
		cout << "An unknown error was occurred in the option selection." << endl;
		electionMenu(elc);
	}
	

	while (true)
	{
		switch (option)
		{
		case electionOptionsMenu::Add_district:
			try
			{
				add_district(*elc);								//Adds a new district to the current elections.
			}
			catch (invalid_command &ex)
			{
				cout << ex.what() << endl;
			}
			catch(invalid_input &ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_argument& ex)
			{
				cout << ex.what() << endl;
			}
			catch (out_of_range& ex)
			{
				cout << ex.what() << endl;
			}
			catch (allocation_failed& ex)
			{
				cout << ex.Message() << endl;
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the district creation." << endl;
			}
			break;
		case electionOptionsMenu::Add_civilian:
			try {
				add_civilian(*elc);								//Adds a new civilian to the current elections.
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_input& ex)
			{
				cout << ex.what() << endl;
			}
			catch (item_not_found& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_argument& ex)
			{
				cout << ex.what() << endl;
			}
			catch (allocation_failed& ex)
			{
				cout << ex.Message() << endl;
				cout << ex.what() << endl;
			}
			catch (out_of_range& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the district creation." << endl;
			}
			break;
		case electionOptionsMenu::Add_party:
			try {
				add_party(*elc);									//Adds a new party to the current elections.
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_input& ex)
			{
				cout << ex.what() << endl;
			}
			catch (item_not_found& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_argument& ex)
			{
				cout << ex.what() << endl;
			}
			catch (allocation_failed& ex)
			{
				cout << ex.Message() << endl;
				cout << ex.what() << endl;
			}
			catch (out_of_range& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the party creation." << endl;
			}
			break;
		case electionOptionsMenu::Add_representative:
			try {
				set_civ_as_rep(*elc);							//Adds an existing civilian as a representative of one of the parties in one of the districts of the election. 
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_input& ex)
			{
				cout << ex.what() << endl;
			}
			catch (item_not_found& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_argument& ex)
			{
				cout << ex.what() << endl;
			}
			catch (allocation_failed& ex)
			{
				cout << ex.Message() << endl;
				cout << ex.what() << endl;
			}
			catch (out_of_range& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the setting of the civilian as a representative." << endl;
			}
				break;
		case electionOptionsMenu::Print_districts:
			try {
				elc->print_districts();										//prints out all the district that take part in the election.
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the printing of the districts." << endl;
			}
			break;
		case electionOptionsMenu::Print_civilians:
			try {
				elc->print_civilians();										//prints out all the civilians that can take part in the election.
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the printing of the civilians." << endl;
			}
			break;
		case electionOptionsMenu::Print_parties:
			try{
				elc->print_parties();										//prints out all the parties that running in the election.
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the printing of the parties." << endl;
			}
			break;
		case electionOptionsMenu::Add_vote:
			try {
				set_vote(*elc);									//get a vote from one of the civilians to one of the parties.
			}
			catch (invalid_command& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_input& ex)
			{
				cout << ex.what() << endl;
			}
			catch (item_not_found& ex)
			{
				cout << ex.what() << endl;
			}
			catch (invalid_argument& ex)
			{
				cout << ex.what() << endl;
			}
			catch (out_of_range& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error was occurred in the party creation." << endl;
			}
			break;
		case electionOptionsMenu::Print_elections:
			try {
				handle_elections(*elc);							//calculates the elections results and prints them out.
			}
			catch (allocation_failed& ex)
			{
				cout << ex.Message() << endl;
				cout << ex.what() << endl;
			}
			catch (item_not_found& ex)
			{
				cout << ex.what() << endl;
			}
			catch (elections_exception& ex)
			{
				cout << ex.what() << endl;
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown error has occurred while printing the election's results" << endl;
			}
			break;
		    case electionOptionsMenu::Exit:
			delete[] elc;
			exit(EXIT_SUCCESS);
		case electionOptionsMenu::Exit0:
			delete[] elc;
			exit(EXIT_SUCCESS);
		case electionOptionsMenu::Save:
			try
			{
				handle_save(*elc);								//saves the current values in the election, so it will be accessible later
			}
			catch (filesystem_exception &ex)
			{
				cout << ex.what();
				if (!ex.filename().empty())
					cout << ex.filename();
				cout << endl;
				
				delete[] elc;
				exit(EXIT_FAILURE);
			}
			catch (ofstream::failure &ex)
			{
				clr();
				cout << ex.code() << " - " << ex.what() << endl;
				delete[] elc;
				exit(ex.code().value());
			}
			catch (...)
			{
				cout << "An unknown error occurred in the saving." << endl;
			}
			break;
		case electionOptionsMenu::Load:
			try {
				handle_load(&elc);								//loads a saved election.
			}
			catch (allocation_failed &ex)
			{
				cout << ex.Message() << endl;
				cout << ex.what() << endl;
			}
			catch (filesystem_exception& ex)
			{
				cout << ex.what();
				if (!ex.filename().empty())
					cout << ex.filename();
				cout << endl;

				delete[] elc;
				exit(EXIT_FAILURE);
			}
			catch (ifstream::failure& ex)
			{
				clr();
				cout << ex.code() << " - " << ex.what() << endl;
				delete[] elc;
				exit(ex.code().value());
			}
            catch (invalid_input& ex)
            {
                cout << ex.what() << endl;
                delete[] elc;
                exit(EXIT_FAILURE);
            }
			catch (...)
			{
				cout << "An unknown error occurred in the loading." << endl;
			}
			printHeader(*elc);
			break;
		case electionOptionsMenu::Debug:										//For debugging only ,please don't deduct points.
			cout << "---Debug---" << endl;
			cout << "Enter a debug command (\"votes\" or \"clr\"): ";
			try {
				getline(cin, debugCommand);

				if (debugCommand == "votes")
					show_votes(*elc);										//Debug: prints all the civilians and their votes
				else if (debugCommand == "clr")
				{
					clr();													//Debug: clears the console.
					printHeader(*elc);
				}
				else
					throw invalid_input("An unknown debug command."); 							//Debug: in case there is invalid input.
			}
			catch (invalid_input& ex)
			{
				cout << ex.what() << endl;
			}
			catch (exception& ex)
			{
				cout << ex.what() << endl;
			}
			catch (...)
			{
				cout << "An unknown debug command." << endl;
			}
			break;
		default:
			break;
		}
		
		printElectionMenu();											//prints the menu, once again.
		try
		{
			option = get_command<electionOptionsMenu>(0, 13);										//gets a command input from the user.
		}
		catch (invalid_argument& ex)
		{
			clr();
			cout << ex.what() << endl;
			electionMenu(elc);
		}
		catch (invalid_input& ex)
		{
			clr();
			cout << ex.what() << endl;
			electionMenu(elc);
		}
		catch (...)
		{
			clr();
			cout << "An unknown error was occurred in the option selection." << endl;
			electionMenu(elc);
		}
	}
}


void mainClass::mainMenu(elections* elc)
{
	mainOptionsMenu option= mainOptionsMenu::do_nothing;
	
	printMainMenu();
	option = get_command<mainOptionsMenu>(0, 2);		//gets a command input from the user.
	switch (option)
	{
	case mainOptionsMenu::create:
		create_elections(&elc);
		clr();
		electionMenu(elc);
		break;
	case mainOptionsMenu::load:
		clr();
		try {
			handle_load(&elc);								//loads a saved election.
		}
		catch (allocation_failed& ex)
		{
			cout << ex.Message() << endl;
			cout << ex.what() << endl;
		}
		catch (filesystem_exception& ex)
		{
			cout << ex.what();
			if (!ex.filename().empty())
				cout << ex.filename();
			cout << endl;

			delete[] elc;
			exit(EXIT_FAILURE);
		}
		catch (ifstream::failure& ex)
		{
			clr();
			cout << ex.code() << " - " << ex.what() << endl;
			delete[] elc;
			exit(ex.code().value());
		}
        catch (invalid_input& ex)
        {
            cout << ex.what() << endl;
            delete[] elc;
            exit(EXIT_FAILURE);
        }
		catch (...)
		{
			cout << "An unknown error occurred in the loading." << endl;
		}
		clr();
		electionMenu(elc);
		break;
	case mainOptionsMenu::Exit0:
		delete[] elc;										//frees the election at the end of the program run.
		exit(EXIT_SUCCESS);
	default:
		break;
	}
}


void mainClass::typeMenu(elections** elc)
{
	int day, month, year,repNum;
	string rN;
	typeOptionsMenu option = typeOptionsMenu::do_nothing;

	printTypeMenu();

	option = get_command<typeOptionsMenu>(0, 2);		//gets a command input from the user.

	switch (option)
	{
	case typeOptionsMenu::normal:
		try {
			get_date(day, month, year);											//gets the date of the election
		}
		catch (invalid_input& ex)
		{
			cout << ex.what() << endl;
			typeMenu(elc);
			return;
		}
		catch (out_of_range& ex)
		{
			cout << ex.what() << endl;
			typeMenu(elc);
			return;
		}
		catch (...)
		{
			cout << "An unknown error occurred in the getting of the date." << endl;
			typeMenu(elc);
			return;
		}

		try {
			*elc = new elections(day, month, year);								//creates normal election instance
		}
		catch (bad_alloc& ex)
		{
			cout << "Memory allocation for the election failed." << endl;
			typeMenu(elc);
			return;
		}
        catch (invalid_input& ex)
        {
            cout << ex.what() << endl;
            typeMenu(elc);
            return;
        }
		break;
	case typeOptionsMenu::simple:
		try {
			get_date(day, month, year);											//gets the date of the election
		}
		catch (invalid_input& ex)
		{
			cout << ex.what() << endl;
			typeMenu(elc);
			return;
		}
		catch (out_of_range& ex)
		{
			cout << ex.what() << endl;
			typeMenu(elc);
			return;
		}
		catch (...)
		{
			cout << "An unknown error occurred in the getting of the date." << endl;
			typeMenu(elc);
			return;
		}
		
		cout << "Enter the number of representatives in the election: ";
		getline(cin,rN);														//gets the number of representatives in the election.
		try {
			repNum = static_cast<int>(stol(rN));
		}
		catch (invalid_argument& ex)
		{
			throw invalid_input("Invalid input of number of representatives.");
		}
		catch (invalid_input& ex)
		{
			cout << ex.what() << endl;\
			typeMenu(elc);
			return;
		}
		catch (out_of_range& ex)
		{
			cout << ex.what() << endl;
			typeMenu(elc);
			return;
		}
		catch (...)
		{
			cout << "An unknown error occurred in the getting of number of representatives." << endl;
			typeMenu(elc);
			return;
		}
		
		if (repNum < 1)														//check that the number is valid.
			throw invalid_input("Illegal number of representatives.");
		
		try{
			*elc = new simpleElections(repNum, day, month, year);				//creates simple election instance
		}
		catch (bad_alloc& ex)
		{
			throw allocation_failed(ex.what(), "Memory allocation for the election failed.");
		}
		catch (allocation_failed& ex)
		{
			cout << ex.Message() << endl;
			cout << ex.what() << endl;
			typeMenu(elc);
			return;
		}
        catch (invalid_input& ex)
        {
            cout << ex.what() << endl;
            typeMenu(elc);
            return;
        }
		break;
	case typeOptionsMenu::Exit0:
		delete[] * elc;
		exit(EXIT_SUCCESS);
	default:
		break;
	}
}


template<class T>
T mainClass::get_command(const int firstOP,const int lastOP)
{
	string command;
	int NumCommand;
	T res;
	if(!(is_enum<T>::value))
		throw invalid_argument("The function \"get_commend\" handles enum classes only.\n");
	else {
		getline(cin, command);																	//gets an input that represent a command in the menuOptions enum
		try {
			NumCommand = static_cast<int>(stol(command));
		}
		catch (invalid_argument &ex){
			throw invalid_input("Invalid input of option number.");
		}
		
		if (NumCommand < firstOP || NumCommand >lastOP)											//checks that the input is valid.
			throw invalid_input("Invalid Option.");
							
		res = static_cast<T>(static_cast<int>(NumCommand));										//returns a valid command that was chosen by the user
	}
	return res;
}


void mainClass::handle_save(elections& elc)
{
	string saveName;
	ofstream out;
	cout << "Enter a name for the save file: ";							
	getline(cin, saveName);														//gets a name for the file.

	out.open(saveName.c_str(), ios::binary | ios::trunc);
	if (!out.is_open())																//in case of an error in the opening of the file.
		throw filesystem_exception("Couldn't open the following file: ",saveName);
	if (!electionsLoader::save(out, &elc))										//saves the election's data into the file.
		throw filesystem_exception("An unknown error occurred in the saving.");

	out.close();
}


void mainClass::handle_load(elections** elc)
{
	ifstream in;
	string saveName;

	cout << "Enter the name of the file:" << endl;										
	getline(cin, saveName);																		//gets file's name
	
	in.open(saveName.c_str(), ios::binary);														//opens the file
	if (!in.is_open())																			//in case of an error in the opening of the file.
		throw filesystem_exception("Couldn't open the following file: ", saveName);

	if(*elc != nullptr)																			//if there is another loaded election, make the variable empty.
		delete *elc;																			

	district::reset_serial_generator();															//resets the serial generators
	party::reset_serial_generator();
	
	*elc = electionsLoader::load(in);															//loads the saves election from the file
	if (*elc == nullptr)																		//exits with error in case the election load failed.
		throw filesystem_exception("An unknown error occurred in the loading.");

	in.close();																					//closes the file
}


void mainClass::get_date(int& day, int& month, int& year)
{

	string dd, mm, yyyy;
	cout << "Enter the date of the election: (day/month/year):" << endl;
	cout << "Day: ";
	getline(cin, dd);
	try{
		day = static_cast<int>(stol(dd));
	}
	catch (invalid_argument& ex)
	{
		throw invalid_input("Invalid input of day.");
	}

	cout << "Month: ";
	getline(cin, mm);
	try {
		month = static_cast<int>(stol(mm));
	}
	catch (invalid_argument& ex)
	{
		throw invalid_input("Invalid input of month.");
	}
	
	cout << "Year: ";
	getline(cin, yyyy);
	try{
		year = static_cast<int>(stol(yyyy));
	}
	catch (invalid_argument& ex)
	{
		throw invalid_input("Invalid input of year.");
	}
	
	if (year <= 0)									//checks the validness of the year.
		throw invalid_input("Invalid year.");
	
	if (month >= 1 && month <= 12)					//checks the validness of the month.
	{												//in case the month contains 31 days.
		if (static_cast<months>(month) == months::Jan || static_cast<months>(month) == months::Mar || static_cast<months>(month) == months::May || static_cast<months>(month) == months::Jul|| static_cast<months>(month) == months::Aug|| static_cast<months>(month) == months::Oct|| static_cast<months>(month) == months::Dec)
		{
			if (day < 1 || day > 31)				//checks the validness of the day.
				throw invalid_input("Invalid days in the month (should be between 1 to 31).");
		}											//in case the month contains 30 days.
		else if (static_cast<months>(month) == months::Apr || static_cast<months>(month) == months::Jun || static_cast<months>(month) == months::Sep || static_cast<months>(month) == months::Nov)
		{
			if (day < 1 || day > 30)				//checks the validness of the day.
				throw invalid_input("Invalid day in the month (should be between 1 to 30).");
		}
		else										//in case the month is February.
		{
			if (year % 4 == 0)						//in case this is a leap year.
			{
				if (day < 1 || day > 29)			//checks the validness of the day.
					throw invalid_input("Invalid day in the month (should between 1 to 29).");
			}
			else									//in case this isn't a leap year.
			{
				if (day < 1 || day > 28)			//checks the validness of the day.
					throw invalid_input("Invalid day in the month (should be between 1 to 28).");
			}
		}
	}
	else											
		throw invalid_input("Invalid month (should be between 1 to 12).");
}

//---------------------------  DEBUG  ---------------------------//

void mainClass::show_votes(elc::elections& elc)
{
	elc.showVotes();
}

//---------------------------  DEBUG  ---------------------------//