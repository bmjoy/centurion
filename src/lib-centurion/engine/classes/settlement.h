/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <string>
#include "building.h"

#ifndef POPULATION_LIMIT
#define POPULATION_LIMIT   10000
#endif 

#ifndef GOLD_LIMIT
#define GOLD_LIMIT   1000000000
#endif 

#ifndef FOOD_LIMIT
#define FOOD_LIMIT   1000000000
#endif 

/// <summary>
/// This class represents a set of buildings that cooperate each other and are closely conencted each other.
/// </summary>
class Settlement
{
public:
	/// <summary>
	/// This function return the name of the current settlement.
	/// </summary>
	/// <returns>The name of the current settlement.</returns>
	std::string GetSettlementName(void);
	/// <summary>
	/// This function sets the name of the current settlement. This name can be usad into the scripts.
	/// </summary>
	/// <param name="par_settlementName">A name for the current settlement.</param>
	void SetSettlementName(const std::string par_settlementName);
	/// <summary>
	/// This function checks if the settlement belong to a player that is not a human player or a player drived by the AI.
	/// </summary>
	/// <returns>True if it is indipendet; false otherwise.</returns>
	bool IsIndipendent(void);

	/// <summary>
	/// This functions returns the amount of food currently owned by the current settlement.
	/// </summary>
	/// <returns>The amount of food.</returns>
	unsigned int GetFood(void);
	/// <summary>
	/// This functions sets the amount of food currently owned by the current settlement.
	/// </summary>
	/// <returns>The amount of food.</returns>
	void SetFood(const unsigned int par_food);
	/// <summary>
	/// This function returns the amount of gold currently owned by the current settlement.
	/// </summary>
	/// <returns>A negative value if the current settlement has a debit; a value greater than or equal to 0 otherwise.</returns>
	int GetGold(void);
	/// <summary>
	/// This function sets the amount of gold currently owned by the current settlement.
	/// </summary>
	/// <param name="par_gold">A negative value if the settlement has a debit; a value greater than or equal to 0 otherwise.</param>
	void SetGold(int par_gold);
	/// <summary>
	/// This function returns the population currentlyowned by the crrent settlement.
	/// </summary>
	/// <returns>The population value.</returns>
	unsigned int GetPopulation(void);
	/// <summary>
	/// This function sets the population currentlyowned by the crrent settlement.
	/// </summary>
	/// <param name="par_population">The population value.</param>
	void SetPopulation(const unsigned int par_population);
	/// <summary>
	/// This function returns the max number of population that the current settlement can at most have.
	/// </summary>
	/// <returns>The max number of population.</returns>
	unsigned int GetMaxPopulation(void);
	/// <summary>
	/// /// This function sets the max number of population that the current settlement can at most have.
	/// </summary>
	/// <param name="par_max_population">The max number of population.</param>
	void SetMaxPopulation(const unsigned int par_max_population);
	/// <summary>
	/// This function return the player to whom the current settlement belongs to.
	/// </summary>
	/// <returns>The playner number.</returns>
	unsigned int GetPlayer(void);
	/// <summary>
	/// This function sets the player to whom the current settlement belongs to.
	/// </summary>
	/// <param name="par_player">The number of the player.</param>
	void SetPlayer(const unsigned int par_player);

	/// <summary>
	/// This function provides a list of all the buildings belonging to the current settlement.
	/// </summary>
	/// <returns>A list of pointers to buildings; this list can be empty if the settlement hasn't got any building.</returns>
	const std::vector<Building*> GetBuildingsBelongToSettlement(void);
	/// <summary>
	/// This function allows to add a new building to the current settlement.
	/// </summary>
	/// <param name="b">A pointer to the building that you want to add to the crrent settlement.</param>
	/// <returns>True if the building was correctly added; false otherwise (namely if the building was already added to the current settleemnt).</returns>
	bool AddBuildingToSettlement(Building* b);
	/// <summary>
	/// /// This function allows to remove a new building to the current settlement.
	/// </summary>
	/// <param name="b">A pointer to the building that you want to remove to the crrent settlement.</param>
	/// <returns>True if the building was correctly removed; false otherwise (namely if the building was never added to the current settleemnt).</returns>
	bool RemoveBuildingFromSettlement(Building* b);

	/// <summary>
	/// Public constructor.
	/// </summary>
	/// <param name="par_player">The player to whom the settlement belongs to.</param>
	Settlement(const unsigned int par_player);
	/// <summary>
	/// The destructor.
	/// </summary>
	~Settlement() {};
private:
	bool bIsIndipendent = false;
	std::string scriptName;
	unsigned int food;
	int gold;
	unsigned int player;
	unsigned int population;
	unsigned int max_population;
	std::map<unsigned int, Building*> buildingsOfSettlement;
};