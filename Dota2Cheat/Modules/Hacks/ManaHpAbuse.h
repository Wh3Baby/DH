#pragma once
#include "../../SDK/pch.h"
#include "../../CheatSDK/include.h"
#include <stack>
#include <unordered_map>
#include <future>

namespace Modules {
	//Automatic mana & HP abuse with items like Arcane Boots or Faerie Fire
	inline class M_ManaHPAbuse {
	public:
		enum class Mode {
			Drop,
			Move
		};

		void ReorderItems(const std::map<CDOTAItem*, int>& positions);
		void DetermineBonusType(CDOTAItem* item);
		bool ItemNeedsExclusion(CDOTAItem* item);

		void ChangeItemStatTo(CDOTAItem* item, ItemStat stat, CBaseEntity* issuer);
		void GetItemsForExclusion(
			CDOTABaseNPC* npc,
			std::set<CDOTAItem*> preservedItems,
			std::map<CDOTAItem*, int>& cachedPositions,
			std::stack<CDOTAItem*>& itemsToExclude);
		enum BonusType {
			Mana,
			Health,
			Both
		};

		std::vector<const char*>* bonusTypes{};
		std::unordered_map<std::string, BonusType> abusedItems = {
			{"item_arcane_boots", Mana},
			{"item_soul_ring", Mana},
			{"item_enchanted_mango", Mana},
			{"item_arcane_ring", Mana},

			{"item_magic_stick", Both},
			{"item_magic_wand", Both},
			{"item_holy_locket" , Both},
			{"item_guardian_greaves", Both},
			{"item_cheese", Both},

			{"item_faerie_fire", Health},
			{"item_greater_faerie_fire", Health}
		};

		std::future<void> manaAbuseReverse;
		void DropMode(CDOTABaseNPC* npc, CDOTAItem* ability,
			std::map<CDOTAItem*, int>& cachedPositions,
			std::stack<CDOTAItem*>& itemsToExclude);
		void MoveMode(CDOTABaseNPC* npc, CDOTAItem* ability,
			std::map<CDOTAItem*, int>& cachedPositions, std::stack<CDOTAItem*>& itemsToExclude);
		void PerformAbuse(CDOTABaseNPC* npc, CDOTAItem* ability);
	} ManaHPAbuse{};
}