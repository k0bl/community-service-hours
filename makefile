CC=g++
CXXFLAGS+=-O3 -std=c++0x -pg -DDEBUG -g -c -Wall 

servicehours: AdminApp.o AdminHomeWidget.o HoursResultWidget.o Student.o StudentApp.o StudentAuthWidget.o StudentDetailsModel.o StudentHomeWidget.o StudentHoursWidget.o StudentPrintWidget.o StudentRegistrationView.o StudentSession.o UserSession.o servicehours.o 	
		$(CC) -o servicehours -std=c++0x -g -DDEBUG AdminApp.cpp AdminHomeWidget.cpp HoursResultWidget.cpp Student.cpp StudentApp.cpp StudentAuthWidget.cpp StudentDetailsModel.cpp StudentHomeWidget.cpp StudentHoursWidget.cpp StudentPrintWidget.cpp StudentRegistrationView.cpp StudentSession.cpp UserSession.cpp servicehours.cpp -I /usr/local/include -L/usr/local/lib -lhpdf -lwtdbosqlite3d -lwtdbod -lwthttpd -lwtd -lboost_random -lboost_regex -lboost_signals -lboost_system -lboost_thread -lboost_filesystem -lboost_program_options -lboost_date_time
