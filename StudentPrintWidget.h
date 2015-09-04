/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_PRINT_WIDGET_H_
#define STUDENT_PRINT_WIDGET_H_

#include <fstream>
#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <Wt/WContainerWidget>
#include <Wt/WString>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Postgres>
#include "StudentSession.h"
#include "HoursResultWidget.h"

using namespace Wt;
using namespace Wt::Dbo;
class StudentSession;

class StudentPrintWidget : public Wt::WContainerWidget
{

public:
	StudentPrintWidget(const char *conninfo, StudentSession *session, Wt::WContainerWidget *parent = 0);
	
	Wt::Dbo::Session dbsession;
	Wt::Dbo::backend::Postgres pg_;
	const char *conninfo_;


private:
	Wt::WString uuid() {
		boost::uuids::uuid uuidx = boost::uuids::random_generator()();

		return boost::lexical_cast<std::string>(uuidx);
	}

	StudentSession *session_;
	HoursResultWidget *resultWidget;


public:
	//Patient demographics info input variables
	Wt::WLineEdit *hoursStartTime_;
	Wt::WLineEdit *hoursEndTime_;
	Wt::WLineEdit *enteredHours_;
	Wt::WLineEdit *hoursDescription_;

	void hoursList();
	void addHoursDialog();
	void calcHours();
	void complete();
	void sendHours();
	void saveHours();

	void showResult(Wt::WDate& startDate, Wt::WDate& endDate);

	Wt::WString selectedDateOut_;
	Wt::WString startTimeOut_;
	Wt::WString endTimeOut_;
	Wt::WString totalTimeOut_;
	Wt::WString descriptionOut_;

	Wt::WText *outHoursId_;

	std::string strDate();
	std::string strStartTime();
	std::string strEndTime();
	std::string strTotalHours();
	std::string strDescription();
	
	Wt::WString setHoursId();
	Wt::WString strHoursId();

	Wt::WText *staticHours_;
	Wt::WText *hoursSent_;

	Wt::WString *startEntry;
	Wt::WString *endEntry;
	
	Wt::WStackedWidget *resultStack_;




};

#endif STUDENT_PRINT_WIDGET_H
