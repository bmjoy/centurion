#pragma once

#include <global.hpp>
#include <shader.h>
#include <json.hpp>

class CBitmapFont : public Shader 
 {
  public:
   CBitmapFont();
   void set_align(std::string hAlign = "left", std::string vAlign = "normal");
   void create();
   int getShaderId() { return shaderId; }
   void render(std::string font, float xPos, float yPos, std::string text, glm::vec4 color = glm::vec4(255.f));
   ~CBitmapFont();

 private:
	 struct CharData {		 
		 int charWidth[256];
		 float fontHeight;
		 float fontWidth;
		 int startChar;
	 };
	 
	 std::map<std::string, CharData> fontData;

	 std::string h_align, v_align, path;

	 int total_width;
	 int offset_x;
	 glm::mat4 modelMat;

	 std::map<std::string, int> hAlignMap;
	 std::map<std::string, int> vAlignMap;
};