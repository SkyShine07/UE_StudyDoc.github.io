// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhanceAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "EnhanceAS_WidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UEnhanceAS_WidgetBase : public UUserWidget
{
	GENERATED_BODY()
	

	
protected:
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UEnhanceAbilitySystemComponent* EnhanceASComponent;
	
	
	virtual void NativeOnInitialized() override;
	
	
	
	
};
