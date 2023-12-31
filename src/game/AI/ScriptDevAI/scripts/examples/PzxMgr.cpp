#include "PzxMgr.h"

INSTANTIATE_SINGLETON_1(PzxMgr);

bool PzxMgr::Reload()
{
	treeMenu.clear();
	allMenu.clear();
	accountPMap.clear();

	tfTimeMap.clear();
	acctTypeMap.clear();

	//loadAllPonits();
	//loadAllTimes();
	treeMenu = loadAllMenu(0);
	return true;
}


void PzxMgr::loadAllTimes()
{

	uint32 count = 0;

	QueryResult* result = CharacterDatabase.PQuery("SELECT guid,type,value,accid FROM _character_extra ");
	if (!result)
	{
		BarGoLink bar(1);
		bar.step();

		sLog.outString();
		sLog.outString(">> Loaded 0 creature respawn time.");
	}

	BarGoLink bar(result->GetRowCount());
	//CharaPointMap unMap;
	do
	{
		bar.step();
		Field* fields = result->Fetch();

		uint32 guid = fields[0].GetUInt32();
		uint32 type = fields[1].GetUInt32();
		uint64 val = fields[2].GetUInt64();
		uint64 accid = fields[3].GetUInt64();
		if (accid > 0 && acctTypeMap.find(accid) != acctTypeMap.end()) {//获取所有ID的值
			acctTypeMap[accid].insert(type);
		}
		else {
			std::set<uint32> tps;
			tps.insert(type);
			acctTypeMap.insert(std::pair<uint32, std::set<uint32>>(accid, tps));
		}

		if (tfTimeMap.find(guid) != tfTimeMap.end()) {//已经存在

			std::map<uint32, uint64>::iterator got = tfTimeMap[guid].find(type);
			if (got != tfTimeMap[guid].end()) {
				got->second = val;
			}
			else {
				tfTimeMap[guid].insert(std::pair<uint32, uint64>(type, val));
			}
		}
		else {//不存在
			std::map<uint32, uint64> newVVV;
			newVVV.insert(std::pair<uint32, uint64>(type, val));
			tfTimeMap.insert(std::pair<uint32, std::map<uint32, uint64>>(guid, newVVV));
		}
		++count;

	} while (result->NextRow());

	delete result;

	sLog.outString();
	sLog.outString(">> Loaded %u AccountPoint", count);

}
bool PzxMgr::updateTime(uint32 type, uint32 guid, uint32 accid, uint64 time) {
	QueryResult* result = CharacterDatabase.PQuery("SELECT guid,type,value FROM _character_extra where guid='%u' and type='%u'", guid, type);
	if (!result)
	{
		BarGoLink bar(1);
		bar.step();
		if (CharacterDatabase.PExecute("insert into _character_extra values ('%u','%u','%u','%u')", guid, type, time, accid)) {
			if (accid > 0 && acctTypeMap.find(accid) != acctTypeMap.end()) {//获取所有ID的值
				acctTypeMap[accid].insert(type);
			}
			else {
				std::set<uint32> tps;
				tps.insert(type);
				acctTypeMap.insert(std::pair<uint32, std::set<uint32>>(accid, tps));
			}
			//tfTimeMap 对应增加时长
			std::map<uint32, uint64>::iterator got = tfTimeMap[guid].find(type);
			if (got != tfTimeMap[guid].end()) {
				got->second = time;//更新值
			}
			else {
				tfTimeMap[guid].insert(std::pair<uint32, uint64>(type, time));
			}
			return true;
		}
		else {
			sLog.outString(">> [PZX] system error 1", time);
			return false;
		}


	}
	if (CharacterDatabase.DirectPExecute("update _character_extra set value='%u' ,accid='%u' where guid='%u' and type='%u'", time, accid, guid, type)) {//减少成功

		if (accid > 0 && acctTypeMap.find(accid) != acctTypeMap.end()) {//获取所有ID的值
			acctTypeMap[accid].insert(type);
		}
		else {
			std::set<uint32> tps;
			tps.insert(type);
			acctTypeMap.insert(std::pair<uint32, std::set<uint32>>(accid, tps));
		}

		std::map<uint32, uint64>::iterator got = tfTimeMap[guid].find(type);
		if (got != tfTimeMap[guid].end()) {
			got->second = time;//更新值
		}
		else {
			tfTimeMap[guid].insert(std::pair<uint32, uint64>(type, time));
		}
		return true;

	}
	else {
		sLog.outString(">> [PZX]system db error 2", time);
	}
	return false;
}

