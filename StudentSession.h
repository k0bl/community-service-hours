/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENTSESSION_H_
#define STUDENTSESSION_H_

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>
#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"
#include <Wt/WString>
#include "Wt/Auth/Login"
#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>
#include "Student.h"

namespace dbo = Wt::Dbo;
using namespace Wt;

typedef Wt::Auth::Dbo::UserDatabase<StudentAuthInfo> StudentDatabase;

class StudentSession : public dbo::Session
{
public:
	static void configureAuth();
	StudentSession(const std::string& sqliteDb);

	dbo::ptr<Student> student();
	dbo::weak_ptr<Student> wstudent();
	dbo::ptr<Student> student(const Wt::Auth::User& student);

	Wt::Auth::AbstractUserDatabase& students();
	Wt::Auth::Login& login() { return login_; }

	static const Wt::Auth::AuthService& auth();
	static const Wt::Auth::PasswordService& passwordAuth();
	

	Wt::WString strFirstName();
	Wt::WString strLastName();
	Wt::WString strStudentId();
	

private:
	dbo::backend::Sqlite3 connection_;
	StudentDatabase *students_;
	Wt::Auth::Login login_;

};
#endif // STUDENTSESSION_H_
