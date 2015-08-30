/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_HOME_WIDGET_H_
#define STUDENT_HOME_WIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include "StudentSession.h"

using namespace Wt::Dbo;
using namespace Wt;

class StudentSession;
class StudentHomeWidget : public Wt::WContainerWidget
{
public:
	StudentHomeWidget(StudentSession *session, Wt::WContainerWidget *parent = 0);
private:
	StudentSession *session_;
};

#endif STUDENT_HOME_WIDGET_H_
