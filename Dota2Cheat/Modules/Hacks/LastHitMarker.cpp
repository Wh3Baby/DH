#include "LastHitMarker.h"

void Hacks::LastHitMarker::DrawCircleFor(CDOTABaseNPC* creep) {
	ImColor color = creep->IsSameTeam(ctx.localHero) ?
		ImColor{ 0,255,0 } :
		ImColor{ 255,0,0 };
	float radius = 10 * (1200 / Config::CameraDistance * 1.2);
	ImGui::GetForegroundDrawList()->AddCircleFilled(WorldToScreen(creep->GetHealthBarPos()), radius, color);
}

void Hacks::LastHitMarker::Draw() {
	if (!Config::LastHitMarker)
		return;

	bool hasQBlade = ctx.localHero->HasOneOfModifiers({
			"modifier_item_quelling_blade",
			"modifier_item_battlefury"
		});
	auto attackRange = ctx.localHero->GetAttackRange();
	for (auto& wrapper : ctx.creeps) {
		auto creep = wrapper.ent;
		if (!IsValidReadPtr(creep)
			|| !IsValidReadPtr(creep->GetIdentity())
			|| !creep->IsTargetable())
			continue;

		// Distance check
		if (!IsWithinRadius(
			creep->GetPos(),
			ctx.localHero->GetPos(),
			ctx.localHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 1000 : attackRange * 1.2
			))
			continue;

		// Deny check
		if (creep->IsSameTeam(ctx.localHero) && (float)creep->GetHealth() / creep->GetMaxHealth() >= 0.5f)
			continue;

		int dmg = ctx.localHero->GetAttackDamageMin();

		if (hasQBlade)
			dmg += ctx.localHero->GetAttackCapabilities() == DOTA_UNIT_CAP_MELEE_ATTACK ? 8 : 4;

		if (wrapper.creepType == CreepType::Siege)
			dmg *= 0.5f;

		float dmgReduction = (0.06f * creep->GetPhysicalArmorValue()) / (1 + 0.06f * abs(creep->GetPhysicalArmorValue()));
		// Damage check
		if (creep->GetHealth() >= dmg * (1 - dmgReduction))
			continue;

		DrawCircleFor(creep);
	}
}
