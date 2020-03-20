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
	Settlement *GetSettlement(void);
	void SetSettlement(Settlement *par_settlement);

	void SetCategory(const string par_category);
	string GetCategory(void);

	void SetMaxHealth(const unsigned int par_maxHealth);
	unsigned int GetMaxHealth(void);

	void SetRepairRate(const unsigned int par_repairRate);
	unsigned int GetRepairRate(void);

	void SetLoyaltyFearHealthPercent(const unsigned int par_loyaltyFearHealthPercent);
	unsigned int GetLoyaltyFearHealthPercent(void);

	void CanBeClickableInMimimap(const bool par_clickable);
	bool IsClickableInMimimap(void);

	bool IsCentralBuilding(void);
	bool IsVillageHall(void);
	bool IsTownhall(void);
	bool IsOutpost(void);
	bool IsShipyard(void);
	
	void SetPlaceable(const bool placeable);
	bool IsPlaceable(void);
	
	void StartGoldProduction(void);
	void StopGoldProduction(void);
	void StartFoodProduction(void);
	void StopFoodProduction(void);

	void SetEntPath(const string par_ent_path);
	void SetPassPath(const string par_pass_path);

	void prepare() override;
	//void Create(string className);
	void SetBuildingProperties(ObjectData::ObjectXMLClassData &objData);
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	void SetStatus(const bool bIsCreated);

	void setWaitingToBeErased(bool b) { waitingToBeErased = b; }
	bool getWaitingToBeErased() { return waitingToBeErased; }

	Building();
	~Building();

private:
	game::ObjectUI* buildingUI;
	map<int, Building*> subs_buildings; // dependent buildings connected to indipendent one
	vector<Unit> unitsInside;
	bool waitingToBeErased;
	bool bIsCreated;
	size_t buildingListSize;
	gui::Circle circle[2];
	string category;
	unsigned int maxHealth;
	unsigned int repairRate;
	unsigned int loyaltyFearHealthPercent;
	bool bIsClickableInMimimap = false;
	bool bIsCentralBuilding = false;
	bool bIsOutpost = false;
	bool bIsTownhall = false;
	bool bIsVillageHall = false;
	bool bIsShipyard = false;
	bool bIsPlaceable = false;
	bool bAutoRepair = false;
	bool bCanProduceGold = false;
	bool bCanProduceFood = false;
	string ent_path;
	string pass_path;
	vector<Unit> holdUnits;
	Settlement *settlement;

	//sound selectionSound; TODO
};