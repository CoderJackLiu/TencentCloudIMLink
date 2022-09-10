﻿// Fill out your copyright notice in the Description page of Project Settings.


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


//todo create group

DECLARATION_CALLBACK_DELEGATE(CreateGroup)
DECLARATION_FAILURE_CALLBACK_DELEGATE(CreateGroup)

void UTencentIMLibrary::CreateGroup(const FString& groupType, const FString& groupID, const FString& groupName, FIMCallbackDelegate OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	CreateGroup_Delegate = OnSuccessDelegate;
	CreateGroup_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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
	Tencent_IM.CreateGroup(groupType, groupID, groupName, CallBack);
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

			GetUsersInfo_UserFullInfoDelegate.ExecuteIfBound(ToTIMUserFullInfoArray(message));
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
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateTextAtMessage(ToIMString(text), ToIMStringArray(atUserList)));
}

FTIMMessage UTencentIMLibrary::CreateCustomMessage(const V2TIMBuffer& data)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateCustomMessage(data));
}

FTIMMessage UTencentIMLibrary::CreateCustomMessage(const V2TIMBuffer& data, const FString& description, const FString& extension)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateCustomMessage(data, ToIMString(description), ToIMString(extension)));
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

FTIMMessage UTencentIMLibrary::CreateFaceMessage(int32 index, const V2TIMBuffer& data)
{
	//todo 
	return FTIMMessage();
}

FTIMMessage UTencentIMLibrary::CreateMergerMessage(const TArray<FTIMMessage>& messageList, const FString& title, const TArray<FString>& abstractList, const FString& compatibleText)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateMergerMessage(ToV2IMMessageArray(messageList), ToIMString(title), ToIMStringArray(abstractList),
	                                                                                    ToIMString(compatibleText)));
}

FTIMMessage UTencentIMLibrary::CreateForwardMessage(const FTIMMessage& message)
{
	return ToMessage(Tencent_IM.GetInstance()->GetMessageManager()->CreateForwardMessage(ToIMMessage(message)));
}

FString UTencentIMLibrary::SendMessage(FTIMMessage& message, const FString& receiver, const FString& groupID, V2TIMMessagePriority priority, bool onlineUserOnly,
	const V2TIMOfflinePushInfo& offlinePushInfo, V2TIMSendCallback* callback)
{
	//todo
	return "";
}


//------------------------------------------------------
//base function
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
	
	//todo DingLuckyGirl

	
	
	return OutTimMessage;
}

FTIMMessage UTencentIMLibrary::ToMessage(const V2TIMMessage& TimMessage)
{
	//todo DingLuckyGirl
	FTIMMessage OutTIMMessage;
	OutTIMMessage.msgID=ToFString(TimMessage.msgID);
	
	return OutTIMMessage;
}

TArray<FTIMMessage> UTencentIMLibrary::ToIMMessageArray(const V2TIMMessageVector& MessageVector)
{
	//todo
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
