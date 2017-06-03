#include "Database.h"

using namespace std;

int Database::counter = 0;
int Database::selectCounter = 0;

Database::Database()
{
    rc = sqlite3_open(FILENAME, &dbFile);

    if (rc)
    {
        cout << "Can't open database: " << sqlite3_errmsg(dbFile) << endl;
    }
}
Database::~Database()
{

    sqlite3_close(dbFile);
}

int Database::countCallback(void *data, int argc, char **argv, char **movieCols)
{
    if (argv[0])
        counter++;
    return 0;
}

int Database::moviesCallback(void *data, int argc, char **argv, char **movieCols)
{

    string
        id = argv[0],
        title = argv[1],
        producer = argv[2],
        year = argv[3],
        info = argv[4],
        imdb = argv[5];
    Movie movie(id, title, producer, year, info, imdb);
    cout << movie.ToJson();

    if (++selectCounter != counter)
        cout << ",";
    return 0;
};

int Database::locationsCallback(void *data, int argc, char **argv, char **movieCols)
{
    //TODO ADD ID TO THE LOCATION CLASS
    string
        id = argv[0],
        country = argv[1],
        city = argv[2],
        address = argv[3];

    Location location(id,country, city,address);
    cout << location.ToJson();

    if (++selectCounter != counter)
        cout << ",";
    return 0;
};

int Database::keyCallback(void *data, int argc, char **argv, char **keyCol)
{
    string key = argv[0];

    cout << "\"" << key << "\"";

    return 1; //Force no more than one key for a user
};

int Database::ordersCallback(void *data, int argc, char **argv, char **keyCol)
{
    string 
        orderID = argv[0],
        movieID = argv[1],
        locationID = argv[2],
        canceledStatus = argv[3],
        key = argv[4];
    
    int isCanceled = canceledStatus.compare("1");
    bool canceled = isCanceled==0;
    
    Order order(orderID,movieID,locationID,canceled,key);

    cout << order.ToJson();
    if (++selectCounter != counter)
        cout << ",";
    return 0;
};

void Database::selectAllMoviesQuery()
{

    counter = selectCounter = 0;
    rc = sqlite3_exec(dbFile, "SELECT * FROM MOVIES", countCallback, (void *)data, &errMsg);

    cout << "{\"status\":\"success\",\"movies\":[";
    rc = sqlite3_exec(dbFile, "SELECT * FROM MOVIES", moviesCallback, (void *)data, &errMsg);
    cout << "]}";

    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
};

void Database::selectAllLocationsQuery()
{

    counter = selectCounter = 0;
    rc = sqlite3_exec(dbFile, "SELECT * FROM LOCATIONS", countCallback, (void *)data, &errMsg);
    cout << "{\"status\":\"success\",\"locations\":[";
    rc = sqlite3_exec(dbFile, "SELECT * FROM LOCATIONS", locationsCallback, (void *)data, &errMsg);
    cout << "]}";
    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
};

void Database::selectMovieByID(string movieID)
{
    counter = selectCounter = 0;
    string query = "SELECT * FROM MOVIES WHERE ID = " + movieID;

    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    cout << "{\"status\":\"success\",\"movies\":[";
    rc = sqlite3_exec(dbFile, query.c_str(), moviesCallback, (void *)data, &errMsg);
    cout << "]}";

    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
}

void Database::selectMovieByTitle(string movieTitle)
{
    counter = selectCounter = 0;
    string query = "SELECT * FROM MOVIES WHERE TITLE LIKE '" + movieTitle + "%'";

    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    cout << "{\"status\":\"success\",\"movies\":[";
    rc = sqlite3_exec(dbFile, query.c_str(), moviesCallback, (void *)data, &errMsg);
    cout << "]}";

    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
}

void Database::selectLocationByCity(string cityName)
{
    counter = selectCounter = 0;
    string query = "SELECT * FROM LOCATIONS WHERE CITY LIKE '" + cityName + "%'";

    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    cout << "{\"status\":\"success\",\"locations\":[";
    rc = sqlite3_exec(dbFile, query.c_str(), locationsCallback, (void *)data, &errMsg);
    cout << "]}";

    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
}

void Database::selectLocationByCountry(string countryName)
{
    counter = selectCounter = 0;
    string query = "SELECT * FROM LOCATIONS WHERE COUNTRY LIKE '" + countryName + "%'";

    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    cout << "{\"status\":\"success\",\"locations\":[";
    rc = sqlite3_exec(dbFile, query.c_str(), locationsCallback, (void *)data, &errMsg);
    cout << "]}";

    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
}

