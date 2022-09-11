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

	Tencent_IM.GetInstance()->GetMessageManager()->SetC2CReceiveMessageOpt(ToIMStringArray(userIDList), ToTIMReceiveMessageOpt(opt), Normal_callback_);
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
	Tencent_IM.GetInstance()->GetMessageManager()->GetC2CReceiveMessageOpt(ToIMStringArray(userIDList), CallBack);
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

	//todo 做法探究；
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
		virtual void OnSuccess(const V2TIMMessage& message) override
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
	Tencent_IM.GetInstance()->GetMessageManager()->FindMessages(ToIMStringArray(messageIDList), CallBack);
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
void UTencentIMLibrary::GetJoinedGroupList(FIMGroupInfoArrayCallback OnSuccessDelegate,FIMFailureCallback OnFailureDelegate)
{
	GetJoinedGroupList_GroupInfoArrayDelegate = OnSuccessDelegate;
	GetJoinedGroupList_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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
	//todo finish

	return TArray<FTIMGroupInfo>();
}

V2TIMGroupInfoVector UTencentIMLibrary::ToTIMGroupInfoVector(const TArray<FTIMGroupInfo>& GroupInfo)
{
	//todo finish
	
	return V2TIMGroupInfoVector();
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
DECLARATION_GroupMemFullInfos_DELEGATE(GetGroupMembersInfo)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetGroupMembersInfo)
void UTencentIMLibrary::GetGroupMembersInfo(const FString& groupID,const TArray<FString>& memberList, FGroupMemFullInfosCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
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
	Tencent_IM.GetInstance()->GetGroupManager()->GetGroupMembersInfo(ToIMString(groupID),ToIMStringArray(memberList),CallBack);

}

V2TIMGroupMemberFullInfoVector UTencentIMLibrary::ToGroupMemberFullInfoVector(const TArray<FTIMGroupMemberFullInfo>& GPFullInfos)
{
	//todo finish
	return V2TIMGroupMemberFullInfoVector();
}

TArray<FTIMGroupMemberFullInfo> UTencentIMLibrary::ToTIMGroupMemberFullInfoArray(const V2TIMGroupMemberFullInfoVector& GPFullInfos)
{
	//todo finish
	return TArray<FTIMGroupMemberFullInfo>();
}


DECLARATION_ConversationRst_DELEGATE(GetConversationList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetConversationList)
void UTencentIMLibrary::GetConversationList(const FString& nextSeq, int32 count, FMTIMConversationResultCallback OnSuccessDelegate,FIMFailureCallback OnFailureDelegate)
{
	GetConversationList_CsRstDelegate = OnSuccessDelegate;
	GetConversationList_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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
	Tencent_IM.GetInstance()->GetConversationManager()->GetConversationList(FCString::Strtoui64(*nextSeq, NULL, 10),count,CallBack);

}

FTIMConversationResult UTencentIMLibrary::ToTIMConversationResult(const V2TIMConversationResult& ConversationResult)
{
	FTIMConversationResult Temp=FTIMConversationResult();
	Temp.conversationList=ToTIMConversationArray(ConversationResult.conversationList);
	Temp.isFinished=ConversationResult.isFinished;
	Temp.nextSeq=FString::Printf(TEXT("%llu"), ConversationResult.nextSeq);
	return Temp;
}

V2TIMVConversationVector UTencentIMLibrary::ToV2TIMVConversationVector(const TArray<FTIMConversation>& Array_Conversation)
{
	V2TIMVConversationVector ConversationVector=V2TIMVConversationVector();
	for (FTIMConversation Conversation : Array_Conversation)
	{
		ConversationVector.PushBack(ToTIMConversation(Conversation));
	}
	return ConversationVector;
}

TArray<FTIMConversation> UTencentIMLibrary::ToTIMConversationArray(const V2TIMVConversationVector& ConversationVector)
{
	TArray<FTIMConversation> Conversation;
	for (int i=0;i<ConversationVector.Size();i++)
	{
		Conversation.Add(ToConversation(ConversationVector[i]));
	}
	return Conversation;
}
FTIMConversation UTencentIMLibrary::ToConversation(V2TIMConversation Conversation)
{
	//todo
	return FTIMConversation();
}


V2TIMConversation UTencentIMLibrary::ToTIMConversation(FTIMConversation Conversation)
{
	//todo
	return V2TIMConversation();
}

DECLARATION_TIMConversation_DELEGATE(GetConversation)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetConversation)
void UTencentIMLibrary::GetConversation(const FString& conversationID, FTIMConversationCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetConversation_ConversationDelegate = OnSuccessDelegate;
	GetConversation_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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
	Tencent_IM.GetInstance()->GetConversationManager()->GetConversation(ToIMString(conversationID),CallBack);
}

DECLARATION_TIMConversationVector_DELEGATE(GetConversationListByIDList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetConversationListByIDList)
void UTencentIMLibrary::GetConversationListByIDList(const TArray<FString>& conversationIDList, FTIMConversationVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetConversationListByIDList_ConversationVectorDelegate = OnSuccessDelegate;
	GetConversationListByIDList_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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
	Tencent_IM.GetInstance()->GetConversationManager()->GetConversationList(ToIMStringArray(conversationIDList),CallBack);
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
	Tencent_IM.GetInstance()->GetConversationManager()->SetConversationDraft(ToIMString(conversationID), ToIMString(draftText),Callback);
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
	Tencent_IM.GetInstance()->GetConversationManager()->PinConversation(ToIMString(conversationID), isPinned,Callback);
}

