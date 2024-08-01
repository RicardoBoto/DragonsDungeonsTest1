// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "DiceLibrary.h"
#include "Engine/DataAsset.h"

#include "DDCharacter.generated.h"

/** Enumerator for all abilities */
UENUM(BlueprintType)
enum class EDDCharacterAbilities : uint8
{
    STR = 0		UMETA(DisplayName = "Strength"),
	INT = 1		UMETA(DisplayName = "Intelligence"),
	WIS = 2		UMETA(DisplayName = "Wisdom"),
	DEX = 3		UMETA(DisplayName = "Dexterity"),
	CON = 4     UMETA(DisplayName = "Constitution"),
	CHA = 5		UMETA(DisplayName = "Charisma")
};

// ======================== Data ========================

USTRUCT(BlueprintType)
struct FDDNameData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    /** Name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

public:

    /** Constructor default */
	FDDNameData();

};

USTRUCT(BlueprintType)
struct FDDCharacterAbilityMinTuple
{
    GENERATED_USTRUCT_BODY()

public:

    /** Ability to affect */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDDCharacterAbilities Ability;

	/** Minimum value to accept */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MinValue;

public:

    /** Constructor default */
	FDDCharacterAbilityMinTuple();

};

USTRUCT(BlueprintType)
struct FDDCharacterAbilityModifierTuple
{
    GENERATED_USTRUCT_BODY()

public:

    /** Ability to affect */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDDCharacterAbilities Ability;

	/** Value amount to modify */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Value;

public:

    /** Constructor default */
	FDDCharacterAbilityModifierTuple();

};

USTRUCT(BlueprintType)
struct FDDCharacterRace : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    /** Race name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Race;

	/** List of min values for abilities */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDDCharacterAbilityMinTuple> AbilityMinStats;

	/** List of ability modifiers */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDDCharacterAbilityModifierTuple> AbilityModifiers;

	/** Racial ability */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RaceAbility;

	/** List of allowed classes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDataTableRowHandle> ListAllowedClass;

public:

    /** Constructor default */
	FDDCharacterRace();

};


USTRUCT(BlueprintType)
struct FDDCharacterClass : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    /** Class name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ClassName;

    /** Hit dice */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDDDiceRoll HitDice;

	/** Prime ability */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDDCharacterAbilities PrimeAbility;

public:

    /** Constructor default */
	FDDCharacterClass();

};

USTRUCT(BlueprintType)
struct FCharacterSpecs 
{
    GENERATED_USTRUCT_BODY()

public:

	/** Character Name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CharName;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseStrength;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Strength;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseIntelligence;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Intelligence;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseWisdom;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Wisdom;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseDexterity;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Dexterity;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseConstitution;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Constitution;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseCharisma;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Charisma;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle CharRace;
	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle CharClass;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 TotalHitPoints;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Gold;

	/** Constructor */
	FCharacterSpecs();

};

USTRUCT(BlueprintType)
struct FDDCharactersTable : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    /**  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCharacterSpecs CharacterSpecs;

public:

    /** Constructor default */
	FDDCharactersTable();

};

UCLASS()
class CAPTUREAGETOOLTEST_API ADDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADDCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
