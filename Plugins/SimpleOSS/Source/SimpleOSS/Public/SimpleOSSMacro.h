// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once 

#include "SimpleOSSLog.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsPlatformMisc.h"
#else
#if PLATFORM_LINUX
#include "Lumin/LuminPlatformMisc.h"
#else
#if PLATFORM_MAC
#include "Mac/MacPlatformMisc.h"
#else
#if PLATFORM_IOS
#include "IOS/IOSPlatformMisc.h"
#else
#if PLATFORM_ANDROID
#include "Android/AndroidPlatformMisc.h"
#else
#include "GenericPlatform/GenericPlatformFile.h"
#endif
#endif
#endif
#endif
#endif

#define SIMPLEOSS_MACRO_ERROR(Outcome) \
AuxiliaryTools.Print(Outcome##.error().Code()); \
AuxiliaryTools.Print(Outcome##.error().Message()); \
AuxiliaryTools.Print(Outcome##.error().RequestId()); 

#define SIMPLEOSS_MACRO_TYPE1(Request) \
SIMPLEOSS_MACRO_META(Request) \
SIMPLEOSS_MACRO_PROGRESSCALLBACK(Request)

#define SIMPLEOSS_MACRO_META(Request) \
/*Set HTTP header*/  \
ObjectMetaData meta; \
{ \
	if (!OSSObjectMeta.ContentType.IsEmpty())\
	{\
		meta.setContentType(TCHAR_TO_UTF8(*OSSObjectMeta.ContentType)); \
	}\
	if (!OSSObjectMeta.CacheControl.IsEmpty()) \
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.CacheControl));\
	}\
	if (!OSSObjectMeta.ContentDisposition.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentDisposition));\
	}\
	if (!OSSObjectMeta.ContentEncoding.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentEncoding));\
	}\
	if (!OSSObjectMeta.ContentMd5.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentMd5));\
	}\
}\
/*user-defined metadata */ \
for (auto &Tmp : CustomMeta)\
{\
	meta.UserMetaData()[TCHAR_TO_UTF8(*Tmp.Key)] = TCHAR_TO_UTF8(*Tmp.Value);\
}\
Request.MetaData() = meta; 

#define SIMPLEOSS_MACRO_PROGRESSCALLBACK(Request) \
TransferProgress progressCallback = { InProgressCallback , nullptr }; \
Request.setTransferProgress(progressCallback);

#define CREATE_CONFIGURATION_AND_STORAGE \
/* Memory pool (pool) for memory management, equivalent to APR pool. In fact, modern code is in Apr Library 。*/ \
aos_pool_t *pool;  \
/* Recreate a memory pool. The second parameter is null, indicating that no other memory pool is inherited 。*/ \
aos_pool_create(&pool, NULL); \
/* Create and initialize options. This parameter includes global configuration information such as endpoint, access ﹣ key ﹣ ID, access ﹣ key ﹣ secret, is ﹣ CNAME, curl, etc 。*/ \
oss_request_options_t *oss_client_options; \
/* Allocate memory to options in memory pool 。*/ \
oss_client_options = oss_request_options_create(pool); \
/*Initialize the client's options OSS client options. */ \
init_options(oss_client_options);

