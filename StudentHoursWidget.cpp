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
#include <Wt/WTimeEdit>

#include <Wt/WTextEdit>
#include <Wt/Utils>

//dbo stuff
#include <Wt/Dbo/collection>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Sqlite3>
#include <string>

//other header files
#include "StudentHoursWidget.h"
#include "Hours.h"
#include "StudentSession.h"

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

StudentHoursWidget::StudentHoursWidget(StudentSession *session, WContainerWidget *parent) :
WContainerWidget(parent),
sqlite3_(Wt::WApplication::appRoot() + "hours.db"),
session_(session)

{
	dbsession.setConnection(sqlite3_);
	sqlite3_.setProperty("show-queries", "true");
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

void StudentHoursWidget::hoursList()
{

	Wt::log("notice") << "session is being queried for student ID";
	
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
			new Wt::WText((*i)->hoursEndTime_, table->elementAt(row, 3)),
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


	Wt::WContainerWidget *result = new Wt::WContainerWidget();
	result->setStyleClass("hours-result");
	
	Wt::WPushButton *addTimeButton = new Wt::WPushButton("Add Hours");
	result->addWidget(addTimeButton);
	result->addWidget(new Wt::WText("<h3>Completed Hours</h3>"));
	
	addTimeButton->clicked().connect(boost::bind(&StudentHoursWidget::addHoursDialog, this));	


	result->addWidget(table);
	result->addWidget(finaltotal);
	addWidget(result);

}

WString StudentHoursWidget::setHoursId()
{
	return uuid();
}

WString StudentHoursWidget::strHoursId()
{
	
}
void StudentHoursWidget::addHoursDialog()
{
	//create patient id
	staticHours_ = new Wt::WText();
	staticHours_->setText(setHoursId());
	
	Wt::WDialog *dialog = new Wt::WDialog("Add Time");
	dialog->setStyleClass("add-hours-dialog");
	Wt::WContainerWidget *container = new Wt::WContainerWidget(dialog->contents());
	container->setStyleClass("hours-dialog-container");

	Wt::WHBoxLayout *hbox = new Wt::WHBoxLayout();
	container->setLayout(hbox);
	
	Wt::WVBoxLayout *vbox1 = new Wt::WVBoxLayout();
	hbox->addLayout(vbox1);

	Wt::WVBoxLayout *vbox2 = new Wt::WVBoxLayout();
	hbox->addLayout(vbox2);
    

	Wt::WLabel *descriptionLbl = new Wt::WLabel("Description of Time");
	
	Wt::WTextEdit *descriptionTa = new Wt::WTextEdit();
	descriptionTa->setHeight(100);
	
	vbox1->addWidget(descriptionLbl);
	vbox1->addWidget(descriptionTa);
	vbox1->addWidget(new Wt::WBreak());
	
	Wt::WLabel *startLbl = new Wt::WLabel("Start Time (i.e. 9:15 AM)");
	Wt::WTimeEdit *de1 = new Wt::WTimeEdit();
	de1->setFormat("h:m a");
	vbox1->addWidget(startLbl);
	vbox1->addWidget(de1);
	vbox1->addWidget(new Wt::WBreak());

	Wt::WLabel *endLbl = new Wt::WLabel("End Time (i.e. 4:15 PM)");	
	Wt::WTimeEdit *de2 = new Wt::WTimeEdit();
	de2->setFormat("h:m a"); // Apply a different date format.
	vbox1->addWidget(endLbl);
	vbox1->addWidget(de2);
	vbox1->addWidget(new Wt::WBreak());

	Wt::WPushButton *checkTime = new Wt::WPushButton("Calculate Time");
	vbox1->addWidget(checkTime);

	Wt::WText *timeOut = new Wt::WText();
	Wt::WText *total = new Wt::WText();

	de1->changed().connect(std::bind([=] () {
	    if (de1->validate() == Wt::WValidator::Valid) {
	    timeOut->setText("Start Time is Valid.");

	    }
	}));

	de2->changed().connect(std::bind([=] () {
	    if (de1->validate() == Wt::WValidator::Valid) {
	    timeOut->setText("End Time is Valid.");
	    }
	}));
	
	vbox1->addWidget(timeOut);
	vbox1->addWidget(new Wt::WBreak());
	
	checkTime->clicked().connect(std::bind([=] () {
	    if (de1->text().empty() || de2->text().empty())
			timeOut->setText("You need to enter a start time and an end time");
	    else {
			float secs = de1->time().secsTo(de2->time()) + 1;
	    	timeOut->setText(Wt::WString("You started at "
				     	"{1} and finished at {2} ?...").arg(de1->time().toString()).arg(de2->time().toString()));
	    	std::cout << secs;
	    	float hours = secs/60/60;
	    	
	    	std::stringstream ss;
	    	
	    	ss << std::setprecision(1) << std::fixed << static_cast<float>(hours);
	    	
	    	Wt::WString hout;
	    	hout = ss.str();

	    	total->setText("You worked for " + hout + " hours. Is This Correct?");


			vbox1->addWidget(total);
		    vbox1->addWidget(new Wt::WBreak());

    	    Wt::WPushButton *saveTime = new Wt::WPushButton("Yes");
			vbox1->addWidget(saveTime);
			saveTime->clicked().connect(std::bind([=] () {

				StudentHoursWidget::startTimeOut_ = de1->text().toUTF8();

				StudentHoursWidget::endTimeOut_ = de2->text().toUTF8();

				StudentHoursWidget::totalTimeOut_ = hout;				
			}));
		}

	}));
	
	Wt::WCalendar *c2 = new Wt::WCalendar();
	c2->setSelectionMode(Wt::ExtendedSelection);

	Wt::WText* out = new Wt::WText();
	out->addStyleClass("help-block");

	c2->selectionChanged().connect(std::bind([=]() {
		Wt::WString selected;
		std::set<Wt::WDate> selection = c2->selection();

		for (std::set<Wt::WDate>::const_iterator it = selection.begin();
			it != selection.end(); ++it) {
			if (!selected.empty())
				selected += ", ";

			const Wt::WDate& d = *it;
			Wt::log("notice") << "Pre-Converted Date DATE:" << d.toString("MM/dd/yyyy");
			selected = d.toString("MM/dd/yyyy");
			StudentHoursWidget::selectedDateOut_ = d;
			Wt::log("notice") << "Selected Date Out DATE:" << selectedDateOut_.toString("MM/dd/yyyy");
		}

		out->setText(Wt::WString::fromUTF8
			("{1}")
			.arg(selected));


	}));

	Wt::WLabel *calLbl = new Wt::WLabel("Select the date you volunteered.");	
	
	vbox2->addWidget(c2);
	vbox2->addWidget(out);
	
	container->show();
	
	
	Wt::WPushButton *ok = new Wt::WPushButton("Save", dialog->footer());
	ok->setDefault(true);

	Wt::WPushButton *cancel = new Wt::WPushButton("Cancel", dialog->footer());
	dialog->rejectWhenEscapePressed();
	
	ok->clicked().connect(std::bind([=]() {
		StudentHoursWidget::descriptionOut_ = descriptionTa->text().toUTF8();
		//get practice details from practice model
		Wt::log("notice") << "(Selected Date" << StudentHoursWidget::selectedDateOut_.toString("MM/dd/yyyy");
		Wt::log("notice") << "(Start Time: " << StudentHoursWidget::startTimeOut_;
		Wt::log("notice") << "(End Time: " << StudentHoursWidget::endTimeOut_;
		Wt::log("notice") << "(Total Time: " << StudentHoursWidget::totalTimeOut_;
		Wt::log("notice") << "(Description: " << StudentHoursWidget::descriptionOut_;

		Wt::WString studentFirst = session_->strFirstName();
		Wt::WString studentLast = session_->strLastName();
		Wt::WString studentId = session_->strStudentId();
		Wt::WString studid = session_->strStudentId();

		Wt::Dbo::Transaction htrans(dbsession);

		Wt::Dbo::ptr<Hours> h = dbsession.add(new Hours());

		h.modify()->hoursDate_ = StudentHoursWidget::selectedDateOut_;
		h.modify()->hoursStartTime_ = StudentHoursWidget::startTimeOut_;
		h.modify()->hoursEndTime_ = StudentHoursWidget::endTimeOut_;
		h.modify()->totalHours_ = StudentHoursWidget::totalTimeOut_;
		h.modify()->hoursDescription_ = StudentHoursWidget::descriptionOut_;
	
		h.modify()->studentFirstName_ = studentFirst.toUTF8();
		h.modify()->studentLastName_ = studentLast.toUTF8();
		h.modify()->studentId_ = studid.toUTF8();

		h.modify()->hoursId_ = staticHours_->text().toUTF8();

		htrans.commit();
		
		dialog->accept();
	
	}));
	
	cancel->clicked().connect(dialog, &Wt::WDialog::reject);

	dialog->show();

}
