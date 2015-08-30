/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#include "StudentRegistrationView.h"
#include "StudentDetailsModel.h"

#include <Wt/WLineEdit>

using namespace Wt;

StudentRegistrationView::StudentRegistrationView(StudentSession& session,
	Wt::Auth::AuthWidget *authWidget)
	: Wt::Auth::RegistrationWidget(authWidget),
	session_(session)
{
	setTemplateText(tr("template.registration"));
	detailsModel_ = new StudentDetailsModel(session_, this);

	updateView(detailsModel_);
}

Wt::WFormWidget *StudentRegistrationView::createFormWidget(Wt::WFormModel::Field field)
{
	if (field == StudentDetailsModel::StudentFirstName)
		return new Wt::WLineEdit();
	else if (field == StudentDetailsModel::StudentLastName)
		return new Wt::WLineEdit();
	else
		return Wt::Auth::RegistrationWidget::createFormWidget(field);
}

bool StudentRegistrationView::validate()
{
	bool result = Wt::Auth::RegistrationWidget::validate();

	updateModel(detailsModel_);
	if (!detailsModel_->validate())
		result = false;
	updateView(detailsModel_);

	return result;
}

void StudentRegistrationView::registerUserDetails(Wt::Auth::User& student)
{
	Wt::log("notice") << "registerStudentDetails has been called";
	detailsModel_->save(student);
}
