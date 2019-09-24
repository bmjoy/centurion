#pragma once

#include <global.hpp>
#include <shader.h>
#include <json.hpp>

class CBitmapFont : public Shader 
 {
  public:
   CBitmapFont();
   CBitmapFont(int shader_id);
   void set_align(std::string hAlign = "left", std::string vAlign = "normal");
   void create();
   int getShaderId() { return shaderId; }
   void render(std::string font, float xPos, float yPos, std::string text, glm::vec4 color = glm::vec4(255.f));
   ~CBitmapFont();

 private:
	 std::string h_align, v_align, path;
	 float offset_x, total_width, avg_height;
	 glm::mat4 modelMat;
	 std::map<std::string, json> font_data;
 };