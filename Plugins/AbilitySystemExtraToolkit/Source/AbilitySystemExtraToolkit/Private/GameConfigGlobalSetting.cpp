#include "GameConfigGlobalSetting.h"

UGameConfigGlobalSetting::UGameConfigGlobalSetting()
{
	CategoryName="Game";
	
}

const UGameConfigGlobalSetting* UGameConfigGlobalSetting::Get()
{
	return GetDefault<UGameConfigGlobalSetting>();
	
}
