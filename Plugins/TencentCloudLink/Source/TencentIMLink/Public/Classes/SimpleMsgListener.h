// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TencentDataType.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "SimpleMsgListener.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API USimpleMsgListener : public UUserWidget, public V2TIMSimpleMsgListener
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvC2CTextMessage(const FString& msgID, const FTIMUserFullInfo& sender, const FString& text);

private:
	/**
   * 收到 C2C 文本消息
   *
   * @param msgID 消息唯一标识
   * @param sender 发送方信息
   * @param text 发送内容
   */

	virtual void OnRecvC2CTextMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMString& text) override;

	/**
	 * 收到 C2C 自定义（信令）消息
	 *
	 * @param msgID 消息唯一标识
	 * @param sender 发送方信息
	 * @param customData 发送内容
	 */
	virtual void OnRecvC2CCustomMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMBuffer& customData) override;

	/**
	 * 收到群文本消息
	 *
	 * @param msgID 消息唯一标识
	 * @param groupID 群 ID
	 * @param sender 发送方群成员信息
	 * @param text 发送内容
	 */
	virtual void OnRecvGroupTextMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMString& text) override;

	/**
	 * 收到群自定义（信令）消息
	 *
	 * @param msgID 消息唯一标识
	 * @param groupID 群 ID
	 * @param sender 发送方群成员信息
	 * @param customData 发送内容
	 */
	virtual void OnRecvGroupCustomMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMBuffer& customData) override;
};
