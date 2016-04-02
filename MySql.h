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
#include "Spawns.h"
//Table models
#include "Models.h"
#include "SpawnWorld.h"
#include "Bounds.h"

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
class MySql 
{
public:
	MySql(AppSettings* appSettings) {
		_appSettings = appSettings;
	
	}
	~MySql() {
		
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
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str()); // replace with your statementt
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
		std::string query = "select * from models where Name = '" + modelName + "'";
		return getModel(query);
	}
	Models * getModelById(int modelID) {
		std::string query = "select * from models where id = " + std::to_string(modelID);
		return getModel(query);
	}
	Models * getModel(std::string query) {
		Models * model = new Models();
		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				model->setId(std::stoi(res->getString("ID").c_str()));
				model->setName(res->getString("Name").c_str());
				model->setMeshName(res->getString("MeshName").c_str());
				model->setDimensions(Ogre::Vector3(std::stod(res->getString("Width").c_str()), std::stod(res->getString("Height").c_str()), std::stod(res->getString("Depth").c_str())));
				//break;
			}
			return model;
		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			model->setName("Error");
			return model;

		}
	}
	void getModelList(MyGUI::ListBox * meshList) {
		std::string query = "select MeshName from models";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				meshList->addItem(res->getString("MeshName").c_str());
			}
		}
		catch (sql::SQLException &e) {

			

		}
	}
	//spawn all objects from world table
	void getWorld(Spawns* spawns) {
		SpawnWorld * spawnWorld = new SpawnWorld();
		std::string query = "select * from world";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str()); 
			while (res->next()) {

				spawns->createObject(getModelById(std::stoi(res->getString("ModelID").c_str())), 
					Ogre::Vector3(std::stod(res->getString("x").c_str()), std::stod(res->getString("y").c_str()), std::stod(res->getString("z").c_str())),
					std::stod(res->getString("Scale").c_str()), std::stod(res->getString("Mass").c_str()), std::stod(res->getString("Type").c_str()));

			}

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			//model->setName("Error");

		}
	}
	/************************Spawn world with bounding boxes******************************/
	void getWorldExtended() {




	}
	/**********************************************End*********************************/
	//Get bounding boxes from bounds table
	void spanwBounds(Spawns* spawns) {
		Bounds * bounds = new Bounds();
		std::string query = "select * from bounds";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str()); 
			while (res->next()) {
				spawns->createBoundingBox(std::stod(res->getString("x").c_str()),
					std::stod(res->getString("y").c_str()), 
					std::stod(res->getString("z").c_str()),
					std::stod(res->getString("width").c_str()),
					std::stod(res->getString("height").c_str()),
					std::stod(res->getString("deep").c_str())
					);
			}

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
		}
	}
	void createBound(float x, float y, float z, float width, float height, float deep,int type) {
		std::string query = "INSERT INTO bounds(x, y, z, width, height, deep, type) VALUES ("+ std::to_string(x) +" , " + std::to_string(y) + ", " 
			+ std::to_string(z) + ", " + std::to_string(width) + ", " + std::to_string(height) + ", " + std::to_string(deep) + ", " + std::to_string(type) + ")";
		sqlInsert(query);
	}
	void createSpawn(std::string meshName, float x, float y, float z, float scale, float mass,float type) {
		std::string query = "select ID from models where MeshName = '" + meshName + "'";
		int modelID = -1;
		//Get model id
		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				modelID = std::stoi(res->getString("ID").c_str());
			}

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
		}
		//Check if model id received
		if (modelID != -1) {
			std::string queryInsertWorld = "INSERT INTO world(ModelID,x, y, z, Scale, Mass, Type) VALUES ("+ std::to_string(modelID)+" , " + std::to_string(x) + " , " + std::to_string(y) + ", "
				+ std::to_string(z) + ", " + std::to_string(scale) + ", " + std::to_string(mass) + ", " + std::to_string(type) + ")";
			sqlInsert(queryInsertWorld);
		}
	}
	//sql insert statement
	void sqlInsert(std::string query) {
		
		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
		}

	}
private:
	sql::Driver *driver;
	sql::Connection *con;


	AppSettings* _appSettings;



};

#endif