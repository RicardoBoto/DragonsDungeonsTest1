// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDCharacter.h"
#include "EditorToolCharacterStats.generated.h"

/**
 * 
 */
UCLASS()
class CAPTUREAGETOOLTEST_API UEditorToolCharacterStats : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetUpCharacterSpec(FCharacterSpecs& InCharSpecs);

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DisplayNameTitle;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharRaceText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharClassText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharClassPrimeStatText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharStrBase;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharStrFinal;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharIntBase;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharIntFinal;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharWisBase;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharWisFinal;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharDexBase;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharDexFinal;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharConBase;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharConFinal;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharChaBase;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharChaFinal;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharHPText;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* CharGoldText;

};
