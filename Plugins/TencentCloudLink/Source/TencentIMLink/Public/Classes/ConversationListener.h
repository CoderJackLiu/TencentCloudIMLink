// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "TencentDataType.h"
#include "TencentIMLibrary.h"
#include "ConversationListener.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UConversationListener : public UUserWidget,public V2TIMAdvancedMsgListener,public V2TIMGroupListener,public V2TIMSimpleMsgListener
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


public:
	/**
	* 收到 C2C 文本消息
	*
	* @param msgID 消息唯一标识
	* @param sender 发送方信息
	* @param text 发送内容
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvC2CTextMessage(const FString &msgID, const FTIMUserFullInfo &sender,const FString &text);

	/**
	 * 收到 C2C 自定义（信令）消息
	 *
	 * @param msgID 消息唯一标识
	 * @param sender 发送方信息
	 * @param customData 发送内容
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvC2CCustomMessage(const FString &msgID, const FTIMUserFullInfo &sender,const FBuffer &customData);

	/**
	 * 收到群文本消息
	 *
	 * @param msgID 消息唯一标识
	 * @param groupID 群 ID
	 * @param sender 发送方群成员信息
	 * @param text 发送内容
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvGroupTextMessage(const FString &msgID, const FString &groupID,const FTIMGroupMemberFullInfo &sender,const FString &text);

	/**
	 * 收到群自定义（信令）消息
	 *
	 * @param msgID 消息唯一标识
	 * @param groupID 群 ID
	 * @param sender 发送方群成员信息
	 * @param customData 发送内容
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvGroupCustomMessage(const FString &msgID, const FString &groupID,const FTIMGroupMemberFullInfo &sender, const FBuffer &customData);
	

	
private:

	virtual void OnRecvC2CTextMessage(const V2TIMString &msgID, const V2TIMUserFullInfo &sender, const V2TIMString &text)
	{
		OnRecvC2CTextMessage(UTencentIMLibrary::ToFString(msgID),UTencentIMLibrary::ToTIMUserFullInfo(sender),UTencentIMLibrary::ToFString(text));
	}


	virtual void OnRecvC2CCustomMessage(const V2TIMString &msgID, const V2TIMUserFullInfo &sender,const V2TIMBuffer &customData)
	{
		OnRecvC2CCustomMessage(UTencentIMLibrary::ToFString(msgID),UTencentIMLibrary::ToTIMUserFullInfo(sender),UTencentIMLibrary::ToBuffer(customData));
	}


	virtual void OnRecvGroupTextMessage(const V2TIMString &msgID, const V2TIMString &groupID,const V2TIMGroupMemberFullInfo &sender,const V2TIMString &text)
	{
		OnRecvGroupTextMessage(UTencentIMLibrary::ToFString(msgID),UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberFullInfo(sender),UTencentIMLibrary::ToFString(text));
	}


	virtual void OnRecvGroupCustomMessage(const V2TIMString &msgID, const V2TIMString &groupID, const V2TIMGroupMemberFullInfo &sender,const V2TIMBuffer &customData)
	{
		OnRecvGroupCustomMessage(UTencentIMLibrary::ToFString(msgID),UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberFullInfo(sender),UTencentIMLibrary::ToBuffer(customData));
	}
public:
	/**
	* 收到新消息
	*
	* @param message 消息
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReceiveNewMessage(const FString &UserName,const FString &message);
	
	/**
	 * 收到消息撤回的通知
	 *
	 * @param messageID 消息唯一标识
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRecvMessageRevoked(const FString &message);

	/**
	 * 消息内容被修改（第三方服务回调修改了消息内容）
	 */
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


	virtual void OnRecvMessageRevoked(const V2TIMString &messageID) override
	{
		OnRecvMessageRevoked(UTencentIMLibrary::ToFString(messageID));
	}


	virtual void OnRecvMessageModified(const V2TIMMessage &message) override
	{
		OnRecvMessageModified(UTencentIMLibrary::ToMessage(message));
	}

