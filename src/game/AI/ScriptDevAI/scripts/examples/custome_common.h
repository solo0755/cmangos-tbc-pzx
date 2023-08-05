#pragma once

#ifndef SC_COMMON_H
#define SC_COMMON_H
#include <ctime>
#include <string> 
#include <codecvt>
#include <regex>
#include "AI/ScriptDevAI/include/sc_common.h"
#include "World/World.h"
#include "Globals/ObjectMgr.h"
enum Enchants
{
	WEP2H_SUPERIOR_IMPACT = 20,
	WEP2H_AGILITY,
	WEP_CRUSADER,
	WEP1H_AGILITY,
	WEP_SPELLPOWER,
	WEP_HEAL,
	OFFHAND_SPIRIT,
	OFFHAND_STAM,
	OFFHAND_FROSTRES,
	CHEST_STATS,
	CLOAK_DODGE,
	CLOAK_SUB,
	CLOAK_ARMOR,
	CLOAK_AGILITY,
	BRACER_STAM,
	BRACER_STR,
	BRACER_HEAL,
	BRACER_INT,
	GLOVES_AGI,
	GLOVES_FIRE,
	GLOVES_FROST,
	GLOVES_SHADOW,
	GLOVES_HEALING,
	BOOTS_AGI,
	BOOTS_SPEED,
	BOOTS_STAM,
};


void  pzx_Enchant(Player* player, Item* item, uint32 enchantid);
void  pzx_CompleteLearnProfession(Player* pPlayer, Creature* pCreature, SkillType skill);



#define SPELL_LIGHTNING_VISUAL 24240

bool GossipHello_PremadeGearNPC(Player* player, Creature* creature, int pzx_sender);

bool GossipSelect_PremadeGearNPC(Player* player, Creature* creature, uint32 sender, uint32 action);

bool GossipHello_PremadeSpecNPC(Player* player, Creature* creature, int pzx_sender);

bool GossipSelect_PremadeSpecNPC(Player* player, Creature* creature, uint32 sender, uint32 action);








#endif