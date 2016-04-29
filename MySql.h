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
#include "BotModel.h"

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
				model->setType(std::stoi(res->getString("Type").c_str()));
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
		std::string query = "select MeshName from models where Type=0";

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
	//get bots
	std::vector<BotModel*> getBots(Spawns* spawns) {
		std::vector<BotModel*> mBots;
		BotModel*  mBot;
		std::string query = "select * from Bots";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				int id =std::stoi(res->getString("ID").c_str());
				int meshID = std::stoi(res->getString("MeshID").c_str());
				Ogre::String meshName = "";
				
				std::string query2 = "select MeshName from models where ID="+std::to_string(meshID);
				sql::Statement * stmt2 = con->createStatement();
				sql::ResultSet * res2 = stmt2->executeQuery(query2.c_str());
				while (res2->next()) {
					meshName=res2->getString("MeshName").c_str();
				}

				Ogre::Vector3 startPos = Ogre::Vector3(std::stod(res->getString("X").c_str()), std::stod(res->getString("Y").c_str()), std::stod(res->getString("Z").c_str()));
				mBot = spawns->createBot(meshName,startPos);
				mBot->setId(id);
				mBot->setMeshName(meshName);
				mBots.push_back(mBot);
			}
		}catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			//model->setName("Error");

		}
		return mBots;
	}


	
	//spawn all objects from world table
	void getWorld(Spawns* spawns) {
		SpawnWorld * spawnWorld = new SpawnWorld();
		std::string query = "select * from world";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str()); 
			while (res->next()) {

				/*
				spawns->createObject(getModelById(std::stoi(res->getString("ModelID").c_str())), 
					Ogre::Vector3(std::stod(res->getString("x").c_str()), std::stod(res->getString("y").c_str()), std::stod(res->getString("z").c_str())),
					std::stod(res->getString("Width").c_str()), std::stod(res->getString("Height").c_str()), std::stod(res->getString("Deep").c_str()),
					std::stod(res->getString("Scale").c_str()), std::stod(res->getString("Mass").c_str()), std::stod(res->getString("Type").c_str()));
					*/
				//Bounds with integrated shape
				
				Ogre::Vector3 position(std::stod(res->getString("x").c_str()), std::stod(res->getString("y").c_str()), std::stod(res->getString("z").c_str()));
				Ogre::Vector3 scaleDimensions(std::stod(res->getString("Width").c_str()), std::stod(res->getString("Height").c_str()), std::stod(res->getString("Deep").c_str()));
				Ogre::Vector3 rotation(std::stod(res->getString("RotX").c_str()), std::stod(res->getString("RotY").c_str()), std::stod(res->getString("RotZ").c_str()));
				float scale = 1;
				float mass = std::stod(res->getString("Mass").c_str());
				int id = std::stoi(res->getString("ID").c_str());
				//
				std::string sqlTableName = "bounds";
				
				int worldID = std::stoi(res->getString("ID").c_str());
				
				
				//int tableRecords = getRecordCount(sqlTableName, "worldID="+ Ogre::StringConverter::toString(worldID));
				std::vector<Bounds*> boundsContainer = getBoundsByWorldID(worldID);
				
				if (boundsContainer.size() ==1) {
					spawns->createObjectBoxDescription(
						id,
						getModelById(std::stoi(res->getString("ModelID").c_str()))->getMeshName(),
						position,
						scaleDimensions,
						boundsContainer.front()->getDimensions(),
						scale,
						rotation,
						mass,
						boundsContainer.front()->getType());
				}
				else if (boundsContainer.size() == 0) {
					spawns->createMeshOnly(
						id,
						getModelById(std::stoi(res->getString("ModelID").c_str()))->getMeshName(),
						position,
						scaleDimensions,
						rotation
						);
				}
				//createObjectBoxDescription(Models * model, Ogre::Vector3 position, Ogre::Vector3 scaleDimensions, Ogre::Vector3 boxDimensions, float scale, Ogre::Vector3 rotation, Ogre::Real mass, int type)

			}
			//Spawn bounds without models
			spawnBounds(spawns);

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			//model->setName("Error");

		}
	}
	//Last insert id
	int getLastInsertID() {
		
		std::string query = "select LAST_INSERT_ID() AS ID";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				return std::stoi(res->getString("ID").c_str());
			}

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			return -1;
			//model->setName("Error");

		}
	}
	//SELECT COUNT(ID) as count FROM bounds where id=5 
	//record count from table where id
	int getRecordCount(std::string tableName, std::string parameter) {
		std::string query = "SELECT COUNT(ID) as count FROM "+tableName+" where "+parameter;

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				return std::stoi(res->getString("count").c_str());
			}

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
			return -1;
			//model->setName("Error");

		}
	}
	/************************Spawn world with bounding boxes******************************/
	void getWorldExtended() {




	}
	/**********************************************End*********************************/
	//Get bounding boxes from bounds table
	void spawnBounds(Spawns* spawns) {
		Bounds * bounds = new Bounds();
		std::string query = "select * from bounds where worldID=-1";

		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str()); 
			while (res->next()) {
				Ogre::Vector3 position(std::stod(res->getString("x").c_str()), std::stod(res->getString("y").c_str()), std::stod(res->getString("z").c_str()));
				Ogre::Vector3 boxDimensions(std::stod(res->getString("Width").c_str()), std::stod(res->getString("Height").c_str()), std::stod(res->getString("Deep").c_str()));
				Ogre::Vector3 rotation(std::stod(res->getString("RotX").c_str()), std::stod(res->getString("RotY").c_str()), std::stod(res->getString("RotZ").c_str()));
				spawns->createBoundingBox(
					position,
					boxDimensions,
					rotation
					);
			}

		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
		}
	}
	//GetBounds by world id
	std::vector<Bounds*> getBoundsByWorldID(int worldID){
		std::vector<Bounds*> boundsContainer;
		std::string query = "select * from bounds where worldID="+ Ogre::StringConverter::toString(worldID);
		try {
			sql::Statement * stmt = con->createStatement();
			sql::ResultSet * res = stmt->executeQuery(query.c_str());
			while (res->next()) {
				Bounds * bounds = new Bounds();
				bounds->setPosition(Ogre::Vector3(std::stod(res->getString("x").c_str()), std::stod(res->getString("y").c_str()), std::stod(res->getString("z").c_str())));
				bounds->setDimensions(Ogre::Vector3(std::stod(res->getString("width").c_str()), std::stod(res->getString("height").c_str()), std::stod(res->getString("deep").c_str())));
				bounds->setType(std::stod(res->getString("type").c_str()));
				bounds->setWorldId(std::stod(res->getString("worldID").c_str()));
				boundsContainer.push_back(bounds);
			}
		}
		catch (sql::SQLException &e) {

			Ogre::String error = "Error";
		}
		
		return boundsContainer;
	}


	void createBound(float x, float y, float z, float width, float height,  float deep, Ogre::Vector3 rotation, int type,int worldID) {
		std::string query = "INSERT INTO bounds(x, y, z, width, height,deep,RotX,RotY,RotZ, type,worldID) VALUES ("+ 
			std::to_string(x) +" , " + 
			std::to_string(y) + ", " +
			std::to_string(z) + ", " + 
			std::to_string(width) + ", " + 
			std::to_string(height) + ", " + 
			std::to_string(deep) + ", " + 
			std::to_string(rotation.x) + ", " +
			std::to_string(rotation.y) + ", " +
			std::to_string(rotation.z) + ", "+
			std::to_string(type)+ ", " + 
			std::to_string(worldID) 
			+ ")";
		sqlInsert(query);
	}
	void createSpawn(std::string meshName, float x, float y, float z, float scaleX, float scaleY, float scaleZ, float scale , Ogre::Vector3 rotation, float mass,float type) {
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
			std::string queryInsertWorld = "INSERT INTO world(ModelID,x, y, z, width, height, deep, Scale,RotX,RotY,RotZ, Mass, Type) VALUES ("+ 
				std::to_string(modelID)+" , " + 
				std::to_string(x) + " , " +
				std::to_string(y) + ", " +
				std::to_string(z) + ", " +
				std::to_string(scaleX) + ", " + 
				std::to_string(scaleY) + ", " + 
				std::to_string(scaleZ) + ", " + 
				std::to_string(scale) + ", " +
				std::to_string(rotation.x) + ", " + 
				std::to_string(rotation.y) + ", " +
				std::to_string(rotation.z) + ", "
				+ std::to_string(mass) + ", " 
				+ std::to_string(type) + 
				")";
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