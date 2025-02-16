#include "User.h"
#include <cppcms/service.h>  
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/http_response.h>
#include <cppdb/frontend.h>
#include <picojson.h>

// constructor
User::User(cppcms::service &srv) : Master(srv)
{
    dispatcher().assign("", &User::getUser, this);
    mapper().assign("");

    dispatcher().assign("/(\\d+)", &User::getUserById, this, 1);
    mapper().assign("/{1}");
}

void User::getUser()
{   
    try
    {
        std::cout << "Checking database connection..." << std::endl;
        if (!sql().is_open()) {
            std::cerr << "Database connection is NOT open!" << std::endl;
            response().out() << "Database connection error!";
            return;
        }

        cppdb::result res = sql() << "SELECT usrsFirstName, usrsLoginId FROM users";
        picojson::array usersArray;

        while(res.next()){
            std::string usrsFirstName, usrsLoginId;
            res.fetch(0, usrsFirstName);
            res.fetch(1, usrsLoginId);

            picojson::object user;
            user["firstName"] = picojson::value(usrsFirstName);
            user["loginId"] = picojson::value(usrsLoginId);

            usersArray.push_back(picojson::value(user));
        }

        picojson::value jsonResponse(usersArray);
        std::string jsonString = jsonResponse.serialize();

        // Send JSON response
        response().content_type("application/json");
        response().out() << jsonString;

    } catch (const std::exception &e) {
        response().status(500);
        response().out() << "{\"error\": \"Database query failed: " << e.what() << "\"}";
    }
}

void User::getUserById(std::string id)
{
    try {
        int userId = std::stoi(id);
        
        std::cout << "Checking database connection..." << std::endl;
        if (!sql().is_open()) {
            std::cerr << "Database connection is NOT open!" << std::endl;
            response().out() << "Database connection error!";
            return;
        }
        
        std::cout << "Executing query for userId: " << userId << std::endl;

        cppdb::result res = sql() << "SELECT usrsFirstName, usrsLoginId FROM users WHERE usrsId=?" 
            << userId;

        bool hasResult = res.next();
        std::cout << "res.next() returned: " << hasResult << std::endl;
        
        if (hasResult) {
            std::cout << res.get<std::string>("usrsFirstName") << " is " << res.get<std::string>("usrsLoginId") << " loginID" << std::endl;
            std::string usrsFirstName, usrsLoginId;
            res.fetch(0, usrsFirstName);
            res.fetch(1, usrsLoginId);

            response().out() 
                             << ", \"First Name\": \"" << usrsFirstName
                             << "\", \"Login ID\": \"" << usrsLoginId << "\" }";
        } else {
            response().status(404);
            response().out() << "{\"error\": \"User not found\"}";
        }
    } catch (const std::exception &e) {
        response().status(500);
        response().out() << "{\"error\": \"Database query failed: " << e.what() << "\"}";
    }
    // response().out() <<"get user by id "<< id;
}
