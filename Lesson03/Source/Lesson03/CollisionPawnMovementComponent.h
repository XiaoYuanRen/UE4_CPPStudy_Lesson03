// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CollisionPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class LESSON03_API UCollisionPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
	
};
