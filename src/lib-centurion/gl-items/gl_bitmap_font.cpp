#include "gl_bitmap_font.h"

#include <settings.h>
#include <stb_image.h>
#include <file_manager.h>

using namespace std;
using namespace glm;

bool glBitmapFont::isArabic(int codepoint) {
	return ((codepoint >= 1536 && codepoint <= 1919) || (codepoint >= 2208 && codepoint <= 2303) || (codepoint >= 64336 && codepoint <= 64831) || (codepoint >= 65010 && codepoint <= 65276) || (codepoint == 32));
}

glBitmapFont::glBitmapFont() {
	vPath = "assets/shaders/font/vertex.glsl";
	fPath = "assets/shaders/font/fragment.glsl";
	h_align = "left";
	v_align = "normal";
	hAlignMap["left"] = 0;
	hAlignMap["center"] = 1;
	vAlignMap["normal"] = 0;
	vAlignMap["middle"] = 1;
}

void glBitmapFont::set_align(string hAlign, string vAlign) {
	h_align = hAlign;
	v_align = vAlign;
}

/* Dynamic text */

void glBitmapFont::create() {

	glUseProgram(shaderId);

	unsigned int indices[] = {
		 0, 1, 3,   // first triangle
		 1, 2, 3    // second triangle
	};

	float vertices[] = {
		// positions	// uv coords		
		0.f,	-1.f,	0.0f, 1.0,   // in basso a sx
		1.f,	-1.f,	1.0, 1.0,    // in basso a dx
		1.f,	0.f,	1.0, 0.0f,   // in alto a dx
		0.f,	0.f,	0.0f, 0.0f   // in alto a sx
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Texture */

	vector<FileManager::file_info> fonts = FileManager::GetAllFilesNamesWithinSubfolders("assets\\fonts", "png");
	string fontName;

	for (int i = 0; i < fonts.size(); i++){
		fontName = fonts[i].name;
		fontIdMap[fontName] = i;
		
		// LOAD IMAGE
		path = "assets/fonts/" + fontName + ".png";
		textureIdList.push_back(0);
		textureInfoList.push_back(ivec3(0, 0, 0));
		unsigned char *data = stbi_load(path.c_str(), &textureInfoList[i].x, &textureInfoList[i].y, &textureInfoList[i].z, 0);
		if (!data) { cout << "Failed to load texture" << endl; }

		glUniform1i(glGetUniformLocation(shaderId, "tex"), 0); // texture
		glGenTextures(1, &textureIdList[i]);
		glBindTexture(GL_TEXTURE_2D, textureIdList[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[i].x, textureInfoList[i].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		textureIdMap[fontName] = textureIdList[i];
		
		// LOAD DATA FROM XML
		path = Folders::GAME + "assets//fonts//" + fontName + ".xml";
		try {
			tinyxml2::XMLDocument xmlFile;
			xmlFile.LoadFile(path.c_str());
			tinyxml2::XMLElement *levelElement = xmlFile.FirstChildElement("chars");
			for (tinyxml2::XMLElement* child = levelElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
			{
				Character CharData = Character();
				string parsedStr;

				parsedStr = child->Attribute("id");
				int charID = stoi(parsedStr);

				parsedStr = child->Attribute("x");
				CharData.x = stoi(parsedStr);

				parsedStr = child->Attribute("y");
				CharData.y = stoi(parsedStr);

				parsedStr = child->Attribute("width");
				CharData.width = stoi(parsedStr);

				parsedStr = child->Attribute("height");
				CharData.height = stoi(parsedStr);

				parsedStr = child->Attribute("xoffset");
				CharData.xoffset = stoi(parsedStr);

				parsedStr = child->Attribute("yoffset");
				CharData.yoffset = stoi(parsedStr);

				parsedStr = child->Attribute("xadvance");
				CharData.xadvance = stoi(parsedStr);

				parsedStr = child->Attribute("lineheight");
				CharData.line_height = stoi(parsedStr);
				fontData[i][charID] = CharData;
			}
		}
		catch (...) {
			std::cout << "An error occurred reading font XML files." << std::endl;
			//forceGameClosure("NOT_FOUND", "noFile");
		}
		
	}
}

void glBitmapFont::UpdateText(StaticTextData* data, std::wstring new_text)
{
	int fontID, letterspacing = 0;
	fontID = fontIdMap[data->fontName];

	int totw = 0;
	(*data).text = "";
	(*data).X.clear();
	(*data).Y.clear();
	(*data).charList.clear();
	(*data).charsWidth.clear();
	(*data).textSize = (int)new_text.size();
	for (int i = 0; i < (*data).textSize; i++) {

		GLint codepoint = GLint(new_text[i]);

		(*data).X.push_back(data->startX + totw);
		(*data).Y.push_back(data->startY - fontData[fontID][codepoint].line_height);
		(*data).charList.push_back(fontData[fontID][codepoint]);
		(*data).charsWidth.push_back(fontData[fontID][codepoint].xadvance + letterspacing);
		(*data).text += (char)new_text[i];
		totw += (fontData[fontID][codepoint].xadvance + letterspacing);
	}
}

void glBitmapFont::render_dynamic(string &font, float xPos, float yPos, string &text, vec4 &color, bool shadow, bool bold) {
	
	int fontID;
	GLint textureID;

	glUseProgram(shaderId);
	glUniform4f(glGetUniformLocation(shaderId, "color"), color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f);
	glUniform1f(glGetUniformLocation(shaderId, "y"), yPos);
	glUniform1i(glGetUniformLocation(shaderId, "hAlign"), hAlignMap[h_align]);
	glUniform1i(glGetUniformLocation(shaderId, "vAlign"), vAlignMap[v_align]);
	glUniform1i(glGetUniformLocation(shaderId, "fontHeight"), 18);
	glUniform1i(glGetUniformLocation(shaderId, "shadow"), 0);
	//wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	//wstring wtext = converter.from_bytes(text);
	
	string wtext = text;

	textureID = textureIdMap[font];
	fontID = fontIdMap[font];

	int letterspacing = 0;

	/*if (Settings::Language == "arabic" && this->isArabic((GLint)wtext[0])) {
		if (!bold) {
			fontID = fontIdMap["arabic_16px"];
			textureID = textureIdMap["arabic_16px"];
		}
		else {
			fontID = fontIdMap["arabic_16px_bold"];
			textureID = textureIdMap["arabic_16px_bold"];
		}
		letterspacing = -1;
	}*/

	if (h_align != "left"){
		total_width = 0;
		for (int i = 0; i < wtext.size(); i++) {
			total_width += fontData[fontID][wtext[i]].xadvance + letterspacing;
		}
		glUniform1i(glGetUniformLocation(shaderId, "totalWidth"), total_width);
	}

	glBindVertexArray(VAO);
	
	glActiveTexture(GL_TEXTURE0);

	
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	offset_x = 0;
	for (int i = 0; i < wtext.size(); i++) {
		GLint codepoint;
		if (Settings::Language == "arabic" && this->isArabic((GLint)wtext[0])) codepoint = GLint(wtext[wtext.size() - i - 1]);
		else codepoint = GLint(wtext[i]);
		glUniform1f(glGetUniformLocation(shaderId, "x"), xPos + offset_x);
		glUniform1i(glGetUniformLocation(shaderId, "char_xpos"), fontData[fontID][codepoint].x);
		glUniform1i(glGetUniformLocation(shaderId, "char_ypos"), fontData[fontID][codepoint].y);
		glUniform1i(glGetUniformLocation(shaderId, "char_width"), fontData[fontID][codepoint].width);
		glUniform1i(glGetUniformLocation(shaderId, "char_height"), fontData[fontID][codepoint].height);
		glUniform1i(glGetUniformLocation(shaderId, "char_yoffset"), fontData[fontID][codepoint].yoffset);
		if (shadow) {
			glUniform1i(glGetUniformLocation(shaderId, "shadow"), 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glUniform1i(glGetUniformLocation(shaderId, "shadow"), 0);
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		offset_x += fontData[fontID][codepoint].xadvance + letterspacing;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/* Static text */

glBitmapFont::StaticTextData glBitmapFont::create_static(string &font, const char* text, float x, float y, bool bold, int line_number) {
	StaticTextData static_data = StaticTextData();
	int fontID, letterspacing = 0;
	fontID = fontIdMap[font];
	static_data.textureID = textureIdMap[font];
	static_data.fontName = font;
	static_data.startX = x;
	static_data.startY = y;
	/*if (Settings::Language == "arabic" && this->isArabic((GLint)wtext[0])) {
		if (!bold) {
			fontID = fontIdMap["arabic_16px"];
			static_data.textureID = textureIdMap["arabic_16px"];
		}
		else {
			fontID = fontIdMap["arabic_16px_bold"];
			static_data.textureID = textureIdMap["arabic_16px_bold"];
		}
		letterspacing = -1;
	}*/

	// x positions, chars and total width

	size_t textSize = string(text).size();

	int totw = 0;
	static_data.text = "";
	for (int i = 0; i < textSize; i++) {

		GLint codepoint;
		if (Settings::Language == "arabic" && this->isArabic((GLint)text[0])) codepoint = GLint(text[textSize - i - 1]);
		else codepoint = GLint(text[i]);

		static_data.X.push_back(x + totw);
		static_data.Y.push_back(y - line_number * fontData[fontID][codepoint].line_height);
		static_data.charList.push_back(fontData[fontID][codepoint]);
		static_data.charsWidth.push_back(fontData[fontID][codepoint].xadvance + letterspacing);
		static_data.text += (char)text[i];
		totw += (fontData[fontID][codepoint].xadvance + letterspacing);
	}
	static_data.totalWidth = totw;

	// other information
	static_data.textSize = (int)textSize;
	static_data.fontHeight = 18;
	return static_data;
}

glBitmapFont::StaticTextData glBitmapFont::create_static(std::string & font, const wchar_t * text, float x, float y, bool bold, int line_number)
{
	StaticTextData static_data = StaticTextData();
	int fontID, letterspacing = 0;
	fontID = fontIdMap[font];
	static_data.textureID = textureIdMap[font];
	static_data.fontName = font;
	static_data.startX = x;
	static_data.startY = y;

	size_t textSize = wstring(text).size();

	int totw = 0;
	static_data.text = "";
	for (int i = 0; i < textSize; i++) {

		GLint codepoint;
		if (Settings::Language == "arabic" && this->isArabic((GLint)text[0])) codepoint = GLint(text[textSize - i - 1]);
		else codepoint = GLint(text[i]);

		static_data.X.push_back(x + totw);
		static_data.Y.push_back(y - line_number * fontData[fontID][codepoint].line_height);
		static_data.charList.push_back(fontData[fontID][codepoint]);
		static_data.charsWidth.push_back(fontData[fontID][codepoint].xadvance + letterspacing);
		static_data.text += (char)text[i];
		totw += (fontData[fontID][codepoint].xadvance + letterspacing);
	}
	static_data.totalWidth = totw;

	// other information
	static_data.textSize = (int)textSize;
	static_data.fontHeight = 18;

	return static_data;
}

glBitmapFont::StaticTextData glBitmapFont::create_static(string &font, const int* codepoints, int textSize, float x, float y, bool bold, int line_number) {
	StaticTextData static_data = StaticTextData();

	int fontID, letterspacing = 0;
	fontID = fontIdMap[font];
	static_data.textureID = textureIdMap[font];
	static_data.fontName = font;
	static_data.startX = x;
	static_data.startY = y;

	// x positions, chars and total width
	
	int totw = 0;
	static_data.text = "";
	for (int i = 0; i < textSize; i++) {
		GLint codepoint = GLint(codepoints[i]);
		static_data.X.push_back(x + totw);
		static_data.Y.push_back(y - line_number * fontData[fontID][codepoint].line_height);
		static_data.charList.push_back(fontData[fontID][codepoint]);
		static_data.charsWidth.push_back(fontData[fontID][codepoint].xadvance + letterspacing);
		static_data.text += (char)codepoint;
		totw += (fontData[fontID][codepoint].xadvance + letterspacing);
	}	
	static_data.totalWidth = totw;

	// other information
	static_data.textSize = (int)textSize;
	static_data.fontHeight = 18;
	return static_data;
}

void glBitmapFont::render_static(StaticTextData &data) {

	glUseProgram(shaderId);
	glUniform4f(glGetUniformLocation(shaderId, "color"), data.color.x / 255.f, data.color.y / 255.f, data.color.z / 255.f, data.color.w / 255.f);
	if (data.y > 0) glUniform1f(glGetUniformLocation(shaderId, "y"), data.y); // one line text
	glUniform1i(glGetUniformLocation(shaderId, "hAlign"), hAlignMap[h_align]);
	glUniform1i(glGetUniformLocation(shaderId, "vAlign"), vAlignMap[v_align]);
	glUniform1i(glGetUniformLocation(shaderId, "fontHeight"), data.fontHeight);
	glUniform1i(glGetUniformLocation(shaderId, "totalWidth"), data.totalWidth);
	glUniform1i(glGetUniformLocation(shaderId, "shadow"), 0);

	/* Draw */
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data.textureID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	for (int i = 0; i < data.textSize; i++) {
		glUniform1f(glGetUniformLocation(shaderId, "x"), data.X[i]);
		if (data.y == 0) glUniform1f(glGetUniformLocation(shaderId, "y"), data.Y[i]); // text box
		glUniform1i(glGetUniformLocation(shaderId, "char_xpos"), data.charList[i].x);
		glUniform1i(glGetUniformLocation(shaderId, "char_ypos"), data.charList[i].y);
		glUniform1i(glGetUniformLocation(shaderId, "char_width"), data.charList[i].width);
		glUniform1i(glGetUniformLocation(shaderId, "char_height"), data.charList[i].height);
		glUniform1i(glGetUniformLocation(shaderId, "char_yoffset"), data.charList[i].yoffset);
		if (data.shadow) {
			glUniform1i(glGetUniformLocation(shaderId, "shadow"), 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glUniform1i(glGetUniformLocation(shaderId, "shadow"), 0);
		}
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glBitmapFont::~glBitmapFont() {}