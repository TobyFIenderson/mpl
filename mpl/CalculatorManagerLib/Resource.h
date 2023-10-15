#pragma once
#ifndef __INCLUDE_RESOURCE_H__
#define __INCLUDE_RESOURCE_H__

#include "CalculatorManagerLibNS.h"
#include <unordered_map>
#include "IResourceProvider.h"
#include "Command.h"
#include "CalculatorError.h"

CM_BEGIN

class Resource
{
public:
	static std::string_view GetCommandStringIDFromCommand(Command cmd);
	static std::string_view GetErrorStringIDFromErrorCode(ErrorCode ret_code);
	static void LoadRresource(IResourceProvider* res_provider);
	static std::string GetString(std::string_view str_id);
	static std::string GetStringFromCommand(Command cmd);
	static std::string GetStringFromErrorCode(ErrorCode ret_code);
	static bool AlreadyLoadedRresource();
	static void AlreadyLoadedRresource(bool flag);
private:
	static std::unordered_map<Command, std::string_view> CMD_STR_ID;
	static std::unordered_map<ErrorCode, std::string_view> ERR_STR_ID;
	static std::unordered_map<std::string_view, std::string> ENGINE_RES_STR;
	static bool is_inited;
};

CM_END

#endif // !__INCLUDE_RESOURCE_H__