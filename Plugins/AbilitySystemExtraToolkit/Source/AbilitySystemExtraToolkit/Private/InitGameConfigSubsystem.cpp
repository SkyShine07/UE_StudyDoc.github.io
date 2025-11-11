// Fill out your copyright notice in the Description page of Project Settings.


#include "InitGameConfigSubsystem.h"

#include "AbilitySystemExtraToolKitDebug.h"
#include "AbilitySystemGlobals.h"
#include "GameConfigGlobalSetting.h"
#include "MoviePlayer.h"
#include "Slate/LoadScreen.h"


void UInitGameConfigSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 初始化TargetData数据

	UAbilitySystemGlobals::Get().InitGlobalData();


	//设置加载地图时的载入界面

	//FCoreDelegates::OnInit
	FCoreUObjectDelegates::PreLoadMap.AddLambda([](const FString&)
	{
		FLoadingScreenAttributes LoadingScreenAttributes;
		const UGameConfigGlobalSetting* GameConfigGlobalSetting=NewObject<UGameConfigGlobalSetting>() ;
		
		LoadingScreenAttributes.WidgetLoadingScreen=FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		LoadingScreenAttributes.MoviePaths=GameConfigGlobalSetting->MoviePaths;
		LoadingScreenAttributes.bAllowEngineTick=GameConfigGlobalSetting->bAllowEngineTick;
		LoadingScreenAttributes.bMoviesAreSkippable=GameConfigGlobalSetting->bMoviesAreSkippable;
		LoadingScreenAttributes.bWaitForManualStop=GameConfigGlobalSetting->bWaitForManualStop;
		LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes=GameConfigGlobalSetting->bAutoCompleteWhenLoadingCompletes;
		LoadingScreenAttributes.MinimumLoadingScreenDisplayTime=GameConfigGlobalSetting->MinimumLoadingScreenDisplayTime;
		LoadingScreenAttributes.PlaybackType=GameConfigGlobalSetting->PlaybackType;
		LoadingScreenAttributes.bAllowInEarlyStartup=GameConfigGlobalSetting->bAllowInEarlyStartup;
		
		if (GetMoviePlayer())
		{
			GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
			
			
		}
		
	});

	
}
