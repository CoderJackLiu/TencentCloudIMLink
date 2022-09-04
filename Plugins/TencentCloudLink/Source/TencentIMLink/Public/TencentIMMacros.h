// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TencentIMMacros.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE(FIMCallbackDelegate);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FIMFailureCallbackDelegate, int, ErrorCode, FString, CodeStr);

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

#define DECLARATION_FAILURE_CALLBACK_DELEGATE(Func) \
FIMFailureCallbackDelegate Func##_Delegate2; \
void Func##_Local2(int Code,const FString& Message) \
{ \
	FScopeLock ScopeLock(&TencentMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([Code,Message]()\
	{\
	Func##_Delegate2.ExecuteIfBound(Code, Message);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
}

UCLASS()
class TENCENTIMLINK_API UTencentIMMacros : public UObject
{
	GENERATED_BODY()
};
