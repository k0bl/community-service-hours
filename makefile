CC=g++
CXXFLAGS+=-O3 -std=c++0x -pg -g -DDEBUG -c -Wall 

servicehours: HoursResultWidget.o RecentHoursWidget.o ReportResource.o Student.o StudentApp.o StudentAuthWidget.o StudentDetailsModel.o StudentHomeWidget.o StudentHoursWidget.o StudentPrintWidget.o StudentRegistrationView.o StudentSession.o servicehours.o 	
		$(CC) -o servicehours -std=c++0x -g -DDEBUG HoursResultWidget.cpp RecentHoursWidget.cpp ReportResource.cpp Student.cpp StudentApp.cpp StudentAuthWidget.cpp StudentDetailsModel.cpp StudentHomeWidget.cpp StudentHoursWidget.cpp StudentPrintWidget.cpp StudentRegistrationView.cpp StudentSession.cpp servicehours.cpp -I /usr/local/include -L /usr/local/lib -lhpdf -lwtdbopostgres -lwtdbod -lwthttpd -lwtd -lboost_random -lboost_regex -lboost_signals -lboost_system -lboost_thread -lboost_filesystem -lboost_program_options -lboost_date_time
