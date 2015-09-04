/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#include "StudentSession.h"
#include "Student.h"

#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

#include <Wt/Dbo/collection>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Postgres>
#include <string>

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"



using namespace Wt;

namespace {
	Wt::Auth::AuthService myAuthService;
	Wt::Auth::PasswordService myPasswordService(myAuthService);
}

void StudentSession::configureAuth()
{
	myAuthService.setAuthTokensEnabled(true, "logincookie");
	myAuthService.setEmailVerificationEnabled(true);

	Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
	verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
	myPasswordService.setVerifier(verifier);
	myPasswordService.setAttemptThrottlingEnabled(true);
	myPasswordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

}

StudentSession::StudentSession(const std::string& db)
	: connection_(db)
{
	setConnection(connection_);
	
	connection_.setProperty("show-queries", "true");

	mapClass<Student>("student");
	mapClass<StudentAuthInfo>("auth_info");
	mapClass<StudentAuthInfo::AuthIdentityType>("auth_identity");
	mapClass<StudentAuthInfo::AuthTokenType>("auth_token");

	try {
		createTables();
		std::cerr << "Created database." << std::endl;
	}
	catch (Wt::Dbo::Exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Using existing database";
	}

	students_ = new StudentDatabase(*this);
}
Wt::Auth::AbstractUserDatabase& StudentSession::students()
{
	return *students_;
}

dbo::ptr<Student> StudentSession::student() 
{
	if (login_.loggedIn()) {
		return student(login_.user());
	}
	else
		return dbo::ptr<Student>();
}

dbo::ptr<Student> StudentSession::student(const Wt::Auth::User& authUser)
{
	dbo::ptr<StudentAuthInfo> authInfo = students_->find(authUser);

	dbo::ptr<Student> student = authInfo->user();

	if (!student) {
		student = add(new Student());
		authInfo.modify()->setUser(student);
	}

	return student;
}

Wt::WString StudentSession::strFirstName()
{
	Wt::log("notice") << "StudentSession::strFirstName dbo::Transaction transaction(StudentSession)";
	Wt::Dbo::Transaction transaction(*this);
	Wt::log("notice") << "StudentSession::strFirstName dbo::ptr<Student> s = student()";
	dbo::ptr<Student> s = student();
	Wt::log("notice") << "StudentSession::strFirstName return s->studentFirst_";
	return s->studentFirst_;
	Wt::log("notice") << "StudentSession::strFirstName transaction.commit()";
}

Wt::WString StudentSession::strLastName()
{
	Wt::log("notice") << "StudentSession::strLastName dbo::Transaction transaction(StudentSession)";
	Wt::Dbo::Transaction transaction(*this);
	Wt::log("notice") << "StudentSession::strLastName dbo::ptr<Student> s = student()";
	dbo::ptr<Student> s = student();
	Wt::log("notice") << "StudentSession::strLastName return s->studentLast_";
	return s->studentLast_;
	Wt::log("notice") << "StudentSession::strLastName transaction.commit()";
}

Wt::WString StudentSession::strStudentId()
{
	Wt::log("notice") << "StudentSession::strStudenId dbo::Transaction transaction(StudentSession)";
	Wt::Dbo::Transaction transaction(*this);
	Wt::log("notice") << "StudentSession::strStudenId dbo::ptr<Student> s = student()";
	dbo::ptr<Student> s = student();
	Wt::log("notice") << "StudentSession::strStudenId return s->studentId";
	return s->studentId_;
	Wt::log("notice") << "StudentSession::strStudenId transaction.commit()";
}

const Wt::Auth::AuthService& StudentSession::auth()
{
	return myAuthService;
}

const Wt::Auth::PasswordService& StudentSession::passwordAuth()
{
	return myPasswordService;
}

