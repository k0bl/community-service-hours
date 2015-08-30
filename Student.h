/*
* Copyright (C) 2015 Cody Scherer <cody@pes7.com> PES7, Inc | Long Beach, CA.
*
* See the LICENSE file for terms of use.
*
* This may look like C code but its really C++
*/

#ifndef STUDENT_H_
#define STUDENT_H_

#include <Wt/Dbo/Types>
#include <Wt/WString>
#include <Wt/WDate>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/WGlobal>

namespace dbo = Wt::Dbo;

class Student;
typedef Wt::Auth::Dbo::AuthInfo<Student> StudentAuthInfo;

class Student {
public:
	/* You probably want to add other Student information here */
	std::string studentFirst_;
	std::string studentLast_;
	std::string studentId_;
	
	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, studentFirst_, "first_name");
		dbo::field(a, studentLast_, "last_name");
		dbo::field(a, studentId_, "student_id");
	}
};

#endif // Student_H_