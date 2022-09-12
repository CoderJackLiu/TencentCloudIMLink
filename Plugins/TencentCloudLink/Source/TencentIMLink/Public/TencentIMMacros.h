// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TencentDataType.h"
#include "TencentIMMacros.generated.h"

/**
 * 
 */
//------------------------------------
//success
DECLARE_DYNAMIC_DELEGATE(FIMCallbackDelegate);

DECLARE_DYNAMIC_DELEGATE_OneParam(FIMCallbackTextDelegate, FString, Text);

DECLARE_DYNAMIC_DELEGATE_OneParam(FMessage_ArrayCallbackTextDelegate, const TArray<FTIMReceiveMessageOptInfo>&, Messages);

//userinfo
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMUserFullInfoCallback, const TArray<FTIMUserFullInfo>&, UserInfos);

//groupInfos
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMGroupInfoArrayCallback, const TArray<FTIMGroupInfo>&, GroupInfos);

//mem count 
DECLARE_DYNAMIC_DELEGATE_OneParam(FGroupMemCountCallback, const int64&, GroupInfos);

//group mem full info
DECLARE_DYNAMIC_DELEGATE_OneParam(FGroupMemFullInfosCallback, const TArray<FTIMGroupMemberFullInfo>&, GroupMemFullInfos);

//group invite
DECLARE_DYNAMIC_DELEGATE_OneParam(FGroupIniteUserCallback, const TArray<FTIMGroupMemberOperationResult>&, IniteResults);

//加群申请
DECLARE_DYNAMIC_DELEGATE_OneParam(FGroupAppRstCallback, const FTIMGroupApplicationResult&, AppRstResults);


//
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMGroupMessageInfoCallback, const TArray<FTIMMessage>&, Messages);

DECLARE_DYNAMIC_DELEGATE_OneParam(FMessageSearchResultCallback, const FTIMMessageSearchResult&, Result);

DECLARE_DYNAMIC_DELEGATE_OneParam(FMTIMConversationResultCallback, const FTIMConversationResult&, Result);

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMConversationCallback, const FTIMConversation&, Conversation);



DECLARE_DYNAMIC_DELEGATE_OneParam(FFriendAPPArrayAddedCallback, const TArray<FTIMFriendApplication>&, applicationArray);