void Database::selectLocationByID(string locationID)
{
    counter = selectCounter = 0;
    string query = "SELECT * FROM LOCATIONS WHERE ID = " + locationID;

    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    cout << "{\"status\":\"success\",\"locations\":[";
    rc = sqlite3_exec(dbFile, query.c_str(), locationsCallback, (void *)data, &errMsg);
    cout << "]}";

    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
}

void Database::selectKey(string username, string password)
{
    counter = selectCounter = 0;
    string query = "SELECT KEY FROM USERS WHERE NAME = '" + username + "' AND PASSWORD = '" + password + "';";
    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);
    if (!counter)
        cout << "{\"status\":\"failure\",\"message\":\"User does not exist\"}";

    else
    {
        cout << "{\"status\":\"success\",\"key\":";
        rc = sqlite3_exec(dbFile, query.c_str(), keyCallback, (void *)data, &errMsg);
        cout << "}";
    }
    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
};

string Database::createKey(string username, string password)
{
    string key = "";
    for(int i = 0; i < username.size(); ++i){
        switch(username[i]){
            case 'a':
            case 'A':
                key += "4";
                break;
            case 'b':
            case 'B':
                key += "8";
                break;
            case 'e':
            case 'E':
                key += "3";
                break;
            case 'g':
            case 'G':
                key += "6";
                break;
            case 'l':
            case 'L':
                key += "1";
                break;
            case 'o':
            case 'O':
                key += "0";
                break;
            case 's':
            case 'S':
                key += "5";
                break;
            case 't':
            case 'T':
                key += "7";
                break;
            case 'z':
            case 'Z':
                key += "2";
                break;
            default:
                key += toupper(username[i]);
        }
    }
    key += "K3Y";
    for(int i = password.size() - 1; i >= 0; --i)
        key += toupper(password[i]);
    return key;
}

void Database::createAccount(string username, string password)
{
    counter = selectCounter = 0;
    string query = "SELECT KEY FROM USERS WHERE NAME = '" + username + "' AND PASSWORD = '" + password + "';";
    //Checking if username and password already exist
    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    if (counter)
    {
        cout << "{\"status\":\"failure\",\"message\":\"User already exists!\"}";
        if (rc != SQLITE_OK)
            sqlite3_free(errMsg);
    }
    else
    {
        counter = selectCounter = 0;
        string key = createKey(username, password);
        query = "INSERT INTO USERS VALUES('";
        query += key;
        query += "','";
        query += username;
        query += "','";
        query += password;
        query += "');";
        rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

        //Returning key if successful
        if (rc == SQLITE_OK)
            selectKey(username, password);
        else
            sqlite3_free(errMsg);
    }
}

void Database::selectOrdersByKey(string key){
    counter = selectCounter = 0;
    string query = "SELECT * FROM ORDERS WHERE USERKEY = '" + key + "'";
    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);
   
    cout << "{\"status\":\"success\",\"orders\":[";
    rc = sqlite3_exec(dbFile, query.c_str(), ordersCallback, (void *)data, &errMsg);
    cout << "]}";
    
    if (rc != SQLITE_OK)
        sqlite3_free(errMsg);
}

void Database::createOrder(string key, string movieID, string locationID){
    counter = selectCounter = 0;
    //Checking Movie exists:
    string  
    queryCheckMovie = "SELECT ID FROM MOVIES WHERE ID = " + movieID + ";",
    queryCheckLocation = "SELECT ID FROM LOCATIONS WHERE ID = " + locationID + ";",
    query = "INSERT INTO ORDERS(MOVIEID,LOCATIONID,USERKEY)VALUES('";
    query += movieID;
    query += ",";
    query += locationID;
    query += ",'";
    query += key;
    query += "');";    

    rc = sqlite3_exec(dbFile, queryCheckMovie.c_str(), countCallback, (void *)data, &errMsg);
    if (counter == 0){ cout << "{\"status\":\"failure\",\"message\":\"Movie does not exist!\"}"; return; }
    counter = 0;

    rc = sqlite3_exec(dbFile, queryCheckLocation.c_str(), countCallback, (void *)data, &errMsg);
    if (counter == 0){ cout << "{\"status\":\"failure\",\"message\":\"Location does not exist!\"}"; return; }
    

    rc = sqlite3_exec(dbFile, query.c_str(), countCallback, (void *)data, &errMsg);

    cout << "Tickets successfuly ordered"; //TEMPORARY , WILL USE ORDER INSTEAD
}