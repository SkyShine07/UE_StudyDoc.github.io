// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MediaTexture.h"
#include "MoviePlayer.h"
#include "Engine/DeveloperSettings.h"

#include "GameConfigGlobalSetting.generated.h"

/**
 * 
 */
UCLASS(config=Engine, DefaultConfig,meta=(DisplayName="GameConfigGlobalSetting"))
class  ABILITYSYSTEMEXTRATOOLKIT_API UGameConfigGlobalSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	UGameConfigGlobalSetting();


	/*
	 * **********  LoadScreen 设置属性   *************
	 */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	TSoftObjectPtr<UTexture> LoadScreenimage;
	
	/** The movie paths local to the game's Content/Movies/ directory we will play. */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	TArray<FString> MoviePaths;

	/** The minimum time that a loading screen should be opened for. */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	float MinimumLoadingScreenDisplayTime;

	/** If true, the loading screen will disappear as soon as all movies are played and loading is done. */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	bool bAutoCompleteWhenLoadingCompletes;

	/** If true, movies can be skipped by clicking the loading screen as  long as loading is done. */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	bool bMoviesAreSkippable;

	/** If true, movie playback continues until Stop is called. */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	bool bWaitForManualStop;

	/** If true loading screens here cannot have any uobjects of any kind or use any engine features at all. This will start the movies very early as a result on platforms that support it */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	bool bAllowInEarlyStartup;

	/** If true, this will call the engine tick while the game thread is stalled waiting for a loading movie to finish. This only works for post-startup load screens and is potentially unsafe */
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	bool bAllowEngineTick;

	/** Should we just play back, loop, etc.  NOTE: if the playback type is MT_LoopLast, then bAutoCompleteWhenLoadingCompletes will be togged on when the last movie is hit*/
	UPROPERTY(config, EditAnywhere, Category = LoadingScreen)
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	

	static  const UGameConfigGlobalSetting* Get();
	
};


