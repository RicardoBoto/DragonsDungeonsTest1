// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorToolCharacterStats.h"
#include "Components/TextBlock.h"

void UEditorToolCharacterStats::SetUpCharacterSpec(FCharacterSpecs& InCharSpecs)
{
	FDDCharacterRace* CharRace = InCharSpecs.CharRace.GetRow<FDDCharacterRace>(TEXT("UEditorToolCharacterStats::SetUpCharacterSpec get row for race"));
	FDDCharacterClass* CharClass = InCharSpecs.CharClass.GetRow<FDDCharacterClass>(TEXT("UEditorToolCharacterStats::SetUpCharacterSpec get row for class"));

	if (DisplayNameTitle == nullptr
		|| CharRaceText == nullptr
		|| CharClassText == nullptr
		|| CharClassPrimeStatText == nullptr
		|| CharStrFinal == nullptr
		|| CharIntBase == nullptr
		|| CharIntFinal == nullptr
		|| CharWisBase == nullptr
		|| CharWisFinal == nullptr
		|| CharDexBase == nullptr
		|| CharDexFinal == nullptr
		|| CharConBase == nullptr
		|| CharConFinal == nullptr
		|| CharChaBase == nullptr
		|| CharChaFinal == nullptr
		|| CharHPText == nullptr
		|| CharGoldText == nullptr
		|| CharRace == nullptr
		|| CharClass == nullptr)
	{
		return;
	}

	DisplayNameTitle->SetText(FText::FromString(InCharSpecs.CharName));

	CharRaceText->SetText(FText::FromString(CharRace->Race));

	CharClassText->SetText(FText::FromString(CharClass->ClassName));

	FText EnumDisplayName;
	UEnum::GetDisplayValueAsText(CharClass->PrimeAbility, EnumDisplayName);
	CharClassPrimeStatText->SetText(EnumDisplayName);

	CharStrBase->SetText(FText::FromString(FString::FromInt(InCharSpecs.BaseStrength)));
	CharStrFinal->SetText(FText::FromString(FString::FromInt(InCharSpecs.Strength)));
	CharIntBase->SetText(FText::FromString(FString::FromInt(InCharSpecs.BaseIntelligence)));
	CharIntFinal->SetText(FText::FromString(FString::FromInt(InCharSpecs.Intelligence)));
	CharWisBase->SetText(FText::FromString(FString::FromInt(InCharSpecs.BaseWisdom)));
	CharWisFinal->SetText(FText::FromString(FString::FromInt(InCharSpecs.Wisdom)));
	CharDexBase->SetText(FText::FromString(FString::FromInt(InCharSpecs.BaseDexterity)));
	CharDexFinal->SetText(FText::FromString(FString::FromInt(InCharSpecs.Dexterity)));
	CharConBase->SetText(FText::FromString(FString::FromInt(InCharSpecs.BaseConstitution)));
	CharConFinal->SetText(FText::FromString(FString::FromInt(InCharSpecs.Constitution)));
	CharChaBase->SetText(FText::FromString(FString::FromInt(InCharSpecs.BaseCharisma)));
	CharChaFinal->SetText(FText::FromString(FString::FromInt(InCharSpecs.Charisma)));

	CharHPText->SetText(FText::FromString(FString::FromInt(InCharSpecs.TotalHitPoints)));
	CharGoldText->SetText(FText::FromString(FString::FromInt(InCharSpecs.Gold)));

}
