// Fill out your copyright notice in the Description page of Project Settings.


#include "TencentIMLink/Public/TencentIMLibrary.h"

#include "TencentIMManage.h"

FCriticalSection TencentMutex;

FString UTencentIMLibrary::GetSDKVersion()
{
	return Tencent_IM.GetSDKVersion();
}

int64 UTencentIMLibrary::GetServerTime()
{
	return Tencent_IM.GetServerTime();
}

void UTencentIMLibrary::LogIn(const FString& InUserId, const FString& InIMUserSigId)
{
	Tencent_IM.LogIn(InUserId, InIMUserSigId);
}

void UTencentIMLibrary::LogOut()
{
	Tencent_IM.LogOut();
}

FString UTencentIMLibrary::GetLoginUser()
{
	return Tencent_IM.GetLoginUser();
}

FString UTencentIMLibrary::SendC2CTextMessage(FString text, FString userId)
{
	return Tencent_IM.SendC2CTextMessage(text, userId);
}

FString UTencentIMLibrary::SendGroupTextMessage(const FString& text, const FString& groupID, EIMMessagePriority priority)
{
	return Tencent_IM.SendGroupTextMessage(text, groupID, priority);
}

FString UTencentIMLibrary::SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, EIMMessagePriority priority)
{
	return Tencent_IM.SendGroupCustomMessage(customData, groupID, priority);
}


DECLARATION_CALLBACK_DELEGATE(JoinGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(JoinGroup)

void UTencentIMLibrary::JoinGroup(const FString& groupID, const FString& message, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallbackDelegate OnFailureDelegate)
{
	JoinGroup_Delegate = OnSuccessDelegate;
	JoinGroup_Delegate2 = OnFailureDelegate;
	class NormalCallback : public V2TIMCallback
	{
	public:
		NormalCallback()
		{
		}

		~NormalCallback() override
		{
		}

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login OnSuccess"));
			JoinGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			JoinGroup_Delegate2.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();

	Tencent_IM.JoinGroup(groupID, message, Callback);
}

DECLARATION_CALLBACK_DELEGATE(QuitGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(QuitGroup)
void UTencentIMLibrary::QuitGroup(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallbackDelegate OnFailureDelegate)
{
	QuitGroup_Delegate = OnSuccessDelegate;
	QuitGroup_Delegate2 = OnFailureDelegate;
	class NormalCallback : public V2TIMCallback
	{
	public:
		NormalCallback()
		{
		}

		~NormalCallback() override
		{
		}

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login OnSuccess"));
			// QuitGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			// OnFailureDelegate.ExecuteIfBound(error_code,TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();

	Tencent_IM.QuitGroup(groupID, Callback);
}


ELoginStatus UTencentIMLibrary::GetLoginStatus()
{
	return Tencent_IM.GetLoginStatus();
}
