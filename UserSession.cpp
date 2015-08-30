/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#include "UserSession.h"
#include "User.h"

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>

#include "Wt/Auth/Dbo/AuthInfo"
#include "Wt/Auth/Dbo/UserDatabase"

#include "Wt/Auth/AuthService"
#include "Wt/Auth/HashFunction"
#include "Wt/Auth/PasswordService"
#include "Wt/Auth/PasswordStrengthValidator"
#include "Wt/Auth/PasswordVerifier"

using namespace Wt;
using namespace Wt::Dbo;

namespace {
	Wt::Auth::AuthService myAuthService;
	Wt::Auth::PasswordService myPasswordService(myAuthService);
}

void UserSession::configureAuth()
{
	myAuthService.setAuthTokensEnabled(true, "logincookie");
	myAuthService.setEmailVerificationEnabled(true);

	Wt::Auth::PasswordVerifier *verifier = new Wt::Auth::PasswordVerifier();
	verifier->addHashFunction(new Wt::Auth::BCryptHashFunction(7));
	myPasswordService.setVerifier(verifier);
	myPasswordService.setAttemptThrottlingEnabled(true);
	myPasswordService.setStrengthValidator(new Wt::Auth::PasswordStrengthValidator());

}

UserSession::UserSession(const std::string& sqliteDb)
	: connection_(sqliteDb)
{
	setConnection(connection_);

	mapClass<User>("user");
	mapClass<AuthInfo>("auth_info");
	mapClass<AuthInfo::AuthIdentityType>("auth_identity");
	mapClass<AuthInfo::AuthTokenType>("auth_token");

	try {
		createTables();
		std::cerr << "Created database." << std::endl;
	}
	catch (Wt::Dbo::Exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "Using existing database";
	}

	users_ = new UserDatabase(*this);
}
Wt::Auth::AbstractUserDatabase& UserSession::users()
{
	return *users_;
}

dbo::ptr<User> UserSession::user() const
{
	if (login_.loggedIn()) {
		dbo::ptr<AuthInfo> authInfo = users_->find(login_.user());
		return authInfo->user();
	}
	else
		return dbo::ptr<User>();
}

const Wt::Auth::AuthService& UserSession::auth()
{
	return myAuthService;
}

const Wt::Auth::PasswordService& UserSession::passwordAuth()
{
	return myPasswordService;
}

