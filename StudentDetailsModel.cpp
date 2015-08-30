/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#include <Wt/WText>
#include <Wt/WApplication>
#include "StudentDetailsModel.h"
#include "Student.h"
#include "StudentSession.h"
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>

const Wt::WFormModel::Field StudentDetailsModel::StudentFirstName = "student-firstname";
const Wt::WFormModel::Field StudentDetailsModel::StudentLastName = "student-lastname";

namespace dbo = Wt::Dbo;

StudentDetailsModel::StudentDetailsModel(StudentSession& session, Wt::WObject *parent)
	: Wt::WFormModel(parent),
	session_(session)
{
	addField(StudentFirstName, Wt::WString::tr("student-firstname-info"));
	addField(StudentLastName, Wt::WString::tr("student-lastname-info"));


	staticPrac_ = new Wt::WText();
	staticPrac_->setText(setPracId());
}

WString StudentDetailsModel::setPracId()
{
	return uuid();
}

WString StudentDetailsModel::strPracId()
{
	return staticPrac_->text();
}

void StudentDetailsModel::save(const Wt::Auth::User& student)
{

	std::stringstream ss;
	ss << "Generated Student ID: " + strPracId();
	std::cout << ss;

	Wt::Dbo::ptr<Student> stud = session_.student(student);

	stud.modify()->studentFirst_ = valueText(StudentFirstName).toUTF8();
	stud.modify()->studentLast_ = valueText(StudentLastName).toUTF8();
	stud.modify()->studentId_ = strPracId().toUTF8();

}
