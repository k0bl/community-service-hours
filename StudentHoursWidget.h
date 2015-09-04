/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_HOURS_WIDGET_H_
#define STUDENT_HOURS_WIDGET_H_

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

#include <Wt/WStackedWidget>
#include <Wt/WContainerWidget>
#include <Wt/WString>
#include <Wt/WDate>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include "StudentSession.h"
#include "RecentHoursWidget.h"

using namespace Wt;
using namespace Wt::Dbo;
class StudentSession;

class StudentHoursWidget : public Wt::WContainerWidget
{

public:
	StudentHoursWidget(const char *conninfo, StudentSession *session, Wt::WContainerWidget *parent = 0);
	
	Wt::Dbo::Session dbsession;
	Wt::Dbo::backend::Postgres pg_;

private:
	Wt::WString uuid() {
		boost::uuids::uuid uuidx = boost::uuids::random_generator()();

		return boost::lexical_cast<std::string>(uuidx);
	}

	StudentSession *session_;
	RecentHoursWidget *resultWidget;
	const char *conninfo_;


public:
	//Patient demographics info input variables
	Wt::WLineEdit *hoursStartTime_;
	Wt::WLineEdit *hoursEndTime_;
	Wt::WLineEdit *enteredHours_;
	Wt::WLineEdit *hoursDescription_;

	void showResult();
	void hoursList();
	void addHoursDialog();
	void calcHours();
	void complete();
	void sendHours();
	void saveHours();

	Wt::WDate selectedDateOut_;
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

	Wt::WStackedWidget *resultStack_;

};

#endif STUDENT_HOURS_WIDGET_H
