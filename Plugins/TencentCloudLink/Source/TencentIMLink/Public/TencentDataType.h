// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "V2TIMFriendship.h"

#include "UObject/Object.h"
#include "TencentDataType.generated.h"


UENUM(BlueprintType)
enum class ELoginStatus :uint8
{
	/// 已登录
	None = 0,
	V2TIM_STATUS_LOGINED = 1,
	/// 登录中
	V2TIM_STATUS_LOGINING = 2,
	/// 无登录
	V2TIM_STATUS_LOGOUT = 3,
};

UENUM(BlueprintType)
enum class EIMMessagePriority :uint8
{
	/// 默认
	V2TIM_PRIORITY_DEFAULT = 0,
	/// 高优先级，一般用于礼物等重要消息
	V2TIM_PRIORITY_HIGH = 1,
	/// 常规优先级，一般用于普通消息
	V2TIM_PRIORITY_NORMAL = 2,
	/// 低优先级，一般用于点赞消息
	V2TIM_PRIORITY_LOW = 3,
};

UCLASS()
class UMyClass : public UObject
{
	GENERATED_BODY()
public:
	
};

//todo unknown custom Info for sun shi jia
// UCLASS(Blueprintable,BlueprintType)
// class TENCENTIMLINK_API UTIMBuffer: public UObject
// {
// public:
// 	GENERATED_BODY()
//
// 	UTIMBuffer();
//
// 	UTIMBuffer(const UTIMBuffer& buffer);
//
// 	UTIMBuffer(const uint8* data, uint64 size);
//
// 	virtual ~UTIMBuffer() override;
//
// 	const uint8* Data() const;
//
// 	uint64 Size() const;
//
// 	UTIMBuffer& operator=(const UTIMBuffer& buffer);
//
// private:
// 	
// 	uint8* buffer_;
// 	uint64 length_;
// };

USTRUCT(Blueprintable)
struct TENCENTIMLINK_API FV2TIMSDKConfig
{
	GENERATED_BODY()

	struct V2TIMSDKConfig;
};

UENUM(BlueprintType)
enum class ETIMGender:uint8
{
	/// 未知性别
	V2TIM_GENDER_UNKNOWN = 0,
	/// 男性
	V2TIM_GENDER_MALE = 1,
	/// 女性
	V2TIM_GENDER_FEMALE = 2,
};

/// 好友验证方式
UENUM(BlueprintType)
enum class ETIMFriendAllowType:uint8
{
	/// 同意任何用户加好友
	V2TIM_FRIEND_ALLOW_ANY = 0,
	/// 需要验证
	V2TIM_FRIEND_NEED_CONFIRM = 1,
	/// 拒绝任何人加好友
	V2TIM_FRIEND_DENY_ANY = 2,
};


/// 好友申请类型
UENUM(BlueprintType)
enum class FV2TIMFriendApplicationType:uint8
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
enum class FV2TIMFriendType:uint8
{
	/// 单向好友
	FT_NONE = 0,
	FV2TIM_FRIEND_TYPE_SINGLE = 1,
	/// 双向好友
	FV2TIM_FRIEND_TYPE_BOTH = 2,
};

/// 好友关系类型
UENUM(BlueprintType)
enum class FV2TIMFriendRelationType:uint8
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
enum class FV2TIMFriendAcceptType:uint8
{
	/// 接受加好友（建立单向好友）
	FV2TIM_FRIEND_ACCEPT_AGREE = 0,
	/// 接受加好友并加对方为好友（建立双向好友）
	FV2TIM_FRIEND_ACCEPT_AGREE_AND_ADD = 1,
};


// 用户资料修改标记
UENUM(BlueprintType)
enum class FV2TIMUserInfoModifyFlag:uint8
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
enum class ETIMFriendInfoModifyFlag:uint8
{
	// 未定义
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_UNKNOWN = 0,
	// 好友备注
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_REMARK = 1,
	// 好友自定义字段
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_CUSTOM = 2,
};

USTRUCT(Blueprintable)
struct TENCENTIMLINK_API FTIMUserFullInfo
{
	GENERATED_BODY()

	/// 用户 ID
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	FString userID;
	/// 用户昵称
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	FString nickName;
	/// 用户头像
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	FString faceURL;

	/// 用户签名
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	FString selfSignature;

	/// 用户性别
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	ETIMGender gender;
	/// 用户角色
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	int32 role;
	/// 用户等级
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	int32 level;
	/// 出生日期
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	int32 birthday;
	/// 用户好友验证方式
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	ETIMFriendAllowType allowType;
	/// 用户自定义字段
	/// 首先要在 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 用户自定义字段)
	/// 配置用户自定义字段，然后再调用该接口进行设置，key 值不需要加 Tag_Profile_Custom_ 前缀。
	
	// FV2TIMCustomInfo customInfo;
	
	//todo ????
	// UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	TMap<FString,V2TIMBuffer> customInfo;
	
	// 用户资料修改标记位
	// 枚举 V2TIMUserInfoModifyFlag 列出哪些字段支持修改，如果您修改用户资料，请设置这个字段值
	// 支持同时修改多个字段，多个枚举值按位或 | 组合，例如，同时修改用户昵称和用户签名
	// info.nickName = "new nickname";
	// info.selfSignature = "new self signature";
	// info.modifyFlag = V2TIM_USER_INFO_MODIFY_FLAG_NICK | V2TIM_USER_INFO_MODIFY_FLAG_SELF_SIGNATURE;
	UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	int32 modifyFlag;


};


DECLARE_DYNAMIC_DELEGATE_TwoParams(FProgressCallbackDelegate, int64, ConsumedBytes, int64, TotalBytes);

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UTencentDataType : public UObject
{
	GENERATED_BODY()
};
