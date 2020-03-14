#include <global>
#include <surface>
#include <maths.h>
#include <json.hpp>
#include <game/strategy.h>
#include <picking.h>
#include <object/object.h>
#include <menu>

#include <errorCodes.h>
#include <file_manager.h>

#include <engine.h>
#include <interface>

/* ----- GLOBAL ----- */

using namespace glb;
using namespace std;



namespace glb {
	mat4 menuProjection;
	mat4 cameraProjection;
	mat4 minimapProjection;
	
	void initParams() {
		try {
			
			ErrorCodes::ReadErrorCodesXml();
			Logger::CleanLogs();

			Settings::Init();
			Settings::ReadSettings();

			Engine::myWindow::Ratio = Engine::myWindow::Width / Engine::myWindow::Height;
			Engine::myWindow::WidthZoomed = Engine::myWindow::Width + (Engine::Camera::GetCurrentZoom() - 1) * Engine::Camera::GetZoomFactor();
			Engine::myWindow::HeightZoomed = Engine::myWindow::Height + (Engine::Camera::GetCurrentZoom() - 1) * Engine::Camera::GetZoomFactor() / Engine::myWindow::Ratio;

			setMenuProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::Width, 0.0f, Engine::myWindow::Height, -100.0f, 100.0f));
			setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));


			ifstream data_path("assets/data/data.json");
			//Close the game if it wasn't able to find or process data.json file
			if (!data_path.good()) {
				//forceGameClosure("NOT_FOUND", "ERROR_data");
			}
			json data = json::parse(data_path);

			for (int i = 0; i < data["player_colors"].size(); i++) {
				vec3 color = vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]);
				Game::AddColor(color);
			}

			
		}
		catch (...) {
			std::cout << "An error occurred" << std::endl;
		}
	}

	void saveCurrentScenario(string name) {


		// create a folder which will contain all scenario files
		string path = "scenarios/" + name;
		_mkdir(path.c_str());

		// save heights 
		ofstream heightsFile(path + "/heights");
		if (heightsFile.is_open()) {
			for (int i = 0; i < mapgen::nVertices * 4; i += 4)
				if (i == 0) {
					heightsFile << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
				}
				else {
					heightsFile << "," << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
				}
		}
		heightsFile.close();

		// save texture type
		ofstream textureFile(path + "/texture");
		if (textureFile.is_open()) {
			for (int i = 0; i < mapgen::nVertices; i++)
				if (i == 0) {
					textureFile << mapgen::MapTextures()[i];
				}
				else {
					textureFile << "," << mapgen::MapTextures()[i];
				}
		}
		textureFile.close();

		// save objects
		ofstream objectsFile(path + "/objects.tsv");
		if (objectsFile.is_open()) {
			objectsFile << "type\tclass\tname\tsettlement\tplayerID\tx\ty\txoffset\tyoffset" << "\n";

			vector<Building*> listOfBuildings = Game::GetListOfBuildings();

			for (int i = 0; i < listOfBuildings.size(); i++) {
				objectsFile << listOfBuildings[i]->GetType() << "\t";
				objectsFile << listOfBuildings[i]->GetClassName() << "\t";
				objectsFile << listOfBuildings[i]->GetName() << "\t";
				objectsFile << listOfBuildings[i]->GetSettlement().GetSettlementName() << "\t";
				objectsFile << listOfBuildings[i]->GetPlayer() << "\t";
				objectsFile << listOfBuildings[i]->GetPosition().x << "\t";
				objectsFile << listOfBuildings[i]->GetPosition().y << "\t";
				if (listOfBuildings[i]->is_independent()) {
					objectsFile << 0 << "\t";
					objectsFile << 0;
				}
				else {
					objectsFile << listOfBuildings[i]->GetPosition().x - listOfBuildings[i]->get_settlement_building()->GetPosition().x << "\t";
					objectsFile << listOfBuildings[i]->GetPosition().y - listOfBuildings[i]->get_settlement_building()->GetPosition().y;
				}
				objectsFile << "\n";
			}

			vector<Decoration*> listOfDecorations = Game::GetListOfDecorations();

			for (int i = 0; i < listOfDecorations.size(); i++) 
			{
				objectsFile << listOfDecorations[i]->GetType() << "\t";
				objectsFile << listOfDecorations[i]->GetClassName() << "\t";
				objectsFile << "N/A" << "\t";
				objectsFile << listOfDecorations[i]->get_settlement_name() << "\t";
				objectsFile << 0 << "\t";
				objectsFile << listOfDecorations[i]->GetPosition().x << "\t";
				objectsFile << listOfDecorations[i]->GetPosition().y << "\t";
				if (listOfDecorations[i]->get_settlement_name() != "N/A") 
				{
					objectsFile << listOfDecorations[i]->GetPosition().x - listOfDecorations[i]->get_settlement_building()->GetPosition().x << "\t";
					objectsFile << listOfDecorations[i]->GetPosition().y - listOfDecorations[i]->get_settlement_building()->GetPosition().y << "\n";
				}
				else {
					objectsFile << 0 << "\t";
					objectsFile << 0 << "\n";
				}
			}
		}
		objectsFile.close();

		cout << "[DEBUG] The map is saved with the following name: " + name << endl;

	}

	void openScenario(string name) 
	{
		// read heights
		{
			fstream fin;
			fin.open("scenarios/" + name + "/heights");
			string line, number;
			getline(fin, line);
			stringstream s(line);
			int i = 0;
			while (getline(s, number, ',')) {
				mapgen::MapHeights()[i] = stof(number);
				i++;
			}
		}
		// read texture
		{
			fstream fin;
			fin.open("scenarios/" + name + "/texture");
			string line, number;
			getline(fin, line);
			stringstream s(line);
			int i = 0;
			while (getline(s, number, ',')) {
				mapgen::MapTextures()[i] = stof(number);
				i++;
			}
		}
		// read objects
		{
			fstream fin;
			fin.open("scenarios/" + name + "/objects.tsv");
			string line, value;
			int row = 0;
			while (getline(fin, line)) {
				if (row > 0) {
					string objectsInfo[9];
					stringstream s(line);
					int i = 0;
					while (getline(s, value, '\t')) {
						objectsInfo[i] = value;
						i++;
					}
					string type = objectsInfo[0];
					string className = objectsInfo[1];
					string name = objectsInfo[2];
					string settl_name = objectsInfo[3];
					int playerID = stoi(objectsInfo[4]);
					float xPos = stof(objectsInfo[5]);
					float yPos = stof(objectsInfo[6]);
					float xOffset = stof(objectsInfo[7]);
					float yOffset = stof(objectsInfo[8]);
					if (type == "building") {
						Building* b = new Building();
						b->SetClassName(className);
						b->SetPlayer(playerID);
						b->SetPosition(vec3(xPos, yPos, 0.f));
						b->SetPickingID(PickingObject::GetPickingId());
						b->GetSettlement().SetSettlementName(settl_name);
						b->SetType("building");
						b->create(name);
						Game::AddGameObject(b->GetPickingID(), b);
					}
					if (type == "decoration") {
						Decoration* d = new Decoration();
						d->SetClassName(className);
						d->SetPlayer(0);
						d->SetPosition(vec3(xPos, yPos, 0.f));
						d->SetPickingID(PickingObject::GetPickingId());
						d->create();
						d->SetType("decoration");
						Game::AddGameObject(d->GetPickingID(), d);
					}
				}
				row++;
			}

			Game::UpdateSettlementBuildings(); // set central building for every dependent building
		}
	}
};
