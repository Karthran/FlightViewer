// Fill out your copyright notice in the Description page of Project Settings.


#include "TcpNetActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogTcpNetActor, All, All)

void ATcpNetActor::ConnectToServer() {
	if (isConnected(connectionIdServer))
	{
		UE_LOG(LogTcpNetActor, Log, TEXT("Log: Can't connect SECOND time. We're already connected!"));
		return;
	}
	FTcpSocketDisconnectDelegate disconnectDelegate;
	disconnectDelegate.BindDynamic(this, &ATcpNetActor::OnDisconnected);
	FTcpSocketConnectDelegate connectDelegate;
	connectDelegate.BindDynamic(this, &ATcpNetActor::OnConnected);
	FTcpSocketReceivedMessageDelegate receivedDelegate;
	receivedDelegate.BindDynamic(this, &ATcpNetActor::OnMessageReceived);
	Connect("127.0.0.1", 27777, disconnectDelegate, connectDelegate, receivedDelegate, connectionIdServer);
	//	UE_LOG(LogTcpNetActor, Warning, TEXT("Connected to server."))
}

void ATcpNetActor::BeginPlay()
{
	ConnectToServer();
}

void ATcpNetActor::OnConnected(int32 ConId) {
	UE_LOG(LogTcpNetActor, Warning, TEXT("Log: Connected to server."));
}

void ATcpNetActor::OnDisconnected(int32 ConId) {
	UE_LOG(LogTcpNetActor, Warning, TEXT("Log: OnDisconnected."));
}

void ATcpNetActor::OnMessageReceived(int32 ConId, TArray<uint8>& Message) {
	UE_LOG(LogTcpNetActor, Warning, TEXT("Log: Received message."));
	// In this example, we always encode messages a certain way:
	// The first 4 bytes contain the length of the rest of the message.
//	while (Message.Num() != 0) {
		// read expected length
	int32 msgLength = Message.Num();
	//TArray<uint8> yourMessage;
	// read the message itself
	FString recMessage = Message_ReadString(Message, msgLength);
	//if (!Message_ReadBytes(msgLength, Message, yourMessage)) {
	//	// If it couldn't read expected number of bytes, something went wrong.
	//	// Print a UE_LOG here, while your project is in development.
	//	continue;
	//}
	//UE_LOG(LogTcpNetActor, Warning, TEXT("Received %s Length %d"), *FString(recMessage), msgLength);

	OnReceivedStringDelegate.ExecuteIfBound(recMessage);
	// If the message was read, then treat "yourMessage" here!
	// ...
	// And then we go back to the "while", because we may have received multiple messages in a frame, 
	// so they all have to be read.
//	}
}