//1 todo Listener
#define DECLARATION_FriendApplicationListAdded_DELEGATE(Func) \
FFriendAPPArrayAddedCallback Func##_FriendAPPArrayAddedDelegate; \
void Func##_Local(const TArray<FTIMFriendApplication>& applicationArray) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([applicationArray]()\
{\
Func##_FriendAPPArrayAddedDelegate.ExecuteIfBound(applicationArray);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FFriendAPPArrayDeletedCallback, const TArray<FString>&, Results);

//2 todo Listener
#define DECLARATION_FriendApplicationListDeleted_DELEGATE(Func) \
FFriendAPPArrayDeletedCallback Func##_FriendAPPListDeleted; \
void Func##_Local(const TArray<FString>& Results) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Results]()\
{\
Func##_FriendAPPListDeleted.ExecuteIfBound(Results);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FFriendListAddedCallback, const TArray<FTIMFriendInfo>&, Results);

//3 todo Listener
#define DECLARATION_FriendListAdded_DELEGATE(Func) \
FFriendListAddedCallback Func##_FriendListAddedDelegate; \
void Func##_Local(const TArray<FTIMFriendInfo>& Results) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Results]()\
{\
Func##_FriendListAddedDelegate.ExecuteIfBound(Results);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FFriendListDeletedCallback, const TArray<FString>&, Results);

//4 todo Listener
#define DECLARATION_FriendListDeleted_DELEGATE(Func) \
FFriendListDeletedCallback Func##_FriendInfoArrayDeletedDelegate; \
void Func##_Local2(const TArray<FString>& Results) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Results]()\
{\
Func##_FriendInfoArrayDeletedDelegate.ExecuteIfBound(Results);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FBlackListAddedCallback, const TArray<FTIMFriendInfo>&, Results);

//5 todo Listener
#define DECLARATION_FriendInfoArrayDeleted_DELEGATE(Func) \
FBlackListAddedCallback Func##_BlackListAddedDelegate; \
void Func##_Local3(const TArray<FTIMFriendInfo>& Results) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Results]()\
{\
Func##_BlackListAddedDelegate.ExecuteIfBound(Results);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}


DECLARE_DYNAMIC_DELEGATE_OneParam(FBlackListDeletedCallback, const TArray<FString>&, Results);

//todo Listener
#define DECLARATION_BlackListDeleted_DELEGATE(Func) \
FBlackListDeletedCallback Func##_BlackListDeletedDelegate; \
void Func##_Local4(const TArray<FString>& Results) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Results]()\
{\
Func##_BlackListDeletedDelegate.ExecuteIfBound(Results);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FFriendInfoChangedCallback, const TArray<FTIMFriendInfo>&, Results);

//todo Listener
#define DECLARATION_FriendInfoChanged_DELEGATE(Func) \
FFriendInfoChangedCallback Func##_FriendInfoChangedDelegate; \
void Func##_Local5(const TArray<FTIMFriendInfo>& Results) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Results]()\
{\
Func##_FriendInfoChangedDelegate.ExecuteIfBound(Results);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}










//todo success 加群申请
#define DECLARATION_GroupAPPResult_DELEGATE(Func) \
FGroupAppRstCallback Func##_GPAppRstDelegate; \
void Func##_Local37(const FTIMGroupApplicationResult& AppRstResults) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([AppRstResults]()\
{\
Func##_GPAppRstDelegate.ExecuteIfBound(AppRstResults);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success group invite member
#define DECLARATION_GroupInviteMem_DELEGATE(Func) \
FGroupIniteUserCallback Func##_GPInviteMemDelegate; \
void Func##_Local37(const TArray<FTIMGroupMemberOperationResult>& IniteResults) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([IniteResults]()\
{\
Func##_GPInviteMemDelegate.ExecuteIfBound(IniteResults);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success group mem full info 
#define DECLARATION_GroupMemFullInfos_DELEGATE(Func) \
FGroupMemFullInfosCallback Func##_GPMemFullInfoDelegate; \
void Func##_Local37(const TArray<FTIMGroupMemberFullInfo>& MemsFullInfos) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([MemsFullInfos]()\
{\
Func##_GPMemFullInfoDelegate.ExecuteIfBound(MemsFullInfos);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success mem count 
#define DECLARATION_GroupMemCount_DELEGATE(Func) \
FGroupMemCountCallback Func##_GroupMemCountDelegate; \
void Func##_Local37(const int64& Count) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Count]()\
{\
Func##_GroupMemCountDelegate.ExecuteIfBound(Count);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success message 
#define DECLARATION_TIMConversation_DELEGATE(Func) \
FTIMConversationCallback Func##_ConversationDelegate; \
void Func##_Local37(const FTIMConversation& Conversation) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Conversation]()\
{\
Func##_ConversationDelegate.ExecuteIfBound(Conversation);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMConversationVectorCallback, const TArray<FTIMConversation>&, array_Conversation);

//todo success message 
#define DECLARATION_TIMConversationVector_DELEGATE(Func) \
FTIMConversationVectorCallback Func##_ConversationVectorDelegate; \
void Func##_Local51(const TArray<FTIMConversation>& array_Conversation) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([array_Conversation]()\
{\
Func##_ConversationVectorDelegate.ExecuteIfBound(array_Conversation);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMuint64Callback, const uint64&, Result);

//todo success message 
#define DECLARATION_TIMuint64_DELEGATE(Func) \
FTIMuint64Callback Func##_uint64Delegate; \
void Func##_Local65(const uint64& Message) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Message]()\
{\
Func##_uint64Delegate.ExecuteIfBound(Message);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendInfoVectorCallback, const TArray<FTIMFriendInfo>&, FriendInfo);

//todo success message 
#define DECLARATION_TIMFriendInfoVector_DELEGATE(Func) \
FTIMFriendInfoVectorCallback Func##_TIMFriendInfoVectorDelegate; \
void Func##_Local79(const TArray<FTIMFriendInfo>& FriendInfo) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendInfo]()\
{\
Func##_TIMFriendInfoVectorDelegate.ExecuteIfBound(FriendInfo);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendInfoResultVectorCallback, const TArray<FTIMFriendInfoResult>&, FriendInfoResult);

//todo success message 
#define DECLARATION_TIMFriendInfoResultVector_DELEGATE(Func) \
FTIMFriendInfoResultVectorCallback Func##_TIMFriendInfoResultVectorDelegate; \
void Func##_Local93(const TArray<FTIMFriendInfoResult>& FriendInfoResult) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendInfoResult]()\
{\
Func##_TIMFriendInfoResultVectorDelegate.ExecuteIfBound(FriendInfoResult);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}


DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendOperationResulCallback, const FTIMFriendOperationResult&, FriendOperationResult);

//todo success message 
#define DECLARATION_TIMFriendOperationResul_DELEGATE(Func) \
FTIMFriendOperationResulCallback Func##_TIMFriendOperationResulDelegate; \
void Func##_Local93(const FTIMFriendOperationResult& FriendOperationResult) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendOperationResult]()\
{\
Func##_TIMFriendOperationResulDelegate.ExecuteIfBound(FriendOperationResult);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendOperationResultVectorCallback, const TArray<FTIMFriendOperationResult>&, FriendOperationResult);

//todo success message 
#define DECLARATION_TIMFriendOperationResultVector_DELEGATE(Func) \
FTIMFriendOperationResultVectorCallback Func##_TIMFriendOperationResultVectorDelegate; \
void Func##_Local122(const TArray<FTIMFriendOperationResult>& FriendOperationResult) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendOperationResult]()\
{\
Func##_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(FriendOperationResult);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendCheckResultVectorCallback, const TArray<FTIMFriendCheckResult>&, FriendCheckResult);

// //todo success message 
#define DECLARATION_TIMFriendCheckResultVector_DELEGATE(Func) \
FTIMFriendCheckResultVectorCallback Func##_TIMFriendCheckResultVectorDelegate; \
void Func##_Local93(const TArray<FTIMFriendCheckResult>& FriendCheckResult) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendCheckResult]()\
{\
Func##_TIMFriendCheckResultVectorDelegate.ExecuteIfBound(FriendCheckResult);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendApplicationResultCallback, const FTIMFriendApplicationResult&, FriendApplicationResult);

// //todo success message 
#define DECLARATION_TIMFriendApplicationResult_DELEGATE(Func) \
FTIMFriendApplicationResultCallback Func##_TIMFriendApplicationResultDelegate; \
void Func##_Local85(const FTIMFriendApplicationResult& FriendApplicationResult) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendApplicationResult]()\
{\
Func##_TIMFriendApplicationResultDelegate.ExecuteIfBound(FriendApplicationResult);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMFriendGroupVectorCallback, const TArray<FTIMFriendGroup>& , FriendGroup);
// //todo success message 
#define DECLARATION_TIMFriendGroupVector_DELEGATE(Func) \
FTIMFriendGroupVectorCallback Func##_TIMFriendGroupVectorDelegate; \
void Func##_Local85(const TArray<FTIMFriendGroup>& FriendGroup) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([FriendGroup]()\
{\
Func##_TIMFriendGroupVectorDelegate.ExecuteIfBound(FriendGroup);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FTIMGroupMemberInfoResultCallback, const FTIMGroupMemberInfoResult& , GroupMemberInfoResult);
// //todo success message 
#define DECLARATION_TIMGroupMemberInfoResult_DELEGATE(Func) \
FTIMGroupMemberInfoResultCallback Func##_TIMGroupMemberInfoResultDelegate; \
void Func##_Local85(const FTIMGroupMemberInfoResult& GroupMemberInfoResult) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([GroupMemberInfoResult]()\
{\
Func##_TIMGroupMemberInfoResultDelegate.ExecuteIfBound(GroupMemberInfoResult);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

DECLARE_DYNAMIC_DELEGATE_OneParam(FIMMessageInfoCallback, const FTIMMessage& , Message);

//failure
DECLARE_DYNAMIC_DELEGATE_TwoParams(FIMFailureCallback, int, ErrorCode, FString, ErrorMassage);

//progress
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMProgressCallback, uint32, Progress);


// DECLARE_DYNAMIC_DELEGATE_TwoParams(FProgressCallbackDelegate, int64, ConsumedBytes, int64, TotalBytes);


//todo Success with no para
#define DECLARATION_CALLBACK_DELEGATE(Func) \
FIMCallbackDelegate Func##_Delegate; \
void Func##_Local() \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([]()\
{\
Func##_Delegate.ExecuteIfBound();\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success with FString para
#define DECLARATION_StringCALLBACK_DELEGATE(Func) \
FIMCallbackTextDelegate Func##_TextDelegate; \
void Func##_Local(const FString& Message) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Message]()\
{\
Func##_TextDelegate.ExecuteIfBound(Message);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success Receive Message option Arrays
#define DECLARATION_MessageOptCALLBACK_DELEGATE(Func) \
FMessage_ArrayCallbackTextDelegate Func##_MessageArrayDelegate; \
void Func##_Local5(const TArray<FTIMReceiveMessageOptInfo>& Messages) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Messages]()\
{\
Func##_MessageArrayDelegate.ExecuteIfBound(Messages);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success message Arrays
#define DECLARATION_GroupMessageCALLBACK_DELEGATE(Func) \
FIMGroupMessageInfoCallback Func##_GroupMessageArrayDelegate; \
void Func##_Local6(const TArray<FTIMMessage>& Messages) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Messages]()\
{\
Func##_GroupMessageArrayDelegate.ExecuteIfBound(Messages);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

//todo success message 
#define DECLARATION_MessageCALLBACK_DELEGATE(Func) \
FIMMessageInfoCallback Func##_MessageDelegate; \
void Func##_Local7(const FTIMMessage& Message) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Message]()\
{\
Func##_MessageDelegate.ExecuteIfBound(Message);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define DECLARATION_ConversationRst_DELEGATE(Func) \
FMTIMConversationResultCallback Func##_CsRstDelegate; \
void Func##_Local(const FTIMConversationResult& Result) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Result]()\
{\
Func##_CsRstDelegate.ExecuteIfBound(Result);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}


#define DECLARATION_MessageSearchResultCALLBACK_DELEGATE(Func) \
FMessageSearchResultCallback Func##_MsgSearchReaultDelegate; \
void Func##_Local8(const FTIMMessageSearchResult& Result) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Result]()\
{\
Func##_MsgSearchReaultDelegate.ExecuteIfBound(Result);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define DECLARATION_GroupInfoArray_DELEGATE(Func) \
FIMGroupInfoArrayCallback Func##_GroupInfoArrayDelegate; \
void Func##_Local8(const TArray<FTIMGroupInfo>& GroupInfos) \
{ \
FScopeLock ScopeLock(&TencentMutex); \
auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([GroupInfos]()\
{\
Func##_GroupInfoArrayDelegate.ExecuteIfBound(GroupInfos);\
}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}


//todo failure with int and FString para
#define DECLARATION_FAILURE_CALLBACK_DELEGATE(Func) \
FIMFailureCallback Func##_FailureDelegate; \
void Func##_Local2(int Code,const FString& Message) \
{ \
	FScopeLock ScopeLock(&TencentMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Code,Message]()\
	{\
	Func##_FailureDelegate.ExecuteIfBound(Code, Message);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
}

//todo progress with uint32 para
#define DECLARATION_Progress_CALLBACK_DELEGATE(Func) \
FIMProgressCallback Func##_ProgressDelegate; \
void Func##_Local3(uint32 Code) \
{ \
	FScopeLock ScopeLock(&TencentMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Code]()\
	{\
	Func##_ProgressDelegate.ExecuteIfBound(Code);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
}

//todo user full info with TIMUserFullInfo Array para
#define DECLARATION_UserInfo_CALLBACK_DELEGATE(Func) \
FIMUserFullInfoCallback Func##_UserFullInfoDelegate; \
void Func##_Local4(TArray<FTIMUserFullInfo> UserFullInfos) \
{ \
	FScopeLock ScopeLock(&TencentMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([UserFullInfos]()\
	{\
	Func##_UserFullInfoDelegate.ExecuteIfBound(UserFullInfos);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
}

UCLASS()
class TENCENTIMLINK_API UTencentIMMacros : public UObject
{
	GENERATED_BODY()
};
