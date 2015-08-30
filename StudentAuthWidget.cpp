/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#include "StudentAuthWidget.h"
#include "StudentRegistrationView.h"
#include "StudentSession.h"
#include <Wt/WContainerWidget>
StudentAuthWidget::StudentAuthWidget(StudentSession& session, Wt::WContainerWidget* parent)
	: Wt::Auth::AuthWidget(StudentSession::auth(), session.students(), session.login(), parent),
	session_(session)
{  }

Wt::WWidget *StudentAuthWidget::createRegistrationView(const Wt::Auth::Identity& id)
{
	StudentRegistrationView *w = new StudentRegistrationView(session_, this);
	Wt::Auth::RegistrationModel *model = createRegistrationModel();

	if (id.isValid())
		model->registerIdentified(id);

	w->setModel(model);
	
	return w;
}
