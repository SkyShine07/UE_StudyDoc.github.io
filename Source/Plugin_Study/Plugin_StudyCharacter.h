// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ExampleAttributeSet.h"


#include "Plugin_StudyCharacter.generated.h"

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class APlugin_StudyCharacter : public ACharacter
{
	GENERATED_BODY()

private:


	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,meta=(AllowPrivateAccess=true))
	UExampleAttributeSet* ExampleAttributeSet;
	
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,meta=(AllowPrivateAccess=true))
	UAbilitySystemComponent* AbilitySystemComponent;


	//void AddAttributesByClass();



	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;




public:

	/** Constructor */
	APlugin_StudyCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the camera component **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns the Camera Boom component **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	virtual void PossessedBy(AController* NewController) override;
	virtual  void OnRep_PlayerState() override;
};

