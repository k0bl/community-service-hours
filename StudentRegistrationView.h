/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_REGISTRATION_VIEW_H_
#define STUDENT_REGISTRATION_VIEW_H_

#include <Wt/Auth/RegistrationWidget>

class StudentSession;
class StudentDetailsModel;

class StudentRegistrationView : public Wt::Auth::RegistrationWidget
{
public:
	StudentRegistrationView(StudentSession& session, Wt::Auth::AuthWidget *authWidget = 0);

	/* specialize to create user details fields */
	virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);

	/* specialize to also validate the user details */
	virtual bool validate();

	/* specialize to register user details */
	virtual void registerUserDetails(Wt::Auth::User& student);

private:
	StudentSession& session_;

	StudentDetailsModel *detailsModel_;
};

#endif // STUDENT_REGISTRATION_VIEW_H_
