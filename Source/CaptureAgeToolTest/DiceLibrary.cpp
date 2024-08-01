// Fill out your copyright notice in the Description page of Project Settings.


#include "DiceLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// ======================== Data ========================

FDDDiceRoll::FDDDiceRoll()
	: NrRolls(1)
	, NrDice(6)
{
}

// ======================== Library ========================

int32 UDiceLibrary::RollDice(const int32 NrRolls, const int32 NrDice)
{
	int32 result = 0;

	for (int32 i = 0; i < NrRolls; i++)
	{
		result += FMath::RandRange(1, NrDice);
	}

	return result;
}

