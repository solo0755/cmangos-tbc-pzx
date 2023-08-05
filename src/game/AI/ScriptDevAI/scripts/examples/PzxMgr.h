
#include "Config/ConfigPzx.h"
#include "Platform/Define.h"
#include "Database/DatabaseEnv.h"
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace std; 
using namespace boost;
#define PZX_GEAR 998
#define PZX_SPEC 999
struct m_Tele_xyzm {
	float x;
	float y;
	float z;
	uint32 mapid;
};
struct m_Enchant {
	EquipmentSlots itemSlot;
	uint32 enchatSpell;
	vector<uint32> subclass;
	bool hasSubclass;
};
struct m_Spell {
	uint32 learnSpell;
};
struct m_Skill {
	SkillType learnSkill;
};
struct m_BuyItem {
	uint32 itemID;
	uint32 itemNum;
};

struct MenuTree
{
	uint32 id;
	uint32 pid;
	string name;
	uint32 unionID;//同级互斥编号
	uint32 needval;
	uint32 racemask;//种族掩码
	uint32 type; //菜单类型
	m_Tele_xyzm m_tele_xyzm;
	m_Enchant m_enchant;
	m_Spell m_spell;
	m_Skill m_skill;
	m_BuyItem m_buyItem;
	uint32 unioncheck; //互斥开关
	uint32 popMenu; //是否是弹出框
	uint32 iconID; //是否是弹出框
	map<uint32, MenuTree> children;

};
enum PzxMenuType //菜单类型
{
	T_BACK_TO_MAIN = 1,//
	T_GM_RELOAD = 2,//
	T_INIT_Gear = 3,//
	T_INIT_Spec = 4,//
	T_TEL_TO_XYZ = 11,//
	T_BUY_TIME = 12, //
	T_LEARN_SKILL = 13, //
	T_LEARN_SPELL = 14, //
	T_ITEM_ENCHANT = 15 //
};
enum GmOnOffOptions //内存开光
{
	GMOPTIONS_O1 = 0,//
	GMOPTIONS_O2 = 1, //
	GMOPTIONS_O3 = 2, //
	GMOPTIONS_O4 = 3 //
};

#define MAX_GmOnOffOptions 4

struct AccountPoint {
	uint32 id;
	uint32 idx;
	int32 val;

};
typedef map<uint32, AccountPoint> AccountPointMap;
typedef map<uint32, MenuTree> CharaMenuMap;
typedef map<uint32, int32> _AccountPointMap;

class PzxMgr
{
    public:
		bool startLienster=false;
		CharaMenuMap treeMenu;
		CharaMenuMap allMenu;
		 AccountPointMap accountPMap;
		int32 m_GMONOFFOPTONS[MAX_GmOnOffOptions] = { 0 };
		map<uint32, map<uint32, uint64>> tfTimeMap;
		map<uint32, set<uint32>> acctTypeMap;
		bool Reload();
		void loadAllTimes();
		uint32 getMyPoint(uint32 accountID);
		bool updateTime(uint32 type, uint32 guid, uint32 accid,uint64 time);
		bool cutPoints(int cutPoint, uint32 accountID);
		bool addPoints(uint32 cutPoint, uint32 accountID);
		CharaMenuMap loadAllMenu(uint32 pid);
		MenuTree getTreeByID(uint32 pid);
		bool isTimeOut(uint32 guid, uint32 type);
		uint64 getTimeLength(uint32 guid, uint32 type);
		bool getAcctType(uint32 accid, uint32 type);
		 void loadAllPonits();
};

#define sPzxMgr MaNGOS::Singleton<PzxMgr>::Instance()
/// @}

