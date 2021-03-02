#pragma once
#include "electionHeader.h"
#include <iostream>

namespace elc {

	class simpleElections : public elections {
	public:

		explicit simpleElections(int representativesNumber, unsigned int _day = 1, unsigned int _month = 1, unsigned int _year = 2020);
		virtual ~simpleElections() = default;

		virtual bool set_new_civilian(const std::string& civName, unsigned int civId, unsigned int civBirthYear, district* civHomeDis) override;
		virtual bool set_new_district(const std::string& disName, unsigned int disRepNum, int type) override;
		virtual bool set_civilian_as_representative(civilian* civ, party* pty, district* dis) override;
		virtual void print_districts() override;
		virtual void print_parties() override;

		virtual void print_elections() override;

		simpleElections(const simpleElections&) = delete;
		simpleElections& operator=(const simpleElections&) = delete;

		const int getType() const override { return 1; }
	};

}