#define STRING_TO_AOS_STRING_T(ObjectName,InObjectName) \
aos_string_t ObjectName;\
aos_str_null(&ObjectName); \
ANSICHAR* ObjectName##_name = TCHAR_TO_UTF8(*InObjectName); \
/*Assign data of type char * to the storage space of type AOS ﹣ string ﹣ t */ \
aos_str_set(&ObjectName, ObjectName##_name);

#define UTF8_STRING_TO_AOS_STRING_T(ObjectName,InObjectName) \
aos_string_t ObjectName;\
ANSICHAR* InObjectName##_Char = TCHAR_TO_UTF8(*InObjectName);\
/*Utf8 code the multi byte Chinese name.*/ \
ANSICHAR InObjectName##buf[1024];\
ANSICHAR *Tmp_##ObjectName = multibyte_to_utf8(InObjectName##_Char, InObjectName##buf, 1024);\
/*Assign data of type char * to the storage space of type AOS ﹣ string ﹣ t */  \
aos_str_set(&ObjectName, Tmp_##ObjectName);

#define STATEMENT_VISITSUCCESS \
bool bVisitSuccess = false;

#define AOS_POOL_DESTROY_IT(pool) \
aos_pool_destroy(pool);

#define OSS_SDK_RESPONSE_MSG \
if (bVisitSuccess) \
{\
	OSS_SUCCESS_MSG("ErrorCode[%i] ErrorCodeChar[%s] ReqID[%s] ErrorMsg[%s]  ", resp_status->code, UTF8_TO_TCHAR(resp_status->error_code),UTF8_TO_TCHAR(resp_status->error_msg), UTF8_TO_TCHAR(resp_status->req_id))\
}\
else \
{\
	OSS_ERROR_MSG("ErrorCode[%i] ErrorCodeChar[%s] ReqID[%s] ErrorMsg[%s]  ", resp_status->code, UTF8_TO_TCHAR(resp_status->error_code), UTF8_TO_TCHAR(resp_status->error_msg), UTF8_TO_TCHAR(resp_status->req_id))\
}

#define CHECK_OSS_RESULT(Code_True,Code_False) \
STATEMENT_VISITSUCCESS \
if (aos_status_is_ok(resp_status)){bVisitSuccess = true; Code_True }\
else {bVisitSuccess = false; Code_False} \
AOS_POOL_DESTROY_IT(pool); 

#define CHECK_OSS_RESULT_NOBOOL(Code_True,Code_False) \
if (aos_status_is_ok(resp_status)){bVisitSuccess = true; Code_True }\
else {bVisitSuccess = false; Code_False} 

#define RETURN_FALSE \
return false;

#define PRING_LOG(Log) TEXT(Log); 

#define RETURN_TRUE \
return true;

#define DECLARATION_CALLBACK_DELEGATE(Func) \
FProgressCallbackDelegate Func##_Delegate; \
void Func##_Local(int64 ConsumedBytes, int64 TotalBytes) \
{ \
	FScopeLock ScopeLock(&OSSMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([ConsumedBytes,TotalBytes]()\
	{\
		Func##_Delegate.ExecuteIfBound(ConsumedBytes, TotalBytes);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}
#define DECLARATION_1_CALLBACK_DELEGATE(Func,DelegateName) \
FBufferByOSSCallbackDelegate Func##DelegateName; \
void Func##_##DelegateName(const FString &InBuffer) \
{ \
	FScopeLock ScopeLock(&OSSMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([InBuffer]()\
	{\
		Func##DelegateName.ExecuteIfBound(InBuffer);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define DECLARATION_2_CALLBACK_DELEGATE(Func,DelegateName) \
FBytesBufferByOSSCallbackDelegate Func##DelegateName; \
void Func##_##DelegateName(const ANSICHAR* InBuffer,int32 InSize) \
{ \
	FScopeLock ScopeLock(&OSSMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([InBuffer,InSize]()\
	{\
	   TArray<uint8> OutData((uint8*)InBuffer, InSize);\
	   Func##DelegateName.ExecuteIfBound(OutData);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define DECLARATION_END_CALLBACK_DELEGATE(Func) \
DECLARATION_1_CALLBACK_DELEGATE(Func,_End_Delegate)

#define DECLARATION_END_BYTE_CALLBACK_DELEGATE(Func) \
DECLARATION_2_CALLBACK_DELEGATE(Func,_End_Delegate)

#define DECLARATION_START_CALLBACK_DELEGATE(Func) \
DECLARATION_1_CALLBACK_DELEGATE(Func,_Start_Delegate)

#define DECLARATION_CALLBACK_DELEGATE_PART(Func) \
FCallbackUploadPartDelegate Func##_Delegate; \
void Func##_Local(FString UploadId, int64  PartCount, int64 Size, int64 InTotal, bool bPart, bool  bComplete) \
{ \
	FScopeLock ScopeLock(&OSSMutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([UploadId,PartCount,Size,InTotal,bPart,bComplete]()\
	{\
		Func##_Delegate.ExecuteIfBound(UploadId, PartCount,Size,InTotal,bPart,bComplete);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define OSS_REGISTRATION_CONFIGURATION \
IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile(); \
FString AccountsSettingsPaths = FPaths::ProjectConfigDir() / TEXT("OSSLoaclAccountsSettings.ini"); \
if (!FPaths::FileExists(AccountsSettingsPaths))\
{\
	if (!FPaths::DirectoryExists(FPaths::ProjectConfigDir()))\
	{\
		if (!PlatformFile.CreateDirectoryTree(*FPaths::ProjectConfigDir()))\
		{\
			return;\
		}\
	}\
	FString StringContent = " ";\
	FFileHelper::SaveStringToFile(StringContent, *AccountsSettingsPaths, FFileHelper::EEncodingOptions::ForceUTF8); \
}

#define CLAIM_METADATA(OSSMeta) \
std::map<std::string, std::string> _OSSMetaMap_;\
for (auto& Tmp : OSSMeta)\
{\
	_OSSMetaMap_.insert(std::pair<std::string, std::string>(TCHAR_TO_UTF8(*Tmp.Key), TCHAR_TO_UTF8(*Tmp.Value)));\
}\
if (_OSSMetaMap_##.size()) \
{\
	headers = aos_table_make(pool, _OSSMetaMap_##.size()); \
	for (auto &Tmp : _OSSMetaMap_) \
	{ \
		apr_table_set(headers,Tmp.first.c_str(),Tmp.second.c_str()); \
	}\
}

#define OSS_INFO_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("%s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_DEBUG_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[DEBUG] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREENDE_DEBUG_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[SUCCESS] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, Msg); \
}

#define OSS_SUCCESS_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_GREEN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[SUCCESS] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREENDE_SUCCESS_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_GREEN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[SUCCESS] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, Msg); \
}

#define OSS_WARNING_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_YELLOW); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Warning, TEXT("[WARNING] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREEN_WARNING_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_YELLOW); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	const FString NewMsg = FString::Printf(TEXT("[WARNING] %s"), *Msg); \
	UE_LOG(LogSimpleOSS, Warning, TEXT("%s"), *NewMsg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, NewMsg); \
}

#define OSS_ERROR_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_RED); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Error, TEXT("[ERROR] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREEN_ERROR_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_RED); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	const FString NewMsg = FString::Printf(TEXT("[ERROR] %s"), *Msg); \
	UE_LOG(LogSimpleOSS, Error, TEXT("%s"), *NewMsg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, NewMsg); \
}

#define OSS_SCREENDE_MESSAGE(Format, ...) \
{ \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, Msg); \
}