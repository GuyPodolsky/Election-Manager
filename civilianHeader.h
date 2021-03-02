#pragma once
#include <iostream>

namespace elc {

	class district;
	class party;

	class civilian {
	private:

		std::string name;
		unsigned int id;
		unsigned int birthYear;
		district* homeDis;

		party* repParty;		//if == nullptr, is not a party representitive
		district* repDis;		

		party* voteParty;		//if == nullptr, did not vote

	public:
		static const short unsigned int validIDLength = 9;
		
		civilian();
		civilian(const std::string& _name, unsigned int _id, unsigned int _birthYear, district* _dis);
		civilian(const civilian& src) = delete;
		~civilian();

		civilian& operator=(const civilian&) = delete;

		bool is_rep() const { return repParty != nullptr; }			//returns true if the civilian is a representitive of a party
		bool did_vote() const { return voteParty != nullptr; }		//returns true if the civilian has voted

		friend std::ostream& operator<<(std::ostream& os, const civilian& civ);

		bool save(std::ostream& os) const;
		bool load(std::istream& is);

		//-------- getters / setters --------//

		const std::string& getName() const { return name; }
		int getId() const { return id; }
		int getBirthYear() const { return birthYear; }
		const district* getHomeDistrict() const;
		district* getHomeDistrict();


		const party* getRepParty() const;
		party* getRepParty();
		const district* getRepDis() const;
		district* getRepDis();
		const party* getVoteParty() const;
		party* getVoteParty();


		bool setName(const std::string& newName) { name = newName; return true; }
		bool setId(int newId) { id = newId; return true; }
		bool setBirthYear(int newBirthYear) { birthYear = newBirthYear; return true; }
		bool setHomeDistrict(district* newHomeDis) { homeDis = newHomeDis; return true; }

		bool setRepresentive(party* _repParty, district* _repDis = nullptr);		//sets civilian as a representor of a district
																					//in the name of repParty,
																					//if district == nullptr the civilian is the head

		bool setVote(party* _voteParty);

		//-------- getters / setters --------//


	};
	
}