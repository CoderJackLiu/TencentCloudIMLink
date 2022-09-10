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
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMCallbackTextDelegate,FString,Text);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMessage_ArrayCallbackTextDelegate,const TArray<FTIMReceiveMessageOptInfo>&,Messages);

//userinfo
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMUserFullInfoCallback, const TArray<FTIMUserFullInfo>&, UserInfos);

//groupInfos
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMGroupInfoArrayCallback, const TArray<FTIMGroupInfo>&, GroupInfos);


//
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMGroupMessageInfoCallback, const TArray<FTIMMessage>& , Messages);

DECLARE_DYNAMIC_DELEGATE_OneParam(FMessageSearchResultCallback, const FTIMMessageSearchResult& , Result);

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
