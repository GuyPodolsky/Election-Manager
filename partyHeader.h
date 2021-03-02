#pragma once
#include "dynamicArray.h"
#include <iostream>
#include <map>

namespace elc {

	class district;
	class civilian;

	class party {
	private:

		static unsigned int serial_generator;

		unsigned int serial;
		std::string name;
		civilian* head;
		std::map<int, dynamicArray<civilian*>> representors;

	public:

		party(const std::string& _name, civilian* _head);
		~party();

		void addRep(civilian* rep, district* repDis);

		static void reset_serial_generator();

		dynamicArray<civilian*>* get_potential_representors_of_district(const district* dis);

		friend std::ostream& operator<<(std::ostream& os, const party& pty);

		bool save(std::ostream& os) const;
		bool load(std::istream& is);

		//-------- getters / setters --------//

		unsigned int getSerial() const { return serial; }

		const std::string& getName() const { return name; }
		bool setName(const std::string& newName) { name = newName; return true; }

		const civilian* getHead() const { return head; }
		civilian* getHead() { return head; }
		bool setHead(civilian* newHead) { head = newHead; return true; }

		std::map<int, dynamicArray<civilian*>>& getDisRepresentors() { return representors; }
		const std::map<int, dynamicArray<civilian*>>& getDisRepresentors() const { return representors; }

		//-------- getters / setters --------//


		party(const party&) = delete;
		party& operator=(const party&) = delete;

	};
}