// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetActor/ATargetActor_BlueprintBase.h"

#include "Abilities/GameplayAbility.h"
#include "Net/UnrealNetwork.h"


ATargetActor_BlueprintBase::ATargetActor_BlueprintBase()
{
	
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates=true;
	ShouldProduceTargetDataOnServer=true;

	
	
}

bool ATargetActor_BlueprintBase::IsConfirmTargetingAllowed()
{
	return Super::IsConfirmTargetingAllowed()&&BP_IsConfirmTargetingAllowed();
	
}

void ATargetActor_BlueprintBase::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	BP_StartTargeting(Ability);
	
	
}


void ATargetActor_BlueprintBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bSetLoactionUnderCursorHit)
	{
		SetActorLocationUndercursorHit();
	}

	if (bDebug&&SourceActor)
	{
		DrawDebugSphere(GetWorld(),GetActorLocation(),100,16,FColor::Cyan);
		DrawDebugLine(GetWorld(),SourceActor->GetActorLocation(),GetActorLocation(),FColor::Green);
	}

	
}


void ATargetActor_BlueprintBase::ConfirmTargetingAndContinue()
{

	
	BP_ConfirmTargetingAndContinue();

	Super::ConfirmTargetingAndContinue();
	
}

void ATargetActor_BlueprintBase::CancelTargeting()
{
	BP_CancelTargetinge();
	Super::CancelTargeting();
}

void ATargetActor_BlueprintBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&  OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass,bSetLoactionUnderCursorHit);
	

	
}

void ATargetActor_BlueprintBase::TargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	check(ShouldProduceTargetData());
	if (IsConfirmTargetingAllowed())
	{
		TargetDataReadyDelegate.Broadcast(TargetDataHandle);
	}

}

void ATargetActor_BlueprintBase::SetActorLocationUndercursorHit()
{
	if (PrimaryPC)
	{
		FHitResult HitResult;
		PrimaryPC->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);

		if (HitResult.bBlockingHit)
		{
			SetActorLocation(HitResult.ImpactPoint);
		}
	}
	
}

