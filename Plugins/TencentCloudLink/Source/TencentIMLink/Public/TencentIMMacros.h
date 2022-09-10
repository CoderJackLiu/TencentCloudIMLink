// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TencentDataType.h"
#include "TencentIMMacros.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE(FIMCallbackDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMCallbackTextDelegate,FString,Text);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FIMFailureCallback, int, ErrorCode, FString, ErrorMassage);
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMProgressCallback, uint32, Progress);
DECLARE_DYNAMIC_DELEGATE_OneParam(FIMUserFullInfoCallback, TArray<FTIMUserFullInfo>, UserInfos);

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
