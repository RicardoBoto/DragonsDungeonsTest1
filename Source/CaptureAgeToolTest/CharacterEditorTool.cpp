// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEditorTool.h"

#include "Engine/DataTable.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "DiceLibrary.h"
#include "EditorToolCharacterStats.h"
#include "DataTableUtils.h"

FDDCharacterCacheUI::FDDCharacterCacheUI()
	: RowNameId(FName())
	, CharacterName(FString())
{
}

FDDCharacterCacheUI::FDDCharacterCacheUI(FName InRowNameId, FString InCharacterName)
	: RowNameId(InRowNameId)
	, CharacterName(InCharacterName)
{
}

void UCharacterEditorTool::NativeConstruct()
{
	Super::NativeConstruct();

	CachedIndex = INDEX_NONE;

	if (CharacterStatsWidget != nullptr)
	{
		CharacterStatsWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Button_Delete != nullptr)
	{
		Button_Delete->SetIsEnabled(false);
	}

	if (Button_Export != nullptr)
	{
		Button_Export->SetIsEnabled(false);
	}
}

void UCharacterEditorTool::SetupCharacterCache()
{
	if (CharactersTable != nullptr && VerticalBoxAllCharacters != nullptr)
	{
		TArray<FName> TableNameIds = CharactersTable->GetRowNames();
		
		AllCharactersCache.Empty();
		AllCharactersCache.Reserve(TableNameIds.Num());

		for (const FName& CurrRowId : TableNameIds)
		{
			FDDCharactersTable* CharRowData = CharactersTable->FindRow<FDDCharactersTable>(CurrRowId, TEXT("UCharacterEditorTool::SetupCharacterCache() find row"));
			
			if (CharRowData != nullptr)
			{
				FDDCharacterCacheUI NewCacheEntry = FDDCharacterCacheUI(CurrRowId, CharRowData->CharacterSpecs.CharName);
				AllCharactersCache.Emplace(NewCacheEntry);
			}
		}
	}
}

void UCharacterEditorTool::SetSelectedCharacter(const int32 InCachedIndex)
{
	if (CharacterStatsWidget == nullptr || CharactersTable == nullptr || !AllCharactersCache.IsValidIndex(InCachedIndex))
	{
		return;
	}

	FDDCharacterCacheUI& CurrSelCache = AllCharactersCache[InCachedIndex];

	if (FDDCharactersTable* CharRowData = CharactersTable->FindRow<FDDCharactersTable>(CurrSelCache.RowNameId, TEXT("UCharacterEditorTool::SetSelectedCharacter() find row")))
	{
		CachedIndex = InCachedIndex;

		CalculateModifiers(CharRowData->CharacterSpecs);

		CharacterStatsWidget->SetUpCharacterSpec(CharRowData->CharacterSpecs);
		CharacterStatsWidget->SetVisibility(ESlateVisibility::Visible);

		if (Button_Delete != nullptr)
		{
			Button_Delete->SetIsEnabled(true);
		}
	}

}

void UCharacterEditorTool::GenerateNewCharacter()
{
	if (CharactersTable == nullptr && RacesTable == nullptr && ClassTable == nullptr && NamesTable == nullptr)
	{
		return;
	}

	FDDCharactersTable NewCharRow;

	TArray<FName> TableNameIds = NamesTable->GetRowNames();
	int32 RandNameIndex = FMath::RandRange(0, TableNameIds.Num() - 1);

	FDDNameData* NameData = NamesTable->FindRow<FDDNameData>(TableNameIds[RandNameIndex], TEXT("UCharacterEditorTool::SetupCharacterCache() find row"));
	if (NameData != nullptr)
	{
		NewCharRow.CharacterSpecs.CharName = NameData->Name;
	}

	NewCharRow.CharacterSpecs.BaseStrength = UDiceLibrary::RollDice(3, 6);
	NewCharRow.CharacterSpecs.BaseIntelligence = UDiceLibrary::RollDice(3, 6);
	NewCharRow.CharacterSpecs.BaseWisdom = UDiceLibrary::RollDice(3, 6);
	NewCharRow.CharacterSpecs.BaseDexterity = UDiceLibrary::RollDice(3, 6);
	NewCharRow.CharacterSpecs.BaseConstitution = UDiceLibrary::RollDice(3, 6);
	NewCharRow.CharacterSpecs.BaseCharisma = UDiceLibrary::RollDice(3, 6);


	TArray<FName> TableRaceIds = RacesTable->GetRowNames();
	int32 RandRaceIndex = FMath::RandRange(0, TableRaceIds.Num() - 1);

	FDataTableRowHandle CharRaceRowHandler = FDataTableRowHandle();
	CharRaceRowHandler.DataTable = RacesTable;
	CharRaceRowHandler.RowName = TableRaceIds[RandRaceIndex];
	NewCharRow.CharacterSpecs.CharRace = CharRaceRowHandler;

	FDDCharacterRace* CharRace = CharRaceRowHandler.GetRow<FDDCharacterRace>(TEXT("UEditorToolCharacterStats::SetUpCharacterSpec get row for race"));
	if (CharRace != nullptr)
	{
		CharRace->ListAllowedClass.Num();
		int32 RandClassAllowedIndex = FMath::RandRange(0, CharRace->ListAllowedClass.Num() - 1);
		NewCharRow.CharacterSpecs.CharClass = CharRace->ListAllowedClass[RandClassAllowedIndex];
	}

	CalculateModifiers(NewCharRow.CharacterSpecs);

	FDDCharacterClass* CharClass = NewCharRow.CharacterSpecs.CharClass.GetRow<FDDCharacterClass>(TEXT("UEditorToolCharacterStats::SetUpCharacterSpec get row for class"));
	if(CharClass != nullptr)
	{
		const int32 FinalHP = UDiceLibrary::RollDice(CharClass->HitDice.NrRolls, CharClass->HitDice.NrDice) + GetConstitutionHPModifier(NewCharRow.CharacterSpecs.Constitution);
		NewCharRow.CharacterSpecs.TotalHitPoints = FinalHP;
	}

	NewCharRow.CharacterSpecs.Gold = UDiceLibrary::RollDice(3, 6) * 10;

	int32 lastNrCreated = FCString::Atoi(*AllCharactersCache.Last().RowNameId.ToString());
	FName NewRowName = DataTableUtils::MakeValidName(FString::Printf(TEXT("%d"), lastNrCreated + 1));

	CharactersTable->AddRow(NewRowName, NewCharRow);
	CharactersTable->MarkPackageDirty();

	SetupCharacterCache();
}

