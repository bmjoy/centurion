#pragma once

#include <ui.h>
#include <json.hpp>
#include "object.h"
#include "settlement.h"

using namespace std;
using namespace glm;

class Unit;
namespace game { class ObjectUI; };

class Player;

class Settlement;

class Building : public GObject
{
public:
	Settlement GetSettlement(void);
	void SetSettlement(const Settlement par_settlement);

	bool IsClickableInMimimap(void);
	void CanBeClickableInMimimap(const bool par_clickable);

	bool IsPlaceable(void);
	void SetPlaceable(const bool placeable);
	bool IsCentralBuilding(void);

	string GetCategory(void);
	bool IsVillageHall(void);
	bool IsTownhall(void);
	bool IsOutpost(void);
	bool IsShipyard(void);


	void prepare() override;
	void Create(string className);
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	void SetStatus(const bool bIsCreated);

	void setWaitingToBeErased(bool b) { waitingToBeErased = b; }
	bool getWaitingToBeErased() { return waitingToBeErased; }

	Building();
	~Building();
protected:
	void IAmACentralBuilding(void);
	void IAmATownhall(void);
	void IAmAnOutpost(void);
	void IAmAVillageHall(void);
	void IAmAShipyard(void);
	void StartGoldProduction(void);
	void StopGoldProduction(void);
	void StartFoodProduction(void);
	void StopFoodProduction(void);
private:
	game::ObjectUI* buildingUI;
	map<int, Building*> subs_buildings; // dependent buildings connected to indipendent one
	vector<Unit> unitsInside;
	bool waitingToBeErased;
	bool bIsCreated;
	size_t buildingListSize;
	bool bIsPlaceable;
	bool bIsClickableInMimimap;
	gui::Circle circle[2];
	string category;
	string ent_path;
	string pass_path;
	bool bIsOutpost = false;
	bool bIsTownhall = false;
	bool bIsVillageHall = false;
	bool bIsCentralBuilding = false;
	bool bIsShipyard = false;
	bool bCanProduceGold = false;
	bool bCanProduceFood = false;
	vector<Unit> holdUnits;
	Settlement settlement;

	//sound selectionSound; TODO
};