DECLARATION_TIMuint64_DELEGATE(GetTotalUnreadMessageCount)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetTotalUnreadMessageCount)
void UTencentIMLibrary::GetTotalUnreadMessageCount(FTIMuint64Callback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetTotalUnreadMessageCount_uint64Delegate = OnSuccessDelegate;
	GetTotalUnreadMessageCount_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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

DECLARATION_TIMFriendInfoVector_DELEGATE(GetFriendList)
DECLARATION_FAILURE_CALLBACK_DELEGATE(GetFriendList)
void UTencentIMLibrary::GetFriendList(FTIMFriendInfoVectorCallback OnSuccessDelegate, FIMFailureCallback OnFailureDelegate)
{
	GetFriendList_TIMFriendInfoVectorDelegate = OnSuccessDelegate;
	GetFriendList_FailureDelegate = OnFailureDelegate;
	//todo 做法探究；
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

TArray<FTIMFriendInfo> UTencentIMLibrary::ToFriendInfoArray(const V2TIMFriendInfoVector& Info)
{
	TArray<FTIMFriendInfo> Conversation;
	for (int i=0;i<Info.Size();i++)
	{
		Conversation.Add(ToFriendInfo(Info[i]));
	}
	return Conversation;
}

FTIMFriendInfo UTencentIMLibrary::ToFriendInfo(const V2TIMFriendInfo& Info)
{
	FTIMFriendInfo info= FTIMFriendInfo();
	info.userID=ToFString(Info.userID);
	info.friendRemark=ToFString(Info.friendRemark);
	info.friendCustomInfo=ToTIMCustomInfo_(Info.friendCustomInfo);
	info.friendGroups= ToIArrayString(Info.friendGroups);
	info.userFullInfo=ToTIMUserFullInfo(Info.userFullInfo);
	info.modifyFlag=FString::Printf(TEXT("%lu"), Info.modifyFlag);
	return info;
}

TArray<FTIMCreateGroupMemberInfo> UTencentIMLibrary::ToGroupMemberInfoArray(const V2TIMCreateGroupMemberInfoVector& MemberInfoVector)
{
	//todo finish
	return TArray<FTIMCreateGroupMemberInfo>();
}

V2TIMCreateGroupMemberInfoVector UTencentIMLibrary::ToCreateGroupMemberInfoVector(const TArray<FTIMCreateGroupMemberInfo>& MemberInfoVector)
{

	//todo finish
	return V2TIMCreateGroupMemberInfoVector();
}


FTIMGroupInfo UTencentIMLibrary::ToGroupInfo(const V2TIMGroupInfo& GroupInfo)
{
	
	return FTIMGroupInfo();
}

V2TIMGroupInfo UTencentIMLibrary::ToTIMGroupInfo(const FTIMGroupInfo& GroupInfo)
{
	
	return V2TIMGroupInfo();
}

//------------------------------------------------------
//base convert function



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

TArray<FString> UTencentIMLibrary::ToIArrayString(V2TIMStringVector TIMStringVector)
{
	TArray<FString> res;
	for(int i=0;i<TIMStringVector.Size();i++)
	{
		res.Add(ToFString(TIMStringVector[i]));
	}

	return res;
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
	FMemory::Memcpy(Buffer.Buffer.GetData(),TIMBuffer.Data(),TIMBuffer.Size());
	return Buffer;
}

V2TIMBuffer UTencentIMLibrary::ToTIMBuffer(FBuffer TIMBuffer)
{
	V2TIMBuffer Buffer(TIMBuffer.Buffer.GetData(),TIMBuffer.Buffer.Num());
	return Buffer;
}

TMap<FString, FBuffer> UTencentIMLibrary::ToTIMCustomInfo_(V2TIMCustomInfo CustomInfo)
{
	TMap<FString, FBuffer> OutCustomInfo;
	for (int32 i = 0; i < CustomInfo.AllKeys().Size(); i++)
	{
		V2TIMString Key = CustomInfo.AllKeys()[i];
		OutCustomInfo.Add(ToFString(Key),ToBuffer(CustomInfo.Get(Key)));
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

	//todo DingLuckyGirl


	return OutTimMessage;
}

FTIMMessage UTencentIMLibrary::ToMessage(const V2TIMMessage& TimMessage)
{
	//todo DingLuckyGirl
	FTIMMessage OutTIMMessage;
	OutTIMMessage.msgID = ToFString(TimMessage.msgID);

	return OutTIMMessage;
}

TArray<FTIMMessage> UTencentIMLibrary::ToMessageArray(const V2TIMMessageVector& MessageVector)
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
	OutParam.keywordList = ToIMStringArray(MessageSearchParam.keywordList);
	OutParam.keywordListMatchType = ToTIMKeywordListMatchType(MessageSearchParam.keywordListMatchType);
	OutParam.senderUserIDList = ToIMStringArray(MessageSearchParam.senderUserIDList);
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
