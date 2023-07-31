
#include "Config/ConfigPzx.h"
#include "Platform/Define.h"
#include "Database/DatabaseEnv.h"



struct MenuTree
{
	uint32 id;
	uint32 pid;
	std::string name;
	uint32 unionID;//������
	uint32 needval;
	uint32 type; //�˵����ͣ�0�˵�,1���ؽڵ㣬2������Ʒ��3����ʱ����4ѧϰ����,5���͵ص�
	uint32 racemask;//��������
	uint32 itemid;//��ƷID
	uint32 item_count;//��Ʒ����
	std::string   xyz; //����λ��
	//uint32 spellid; //����ID
	//uint32 timeLength; //ʱ��
	uint32 unioncheck; //���⿪��
	uint32 popMenu; //�Ƿ��ǵ�����
	uint32 iconID; //�Ƿ��ǵ�����
	std::map<uint32, MenuTree> children;

};
enum PzxMenuType //�˵�����
{
	T_BACK_TO_MAIN = 1,//
	T_GM_RELOAD = 2,//
	T_TEL_TO_XYZ = 11,//
	T_BUY_TIME = 12, //
	T_LEARN_SKILL = 13, //
	T_LEARN_SPELL = 14, //
	T_ITEM_ENCHAT = 15 //
};
enum GmOnOffOptions //�ڴ濪��
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
typedef std::map<uint32, AccountPoint> AccountPointMap;
typedef std::map<uint32, MenuTree> CharaMenuMap;
typedef std::map<uint32, int32> _AccountPointMap;

class PzxMgr
{
    public:
		bool startLienster=false;
		CharaMenuMap treeMenu;
		CharaMenuMap allMenu;
		 AccountPointMap accountPMap;
		int32 m_GMONOFFOPTONS[MAX_GmOnOffOptions] = { 0 };
		std::map<uint32, std::map<uint32, uint64>> tfTimeMap;
		std::map<uint32, std::set<uint32>> acctTypeMap;
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

