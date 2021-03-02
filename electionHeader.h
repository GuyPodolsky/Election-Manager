#pragma once
#include "dynamicArray.h"
#include "civilianHeader.h"
#include "districtHeader.h"
#include "partyHeader.h"
#include "ProgramExceptions.h"
#include <iostream>

using namespace std;

namespace elc {

	class elections {
	private:

		static const int point_to_nullptr = -1;		//for saving and loading

		void print_district_results(district* dis);
		void print_split_district_electors(district* dis);

		//we are saving all the pointers as the indexes in the 3 main arrays:
		//we save all the civilian pointers as their indexes in civBook
		//we save all the districts pointers as their indexes in districts (the array in elections)
		//we save all the parties pointers as their indexes in parties (the array in elections)


		bool save_civilianArray_indexes(std::ostream& os, const dynamicArray<civilian*>& civArr) const;
		bool save_districtArray_indexes(std::ostream& os, const dynamicArray<district*>& disArr) const;
		bool save_civilian_indexes(std::ostream& os, const civilian& civ) const;
		bool save_district_indexes(std::ostream& os, const district& dis) const;
		bool save_party_indexes(std::ostream& os, const party& pty) const;

		bool load_civilianArray_indexes(std::istream& is, dynamicArray<civilian*>& civArr);
		bool load_districtArray_indexes(std::istream& is, dynamicArray<district*>& disArr);
		bool load_civilian_indexes(std::istream& is, civilian& civ);
		bool load_district_indexes(std::istream& is, district& dis);
		bool load_party_indexes(std::istream& is, party& pty);

		int get_index_of_civilian(const civilian* civ) const;
		int get_index_of_district(const district* dis) const;
		int get_index_of_party(const party* pty) const;

	protected:

		unsigned int day, month, year;

		dynamicArray<civilian*> civBook;
		dynamicArray<district*> districts;
		dynamicArray<party*> parties;
		std::map<int, int> totalVotes;

		bool election_in_district(district* dis);
																			// returns the index that the next district needs to start from,
																			// if something went wrong return bad_elections

		void print_district_elected_reps(district* dis);
		void get_total_electors(std::map<int, int>& electorArr);
		void print_finale_results();


	public:
        enum class months { Jan=1,Feb=2,Mar=3,Apr=4,May=5,Jun=6,Jul=7,Aug=8,Sep=9,Oct=10,Nov=11,Dec=12};
		static const int district_type_normal = 0;
		static const int district_type_split = 1;

		elections(unsigned int _day = 1, unsigned int _month = 1, unsigned int _year = 2020);
		virtual ~elections();

		virtual const int getType() const { return district_type_normal; }
		virtual bool set_new_civilian(const std::string& civName, unsigned int civId, unsigned int civBirthYear, district* civHomeDis);
		virtual bool set_new_district(const std::string& disName, unsigned int disRepNum, int type);
		bool set_new_party(const std::string& ptyName, civilian* ptyHead);
		virtual bool set_civilian_as_representative(civilian* civ, party* pty, district* dis);

		void print_civilians();
		virtual void print_districts();
		virtual void print_parties();
		void showVotes();		//for debug

		virtual bool save(std::ostream& os) const;
		virtual	bool load(std::istream& is);

		bool set_vote(civilian* votingCiv, party* votedParty);
		bool do_elections();

		virtual void print_elections();

		district* find_district_by_serial(int serialToFind);
		civilian* find_civilian_by_id(int idToFind);
		party* find_party_by_serial(int serialToFind);

		unsigned int getYear() const { return year; }
		unsigned int getMonth() const { return month; }
		unsigned int getDay() const { return day; }

		elections(const elections&) = delete;
		elections& operator=(const elections&) = delete;


		//---------------------------  DEBUG  ---------------------------//

		bool set_new_civilian_DEBUG(civilian* civ);
		bool set_new_district_DEBUG(district* dis);
		bool set_new_party_DEBUG(party* pty);

		//---------------------------  DEBUG  ---------------------------//

	};

}