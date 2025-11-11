#pragma once

#include "Core.h"

#define UE_STRING_PRINTF_FMT_CHARTYPE          TCHAR
using FmtCharType   = UE_STRING_PRINTF_FMT_CHARTYPE;


namespace ASDebug
{

	template <typename... Types>
	static void  PrintText(UE::Core::TCheckedFormatString<FmtCharType, Types...> Fmt, Types... Args)
	{
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::MakeRandomColor(),FString::Printf(Fmt,Args...));
	};


	
	static void  PrintText( FString&  Str)
	{
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::MakeRandomColor(),Str);
	};


	
}