uint32 PzxMgr::getMyPoint(uint32 accountID) {
	AccountPointMap::const_iterator got = accountPMap.find(accountID);

	if (got != accountPMap.end()) {
		if (got->second.val < 0)
			return 0;
		else
			return got->second.val;
	}
	return 0;
}
MenuTree  PzxMgr::getTreeByID(uint32 id) {
	CharaMenuMap::const_iterator got = allMenu.find(id);

	if (got != allMenu.end()) {
		return got->second;
	}
	return{};
}
uint64 PzxMgr::getTimeLength(uint32 guid, uint32 type) {
	std::map<uint32, std::map<uint32, uint64>> ::iterator it = tfTimeMap.find(guid);

	if (it != tfTimeMap.end()) {
		//判断双天赋是否超期需要重新开启
		std::map<uint32, uint64> v = it->second;
		std::map<uint32, uint64>::const_iterator timeIT = v.find(type);
		if (timeIT != v.end()) {
			return timeIT->second;
		}
		else
			return 0;
	}
	else
		return 0;
}

bool PzxMgr::getAcctType(uint32 accid, uint32 type) {
	if (accid > 0 && acctTypeMap.find(accid) != acctTypeMap.end()) {//获取所有ID的值
		std::set<uint32> itTypes = acctTypeMap[accid];
		//std::set<uint32> ::iterator iter;
		return itTypes.find(type) != itTypes.end();
	}
	else {
		return false;
	}
}
bool PzxMgr::isTimeOut(uint32 guid, uint32 type) {
	std::map<uint32, std::map<uint32, uint64>> ::iterator it = tfTimeMap.find(guid);

	if (it != tfTimeMap.end()) {
		//判断双天赋是否超期需要重新开启
		std::map<uint32, uint64> v = it->second;
		std::map<uint32, uint64>::const_iterator timeIT = v.find(type);
		if (timeIT != v.end()) {
			MenuTree getTree = getTreeByID(type);
			return sizeof(getTree) > 1 ? time(nullptr) > (/**getTree.itemid + **/timeIT->second) : true;
		}
		else
			return true;
	}
	else
		return true;
}

bool PzxMgr::addPoints(uint32 cutPoint, uint32 accountID) {
	bool ret = LoginDatabase.DirectPExecute("update account set zanzu=zanzu+'%u' where id='%u'", cutPoint, accountID);

	if (ret) {//增加成功

		AccountPointMap::iterator got = accountPMap.find(accountID);
		if (got != accountPMap.end()) {
			uint32 newVel = got->second.val + cutPoint;
			accountPMap[got->first].val = newVel;//更新值
			sLog.outString(u8">> [PZX]1id:%d + %d point", accountID, cutPoint);

		}
		else {
			sLog.outString(u8">> [PZX ERROR]2id:%d + %d point", accountID, cutPoint);
		}
		return true;
	}
	else {
		sLog.outString(u8">> [PZX ERROR]3id:%d + %d point", accountID, cutPoint);
	}
	return false;
}
bool PzxMgr::cutPoints(int cutPoint, uint32 accountID) {
	if (cutPoint == 0) {//免费的
		return true;
	}
	bool ret = false;
	if (cutPoint > 0) {
		ret = LoginDatabase.DirectPExecute("update _account_extra set val=val-'%u' where id='%u'", cutPoint, accountID);
	}
	else {
		ret = LoginDatabase.DirectPExecute("update _account_extra set val=val+'%u' where id='%u'", -cutPoint, accountID);
	}

	if (ret) {//减少成功
		AccountPointMap::iterator got = accountPMap.find(accountID);
		if (got != accountPMap.end()) {
			uint32 newVel = got->second.val - cutPoint;
			accountPMap[got->first].val = newVel;//更新值
			sLog.outString(u8">> [PZX]%d 扣减 %d", accountID, cutPoint);
			return true;
		}
		else {
			sLog.outString(u8">> [PZX]%d 系统异常a %d", accountID, cutPoint);
		}
	}
	else {
		sLog.outString(u8">> [PZX]%d 系统异常b %d", accountID, cutPoint);
	}
	return false;
}

