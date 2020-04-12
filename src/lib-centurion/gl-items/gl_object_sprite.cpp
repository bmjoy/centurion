#include "gl_object_sprite.h"

#include <stb_image.h>
#include <classes/classes-data.h>
#include <file_manager.h>
#include <logger.h>

using namespace tinyxml2;
using namespace std;
using namespace glm;

glObjectSprite::glObjectSprite() {}

glObjectSprite::~glObjectSprite() {}

void glObjectSprite::Create() {
	string spritePath = "";
	try
	{
		glUseProgram(shaderId);
		GenerateBuffers();

		std::string fullName;
		SpriteData spriteData = SpriteData();

		for (int j = 0; j < entPathList.size(); ++j) {

			spritePath = entPathList[j];
			fullName = Folders::GAME + entPathList[j];
			string entityFolder = FileManager::GetFileFolderPath(fullName.c_str());

			try {

				XMLDocument xmlFile;
				xmlFile.LoadFile(fullName.c_str());
				XMLElement *_entityXml = xmlFile.FirstChildElement("entity");

				std::string _className = string(_entityXml->Attribute("class_name"));

				for (XMLElement* _img_it = _entityXml->FirstChildElement("ent_images")->FirstChildElement(); _img_it != NULL; _img_it = _img_it->NextSiblingElement())
				{
					string image_name = string(_img_it->Attribute("file")) + ".png";
					string image_path = entityFolder + "\\" + image_name;

					/* save texture info */
					int width, height, nrChannels;
					GLuint textureID;
					data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
					if (!data) {
						std::cout << "Failed to load texture" << std::endl;
					}

					/* load texture to gpu */
					glGenTextures(1, &textureID);
					glBindTexture(GL_TEXTURE_2D, textureID);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

					// create texture and generate mipmaps
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);

					// save sprite data
					spriteData.width = float(width);
					spriteData.height = float(height);
					spriteData.textureId = textureID;

					// send data to ClassesData via classname
					ClassesData::GetClassesData(_className)->SetSpriteData(spriteData);

					stbi_image_free(data);
				}
			}
			catch (...) {
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the following sprite: \"" + spritePath + "\". ", "Warn", "", "BuildingSprite", "Create");
				Logger::Warn(msg);
			}
		}
	}
	catch (const std::exception&)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the following sprite: \"" + spritePath + "\"", "Warn", "", "BuildingSprite", "Create");
		Logger::Warn(msg);
	}
}

void glObjectSprite::Render()
{
}

void glObjectSprite::GenerateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
