
#include "custome_common.h"
using namespace std;


void pzx_Enchant(Player* player, Item* item, uint32 enchantid)
{
	if (!item)
	{
		player->GetSession()->SendNotification("You must first equip the item you are trying to enchant.");
		return;
	}

	if (!enchantid)
	{
		player->GetSession()->SendNotification("Something went wrong.");
		return;
	}

	item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
	item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
	player->GetSession()->SendNotification("%s succesfully enchanted", item->GetProto()->Name1);
}



void pzx_LearnSkillRecipesHelper(Player* player, uint32 skill_id)
{
	uint32 classmask = player->getClassMask();

	for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
	{
		SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
		if (!skillLine)
			continue;

		// wrong skill
		if (skillLine->skillId != skill_id)
			continue;

		// not high rank
		if (skillLine->forward_spellid)
			continue;

		// skip racial skills
		if (skillLine->racemask != 0)
			continue;

		// skip wrong class skills
		if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
			continue;

		SpellEntry const* spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(skillLine->spellId);
		if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
			continue;

		player->learnSpell(skillLine->spellId, false);
	}
}

bool pzx_LearnAllRecipesInProfession(Player* pPlayer, SkillType skill)
{
	ChatHandler handler(pPlayer->GetSession());
	char* skill_name;

	SkillLineEntry const* SkillInfo = sSkillLineStore.LookupEntry(skill);
	skill_name = SkillInfo->name[sWorld.GetDefaultDbcLocale()];

	if (!SkillInfo)
	{
		sLog.outError("Profession NPC: received non-valid skill ID");
		return false;
	}

	pPlayer->SetSkill(SkillInfo->id, 375, 375);
	pzx_LearnSkillRecipesHelper(pPlayer, SkillInfo->id);
	pPlayer->GetSession()->SendNotification("All recipes for %s learned", skill_name);
	return true;
}



void pzx_CompleteLearnProfession(Player* pPlayer, Creature* pCreature, SkillType skill)
{
	if (pPlayer->GetFreePrimaryProfessionPoints() == 0 && !(skill == SKILL_COOKING || skill == SKILL_FIRST_AID))
	{
		pPlayer->GetSession()->SendNotification("You already know two primary professions.");
	}
	else
	{
		if (!pzx_LearnAllRecipesInProfession(pPlayer, skill))
			pPlayer->GetSession()->SendNotification("Internal error.");
	}
}

#define SPELL_LIGHTNING_VISUAL 24240

bool GossipHello_PremadeGearNPC(Player* player, Creature* creature, int pzx_sender)
{
	for (auto itr : sObjectMgr.GetPlayerPremadeGearTemplates())
	{
		if (itr.second.requiredClass == player->getClass())
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, itr.second.name.c_str(), pzx_sender, itr.first);
		}
	}

	player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetObjectGuid());
	return true;
}

bool GossipSelect_PremadeGearNPC(Player* player, Creature* creature, uint32 sender, uint32 action)
{
	player->PlaySpellVisual(SPELL_LIGHTNING_VISUAL);
	sObjectMgr.ApplyPremadeGearTemplateToPlayer(action, player);
	player->CLOSE_GOSSIP_MENU();
	return true;
}

bool GossipHello_PremadeSpecNPC(Player* player, Creature* creature, int pzx_sender)
{
	for (auto itr : sObjectMgr.GetPlayerPremadeSpecTemplates())
	{
		if (itr.second.requiredClass == player->getClass())
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, itr.second.name.c_str(), pzx_sender, itr.first);
		}
	}

	player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetObjectGuid());
	return true;
}

bool GossipSelect_PremadeSpecNPC(Player* player, Creature* creature, uint32 sender, uint32 action)
{
	player->PlaySpellVisual(SPELL_LIGHTNING_VISUAL);
	sObjectMgr.ApplyPremadeSpecTemplateToPlayer(action, player);
	player->CLOSE_GOSSIP_MENU();
	return true;
}