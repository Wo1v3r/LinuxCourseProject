#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include "Database.h"

using namespace std;

class RequestManager
{
private:
	static const string options[];
	static const int optionsAmount;

	enum OPT {REGISTER,KEY,ORDER,CANCEL,HISTORY,MOVIES,LOCATIONS };
	enum SUBOPT {BY_ID,BY_TITLE,BY_COUNTRY,BY_CITY};


	//Account management
	static void registerAccount(string , string );
	static void recoverKey(string , string );
	static void getHistory(string );

	////Ticket Management
	static void orderTicket(string, string ,string);
	static void cancelTicket(string ,string);

	////Information
	static void getMoviesList();
	static void getLocationsList();
	static void getMovieInfo(string , int);
	static void getLocationInfo(string, int);

	//Parser
	static bool  parseOneParam(string , string , string&);
	static bool  parseTwoParam(string , string , string , string&, string&);
	static bool  parseThreeParam(string , string , string ,string,string&, string&, string&);

public:
	static void parse(const char*);
};

#endif
