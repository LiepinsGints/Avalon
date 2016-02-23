#ifndef MySql_H
#define MySql_H


#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <vector>

#include "AppSettings.h"
//Db models
#include "Models.h"


class MySql 
{
public:
	MySql(AppSettings* appSettings) {
		_appSettings = appSettings;
	
	}
	~MySql() {
		delete res;
		delete stmt;
		delete con;
	}

	bool mySqlConnect() {
		try {
			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect(_appSettings->getMySqlAdress(), _appSettings->getMySqlUserName(), _appSettings->getMySqlPassword());
			/* Connect to the MySQL test database */
			con->setSchema(_appSettings->getDbName());
			return true;
		}
		catch (sql::SQLException &e) {
			return false;
		}
	}
	Ogre::String executeQuery(Ogre::String query) {
		try {
			stmt = con->createStatement();
			res = stmt->executeQuery(query); // replace with your statement
			while (res->next()) {
				//cout << "\t... MySQL replies: ";
				/* Access column data by alias or column name */
				//cout << res->getString("_message") << endl;
				//cout << "\t... MySQL says it again: ";
				/* Access column fata by numeric offset, 1 is the first column */
				//cout << res->getString(1) << endl;
			}
		}
		catch (sql::SQLException &e) {
			
			Ogre::String error = "Error";
			return error;
			
		}
	}

	//Get model from db
	Models * getModelByName(std::string modelName) {
		Models * model = new Models();
		try {
			stmt = con->createStatement();
			std::string query= "select * from models where Name = '"+ modelName +"'" ;
			res = stmt->executeQuery(query.c_str()); // replace with your statement
			while (res->next()) {
				model->setId(std::stoi(res->getString("ID").c_str()));
				model->setName(res->getString("Name").c_str());		
				model->setMeshName(res->getString("MeshName").c_str());
				model->setDimensions(Ogre::Vector3(std::stod(res->getString("Width").c_str()), std::stod(res->getString("Height").c_str()), std::stod(res->getString("Depth").c_str())));
				break;
			}
			return model;
		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			model->setName("Error");
			return model;

		}
	}
private:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;

	AppSettings* _appSettings;



};

#endif