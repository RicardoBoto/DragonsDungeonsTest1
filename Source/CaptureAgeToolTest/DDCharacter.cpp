// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCharacter.h"

// ======================== Data ========================

FDDNameData::FDDNameData()
	: Name(FString())
{
}

FDDCharacterAbilityMinTuple::FDDCharacterAbilityMinTuple()
	: Ability(EDDCharacterAbilities::STR)
	, MinValue(5)
{
}

FDDCharacterAbilityModifierTuple::FDDCharacterAbilityModifierTuple()
	: Ability(EDDCharacterAbilities::STR)
	, Value(0)
{
}

FDDCharacterRace::FDDCharacterRace()
	: Race(FString())
	, AbilityMinStats(TArray<FDDCharacterAbilityMinTuple>())
	, AbilityModifiers(TArray<FDDCharacterAbilityModifierTuple>())
	, RaceAbility(FString())
	, ListAllowedClass(TArray<FDataTableRowHandle>())
{
}

FDDCharacterClass::FDDCharacterClass()
	: ClassName(FString())
	, HitDice(FDDDiceRoll())
	, PrimeAbility(EDDCharacterAbilities())
{
}

FCharacterSpecs::FCharacterSpecs()
	: CharName(FString())
	, BaseStrength(10)
	, Strength(10)
	, BaseIntelligence(10)
	, Intelligence(10)
	, BaseWisdom(10)
	, Wisdom(10)
	, BaseDexterity(10)
	, Dexterity(10)
	, BaseConstitution(10)
	, Constitution(10)
	, BaseCharisma(10)
	, Charisma(10)
	, CharRace(FDataTableRowHandle())
	, CharClass(FDataTableRowHandle())
	, TotalHitPoints(0)
	, Gold(0)
{
}

FDDCharactersTable::FDDCharactersTable()
{
}

// ======================== Character ========================

// Sets default values
ADDCharacter::ADDCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADDCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

