#pragma once
#ifndef __INCLUDE_IRESOURCE_PROVIDER_H__
#define __INCLUDE_IRESOURCE_PROVIDER_H__

#include "CalculatorManagerLibNS.h"
#include <string>
CM_BEGIN
class IResourceProvider
{
public:
	virtual ~IResourceProvider() {}
	virtual std::string GetResourceString(std::string_view string_id) = 0;
};
CM_END

#endif // !__INCLUDE_IRESOURCE_PROVIDER_H__