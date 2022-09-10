// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "TencentDataType.h"

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

#include "TencentIMMacros.h"

#include "TencentIMLibrary.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FIMNormal_SuccessDelegate);
DECLARE_DELEGATE(FIMNormal_FaliureDelegate);


class NormalCallback : public V2TIMCallback
{
public:
	NormalCallback()
	{
	};

	~NormalCallback()
	{
	};

	FIMNormal_SuccessDelegate Normal_SuccessDelegate;
	FIMNormal_FaliureDelegate Normal_FaliureDelegate;

	void OnSuccess() override
	{
		Normal_SuccessDelegate.ExecuteIfBound();
		UE_LOG(LogTemp, Log, TEXT("<== login OnSuccess"));
	};

	void OnError(int error_code, const V2TIMString& error_message) override
	{
		Normal_FaliureDelegate.ExecuteIfBound();
		UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
	};
};

UCLASS()
class TENCENTIMLINK_API UTencentIMLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TencentIMLink|Init")
	static ELoginStatus GetLoginStatus();

	UFUNCTION(BlueprintPure, Category = "TencentIMLink|Init")
	static FString GetSDKVersion();

	UFUNCTION(BlueprintPure, Category = "TencentIMLink|Init")
	static int64 GetServerTime();

	/**
	 *
	*/
	// UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Init")
	// static void InitAccounts(const FString& InUserId, const FString& InIMUserSigId);


	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Log")
	static void LogIn(const FString& InUserId, const FString& InIMUserSigId);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Log")
	static void LogOut(FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);

	UFUNCTION(BlueprintPure, Category = "TencentIMLink|Init")
	static FString GetLoginUser();

	/*
	 * 3.1 设置基本消息（文本消息和自定义消息）的事件监听器
	 */


	// static void AddSimpleMsgListener(V2TIMSimpleMsgListener* listener);
	//
	// static void RemoveSimpleMsgListener(V2TIMSimpleMsgListener* listener);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Log")
	static FString SendC2CTextMessage(FString text, FString userId, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate,
	                                  FIMProgressCallback OnProgressDelegate);

	/*
	 * 3.4 发送单聊自定义（信令）消息（最大支持 8KB）
	 */

	//static FString SendC2CCustomMessage(const V2TIMBuffer& customData, const FString& userID) ;

	/*
	 * 3.5 发送群聊普通文本消息（最大支持 8KB）
	 */
	UFUNCTION(BlueprintCallable, Category = "TencentIMLink")
	static FString SendGroupTextMessage(const FString& text, const FString& groupID, EIMMessagePriority priority);

	/**
	 *	3.6 发送群聊自定义（信令）消息（最大支持 8KB）
	*/
	// UFUNCTION(BlueprintCallable, Category = "TencentIMLink")
	static FString SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, EIMMessagePriority priority);


	///-----------------------------------------------------------------------------
	///Group Func
	///todo group category
	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Group")
	static void CreateGroup(const FString& groupType, const FString& groupID, const FString& groupName, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Group")
	static void JoinGroup(const FString& groupID, const FString& message, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Group")
	static void QuitGroup(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Group")
	void DismissGroup(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);


	//--------------------------------------------
	//User func
	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|User")
	static void GetUsersInfo(const TArray<FString>& userIDList, FIMUserFullInfoCallback UserInfoDelegate);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink|User")
	static void SetSelfInfo(const FTIMUserFullInfo& Info, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);


	//------------------------------
	//AddAdvancedMsgListener

	/*
	 * 2.1 创建文本消息
	 */
	// UFUNCTION(BlueprintCallable, Category = "TencentIMLink|Advanced|Message")
	//  FTIMMessage CreateTextMessage(const FString& text);


public:
	static V2TIMString ToIMString(const FString& InStr);

	static FString ToFString(const V2TIMString& InStr);

	static V2TIMStringVector ToIMStringArray(TArray<FString> InStrArray);

	static ELoginStatus ToTIMLoginStatus(const V2TIMLoginStatus& Status);

	static V2TIMMessagePriority GetMessagePriority(EIMMessagePriority InPriority);

	static FTIMUserFullInfo ToTIMUserFullInfo(const V2TIMUserFullInfo& info);

	static V2TIMUserFullInfo ToV2TIMUserFullInfo(const FTIMUserFullInfo& info);

	static ETIMGender ToTIMGender(V2TIMGender Gender);

	static ETIMFriendAllowType ToTIMAllowType(V2TIMFriendAllowType AllowType);

	static V2TIMFriendAllowType ToV2TIMAllowType(const ETIMFriendAllowType& AllowType);

	static TMap<FString, V2TIMBuffer> ToTIMCustomInfo(V2TIMCustomInfo CustomInfo);

	static V2TIMCustomInfo ToV2TIMCustomInfo(TMap<FString, V2TIMBuffer> CustomInfo);

	static TArray<FTIMUserFullInfo> ToTIMUserFullInfoArray(const V2TIMUserFullInfoVector& FullInfoVector);
};