void UCharacterEditorTool::DeleteCharacter()
{
	if (AllCharactersCache.IsValidIndex(CachedIndex))
	{
		CharactersTable->RemoveRow(AllCharactersCache[CachedIndex].RowNameId);
		CharactersTable->MarkPackageDirty();
	}

	CachedIndex = INDEX_NONE;
	if (CharacterStatsWidget != nullptr)
	{
		CharacterStatsWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (Button_Delete != nullptr)
	{
		Button_Delete->SetIsEnabled(false);
	}

	SetupCharacterCache();
}

void UCharacterEditorTool::ClearModifiers(FCharacterSpecs& CharSpecs)
{
	CharSpecs.Strength = CharSpecs.BaseStrength;
	CharSpecs.Intelligence = CharSpecs.BaseIntelligence;
	CharSpecs.Wisdom = CharSpecs.BaseWisdom;
	CharSpecs.Dexterity = CharSpecs.BaseDexterity;
	CharSpecs.Constitution = CharSpecs.BaseConstitution;
	CharSpecs.Charisma = CharSpecs.BaseCharisma;
}

void UCharacterEditorTool::CalculateModifiers(FCharacterSpecs& CharSpecs)
{
	FDDCharacterRace* CharRace = CharSpecs.CharRace.GetRow<FDDCharacterRace>(TEXT("UEditorToolCharacterStats::CalculateModifiers get row for race"));

	if (CharRace != nullptr)
	{
		ClearModifiers(CharSpecs);

		for (FDDCharacterAbilityModifierTuple& AbiMod : CharRace->AbilityModifiers)
		{
			ModifyHability(CharSpecs, AbiMod);
		}

		for (FDDCharacterAbilityMinTuple& AbiMinMod : CharRace->AbilityMinStats)
		{
			LowerClampHability(CharSpecs, AbiMinMod);
		}
	}
}

void UCharacterEditorTool::ModifyHability(FCharacterSpecs& CharSpecs, const FDDCharacterAbilityModifierTuple& AbiMod)
{
	switch (AbiMod.Ability)
	{
		case EDDCharacterAbilities::STR :
			CharSpecs.Strength += AbiMod.Value;
			return;

		case EDDCharacterAbilities::INT :
			CharSpecs.Intelligence += AbiMod.Value;
			return;

		case EDDCharacterAbilities::WIS :
			CharSpecs.Wisdom += AbiMod.Value;
			return;

		case EDDCharacterAbilities::DEX :
			CharSpecs.Dexterity += AbiMod.Value;
			return;

		case EDDCharacterAbilities::CON:
			CharSpecs.Constitution += AbiMod.Value;
			return;

		case EDDCharacterAbilities::CHA:
			CharSpecs.Charisma += AbiMod.Value;
			return;

		default:
			return;
	}
}

void UCharacterEditorTool::LowerClampHability(FCharacterSpecs& CharSpecs, const FDDCharacterAbilityMinTuple& AbiMinMod)
{
	switch (AbiMinMod.Ability)
	{
		case EDDCharacterAbilities::STR :
			if (CharSpecs.Strength < AbiMinMod.MinValue)
			{
				CharSpecs.Strength = AbiMinMod.MinValue;
			}
			return;

		case EDDCharacterAbilities::INT :
			if (CharSpecs.Intelligence < AbiMinMod.MinValue)
			{
				CharSpecs.Intelligence = AbiMinMod.MinValue;
			}
			return;

		case EDDCharacterAbilities::WIS :
			if (CharSpecs.Wisdom < AbiMinMod.MinValue)
			{
				CharSpecs.Wisdom = AbiMinMod.MinValue;
			}
			return;

		case EDDCharacterAbilities::DEX :
			if (CharSpecs.Dexterity < AbiMinMod.MinValue)
			{
				CharSpecs.Dexterity = AbiMinMod.MinValue;
			}
			return;

		case EDDCharacterAbilities::CON:
			if (CharSpecs.Constitution < AbiMinMod.MinValue)
			{
				CharSpecs.Constitution = AbiMinMod.MinValue;
			}
			return;

		case EDDCharacterAbilities::CHA:
			if (CharSpecs.Charisma < AbiMinMod.MinValue)
			{
				CharSpecs.Charisma = AbiMinMod.MinValue;
			}
			return;

		default:
			return;
	}
}

int32 UCharacterEditorTool::GetConstitutionHPModifier(const int32 InConsttutionVal) const
{
	if (InConsttutionVal >= 18)
	{
		return 3;
	}
	else if (InConsttutionVal >= 16)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

