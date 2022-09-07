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

USTRUCT(Blueprintable)
struct TENCENTIMLINK_API FV2TIMSDKConfig
{
	GENERATED_BODY()

	struct V2TIMSDKConfig;
};

UENUM(BlueprintType)
enum FV2TIMGender
{
	/// 未知性别
	FV2TIM_GENDER_UNKNOWN = 0,
	/// 男性
	FV2TIM_GENDER_MALE = 1,
	/// 女性
	FV2TIM_GENDER_FEMALE = 2,
};

/// 好友验证方式
UENUM(BlueprintType)
enum FV2TIMFriendAllowType
{
	/// 同意任何用户加好友
	FV2TIM_FRIEND_ALLOW_ANY = 0,
	/// 需要验证
	FV2TIM_FRIEND_NEED_CONFIRM = 1,
	/// 拒绝任何人加好友
	FV2TIM_FRIEND_DENY_ANY = 2,
};


/// 好友申请类型
UENUM(BlueprintType)
enum FV2TIMFriendApplicationType
{
	/// 别人发给我的
	FAT_NONE = 0,
	FV2TIM_FRIEND_APPLICATION_COME_IN = 1,
	/// 我发给别人的
	FV2TIM_FRIEND_APPLICATION_SEND_OUT = 2,
	/// 别人发给我的 和 我发给别人的。仅拉取时有效
	FV2TIM_FRIEND_APPLICATION_BOTH = 3,
};

/// 好友类型
UENUM(BlueprintType)
enum FV2TIMFriendType
{
	/// 单向好友
	FT_NONE = 0,
	FV2TIM_FRIEND_TYPE_SINGLE = 1,
	/// 双向好友
	FV2TIM_FRIEND_TYPE_BOTH = 2,
};

/// 好友关系类型
UENUM(BlueprintType)
enum FV2TIMFriendRelationType
{
	/// 不是好友
	FV2TIM_FRIEND_RELATION_TYPE_NONE = 0x0,
	/// 对方在我的好友列表中
	FV2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST = 0x1,
	/// 我在对方的好友列表中
	FV2TIM_FRIEND_RELATION_TYPE_IN_OTHER_FRIEND_LIST = 0x2,
	/// 互为好友
	FV2TIM_FRIEND_RELATION_TYPE_BOTH_WAY = 0x3,
};

/// 好友申请接受类型
UENUM(BlueprintType)
enum FV2TIMFriendAcceptType
{
	/// 接受加好友（建立单向好友）
	FV2TIM_FRIEND_ACCEPT_AGREE = 0,
	/// 接受加好友并加对方为好友（建立双向好友）
	FV2TIM_FRIEND_ACCEPT_AGREE_AND_ADD = 1,
};

// 用户资料修改标记
UENUM(BlueprintType)
enum FV2TIMUserInfoModifyFlag
{
	// 未定义
	FV2TIM_USER_INFO_MODIFY_FLAG_UNKNOWN = 0,
	// 昵称
	FV2TIM_USER_INFO_MODIFY_FLAG_NICK = 1,
	// 头像
	FV2TIM_USER_INFO_MODIFY_FLAG_FACE_URL = 2,
	// 性别
	FV2TIM_USER_INFO_MODIFY_FLAG_GENDER = 3,
	// 生日
	FV2TIM_USER_INFO_MODIFY_FLAG_BIRTHDAY = 4,
	// 修改签名
	FV2TIM_USER_INFO_MODIFY_FLAG_SELF_SIGNATURE = 7,
	// 等级
	FV2TIM_USER_INFO_MODIFY_FLAG_LEVEL = 8,
	// 角色
	FV2TIM_USER_INFO_MODIFY_FLAG_ROLE = 9,
	// 好友验证方式
	FV2TIM_USER_INFO_MODIFY_FLAG_ALLOW_TYPE = 10,
	// 自定义字段
	FV2TIM_USER_INFO_MODIFY_FLAG_CUSTOM = 11,
};

// 好友资料修改标记
UENUM(BlueprintType)
enum FV2TIMFriendInfoModifyFlag
{
	// 未定义
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_UNKNOWN = 0,
	// 好友备注
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_REMARK = 1,
	// 好友自定义字段
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_CUSTOM = 2,
};


USTRUCT(Blueprintable)
struct TENCENTIMLINK_API FV2TIMUserFullInfo
{
	GENERATED_BODY()

	/// 用户 ID
	FString userID;
	/// 用户昵称
	FString nickName;
	/// 用户头像
	FString faceURL;

	/// 用户签名
	FString selfSignature;
	/// 用户性别
	FV2TIMGender gender;
	/// 用户角色
	uint32_t role;
	/// 用户等级
	uint32_t level;
	/// 出生日期
	uint32_t birthday;
	/// 用户好友验证方式
	FV2TIMFriendAllowType allowType;
	/// 用户自定义字段
	/// 首先要在 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 用户自定义字段)
	/// 配置用户自定义字段，然后再调用该接口进行设置，key 值不需要加 Tag_Profile_Custom_ 前缀。
	V2TIMCustomInfo customInfo;
	// 用户资料修改标记位
	// 枚举 V2TIMUserInfoModifyFlag 列出哪些字段支持修改，如果您修改用户资料，请设置这个字段值
	// 支持同时修改多个字段，多个枚举值按位或 | 组合，例如，同时修改用户昵称和用户签名
	// info.nickName = "new nickname";
	// info.selfSignature = "new self signature";
	// info.modifyFlag = V2TIM_USER_INFO_MODIFY_FLAG_NICK | V2TIM_USER_INFO_MODIFY_FLAG_SELF_SIGNATURE;
	uint32_t modifyFlag;
};

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


	UFUNCTION(BlueprintCallable, Category = "TencentIMLink")
	static void JoinGroup(const FString& groupID, const FString& message, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);

	UFUNCTION(BlueprintCallable, Category = "TencentIMLink")
	static void QuitGroup(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate);

	static V2TIMString ToIMString(const FString& InStr);
	static FString ToFString(const V2TIMString& InStr);
	static V2TIMStringVector ToIMStringArray(TArray<FString> InStrArray);
	static ELoginStatus GetLoginStatus(const V2TIMLoginStatus& Status);

	static V2TIMMessagePriority GetMessagePriority(EIMMessagePriority InPriority);
};
