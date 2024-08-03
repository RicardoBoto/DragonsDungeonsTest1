// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterEditorTool.h"

#include "Engine/DataTable.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "DiceLibrary.h"
#include "EditorToolCharacterStats.h"
#include "DataTableUtils.h"
#include "JsonObjectConverter.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/MessageDialog.h"

#include "UObject/Package.h"
#include "UObject/SavePackage.h"

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

void UCharacterEditorTool::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// CachedIndex = INDEX_NONE;

	// SetUISelection(false);
}

void UCharacterEditorTool::NativeConstruct()
{
	Super::NativeConstruct();

	CachedIndex = INDEX_NONE;
	
	SetUISelection(false);
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
	if (CharacterStatsWidget == nullptr || CharactersTable == nullptr || !AllCharactersCache.IsValidIndex(InCachedIndex) || VerticalBoxAllCharacters == nullptr)
	{
		return;
	}

	FDDCharacterCacheUI& CurrSelCache = AllCharactersCache[InCachedIndex];

	if (FDDCharactersTable* CharRowData = CharactersTable->FindRow<FDDCharactersTable>(CurrSelCache.RowNameId, TEXT("UCharacterEditorTool::SetSelectedCharacter() find row")))
	{
		if (UWidget* UserEntryWidPre = VerticalBoxAllCharacters->GetChildAt(CachedIndex))
		{
			UserEntryWidPre->SetIsEnabled(true);
		}

		CachedIndex = InCachedIndex;

		if (UWidget* UserEntryWidPos = VerticalBoxAllCharacters->GetChildAt(CachedIndex))
		{
			UserEntryWidPos->SetIsEnabled(false);
		}

		CalculateModifiers(CharRowData->CharacterSpecs);
		CharacterStatsWidget->SetUpCharacterSpec(CharRowData->CharacterSpecs);

		SetUISelection(true);
	}
}

void UCharacterEditorTool::SetSelectedCharacterByRowID(const FName InRowNameID)
{
	for (int32 i = 0; i < AllCharactersCache.Num(); i++)
	{
		if (AllCharactersCache[i].RowNameId == InRowNameID)
		{
			SetSelectedCharacter(i);
			return;
		}
	}
}

void UCharacterEditorTool::GenerateNewCharacter()
{
	if (CharactersTable == nullptr || RacesTable == nullptr || ClassTable == nullptr || NamesTable == nullptr)
	{
		return;
	}

	FDDCharactersTable NewCharRow;

	TArray<FName> TableNameIds = NamesTable->GetRowNames();
	const int32 RandNameIndex = FMath::RandRange(0, TableNameIds.Num() - 1);

	FDDNameData* NameData = NamesTable->FindRow<FDDNameData>(TableNameIds[RandNameIndex], TEXT("UCharacterEditorTool::GenerateNewCharacter() find name row"));
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

	const FDDCharacterRace* const CharRace = CharRaceRowHandler.GetRow<FDDCharacterRace>(TEXT("UEditorToolCharacterStats::GenerateNewCharacter() get row for race"));
	if (CharRace != nullptr)
	{
		CharRace->ListAllowedClass.Num();
		int32 RandClassAllowedIndex = FMath::RandRange(0, CharRace->ListAllowedClass.Num() - 1);
		NewCharRow.CharacterSpecs.CharClass = CharRace->ListAllowedClass[RandClassAllowedIndex];
	}

	CalculateModifiers(NewCharRow.CharacterSpecs);

	const FDDCharacterClass* const CharClass = NewCharRow.CharacterSpecs.CharClass.GetRow<FDDCharacterClass>(TEXT("UEditorToolCharacterStats::GenerateNewCharacter() get row for class"));
	if(CharClass != nullptr)
	{
		const int32 FinalHP = UDiceLibrary::RollDice(CharClass->HitDice.NrRolls, CharClass->HitDice.NrDice) + GetConstitutionHPModifier(NewCharRow.CharacterSpecs.Constitution);
		NewCharRow.CharacterSpecs.TotalHitPoints = FinalHP;
	}

	NewCharRow.CharacterSpecs.Gold = UDiceLibrary::RollDice(3, 6) * 10;

	AddNewCharacterToDB(NewCharRow);
}

void UCharacterEditorTool::DeleteCharacter()
{
	if (CharactersTable == nullptr || !AllCharactersCache.IsValidIndex(CachedIndex))
	{
		return;
	}

	CharactersTable->RemoveRow(AllCharactersCache[CachedIndex].RowNameId);
	SaveCharacterTable();

	CachedIndex = INDEX_NONE;
	SetUISelection(false);

	SetupCharacterCache();
}

