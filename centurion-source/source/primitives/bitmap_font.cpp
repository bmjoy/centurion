#include "bitmap_font.h"

#include <settings.h>
#include <stb_image.h>
#include <codecvt>
#include <locale>
#include <fontCharacters-xml.hxx>

#include <file_manager.h>

using namespace glb;

namespace txt {
	bool isArabic(int codepoint) {
		return ((codepoint >= 1536 && codepoint <= 1919) || (codepoint >= 2208 && codepoint <= 2303) || (codepoint >= 64336 && codepoint <= 64831) || (codepoint >= 65010 && codepoint <= 65276) || (codepoint == 32));
	}
};

BitmapFont::BitmapFont() {
	vPath = "assets/shaders/font/vertex.glsl";
	fPath = "assets/shaders/font/fragment.glsl";
	h_align = "left";
	v_align = "normal";
	hAlignMap["left"] = 0;
	hAlignMap["center"] = 1;
	vAlignMap["normal"] = 0;
	vAlignMap["middle"] = 1;
}

void BitmapFont::set_align(string hAlign, string vAlign) {
	h_align = hAlign;
	v_align = vAlign;
}

/* Dynamic text */

void BitmapFont::create() {

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
		path = "assets/fonts/" + fontName + ".xml";
		try {
			auto_ptr<chars> charsXML = chars_(path);

			chars::char_iterator it;
			for (it = charsXML->char_().begin(); it != charsXML->char_().end(); it++) {
				txt::Character CharData = txt::Character();
				int charID = int(it->id());
				CharData.x = int(it->x());
				CharData.y = int(it->y());
				CharData.width = int(it->width());
				CharData.height = int(it->height());
				CharData.xoffset = int(it->xoffset());
				CharData.yoffset = int(it->yoffset());
				CharData.xadvance = int(it->xadvance());
				CharData.line_height = int(it->lineheight());
				fontData[i][charID] = CharData;
			}
		}
		catch(const xml_schema::exception & e) {
			std::cout << e << std::endl;
		}
		catch (...) {
			std::cout << "An error occurred reading font XML files." << std::endl;
			//forceGameClosure("NOT_FOUND", "noFile");
		}
		
	}
}

void BitmapFont::render_dynamic(string &font, float xPos, float yPos, string &text, vec4 &color, bool shadow, bool bold) {
	
	int fontID;
	GLint textureID;

	glUseProgram(shaderId);
	glUniform4f(glGetUniformLocation(shaderId, "color"), color.x / 255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f);
	glUniform1f(glGetUniformLocation(shaderId, "y"), yPos);
	glUniform1i(glGetUniformLocation(shaderId, "hAlign"), hAlignMap[h_align]);
	glUniform1i(glGetUniformLocation(shaderId, "vAlign"), vAlignMap[v_align]);
	glUniform1i(glGetUniformLocation(shaderId, "fontHeight"), 18);
	glUniform1i(glGetUniformLocation(shaderId, "shadow"), 0);
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wtext = converter.from_bytes(text);
	
	textureID = textureIdMap[font];
	fontID = fontIdMap[font];

	int letterspacing = 0;

	if (Settings::Language == "arabic" && txt::isArabic((GLint)wtext[0])) {
		if (!bold) {
			fontID = fontIdMap["arabic_16px"];
			textureID = textureIdMap["arabic_16px"];
		}
		else {
			fontID = fontIdMap["arabic_16px_bold"];
			textureID = textureIdMap["arabic_16px_bold"];
		}
		letterspacing = -1;
	}

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
		if (Settings::Language == "arabic" && txt::isArabic((GLint)wtext[0])) codepoint = GLint(wtext[wtext.size() - i - 1]);
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

txt::StaticData BitmapFont::create_static(string &font, string &text, float x, float y, bool bold, int line_number) {
	txt::StaticData static_data = txt::StaticData();
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wtext = converter.from_bytes(text);

	int fontID, letterspacing = 0;
	fontID = fontIdMap[font];
	static_data.textureID = textureIdMap[font];
	if (Settings::Language == "arabic" && txt::isArabic((GLint)wtext[0])) {
		if (!bold) {
			fontID = fontIdMap["arabic_16px"];
			static_data.textureID = textureIdMap["arabic_16px"];
		}
		else {
			fontID = fontIdMap["arabic_16px_bold"];
			static_data.textureID = textureIdMap["arabic_16px_bold"];
		}
		letterspacing = -1;
	}

	// x positions, chars and total width
	
	int totw = 0;
	for (int i = 0; i < wtext.size(); i++) {
		
		GLint codepoint;
		if (Settings::Language == "arabic" && txt::isArabic((GLint)wtext[0])) codepoint = GLint(wtext[wtext.size() - i - 1]);
		else codepoint = GLint(wtext[i]);
		
		static_data.X.push_back(x + totw);
		static_data.Y.push_back(y - line_number * fontData[fontID][codepoint].line_height);
		static_data.charList.push_back(fontData[fontID][codepoint]);
		static_data.charsWidth.push_back(fontData[fontID][codepoint].xadvance + letterspacing);
		totw += (fontData[fontID][codepoint].xadvance + letterspacing);
	}	
	static_data.totalWidth = totw;

	// other information
	static_data.textSize = (int)wtext.size();
	static_data.fontHeight = 18;
	return static_data;
}

void BitmapFont::render_static(txt::StaticData &data) {

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

BitmapFont::~BitmapFont() {}