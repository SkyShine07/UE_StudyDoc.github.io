// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnhanceAS_WidgetBase.h"

#include "AbilitySystemExtraToolKitDebug.h"
#include "EnhanceAbilitySystemComponent.h"


void UEnhanceAS_WidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (GetOwningPlayerPawn())
	{
		UActorComponent* asc=GetOwningPlayerPawn()->GetComponentByClass(UEnhanceAbilitySystemComponent::StaticClass());

		if (asc)
		{
			EnhanceASComponent=Cast<UEnhanceAbilitySystemComponent>(asc);
			
			/*if (EnhanceASComponent)
			{
				FString STR="PawnisVaild";
				ASDebug::PrintText(STR);
			}*/
		}
	}
			
};


