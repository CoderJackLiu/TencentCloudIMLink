// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TencentDataType.h"
#include "TencentIMLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "GroupListenerWidget.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UGroupListenerWidget : public UUserWidget, public V2TIMGroupListener
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
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
