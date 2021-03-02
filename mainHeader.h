#pragma once
#include "electionsLoaderHeader.h"
#include "ProgramExceptions.h"
#include <map>
#include <typeinfo>

using namespace std;

class mainClass {
private:
	static const short int legalAgeToVote = 18;
	static const short unsigned int validIDLength = 9;
	static int getID();


public:
	enum class months { Jan=1,Feb=2,Mar=3,Apr=4,May=5,Jun=6,Jul=7,Aug=8,Sep=9,Oct=10,Nov=11,Dec=12};
	enum class mainOptionsMenu {
		Exit0, create, load, do_nothing=-1
	};

	enum class typeOptionsMenu {
		Exit0, normal, simple, do_nothing=-1
	};
	
	enum class electionOptionsMenu {
		Exit0, Add_district, Add_civilian, Add_party, Add_representative, Print_districts, Print_civilians,
		Print_parties, Add_vote, Print_elections, Exit, Save, Load, Debug, do_nothing=-1
	};

	//functions get and check the input then send the input to elections

	static void add_district(elc::elections& elc);
	static void add_civilian(elc::elections& elc);
	static void add_party(elc::elections& elc);
	static void set_civ_as_rep(elc::elections& elc);
	static void set_vote(elc::elections& elc);
	static void handle_elections(elc::elections& elc);
	static void printElectionMenu();
	static void printTypeMenu();
	static void printMainMenu();
	static void printHeader(elc::elections& elc);
	static void clr();
	static void run();
	static void create_elections(elc::elections** elc);
	static void electionMenu(elc::elections* elc);
	static void typeMenu(elc::elections** elc);
	static void mainMenu(elc::elections* elc);
	template<class T>
	static T get_command(const int,const int);
	static void handle_save(elc::elections& elc);
	static void handle_load(elc::elections** elc);
	static void get_date(int& day, int& month, int& year);
	static void show_votes(elc::elections& elc);							//used for debugging only
};