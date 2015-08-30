/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_AUTH_WIDGET_H_
#define STUDENT_AUTH_WIDGET_H_

#include <Wt/Auth/AuthWidget>
#include <Wt/WContainerWidget>

class StudentSession;

class StudentAuthWidget : public Wt::Auth::AuthWidget
{
public:
	StudentAuthWidget(StudentSession& session, Wt::WContainerWidget*parent = 0);

	/* We will use a custom registration view */
	virtual Wt::WWidget *createRegistrationView(const Wt::Auth::Identity& id);

private:
	StudentSession& session_;
};

#endif // STUDENT_AUTH_WIDGET_H_