public:
	/**
	 * 有新成员加入群（该群所有的成员都能收到，会议群（Meeting）默认无此回调，如需回调请提交工单配置）
	 *
	 * @param groupID    群 ID
	 * @param memberList 加入的成员
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMemberEnter(const FString& groupID, const TArray<FTIMGroupMemberInfo>& memberList);

	/**
	 * 有成员离开群（该群所有的成员都能收到，会议群（Meeting）默认无此回调，如需回调请提交工单配置）
	 *
	 * @param groupID 群 ID
	 * @param member  离开的成员
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMemberLeave(const FString& groupID, const FTIMGroupMemberInfo& member);
	/**
	 * 某成员被拉入某群（该群所有的成员都能收到）
	 *
	 * @param groupID    群 ID
	 * @param opUser     处理人
	 * @param memberList 被拉进群成员
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMemberInvited(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList);
	/**
	 * 有成员被踢出某群（该群所有的成员都能收到）
	 *
	 * @param groupID    群 ID
	 * @param opUser     处理人
	 * @param memberList 被踢成员
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMemberKicked(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList);

	/**
	 * 某成员信息发生变更（该群所有的成员都能收到）。
	 * @note 会议群（Meeting）和直播群（AVChatRoom）默认无此回调，如需回调请提交工单配置
	 *
	 * @param groupID 群 ID
	 * @param v2TIMGroupMemberChangeInfoList 被修改的群成员信息
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnMemberInfoChanged(const FString& groupID, const TArray<FTIMGroupMemberChangeInfo>& v2TIMGroupMemberChangeInfoList);

	/**
	 * 创建群（主要用于多端同步）
	 *
	 * @param groupID 群 ID
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGroupCreated(const FString& groupID);

	/**
	 * 群被解散了（全员能收到）
	 *
	 * @param groupID 群 ID
	 * @param opUser  处理人
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGroupDismissed(const FString& groupID, const FTIMGroupMemberInfo& opUser) ;

	/**
	 * 群被回收（全员能收到）
	 *
	 * @param groupID 群 ID
	 * @param opUser  处理人
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGroupRecycled(const FString& groupID, const FTIMGroupMemberInfo& opUser) ;
	
	/**
	 * 群信息被修改（全员能收到）
	 *
	 * @param changeInfos 修改的群信息
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGroupInfoChanged(const FString& groupID, const TArray<FTIMGroupChangeInfo>& changeInfos);
	
	/**
	 * 有新的加群请求（只有群主或管理员会收到）
	 *
	 * @param groupID  群 ID
	 * @param member   申请人
	 * @param opReason 申请原因
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReceiveJoinApplication(const FString& groupID, const FTIMGroupMemberInfo& member, const FString& opReason);

	/**
	 * 加群请求已经被群主或管理员处理了（只有申请人能够收到）
	 *
	 * @param groupID     群 ID
	 * @param opUser      处理人
	 * @param isAgreeJoin 是否同意加群
	 * @param opReason    处理原因
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnApplicationProcessed(const FString& groupID, const FTIMGroupMemberInfo& opUser, bool isAgreeJoin, const FString& opReason);

	/**
	 * 指定管理员身份
	 *
	 * @param groupID    群 ID
	 * @param opUser     处理人
	 * @param memberList 被处理的群成员
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGrantAdministrator(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList);
	
	/**
	 * 取消管理员身份
	 *
	 * @param groupID    群 ID
	 * @param opUser     处理人
	 * @param memberList 被处理的群成员
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRevokeAdministrator(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList); 
	/**
	 * 主动退出群组（主要用于多端同步，直播群（AVChatRoom）不支持）
	 *
	 * @param groupID 群 ID
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnQuitFromGroup(const FString& groupID);

	/**
	 * 收到 RESTAPI 下发的自定义系统消息
	 *
	 * @param groupID    群 ID
	 * @param customData 自定义数据
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReceiveRESTCustomData(const FString& groupID, const FBuffer& customData);

	/**
	 * 收到群属性更新的回调
	 *
	 * @param groupID           群 ID
	 * @param groupAttributeMap 群的所有属性
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGroupAttributeChanged(const FString& groupID, const TMap<FString,FString>& groupAttributeMap);
	
private:

	virtual void OnMemberEnter(const V2TIMString& groupID, const V2TIMGroupMemberInfoVector& memberList) override
	{
		OnMemberEnter(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfoArray(memberList));
	}

	virtual void OnMemberLeave(const V2TIMString& groupID, const V2TIMGroupMemberInfo& member) override
	{
		OnMemberLeave(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(member));
	}

	virtual void OnMemberInvited(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser, const V2TIMGroupMemberInfoVector& memberList) override
	{
		OnMemberInvited(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser),UTencentIMLibrary::ToTIMGroupMemberInfoArray(memberList));
	}

	virtual void OnMemberKicked(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser, const V2TIMGroupMemberInfoVector& memberList) override
	{
		OnMemberInvited(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser),UTencentIMLibrary::ToTIMGroupMemberInfoArray(memberList));

	}

	virtual void OnMemberInfoChanged(const V2TIMString& groupID, const V2TIMGroupMemberChangeInfoVector& v2TIMGroupMemberChangeInfoList) override
	{
		OnMemberInfoChanged(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberChangeInfoArray(v2TIMGroupMemberChangeInfoList));
	}


	virtual void OnGroupCreated(const V2TIMString& groupID) override
	{
		OnGroupCreated(UTencentIMLibrary::ToFString(groupID));
	}

	virtual void OnGroupDismissed(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser) override
	{
		OnGroupDismissed(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser));
	}


	virtual void OnGroupRecycled(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser) override
	{
		OnGroupRecycled(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser));
	}


	virtual void OnGroupInfoChanged(const V2TIMString& groupID, const V2TIMGroupChangeInfoVector& changeInfos) override
	{
		OnGroupInfoChanged(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupChangeInfoArray(changeInfos));
	}


	virtual void OnReceiveJoinApplication(const V2TIMString& groupID, const V2TIMGroupMemberInfo& member, const V2TIMString& opReason) override
	{
		OnReceiveJoinApplication(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(member),UTencentIMLibrary::ToFString(opReason));
	}


	virtual void OnApplicationProcessed(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser, bool isAgreeJoin, const V2TIMString& opReason) override
	{
		OnApplicationProcessed(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser),isAgreeJoin,UTencentIMLibrary::ToFString(opReason));
	}


	virtual void OnGrantAdministrator(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser, const V2TIMGroupMemberInfoVector& memberList) override
	{
		OnGrantAdministrator(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser),UTencentIMLibrary::ToTIMGroupMemberInfoArray(memberList));
	}


	virtual void OnRevokeAdministrator(const V2TIMString& groupID, const V2TIMGroupMemberInfo& opUser, const V2TIMGroupMemberInfoVector& memberList) override
	{
		OnRevokeAdministrator(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToTIMGroupMemberInfo(opUser),UTencentIMLibrary::ToTIMGroupMemberInfoArray(memberList));
	}


	virtual void OnQuitFromGroup(const V2TIMString& groupID) override
	{
		OnQuitFromGroup(UTencentIMLibrary::ToFString(groupID));
	}


	virtual void OnReceiveRESTCustomData(const V2TIMString& groupID, const V2TIMBuffer& customData) override
	{
		OnReceiveRESTCustomData(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToBuffer(customData));
	}


	virtual void OnGroupAttributeChanged(const V2TIMString& groupID, const V2TIMGroupAttributeMap& groupAttributeMap) override
	{
		OnGroupAttributeChanged(UTencentIMLibrary::ToFString(groupID),UTencentIMLibrary::ToFStringMap(groupAttributeMap));
	}
};
