/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include "object.h"
#include "settlement.h"

/// <summary>
/// (???)Avoid overlap beetwen two buildings when a new building is about to be created.
/// </summary>
#ifndef RADIUS_OFFSET
#define RADIUS_OFFSET 100
#endif

#define MAX_DISTANCE  2000

class Unit;
namespace game { class ObjectUI; };
class Player;
class Settlement;

class Building : public GObject
{
public:

	/// <summary>
	/// This function sets a settlement to whom the current building will belong to.
	/// </summary>
	/// <param name="par_settlement">The settlement.</param>
	void SetSettlement(Settlement *par_settlement);

	/// <summary>
	/// This funtion returns the settlement to whom the current building belong to. 
	/// </summary>
	/// <returns>A pointer to the settlement.</returns>
	Settlement *GetSettlement(void);

	/// <summary>
	/// This functions set the max health of the current building..
	/// </summary>
	/// <param name="par_maxHealth">The max health of the building.</param>
	void SetMaxHealth(const unsigned int par_maxHealth);

	/// <summary>
	/// This function returns the max health of the current building..
	/// </summary>
	/// <returns>The value of the max health of the current building</returns>
	unsigned int GetMaxHealth(void);

	/// <summary>
	/// This function sets a repair rate with which the repair building will be repaired 
	/// when its health is less than its max health.
	/// </summary>
	/// <param name="par_repairRate">The repair rate of the current building.</param>
	void SetRepairRate(const unsigned int par_repairRate);

	/// <summary>
	/// This function returns the repair rate with which the repair building will be repaired 
	/// when its health is less than its max health.
	/// </summary>
	/// <returns>The value of the repair rate of the current building.</returns>
	unsigned int GetRepairRate(void);

	/// <summary>
	/// This function sets the loyalty fear health percent of the current building.
	/// If the health of the current building is less than the value of this parameter, 
	/// then the loyolty of the building will get down faster.
	/// </summary>
	/// <param name="par_loyaltyFearHealthPercent"></param>
	void SetLoyaltyFearHealthPercent(const unsigned int par_loyaltyFearHealthPercent);

	/// <summary>
	/// This function returns the loyalty fear health percent of the current building.
	/// If the health of the current building is less than the value of this parameter, 
	/// then the loyolty of the building will get down faster.
	/// </summary>
	/// <returns>The value of loyalty fear health percent of the current buuilding.</returns>
	unsigned int GetLoyaltyFearHealthPercent(void);

	/// <summary>
	/// Through this function it's possible to indicate if the current building can be
	/// clickable in minimap.
	/// </summary>
	/// <param name="par_clickable">True = clickable; false = not clickable</param>
	void CanBeClickableInMimimap(const bool par_clickable);

	/// <summary>
	/// This function checks if the current building can be clickable in minimap.
	/// </summary>
	/// <returns>True if it can be clickable; false otherwise.</returns>
	bool IsClickableInMimimap(void);

	/// <summary>
	/// This function checks if the current building is a central building,
	/// that is if it generate a new settlement,
	/// </summary>
	/// <returns>True if it is centra building; false otherwise.</returns>
	bool IsCentralBuilding(void);
	
	/// <summary>
	/// This functionc checks if the the class of the current building is an heir of Village Hall class,
	/// and consequently if it is a central building.
	/// </summary>
	/// <returns>True if it is an heir of VillageHall class; false otherwise.</returns>
	bool IsVillageHall(void);
	
	/// <summary>
	/// This functionc checks if the the class of the current building is an heir of Townhall class,
	/// and consequently if it is a central building.
	/// </summary>
	/// <returns>True if it is an heir of Townhall class; false otherwise.</returns>
	bool IsTownhall(void);
	
	/// <summary>
	/// This functionc checks if the the class of the current building is an heir of Outpost class,
	/// and consequently if it is a central building.
	/// </summary>
	/// <returns>True if it is an heir of Outpost class; false otherwise.</returns>
	bool IsOutpost(void);
	
	/// <summary>
	/// This functionc checks if the the class of the current building is an heir of Shipyward class,
	/// and consequently if it is a central building.
	/// </summary>
	/// <returns>True if it is an heir of Shipyard class; false otherwise.</returns>
	bool IsShipyard(void);
	
