// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TcpSocketConnection.h"
#include "TcpNetActor.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnReceivedStringDelegate, FString, Message);

UCLASS()
class FLIGHTVIEWER_API ATcpNetActor : public ATcpSocketConnection
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void OnConnected(int32 ConnectionId);

	UFUNCTION()
		void OnDisconnected(int32 ConId);

	UFUNCTION()
		void OnMessageReceived(int32 ConId, TArray<uint8>& Message);

	UFUNCTION(BlueprintCallable)
		void ConnectToServer();

	UPROPERTY()
		int32 connectionIdServer;

	FOnReceivedStringDelegate OnReceivedStringDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
