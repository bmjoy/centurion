#include <ui.h>
#include "iframe-xml.hxx"

#include <file_manager.h>
#include <logger.h>

gui::Iframe::Iframe()
{
}

gui::Iframe::Iframe(string _name)
{
	name = _name;
}

void gui::Iframe::Create(int xPos, int yPos, int width, int height)
{
	w = width;
	h = height;
	x = xPos;
	y = yPos;

	try
	{
		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "iframe.xsd");
		auto_ptr<c_iframe> iframexml = c_iframe_(Folders::INTERFACE_IFRAME + name + ".xml", 0, props);
		
		string back_name = string(iframexml->background().image_name());
		string topleft_name = string(iframexml->top_left().image_name());
		string topright_name = string(iframexml->top_right().image_name());
		string bottomright_name = string(iframexml->bottom_right().image_name());
		string bottomleft_name = string(iframexml->bottom_left().image_name());
		string right_name = string(iframexml->right().image_name());
		string left_name = string(iframexml->left().image_name());
		string top_name = string(iframexml->top().image_name());
		string bottom_name = string(iframexml->bottom().image_name());

		back = new gui::Image(back_name);
		back->create("bottom-left", xPos, yPos, w, h, 0);
		
		left = new gui::Image(left_name);
		left->create("bottom-left", xPos, yPos, 0, h, 0);

		top = new gui::Image(top_name);
		top->create("bottom-left", xPos, yPos + h, w, 0, 0);

		right = new gui::Image(right_name);
		right->create("bottom-left", xPos + w, yPos , 0, h, 0);

		bottom = new gui::Image(bottom_name);
		bottom->create("bottom-left", xPos, yPos, w, 0, 0);

		topleft = new gui::Image(topleft_name);
		topleft->create("bottom-left", xPos, yPos + h, 0, 0, 0);

		topright = new gui::Image(topright_name);
		topright->create("bottom-left", xPos + w, yPos + h, 0, 0, 0);

		bottomright = new gui::Image(bottomright_name);
		bottomright->create("bottom-left", xPos + w, yPos, 0, 0, 0);

		bottomleft = new gui::Image(bottomleft_name);
		bottomleft->create("bottom-left", xPos, yPos, 0, 0, 0);
	}
	catch (const xml_schema::exception & e) {
		string emsg = string(e.what());
		Logger::LogMessage msg = Logger::LogMessage(emsg, "gui", "Iframe", "Create");
		Logger::Error(msg);
		throw;
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the iframe data of \"" + name + "\"", "gui", "Iframe", "Create");
		Logger::Error(msg);
		throw;
	}
}

void gui::Iframe::Render()
{
	back->render(false, 0, 0, true);
	left->render(false, 0, 0, true);
	top->render(false, 0, 0, true);
	right->render(false, 0, 0, true);
	bottom->render(false, 0, 0, true);
	topleft->render(false);
	topright->render(false);
	bottomright->render(false);
	bottomleft->render(false);
}

gui::Iframe::~Iframe()
{
}
