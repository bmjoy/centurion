#include <game/strategy.h>
#include <game/editor.h>
#include <math>
#include <engine/camera.h>
#include <engine/window.h>
#include <engine/mouse.h>
#include <player>
#include <surface>
#include <picking>
#include <interface>

using namespace glb;
using namespace std;
using namespace glm;

using namespace engine;



map<int, Building> buildings = { };
map<int, Building*> independent_buildings = { };
map<int, Unit> units = { };
map<int, Decoration> decorations = { };
vector<string> outposts;
array<Player, 8> playersList;