	/// <summary>
	/// This function allows the current building to produce gold if it couldn't previously. 
	/// </summary>
	void StartGoldProduction(void);

	/// <summary>
	/// This function prevents the current building to produce gold if it could previously. 
	/// </summary>
	void StopGoldProduction(void);

	/// <summary>
	/// This function allows the current building to produce food if it couldn't previously. 
	/// </summary>
	void StartFoodProduction(void);

	/// <summary>
	/// This function prevents the current building to produce food if it could previously. 
	/// </summary>
	void StopFoodProduction(void);

	/// <summary>
	/// This function sets the path in which can be found the entity of the current building.
	/// </summary>
	/// <param name="par_ent_path">The path of the entity.</param>
	void SetEntPath(const std::string par_ent_path);

	/// <summary>
	/// This function sets the path in which can be found the pass table of the current building.
	/// </summary>
	/// <param name="par_ent_path">The path og the pass table.</param>
	void SetPassPath(const std::string par_pass_path);
	
	/// <summary>
	/// This functions sets the properties of the current building when it is being created. 
	/// The values of these properties were previously read 
	/// from an XML file and they are now stored into a ObjectData::ObjectXMLClassData object.
	/// In addition, if the building is a central building, a new settlement is also created; otherwise, the function checks
	/// if the is a existing  settlement close to the current building: if not, the building cannot be created.
	/// </summary>
	/// <param name="objData">The object in which are stored the properties of the current building.</param>
	/// <param name="_temporary">Boolean: true = the object is temporary (e.g. an object that is being inserted in the editor)</param>
	/// <returns>True if the building can be created; false otherwise.</returns>
	bool SetBuildingProperties(ObjectData::ObjectXMLClassData &objData, const bool _temporary = false);

	/// <summary>
	/// This function performs the rendering of the current building.
	/// </summary>
	/// <param name="picking">Checks if it's the picking phase.</param>
	/// <param name="clickID"></param>
	/// <param name="not_placeable"></param>
	void Render(const bool picking, const unsigned int clickID = 0) override;


	void SetStatus(const bool bIsCreated);

	/// <summary>
	/// (???) Da rivedere.
	/// </summary>
	/// <param name="b"></param>
	void setWaitingToBeErased(bool b) { waitingToBeErased = b; }

	/// <summary>
	/// (???) Da rivedere.
	/// </summary>
	/// <returns></returns>
	bool getWaitingToBeErased() { return waitingToBeErased; }

	Building(void);
	~Building(void);
protected:
	/// <summary>
	/// Protected method that checks if the object is placeable and sets the relative protected boolean
	/// </summary>
	void CheckIfPlaceable(void) override;

private:
	//game::ObjectUI* buildingUI;
	bool waitingToBeErased;
	bool bIsCreated;
	gui::Circle circle[2];
	unsigned int maxHealth;
	unsigned int repairRate;
	unsigned int loyaltyFearHealthPercent;
	bool bIsClickableInMimimap = false;
	bool bIsCentralBuilding = false;
	bool bIsOutpost = false;
	bool bIsTownhall = false;
	bool bIsVillageHall = false;
	bool bIsShipyard = false;
	bool bAutoRepair = false;
	bool bCanProduceGold = false;
	bool bCanProduceFood = false;
	std::string ent_path;
	std::vector<Unit> holdUnits;
	Settlement *settlement;
	//Private members:
	/// <summary>
	/// (???) Da rivedere, non funziona correttamente. 
	/// This function, called by the method Building::SetBuildingProperties, checks if there is an existing settlement close 
	/// to the current building.
	/// </summary>
	/// <param name="b">The building that has invoked the method Building::SetBuildingProperties.</param>
	/// <param name="_temporary">Boolean: true = the object is temporary (e.g. an object that is being inserted in the editor)</param>
	/// <returns>True if there is a settlement; false otherwise.</returns>
	static bool FindASettlement(Building* b, const bool _temporary = false);
	//static properties
	/*/
	class SettlementSet
	{
	public:
		Settlement *set;
		std::vector<float> xPoint;
		std::vector<float> yPoint;
		std::vector<float> radius;
	};
	*/
	static std::vector<Settlement*> settlementsList;
	//sound selectionSound; TODO
};