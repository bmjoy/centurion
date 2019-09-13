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
   void create(std::string font);
   void render(float xPos, float yPos, std::string text, glm::vec4 color = glm::vec4(255.f));
   ~CBitmapFont();

 private:
	 std::string font_name, h_align, v_align, path;
	 int w, h;
	 float nrows, ncols, offset_x, total_width, avg_height;
	 glm::mat4 modelMat;
	 json font_data;
	 	 
 };