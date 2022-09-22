// Fill out your copyright notice in the Description page of Project Settings.


#include "TencentIMLink/Public/TencentIMLibrary.h"

#include "DebugDefs.h"
#include "TencentIMManage.h"
#include "Engine/Engine.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include <string>
#include <stdlib.h>

FCriticalSection TencentMutex;

FString UTencentIMLibrary::GetSDKVersion()
{
	return Tencent_IM.GetSDKVersion();
}

int64 UTencentIMLibrary::GetServerTime()
{
	return Tencent_IM.GetServerTime();
}

DECLARATION_CALLBACK_DELEGATE(LogIn)
DECLARATION_FAILURE_CALLBACK_DELEGATE(LogIn)

void UTencentIMLibrary::LogIn(const FString& InUserId, const FString& InIMUserSigId, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	LogIn_Delegate = OnSuccessDelegate;
	LogIn_FailureDelegate = OnFailureDelegate;
	class LogInCallback : public V2TIMCallback
	{
	public:
		LogInCallback()
		{
		};

		~LogInCallback()
		{
		};

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut OnSuccess"));
			LogIn_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
			LogIn_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};
	LogInCallback* LogIn_callback_ = new LogInCallback();
	// Tencent_IM.GetInstance()->Login(ToIMString(InUserId), ToIMString(InIMUserSigId),LogOut_callback_);
	FString guidUser = InUserId;
	// guidUser="10057992";
	// https://answers.unrealengine.com/questions/49838/fguid-doesnt-create-unique-guids-on-os-x.html
#if PLATFORM_MAC || PLATFORM_IOS
	int r = rand() + rand();
	std::string sRand = std::to_string(r);
	const char* imTestUserId = sRand.c_str();
#else

	const char* imTestUserId = TCHAR_TO_ANSI(*guidUser);
#endif

#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv()) {
		jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
		jstring jsUserId = Env->NewStringUTF(imTestUserId);
		jstring jsKey = Env->NewStringUTF(SECRETKEY);
		jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
		FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
		auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
		const char* userSig = twoHundredAnsi.Get();
		Tencent_IM.GetInstance()->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), LogIn_callback_);
	}
#else
	const char* userSig = GenerateTestUserSig().genTestUserSig(imTestUserId, SDKAppID, SECRETKEY);
	FString Test = userSig;
	UE_LOG(LogTemp, Warning, TEXT("UserID::  %s"), *Test);
	Tencent_IM.GetInstance()->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), LogIn_callback_);
#endif
	// Tencent_IM.LogIn(InUserId, InIMUserSigId);
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
	const std::string TempUserId(Tencent_IM.GetInstance()->GetInstance()->GetLoginUser().CString());
	FString OutUserName = TempUserId.c_str();
	return OutUserName;
}

DECLARATION_FAILURE_CALLBACK_DELEGATE(SendC2CTextMessage)
DECLARATION_Progress_CALLBACK_DELEGATE(SendC2CTextMessage)
DECLARATION_MessageCALLBACK_DELEGATE(SendC2CTextMessage)

FString UTencentIMLibrary::SendC2CTextMessage(FString text, FString userId, FIMMessageInfoCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate,
                                              FIMProgressCallback OnProgressDelegate)
{
	SendC2CTextMessage_MessageDelegate = OnSuccessDelegate;
	SendC2CTextMessage_FailureDelegate = OnFailureDelegate;
	SendC2CTextMessage_ProgressDelegate = OnProgressDelegate;
	class SendC2CTextMessageCallback : public V2TIMSendCallback
	{
	public:
		SendC2CTextMessageCallback()
		{
		};

		~SendC2CTextMessageCallback()
		{
		};

		void OnSuccess(const V2TIMMessage& InStr) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage OnSuccess"));
			// SendC2CTextMessage_Delegate.ExecuteIfBound();
			SendC2CTextMessage_MessageDelegate.ExecuteIfBound(ToMessage(InStr));
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

	SendC2CTextMessageCallback* SendC2CTextMessage_Callback = new SendC2CTextMessageCallback();
	return ToFString(Tencent_IM.GetInstance()->SendC2CTextMessage(ToIMString(text), ToIMString(userId), SendC2CTextMessage_Callback));
}

DECLARATION_MessageCALLBACK_DELEGATE(SendC2CCustomMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SendC2CCustomMessage)
DECLARATION_Progress_CALLBACK_DELEGATE(SendC2CCustomMessage)

FString UTencentIMLibrary::SendC2CCustomMessage(const FBuffer& customData, const FString& userID, FIMMessageInfoCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate,
                                                FIMProgressCallback OnProgressDelegate)
{
	SendC2CCustomMessage_MessageDelegate = OnSuccessDelegate;
	SendC2CCustomMessage_FailureDelegate = OnFailureDelegate;
	SendC2CCustomMessage_ProgressDelegate = OnProgressDelegate;

	class SendCustomCallback : public V2TIMSendCallback
	{
	public:
		SendCustomCallback()
		{
		};

		~SendCustomCallback()
		{
		};

		void OnSuccess(const V2TIMMessage& InStr) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage OnSuccess"));
			SendC2CCustomMessage_MessageDelegate.ExecuteIfBound(ToMessage(InStr));
		};

		void OnProgress(uint32_t progress) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage progress"));
			SendC2CCustomMessage_ProgressDelegate.ExecuteIfBound(progress);
		}

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
			SendC2CCustomMessage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};

	SendCustomCallback* SendCustom_callback_ = new SendCustomCallback();
	return ToFString(Tencent_IM.GetInstance()->SendC2CCustomMessage(ToTIMBuffer(customData), ToIMString(userID), SendCustom_callback_));
}

DECLARATION_MessageCALLBACK_DELEGATE(SendGroupTextMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SendGroupTextMessage)
DECLARATION_Progress_CALLBACK_DELEGATE(SendGroupTextMessage)

FString UTencentIMLibrary::SendGroupTextMessage(const FString& text, const FString& groupID, EIMMessagePriority priority, FIMMessageInfoCallback OnSuccessDelegate,
                                                FIMFailureCallback OnFailureDelegate,
                                                FIMProgressCallback OnProgressDelegate)
{
	SendGroupTextMessage_MessageDelegate = OnSuccessDelegate;
	SendGroupTextMessage_FailureDelegate = OnFailureDelegate;
	SendGroupTextMessage_ProgressDelegate = OnProgressDelegate;
	class SendGroupTextMessageCallback : public V2TIMSendCallback
	{
	public:
		SendGroupTextMessageCallback()
		{
		};

		~SendGroupTextMessageCallback()
		{
		};

		void OnSuccess(const V2TIMMessage& InStr) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendGroupTextMessage OnSuccess"));
			SendGroupTextMessage_MessageDelegate.ExecuteIfBound(ToMessage(InStr));
		};

		void OnProgress(uint32_t progress) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendGroupTextMessage progress"));
			SendGroupTextMessage_ProgressDelegate.ExecuteIfBound(progress);
		}

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendGroupTextMessage failed OnError ======: %d"), error_code);
			SendGroupTextMessage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};
	SendGroupTextMessageCallback* SendGroupTextMessage_CallBack = new SendGroupTextMessageCallback();
	return ToFString(Tencent_IM.GetInstance()->SendGroupTextMessage(ToIMString(text), ToIMString(groupID), GetMessagePriority(priority), SendGroupTextMessage_CallBack));
}

DECLARATION_MessageCALLBACK_DELEGATE(SendGroupCustomMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SendGroupCustomMessage)
DECLARATION_Progress_CALLBACK_DELEGATE(SendGroupCustomMessage)

FString UTencentIMLibrary::SendGroupCustomMessage(const FBuffer& customData, const FString& groupID, EIMMessagePriority priority, FIMMessageInfoCallback OnSuccessDelegate,
                                                  FIMFailureCallback OnFailureDelegate,
                                                  FIMProgressCallback OnProgressDelegate)
{
	SendGroupCustomMessage_MessageDelegate = OnSuccessDelegate;
	SendGroupCustomMessage_FailureDelegate = OnFailureDelegate;
	SendGroupCustomMessage_ProgressDelegate = OnProgressDelegate;

	class SendMessageCallback : public V2TIMSendCallback
	{
	public:
		SendMessageCallback()
		{
		};


		~SendMessageCallback()
		{
		};

		void OnSuccess(const V2TIMMessage& InMessage) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage OnSuccess"));
			SendGroupCustomMessage_MessageDelegate.ExecuteIfBound(ToMessage(InMessage));
		};

		void OnProgress(uint32_t progress) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage progress"));
			SendGroupCustomMessage_ProgressDelegate.ExecuteIfBound(progress);
		}

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
			SendGroupCustomMessage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};
	SendMessageCallback* CallBack = new SendMessageCallback();
	return ToFString(Tencent_IM.GetInstance()->SendGroupCustomMessage(ToTIMBuffer(customData), ToIMString(groupID), GetMessagePriority(priority), CallBack));
}


DECLARATION_CALLBACK_DELEGATE(CreateGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(CreateGroup)

void UTencentIMLibrary::CreateGroup(const FString& groupType, const FString& groupID, const FString& groupName, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	CreateGroup_Delegate = OnSuccessDelegate;
	CreateGroup_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMString>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMString& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			CreateGroup_Delegate.ExecuteIfBound();
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			CreateGroup_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->CreateGroup(ToIMString(groupType), ToIMString(groupID), ToIMString(groupName), CallBack);
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

DECLARATION_CALLBACK_DELEGATE(DismissGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DismissGroup)

void UTencentIMLibrary::DismissGroup(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	DismissGroup_Delegate = OnSuccessDelegate;
	DismissGroup_FailureDelegate = OnFailureDelegate;
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
			DismissGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			DismissGroup_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.QuitGroup(groupID, Callback);
}

DECLARATION_UserInfo_CALLBACK_DELEGATE(GetUsersInfo)

void UTencentIMLibrary::GetUsersInfo(const TArray<FString>& userIDList, FIMUserFullInfoCallback UserInfoDelegate)
{
	GetUsersInfo_UserFullInfoDelegate = UserInfoDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMUserFullInfoVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMUserFullInfoVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			TArray<FTIMUserFullInfo> Test = ToTIMUserFullInfoArray(message);
			GetUsersInfo_UserFullInfoDelegate.ExecuteIfBound(Test);
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			CreateGroup_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetUsersInfo(userIDList, CallBack);
}

DECLARATION_CALLBACK_DELEGATE(SetSelfInfo)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetSelfInfo)

void UTencentIMLibrary::SetSelfInfo(const FTIMUserFullInfo& Info, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetSelfInfo_FailureDelegate = OnFailureDelegate;
	SetSelfInfo_Delegate = OnSuccessDelegate;
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
			SetSelfInfo_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			SetSelfInfo_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->SetSelfInfo(ToV2TIMUserFullInfo(Info), Callback);
}


FTIMMessage UTencentIMLibrary::CreateTextMessage(const FString& text)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateTextMessage(ToIMString(text)));
}

FTIMMessage UTencentIMLibrary::CreateTextAtMessage(const FString& text, const TArray<FString>& atUserList)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateTextAtMessage(ToIMString(text), ToIMStringVector(atUserList)));
}

FTIMMessage UTencentIMLibrary::CreateCustomMessage(const FBuffer& data)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateCustomMessage(ToTIMBuffer(data)));
}

FTIMMessage UTencentIMLibrary::CreateCustomMessage_MultiPara(const FBuffer& data, const FString& description, const FString& extension)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateCustomMessage(ToTIMBuffer(data), ToIMString(description), ToIMString(extension)));
}

FTIMMessage UTencentIMLibrary::CreateImageMessage(const FString& imagePath)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateImageMessage(ToIMString(imagePath)));
}

FTIMMessage UTencentIMLibrary::CreateSoundMessage(const FString& soundPath, int32 duration)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateSoundMessage(ToIMString(soundPath), FMath::Clamp(duration, 0, 2147483647)));
}

FTIMMessage UTencentIMLibrary::CreateVideoMessage(const FString& videoFilePath, const FString& type, int32 duration, const FString& snapshotPath)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateVideoMessage(ToIMString(videoFilePath), ToIMString(type), FMath::Clamp(duration, 0, 2147483647),
	                                                                                   ToIMString(snapshotPath)));
}

FTIMMessage UTencentIMLibrary::CreateFileMessage(const FString& filePath, const FString& fileName)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateFileMessage(ToIMString(filePath), ToIMString(fileName)));
}

FTIMMessage UTencentIMLibrary::CreateLocationMessage(const FString& desc, double longitude, double latitude)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateLocationMessage(ToIMString(desc), longitude, latitude));
}

FTIMMessage UTencentIMLibrary::CreateFaceMessage(int32 index, const FBuffer& data)
{
	ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateFaceMessage(index, ToTIMBuffer(data)));
	return FTIMMessage();
}

FTIMMessage UTencentIMLibrary::CreateMergerMessage(const TArray<FTIMMessage>& messageList, const FString& title, const TArray<FString>& abstractList, const FString& compatibleText)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateMergerMessage(ToV2IMMessageArray(messageList), ToIMString(title), ToIMStringVector(abstractList),
	                                                                                    ToIMString(compatibleText)));
}

FTIMMessage UTencentIMLibrary::CreateForwardMessage(const FTIMMessage& message)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateForwardMessage(ToIMMessage(message)));
}

DECLARATION_MessageCALLBACK_DELEGATE(SendMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SendMessage)
DECLARATION_Progress_CALLBACK_DELEGATE(SendMessage)

FString UTencentIMLibrary::SendMessage(const FTIMMessage& message, const FString& receiver, const FString& groupID, EIMMessagePriority priority, bool onlineUserOnly,
                                       const FTIMOfflinePushInfo& offlinePushInfo, FIMMessageInfoCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate,
                                       FIMProgressCallback OnProgressDelegate)
{
	SendMessage_MessageDelegate = OnSuccessDelegate;
	SendMessage_FailureDelegate = OnFailureDelegate;
	SendMessage_ProgressDelegate = OnProgressDelegate;
	class SendMessageCallback : public V2TIMSendCallback
	{
	public:
		SendMessageCallback()
		{
		};


		~SendMessageCallback()
		{
		};

		void OnSuccess(const V2TIMMessage& InMessage) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage OnSuccess"));
			SendMessage_MessageDelegate.ExecuteIfBound(ToMessage(InMessage));
		};

		void OnProgress(uint32_t progress) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SendC2CTextMessage progress"));
			SendMessage_ProgressDelegate.ExecuteIfBound(progress);
		}

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
			SendMessage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};
	V2TIMMessage TimMessage = ToIMMessage(message);
	SendMessageCallback* SendMessage_Callback = new SendMessageCallback();
	return ToFString(Tencent_IM.GetInstance()->GetMessageManager()->SendMessage(
		TimMessage, ToIMString(receiver), ToIMString(groupID), GetMessagePriority(priority), onlineUserOnly, ToTIMOfflinePushInfo(offlinePushInfo), SendMessage_Callback));
}

DECLARATION_CALLBACK_DELEGATE(SetC2CReceiveMessageOpt)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetC2CReceiveMessageOpt)

void UTencentIMLibrary::SetC2CReceiveMessageOpt(const TArray<FString>& userIDList, ETIMReceiveMessageOpt opt, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetC2CReceiveMessageOpt_Delegate = OnSuccessDelegate;
	SetC2CReceiveMessageOpt_FailureDelegate = OnFailureDelegate;
	class NormalCallback : public V2TIMCallback
	{
	public:
		NormalCallback()
		{
		};

		~NormalCallback()
		{
		};

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SetC2CReceiveMessageOpt OnSuccess"));
			SetC2CReceiveMessageOpt_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== SetC2CReceiveMessageOpt failed OnError ======: %d"), error_code);
			SetC2CReceiveMessageOpt_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		};
	};
	NormalCallback* Normal_callback_ = new NormalCallback();

	Tencent_IM.GetInstance()->GetMessageManager()->SetC2CReceiveMessageOpt(ToIMStringVector(userIDList), ToTIMReceiveMessageOpt(opt), Normal_callback_);
}

DECLARATION_MessageOptCALLBACK_DELEGATE(GetC2CReceiveMessageOpt)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetC2CReceiveMessageOpt)

