// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "EditorToolCharacterStats.h"
#include "Blueprint/UserWidget.h"
#include "DDCharacter.h"

#include "CharacterEditorTool.generated.h"


USTRUCT(BlueprintType)
struct FDDCharacterCacheUI
{
    GENERATED_USTRUCT_BODY()

public:

    /** Row name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RowNameId;

    /** Character name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CharacterName;

public:

    /** Constructor default */
	FDDCharacterCacheUI();

	/** Constructor with args */
	FDDCharacterCacheUI(FName InRowNameId, FString InCharacterName);
};

/**
 * 
 */
UCLASS()
class CAPTUREAGETOOLTEST_API UCharacterEditorTool : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetupCharacterCache();

	UFUNCTION(BlueprintCallable)
	void SetSelectedCharacter(const int32 InCachedIndex);

	void SetSelectedCharacterByRowID(const FName InRowNameID);

	UFUNCTION(BlueprintCallable)
	void GenerateNewCharacter();

	UFUNCTION(BlueprintCallable)
	void DeleteCharacter();

	UFUNCTION(BlueprintCallable)
	void ExportCharacter();

	UFUNCTION(BlueprintCallable)
	void ImportCharacter();

	void ClearModifiers(FCharacterSpecs& CharSpecs);

	void CalculateModifiers(FCharacterSpecs& CharSpecs);

	void ModifyHability(FCharacterSpecs& CharSpecs, const FDDCharacterAbilityModifierTuple& AbiMod);

	void LowerClampHability(FCharacterSpecs& CharSpecs, const FDDCharacterAbilityMinTuple& AbiMinMod);

	int32 GetConstitutionHPModifier(const int32 InConsttutionVal) const;

private:

	FString GetExportFolder() const;

	void AddNewCharacterToDB(const FDDCharactersTable& InCharacterData);

	void SaveCharacterTable();

	FName GetNewUniqueNameID(FString CharStrName);

	void UserFeedbackMessage(FString InMessage, FString InTitle = FString());

	void SetUISelection(const bool InIsSelected);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* VerticalBoxAllCharacters;    
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	UEditorToolCharacterStats* CharacterStatsWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button_Delete;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button_Export;
	
	/** Cached list of all characters */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FDDCharacterCacheUI> AllCharactersCache;

	/** Data table for all characters */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* CharactersTable;

	/** Data table for all Races */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* RacesTable;

	/** Data table for all Races */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ClassTable;

	/** Data table for all default names */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* NamesTable;

	// ============ Selected Data ============
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CachedIndex = -1;

};
