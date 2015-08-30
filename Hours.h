/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef HOURS_H_
#define HOURS_H_

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/WtSqlTraits>
namespace dbo = Wt::Dbo;

class Hours
{
public:
	Wt::WDate hoursDate_;
	Wt::WString hoursStartTime_;
	Wt::WString hoursEndTime_;
	Wt::WString enteredHours_;
	Wt::WString totalHours_;
	Wt::WString hoursDescription_;
	Wt::WString submittedOn_;

	Wt::WString studentFirstName_;
	Wt::WString studentLastName_;
	Wt::WString studentId_;

	Wt::WString hoursId_;



	//dbo template for reperral
	template<class Action>
	void persist(Action& a)
	{
		//class mapping for reperring practice
		dbo::field(a, hoursDate_, "hours_date");
		dbo::field(a, hoursStartTime_, "start_time");
		dbo::field(a, hoursEndTime_, "end_time");
		dbo::field(a, enteredHours_, "entered_hours");
		dbo::field(a, totalHours_, "total_hours");
		dbo::field(a, hoursDescription_, "description");
		dbo::field(a, submittedOn_, "sub_on");

		//class mapping for patient info

		dbo::field(a, studentFirstName_, "stud_first");
		dbo::field(a, studentLastName_, "stud_last");
		dbo::field(a, studentId_, "stud_id");
		dbo::field(a, hoursId_, "hours_id");
	}

};

#endif //HOURS_H_