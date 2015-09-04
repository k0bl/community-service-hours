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
#include <Wt/WStackedWidget>

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
#include "StudentPrintWidget.h"
#include "Hours.h"
#include "StudentSession.h"
#include "HoursResultWidget.h"

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


StudentPrintWidget::StudentPrintWidget(const char *conninfo, StudentSession *session, WContainerWidget *parent) :
WContainerWidget(parent),
conninfo_(conninfo),
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

void StudentPrintWidget::hoursList()
{

	Wt::WContainerWidget *container = new Wt::WContainerWidget();
	container->setStyleClass("print-hours-container");	

	Wt::WContainerWidget *container2 = new Wt::WContainerWidget();
	container2->setStyleClass("print-hours-container");

	Wt::WTemplate *form = new Wt::WTemplate(Wt::WString::tr("dateEdit-template"));
	form->addFunction("id", &Wt::WTemplate::Functions::id);

	Wt::WText *delabel = new Wt::WText("Please select the date range for the hours report.");
	container->addWidget(delabel);
	container->addWidget(new Wt::WBreak());
	container->addWidget(new Wt::WBreak());
	Wt::WDateEdit *de1 = new Wt::WDateEdit();
	form->bindWidget("from", de1);
	de1->setFormat("MM/dd/yyyy"); // Apply a different date format.
	de1->setPlaceholderText("Start Date");
	de1->setStyleClass("print-de1");
	container->addWidget(de1);

	Wt::WDateEdit *de2 = new Wt::WDateEdit();
	form->bindWidget("to", de2);
	de2->setFormat("MM/dd/yyyy"); // Apply a different date format.
	de2->setPlaceholderText("End Date");
	de2->setStyleClass("print-de2");
	container->addWidget(de2);

	Wt::WPushButton *button = new Wt::WPushButton("Get Hours");
	form->bindWidget("save", button);
	button->setStyleClass("print-button");
	container->addWidget(button);
	
	Wt::WText *out = new Wt::WText();
	form->bindWidget("out", out);
	
	resultStack_ = new Wt::WStackedWidget(container);

	container->addWidget(out);
	button->clicked().connect(std::bind([=] () {	
		Wt::WDate start;
		Wt::WDate end;
		start = de1->date();
		end = de2->date();
		StudentPrintWidget::showResult(start, end);
	}));

	addWidget(container);


}

void StudentPrintWidget::showResult(Wt::WDate& startDate, Wt::WDate& endDate)
{
	resultStack_->clear();
	resultWidget = new HoursResultWidget(conninfo_, session_, resultStack_);
	resultWidget->showRange(startDate, endDate);

	resultStack_->setCurrentWidget(resultWidget);

}