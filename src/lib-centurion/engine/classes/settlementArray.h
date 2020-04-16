/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <header.h>

class Settlement;

namespace SettlementArray
{
	/// <summary>
	/// This static function cleans common settlements list using in the editor.
	/// </summary>
	void ResetSettlementsList(void);

	std::vector<Settlement*>* GetSettlementListPtr(void);

	Settlement* GetSettlementById(const unsigned int id);

	void PushSettlement(Settlement* settlPtr);

	void EraseSettlement(Settlement* settlPtr);

	unsigned int Size(void);

};