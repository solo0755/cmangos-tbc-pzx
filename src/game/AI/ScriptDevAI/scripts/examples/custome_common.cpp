#include <ctime>
#include <string> 
#include <codecvt>
#include <regex>
#include "AI/ScriptDevAI/include/sc_common.h"


void SkipWhiteSpaces(char** args)
{
	if (!*args)
		return;

	while (isWhiteSpace(**args))
		++(*args);
}
bool  ExtractUInt32Base(char** args, uint32& val, uint32 base)
{
	if (!*args || !**args)
		return false;

	char* tail = *args;

	unsigned long valRaw = strtoul(*args, &tail, base);

	if (tail != *args && isWhiteSpace(*tail))
		*(tail++) = '\0';
	else if (tail && *tail)                                 // some not whitespace symbol
		return false;                                       // args not modified and can be re-parsed

	if (valRaw > std::numeric_limits<uint32>::max())
		return false;

	// value successfully extracted
	val = uint32(valRaw);
	*args = tail;

	SkipWhiteSpaces(args);
	return true;
}
bool  ExtractUInt32(char** args, uint32& val) { return ExtractUInt32Base(args, val, 10); }
bool  ExtractOptUInt32(char** args, uint32& val, uint32 defVal)
{
	if (!*args || !**args)
	{
		val = defVal;
		return true;
	}

	return ExtractUInt32(args, val);
}

bool  ExtractFloat(char** args, float& val)
{
	if (!*args || !**args)
		return false;

	char* tail = *args;

	double valRaw = strtod(*args, &tail);

	if (tail != *args && isWhiteSpace(*tail))
		*(tail++) = '\0';
	else if (tail && *tail)                                 // some not whitespace symbol
		return false;                                       // args not modified and can be re-parsed

															// value successfully extracted
	val = float(valRaw);
	*args = tail;

	SkipWhiteSpaces(args);
	return true;
}


bool teleToPoint(Player* player, std::string xyz, uint32 needval) {
	uint32 accID = player->GetSession()->GetAccountId();

	const char* _args = xyz.c_str();
	char* args = const_cast<char*>(_args);
	sLog.outString(u8">> [PZX] xyzzzzzz=%s", args);
	float x;
	if (!ExtractFloat(&args, x)) {
		sLog.outString(u8">> [PZX] xyz1=%s", args);
		return false;
	}

	float y;
	if (!ExtractFloat(&args, y)) {
		sLog.outString(u8">> [PZX] xyz2=%s", args);
		return false;
	}

	float z;
	if (!ExtractFloat(&args, z)) {
		sLog.outString(u8">> [PZX] xyz3=%s", args);
		return false;
	}

	uint32 mapid;
	if (!ExtractOptUInt32(&args, mapid, player->GetMapId())) {
		sLog.outString(u8">> [PZX] xyz4=%s", args);
		return false;
	}
	return player->TeleportTo(mapid, x, y, z, 0.0f);
}