void PzxMgr::loadAllPonits()
{
	try
	{


		//startLienster = true;
		uint32 count = 0;
		QueryResult* result = LoginDatabase.PQuery("SELECT id,idx,val FROM _account_extra ");
		if (!result)
		{
			BarGoLink bar(1);
			bar.step();

			sLog.outString();
			sLog.outString(">> Loaded 0 Player Ponits.");
			//return;//这里return有点BUG，需要至少有一条数据才触发，基本上都有，先去掉
		}

		BarGoLink bar(result->GetRowCount());
		//CharaPointMap unMap;
		do
		{
			bar.step();
			Field* fields = result->Fetch();

			uint32 accountID = fields[0].GetUInt32();
			uint32 idx = fields[1].GetUInt32();
			int32 val = fields[2].GetUInt32();


			AccountPointMap::iterator got = accountPMap.find(accountID);
			if (got != accountPMap.end()) {
				got->second.val = val;//更新值,定时器做同步
			}
			else {
				AccountPoint pt = { accountID, idx, val };
				accountPMap.insert(std::pair<uint32, AccountPoint>(accountID, pt));
			}

			++count;

		} while (result->NextRow());

		delete result;

		//同步积分 这里只能有一个服务器做
		if (sConfigPzx.GetIntDefault("server.computerPoint", 0)) {

			_AccountPointMap _accountPMap;
			try
			{
				//查询
				QueryResult* result2 = LoginDatabase.PQuery("SELECT id,zanzu FROM account  where zanzu>1");
				if (!result2)
				{
					BarGoLink bar(1);
					bar.step();
					return;
				}

				BarGoLink bar2(result2->GetRowCount());
				do
				{
					bar2.step();
					Field* fields = result2->Fetch();
					uint32 accountID = fields[0].GetUInt32();
					int32 val = fields[1].GetInt32();
					_AccountPointMap::iterator got = _accountPMap.find(accountID);
					if (got != _accountPMap.end()) {
						got->second = val;//更新值,定时器做同步
					}
					else {
						_accountPMap.insert(std::pair<uint32, int32>(accountID, val));
					}
				} while (result2->NextRow());
				delete result2;
			}
			catch (...)
			{
				sLog.outError("[pzx-exception-SEH-loadAllPonits1]");

			}
			_AccountPointMap::iterator sysnIt = _accountPMap.begin();
			while (sysnIt != _accountPMap.end()) {
				uint32 accID = sysnIt->first;
				int32 synVal = sysnIt->second - 1;
				AccountPointMap::iterator got = accountPMap.find(accID);
				if (got != accountPMap.end()) {//以前有数据，清理account，增加_account_extra
					sLog.outString(">> Syn Player point ==> accid=%d,point=%d", accID, synVal);
					//1 做update
					LoginDatabase.PExecute("update account set zanzu=1 where id='%u'", accID);//清零
					LoginDatabase.PExecute("update _account_extra set val=val+%u where id='%u'", synVal, accID);//清零

					//2更新map
					got->second.val = got->second.val + synVal;//更新值,定时器做同步
				}
				else {//以前没充值过，新增数据并且清理
					sLog.outString(">> Create Player point ==> accid=%d,point=%d", accID, synVal);
					//1 做插入语句
					LoginDatabase.PExecute("INSERT INTO _account_extra (`id`, `idx`, `val`) VALUES('%u','1','%u')", accID, synVal);//增加
					AccountPoint pt = { accID, 1, synVal };
					accountPMap.insert(std::pair<uint32, AccountPoint>(accID, pt));
					LoginDatabase.PExecute("update account set 1=1 where id='%u'", accID);//清零
				}
				sysnIt++;
			}
			sLog.outString();
			sLog.outString(">> Loaded %u AccountPoint", count);
		}
	}
	catch (...)
	{
		sLog.outError("[pzx-exception-SEH-loadAllPonits2]");

	}
	//startLienster = false;
}
CharaMenuMap PzxMgr::loadAllMenu(uint32 pid)
{
	uint32 count = 0;
	QueryResult* result = CharacterDatabase.PQuery("SELECT * FROM _pzx_cardTree where pid='%u'", pid);
	if (!result)
	{
		BarGoLink bar(1);
		bar.step();

		sLog.outString();
		sLog.outString(">> Loaded p-menu:%u empty ", pid);
		return{};
	}
	BarGoLink bar(result->GetRowCount());
	CharaMenuMap menuMap;
	do
	{
		bar.step();
		MenuTree pt;
		Field* fields = result->Fetch();
		pt.id = fields[0].GetUInt32();
		pt.pid = fields[1].GetUInt32();
		pt.name = fields[2].GetString();
		pt.unionID = fields[3].GetUInt32();
		pt.needval = fields[4].GetUInt32();
		pt.racemask = fields[5].GetUInt32();
		uint32 type = fields[6].GetUInt32();
		pt.type = type;
		string param1 = fields[7].GetString();
		string param2 = fields[8].GetString();
		string param3 = fields[9].GetString();
		string param4 = fields[10].GetString();
		string param5 = fields[11].GetString();
		try
		{

	
		switch (type)
		{
		case T_TEL_TO_XYZ:
			m_Tele_xyzm m_tele;
			m_tele.x = stof(param1);
			m_tele.y = stof(param2);
			m_tele.z = stof(param3);
			m_tele.mapid = stof(param4);
			pt.m_tele_xyzm = m_tele;
			break;
		case T_BUY_TIME:
		{
			m_BuyItem m_buyItem;
			m_buyItem.itemID = stod(param1);
			if (param2.empty()) {
				m_buyItem.itemNum = 1;
			}
			else {
				m_buyItem.itemNum = stod(param2);
			}
			pt.m_buyItem = m_buyItem;
		}
		case T_LEARN_SKILL:
		{
			m_Skill m_skill;
			m_skill.learnSkill = static_cast<SkillType>(stod(param1));
			pt.m_skill = m_skill;
		}
		break;
		case T_LEARN_SPELL:
		{
			m_Spell m_spell;
			m_spell.learnSpell = stod(param1);
			pt.m_spell = m_spell;
		}
		break;
		case T_ITEM_ENCHANT:
		{
			m_Enchant m_enchant;
			m_enchant.itemSlot = static_cast<EquipmentSlots>(stod(param1));
			m_enchant.enchatSpell = stod(param2);
			m_enchant.hasSubclass = false;
			if (!param3.empty()) {

				vector<string> result;

				split(result, param3, boost::is_any_of(","));
				vector<uint32> subclasses;
				for (const std::string& str : result) {
					subclasses.push_back(stod(str));
				}
				m_enchant.subclass = subclasses;
				m_enchant.hasSubclass = true;
			}

			pt.m_enchant = m_enchant;
		}break;

		default:
			break;
		}
		}
		catch (const std::exception& e)
		{
			//这条记录有问题，不加载
			sLog.outError(">> Loaded menu-id:%u exception,%s",pt.id,e.what());
			continue;

		}
		pt.unioncheck = fields[12].GetUInt32();

		pt.popMenu = fields[13].GetUInt32();
		pt.iconID = fields[14].GetUInt32();
		pt.children = loadAllMenu(pt.id);
		//1.先找出pid为0的菜单项目

		//MenuTree pt = { id, pid, name,unionID,needval,raceMask,type,unioncheck,popMenu,iconID,loadAllMenu(id) };
		//sLog.outString("loadAllMenu4");
		pair<uint32, MenuTree> menu(pt.id, pt);
		menuMap.insert(menu);
		allMenu.insert(menu);
		++count;
	} while (result->NextRow());
	delete result;
	sLog.outString();
	sLog.outString(">> Loaded menu:%u ,size:%u", pid, count);

	return menuMap;

}