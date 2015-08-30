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
#include <Wt/WNavigationBar>
#include <Wt/WPopupMenu>
#include <Wt/WPopupMenuItem>

#include "AdminApp.h"
#include "AdminHomeWidget.h"

AdminApplication::AdminApplication(const Wt::WEnvironment& env)
	: Wt::WApplication(env),
	session_(appRoot() + "auth.db")
{
	session_.login().changed().connect(this, &AdminApplication::authEvent);

	root()->addStyleClass("container");
	setTheme(new Wt::WBootstrapTheme());

	messageResourceBundle().use("templates");
	messageResourceBundle().use("strings");
	useStyleSheet("css/style.css");
	Wt::WContainerWidget *container = new Wt::WContainerWidget();

	Wt::WImage *image = new Wt::WImage(Wt::WLink("images/logo.png"),
		container);
	image->setAlternateText("usc logo");
	image->show();
	root()->addWidget(container);
	
	Wt::WMenu *rightMenu = new Wt::WMenu();
	
	Wt::Auth::AuthWidget *authWidget
		= new Wt::Auth::AuthWidget(UserSession::auth(), session_.users(),
		session_.login());

	authWidget->model()->addPasswordAuth(&UserSession::passwordAuth());
	authWidget->setRegistrationEnabled(true);

	links_ = new WContainerWidget();
	links_->setStyleClass("links");
	root()->addWidget(links_);

	//nav
	Wt::WNavigationBar *navigation = new Wt::WNavigationBar(links_);
	navigation->setTitle("Adminnician Portal",
		"https://127.0.0.1/admins");
	navigation->setResponsive(true);

	Wt::WStackedWidget *contentsStack = new Wt::WStackedWidget();
	contentsStack->addStyleClass("contents");

	// Setup a Left-aligned menu.
	Wt::WMenu *leftMenu = new Wt::WMenu(Wt::Vertical, links_);
	//navigation->addMenu(leftMenu);

	leftMenu->setStyleClass("nav nav-pills nav-stacked left-menu");

	leftMenu->addItem("Home")
		->setLink(Wt::WLink(Wt::WLink::InternalPath, "/home"));
	
	navigation->addMenu(rightMenu, Wt::AlignRight);
	//end nav
	

	authWidget->processEnvironment();

	mainStack_ = new WStackedWidget();
	
	navigation->addWidget(authWidget, Wt::AlignRight);
	container->addWidget(authWidget);
	

	Wt::WContainerWidget *container2 = new Wt::WContainerWidget(links_);
	Wt::WHBoxLayout *hbox = new Wt::WHBoxLayout();
	
	container2->setLayout(hbox);
	
	hbox->addWidget(leftMenu);
	hbox->addWidget(mainStack_);

	links_->addWidget(container2);
	
	links_->hide();
	
	WApplication::instance()->internalPathChanged()
		.connect(this, &AdminApplication::handleInternalPath);

}



void AdminApplication::authEvent() {

	if (session_.login().loggedIn()){
		links_->show();
		handleInternalPath(WApplication::instance()->internalPath());
		Wt::log("notice") << "Admin user logged in.";

	}
	else {
		mainStack_->clear();
		links_->hide();
		Wt::log("notice") << "Admin user logged out.";
		this->refresh();
		this->redirect("https://127.0.0.1/admin");
	}
}

void AdminApplication::handleInternalPath(const std::string &internalPath)
{
	if (session_.login().loggedIn()) {
		if (internalPath == "/home")
			showHome();
		else
			WApplication::instance()->setInternalPath("/home", true);
	}
}

void AdminApplication::showHome()
{
	mainStack_->clear();
	home_ = new AdminHomeWidget(mainStack_);
	mainStack_->setCurrentWidget(home_);
}


WApplication *createAdminApplication(const WEnvironment& env)
{
	return new AdminApplication(env);
}