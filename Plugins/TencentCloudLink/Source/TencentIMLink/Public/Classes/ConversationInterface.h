// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "UObject/Interface.h"
#include "ConversationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UConversationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TENCENTIMLINK_API IConversationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// virtual void OnFriendApplicationListAdded(const V2TIMFriendApplicationVector& applicationList)
	// {
	// };
	//
	// virtual void OnFriendApplicationListDeleted(const V2TIMStringVector& userIDList)
	// {
	// };
	//
	// virtual void OnFriendApplicationListRead()
	// {
	// };
	//
	// virtual void OnFriendListAdded(const V2TIMFriendInfoVector& userIDList)
	// {
	// };
	//
	// virtual void OnFriendListDeleted(const V2TIMStringVector& userIDList)
	// {
	// };
	//
	// virtual void OnBlackListAdded(const V2TIMFriendInfoVector& infoList)
	// {
	// };
	//
	// virtual void OnBlackListDeleted(const V2TIMStringVector& userIDList)
	// {
	// };
	//
	// virtual void OnFriendInfoChanged(const V2TIMFriendInfoVector& infoList)
	// {
	// };
};
