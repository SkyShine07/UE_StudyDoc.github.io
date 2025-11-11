// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InitGameConfigSubsystem.generated.h"

/**
 * 
 */


UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UInitGameConfigSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
