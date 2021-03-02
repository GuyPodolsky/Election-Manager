#pragma once
#include "civilianHeader.h"
#include "dynamicArray.h"
#include "saveNload.h"
#include <iostream>
#include <map>


namespace elc {

	class party;

	class district {
	protected:

		static unsigned int serial_generator;

		const unsigned int serial;
		std::string name;
		dynamicArray<civilian*> civOfDis;				//list of civilians voting in this district
		
		dynamicArray<civilian*> elcRes;				//list of elected representatives
		std::map<int, int> votesMap;		//list of the number of votes each party got
		party* winningParty;				//pointer to the winning party in this district

		unsigned int repNum;				//number of representatives

	public:

		district();
		district(const std::string& _name, unsigned int _repNum);
		virtual ~district();
		
		void addCiv(civilian* civ);

		static void reset_serial_generator();
		
		double percentOfVoters() const;

		int get_total_votes() const;

		friend std::ostream& operator<<(std::ostream& os, const district& dis);

		virtual bool save(std::ostream& os) const;
		virtual bool load(std::istream& is);

		//-------- getters / setters --------//

		unsigned int getSerial() const { return serial; }

		const std::string& getName() const { return name; }
		bool setName(const std::string& newName) { name = newName; return true; }

		dynamicArray<civilian*>& getCivOfDis() { return civOfDis; }
		const dynamicArray<civilian*>& getCivOfDis() const { return civOfDis; }

		unsigned int getRepNum() const { return repNum; }
		bool setRepNum(unsigned int newRepNum) { repNum = newRepNum; return true; }

		dynamicArray<civilian*>& getElcRes() { return elcRes; }
		const dynamicArray<civilian*>& getElcRes() const { return elcRes; }

		std::map<int, int>& getVotesMap() { return votesMap; }
		const std::map<int, int>& getVotesMap() const { return votesMap; }

		party* getWinningParty() { return winningParty; }
		const party* getWinningParty() const { return winningParty; }
		bool setWinningParty(party* pty) { winningParty = pty; return true; }

		//-------- getters / setters --------//

		district(const district&) = delete;
		district& operator=(const district&) = delete;
		virtual const int getType() const { return 0; }
	};

}