// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FVGUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLIGHTVIEWER_API UFVGUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "UI")
	FVector GetCurrentCoordinates() const;

};
