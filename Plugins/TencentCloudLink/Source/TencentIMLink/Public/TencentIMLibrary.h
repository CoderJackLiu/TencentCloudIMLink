// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "V2TIMBuffer.h"
#include "V2TIMConversationManager.h"
#include "V2TIMDefine.h"
#include "V2TIMManager.h"
#include "V2TIMMessage.h"
#include "V2TIMMessageManager.h"
#include "V2TIMGroupManager.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMSignaling.h"
#include "V2TIMSignalingManager.h"

#include "TencentIMLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UTencentIMLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*
	 * 初始化SDK
	 */
	//UFUNCTION(BlueprintCallable, Category = "SimpleOSS|InitOSS")
	//	static void InitSDK(const FString& InAccessKeyId, const FString& InAccessKeySecret, const FString& InEndpoint);

	/*
	 * 获取版本号
	 * 
	 */

	UFUNCTION(BlueprintPure, Category = "IMLink|Get")
	static FString GetSDKVersion();

	UFUNCTION(BlueprintPure, Category = "IMLink|Get")
	static int64 GetServerTime();

	/*
	 * logIn
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|InitOSS")
	static void LogIn(const FString& UserId);


	
	
};
