// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
{	/// 默认
	V2TIM_PRIORITY_DEFAULT = 0,
	/// 高优先级，一般用于礼物等重要消息
	V2TIM_PRIORITY_HIGH = 1,
	/// 常规优先级，一般用于普通消息
	V2TIM_PRIORITY_NORMAL = 2,
	/// 低优先级，一般用于点赞消息
	V2TIM_PRIORITY_LOW = 3,
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
