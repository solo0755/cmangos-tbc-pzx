/* Copyright (C) 2009 - 2010 ScriptDevZero <http://github.com/scriptdevzero/scriptdevzero>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "PzxMgr.h"
#include "custome_common.h"
#include <ctime>


bool GossipHello_Pzx(Player* player, Creature* _Creature)
{
	CharaMenuMap mmap = sPzxMgr.treeMenu;
	CharaMenuMap::iterator it;
	for (it = mmap.begin(); it != mmap.end(); it++) {
		uint32 id = it->first;
		MenuTree tree = it->second;
		uint32 unionid = tree.unionID;//时长判断
		uint32 unioncheck = tree.unioncheck;
		uint32 raceMask = tree.racemask;
		if (raceMask && ((player->getRaceMask() & raceMask) == 0)) {
			continue;
		}
		player->ADD_GOSSIP_ITEM(tree.iconID, tree.name.c_str(), GOSSIP_SENDER_MAIN, id);

	}
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetObjectGuid());
	return true;
}
bool GossipSelect_Pzx(Player* player, Creature* _Creature, uint32 sender, uint32 action) {

	if (sender == PZX_GEAR) {
		GossipSelect_PremadeGearNPC(player, _Creature, sender, action);
		return false;
	}
	if (sender == PZX_SPEC) {
		GossipSelect_PremadeSpecNPC(player, _Creature, sender, action);
		return false;
	}
	CharaMenuMap allmap = sPzxMgr.allMenu;
	if (action < 100) {
		if (action == 99)
			GossipHello_Pzx(player, _Creature);
		return false;
	}
	CharaMenuMap::const_iterator got = allmap.find(action);
	if (got != allmap.end()) {
		MenuTree getTree = got->second;
		CharaMenuMap mmap = getTree.children;
		uint32 newActionID = getTree.id;
		if (mmap.size() > 0) {//涵盖子菜单
			CharaMenuMap::iterator it;
			for (it = mmap.begin(); it != mmap.end(); it++) {

				uint32 id = it->first;
				MenuTree tree = it->second;
				uint32 raceMask = tree.racemask;
				if (raceMask && ((player->getRaceMask() & raceMask) == 0)) {
					continue;
				}
				//TODO 此处不坚持union项
				if (tree.popMenu) {
					player->ADD_GOSSIP_ITEM_EXTENDED(tree.iconID, tree.name.c_str(), GOSSIP_SENDER_MAIN, id, "abcdefg", 0, true);
				}
				else {
					player->ADD_GOSSIP_ITEM(tree.iconID, tree.name.c_str(), GOSSIP_SENDER_MAIN, id);
				}
			}
			player->ADD_GOSSIP_ITEM(0, u8"返回主菜单", GOSSIP_SENDER_MAIN, 99);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetObjectGuid());
			return true;
		}
		else {//动作菜单

			switch (got->second.type)
			{
			case T_BACK_TO_MAIN://返回主菜单
			{
				GossipHello_Pzx(player, _Creature);
				break;
			}
			case T_GM_RELOAD:
			{
				sPzxMgr.Reload();
				sConfigPzx.Reload();
				player->CLOSE_GOSSIP_MENU();
				break;
			}
			case T_INIT_Gear:
				GossipHello_PremadeGearNPC(player,_Creature, PZX_GEAR);
				break;
			case T_INIT_Spec:
				GossipHello_PremadeSpecNPC(player, _Creature, PZX_SPEC);
				break;
			case T_TEL_TO_XYZ:
			{
				m_Tele_xyzm m_tele = getTree.m_tele_xyzm;
				player->TeleportTo(m_tele.mapid, m_tele.x, m_tele.y, m_tele.z, 0.0f);
				player->CLOSE_GOSSIP_MENU();
				break;
			}
			case T_ITEM_ENCHANT:
			{
				m_Enchant enchant = getTree.m_enchant;
				Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, enchant.itemSlot);;
				if (item && enchant.hasSubclass)
				{
					ItemPrototype const* itemPro = item->GetProto();
					bool fit = false;
					for (const uint32& ss: enchant.subclass)
					{
						if (itemPro->SubClass  == ss) {
							fit = true;
						}
					}
					if (!fit) {
						player->GetSession()->SendNotification(u8"需要正确的物品类型");
						player->CLOSE_GOSSIP_MENU();
						return true;
					}
		
				}
				pzx_Enchant(player, item, enchant.enchatSpell);
				player->CLOSE_GOSSIP_MENU();
				break;
			}
			case T_LEARN_SKILL:
			{
				SkillType skill = getTree.m_skill.learnSkill;
				if (!player->HasSkill(skill))
					pzx_CompleteLearnProfession(player, _Creature, skill);
				player->CLOSE_GOSSIP_MENU();
				break;
			}
			case T_BUY_TIME:
			{
				m_BuyItem m_buyItem = getTree.m_buyItem;
				//TODO  增加物品

			}
			default:
				break;
			}
		}
	}
	else {//没有这个菜单
		player->GetSession()->SendNotification("Something went wrong.no  menu");
		player->CLOSE_GOSSIP_MENU();
	}

	return true;
}

// TELEPORT NPC





/*
* Custom premade gear and spec scripts
*/




void AddSC_custom_creatures_pzx()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "custom_teleport_npc";
	newscript->pGossipHello = &GossipHello_Pzx;
	newscript->pGossipSelect = &GossipSelect_Pzx;
	newscript->RegisterSelf(false);

	//newscript = new Script;
	//newscript->Name = "custom_enchant_npc";
	//newscript->pGossipHello = &GossipHello_EnchantNPC;
	//newscript->pGossipSelect = &GossipSelect_EnchantNPC;
	//newscript->RegisterSelf(false);

	//newscript = new Script;
	//newscript->Name = "custom_professions_npc";
	//newscript->pGossipHello = &GossipHello_ProfessionNPC;
	//newscript->pGossipSelect = &GossipSelect_ProfessionNPC;
	//newscript->RegisterSelf(false);

	//newscript = new Script;
	//newscript->Name = "custom_premade_gear_npc";
	//newscript->pGossipHello = &GossipHello_PremadeGearNPC;
	//newscript->pGossipSelect = &GossipSelect_PremadeGearNPC;
	//newscript->RegisterSelf(false);

	//newscript = new Script;
	//newscript->Name = "custom_premade_spec_npc";
	//newscript->pGossipHello = &GossipHello_PremadeSpecNPC;
	//newscript->pGossipSelect = &GossipSelect_PremadeSpecNPC;
	//newscript->RegisterSelf(false);


}
