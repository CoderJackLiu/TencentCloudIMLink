// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "TencentIMLibrary.h"
#include "FriendshipListener.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UFriendshipListener : public UUserWidget, public V2TIMFriendshipListener
{
	GENERATED_BODY()
public:
	/**
	  * 好友申请新增通知，两种情况会收到这个回调：
	  * 1. 自己申请加别人好友
	  * 2. 别人申请加自己好友
	  */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFriendApplicationListAdded(const TArray<FTIMFriendApplication>& UserAPPList);
	/**
	 * 好友申请删除通知，四种情况会收到这个回调
	 * 1. 调用 DeleteFriendApplication 主动删除好友申请
	 * 2. 调用 RefuseFriendApplication 拒绝好友申请
	 * 3. 调用 AcceptFriendApplication 同意好友申请且同意类型为 V2TIM_FRIEND_ACCEPT_AGREE 时
	 * 4. 申请加别人好友被拒绝
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFriendApplicationListDeleted(const TArray<FString>& UserAPPDeleteList);
	/**
     * 好友申请已读通知，如果调用 setFriendApplicationRead
     * 设置好友申请列表已读，会收到这个回调（主要用于多端同步）
     */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFriendApplicationListHasRead();

	/**
	 * 好友删除通知，，两种情况会收到这个回调：
	 * 1. 自己删除好友（单向和双向删除都会收到回调）
	 * 2. 好友把自己删除（双向删除会收到）
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFriendListDeleted(const TArray<FString>& DeleteFriendsList);

	/**
	 * 好友新增通知
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFriendListAdded(const TArray<FTIMFriendInfo>& NewFriendList);
	/**
	 * 黑名单新增通知
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBlackListAdded(const TArray<FTIMFriendInfo>& NewBlackListList);
	/**
	 * 黑名单删除通知
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBlackListDeleted(const TArray<FString>& BlackListDeletedList);

	/**
	 * 好友资料更新通知
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFriendInfoChanged(const TArray<FTIMFriendInfo>& FriendsInfoChangeList);

private:
	virtual void OnFriendApplicationListAdded(const V2TIMFriendApplicationVector& applicationList) override
	{
		OnFriendApplicationListAdded(UTencentIMLibrary::ToTIMFriendApplicationArray(applicationList));
	}

	virtual void OnFriendApplicationListDeleted(const V2TIMStringVector& userIDList) override
	{
		OnFriendApplicationListDeleted(UTencentIMLibrary::ToFStringArray(userIDList));
	}

	virtual void OnFriendApplicationListRead() override
	{
		OnFriendApplicationListHasRead();
	}

	virtual void OnFriendListAdded(const V2TIMFriendInfoVector& userIDList) override
	{
		OnFriendListAdded(UTencentIMLibrary::ToFriendInfoArray(userIDList));
	}

	virtual void OnFriendListDeleted(const V2TIMStringVector& userIDList) override
	{
		OnFriendListDeleted(UTencentIMLibrary::ToFStringArray(userIDList));
	}

	virtual void OnBlackListAdded(const V2TIMFriendInfoVector& infoList) override
	{
		OnBlackListAdded(UTencentIMLibrary::ToFriendInfoArray(infoList));
	}

	virtual void OnBlackListDeleted(const V2TIMStringVector& userIDList) override
	{
		OnBlackListDeleted(UTencentIMLibrary::ToFStringArray(userIDList));
	}

	virtual void OnFriendInfoChanged(const V2TIMFriendInfoVector& infoList) override
	{
		OnFriendInfoChanged(UTencentIMLibrary::ToFriendInfoArray(infoList));
	}
};
