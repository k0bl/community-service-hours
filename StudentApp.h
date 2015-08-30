/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENTAPP_H_
#define STUDENTAPP_H_

#include <Wt/WApplication>
#include <Wt/WString>
#include <Wt/Auth/AuthWidget>
#include <Wt/WContainerWidget>
#include <Wt/WAnchor>
#include "StudentHomeWidget.h"
#include "StudentHoursWidget.h"
#include "StudentPrintWidget.h"
#include "StudentSession.h"


using namespace Wt;

class StudentApplication : public Wt::WApplication
{

public:
	StudentApplication(const Wt::WEnvironment& env);
	void handleInternalPath(const std::string &internalPath);

private:
	Wt::WStackedWidget *mainStack_;
	void authEvent();
	void showHome();
	void showHours();
	void showPrint();

	StudentHomeWidget *home_;
	StudentHoursWidget *hours_;
	StudentPrintWidget *print_;

	Wt::WContainerWidget *links_;
	StudentSession session_;

	Wt::WAnchor *homeAnchor_;
	Wt::WAnchor *hoursAnchor_;
	Wt::WAnchor *printAnchor_;	

};

WApplication *createStudentApplication(const WEnvironment& env);

#endif //STAFFAPP_H_