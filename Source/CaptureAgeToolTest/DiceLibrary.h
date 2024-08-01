// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DiceLibrary.generated.h"

// ======================== Data ========================

USTRUCT(BlueprintType)
struct FDDDiceRoll
{
	GENERATED_USTRUCT_BODY()

public:

	/** Number of rolls */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NrRolls;

	/** Dice number */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NrDice;

public:

	/** Constructor default */
	FDDDiceRoll();

};

/**
 * 
 */
UCLASS()
class CAPTUREAGETOOLTEST_API UDiceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** Roll a combo */
	UFUNCTION(BlueprintCallable)
	static int32 RollDice(const int32 NrRolls, const int32 NrDice);

};
