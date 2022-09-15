// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "UObject/Object.h"
#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "TencentDataType.h"
#include "TencentIMLibrary.h"
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

#include "FriendConversation.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UFriendConversation : public UUserWidget,public V2TIMAdvancedMsgListener
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//listener接口：
public:
	/**
	* 收到新消息
	*
	* @param message 消息
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReceiveNewMessage(const FString &UserName,const FString &message);
	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvMessageRevoked(const FString &message);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvMessageModified(const FTIMMessage &message);

private:
	virtual void OnRecvNewMessage(const V2TIMMessage &message) override
	{
		UE_LOG(LogTemp, Log, TEXT("==>OnRecvNewMessage ======"));
		V2TIMSignalingInfo info = V2TIMManager::GetInstance()->GetSignalingManager()->GetSignalingInfo(message);

		for (unsigned long j = 0; j < message.elemList.Size(); ++j)
		{
			
			switch (message.elemList[j]->elemType)
			{
			case V2TIM_ELEM_TYPE_TEXT:
				{
					AsyncTask(ENamedThreads::GameThread, [=]()
					{
						const char* msgEle = ((V2TIMTextElem*)message.elemList[j])->text.CString();
						UTextBlock* textBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
						FString other = message.sender.CString();
						FString ele = UTF8_TO_TCHAR(msgEle); // FString(TEXT("&msgEle"));
						
						// FString UserId=message.nickName.CString();
						OnReceiveNewMessage(other,ele);
					});
				}
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_TEXT"));
				break;
			case V2TIM_ELEM_TYPE_CUSTOM:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_CUSTOM"));
				break;
			case V2TIM_ELEM_TYPE_IMAGE:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_IMAGE"));
				break;
			case V2TIM_ELEM_TYPE_SOUND:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_SOUND"));
				break;
			case V2TIM_ELEM_TYPE_VIDEO:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_VIDEO"));
				break;
			case V2TIM_ELEM_TYPE_FILE:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_FILE"));
				break;
			case V2TIM_ELEM_TYPE_LOCATION:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_LOCATION"));
				break;
			case V2TIM_ELEM_TYPE_FACE:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_FACE"));
				break;
			case V2TIM_ELEM_TYPE_GROUP_TIPS:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_GROUP_TIPS"));
				break;
			case V2TIM_ELEM_TYPE_MERGER:
				UE_LOG(LogTemp,Warning,TEXT("V2TIM_ELEM_TYPE_MERGER"));
				break;
			default:
				UE_LOG(LogTemp,Warning,TEXT("default"));
				break;
			}
		}
	}

	/**
	 * 收到 C2C 消息已读回执
	 *
	 * @param receiptList 已读回执列表
	 */
	virtual void OnRecvC2CReadReceipt(const V2TIMMessageReceiptVector &receiptList) override
	{
		
	}

	/**
	 * 收到消息撤回的通知
	 *
	 * @param messageID 消息唯一标识
	 */
	virtual void OnRecvMessageRevoked(const V2TIMString &messageID) override
	{
		OnRecvMessageRevoked(UTencentIMLibrary::ToFString(messageID));
	}

	/**
	 * 消息内容被修改（第三方服务回调修改了消息内容）
	 */
	virtual void OnRecvMessageModified(const V2TIMMessage &message) override
	{
		OnRecvMessageModified(UTencentIMLibrary::ToMessage(message));
	}
};
