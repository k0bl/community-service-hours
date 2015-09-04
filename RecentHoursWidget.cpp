/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

//std io stuff
#include <sstream>
#include <fstream>
#include <iostream>
#include <locale>
#include <iomanip> //io manipulator
//boost stuff
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>


//wt stuff

#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WBootstrapTheme>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WComboBox>
#include <Wt/WTable>
#include <Wt/WLabel>
#include <Wt/WTableCell>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WStringListModel>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WCheckBox>
#include <Wt/WGroupBox>
#include <Wt/WJavaScript>
#include <Wt/WDialog>
#include <Wt/WTemplate>
#include <Wt/WCalendar>

#include <Wt/WDate>
#include <Wt/WDateEdit>
#include <Wt/WTimeEdit>

#include <Wt/WTextEdit>
#include <Wt/Utils>

//dbo stuff
#include <Wt/Dbo/collection>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Postgres>
#include <string>

//other header files
#include "RecentHoursWidget.h"
#include "Hours.h"
#include "StudentSession.h"
#include "ReportResource.cpp"

using namespace Wt;
namespace dbo = Wt::Dbo;

namespace StudentHours{
	struct HoursItem {
		Wt::WString hoursDate;
		Wt::WString hoursDescription;
		Wt::WString hoursStart;
		Wt::WString hoursEnd;
		Wt::WString hoursTotal;

		HoursItem(const Wt::WString& aHoursDate,
			const Wt::WString& aHoursDescription,
			const Wt::WString& aHoursStart,
			const Wt::WString& aHoursEnd,
			const Wt::WString& aHoursTotal)
			: hoursDate(aHoursDate),
			hoursDescription(aHoursDescription),
			hoursStart(aHoursStart),
			hoursEnd(aHoursEnd),
			hoursTotal(aHoursTotal) {}
	};
}

RecentHoursWidget::RecentHoursWidget(const char *conninfo, StudentSession *session, WContainerWidget *parent) :
WContainerWidget(parent),
pg_(conninfo),
session_(session)

{
	dbsession.setConnection(pg_);
	pg_.setProperty("show-queries", "true");
	dbsession.mapClass<Hours>("Hours");

	Wt::log("notice") << "session is being queried for student first name";
	Wt::WString studentFn = session_->strFirstName();
	Wt::log("notice") << "(The student: " << studentFn << "is adding new time.)";
	
	dbo::Transaction transaction(dbsession);
	try {
		dbsession.createTables();
		log("info") << "Database created";
	}
	catch (...) {
		log("info") << "Using existing database";
	}

	transaction.commit();

}

void RecentHoursWidget::showHours()
{		
	Wt::WContainerWidget *container = new Wt::WContainerWidget();
	
	Wt::WString studid = session_->strStudentId();

	dbo::Transaction transaction(dbsession);

	Wt::log("notice") << "hours is being queried for student hours by id";

	typedef dbo::collection< dbo::ptr<Hours> > Hourss;
	Hourss hourss = dbsession.find<Hours>().where("stud_id = ?").bind(studid);
	std::cerr << hourss.size() << std::endl;

	Wt::WTable *table = new Wt::WTable();
	table->setHeaderCount(1);
	table->setStyleClass("hours-table");
	table->elementAt(0, 0)->addWidget(new Wt::WText("Date"));
	table->elementAt(0, 1)->addWidget(new Wt::WText("Description"));
	table->elementAt(0, 2)->addWidget(new Wt::WText("Start Time"));
	table->elementAt(0, 3)->addWidget(new Wt::WText("End Time"));
	table->elementAt(0, 4)->addWidget(new Wt::WText("Hours Worked"));
	
	
	int row = 1;
	for (Hourss::const_iterator i = hourss.begin(); i != hourss.end(); ++i, ++row)
	{

			new Wt::WText((*i)->hoursDate_.toString("MM/dd/yyyy"), table->elementAt(row, 0)),
			new Wt::WText((*i)->hoursDescription_, table->elementAt(row, 1)),
			new Wt::WText((*i)->hoursStartTime_, table->elementAt(row, 2)),
			new Wt::WText((*i)->hoursEndTime_, table->elementAt(row, 3));
			new Wt::WText((*i)->totalHours_, table->elementAt(row, 4));

			
	}
	
	transaction.commit();
		
	dbo::Transaction totaltrans(dbsession);

	Wt::log("notice") << "hours is being queried for student hours by id";

	typedef dbo::collection< dbo::ptr<Hours> > Hoursc;
	Hoursc hoursc = dbsession.find<Hours>().where("stud_id = ?").bind(studid);
	std::cerr << hoursc.size() << std::endl;


	float showtotal = 0;
	float count = 0;
	for (Hoursc::const_iterator hi = hoursc.begin(); hi != hoursc.end(); ++hi)
	{		
		std::string dbstring;
		dbstring = (*hi)->totalHours_.toUTF8();

  		std::string::size_type sz;     // alias of size_t

  		float convert = std::stof (dbstring,&sz);
		count = count + convert;
		
		Wt::log("notice") << "count is now:" << count;
	}
	totaltrans.commit();

	showtotal = count;
	std::stringstream totalstream;
	totalstream << std::setprecision(1) << std::fixed << static_cast<float>(showtotal);
	Wt::WString showtotalout = totalstream.str();

	Wt::log("notice") << "total is: " << showtotal;
	Wt::log("notice") << "final count is: " << count;


	Wt::WText *finaltotal = new Wt::WText("<h4>Total Community Service Hours: " + showtotalout + "</h4>");
	finaltotal->setStyleClass("final-total");
	
	table->addStyleClass("table form-inline");
	table->addStyleClass("table table-striped");
	table->addStyleClass("table table-hover");
	
	totaltrans.commit();	
	
	Wt::WContainerWidget *result = new Wt::WContainerWidget();
	result->setStyleClass("print-result");
	
	Wt::WContainerWidget *sig = new Wt::WContainerWidget();
	sig->setStyleClass("signature-block");
	
	result->addWidget(table);
	result->addWidget(finaltotal);
	result->addWidget(new Wt::WBreak());


	
	container->addWidget(result);
	addWidget(container);

}