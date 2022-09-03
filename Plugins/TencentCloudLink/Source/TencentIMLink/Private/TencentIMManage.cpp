#include "TencentIMManage.h"

#include "Engine/Engine.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include <string>
#include <stdlib.h>
#include "DebugDefs.h"

#include <memory>
#include <fstream>
#include "Async/AsyncWork.h"
#include "Misc/ScopeLock.h"
#include "Async/TaskGraphInterfaces.h"
#include "CoreGlobals.h"
#include "Misc/ConfigCacheIni.h"

#include <vector>
#include <string>
#include <map>

#include "TencentCallBacks.h"

// #if PLATFORM_WINDOWS
// #pragma optimize("",off) 
// #endif

FTencentIMManage* FTencentIMManage::TencentIM = nullptr;

FTencentIMManage::FTencentIMManage(): IM()
{
}

FTencentIMManage::~FTencentIMManage()
{
}

FTencentIMManage* FTencentIMManage::Get()
{
	if (!TencentIM)
	{
		TencentIM = new FTencentIMManage();
		V2TIMSDKConfig timConfig;
		if (V2TIMManager::GetInstance()->InitSDK(SDKAppID, timConfig))
		{
			exit(1);
		}

		// Tencent_IM.InitLocalAccounts();
	}

	return TencentIM;
}

void FTencentIMManage::Destroy()
{
	if (TencentIM)
	{
		//Save cofig
		// TencentIM->GetIM().SaveAccounts();

		V2TIMManager::GetInstance()->UnInitSDK();
		delete TencentIM;
	}

	TencentIM = nullptr;
}

FString FTencentIMManage::FTencentIM::GetSDKVersion()
{
	return "";
}

int64 FTencentIMManage::FTencentIM::GetServerTime()
{
	return 0;
}

V2TIMManager* FTencentIMManage::FTencentIM::GetInstance()
{
	return V2TIMManager::GetInstance();
}

void FTencentIMManage::FTencentIM::InitAccounts(const FString& InUserId, const FString& InIMUserSigId)
{
}

void FTencentIMManage::FTencentIM::LogIn(const FString& InUserId, const FString& InIMUserSigId)
{
	class LoginCallback : public V2TIMCallback
	{
	public:
		LoginCallback()
		{
		};

		~LoginCallback()
		{
		};

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login OnSuccess"));
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
		};
	};
	LoginCallback* login_callback_ = new LoginCallback();
	const char* imTestUserId = TCHAR_TO_ANSI(*InUserId);
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
		jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
		jstring jsUserId = Env->NewStringUTF(imTestUserId);
		jstring jsKey = Env->NewStringUTF(SECRETKEY);
		jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
		FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
		auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
		const char* userSig = twoHundredAnsi.Get();
		timInstance->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), login_callback_);
	}
#else
	const char* userSig = GenerateTestUserSig().genTestUserSig(imTestUserId, SDKAppID, SECRETKEY);
	FString Test = userSig;
	UE_LOG(LogTemp, Warning, TEXT("UserID::  %s"), *Test);

	GetInstance()->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), login_callback_);
#endif
}

void FTencentIMManage::FTencentIM::LogOut()
{
	class LogOutCallback : public V2TIMCallback
	{
	public:
		LogOutCallback()
		{
		};

		~LogOutCallback()
		{
		};

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut OnSuccess"));
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
		};
	};
	LogOutCallback* LogOut_callback_ = new LogOutCallback();
	GetInstance()->Logout(LogOut_callback_);
}

FString FTencentIMManage::FTencentIM::GetLoginUser()
{
	const std::string TempUserId(GetInstance()->GetLoginUser().CString());
	FString OutUserName = TempUserId.c_str();
	return OutUserName;
}

V2TIMLoginStatus FTencentIMManage::FTencentIM::GetLoginStatus()
{
	//todo return type
	return GetInstance()->GetLoginStatus();
}

void FTencentIMManage::FTencentIM::AddSimpleMsgListener(V2TIMSimpleMsgListener* listener)
{
	GetInstance()->AddSimpleMsgListener(listener);
}

void FTencentIMManage::FTencentIM::RemoveSimpleMsgListener(V2TIMSimpleMsgListener* listener)
{
	GetInstance()->RemoveSimpleMsgListener(listener);
}


SendCallback SendCallbackIns;
FString FTencentIMManage::FTencentIM::SendC2CTextMessage(FString text, FString userId)
{
	return ToFString(GetInstance()->SendC2CTextMessage(ToIMString(text), ToIMString(userId), &SendCallbackIns));
}

FString FTencentIMManage::FTencentIM::SendC2CCustomMessage(const V2TIMBuffer& customData, const FString& userID)
{
	//todo 返回代理
	return ToFString(GetInstance()->SendC2CCustomMessage(customData, ToIMString(userID), nullptr));
}

FString FTencentIMManage::FTencentIM::SendGroupTextMessage(const FString& text, const FString& groupID, V2TIMMessagePriority priority)
{
	SendCallback callback;
	return ToFString(GetInstance()->SendGroupTextMessage(ToIMString(text),ToIMString(groupID),priority,&callback));
}

FString FTencentIMManage::FTencentIM::SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, V2TIMMessagePriority priority)
{
	SendCallback callback;
	return ToFString(GetInstance()->SendGroupCustomMessage(customData ,ToIMString(groupID),priority,&callback));
}

void FTencentIMManage::FTencentIM::AddGroupListener(V2TIMGroupListener* listener)
{
	
}

V2TIMString FTencentIMManage::FTencentIM::ToIMString(const FString& InStr) const
{
	const char* OutIMString = TCHAR_TO_ANSI(*InStr);
	return static_cast<V2TIMString>(OutIMString);
}

FString FTencentIMManage::FTencentIM::ToFString(const V2TIMString& InStr) const
{
	const std::string tempStr=InStr.CString();
	return tempStr.c_str();
}



