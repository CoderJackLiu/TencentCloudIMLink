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

DECLARATION_CALLBACK_DELEGATE(LogOut)
DECLARATION_FAILURE_CALLBACK_DELEGATE(LogOut)

void UTencentIMLibrary::LogOut(FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	LogOut_Delegate = OnSuccessDelegate;
	LogOut_FailureDelegate = OnFailureDelegate;
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
			LogOut_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
			LogOut_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};
	LogOutCallback* LogOut_callback_ = new LogOutCallback();
	Tencent_IM.GetInstance()->Logout(LogOut_callback_);
}

FString UTencentIMLibrary::GetLoginUser()
{
	return Tencent_IM.GetLoginUser();
}

DECLARATION_CALLBACK_DELEGATE(SendC2CTextMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SendC2CTextMessage)
DECLARATION_Progress_CALLBACK_DELEGATE(SendC2CTextMessage)

FString UTencentIMLibrary::SendC2CTextMessage(FString text, FString userId, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate,
                                              FIMProgressCallback OnProgressDelegate)
{
	SendC2CTextMessage_Delegate = OnSuccessDelegate;
	SendC2CTextMessage_FailureDelegate = OnFailureDelegate;
	SendC2CTextMessage_ProgressDelegate = OnProgressDelegate;
	class LogOutCallback : public V2TIMSendCallback
	{
	public:
		LogOutCallback()
		{
		};

		~LogOutCallback()
		{
		};

		void OnSuccess(const V2TIMMessage& InStr) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage OnSuccess"));
			LogOut_Delegate.ExecuteIfBound();
		};

		void OnProgress(uint32_t progress) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage progress"));
			SendC2CTextMessage_ProgressDelegate.ExecuteIfBound(progress);
		}

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
			SendC2CTextMessage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};

	LogOutCallback* LogOut_callback_ = new LogOutCallback();
	return ToFString(Tencent_IM.GetInstance()->SendC2CTextMessage(ToIMString(text), ToIMString(userId), LogOut_callback_));
}

FString UTencentIMLibrary::SendGroupTextMessage(const FString& text, const FString& groupID, EIMMessagePriority priority)
{
	// return Tencent_IM.SendGroupTextMessage(text, groupID, priority);
	return "";
}

FString UTencentIMLibrary::SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, EIMMessagePriority priority)
{
	return "";
}


DECLARATION_CALLBACK_DELEGATE(JoinGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(JoinGroup)

void UTencentIMLibrary::JoinGroup(const FString& groupID, const FString& message, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	JoinGroup_Delegate = OnSuccessDelegate;
	JoinGroup_FailureDelegate = OnFailureDelegate;
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
			JoinGroup_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();

	Tencent_IM.JoinGroup(groupID, message, Callback);
}

DECLARATION_CALLBACK_DELEGATE(QuitGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(QuitGroup)

void UTencentIMLibrary::QuitGroup(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	QuitGroup_Delegate = OnSuccessDelegate;
	QuitGroup_FailureDelegate = OnFailureDelegate;
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
			QuitGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			QuitGroup_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();

	Tencent_IM.QuitGroup(groupID, Callback);
}

V2TIMString UTencentIMLibrary::ToIMString(const FString& InStr)
{
	const char* OutIMString = TCHAR_TO_ANSI(*InStr);
	return static_cast<V2TIMString>(OutIMString);
}

FString UTencentIMLibrary::ToFString(const V2TIMString& InStr)
{
	const std::string tempStr = InStr.CString();
	return tempStr.c_str();
}

V2TIMStringVector UTencentIMLibrary::ToIMStringArray(TArray<FString> InStrArray)
{
	V2TIMStringVector StrVector;
	for (FString Str : InStrArray)
	{
		StrVector.PushBack(ToIMString(Str));
	}
	return StrVector;
}

ELoginStatus UTencentIMLibrary::GetLoginStatus(const V2TIMLoginStatus& Status)
{
	// FString AclString = UTF8_TO_TCHAR(ACLChar);
	if (Status == V2TIM_STATUS_LOGINED)
	{
		return ELoginStatus::V2TIM_STATUS_LOGINED;
	}
	else if (Status == V2TIM_STATUS_LOGINING)
	{
		return ELoginStatus::V2TIM_STATUS_LOGINING;
	}
	else if (Status == V2TIM_STATUS_LOGOUT)
	{
		return ELoginStatus::V2TIM_STATUS_LOGOUT;
	}

	return ELoginStatus::V2TIM_STATUS_LOGOUT;
}

V2TIMMessagePriority UTencentIMLibrary::GetMessagePriority(EIMMessagePriority InPriority)
{
	V2TIMMessagePriority Priority = V2TIM_PRIORITY_DEFAULT;
	switch (InPriority)
	{
	case EIMMessagePriority::V2TIM_PRIORITY_DEFAULT:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_DEFAULT;
		break;
	case EIMMessagePriority::V2TIM_PRIORITY_HIGH:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_HIGH;
		break;
	case EIMMessagePriority::V2TIM_PRIORITY_NORMAL:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_NORMAL;
		break;
	case EIMMessagePriority::V2TIM_PRIORITY_LOW:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_LOW;
		break;
	}
	return Priority;
}


ELoginStatus UTencentIMLibrary::GetLoginStatus()
{
	return Tencent_IM.GetLoginStatus();
}
