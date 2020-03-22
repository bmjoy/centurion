#ifndef BUILDINGSPRITE_H
#define BUILDINGSPRITE_H

#include "object_sprite.h"

/// <summary>
/// This class is used to hold Building-specific methods for graphic rendering
/// </summary>
class BuildingSprite : public ObjectSprite
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	BuildingSprite();

	/// <summary>
	/// Render function.
	/// </summary>
	/// <param name="data">The sprite data of the object; it is generated by the "Create" function of "ObjectSprite" class;</param>
	/// <param name="x">The X-Position of the building;</param>
	/// <param name="y">The Y-Position of the building;</param>
	/// <param name="picking">Boolean; true = picking is active;</param>
	/// <param name="selected">Boolean; true = the building is selected;</param>
	/// <param name="playerColor">The player's color, 3D vector (255-RGB);</param>
	/// <param name="not_placeable">Boolean; true = the building is not placeable.</param>
	void Render(SpriteData &data, float x, float y, bool picking, bool selected, vec3 playerColor, bool not_placeable = false);

	/// <summary>
	/// Destructor
	/// </summary>
	~BuildingSprite();
};

static BuildingSprite bsprite;

#endif