void UCharacterEditorTool::ExportCharacter()
{
	if (!AllCharactersCache.IsValidIndex(CachedIndex))
	{
		return;
	}

	FDDCharacterCacheUI& CharChache = AllCharactersCache[CachedIndex];
	FDDCharactersTable* CharRow = CharactersTable->FindRow<FDDCharactersTable>(CharChache.RowNameId, TEXT("UCharacterEditorTool::ExportCharacter() find row"));

	if (CharRow == nullptr)
	{
		return;
	}

	FString CharRowJson;
	FJsonObjectConverter::UStructToJsonObjectString(FDDCharactersTable::StaticStruct(), CharRow, CharRowJson);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString FilePath = GetExportFolder();
	FString FileName = FString::Printf(TEXT("%s.json"), *CharChache.RowNameId.ToString());
	bool Success = FFileHelper::SaveStringToFile(CharRowJson, *(FilePath + FileName));

	if (Success)
	{
		UserFeedbackMessage(FString(TEXT("Export successful")));
	}
	else
	{
		UserFeedbackMessage(FString(TEXT("Can't write file")), FString(TEXT("Error")));
	}
}


void UCharacterEditorTool::ImportCharacter()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform == nullptr)
	{
		return;
	}

	TArray<FString> OutFilenames;
	const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	DesktopPlatform->OpenFileDialog(ParentWindowWindowHandle, FString(TEXT("Select character file")), GetExportFolder(), FString(), FString(TEXT(".json")), 0, OutFilenames);

	if (OutFilenames.Num() == 1)
	{
		FString FilePathName = GetExportFolder() + OutFilenames[0];
		FString CharRowStructJSON;

		FFileHelper::LoadFileToString(CharRowStructJSON, *FilePathName);

		FDDCharactersTable NewCharRow;
		bool Success = FJsonObjectConverter::JsonObjectStringToUStruct<FDDCharactersTable>(CharRowStructJSON, &NewCharRow, 0, 0);

		if (Success)
		{
			AddNewCharacterToDB(NewCharRow);
			UserFeedbackMessage(FString(TEXT("Import successful")));
		}
		else
		{
			UserFeedbackMessage(FString(TEXT("JSON file format corrupted")), FString(TEXT("Error")));
		}
	}
	else
	{
		UserFeedbackMessage(FString(TEXT("Please select only one file")), FString(TEXT("Error")));
	}
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


FString UCharacterEditorTool::GetExportFolder() const
{
	return FString::Printf(TEXT("%sExports/"), *FPaths::ProjectContentDir());
}

void UCharacterEditorTool::AddNewCharacterToDB(const FDDCharactersTable& InCharacterData)
{
	if (CharactersTable == nullptr)
	{
		return;
	}

	FName NewRowName = GetNewUniqueNameID(InCharacterData.CharacterSpecs.CharName);
	CharactersTable->AddRow(NewRowName, InCharacterData);

	SaveCharacterTable();
	SetupCharacterCache();
	SetSelectedCharacterByRowID(NewRowName);
}


void UCharacterEditorTool::SaveCharacterTable()
{
	if (CharactersTable == nullptr)
	{
		return;
	}

	UPackage* TablePackage = CharactersTable->GetPackage();

	if (TablePackage == nullptr)
	{
		return;
	}

	const FString TableAssetFileName = FPackageName::LongPackageNameToFilename(TablePackage->GetPathName(), FPackageName::GetAssetPackageExtension());

	if (!FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*TableAssetFileName))
	{
		FSavePackageArgs SavePackageArgs;
		UPackage::SavePackage(TablePackage, CharactersTable, *TableAssetFileName, SavePackageArgs);
	}
}

FName UCharacterEditorTool::GetNewUniqueNameID(FString CharStrName)
{
	TArray<FName> RowNames = CharactersTable->GetRowNames();
	FName CharName = FName(CharStrName);

	if (!RowNames.Contains(CharName))
	{
		return CharName;
	}

	bool NameFound = false;
	int32 Counter = 0;
	
	while (!NameFound && Counter < 100)
	{
		FName TestName = FName(FString::Printf(TEXT("%s_%d"), *CharStrName, Counter));

		if (RowNames.Contains(TestName))
		{
			Counter++;
			break;
		}
		else
		{
			return TestName;
		}
	}

	return FName();
}

void UCharacterEditorTool::UserFeedbackMessage(FString InMessage, FString InTitle)
{
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(InMessage), FText::FromString(InTitle));
}

void UCharacterEditorTool::SetUISelection(const bool InIsSelected)
{

	if (CharacterStatsWidget != nullptr)
	{
		CharacterStatsWidget->SetVisibility(InIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}

	if (Button_Export != nullptr)
	{
		Button_Export->SetIsEnabled(InIsSelected);
	}

	if (Button_Delete != nullptr)
	{
		Button_Delete->SetIsEnabled(InIsSelected);
	}
}
