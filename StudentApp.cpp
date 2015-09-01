/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WBootstrapTheme>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>
#include <Wt/WText>
#include <Wt/WContainerWidget>
#include <Wt/WAnchor>
#include <Wt/WStackedWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WImage>
#include <Wt/WLink>

#include <Wt/WMenu>
#include <Wt/WMessageBox>
#include <Wt/WBorderLayout>
#include <Wt/WNavigationBar>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>
#include <Wt/WStackedWidget>

#include "StudentApp.h"
#include "StudentAuthWidget.h"
#include "StudentRegistrationView.h"
#include "StudentSession.h"
#include "StudentHomeWidget.h"
#include "StudentHoursWidget.h"
#include "StudentPrintWidget.h"

using namespace Wt;

StudentApplication::StudentApplication(const Wt::WEnvironment& env)
	: Wt::WApplication(env),
	session_(appRoot() + "studentauth.db")
{
	session_.login().changed().connect(this, &StudentApplication::authEvent);
	messageResourceBundle().use("templates");
	messageResourceBundle().use("composer");
	messageResourceBundle().use("strings");	
	messageResourceBundle().use("auth_strings");
	root()->addStyleClass("container");
	setTheme(new Wt::WBootstrapTheme());

	require("js/jquery.js");
	require("js/sketch.js");

	useStyleSheet("css/style.css");
	useStyleSheet("css/composer.css");
	
	//setOverflow(OverflowHidden);

	Wt::WContainerWidget *container = new Wt::WContainerWidget();

	Wt::WText *toplogo = new Wt::WText("<h2>Kearsarge Conservatory of the Performing Arts</h2>");
	toplogo->setStyleClass("toplogo");
	root()->addWidget(toplogo);

	root()->addWidget(container);
	
	Wt::Auth::AuthWidget *authWidget
		= new StudentAuthWidget(session_, container);
	
	authWidget->model()->addPasswordAuth(&StudentSession::passwordAuth());
	authWidget->setRegistrationEnabled(true);
	authWidget->setStyleClass("app-auth-widget");
	
	links_ = new WContainerWidget();

	links_->setStyleClass("links");
	root()->addWidget(links_);

	Wt::WNavigationBar *navigation = new Wt::WNavigationBar(links_);
	navigation->setTitle("Volunteer Hours Application",
		"https://127.0.0.1/students/home");
	navigation->setResponsive(true);	
	navigation->setStyleClass("top-navigation");

	Wt::WStackedWidget *contentsStack = new Wt::WStackedWidget();
  	
	//contentsStack->addStyleClass("contents");

	// Setup a Left-aligned menu.
	Wt::WMenu *leftMenu = new Wt::WMenu(Wt::Vertical);
	//navigation->addMenu(leftMenu);
	
	leftMenu->setStyleClass("nav nav-pills nav-stacked app-left-menu");
	leftMenu->setWidth(150);

	leftMenu->addItem("Home")
		->setLink(Wt::WLink(Wt::WLink::InternalPath, "/home"));		
	leftMenu->addItem("View Hours")
		->setLink(Wt::WLink(Wt::WLink::InternalPath, "/hours"));
	leftMenu->addItem("Print Hours")
		->setLink(Wt::WLink(Wt::WLink::InternalPath, "/print"));

	Wt::WMenu *rightMenu = new Wt::WMenu();
	navigation->addMenu(rightMenu, Wt::AlignRight);

	authWidget->processEnvironment();

	mainStack_ = new WStackedWidget();
	mainStack_->setStyleClass("app-main-stack");
	navigation->addWidget(authWidget, Wt::AlignRight);
	container->addWidget(authWidget);
	container->setStyleClass("app-main-container");
	Wt::WContainerWidget *container2 = new Wt::WContainerWidget(links_);
	container2->setStyleClass("app-main-container2");
	Wt::WContainerWidget *menucontainer = new Wt::WContainerWidget(links_);
	menucontainer->setStyleClass("app-menu-container");
	menucontainer->addWidget(leftMenu);
	Wt::WHBoxLayout *hbox = new Wt::WHBoxLayout();
	
	container2->setLayout(hbox);
	
	hbox->addWidget(menucontainer);
	hbox->addWidget(mainStack_, 1);

	links_->addWidget(container2);
	
	links_->hide();

	WApplication::instance()->internalPathChanged()
		.connect(this, &StudentApplication::handleInternalPath);
}

void StudentApplication::authEvent() {

	if (session_.login().loggedIn()){
		links_->show();
		handleInternalPath(WApplication::instance()->internalPath());
		Wt::log("notice") << "Student user logged in.";
		Wt::Dbo::Transaction t(session_);
		dbo::ptr<Student> user = session_.student();
		Wt::log("notice") << "(The student: " << user->studentFirst_ << "logged in)";
	}
	else {
		mainStack_->clear(),
			links_->hide(),
			this->refresh();
			Wt::log("notice") << "Student user logged out.";
			this->redirect("http://127.0.0.1:8080/providers");
	}
}

void StudentApplication::handleInternalPath(const std::string &internalPath)
{
	if (session_.login().loggedIn()) {
		if (internalPath == "/home")
			showHome();
		else if (internalPath == "/hours")
			showHours();
		else if (internalPath == "/print")
			showPrint();
		else
			WApplication::instance()->setInternalPath("/home", true);
	}
}

void StudentApplication::showHome()
{
	mainStack_->clear();
	home_ = new StudentHomeWidget(&session_, mainStack_);
	mainStack_->setCurrentWidget(home_);
}

void StudentApplication::showHours()
{
	mainStack_->clear();
	hours_ = new StudentHoursWidget(&session_, mainStack_);
	hours_->hoursList();
	mainStack_->setCurrentWidget(hours_);
}

void StudentApplication::showPrint()
{
	mainStack_->clear();
	print_ = new StudentPrintWidget(&session_, mainStack_);
	print_->hoursList();
	mainStack_->setCurrentWidget(print_);
}

WApplication *createStudentApplication(const WEnvironment& env)
{
	return new StudentApplication(env);
}