#include <gui>
#include <global>
#include <engine/window.h>

namespace gui {

	using namespace std;
	using namespace glm;
	using namespace glb;

	TextBox::TextBox() {
	}

	void TextBox::create(string text, string font, float x, float y, float width, float height, string halign, string valign, vec4 color, string fontWeight) {
	
		lines.clear();
		lines_data.clear();

		if (x < 0) x += engine::myWindow::Width;
		if (y < 0) y += engine::myWindow::Height;
		
		hAlign = halign; vAlign = valign;

		string fontName = font;
		bool isBold = false;
		if (fontWeight == "bold") {
			fontName = fontName + "_bold";
			isBold = true;
		}

		int K = 0;
		for (int i = 0; i < text.size(); i++) {
			if (i < text.size() - 2){
				if ((text[i] == '\\' && text[i + 1] == 'n')) {
					string line = text.substr(K, i - K);
					lines.push_back(line);
					i++;
					K = i + 1;
				}
			}
			if (i == text.size() - 1) {
				string line = text.substr(K, i - K + 1);
				lines.push_back(line);
			}
		}
		int line_number = 0;			
		for (int l = 0; l < lines.size(); l++) {
			string word, TEXT = "", line = lines[l];
			SimpleText WordTemp = SimpleText("static");
			float line_width = 0.f;
			int k = 0;
			for (int j = 0; j < line.size(); j++) {
				if (line[j] == ' ' || j == line.size() - 1) {					
					word = line.substr(k, j - k + 1);
					WordTemp.create_static(word, fontName, 0, 0, hAlign, vAlign, vec4(0), fontWeight);
					float tempWidth = WordTemp.get_width();
					if (line_width + tempWidth <= width) {
						TEXT = TEXT + word;
						line_width += tempWidth;
						cout << TEXT << " [IF]" << endl;
					}
					else {
						lines_data.push_back(obj::Text()->create_static(fontName, TEXT, x, y, isBold, line_number));
						line_width = tempWidth;
						TEXT.clear();
						TEXT = TEXT + word;
						line_number++;
						cout << "   " << TEXT << " [ELSE]" << endl;
					}
					k = j + 1;
				}
				if (j == line.size() - 1) {
					lines_data.push_back(obj::Text()->create_static(fontName, TEXT, x, y, isBold, line_number));
					line_number++;
					cout << "      " << TEXT << " [LAST]" << endl;
				}
			}	
		}

		for (int i = 0; i < lines_data.size(); i++) {
			lines_data[i].color = color;
			lines_data[i].y = 0.f; // to deactivate one-line text
		}
	}

	void TextBox::render() {
		obj::Text()->set_align(hAlign, vAlign);	
		for(int i = 0; i < lines_data.size(); i++)
			obj::Text()->render_static(lines_data[i]);
	}

	TextBox::~TextBox(){}
};