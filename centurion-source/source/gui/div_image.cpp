#include "div_image.h"



DivImage::DivImage()
{
}

void DivImage::create(std::string origin, glm::ivec2 image_size) {
	img = Image(SHD::IMAGE_SHADER_ID);
	img.create(img_path, origin, image_size);
	img.apply_projection_matrix(GLB::MENU_PROJECTION);
	img.apply_view_matrix();
}

void DivImage::render(bool picking) {
	if (!picking) {
		img.render(position.x, position.y);
	}
	else {
		img.render(position.x, position.y, true, picking_id);
	}
}

DivImage::~DivImage()
{
}