void UTencentIMLibrary::GetC2CReceiveMessageOpt(const TArray<FString>& userIDList, FMessage_ArrayCallbackTextDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetC2CReceiveMessageOpt_MessageArrayDelegate = OnSuccessDelegate;
	GetC2CReceiveMessageOpt_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<V2TIMReceiveMessageOptInfoVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMReceiveMessageOptInfoVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetC2CReceiveMessageOpt_MessageArrayDelegate.ExecuteIfBound(ToReceiveMessageOptInfoArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetC2CReceiveMessageOpt_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetMessageManager()->GetC2CReceiveMessageOpt(ToIMStringVector(userIDList), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(SetGroupReceiveMessageOpt)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetGroupReceiveMessageOpt)

void UTencentIMLibrary::SetGroupReceiveMessageOpt(const FString& groupID, ETIMReceiveMessageOpt opt, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetGroupReceiveMessageOpt_Delegate = OnSuccessDelegate;
	SetGroupReceiveMessageOpt_FailureDelegate = OnFailureDelegate;

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

	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->SetGroupReceiveMessageOpt(ToIMString(groupID), ToTIMReceiveMessageOpt(opt), CallBack);
}

DECLARATION_GroupMessageCALLBACK_DELEGATE(GetHistoryMessageList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetHistoryMessageList)

void UTencentIMLibrary::GetHistoryMessageList(const FTIMMessageListGetOption& option, FIMGroupMessageInfoCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetHistoryMessageList_GroupMessageArrayDelegate = OnSuccessDelegate;
	GetHistoryMessageList_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<V2TIMMessageVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMMessageVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetHistoryMessageList_GroupMessageArrayDelegate.ExecuteIfBound(ToMessageArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnError ======"));
			GetHistoryMessageList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetMessageManager()->GetHistoryMessageList(ToIMMessageListGetOption(option), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(RevokeMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(RevokeMessage)

void UTencentIMLibrary::RevokeMessage(const FTIMMessage& message, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	RevokeMessage_Delegate = OnSuccessDelegate;
	RevokeMessage_FailureDelegate = OnFailureDelegate;

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
			RevokeMessage_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			RevokeMessage_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};

	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->RevokeMessage(ToIMMessage(message), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(MarkC2CMessageAsRead)
DECLARATION_FAILURE_CALLBACK_DELEGATE(MarkC2CMessageAsRead)

void UTencentIMLibrary::MarkC2CMessageAsRead(const FString& userID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	MarkC2CMessageAsRead_Delegate = OnSuccessDelegate;
	MarkC2CMessageAsRead_FailureDelegate = OnFailureDelegate;

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
			MarkC2CMessageAsRead_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			MarkC2CMessageAsRead_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};

	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->MarkC2CMessageAsRead(ToIMString(userID), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(MarkGroupMessageAsRead)
DECLARATION_FAILURE_CALLBACK_DELEGATE(MarkGroupMessageAsRead)

void UTencentIMLibrary::MarkGroupMessageAsRead(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	MarkGroupMessageAsRead_Delegate = OnSuccessDelegate;
	MarkGroupMessageAsRead_FailureDelegate = OnFailureDelegate;

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
			MarkGroupMessageAsRead_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			MarkGroupMessageAsRead_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};

	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->MarkGroupMessageAsRead(ToIMString(groupID), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(MarkAllMessageAsRead)
DECLARATION_FAILURE_CALLBACK_DELEGATE(MarkAllMessageAsRead)

void UTencentIMLibrary::MarkAllMessageAsRead(FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	MarkAllMessageAsRead_Delegate = OnSuccessDelegate;
	MarkAllMessageAsRead_FailureDelegate = OnFailureDelegate;

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
			MarkAllMessageAsRead_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			MarkAllMessageAsRead_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};

	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->MarkAllMessageAsRead(CallBack);
}

DECLARATION_CALLBACK_DELEGATE(DeleteMessages)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteMessages)

void UTencentIMLibrary::DeleteMessages(const TArray<FTIMMessage>& messages, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	DeleteMessages_Delegate = OnSuccessDelegate;
	DeleteMessages_FailureDelegate = OnFailureDelegate;
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
			DeleteMessages_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			DeleteMessages_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->DeleteMessages(ToV2IMMessageArray(messages), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(ClearC2CHistoryMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(ClearC2CHistoryMessage)

void UTencentIMLibrary::ClearC2CHistoryMessage(const FString& userID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	ClearC2CHistoryMessage_Delegate = OnSuccessDelegate;
	ClearC2CHistoryMessage_FailureDelegate = OnFailureDelegate;
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
			ClearC2CHistoryMessage_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			ClearC2CHistoryMessage_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->ClearC2CHistoryMessage(ToIMString(userID), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(ClearGroupHistoryMessage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(ClearGroupHistoryMessage)

void UTencentIMLibrary::ClearGroupHistoryMessage(const FString& groupID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	ClearGroupHistoryMessage_Delegate = OnSuccessDelegate;
	ClearGroupHistoryMessage_FailureDelegate = OnFailureDelegate;
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
			ClearGroupHistoryMessage_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			std::string TempStr = error_message.CString();
			ClearGroupHistoryMessage_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* CallBack = new NormalCallback();
	Tencent_IM.GetInstance()->GetMessageManager()->ClearGroupHistoryMessage(ToIMString(groupID), CallBack);
}

DECLARATION_MessageCALLBACK_DELEGATE(InsertGroupMessageToLocalStorage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(InsertGroupMessageToLocalStorage)

FString UTencentIMLibrary::InsertGroupMessageToLocalStorage(FTIMMessage& message, const FString& groupID, const FString& sender, FIMMessageInfoCallback OnSuccessDelegate,
                                                            FIMFailureCallback OnFailureDelegate)
{
	InsertGroupMessageToLocalStorage_MessageDelegate = OnSuccessDelegate;
	InsertGroupMessageToLocalStorage_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMMessage>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMMessage& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			InsertGroupMessageToLocalStorage_MessageDelegate.ExecuteIfBound(ToMessage(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			InsertGroupMessageToLocalStorage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};

	FValueCallBack* CallBack = new FValueCallBack();
	V2TIMMessage OutMessage = ToIMMessage(message);
	return ToFString(Tencent_IM.GetInstance()->GetMessageManager()->InsertGroupMessageToLocalStorage(OutMessage, ToIMString(groupID), ToIMString(sender), CallBack));
}

DECLARATION_MessageCALLBACK_DELEGATE(InsertC2CMessageToLocalStorage)
DECLARATION_FAILURE_CALLBACK_DELEGATE(InsertC2CMessageToLocalStorage)

FString UTencentIMLibrary::InsertC2CMessageToLocalStorage(FTIMMessage& message, const FString& userID, const FString& sender, FIMMessageInfoCallback OnSuccessDelegate,
                                                          FIMFailureCallback OnFailureDelegate)
{
	InsertC2CMessageToLocalStorage_MessageDelegate = OnSuccessDelegate;
	InsertC2CMessageToLocalStorage_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMMessage>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMMessage& InMessage) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			InsertC2CMessageToLocalStorage_MessageDelegate.ExecuteIfBound(ToMessage(InMessage));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			InsertC2CMessageToLocalStorage_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};

	FValueCallBack* CallBack = new FValueCallBack();
	V2TIMMessage OutMessage = ToIMMessage(message);
	return ToFString(Tencent_IM.GetInstance()->GetMessageManager()->InsertGroupMessageToLocalStorage(OutMessage, ToIMString(userID), ToIMString(sender), CallBack));
}

DECLARATION_GroupMessageCALLBACK_DELEGATE(FindMessages)
DECLARATION_FAILURE_CALLBACK_DELEGATE(FindMessages)

void UTencentIMLibrary::FindMessages(const TArray<FString>& messageIDList, FIMGroupMessageInfoCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	FindMessages_GroupMessageArrayDelegate = OnSuccessDelegate;
	FindMessages_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<V2TIMMessageVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMMessageVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			FindMessages_GroupMessageArrayDelegate.ExecuteIfBound(ToMessageArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnError ======"));
			FindMessages_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetMessageManager()->FindMessages(ToIMStringVector(messageIDList), CallBack);
}

DECLARATION_FAILURE_CALLBACK_DELEGATE(SearchLocalMessages)
DECLARATION_MessageSearchResultCALLBACK_DELEGATE(SearchLocalMessages)

void UTencentIMLibrary::SearchLocalMessages(const FTIMMessageSearchParam& searchParam, FMessageSearchResultCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SearchLocalMessages_MsgSearchReaultDelegate = OnSuccessDelegate;
	SearchLocalMessages_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<V2TIMMessageSearchResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMMessageSearchResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			SearchLocalMessages_MsgSearchReaultDelegate.ExecuteIfBound(ToMessageSearchResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnError ======"));
			SearchLocalMessages_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetMessageManager()->SearchLocalMessages(ToTIMessageSearchParam(searchParam), CallBack);
}


DECLARATION_FAILURE_CALLBACK_DELEGATE(CreateGroupByMemBer)
DECLARATION_StringCALLBACK_DELEGATE(CreateGroupByMemBer)

void UTencentIMLibrary::CreateGroupByMemBer(const FTIMGroupInfo& info, const TArray<FTIMCreateGroupMemberInfo>& memberList, FIMCallbackTextDelegate OnSuccessCallback,
                                            FIMFailureCallback OnFailureDelegate)
{
	CreateGroupByMemBer_TextDelegate = OnSuccessCallback;
	CreateGroupByMemBer_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<V2TIMString>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMString& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			CreateGroupByMemBer_TextDelegate.ExecuteIfBound(ToFString(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnError ======"));
			CreateGroupByMemBer_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};

	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetGroupManager()->CreateGroup(ToTIMGroupInfo(info), ToCreateGroupMemberInfoVector(memberList), CallBack);
}

DECLARATION_GroupInfoArray_DELEGATE(GetJoinedGroupList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetJoinedGroupList)

void UTencentIMLibrary::GetJoinedGroupList(FIMGroupInfoArrayCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetJoinedGroupList_GroupInfoArrayDelegate = OnSuccessDelegate;
	GetJoinedGroupList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMGroupInfoVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMGroupInfoVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetJoinedGroupList_GroupInfoArrayDelegate.ExecuteIfBound(ToGroupInfoArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetJoinedGroupList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();

	Tencent_IM.GetInstance()->GetGroupManager()->GetJoinedGroupList(CallBack);
}

TArray<FTIMGroupInfo> UTencentIMLibrary::ToGroupInfoArray(const V2TIMGroupInfoVector& GroupInfoVector)
{
	TArray<FTIMGroupInfo> TIMGroupInfoArray;

	for (int i = 0; i < GroupInfoVector.Size(); ++i)
	{
		TIMGroupInfoArray.Add(ToGroupInfo(GroupInfoVector[i]));
	}

	return TIMGroupInfoArray;
}

V2TIMGroupInfoVector UTencentIMLibrary::ToTIMGroupInfoVector(const TArray<FTIMGroupInfo>& GroupInfo)
{
	V2TIMGroupInfoVector TIMGroupInfoVector;

	for (int i = 0; i < GroupInfo.Num(); ++i)
	{
		TIMGroupInfoVector.PushBack(ToTIMGroupInfo(GroupInfo[i]));
	}

	return TIMGroupInfoVector;
}

DECLARATION_CALLBACK_DELEGATE(SetGroupInfo)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetGroupInfo)

void UTencentIMLibrary::SetGroupInfo(const FTIMGroupInfo& info, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetGroupInfo_FailureDelegate = OnFailureDelegate;
	SetGroupInfo_Delegate = OnSuccessDelegate;
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
			SetGroupInfo_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			SetGroupInfo_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetGroupManager()->SetGroupInfo(ToTIMGroupInfo(info), Callback);
}

DECLARATION_GroupMemCount_DELEGATE(GetGroupOnlineMemberCount)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetGroupOnlineMemberCount)

void UTencentIMLibrary::GetGroupOnlineMemberCount(const FString& groupID, FGroupMemCountCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetGroupOnlineMemberCount_GroupMemCountDelegate = OnSuccessDelegate;
	GetGroupOnlineMemberCount_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<uint32_t>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const uint32_t& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetGroupOnlineMemberCount_GroupMemCountDelegate.ExecuteIfBound(message);
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetGroupOnlineMemberCount_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* Callback = new FValueCallBack();
	Tencent_IM.GetInstance()->GetGroupManager()->GetGroupOnlineMemberCount(ToIMString(groupID), Callback);
}

DECLARATION_TIMGroupMemberInfoResult_DELEGATE(GetGroupMemberList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetGroupMemberList)

void UTencentIMLibrary::GetGroupMemberList(const FString& groupID, int64 filter, const FString& nextSeq, FTIMGroupMemberInfoResultCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetGroupMemberList_TIMGroupMemberInfoResultDelegate = OnSuccessDelegate;
	GetGroupMemberList_FailureDelegate = OnFailureDelegate;

	class FValueCallBack : public V2TIMValueCallback<V2TIMGroupMemberInfoResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMGroupMemberInfoResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetGroupMemberList_TIMGroupMemberInfoResultDelegate.ExecuteIfBound(ToV2TIMGroupMemberInfoResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetGroupMemberList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* Callback = new FValueCallBack();
	Tencent_IM.GetInstance()->GetGroupManager()->GetGroupMemberList(ToIMString(groupID), filter, FCString::Atoi64(GetData(nextSeq)), Callback);
}

FTIMGroupMemberInfoResult UTencentIMLibrary::ToV2TIMGroupMemberInfoResult(const V2TIMGroupMemberInfoResult& Result)
{
	FTIMGroupMemberInfoResult TIMGroupMemberInfoResult;
	TIMGroupMemberInfoResult.nextSequence = FString::Printf(TEXT("%llu"), Result.nextSequence);
	TIMGroupMemberInfoResult.memberInfoList = ToFTIMGroupMemberFullInfoArray(Result.memberInfoList);
	return TIMGroupMemberInfoResult;
}

FTIMGroupMemberInfo UTencentIMLibrary::ToTIMGroupMemberInfo(const V2TIMGroupMemberInfo& GroupMemberInfo)
{
	FTIMGroupMemberInfo OutGroupMemberInfo;
	OutGroupMemberInfo.friendRemark = ToFString(GroupMemberInfo.friendRemark);
	OutGroupMemberInfo.userID = ToFString(GroupMemberInfo.userID);
	OutGroupMemberInfo.nickName = ToFString(GroupMemberInfo.nickName);
	OutGroupMemberInfo.nameCard = ToFString(GroupMemberInfo.nameCard);
	OutGroupMemberInfo.faceURL = ToFString(GroupMemberInfo.faceURL);
	return OutGroupMemberInfo;
}

TArray<FTIMGroupMemberFullInfo> UTencentIMLibrary::ToFTIMGroupMemberFullInfoArray(const V2TIMGroupMemberFullInfoVector& GroupMemberFullInfoVector)
{
	TArray<FTIMGroupMemberFullInfo> TIMGroupMemberFullInfoArray;
	for (int i = 0; i < GroupMemberFullInfoVector.Size(); ++i)
	{
		TIMGroupMemberFullInfoArray.Add(ToTIMGroupMemberFullInfo(GroupMemberFullInfoVector[i]));
	}
	return TIMGroupMemberFullInfoArray;
}

FTIMGroupMemberFullInfo UTencentIMLibrary::ToTIMGroupMemberFullInfo(const V2TIMGroupMemberFullInfo& Info)
{
	FTIMGroupMemberFullInfo TIMGroupMemberFullInfo;
	TIMGroupMemberFullInfo.customInfo = ToTIMCustomInfo_(Info.customInfo);
	TIMGroupMemberFullInfo.role = Info.role;
	TIMGroupMemberFullInfo.muteUntil = Info.muteUntil;
	TIMGroupMemberFullInfo.joinTime = Info.joinTime;
	TIMGroupMemberFullInfo.modifyFlag = Info.modifyFlag;
	return TIMGroupMemberFullInfo;
}

ETIMGroupMemberResult UTencentIMLibrary::ToTIMGroupMemberResult(const V2TIMGroupMemberResult& Result)
{
	switch (Result)
	{
	case V2TIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_FAIL:
		return ETIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_FAIL;
		break;
	case V2TIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_SUCC:
		return ETIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_SUCC;
		break;
	case V2TIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_INVALID:
		return ETIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_INVALID;
		break;
	case V2TIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_PENDING:
		return ETIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_PENDING;
		break;
	case V2TIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_OVERLIMIT:
		return ETIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_OVERLIMIT;
		break;
	}

	return ETIMGroupMemberResult::V2TIM_GROUP_MEMBER_RESULT_FAIL;
}

FTIMGroupMemberOperationResult UTencentIMLibrary::ToTIMGroupMemberOperationResult(const V2TIMGroupMemberOperationResult& Result)
{
	FTIMGroupMemberOperationResult TIMGroupMemberOperationResult;
	TIMGroupMemberOperationResult.userID = ToFString(Result.userID);
	TIMGroupMemberOperationResult.result = ToTIMGroupMemberResult(Result.result);
	return TIMGroupMemberOperationResult;
}

DECLARATION_GroupMemFullInfos_DELEGATE(GetGroupMembersInfo)

DECLARATION_FAILURE_CALLBACK_DELEGATE(GetGroupMembersInfo)

void UTencentIMLibrary::GetGroupMembersInfo(const FString& groupID, const TArray<FString>& memberList, FGroupMemFullInfosCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetGroupMembersInfo_GPMemFullInfoDelegate = OnSuccessDelegate;
	GetGroupMembersInfo_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMGroupMemberFullInfoVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMGroupMemberFullInfoVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetGroupMembersInfo_GPMemFullInfoDelegate.ExecuteIfBound(ToTIMGroupMemberFullInfoArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetGroupMembersInfo_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetGroupManager()->GetGroupMembersInfo(ToIMString(groupID), ToIMStringVector(memberList), CallBack);
}

V2TIMGroupMemberFullInfoVector UTencentIMLibrary::ToGroupMemberFullInfoVector(const TArray<FTIMGroupMemberFullInfo>& GPFullInfos)
{
	V2TIMGroupMemberFullInfoVector TIMGroupMemberFullInfoVector;

	for (int i = 0; i < GPFullInfos.Num(); ++i)
	{
		TIMGroupMemberFullInfoVector.PushBack(ToV2TIMGroupMemberFullInfo(GPFullInfos[i]));
	}

	return TIMGroupMemberFullInfoVector;
}

TArray<FTIMGroupMemberFullInfo> UTencentIMLibrary::ToTIMGroupMemberFullInfoArray(const V2TIMGroupMemberFullInfoVector& GPFullInfos)
{
	TArray<FTIMGroupMemberFullInfo> TIMGroupMemberFullInfoArray;

	for (int i = 0; i < GPFullInfos.Size(); ++i)
	{
		TIMGroupMemberFullInfoArray.Add(ToTIMGroupMemberFullInfo(GPFullInfos[i]));
	}

	return TIMGroupMemberFullInfoArray;
}

DECLARATION_CALLBACK_DELEGATE(MuteGroupMember)
DECLARATION_FAILURE_CALLBACK_DELEGATE(MuteGroupMember)

void UTencentIMLibrary::MuteGroupMember(const FString& groupID, const FString& userID, int32 seconds, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	MuteGroupMember_Delegate = OnSuccessDelegate;
	MuteGroupMember_FailureDelegate = OnFailureDelegate;
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
			MuteGroupMember_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			MuteGroupMember_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetGroupManager()->MuteGroupMember(ToIMString(groupID), ToIMString(userID), seconds, Callback);
}

DECLARATION_GroupInviteMem_DELEGATE(InviteUserToGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(InviteUserToGroup)

void UTencentIMLibrary::InviteUserToGroup(const FString& groupID, const TArray<FString>& userList, FGroupIniteUserCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	InviteUserToGroup_GPInviteMemDelegate = OnSuccessDelegate;
	InviteUserToGroup_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMGroupMemberOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			InviteUserToGroup_GPInviteMemDelegate.ExecuteIfBound(ToGPMemOpArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			InviteUserToGroup_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetGroupManager()->InviteUserToGroup(ToIMString(groupID), ToIMStringVector(userList), CallBack);
}

TArray<FTIMGroupMemberOperationResult> UTencentIMLibrary::ToGPMemOpArray(const V2TIMGroupMemberOperationResultVector& GPMemOPVector)
{
	TArray<FTIMGroupMemberOperationResult> TIMGroupMemberOperationResult;

	for (int i = 0; i < GPMemOPVector.Size(); ++i)
	{
		TIMGroupMemberOperationResult.Add(ToTIMGroupMemberOperationResult(GPMemOPVector[i]));
	}

	return TArray<FTIMGroupMemberOperationResult>();
}

DECLARATION_GroupAPPResult_DELEGATE(GetGroupApplicationList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetGroupApplicationList)

void UTencentIMLibrary::GetGroupApplicationList(FGroupAppRstCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetGroupApplicationList_GPAppRstDelegate = OnSuccessDelegate;
	GetGroupApplicationList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMGroupApplicationResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMGroupApplicationResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetGroupApplicationList_GPAppRstDelegate.ExecuteIfBound(ToGroupAppResArray(message));
		};

		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnFailure ======"));
			GetGroupApplicationList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetGroupManager()->GetGroupApplicationList(CallBack);
}

FTIMGroupApplicationResult UTencentIMLibrary::ToGroupAppResArray(const V2TIMGroupApplicationResult& GroupApplicationResult)
{
	FTIMGroupApplicationResult TIMGroupApplicationResult;
	TIMGroupApplicationResult.unreadCount = FString::Printf(TEXT("%llu"), GroupApplicationResult.unreadCount);
	// TIMGroupApplicationResult.applicationList=GroupApplicationResult.applicationList;
	return FTIMGroupApplicationResult();
}

V2TIMGroupApplicationResult UTencentIMLibrary::ToTIMGroupAppRes(const FTIMGroupApplicationResult& GroupApplicationResult)
{
	//todo finish

	return V2TIMGroupApplicationResult();
}

FTIMGroupApplication UTencentIMLibrary::ToGroupApp(const V2TIMGroupApplication& TIMGroupApplication)
{
	//todo finish

	return FTIMGroupApplication();
}

V2TIMGroupApplication UTencentIMLibrary::ToTIMGroupApp(const FTIMGroupApplication& TIMGroupApplication)
{
	//todo finish

	return V2TIMGroupApplication();
}


DECLARATION_CALLBACK_DELEGATE(AcceptGroupApplication)
DECLARATION_FAILURE_CALLBACK_DELEGATE(AcceptGroupApplication)

void UTencentIMLibrary::AcceptGroupApplication(const FTIMGroupApplication& application, const FString& reason, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	AcceptGroupApplication_Delegate = OnSuccessDelegate;
	AcceptGroupApplication_FailureDelegate = OnFailureDelegate;

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
			AcceptGroupApplication_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			AcceptGroupApplication_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetGroupManager()->AcceptGroupApplication(ToTIMGroupApp(application), ToIMString(reason), Callback);
}

DECLARATION_CALLBACK_DELEGATE(RefuseGroupApplication)
DECLARATION_FAILURE_CALLBACK_DELEGATE(RefuseGroupApplication)

void UTencentIMLibrary::RefuseGroupApplication(const FTIMGroupApplication& application, const FString& reason, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	RefuseGroupApplication_Delegate = OnSuccessDelegate;
	RefuseGroupApplication_FailureDelegate = OnFailureDelegate;

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
			RefuseGroupApplication_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			RefuseGroupApplication_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetGroupManager()->RefuseGroupApplication(ToTIMGroupApp(application), ToIMString(reason), Callback);
}

DECLARATION_CALLBACK_DELEGATE(SetGroupApplicationRead)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetGroupApplicationRead)

void UTencentIMLibrary::SetGroupApplicationRead(FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetGroupApplicationRead_Delegate = OnSuccessDelegate;
	SetGroupApplicationRead_FailureDelegate = OnFailureDelegate;
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
			SetGroupApplicationRead_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			SetGroupApplicationRead_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetGroupManager()->SetGroupApplicationRead(Callback);
}

V2TIMFriendshipListener* UTencentIMLibrary::Friendlistener = nullptr;


DECLARATION_FriendApplicationListAdded_DELEGATE(AddFriendListener)
DECLARATION_FriendApplicationListDeleted_DELEGATE(AddFriendListener)
DECLARATION_CALLBACK_DELEGATE(AddFriendListener)
DECLARATION_FriendListAdded_DELEGATE(AddFriendListener)
DECLARATION_FriendListDeleted_DELEGATE(AddFriendListener)
DECLARATION_FriendInfoArrayDeleted_DELEGATE(AddFriendListener)
DECLARATION_BlackListDeleted_DELEGATE(AddFriendListener)
DECLARATION_FriendInfoChanged_DELEGATE(AddFriendListener)

void UTencentIMLibrary::AddFriendListener(FFriendAPPArrayAddedCallback FriendAppArrayAddedCallback, FFriendAPPArrayDeletedCallback APPDeleteDelegate, FIMCallbackDelegate FriendApplicationListRead,
                                          FFriendListAddedCallback ListAddedCallback, FFriendListDeletedCallback FriendListDeletedCallback, FBlackListAddedCallback BlackListAddedCallback,
                                          FBlackListDeletedCallback BlackListDeletedCallback, FFriendInfoChangedCallback FriendInfoChangedCallback)
{
	AddFriendListener_FriendAPPArrayAddedDelegate = FriendAppArrayAddedCallback;
	AddFriendListener_FriendAPPListDeleted = APPDeleteDelegate;
	AddFriendListener_Delegate = FriendApplicationListRead;
	AddFriendListener_FriendListAddedDelegate = ListAddedCallback;
	AddFriendListener_FriendInfoArrayDeletedDelegate = FriendListDeletedCallback;
	AddFriendListener_BlackListAddedDelegate = BlackListAddedCallback;
	AddFriendListener_BlackListDeletedDelegate = BlackListDeletedCallback;
	AddFriendListener_FriendInfoChangedDelegate = FriendInfoChangedCallback;

	class IMFriendshipListener : public V2TIMFriendshipListener
	{
	public:
		IMFriendshipListener()
		{
		};

		virtual ~IMFriendshipListener()
		{
		};

		/**
		 * 好友申请新增通知，两种情况会收到这个回调：
		 * 1. 自己申请加别人好友
		 * 2. 别人申请加自己好友
		 */
		virtual void OnFriendApplicationListAdded(const V2TIMFriendApplicationVector& applicationList)
		{
			AddFriendListener_FriendAPPArrayAddedDelegate.ExecuteIfBound(ToTIMFriendApplicationArray(applicationList));
		}

		/**
		 * 好友申请删除通知，四种情况会收到这个回调
		 * 1. 调用 DeleteFriendApplication 主动删除好友申请
		 * 2. 调用 RefuseFriendApplication 拒绝好友申请
		 * 3. 调用 AcceptFriendApplication 同意好友申请且同意类型为 V2TIM_FRIEND_ACCEPT_AGREE 时
		 * 4. 申请加别人好友被拒绝
		 */
		virtual void OnFriendApplicationListDeleted(const V2TIMStringVector& userIDList)
		{
			AddFriendListener_FriendAPPListDeleted.ExecuteIfBound(ToFStringArray(userIDList));
		}

		/**
		 * 好友申请已读通知，如果调用 setFriendApplicationRead
		 * 设置好友申请列表已读，会收到这个回调（主要用于多端同步）
		 */
		virtual void OnFriendApplicationListRead()
		{
			AddFriendListener_Delegate.ExecuteIfBound();
		}

		/**
		 * 好友新增通知
		 */
		virtual void OnFriendListAdded(const V2TIMFriendInfoVector& userIDList)
		{
			AddFriendListener_FriendListAddedDelegate.ExecuteIfBound(ToFriendInfoArray(userIDList));
		}

		/**
		 * 好友删除通知，，两种情况会收到这个回调：
		 * 1. 自己删除好友（单向和双向删除都会收到回调）
		 * 2. 好友把自己删除（双向删除会收到）
		 */
		virtual void OnFriendListDeleted(const V2TIMStringVector& userIDList)
		{
			AddFriendListener_FriendInfoArrayDeletedDelegate.ExecuteIfBound(ToFStringArray(userIDList));
		}

		/**
		 * 黑名单新增通知
		 */
		virtual void OnBlackListAdded(const V2TIMFriendInfoVector& infoList)
		{
			AddFriendListener_BlackListAddedDelegate.ExecuteIfBound(ToFriendInfoArray(infoList));
		}

		/**
		 * 黑名单删除通知
		 */
		virtual void OnBlackListDeleted(const V2TIMStringVector& userIDList)
		{
			AddFriendListener_BlackListDeletedDelegate.ExecuteIfBound(ToFStringArray(userIDList));
		}

		/**
		 * 好友资料更新通知
		 */
		virtual void OnFriendInfoChanged(const V2TIMFriendInfoVector& infoList)
		{
			AddFriendListener_FriendInfoChangedDelegate.ExecuteIfBound(ToFriendInfoArray(infoList));
		}
	};
	Friendlistener = new IMFriendshipListener();
	Tencent_IM.GetInstance()->GetFriendshipManager()->AddFriendListener(Friendlistener);
}


DECLARATION_ConversationRst_DELEGATE(GetConversationList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetConversationList)

void UTencentIMLibrary::GetConversationList(const FString& nextSeq, int32 count, FMTIMConversationResultCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetConversationList_CsRstDelegate = OnSuccessDelegate;
	GetConversationList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMConversationResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMConversationResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetConversationList_CsRstDelegate.ExecuteIfBound(ToTIMConversationResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetConversationList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetConversationManager()->GetConversationList(FCString::Strtoui64(*nextSeq, NULL, 10), count, CallBack);
}

FTIMConversationResult UTencentIMLibrary::ToTIMConversationResult(const V2TIMConversationResult& ConversationResult)
{
	FTIMConversationResult Temp = FTIMConversationResult();
	Temp.conversationList = ToTIMConversationArray(ConversationResult.conversationList);
	Temp.isFinished = ConversationResult.isFinished;
	Temp.nextSeq = FString::Printf(TEXT("%llu"), ConversationResult.nextSeq);
	return Temp;
}

V2TIMVConversationVector UTencentIMLibrary::ToV2TIMVConversationVector(const TArray<FTIMConversation>& Array_Conversation)
{
	V2TIMVConversationVector ConversationVector = V2TIMVConversationVector();
	for (FTIMConversation Conversation : Array_Conversation)
	{
		ConversationVector.PushBack(ToTIMConversation(Conversation));
	}
	return ConversationVector;
}

TArray<FTIMConversation> UTencentIMLibrary::ToTIMConversationArray(const V2TIMVConversationVector& ConversationVector)
{
	TArray<FTIMConversation> Conversation;
	for (int i = 0; i < ConversationVector.Size(); i++)
	{
		Conversation.Add(ToConversation(ConversationVector[i]));
	}
	return Conversation;
}

FTIMConversation UTencentIMLibrary::ToConversation(const V2TIMConversation& Conversation)
{
	FTIMConversation conversation;
	conversation.type = ToConversationType(Conversation.type);
	conversation.conversationID = ToFString(Conversation.conversationID);
	conversation.type = ToConversationType(Conversation.type);
	conversation.userID = ToFString(Conversation.userID);
	conversation.groupID = ToFString(Conversation.groupID);
	conversation.groupType = ToFString(Conversation.groupType);
	conversation.showName = ToFString(Conversation.showName);
	conversation.faceUrl = ToFString(Conversation.faceUrl);
	conversation.unreadCount = Conversation.unreadCount;
	conversation.recvOpt = ToReceiveMessageOpt(Conversation.recvOpt);
	conversation.lastMessage = ToMessage(*Conversation.lastMessage);
	conversation.groupAtInfolist = ToGroupAtInfoArray(Conversation.groupAtInfolist);
	conversation.draftText = ToFString(Conversation.draftText);
	conversation.draftTimestamp = FString::Printf(TEXT("%lu"), Conversation.draftTimestamp);
	conversation.isPinned = (Conversation.isPinned);
	conversation.orderKey = FString::Printf(TEXT("%lu"), Conversation.orderKey);
	return conversation;
}

TArray<FTIMGroupAtInfo> UTencentIMLibrary::ToGroupAtInfoArray(const V2TIMGroupAtInfoVector& GroupAtInfoVector)
{
	TArray<FTIMGroupAtInfo> Result;
	for (int i = 0; i < GroupAtInfoVector.Size(); ++i)
	{
		Result.Add(ToGroupAtInfo(GroupAtInfoVector[i]));
	}	
	return TArray<FTIMGroupAtInfo>();
}

V2TIMGroupAtInfoVector UTencentIMLibrary::ToTIMGroupAtInfoVector(const TArray<FTIMGroupAtInfo>& GroupAtInfoVector)
{
	V2TIMGroupAtInfoVector TIMGroupAtInfoVector;
	for (FTIMGroupAtInfo GroupAtInfo : GroupAtInfoVector)
	{
		TIMGroupAtInfoVector.PushBack(ToTIMGroupAtInfo(GroupAtInfo));
	}
	return TIMGroupAtInfoVector;
}

FTIMGroupAtInfo UTencentIMLibrary::ToGroupAtInfo(const V2TIMGroupAtInfo& GroupAtInfo)
{
	FTIMGroupAtInfo TimGroupInfo;
	TimGroupInfo.seq=FString::Printf(TEXT("%lu"), GroupAtInfo.seq);
	TimGroupInfo.atType=ToGroupAtType(GroupAtInfo.atType);
	return TimGroupInfo;
}

V2TIMGroupAtInfo UTencentIMLibrary::ToTIMGroupAtInfo(const FTIMGroupAtInfo& GroupAtInfo)
{
	V2TIMGroupAtInfo TimGroupAtInfo;
	TimGroupAtInfo.seq=FCString::Strtoui64(GetData(GroupAtInfo.seq),NULL, 10);
	TimGroupAtInfo.atType=ToTIMGroupAtType(GroupAtInfo.atType);
	return TimGroupAtInfo;
}
V2TIMGroupAtType UTencentIMLibrary::ToTIMGroupAtType(const ETIMGroupAtType& GroupType)
{
	switch (GroupType)
	{
	case V2TIM_AT_ME:
		return V2TIMGroupAtType::V2TIM_AT_ME;
		break;
	case V2TIM_AT_ALL:
		return V2TIMGroupAtType::V2TIM_AT_ALL;
		break;
	case V2TIM_AT_ALL_AT_ME:
		return V2TIMGroupAtType::V2TIM_AT_ALL_AT_ME;
		break;
	default:
		return V2TIMGroupAtType::V2TIM_AT_ME;
	}
	return V2TIMGroupAtType::V2TIM_AT_ME;
}
ETIMGroupAtType UTencentIMLibrary::ToGroupAtType(const V2TIMGroupAtType& GroupType)
{
	switch (GroupType)
	{
	case V2TIM_AT_ME:
		return ETIMGroupAtType::V2TIM_AT_ME;
		break;
	case V2TIM_AT_ALL:
		return ETIMGroupAtType::V2TIM_AT_ALL;
		break;
	case V2TIM_AT_ALL_AT_ME:
		return ETIMGroupAtType::V2TIM_AT_ALL_AT_ME;
		break;
	default:
		return ETIMGroupAtType::V2TIM_AT_ME;

	}
	return ETIMGroupAtType::V2TIM_AT_ME;
}


ETIMConversationType UTencentIMLibrary::ToConversationType(const V2TIMConversationType& ConvType)
{
	switch (ConvType)
	{
	case V2TIM_C2C:
		return ETIMConversationType::V2TIM_C2C;
	case V2TIM_GROUP:
		return ETIMConversationType::V2TIM_GROUP;
	default:
		return ETIMConversationType::V2TIM_C2C;
	}
	return ETIMConversationType::V2TIM_C2C;
}

V2TIMConversationType UTencentIMLibrary::ToTimConversationType(const ETIMConversationType& ConvType)
{
	switch (ConvType)
	{
	case V2TIM_C2C:
		return V2TIMConversationType::V2TIM_C2C;
	case V2TIM_GROUP:
		return V2TIMConversationType::V2TIM_GROUP;
	default:
		return V2TIMConversationType::V2TIM_C2C;
	}
	return V2TIMConversationType::V2TIM_C2C;
}

V2TIMConversation UTencentIMLibrary::ToTIMConversation(const FTIMConversation& Conversation)
{
	V2TIMConversation TimConversation;
	TimConversation.type=ToTimConversationType(Conversation.type);
	TimConversation.conversationID=ToIMString(Conversation.conversationID);
	TimConversation.userID=ToIMString(Conversation.userID);
	TimConversation.groupID=ToIMString(Conversation.groupID);
	TimConversation.groupType=ToIMString(Conversation.groupType);
	TimConversation.showName=ToIMString(Conversation.showName);
	TimConversation.faceUrl=ToIMString(Conversation.faceUrl);
	TimConversation.unreadCount=(Conversation.unreadCount);
	TimConversation.recvOpt=ToTIMReceiveMessageOpt(Conversation.recvOpt);
	V2TIMMessage lastMessage =ToIMMessage(Conversation.lastMessage);
	TimConversation.lastMessage=&lastMessage;
	TimConversation.groupAtInfolist=ToTIMGroupAtInfoVector(Conversation.groupAtInfolist);
	TimConversation.draftText=ToIMString(Conversation.draftText);
	TimConversation.draftTimestamp=FCString::Strtoui64(GetData(Conversation.draftTimestamp),NULL, 10);
	TimConversation.isPinned=(Conversation.isPinned);
	TimConversation.orderKey=FCString::Strtoui64(GetData(Conversation.orderKey),NULL, 10);
	return V2TIMConversation();
}

DECLARATION_TIMConversation_DELEGATE(GetConversation)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetConversation)

void UTencentIMLibrary::GetConversation(const FString& conversationID, FTIMConversationCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetConversation_ConversationDelegate = OnSuccessDelegate;
	GetConversation_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMConversation>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMConversation& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetConversation_ConversationDelegate.ExecuteIfBound(ToConversation(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetConversation_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetConversationManager()->GetConversation(ToIMString(conversationID), CallBack);
}

DECLARATION_TIMConversationVector_DELEGATE(GetConversationListByIDList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetConversationListByIDList)

void UTencentIMLibrary::GetConversationListByIDList(const TArray<FString>& conversationIDList, FTIMConversationVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetConversationListByIDList_ConversationVectorDelegate = OnSuccessDelegate;
	GetConversationListByIDList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMVConversationVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMVConversationVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetConversationListByIDList_ConversationVectorDelegate.ExecuteIfBound(ToTIMConversationArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetConversation_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetConversationManager()->GetConversationList(ToIMStringVector(conversationIDList), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(DeleteConversation)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteConversation)

void UTencentIMLibrary::DeleteConversation(const FString& conversationID, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	DeleteConversation_FailureDelegate = OnFailureDelegate;
	DeleteConversation_Delegate = OnSuccessDelegate;
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
			DeleteConversation_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			DeleteConversation_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetConversationManager()->DeleteConversation(ToIMString(conversationID), Callback);
}

DECLARATION_CALLBACK_DELEGATE(SetConversationDraft)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetConversationDraft)

void UTencentIMLibrary::SetConversationDraft(const FString& conversationID, const FString& draftText, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetConversationDraft_FailureDelegate = OnFailureDelegate;
	SetConversationDraft_Delegate = OnSuccessDelegate;
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
			SetConversationDraft_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			SetConversationDraft_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetConversationManager()->SetConversationDraft(ToIMString(conversationID), ToIMString(draftText), Callback);
}

DECLARATION_CALLBACK_DELEGATE(PinConversation)
DECLARATION_FAILURE_CALLBACK_DELEGATE(PinConversation)

void UTencentIMLibrary::PinConversation(const FString& conversationID, bool isPinned, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	PinConversation_FailureDelegate = OnFailureDelegate;
	PinConversation_Delegate = OnSuccessDelegate;
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
			PinConversation_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			PinConversation_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetConversationManager()->PinConversation(ToIMString(conversationID), isPinned, Callback);
}

DECLARATION_TIMuint64_DELEGATE(GetTotalUnreadMessageCount)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetTotalUnreadMessageCount)

void UTencentIMLibrary::GetTotalUnreadMessageCount(FTIMuint64Callback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetTotalUnreadMessageCount_uint64Delegate = OnSuccessDelegate;
	GetTotalUnreadMessageCount_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<uint64_t>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const uint64_t& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetTotalUnreadMessageCount_uint64Delegate.ExecuteIfBound(message);
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetTotalUnreadMessageCount_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetConversationManager()->GetTotalUnreadMessageCount(CallBack);
}

DECLARATION_FriendApplicationListAdded_DELEGATE(RemoveFriendListener)
DECLARATION_FriendApplicationListDeleted_DELEGATE(RemoveFriendListener)
DECLARATION_CALLBACK_DELEGATE(RemoveFriendListener)
DECLARATION_FriendListAdded_DELEGATE(RemoveFriendListener)
DECLARATION_FriendListDeleted_DELEGATE(RemoveFriendListener)
DECLARATION_FriendInfoArrayDeleted_DELEGATE(RemoveFriendListener)
DECLARATION_BlackListDeleted_DELEGATE(RemoveFriendListener)
DECLARATION_FriendInfoChanged_DELEGATE(RemoveFriendListener)

void UTencentIMLibrary::RemoveFriendListener(FFriendAPPArrayAddedCallback FriendAppArrayAddedCallback, FFriendAPPArrayDeletedCallback APPDeleteDelegate, FIMCallbackDelegate FriendApplicationListRead,
                                             FFriendListAddedCallback ListAddedCallback, FFriendListDeletedCallback FriendListDeletedCallback, FBlackListAddedCallback BlackListAddedCallback,
                                             FBlackListDeletedCallback BlackListDeletedCallback, FFriendInfoChangedCallback FriendInfoChangedCallback)
{
	RemoveFriendListener_FriendAPPArrayAddedDelegate = FriendAppArrayAddedCallback;
	RemoveFriendListener_FriendAPPListDeleted = APPDeleteDelegate;
	RemoveFriendListener_Delegate = FriendApplicationListRead;
	RemoveFriendListener_FriendListAddedDelegate = ListAddedCallback;
	RemoveFriendListener_FriendInfoArrayDeletedDelegate = FriendListDeletedCallback;
	RemoveFriendListener_BlackListAddedDelegate = BlackListAddedCallback;
	RemoveFriendListener_BlackListDeletedDelegate = BlackListDeletedCallback;
	RemoveFriendListener_FriendInfoChangedDelegate = FriendInfoChangedCallback;

	class IMFriendshipListener : public V2TIMFriendshipListener
	{
	public:
		IMFriendshipListener()
		{
		};

		virtual ~IMFriendshipListener()
		{
		};

		/**
		 * 好友申请新增通知，两种情况会收到这个回调：
		 * 1. 自己申请加别人好友
		 * 2. 别人申请加自己好友
		 */
		virtual void OnFriendApplicationListAdded(const V2TIMFriendApplicationVector& applicationList)
		{
			AddFriendListener_FriendAPPArrayAddedDelegate.ExecuteIfBound(ToTIMFriendApplicationArray(applicationList));
		}

		/**
		 * 好友申请删除通知，四种情况会收到这个回调
		 * 1. 调用 DeleteFriendApplication 主动删除好友申请
		 * 2. 调用 RefuseFriendApplication 拒绝好友申请
		 * 3. 调用 AcceptFriendApplication 同意好友申请且同意类型为 V2TIM_FRIEND_ACCEPT_AGREE 时
		 * 4. 申请加别人好友被拒绝
		 */
		virtual void OnFriendApplicationListDeleted(const V2TIMStringVector& userIDList)
		{
			AddFriendListener_FriendAPPListDeleted.ExecuteIfBound(ToFStringArray(userIDList));
		}

		/**
		 * 好友申请已读通知，如果调用 setFriendApplicationRead
		 * 设置好友申请列表已读，会收到这个回调（主要用于多端同步）
		 */
		virtual void OnFriendApplicationListRead()
		{
			AddFriendListener_Delegate.ExecuteIfBound();
		}

		/**
		 * 好友新增通知
		 */
		virtual void OnFriendListAdded(const V2TIMFriendInfoVector& userIDList)
		{
			AddFriendListener_FriendListAddedDelegate.ExecuteIfBound(ToFriendInfoArray(userIDList));
		}

		/**
		 * 好友删除通知，，两种情况会收到这个回调：
		 * 1. 自己删除好友（单向和双向删除都会收到回调）
		 * 2. 好友把自己删除（双向删除会收到）
		 */
		virtual void OnFriendListDeleted(const V2TIMStringVector& userIDList)
		{
			AddFriendListener_FriendInfoArrayDeletedDelegate.ExecuteIfBound(ToFStringArray(userIDList));
		}

		/**
		 * 黑名单新增通知
		 */
		virtual void OnBlackListAdded(const V2TIMFriendInfoVector& infoList)
		{
			AddFriendListener_BlackListAddedDelegate.ExecuteIfBound(ToFriendInfoArray(infoList));
		}

		/**
		 * 黑名单删除通知
		 */
		virtual void OnBlackListDeleted(const V2TIMStringVector& userIDList)
		{
			AddFriendListener_BlackListDeletedDelegate.ExecuteIfBound(ToFStringArray(userIDList));
		}

		/**
		 * 好友资料更新通知
		 */
		virtual void OnFriendInfoChanged(const V2TIMFriendInfoVector& infoList)
		{
			AddFriendListener_FriendInfoChangedDelegate.ExecuteIfBound(ToFriendInfoArray(infoList));
		}
	};
	Friendlistener = new IMFriendshipListener();
	Tencent_IM.GetInstance()->GetFriendshipManager()->RemoveFriendListener(Friendlistener);
}

DECLARATION_TIMFriendInfoVector_DELEGATE(GetFriendList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetFriendList)

void UTencentIMLibrary::GetFriendList(FTIMFriendInfoVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetFriendList_TIMFriendInfoVectorDelegate = OnSuccessDelegate;
	GetFriendList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendInfoVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendInfoVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetFriendList_TIMFriendInfoVectorDelegate.ExecuteIfBound(ToFriendInfoArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetFriendList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->GetFriendList(CallBack);
}

DECLARATION_TIMFriendInfoResultVector_DELEGATE(GetFriendsInfo)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetFriendsInfo)

void UTencentIMLibrary::GetFriendsInfo(const TArray<FString>& userIDList, FTIMFriendInfoResultVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetFriendsInfo_TIMFriendInfoResultVectorDelegate = OnSuccessDelegate;
	GetFriendsInfo_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendInfoResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendInfoResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetFriendsInfo_TIMFriendInfoResultVectorDelegate.ExecuteIfBound(ToFriendInfoResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetFriendsInfo_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->GetFriendsInfo(ToIMStringVector(userIDList), CallBack);
}

TArray<FTIMFriendInfoResult> UTencentIMLibrary::ToFriendInfoResultArray(const V2TIMFriendInfoResultVector& FriendInfoResultVector)
{
	TArray<FTIMFriendInfoResult> Result;

	for (int i = 0; i < FriendInfoResultVector.Size(); ++i)
	{
		Result.Add(ToFriendInforResult(FriendInfoResultVector[i]));
	}

	return Result;
}

FTIMFriendInfoResult UTencentIMLibrary::ToFriendInforResult(const V2TIMFriendInfoResult& FriendInfoResult)
{
	FTIMFriendInfoResult InfoResult;
	InfoResult.resultCode = FriendInfoResult.resultCode;
	InfoResult.resultInfo = ToFString(FriendInfoResult.resultInfo);
	InfoResult.relation = ETIMFriendRelationType(FriendInfoResult.relation);
	InfoResult.friendInfo = ToFriendInfo(FriendInfoResult.friendInfo);
	return InfoResult;
}

TArray<FTIMFriendInfo> UTencentIMLibrary::ToFriendInfoArray(const V2TIMFriendInfoVector& Info)
{
	TArray<FTIMFriendInfo> Conversation;
	for (int i = 0; i < Info.Size(); i++)
	{
		Conversation.Add(ToFriendInfo(Info[i]));
	}
	return Conversation;
}

FTIMFriendInfo UTencentIMLibrary::ToFriendInfo(const V2TIMFriendInfo& Info)
{
	FTIMFriendInfo info = FTIMFriendInfo();
	info.userID = ToFString(Info.userID);
	info.friendRemark = ToFString(Info.friendRemark);
	info.friendCustomInfo = ToTIMCustomInfo_(Info.friendCustomInfo);
	info.friendGroups = ToFStringArray(Info.friendGroups);
	info.userFullInfo = ToTIMUserFullInfo(Info.userFullInfo);
	info.modifyFlag = FString::Printf(TEXT("%lu"), Info.modifyFlag);
	return info;
}

V2TIMFriendInfo UTencentIMLibrary::ToTIMFriendInfo(const FTIMFriendInfo& Info)
{
	V2TIMFriendInfo info = V2TIMFriendInfo();
	info.userID = ToIMString(Info.userID);
	info.friendRemark = ToIMString(Info.friendRemark);
	info.friendCustomInfo = ToV2TIMCustomInfo(Info.friendCustomInfo);
	info.friendGroups = ToIMStringVector(Info.friendGroups);
	info.userFullInfo = ToV2TIMUserFullInfo(Info.userFullInfo);
	info.modifyFlag = uint32_t(FCString::Atoi(GetData(Info.modifyFlag)));
	return info;
}

DECLARATION_CALLBACK_DELEGATE(SetFriendInfo)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetFriendInfo)

void UTencentIMLibrary::SetFriendInfo(const FTIMFriendInfo& info, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetFriendInfo_FailureDelegate = OnFailureDelegate;
	SetFriendInfo_Delegate = OnSuccessDelegate;
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
			DeleteConversation_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			DeleteConversation_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetFriendshipManager()->SetFriendInfo(ToTIMFriendInfo(info), Callback);
}

DECLARATION_TIMFriendInfoResultVector_DELEGATE(SearchFriends)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SearchFriends)

void UTencentIMLibrary::SearchFriends(const FTIMFriendSearchParam& searchParam, FTIMFriendInfoResultVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SearchFriends_TIMFriendInfoResultVectorDelegate = OnSuccessDelegate;
	SearchFriends_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendInfoResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendInfoResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			SearchFriends_TIMFriendInfoResultVectorDelegate.ExecuteIfBound(ToFriendInfoResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			SearchFriends_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->SearchFriends(ToTIMFriendSearchParam(searchParam), CallBack);
}

DECLARATION_TIMFriendOperationResul_DELEGATE(AddFriend)
DECLARATION_FAILURE_CALLBACK_DELEGATE(AddFriend)

void UTencentIMLibrary::AddFriend(const FTIMFriendAddApplication& application, FTIMFriendOperationResulCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	AddFriend_TIMFriendOperationResulDelegate = OnSuccessDelegate;
	AddFriend_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			AddFriend_TIMFriendOperationResulDelegate.ExecuteIfBound(ToFriendOperationResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			AddFriend_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->AddFriend(ToTIMFriendAddApplication(application), CallBack);
}

DECLARATION_TIMFriendOperationResultVector_DELEGATE(DeleteFromFriendList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteFromFriendList)

void UTencentIMLibrary::DeleteFromFriendList(const TArray<FString>& userIDList, ETIMFriendType deleteType, FTIMFriendOperationResultVectorCallback OnSuccessDelegate,
                                             FIMFailureCallback OnFailureDelegate)
{
	DeleteFromFriendList_TIMFriendOperationResultVectorDelegate = OnSuccessDelegate;
	DeleteFromFriendList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			DeleteFromFriendList_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(ToTIMFriendOperationResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			DeleteFromFriendList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->DeleteFromFriendList(ToIMStringVector(userIDList), ToTIMFriendType(deleteType), CallBack);
}

TArray<FTIMFriendOperationResult> UTencentIMLibrary::ToTIMFriendOperationResultArray(const V2TIMFriendOperationResultVector& TIMFriendOperationResultVector)
{
	TArray<FTIMFriendOperationResult> TIMFriendOperationResultArray;
	for (int i = 0; i < TIMFriendOperationResultVector.Size(); ++i)
	{
		TIMFriendOperationResultArray.Add(ToFriendOperationResult(TIMFriendOperationResultVector[i]));
	}

	return TIMFriendOperationResultArray;
}


DECLARATION_TIMFriendCheckResultVector_DELEGATE(CheckFriend)
DECLARATION_FAILURE_CALLBACK_DELEGATE(CheckFriend)

void UTencentIMLibrary::CheckFriend(const TArray<FString>& userIDList, ETIMFriendType checkType, FTIMFriendCheckResultVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	CheckFriend_TIMFriendCheckResultVectorDelegate = OnSuccessDelegate;
	CheckFriend_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendCheckResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendCheckResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			CheckFriend_TIMFriendCheckResultVectorDelegate.ExecuteIfBound(ToTIMFriendCheckResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			CheckFriend_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->CheckFriend(ToIMStringVector(userIDList), ToTIMFriendType(checkType), CallBack);
}

DECLARATION_TIMFriendApplicationResult_DELEGATE(GetFriendApplicationList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetFriendApplicationList)

void UTencentIMLibrary::GetFriendApplicationList(FTIMFriendApplicationResultCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetFriendApplicationList_TIMFriendApplicationResultDelegate = OnSuccessDelegate;
	GetFriendApplicationList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendApplicationResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendApplicationResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetFriendApplicationList_TIMFriendApplicationResultDelegate.ExecuteIfBound(ToTIMFriendApplicationResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetFriendApplicationList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->GetFriendApplicationList(CallBack);
}

DECLARATION_TIMFriendOperationResul_DELEGATE(AcceptFriendApplication)
DECLARATION_FAILURE_CALLBACK_DELEGATE(AcceptFriendApplication)

void UTencentIMLibrary::AcceptFriendApplication(const FTIMFriendApplication& application, ETIMFriendAcceptType acceptType, FTIMFriendOperationResulCallback OnSuccessDelegate,
                                                FIMFailureCallback OnFailureDelegate)
{
	AcceptFriendApplication_TIMFriendOperationResulDelegate = OnSuccessDelegate;
	AcceptFriendApplication_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			AcceptFriendApplication_TIMFriendOperationResulDelegate.ExecuteIfBound(ToFriendOperationResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			AcceptFriendApplication_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->AcceptFriendApplication(ToV2TIMFriendApplication(application), ToV2TIMFriendAcceptType(acceptType), CallBack);
}

DECLARATION_TIMFriendOperationResul_DELEGATE(RefuseFriendApplication)
DECLARATION_FAILURE_CALLBACK_DELEGATE(RefuseFriendApplication)

void UTencentIMLibrary::RefuseFriendApplication(const FTIMFriendApplication& application, FTIMFriendOperationResulCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	RefuseFriendApplication_TIMFriendOperationResulDelegate = OnSuccessDelegate;
	RefuseFriendApplication_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResult>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResult& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			RefuseFriendApplication_TIMFriendOperationResulDelegate.ExecuteIfBound(ToFriendOperationResult(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			RefuseFriendApplication_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->RefuseFriendApplication(ToV2TIMFriendApplication(application), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(DeleteFriendApplication)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteFriendApplication)

void UTencentIMLibrary::DeleteFriendApplication(const FTIMFriendApplication& application, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	DeleteFriendApplication_FailureDelegate = OnFailureDelegate;
	DeleteFriendApplication_Delegate = OnSuccessDelegate;
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
			DeleteFriendApplication_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			DeleteFriendApplication_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetFriendshipManager()->DeleteFriendApplication(ToV2TIMFriendApplication(application), Callback);
}

DECLARATION_CALLBACK_DELEGATE(SetFriendApplicationRead)
DECLARATION_FAILURE_CALLBACK_DELEGATE(SetFriendApplicationRead)

void UTencentIMLibrary::SetFriendApplicationRead(FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	SetFriendApplicationRead_FailureDelegate = OnFailureDelegate;
	SetFriendApplicationRead_Delegate = OnSuccessDelegate;
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
			SetFriendApplicationRead_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			SetFriendApplicationRead_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetFriendshipManager()->SetFriendApplicationRead(Callback);
}

DECLARATION_TIMFriendOperationResultVector_DELEGATE(AddToBlackList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(AddToBlackList)

void UTencentIMLibrary::AddToBlackList(const TArray<FString>& userIDList, FTIMFriendOperationResultVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	AddToBlackList_TIMFriendOperationResultVectorDelegate = OnSuccessDelegate;
	AddToBlackList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			AddToBlackList_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(ToTIMFriendOperationResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			AddToBlackList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->AddToBlackList(ToIMStringVector(userIDList), CallBack);
}

DECLARATION_TIMFriendOperationResultVector_DELEGATE(DeleteFromBlackList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteFromBlackList)

void UTencentIMLibrary::DeleteFromBlackList(const TArray<FString>& userIDList, FTIMFriendOperationResultVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	DeleteFromBlackList_TIMFriendOperationResultVectorDelegate = OnSuccessDelegate;
	DeleteFromBlackList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			DeleteFromBlackList_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(ToTIMFriendOperationResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			DeleteFromBlackList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->DeleteFromBlackList(ToIMStringVector(userIDList), CallBack);
}

DECLARATION_TIMFriendInfoVector_DELEGATE(GetBlackList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetBlackList)

void UTencentIMLibrary::GetBlackList(FTIMFriendInfoVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetBlackList_TIMFriendInfoVectorDelegate = OnSuccessDelegate;
	GetBlackList_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendInfoVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendInfoVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetBlackList_TIMFriendInfoVectorDelegate.ExecuteIfBound(ToFriendInfoArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetBlackList_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->GetFriendList(CallBack);
}

DECLARATION_TIMFriendOperationResultVector_DELEGATE(CreateFriendGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(CreateFriendGroup)

void UTencentIMLibrary::CreateFriendGroup(const FString& groupName, const TArray<FString>& userIDList, FTIMFriendOperationResultVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	CreateFriendGroup_TIMFriendOperationResultVectorDelegate = OnSuccessDelegate;
	CreateFriendGroup_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			CreateFriendGroup_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(ToTIMFriendOperationResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			CreateFriendGroup_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->CreateFriendGroup(ToIMString(groupName), ToIMStringVector(userIDList), CallBack);
}

DECLARATION_TIMFriendGroupVector_DELEGATE(GetFriendGroups)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetFriendGroups)

void UTencentIMLibrary::GetFriendGroups(const TArray<FString>& groupNameList, FTIMFriendGroupVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetFriendGroups_TIMFriendGroupVectorDelegate = OnSuccessDelegate;
	GetFriendGroups_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendGroupVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendGroupVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			GetFriendGroups_TIMFriendGroupVectorDelegate.ExecuteIfBound(ToTIMFriendGroupArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			GetFriendGroups_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->GetFriendGroups(ToIMStringVector(groupNameList), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(DeleteFriendGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteFriendGroup)

void UTencentIMLibrary::DeleteFriendGroup(const TArray<FString>& groupNameList, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	DeleteFriendGroup_FailureDelegate = OnFailureDelegate;
	DeleteFriendGroup_Delegate = OnSuccessDelegate;
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
			DeleteFriendGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			DeleteFriendGroup_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetFriendshipManager()->DeleteFriendGroup(ToIMStringVector(groupNameList), Callback);
}

DECLARATION_CALLBACK_DELEGATE(RenameFriendGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(RenameFriendGroup)

void UTencentIMLibrary::RenameFriendGroup(const FString& oldName, const FString& newName, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	RenameFriendGroup_FailureDelegate = OnFailureDelegate;
	RenameFriendGroup_Delegate = OnSuccessDelegate;
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
			RenameFriendGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			RenameFriendGroup_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	Tencent_IM.GetInstance()->GetFriendshipManager()->RenameFriendGroup(ToIMString(oldName), ToIMString(newName), Callback);
}

DECLARATION_TIMFriendOperationResultVector_DELEGATE(AddFriendsToFriendGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(AddFriendsToFriendGroup)

void UTencentIMLibrary::AddFriendsToFriendGroup(const FString& groupName, const TArray<FString>& userIDList, FTIMFriendOperationResultVectorCallback OnSuccessDelegate,
                                                FIMFailureCallback OnFailureDelegate)
{
	AddFriendsToFriendGroup_TIMFriendOperationResultVectorDelegate = OnSuccessDelegate;
	AddFriendsToFriendGroup_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			AddFriendsToFriendGroup_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(ToTIMFriendOperationResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			AddFriendsToFriendGroup_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->AddFriendsToFriendGroup(ToIMString(groupName), ToIMStringVector(userIDList), CallBack);
}

DECLARATION_TIMFriendOperationResultVector_DELEGATE(DeleteFriendsFromFriendGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(DeleteFriendsFromFriendGroup)

void UTencentIMLibrary::DeleteFriendsFromFriendGroup(const FString& groupName, const TArray<FString>& userIDList, FTIMFriendOperationResultVectorCallback OnSuccessDelegate,
                                                     FIMFailureCallback OnFailureDelegate)
{
	DeleteFriendsFromFriendGroup_TIMFriendOperationResultVectorDelegate = OnSuccessDelegate;
	DeleteFriendsFromFriendGroup_FailureDelegate = OnFailureDelegate;
	class FValueCallBack : public V2TIMValueCallback<V2TIMFriendOperationResultVector>
	{
	public:
		virtual ~FValueCallBack() override
		{
		}

		/**
		 * 成功时回调，带上 T 类型的参数
		 */
		virtual void OnSuccess(const V2TIMFriendOperationResultVector& message) override
		{
			UE_LOG(LogTemp, Log, TEXT("=== SendCallback OnSuccess ======"));
			DeleteFriendsFromFriendGroup_TIMFriendOperationResultVectorDelegate.ExecuteIfBound(ToTIMFriendOperationResultArray(message));
		};
		/**
		 * 出错时回调
		 *
		 * @param error_code 错误码，详细描述请参见错误码表
		 * @param error_message 错误描述
		 */
		virtual void OnError(int error_code, const V2TIMString& error_message) override
		{
			DeleteFriendsFromFriendGroup_FailureDelegate.ExecuteIfBound(error_code, ToFString(error_message));
		}
	};
	FValueCallBack* CallBack = new FValueCallBack();
	Tencent_IM.GetInstance()->GetFriendshipManager()->DeleteFriendsFromFriendGroup(ToIMString(groupName), ToIMStringVector(userIDList), CallBack);
}

DECLARATION_CALLBACK_DELEGATE(Invite)
DECLARATION_FAILURE_CALLBACK_DELEGATE(Invite)

FString UTencentIMLibrary::Invite(const FString& invitee, const FString& data, bool onlineUserOnly, const FTIMOfflinePushInfo& offlinePushInfo, int32 timeout, FIMCallbackDelegate OnSuccessDelegate,
                                  FIMFailureCallback OnFailureDelegate)
{
	Invite_FailureDelegate = OnFailureDelegate;
	Invite_Delegate = OnSuccessDelegate;
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
			Invite_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			Invite_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	return Tencent_IM.Invite(invitee, data, onlineUserOnly, ToTIMOfflinePushInfo(offlinePushInfo), timeout, Callback);
}

DECLARATION_CALLBACK_DELEGATE(InviteInGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(InviteInGroup)

FString UTencentIMLibrary::InviteInGroup(const FString& groupID, const TArray<FString>& inviteeList, const FString& data, bool onlineUserOnly, int timeout, FIMCallbackDelegate OnSuccessDelegate,
                                         FIMFailureCallback OnFailureDelegate)
{
	InviteInGroup_FailureDelegate = OnFailureDelegate;
	InviteInGroup_Delegate = OnSuccessDelegate;
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
			InviteInGroup_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			InviteInGroup_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	return Tencent_IM.InviteInGroup(groupID, inviteeList, data, onlineUserOnly, timeout, Callback);
}

DECLARATION_CALLBACK_DELEGATE(Cancel)
DECLARATION_FAILURE_CALLBACK_DELEGATE(Cancel)

void UTencentIMLibrary::Cancel(const FString& inviteID, const FString& data, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	Cancel_FailureDelegate = OnFailureDelegate;
	Cancel_Delegate = OnSuccessDelegate;
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
			Cancel_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			Cancel_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	return Tencent_IM.Cancel(inviteID, data, Callback);
}

DECLARATION_CALLBACK_DELEGATE(Accept)
DECLARATION_FAILURE_CALLBACK_DELEGATE(Accept)

void UTencentIMLibrary::Accept(const FString& inviteID, const FString& data, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	Accept_FailureDelegate = OnFailureDelegate;
	Accept_Delegate = OnSuccessDelegate;
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
			Accept_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			Accept_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	return Tencent_IM.Accept(inviteID, data, Callback);
}

DECLARATION_CALLBACK_DELEGATE(Reject)
DECLARATION_FAILURE_CALLBACK_DELEGATE(Reject)

void UTencentIMLibrary::Reject(const FString& inviteID, const FString& data, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	Reject_FailureDelegate = OnFailureDelegate;
	Reject_Delegate = OnSuccessDelegate;
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
			Reject_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			Reject_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	return Tencent_IM.Reject(inviteID, data, Callback);
}

FTIMSignalingInfo UTencentIMLibrary::GetSignalingInfo(const FTIMMessage& msg)
{
	return ToSignalingInfo(Tencent_IM.GetSignalingInfo(ToIMMessage(msg)));
}

DECLARATION_CALLBACK_DELEGATE(AddInvitedSignaling)

DECLARATION_FAILURE_CALLBACK_DELEGATE(AddInvitedSignaling)

void UTencentIMLibrary::AddInvitedSignaling(const FTIMSignalingInfo& info, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	AddInvitedSignaling_FailureDelegate = OnFailureDelegate;
	AddInvitedSignaling_Delegate = OnSuccessDelegate;
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
			AddInvitedSignaling_Delegate.ExecuteIfBound();
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
			const std::string TempStr = error_message.CString();
			AddInvitedSignaling_FailureDelegate.ExecuteIfBound(error_code, TempStr.c_str());
		};
	};
	NormalCallback* Callback = new NormalCallback();
	return Tencent_IM.AddInvitedSignaling(ToTIMSignalingInfo(info), Callback);
}

V2TIMOfflinePushInfo UTencentIMLibrary::ToTIMOfflinePushInfo(const FTIMOfflinePushInfo& Info)
{
	V2TIMOfflinePushInfo TIMOfflinePushInfo;
	TIMOfflinePushInfo.title = ToIMString(Info.title);
	TIMOfflinePushInfo.desc = ToIMString(Info.desc);
	TIMOfflinePushInfo.ext = ToIMString(Info.ext);
	TIMOfflinePushInfo.disablePush = Info.disablePush;
	TIMOfflinePushInfo.iOSSound = ToIMString(Info.iOSSound);
	TIMOfflinePushInfo.ignoreIOSBadge = Info.ignoreIOSBadge;
	TIMOfflinePushInfo.AndroidOPPOChannelID = ToIMString(Info.AndroidOPPOChannelID);
	TIMOfflinePushInfo.AndroidVIVOClassification = Info.AndroidVIVOClassification;

	return TIMOfflinePushInfo;
}

V2TIMSignalingInfo UTencentIMLibrary::ToTIMSignalingInfo(const FTIMSignalingInfo& Info)
{
	V2TIMSignalingInfo TIMSignalingInfo;
	TIMSignalingInfo.inviteID = ToIMString(Info.inviteID);
	TIMSignalingInfo.groupID = ToIMString(Info.groupID);
	TIMSignalingInfo.inviter = ToIMString(Info.inviter);
	TIMSignalingInfo.inviteeList = ToIMStringVector(Info.inviteeList);
	TIMSignalingInfo.data = ToIMString(Info.data);
	TIMSignalingInfo.actionType = ToV2TIMSignalingActionType(Info.actionType);
	TIMSignalingInfo.timeout = Info.timeout;

	return TIMSignalingInfo;
}

FTIMSignalingInfo UTencentIMLibrary::ToSignalingInfo(const V2TIMSignalingInfo& Info)
{
	FTIMSignalingInfo TIMSignalingInfo;
	TIMSignalingInfo.inviteID = ToFString(Info.inviteID);
	TIMSignalingInfo.groupID = ToFString(Info.groupID);
	TIMSignalingInfo.inviter = ToFString(Info.inviter);
	TIMSignalingInfo.inviteeList = ToFStringArray(Info.inviteeList);
	TIMSignalingInfo.data = ToFString(Info.data);
	TIMSignalingInfo.actionType = ToTIMSignalingActionType(Info.actionType);
	TIMSignalingInfo.timeout = Info.timeout;

	return TIMSignalingInfo;
}

ETIMSignalingActionType UTencentIMLibrary::ToTIMSignalingActionType(const V2TIMSignalingActionType& Type)
{
	switch (Type)
	{
	case V2TIMSignalingActionType::SignalingActionType_Invite:
		return ETIMSignalingActionType::SignalingActionType_Invite;
		break;
	case V2TIMSignalingActionType::SignalingActionType_Cancel_Invite:
		return ETIMSignalingActionType::SignalingActionType_Cancel_Invite;
		break;
	case V2TIMSignalingActionType::SignalingActionType_Accept_Invite:
		return ETIMSignalingActionType::SignalingActionType_Accept_Invite;
		break;
	case V2TIMSignalingActionType::SignalingActionType_Reject_Invite:
		return ETIMSignalingActionType::SignalingActionType_Reject_Invite;
		break;
	case V2TIMSignalingActionType::SignalingActionType_Invite_Timeout:
		return ETIMSignalingActionType::SignalingActionType_Invite_Timeout;
		break;
	}

	return ETIMSignalingActionType::SignalingActionType_Invite;
}

V2TIMSignalingActionType UTencentIMLibrary::ToV2TIMSignalingActionType(const ETIMSignalingActionType& Type)
{
	switch (Type)
	{
	case ETIMSignalingActionType::SignalingActionType_Invite:
		return V2TIMSignalingActionType::SignalingActionType_Invite;
		break;
	case ETIMSignalingActionType::SignalingActionType_Cancel_Invite:
		return V2TIMSignalingActionType::SignalingActionType_Cancel_Invite;
		break;
	case ETIMSignalingActionType::SignalingActionType_Accept_Invite:
		return V2TIMSignalingActionType::SignalingActionType_Accept_Invite;
		break;
	case ETIMSignalingActionType::SignalingActionType_Reject_Invite:
		return V2TIMSignalingActionType::SignalingActionType_Reject_Invite;
		break;
	case ETIMSignalingActionType::SignalingActionType_Invite_Timeout:
		return V2TIMSignalingActionType::SignalingActionType_Invite_Timeout;
		break;
	}

	return V2TIMSignalingActionType::SignalingActionType_Invite;
}

TArray<FTIMFriendGroup> UTencentIMLibrary::ToTIMFriendGroupArray(const V2TIMFriendGroupVector& TIMFriendGroup)
{
	TArray<FTIMFriendGroup> Result;
	for (int i = 0; i < TIMFriendGroup.Size(); ++i)
	{
		Result.Add(ToTIMFriendGroup(TIMFriendGroup[i]));
	}

	return Result;
}

FTIMFriendGroup UTencentIMLibrary::ToTIMFriendGroup(const V2TIMFriendGroup& Group)
{
	FTIMFriendGroup TIMFriendGroup;
	TIMFriendGroup.groupName = ToFString(Group.groupName);
	TIMFriendGroup.userCount = FString::Printf(TEXT("%llu"), Group.userCount);
	TIMFriendGroup.friendList = ToFStringArray(Group.friendList);
	return TIMFriendGroup;
}

V2TIMFriendAcceptType UTencentIMLibrary::ToV2TIMFriendAcceptType(const ETIMFriendAcceptType& Type)
{
	switch (Type)
	{
	case ETIMFriendAcceptType::V2TIM_FRIEND_ACCEPT_AGREE:
		return V2TIMFriendAcceptType::V2TIM_FRIEND_ACCEPT_AGREE;
		break;
	case ETIMFriendAcceptType::V2TIM_FRIEND_ACCEPT_AGREE_AND_ADD:
		return V2TIMFriendAcceptType::V2TIM_FRIEND_ACCEPT_AGREE_AND_ADD;
		break;
	}

	return V2TIMFriendAcceptType::V2TIM_FRIEND_ACCEPT_AGREE;
}

V2TIMFriendApplication UTencentIMLibrary::ToV2TIMFriendApplication(const FTIMFriendApplication& TIMFriendApplication)
{
	V2TIMFriendApplication Application;
	Application.userID = ToIMString(TIMFriendApplication.userID);
	Application.nickName = ToIMString(TIMFriendApplication.nickName);
	Application.faceUrl = ToIMString(TIMFriendApplication.faceUrl);
	Application.addTime = FCString::Strtoui64(GetData(TIMFriendApplication.addTime),NULL, 10);
	Application.addSource = ToIMString(TIMFriendApplication.addSource);
	Application.addWording = ToIMString(TIMFriendApplication.addWording);
	Application.type = ToV2TIMFriendApplicationType(TIMFriendApplication.type);
	return Application;
}

V2TIMFriendApplicationType UTencentIMLibrary::ToV2TIMFriendApplicationType(const ETIMFriendApplicationType& Type)
{
	switch (Type)
	{
	case ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN:
		return V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN;
		break;
	case ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_SEND_OUT:
		return V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_SEND_OUT;
		break;
	case ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_BOTH:
		return V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_BOTH;
		break;
	}

	return V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN;
}

FTIMFriendApplicationResult UTencentIMLibrary::ToTIMFriendApplicationResult(const V2TIMFriendApplicationResult& TIMFriendApplicationResult)
{
	FTIMFriendApplicationResult Result;
	Result.unreadCount = FString::Printf(TEXT("%llu"), TIMFriendApplicationResult.unreadCount);
	Result.applicationList = ToTIMFriendApplicationArray(TIMFriendApplicationResult.applicationList);
	return Result;
}

V2TIMGroupMemberFullInfoVector UTencentIMLibrary::ToTIMGroupMemberFullInfoVector(const TArray<FTIMGroupMemberFullInfo>& GroupMemberFullInfoArray)
{
	V2TIMGroupMemberFullInfoVector FullInfoVector;
	for (int i = 0; i < GroupMemberFullInfoArray.Num(); ++i)
	{
		FullInfoVector.PushBack(ToV2TIMGroupMemberFullInfo(GroupMemberFullInfoArray[i]));
	}

	return FullInfoVector;
}

V2TIMGroupMemberFullInfo UTencentIMLibrary::ToV2TIMGroupMemberFullInfo(const FTIMGroupMemberFullInfo& Info)
{
	V2TIMGroupMemberFullInfo TIMGroupMemberFullInfo;
	TIMGroupMemberFullInfo.customInfo = ToV2TIMCustomInfo(Info.customInfo);
	TIMGroupMemberFullInfo.role = Info.role;
	TIMGroupMemberFullInfo.muteUntil = Info.muteUntil;
	TIMGroupMemberFullInfo.joinTime = Info.joinTime;
	TIMGroupMemberFullInfo.modifyFlag = Info.modifyFlag;

	return TIMGroupMemberFullInfo;
}

TArray<FTIMGroupMemberInfo> UTencentIMLibrary::ToTIMGroupMemberInfoArray(const V2TIMGroupMemberInfoVector& GroupMemberInfo)
{
	TArray<FTIMGroupMemberInfo> Result;
	for (int i = 0; i < GroupMemberInfo.Size(); ++i)
	{
		Result.Add(ToTIMGroupMemberInfo(GroupMemberInfo[i]));
	}
	return Result;
}

FTIMGroupMemberChangeInfo UTencentIMLibrary::ToTIMGroupMemberChangeInfo(const V2TIMGroupMemberChangeInfo& MemberChangeInfo)
{
	FTIMGroupMemberChangeInfo OutMemberChangeInfo;
	OutMemberChangeInfo.muteTime = MemberChangeInfo.muteTime;
	OutMemberChangeInfo.userID = ToFString(MemberChangeInfo.userID);
	return OutMemberChangeInfo;
}

TArray<FTIMGroupMemberChangeInfo> UTencentIMLibrary::ToTIMGroupMemberChangeInfoArray(const V2TIMGroupMemberChangeInfoVector& MemberChangeInfo)
{
	TArray<FTIMGroupMemberChangeInfo> TIMFriendApplicationArray;
	for (int i = 0; i < MemberChangeInfo.Size(); ++i)
	{
		TIMFriendApplicationArray.Add(ToTIMGroupMemberChangeInfo(MemberChangeInfo[i]));
	}
	return TIMFriendApplicationArray;
}

ETIMGroupInfoChangeType UTencentIMLibrary::ToTIMGroupInfoChangeType(const V2TIMGroupInfoChangeType& GroupInfo)
{
	// ETIMGroupInfoChangeType InfoChangeType;
	switch (GroupInfo)
	{
	case V2TIM_GROUP_INFO_CHANGE_TYPE_NAME:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_NAME;
	case V2TIM_GROUP_INFO_CHANGE_TYPE_INTRODUCTION:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_INTRODUCTION;
	case V2TIM_GROUP_INFO_CHANGE_TYPE_NOTIFICATION:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_NOTIFICATION;
	case V2TIM_GROUP_INFO_CHANGE_TYPE_FACE:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_FACE;
	case V2TIM_GROUP_INFO_CHANGE_TYPE_OWNER:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_OWNER;
	case V2TIM_GROUP_INFO_CHANGE_TYPE_CUSTOM:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_CUSTOM;
	default:
		return ETIMGroupInfoChangeType::V2TIM_GROUP_INFO_CHANGE_TYPE_NAME;
	}
}

FTIMGroupChangeInfo UTencentIMLibrary::ToTIMGroupChangeInfo(const V2TIMGroupChangeInfo& GroupChangeInfo)
{
	FTIMGroupChangeInfo OutGroupChangeInfo;
	OutGroupChangeInfo.key = ToFString(GroupChangeInfo.key);
	OutGroupChangeInfo.value = ToFString(GroupChangeInfo.value);
	OutGroupChangeInfo.type = ToTIMGroupInfoChangeType(GroupChangeInfo.type);
	return OutGroupChangeInfo;
}

TArray<FTIMGroupChangeInfo> UTencentIMLibrary::ToTIMGroupChangeInfoArray(const V2TIMGroupChangeInfoVector& GroupChangeInfo)
{
	TArray<FTIMGroupChangeInfo> TIMFriendApplicationArray;
	for (int i = 0; i < GroupChangeInfo.Size(); ++i)
	{
		TIMFriendApplicationArray.Add(ToTIMGroupChangeInfo(GroupChangeInfo[i]));
	}
	return TIMFriendApplicationArray;
}

TArray<FTIMFriendApplication> UTencentIMLibrary::ToTIMFriendApplicationArray(const V2TIMFriendApplicationVector& FriendApplicationVector)
{
	TArray<FTIMFriendApplication> TIMFriendApplicationArray;
	for (int i = 0; i < FriendApplicationVector.Size(); ++i)
	{
		TIMFriendApplicationArray.Add(ToTIMFriendApplication(FriendApplicationVector[i]));
	}
	return TIMFriendApplicationArray;
}

FTIMFriendApplication UTencentIMLibrary::ToTIMFriendApplication(const V2TIMFriendApplication& IMFriendApplication)
{
	FTIMFriendApplication TIMFriendApplication;
	TIMFriendApplication.userID = ToFString(IMFriendApplication.userID);
	TIMFriendApplication.nickName = ToFString(IMFriendApplication.nickName);
	TIMFriendApplication.faceUrl = ToFString(IMFriendApplication.faceUrl);
	TIMFriendApplication.addTime = FString::Printf(TEXT("%llu"), IMFriendApplication.addTime);
	TIMFriendApplication.addSource = ToFString(IMFriendApplication.addSource);
	TIMFriendApplication.addWording = ToFString(IMFriendApplication.addWording);
	TIMFriendApplication.type = ToTIMFriendApplicationType(IMFriendApplication.type);

	return TIMFriendApplication;
}

ETIMFriendApplicationType UTencentIMLibrary::ToTIMFriendApplicationType(const V2TIMFriendApplicationType& Type)
{
	switch (Type)
	{
	case V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN:
		return ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN;
		break;
	case V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_SEND_OUT:
		return ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_SEND_OUT;
		break;
	case V2TIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_BOTH:
		return ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_BOTH;
		break;
	}

	return ETIMFriendApplicationType::V2TIM_FRIEND_APPLICATION_COME_IN;
}

TArray<FTIMFriendCheckResult> UTencentIMLibrary::ToTIMFriendCheckResultArray(const V2TIMFriendCheckResultVector& TIMFriendCheckResultVector)
{
	TArray<FTIMFriendCheckResult> Result;
	for (int i = 0; i < TIMFriendCheckResultVector.Size(); ++i)
	{
		Result.Add(ToFTIMFriendCheckResult(TIMFriendCheckResultVector[i]));
	}
	return Result;
}

FTIMFriendCheckResult UTencentIMLibrary::ToFTIMFriendCheckResult(const V2TIMFriendCheckResult& FriendCheckResult)
{
	FTIMFriendCheckResult TIMFriendCheckResult;

	TIMFriendCheckResult.userID = ToFString(FriendCheckResult.userID);
	TIMFriendCheckResult.resultCode = FriendCheckResult.resultCode;
	TIMFriendCheckResult.resultInfo = ToFString(FriendCheckResult.resultInfo);
	TIMFriendCheckResult.relationType = ToTIMFriendRelationType(FriendCheckResult.relationType);
	return TIMFriendCheckResult;
}

V2TIMGroupMemberInfoResult UTencentIMLibrary::ToTIMGroupMemberInfoResult(const FTIMGroupMemberInfoResult& Result)
{
	V2TIMGroupMemberInfoResult TIMGroupMemberInfoResult;
	TIMGroupMemberInfoResult.nextSequence = FCString::Strtoui64(GetData(Result.nextSequence), nullptr, 10);
	TIMGroupMemberInfoResult.memberInfoList = ToTIMGroupMemberFullInfoVector(Result.memberInfoList);
	return TIMGroupMemberInfoResult;
}

ETIMFriendRelationType UTencentIMLibrary::ToTIMFriendRelationType(const V2TIMFriendRelationType& Type)
{
	switch (Type)
	{
	case V2TIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_NONE:
		return ETIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_NONE;
		break;
	case V2TIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST:
		return ETIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST;
		break;
	case V2TIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_IN_OTHER_FRIEND_LIST:
		return ETIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_IN_OTHER_FRIEND_LIST;
		break;
	case V2TIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_BOTH_WAY:
		return ETIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_BOTH_WAY;
		break;
	}

	return ETIMFriendRelationType::V2TIM_FRIEND_RELATION_TYPE_NONE;
}

V2TIMFriendCheckResultVector UTencentIMLibrary::ToV2TIMFriendCheckResultVector(const TArray<FTIMFriendCheckResult>& TIMFriendCheckResult)
{
	V2TIMFriendCheckResultVector TIMFriendCheckResultVector;
	for (auto& FriendCheck : TIMFriendCheckResult)
	{
		TIMFriendCheckResultVector.PushBack(ToV2TIMFriendCheckResult(FriendCheck));
	}
	return TIMFriendCheckResultVector;
}

V2TIMFriendCheckResult UTencentIMLibrary::ToV2TIMFriendCheckResult(const FTIMFriendCheckResult& FriendCheckResult)
{
	V2TIMFriendCheckResult TIMFriendCheckResult;
	TIMFriendCheckResult.userID = ToIMString(FriendCheckResult.userID);
	TIMFriendCheckResult.resultCode = FriendCheckResult.resultCode;
	TIMFriendCheckResult.resultInfo = ToIMString(FriendCheckResult.resultInfo);
	TIMFriendCheckResult.relationType = V2TIMFriendRelationType(FriendCheckResult.relationType);
	return TIMFriendCheckResult;
}

FTIMFriendOperationResult UTencentIMLibrary::ToFriendOperationResult(const V2TIMFriendOperationResult& FriendOperationResult)
{
	FTIMFriendOperationResult Result;
	Result.userID = ToFString(FriendOperationResult.userID);
	Result.resultCode = FriendOperationResult.resultCode;
	Result.resultInfo = ToFString(FriendOperationResult.resultInfo);

	return Result;
}

V2TIMFriendType UTencentIMLibrary::ToTIMFriendType(ETIMFriendType Type)
{
	switch (Type)
	{
	case ETIMFriendType::V2TIM_FRIEND_TYPE_SINGLE:
		return V2TIMFriendType::V2TIM_FRIEND_TYPE_SINGLE;
		break;
	case ETIMFriendType::V2TIM_FRIEND_TYPE_BOTH:
		return V2TIMFriendType::V2TIM_FRIEND_TYPE_BOTH;
		break;
	}

	return V2TIMFriendType::V2TIM_FRIEND_TYPE_SINGLE;
}

V2TIMFriendAddApplication UTencentIMLibrary::ToTIMFriendAddApplication(const FTIMFriendAddApplication& FriendAddApplication)
{
	V2TIMFriendAddApplication Application;
	Application.userID = ToIMString(FriendAddApplication.userID);
	Application.friendRemark = ToIMString(FriendAddApplication.friendRemark);
	Application.friendGroup = ToIMString(FriendAddApplication.friendGroup);
	Application.addWording = ToIMString(FriendAddApplication.addWording);
	Application.addSource = ToIMString(FriendAddApplication.addSource);
	Application.userID = ToIMString(FriendAddApplication.userID);
	Application.addType = ToTIMFriendType(FriendAddApplication.addType);

	return Application;
}

V2TIMFriendSearchParam UTencentIMLibrary::ToTIMFriendSearchParam(const FTIMFriendSearchParam& FriendSearchParam)
{
	V2TIMFriendSearchParam TIMFriendSearchParam;
	TIMFriendSearchParam.keywordList = ToIMStringVector(FriendSearchParam.keywordList);
	TIMFriendSearchParam.isSearchUserID = FriendSearchParam.isSearchUserID;
	TIMFriendSearchParam.isSearchNickName = FriendSearchParam.isSearchNickName;
	TIMFriendSearchParam.isSearchRemark = FriendSearchParam.isSearchRemark;

	return TIMFriendSearchParam;
}

TArray<FTIMCreateGroupMemberInfo> UTencentIMLibrary::ToGroupMemberInfoArray(const V2TIMCreateGroupMemberInfoVector& MemberInfoVector)
{
	TArray<FTIMCreateGroupMemberInfo> GroupInfo;
	for (int i = 0; i < MemberInfoVector.Size(); i++)
	{
		GroupInfo.Add(ToGroupMemberInfo(MemberInfoVector[i]));
	}
	return TArray<FTIMCreateGroupMemberInfo>();
}

V2TIMCreateGroupMemberInfoVector UTencentIMLibrary::ToCreateGroupMemberInfoVector(const TArray<FTIMCreateGroupMemberInfo>& MemberInfoVector)
{
	V2TIMCreateGroupMemberInfoVector GroupMemberInfoVector;
	for (FTIMCreateGroupMemberInfo InfoVector : MemberInfoVector)
	{
		GroupMemberInfoVector.PushBack(ToIMGroupMemberInfo(InfoVector));
	}
	return GroupMemberInfoVector;
}

FTIMGroupInfo UTencentIMLibrary::ToGroupInfo(const V2TIMGroupInfo& GroupInfo)
{
	FTIMGroupInfo TIMGroupInfo;
	TIMGroupInfo.groupID = ToFString(GroupInfo.groupID);
	TIMGroupInfo.groupType = ToFString(GroupInfo.groupType);
	TIMGroupInfo.groupName = ToFString(GroupInfo.groupName);
	TIMGroupInfo.notification = ToFString(GroupInfo.notification);
	TIMGroupInfo.introduction = ToFString(GroupInfo.introduction);
	TIMGroupInfo.faceURL = ToFString(GroupInfo.faceURL);
	TIMGroupInfo.allMuted = GroupInfo.allMuted;
	TIMGroupInfo.owner = ToFString(GroupInfo.owner);
	TIMGroupInfo.createTime = (GroupInfo.createTime);
	TIMGroupInfo.groupAddOpt = ToGroupAddOpt(GroupInfo.groupAddOpt);
	TIMGroupInfo.lastInfoTime = (GroupInfo.lastInfoTime);
	TIMGroupInfo.lastMessageTime = (GroupInfo.lastMessageTime);
	TIMGroupInfo.memberCount = (GroupInfo.memberCount);
	TIMGroupInfo.onlineCount = (GroupInfo.onlineCount);
	TIMGroupInfo.memberMaxCount = (GroupInfo.memberMaxCount);
	TIMGroupInfo.role = GroupInfo.role;
	TIMGroupInfo.recvOpt = ToReceiveMessageOpt(GroupInfo.recvOpt);
	TIMGroupInfo.joinTime = GroupInfo.joinTime;
	TIMGroupInfo.modifyFlag = GroupInfo.modifyFlag;
	return TIMGroupInfo;
}

V2TIMGroupInfo UTencentIMLibrary::ToTIMGroupInfo(const FTIMGroupInfo& GroupInfo)
{
	V2TIMGroupInfo TIMGroupInfo;
	TIMGroupInfo.groupID = ToIMString(GroupInfo.groupID);
	TIMGroupInfo.groupType = ToIMString(GroupInfo.groupType);
	TIMGroupInfo.groupName = ToIMString(GroupInfo.groupName);
	TIMGroupInfo.notification = ToIMString(GroupInfo.notification);
	TIMGroupInfo.introduction = ToIMString(GroupInfo.introduction);
	TIMGroupInfo.faceURL = ToIMString(GroupInfo.faceURL);
	TIMGroupInfo.allMuted = GroupInfo.allMuted;
	TIMGroupInfo.owner = ToIMString(GroupInfo.owner);
	TIMGroupInfo.createTime = (GroupInfo.createTime);
	TIMGroupInfo.groupAddOpt = ToTIMGroupAddOpt(GroupInfo.groupAddOpt);
	TIMGroupInfo.lastInfoTime = (GroupInfo.lastInfoTime);
	TIMGroupInfo.lastMessageTime = (GroupInfo.lastMessageTime);
	TIMGroupInfo.memberCount = (GroupInfo.memberCount);
	TIMGroupInfo.onlineCount = (GroupInfo.onlineCount);
	TIMGroupInfo.memberMaxCount = (GroupInfo.memberMaxCount);
	TIMGroupInfo.role = GroupInfo.role;
	TIMGroupInfo.recvOpt = ToTIMReceiveMessageOpt(GroupInfo.recvOpt);
	TIMGroupInfo.joinTime = GroupInfo.joinTime;
	TIMGroupInfo.modifyFlag = GroupInfo.modifyFlag;
	return TIMGroupInfo;
}

FTIMCreateGroupMemberInfo UTencentIMLibrary::ToGroupMemberInfo(const V2TIMCreateGroupMemberInfo& GroupMemberInfo)
{
	FTIMCreateGroupMemberInfo GroupMemInfo;
	GroupMemInfo.role = GroupMemberInfo.role;
	GroupMemInfo.userID = ToFString(GroupMemberInfo.userID);
	return GroupMemInfo;
}

V2TIMCreateGroupMemberInfo UTencentIMLibrary::ToIMGroupMemberInfo(const FTIMCreateGroupMemberInfo& GroupMemberInfo)
{
	V2TIMCreateGroupMemberInfo GroupMemInfo;
	GroupMemInfo.role = GroupMemberInfo.role;
	GroupMemInfo.userID = ToIMString(GroupMemberInfo.userID);
	return GroupMemInfo;
}

//------------------------------------------------------
//base convert function


V2TIMString UTencentIMLibrary::ToIMString(const FString& InStr)
{
	// const char* OutIMString = TCHAR_TO_ANSI(*InStr);
	std::string strInputMessage(TCHAR_TO_UTF8(*InStr));
	const char* charInputMessage = strInputMessage.c_str();
	return (charInputMessage);
}

FString UTencentIMLibrary::ToFString(const V2TIMString& InStr)
{
	// std::string stdStrTemp2(InStr.CString());
	// FString tempUserText = stdStrTemp2.c_str();
	// // V2TIMString TempStr=InStr;
	// return tempUserText;
	const char* msgEle = InStr.CString();
	return UTF8_TO_TCHAR(msgEle);
}

V2TIMStringVector UTencentIMLibrary::ToIMStringVector(TArray<FString> InStrArray)
{
	V2TIMStringVector StrVector;
	for (FString Str : InStrArray)
	{
		StrVector.PushBack(ToIMString(Str));
	}
	return StrVector;
}

TArray<FString> UTencentIMLibrary::ToFStringArray(V2TIMStringVector TIMStringVector)
{
	TArray<FString> res;
	for (int i = 0; i < TIMStringVector.Size(); i++)
	{
		res.Add(ToFString(TIMStringVector[i]));
	}

	return res;
}

TMap<FString, FString> UTencentIMLibrary::ToFStringMap(V2TIMGroupAttributeMap TIMStringVector)
{
	TMap<FString, FString> OutMap;
	for (int i = 0; i < TIMStringVector.Size(); i++)
	{
		OutMap.Add(ToFString(TIMStringVector.AllKeys()[i]), ToFString(TIMStringVector.Get(TIMStringVector.AllKeys()[i])));
	}
	return OutMap;
}

ELoginStatus UTencentIMLibrary::ToTIMLoginStatus(const V2TIMLoginStatus& Status)
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

FTIMUserFullInfo UTencentIMLibrary::ToTIMUserFullInfo(const V2TIMUserFullInfo& info)
{
	FTIMUserFullInfo UserInfo = FTIMUserFullInfo();
	UserInfo.userID = ToFString(info.userID);
	UserInfo.nickName = ToFString(info.nickName);
	UserInfo.faceURL = ToFString(info.faceURL);
	UserInfo.selfSignature = ToFString(info.selfSignature);
	UserInfo.role = info.role;
	UserInfo.level = info.level;
	UserInfo.birthday = info.birthday;
	UserInfo.allowType = ToTIMAllowType(info.allowType);
	UserInfo.customInfo = ToTIMCustomInfo(info.customInfo);
	UserInfo.modifyFlag = info.modifyFlag;
	return UserInfo;
}

V2TIMUserFullInfo UTencentIMLibrary::ToV2TIMUserFullInfo(const FTIMUserFullInfo& info)
{
	V2TIMUserFullInfo UserInfo = V2TIMUserFullInfo();
	UserInfo.userID = ToIMString(info.userID);
	UserInfo.nickName = ToIMString(info.nickName);
	UserInfo.faceURL = ToIMString(info.faceURL);
	UserInfo.selfSignature = ToIMString(info.selfSignature);
	UserInfo.role = info.role;
	UserInfo.level = info.level;
	UserInfo.birthday = info.birthday;
	UserInfo.allowType = ToV2TIMAllowType(info.allowType);
	UserInfo.customInfo = ToV2TIMCustomInfo(info.customInfo);
	UserInfo.modifyFlag = info.modifyFlag;
	return UserInfo;
}


ELoginStatus UTencentIMLibrary::GetLoginStatus()
{
	return Tencent_IM.GetLoginStatus();
}

ETIMGender UTencentIMLibrary::ToTIMGender(V2TIMGender Gender)
{
	switch (Gender)
	{
	case V2TIM_GENDER_UNKNOWN:
		return ETIMGender::V2TIM_GENDER_UNKNOWN;
		break;
	case V2TIM_GENDER_MALE:
		return ETIMGender::V2TIM_GENDER_MALE;
		break;
	case V2TIM_GENDER_FEMALE:
		return ETIMGender::V2TIM_GENDER_FEMALE;
		break;
	default:
		return ETIMGender::V2TIM_GENDER_UNKNOWN;
	}
}

ETIMFriendAllowType UTencentIMLibrary::ToTIMAllowType(V2TIMFriendAllowType AllowType)
{
	switch (AllowType)
	{
	case V2TIM_FRIEND_ALLOW_ANY:
		return ETIMFriendAllowType::V2TIM_FRIEND_ALLOW_ANY;
		break;
	case V2TIM_FRIEND_NEED_CONFIRM:
		return ETIMFriendAllowType::V2TIM_FRIEND_NEED_CONFIRM;
		break;
	case V2TIM_FRIEND_DENY_ANY:
		return ETIMFriendAllowType::V2TIM_FRIEND_DENY_ANY;
		break;
	default:
		return ETIMFriendAllowType::V2TIM_FRIEND_NEED_CONFIRM;
	}
}

V2TIMFriendAllowType UTencentIMLibrary::ToV2TIMAllowType(const ETIMFriendAllowType& AllowType)
{
	switch (AllowType)
	{
	case ETIMFriendAllowType::V2TIM_FRIEND_ALLOW_ANY:
		return V2TIMFriendAllowType::V2TIM_FRIEND_ALLOW_ANY;
		break;
	case ETIMFriendAllowType::V2TIM_FRIEND_NEED_CONFIRM:
		return V2TIMFriendAllowType::V2TIM_FRIEND_NEED_CONFIRM;
		break;
	case ETIMFriendAllowType::V2TIM_FRIEND_DENY_ANY:
		return V2TIMFriendAllowType::V2TIM_FRIEND_DENY_ANY;
		break;
	default:
		return V2TIMFriendAllowType::V2TIM_FRIEND_NEED_CONFIRM;
	}
}

FBuffer UTencentIMLibrary::ToBuffer(V2TIMBuffer TIMBuffer)
{
	FBuffer Buffer;
	Buffer.Buffer.AddUninitialized(TIMBuffer.Size());
	FMemory::Memcpy(Buffer.Buffer.GetData(), TIMBuffer.Data(), TIMBuffer.Size());
	return Buffer;
}

V2TIMBuffer UTencentIMLibrary::ToTIMBuffer(FBuffer TIMBuffer)
{
	V2TIMBuffer Buffer(TIMBuffer.Buffer.GetData(), TIMBuffer.Buffer.Num());
	return Buffer;
}

TMap<FString, FBuffer> UTencentIMLibrary::ToTIMCustomInfo_(V2TIMCustomInfo CustomInfo)
{
	TMap<FString, FBuffer> OutCustomInfo;
	for (int32 i = 0; i < CustomInfo.AllKeys().Size(); i++)
	{
		V2TIMString Key = CustomInfo.AllKeys()[i];
		OutCustomInfo.Add(ToFString(Key), ToBuffer(CustomInfo.Get(Key)));
	}
	return OutCustomInfo;
}

V2TIMCustomInfo UTencentIMLibrary::ToV2TIMCustomInfo(TMap<FString, FBuffer> CustomInfo)
{
	V2TIMCustomInfo OutCustomInfo;
	TArray<FString> Keys;
	CustomInfo.GetKeys(Keys);
	for (int32 i = 0; i < Keys.Num(); i++)
	{
		FString Key = Keys[i];
		OutCustomInfo.Insert(ToIMString(Key), ToTIMBuffer(CustomInfo.FindRef(Key)));
	}
	return OutCustomInfo;
}


TMap<FString, V2TIMBuffer> UTencentIMLibrary::ToTIMCustomInfo(V2TIMCustomInfo CustomInfo)
{
	TMap<FString, V2TIMBuffer> OutCustomInfo;
	for (int32 i = 0; i < CustomInfo.AllKeys().Size(); i++)
	{
		V2TIMString Key = CustomInfo.AllKeys()[i];
		OutCustomInfo.Add(ToFString(Key), CustomInfo.Get(Key));
	}
	return OutCustomInfo;
}

V2TIMCustomInfo UTencentIMLibrary::ToV2TIMCustomInfo(TMap<FString, V2TIMBuffer> CustomInfo)
{
	V2TIMCustomInfo OutCustomInfo;
	TArray<FString> Keys;
	CustomInfo.GetKeys(Keys);
	for (int32 i = 0; i < Keys.Num(); i++)
	{
		FString Key = Keys[i];
		OutCustomInfo.Insert(ToIMString(Key), CustomInfo.FindRef(Key));
	}
	return OutCustomInfo;
}

TArray<FTIMUserFullInfo> UTencentIMLibrary::ToTIMUserFullInfoArray(const V2TIMUserFullInfoVector& FullInfoVector)
{
	TArray<FTIMUserFullInfo> Info;
	for (int32 i = 0; i < FullInfoVector.Size(); i++)
	{
		Info.Add(ToTIMUserFullInfo(FullInfoVector[i]));
	}
	return Info;
}

V2TIMMessage UTencentIMLibrary::ToIMMessage(const FTIMMessage& TimMessage)
{
	V2TIMMessage OutTimMessage;
	OutTimMessage.msgID = ToIMString(TimMessage.msgID);
	OutTimMessage.timestamp = TimMessage.timestamp;
	OutTimMessage.sender = ToIMString(TimMessage.sender);
	OutTimMessage.nickName = ToIMString(TimMessage.nickName);
	OutTimMessage.friendRemark = ToIMString(TimMessage.friendRemark);
	OutTimMessage.nameCard = ToIMString(TimMessage.nameCard);
	OutTimMessage.faceURL = ToIMString(TimMessage.faceURL);
	OutTimMessage.groupID = ToIMString(TimMessage.groupID);
	OutTimMessage.userID = ToIMString(TimMessage.userID);
	OutTimMessage.seq = FCString::Strtoui64(GetData(TimMessage.seq),NULL, 10);
	OutTimMessage.random = FCString::Strtoui64(GetData(TimMessage.random),NULL, 10);
	OutTimMessage.status = ToTimMessageStatus(TimMessage.status);
	OutTimMessage.isSelf = TimMessage.isSelf;
	OutTimMessage.isRead = TimMessage.isRead;
	OutTimMessage.isPeerRead = TimMessage.isPeerRead;
	OutTimMessage.groupAtUserList = ToIMStringVector(TimMessage.groupAtUserList);
	OutTimMessage.elemList = ToElemVector(TimMessage.elemList);
	OutTimMessage.localCustomData = ToTIMBuffer(TimMessage.localCustomData);
	OutTimMessage.localCustomInt = TimMessage.localCustomInt;
	OutTimMessage.cloudCustomData = ToTIMBuffer(TimMessage.cloudCustomData);
	OutTimMessage.isExcludedFromUnreadCount = TimMessage.isExcludedFromUnreadCount;
	OutTimMessage.isExcludedFromLastMessage = TimMessage.isExcludedFromLastMessage;
	OutTimMessage.targetGroupMemberList = ToIMStringVector(TimMessage.targetGroupMemberList);
	return OutTimMessage;
}

V2TIMMessageStatus UTencentIMLibrary::ToTimMessageStatus(const ETIMMessageStatus& MessageStatus)
{
	//todo 
	
	return V2TIMMessageStatus();
}

FTIMMessage UTencentIMLibrary::ToMessage(const V2TIMMessage& TimMessage)
{
	FTIMMessage OutTIMMessage = FTIMMessage();
	OutTIMMessage.msgID = ToFString(TimMessage.msgID);
	OutTIMMessage.timestamp = TimMessage.timestamp;
	OutTIMMessage.sender = ToFString(TimMessage.sender);
	OutTIMMessage.nickName = ToFString(TimMessage.nickName);
	OutTIMMessage.friendRemark = ToFString(TimMessage.friendRemark);
	OutTIMMessage.nameCard = ToFString(TimMessage.nameCard);
	OutTIMMessage.faceURL = ToFString(TimMessage.faceURL);
	OutTIMMessage.groupID = ToFString(TimMessage.groupID);
	OutTIMMessage.userID = ToFString(TimMessage.userID);
	OutTIMMessage.seq = FString::Printf(TEXT("%lld"), TimMessage.seq);
	OutTIMMessage.random = FString::Printf(TEXT("%lld"), TimMessage.random);
	OutTIMMessage.isSelf = (TimMessage.isSelf);
	OutTIMMessage.isRead = (TimMessage.isRead);
	OutTIMMessage.isPeerRead = (TimMessage.isPeerRead);
	OutTIMMessage.groupAtUserList = ToFStringArray(TimMessage.groupAtUserList);
	OutTIMMessage.elemList = ToTIMElemStringArray(TimMessage.elemList);
	OutTIMMessage.localCustomData = ToBuffer(TimMessage.localCustomData);
	OutTIMMessage.localCustomInt = (TimMessage.localCustomInt);
	OutTIMMessage.cloudCustomData = ToBuffer(TimMessage.localCustomData);
	OutTIMMessage.isExcludedFromUnreadCount = (TimMessage.isExcludedFromUnreadCount);
	OutTIMMessage.isExcludedFromLastMessage = (TimMessage.isExcludedFromLastMessage);
	OutTIMMessage.targetGroupMemberList = ToFStringArray(TimMessage.targetGroupMemberList);

	return OutTIMMessage;
}

TArray<FTIMMessage> UTencentIMLibrary::ToMessageArray(const V2TIMMessageVector& MessageVector)
{
	TArray<FTIMMessage> IMMessages;
	for (int32 i = 0; i < MessageVector.Size(); i++)
	{
		IMMessages.Add(ToMessage(MessageVector[i]));
	}
	return IMMessages;
}

V2TIMMessageVector UTencentIMLibrary::ToV2IMMessageArray(const TArray<FTIMMessage>& MessageArray)
{
	V2TIMMessageVector IMMessageVector;
	for (int32 i = 0; i < MessageArray.Num(); i++)
	{
		IMMessageVector.PushBack(ToIMMessage(MessageArray[i]));
	}
	return IMMessageVector;
}

V2TIMReceiveMessageOpt UTencentIMLibrary::ToTIMReceiveMessageOpt(const ETIMReceiveMessageOpt& MsgOpt)
{
	switch (MsgOpt)
	{
	case ETIMReceiveMessageOpt::V2TIM_RECEIVE_MESSAGE:
		return V2TIMReceiveMessageOpt::V2TIM_RECEIVE_MESSAGE;
	case ETIMReceiveMessageOpt::V2TIM_NOT_RECEIVE_MESSAGE:
		return V2TIMReceiveMessageOpt::V2TIM_NOT_RECEIVE_MESSAGE;
	case ETIMReceiveMessageOpt::V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE:
		return V2TIMReceiveMessageOpt::V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE;
	default:
		return V2TIMReceiveMessageOpt::V2TIM_RECEIVE_MESSAGE;
	}
}

ETIMReceiveMessageOpt UTencentIMLibrary::ToReceiveMessageOpt(const V2TIMReceiveMessageOpt& MsgOpt)
{
	switch (MsgOpt)
	{
	case V2TIM_RECEIVE_MESSAGE:
		return ETIMReceiveMessageOpt::V2TIM_RECEIVE_MESSAGE;
	case V2TIM_NOT_RECEIVE_MESSAGE:
		return ETIMReceiveMessageOpt::V2TIM_NOT_RECEIVE_MESSAGE;
	case V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE:
		return ETIMReceiveMessageOpt::V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE;
	default:
		return ETIMReceiveMessageOpt::V2TIM_RECEIVE_MESSAGE;
	}
}

FTIMReceiveMessageOptInfo UTencentIMLibrary::ToReceiveMessageOptInfo(const V2TIMReceiveMessageOptInfo& OptInfo)
{
	FTIMReceiveMessageOptInfo MsgInfo = FTIMReceiveMessageOptInfo();
	MsgInfo.receiveOpt = ToReceiveMessageOpt(OptInfo.receiveOpt);
	MsgInfo.userID = ToFString(OptInfo.userID);
	return MsgInfo;
}

V2TIMReceiveMessageOptInfo UTencentIMLibrary::ToTIMReceiveMessageOptInfo(const FTIMReceiveMessageOptInfo& OptInfo)
{
	V2TIMReceiveMessageOptInfo MsgInfo = V2TIMReceiveMessageOptInfo();
	MsgInfo.receiveOpt = ToTIMReceiveMessageOpt(OptInfo.receiveOpt);
	MsgInfo.userID = ToIMString(OptInfo.userID);
	return MsgInfo;
}


TArray<FTIMReceiveMessageOptInfo> UTencentIMLibrary::ToReceiveMessageOptInfoArray(const V2TIMReceiveMessageOptInfoVector& MessageOptInfoVector)
{
	TArray<FTIMReceiveMessageOptInfo> OutOpt;
	for (int32 i = 0; i < MessageOptInfoVector.Size(); i++)
	{
		OutOpt.Add(ToReceiveMessageOptInfo(MessageOptInfoVector[i]));
	}
	return OutOpt;
}

V2TIMMessageListGetOption UTencentIMLibrary::ToIMMessageListGetOption(const FTIMMessageListGetOption& Option)
{
	V2TIMMessageListGetOption ListenOption = V2TIMMessageListGetOption();
	ListenOption.getType = ToTIMMessageGetType(Option.getType);
	ListenOption.userID = ToIMString(Option.userID);
	ListenOption.groupID = ToIMString(Option.groupID);
	ListenOption.count = Option.count;
	*ListenOption.lastMsg = ToIMMessage(Option.lastMsg);
	return ListenOption;
}

ETIMMessageGetType UTencentIMLibrary::ToMessageGetType(V2TIMMessageGetType& MessageType)
{
	switch (MessageType)
	{
	case V2TIM_GET_CLOUD_OLDER_MSG:
		return ETIMMessageGetType::V2TIM_GET_CLOUD_OLDER_MSG;
	case V2TIM_GET_CLOUD_NEWER_MSG:
		return ETIMMessageGetType::V2TIM_GET_CLOUD_NEWER_MSG;
	case V2TIM_GET_LOCAL_OLDER_MSG:
		return ETIMMessageGetType::V2TIM_GET_LOCAL_OLDER_MSG;
	case V2TIM_GET_LOCAL_NEWER_MSG:
		return ETIMMessageGetType::V2TIM_GET_LOCAL_NEWER_MSG;
	default:
		return ETIMMessageGetType::V2TIM_GET_CLOUD_OLDER_MSG;
	}
}

V2TIMMessageGetType UTencentIMLibrary::ToTIMMessageGetType(const ETIMMessageGetType& MessageType)
{
	switch (MessageType)
	{
	case ETIMMessageGetType::V2TIM_GET_CLOUD_OLDER_MSG:
		return V2TIMMessageGetType::V2TIM_GET_CLOUD_OLDER_MSG;
	case ETIMMessageGetType::V2TIM_GET_CLOUD_NEWER_MSG:
		return V2TIMMessageGetType::V2TIM_GET_CLOUD_NEWER_MSG;
	case ETIMMessageGetType::V2TIM_GET_LOCAL_OLDER_MSG:
		return V2TIMMessageGetType::V2TIM_GET_LOCAL_OLDER_MSG;
	case ETIMMessageGetType::V2TIM_GET_LOCAL_NEWER_MSG:
		return V2TIMMessageGetType::V2TIM_GET_LOCAL_NEWER_MSG;
	default:
		return V2TIMMessageGetType::V2TIM_GET_CLOUD_OLDER_MSG;
	}
}

V2TIMMessageSearchParam UTencentIMLibrary::ToTIMessageSearchParam(const FTIMMessageSearchParam& MessageSearchParam)
{
	V2TIMMessageSearchParam OutParam = V2TIMMessageSearchParam();
	OutParam.keywordList = ToIMStringVector(MessageSearchParam.keywordList);
	OutParam.keywordListMatchType = ToTIMKeywordListMatchType(MessageSearchParam.keywordListMatchType);
	OutParam.senderUserIDList = ToIMStringVector(MessageSearchParam.senderUserIDList);
	OutParam.messageTypeList = ToTIMElemTypeVector(MessageSearchParam.messageTypeList);
	OutParam.conversationID = ToIMString(MessageSearchParam.conversationID);
	OutParam.searchTimePosition = MessageSearchParam.searchTimePosition;
	OutParam.searchTimePeriod = MessageSearchParam.searchTimePeriod;
	OutParam.pageIndex = MessageSearchParam.pageIndex;
	OutParam.pageSize = MessageSearchParam.pageSize;
	return OutParam;
}

ETIMKeywordListMatchType UTencentIMLibrary::ToKeywordListMatchType(const V2TIMKeywordListMatchType& MessageSearchParam)
{
	switch (MessageSearchParam)
	{
	case V2TIM_KEYWORD_LIST_MATCH_TYPE_OR:
		return ETIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_OR;
	case V2TIM_KEYWORD_LIST_MATCH_TYPE_AND:
		return ETIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_AND;
	default:
		return ETIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_OR;
	}
}

V2TIMKeywordListMatchType UTencentIMLibrary::ToTIMKeywordListMatchType(const ETIMKeywordListMatchType& MessageSearchParam)
{
	switch (MessageSearchParam)
	{
	case ETIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_OR:
		return V2TIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_OR;
	case ETIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_AND:
		return V2TIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_AND;
	default:
		return V2TIMKeywordListMatchType::V2TIM_KEYWORD_LIST_MATCH_TYPE_OR;
	}
}

FTIMElem UTencentIMLibrary::ToElem(const V2TIMElem& TimElem)
{
	FTIMElem Element;
	Element.elemType = ToElemType(TimElem.elemType);
	return Element;
}

V2TIMElem UTencentIMLibrary::ToTIMElem(const FTIMElem& TimElem)
{
	V2TIMElem Element;
	Element.elemType = ToTIMElemType(TimElem.elemType);
	return Element;
}

// TArray<FTIMElem> UTencentIMLibrary::ToTIMElemArray(const V2TIMElemVector& ElementVector)
// {
// 	TArray<FTIMElem> ElementArray;
// 	for (int i = 0; i < ElementVector.Size(); i++)
// 	{
// 		ElementArray.Add(ToElem(*ElementVector[i]));
// 	}
// 	return ElementArray;
// }

TArray<FString> UTencentIMLibrary::ToTIMElemStringArray(const V2TIMElemVector& ElementVector)
{
	TArray<FString> OutMessages;
	for (int i = 0; i < ElementVector.Size(); i++)
	{
		const char* msgEle = ((V2TIMTextElem*)ElementVector[i])->text.CString();
		FString ele = UTF8_TO_TCHAR(msgEle);
		OutMessages.Add(ele);
	}

	return OutMessages;
}

V2TIMElemVector UTencentIMLibrary::ToElemVector(const TArray<FString>& ElementVector)
{
	V2TIMElemVector ElemVector;
	for (FString ele : ElementVector)
	{
		//todo
		// ElemVector.PushBack(&ToTIMElem(ele));
	}

	return V2TIMElemVector();
}


V2TIMElemType UTencentIMLibrary::ToTIMElemType(const ETIMElemType& MessageSearchParam)
{
	switch (MessageSearchParam)
	{
	case ETIMElemType::V2TIM_ELEM_TYPE_NONE:
		return V2TIMElemType::V2TIM_ELEM_TYPE_NONE;
	case ETIMElemType::V2TIM_ELEM_TYPE_TEXT:
		return V2TIMElemType::V2TIM_ELEM_TYPE_TEXT;
	case ETIMElemType::V2TIM_ELEM_TYPE_CUSTOM:
		return V2TIMElemType::V2TIM_ELEM_TYPE_CUSTOM;
	case ETIMElemType::V2TIM_ELEM_TYPE_IMAGE:
		return V2TIMElemType::V2TIM_ELEM_TYPE_IMAGE;
	case ETIMElemType::V2TIM_ELEM_TYPE_SOUND:
		return V2TIMElemType::V2TIM_ELEM_TYPE_SOUND;
	case ETIMElemType::V2TIM_ELEM_TYPE_VIDEO:
		return V2TIMElemType::V2TIM_ELEM_TYPE_VIDEO;
	case ETIMElemType::V2TIM_ELEM_TYPE_FILE:
		return V2TIMElemType::V2TIM_ELEM_TYPE_FILE;
	case ETIMElemType::V2TIM_ELEM_TYPE_LOCATION:
		return V2TIMElemType::V2TIM_ELEM_TYPE_LOCATION;
	case ETIMElemType::V2TIM_ELEM_TYPE_FACE:
		return V2TIMElemType::V2TIM_ELEM_TYPE_FACE;
	case ETIMElemType::V2TIM_ELEM_TYPE_GROUP_TIPS:
		return V2TIMElemType::V2TIM_ELEM_TYPE_GROUP_TIPS;
	case ETIMElemType::V2TIM_ELEM_TYPE_MERGER:
		return V2TIMElemType::V2TIM_ELEM_TYPE_MERGER;
	default:
		return V2TIMElemType::V2TIM_ELEM_TYPE_NONE;
	}
}

ETIMElemType UTencentIMLibrary::ToElemType(const V2TIMElemType& MessageSearchParam)
{
	switch (MessageSearchParam)
	{
	case V2TIM_ELEM_TYPE_NONE:
		return ETIMElemType::V2TIM_ELEM_TYPE_NONE;
	case V2TIM_ELEM_TYPE_TEXT:
		return ETIMElemType::V2TIM_ELEM_TYPE_TEXT;
	case V2TIM_ELEM_TYPE_CUSTOM:
		return ETIMElemType::V2TIM_ELEM_TYPE_CUSTOM;
	case V2TIM_ELEM_TYPE_IMAGE:
		return ETIMElemType::V2TIM_ELEM_TYPE_IMAGE;
	case V2TIM_ELEM_TYPE_SOUND:
		return ETIMElemType::V2TIM_ELEM_TYPE_SOUND;
	case V2TIM_ELEM_TYPE_VIDEO:
		return ETIMElemType::V2TIM_ELEM_TYPE_VIDEO;
	case V2TIM_ELEM_TYPE_FILE:
		return ETIMElemType::V2TIM_ELEM_TYPE_FILE;
	case V2TIM_ELEM_TYPE_LOCATION:
		return ETIMElemType::V2TIM_ELEM_TYPE_LOCATION;
	case V2TIM_ELEM_TYPE_FACE:
		return ETIMElemType::V2TIM_ELEM_TYPE_FACE;
	case V2TIM_ELEM_TYPE_GROUP_TIPS:
		return ETIMElemType::V2TIM_ELEM_TYPE_GROUP_TIPS;
	case V2TIM_ELEM_TYPE_MERGER:
		return ETIMElemType::V2TIM_ELEM_TYPE_MERGER;
	default:
		return ETIMElemType::V2TIM_ELEM_TYPE_NONE;
	}
}

V2TIMElemTypeVector UTencentIMLibrary::ToTIMElemTypeVector(const TArray<ETIMElemType>& MessageSearchParam)
{
	V2TIMElemTypeVector OutVector;
	for (ETIMElemType SearchParam : MessageSearchParam)
	{
		OutVector.PushBack(ToTIMElemType(SearchParam));
	}
	return OutVector;
}

TArray<ETIMElemType> UTencentIMLibrary::ToElemTypeArray(const V2TIMElemTypeVector& MessageSearchParam)
{
	TArray<ETIMElemType> ElementArray;
	for (int32 i = 0; i < MessageSearchParam.Size(); i++)
	{
		ElementArray.Add(ToElemType(MessageSearchParam[i]));
	}
	return ElementArray;
}

FTIMMessageSearchResult UTencentIMLibrary::ToMessageSearchResult(const V2TIMMessageSearchResult& MessageSearchResult)
{
	FTIMMessageSearchResult Result = FTIMMessageSearchResult();
	Result.totalCount = MessageSearchResult.totalCount;
	Result.messageSearchResultItems = ToMessageResultItem(MessageSearchResult.messageSearchResultItems);
	return Result;
}

TArray<FTIMMessageSearchResultItem> UTencentIMLibrary::ToMessageResultItem(const V2TIMMessageSearchResultItemVector& MessageResult)
{
	TArray<FTIMMessageSearchResultItem> ElementArray;
	for (int32 i = 0; i < MessageResult.Size(); i++)
	{
		ElementArray.Add(ToMessageSearchResultItem(MessageResult[i]));
	}
	return ElementArray;
}

FTIMMessageSearchResultItem UTencentIMLibrary::ToMessageSearchResultItem(const V2TIMMessageSearchResultItem& TIMMessageSearchResultItem)
{
	FTIMMessageSearchResultItem Result;
	Result.messageCount = TIMMessageSearchResultItem.messageCount;
	Result.messageList = ToMessageArray(TIMMessageSearchResultItem.messageList);
	Result.conversationID = ToFString(TIMMessageSearchResultItem.conversationID);
	return Result;
}

ETIMGroupAddOpt UTencentIMLibrary::ToGroupAddOpt(const V2TIMGroupAddOpt& GroupAddOpt)
{
	switch (GroupAddOpt)
	{
	case V2TIM_GROUP_ADD_FORBID:
		return ETIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID;
	case V2TIM_GROUP_ADD_AUTH:
		return ETIMGroupAddOpt::V2TIM_GROUP_ADD_AUTH;
	case V2TIM_GROUP_ADD_ANY:
		return ETIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID;
	default:
		return ETIMGroupAddOpt::V2TIM_GROUP_ADD_AUTH;
	}
}

V2TIMGroupAddOpt UTencentIMLibrary::ToTIMGroupAddOpt(const ETIMGroupAddOpt& GroupAddOpt)
{
	switch (GroupAddOpt)
	{
	case ETIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID:
		return V2TIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID;
	case ETIMGroupAddOpt::V2TIM_GROUP_ADD_AUTH:
		return V2TIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID;
	case ETIMGroupAddOpt::V2TIM_GROUP_ADD_ANY:
		return V2TIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID;
	default:
		return V2TIMGroupAddOpt::V2TIM_GROUP_ADD_FORBID;
	}
}
