// Fill out your copyright notice in the Description page of Project Settings.


#include "FVGUIWidget.h"
#include "FVCharacter.h"

FVector UFVGUIWidget::GetCurrentCoordinates() const
{
	const auto Character = Cast<AFVCharacter>(GetOwningPlayerPawn());
	if (!Character)
		return FVector();
	
	return Character->GetCurrentCoordinates();
}
