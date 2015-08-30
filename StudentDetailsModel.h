/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_DETAILS_MODEL_H_
#define STUDENT_DETAILS_MODEL_H_

#include <Wt/WFormModel>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp> 
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>

class StudentSession;

class StudentDetailsModel : public Wt::WFormModel
{
public:
  static const Field StudentFirstName;
  static const Field StudentLastName;
  static const Field StudentId;

  Wt::WText *staticPrac_;
  Wt::WString setPracId();
  Wt::WString strPracId();

  StudentDetailsModel(StudentSession& session, Wt::WObject *parent = 0);

  void save(const Wt::Auth::User& Student);

private:
  StudentSession& session_;
  
  Wt::WString uuid() {
	  boost::uuids::uuid uuidx = boost::uuids::random_generator()();

	  return boost::lexical_cast<std::string>(uuidx);
  }

};

#endif // STUDENT_DETAILS_MODEL