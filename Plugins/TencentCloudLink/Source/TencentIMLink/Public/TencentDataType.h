// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "V2TIMFriendship.h"

#include "UObject/Object.h"
#include "TencentDataType.generated.h"


UENUM(BlueprintType)
enum class ELoginStatus :uint8
{
	/// 已登录
	None = 0,
	V2TIM_STATUS_LOGINED = 1,
	/// 登录中
	V2TIM_STATUS_LOGINING = 2,
	/// 无登录
	V2TIM_STATUS_LOGOUT = 3,
};

UENUM(BlueprintType)
enum class EIMMessagePriority :uint8
{
	/// 默认
	V2TIM_PRIORITY_DEFAULT = 0,
	/// 高优先级，一般用于礼物等重要消息
	V2TIM_PRIORITY_HIGH = 1,
	/// 常规优先级，一般用于普通消息
	V2TIM_PRIORITY_NORMAL = 2,
	/// 低优先级，一般用于点赞消息
	V2TIM_PRIORITY_LOW = 3,
};

UCLASS()
class UMyClass : public UObject
{
	GENERATED_BODY()
public:
};

//todo unknown custom Info for sun shi jia
// UCLASS(Blueprintable,BlueprintType)
// class TENCENTIMLINK_API UTIMBuffer: public UObject
// {
// public:		
// 	GENERATED_BODY()
//
// 	UTIMBuffer();
//
// 	UTIMBuffer(const UTIMBuffer& buffer);
//
// 	UTIMBuffer(const uint8* data, uint64 size);
//
// 	virtual ~UTIMBuffer() override;
//
// 	const uint8* Data() const;
//
// 	uint64 Size() const;
//
// 	UTIMBuffer& operator=(const UTIMBuffer& buffer);
//
// private:
// 	
// 	uint8* buffer_;
// 	uint64 length_;
// };

USTRUCT(Blueprintable)
struct TENCENTIMLINK_API FV2TIMSDKConfig
{
	GENERATED_BODY()

	struct V2TIMSDKConfig;
};

UENUM(BlueprintType)
enum class ETIMGender:uint8
{
	/// 未知性别
	V2TIM_GENDER_UNKNOWN = 0,
	/// 男性
	V2TIM_GENDER_MALE = 1,
	/// 女性
	V2TIM_GENDER_FEMALE = 2,
};

/// 好友验证方式
UENUM(BlueprintType)
enum class ETIMFriendAllowType:uint8
{
	/// 同意任何用户加好友
	V2TIM_FRIEND_ALLOW_ANY = 0,
	/// 需要验证
	V2TIM_FRIEND_NEED_CONFIRM = 1,
	/// 拒绝任何人加好友
	V2TIM_FRIEND_DENY_ANY = 2,
};


/// 好友申请类型
UENUM(BlueprintType)
enum class FV2TIMFriendApplicationType:uint8
{
	/// 别人发给我的
	FAT_NONE = 0,
	FV2TIM_FRIEND_APPLICATION_COME_IN = 1,
	/// 我发给别人的
	FV2TIM_FRIEND_APPLICATION_SEND_OUT = 2,
	/// 别人发给我的 和 我发给别人的。仅拉取时有效
	FV2TIM_FRIEND_APPLICATION_BOTH = 3,
};

/// 好友类型
UENUM(BlueprintType)
enum class FV2TIMFriendType:uint8
{
	/// 单向好友
	FT_NONE = 0,
	FV2TIM_FRIEND_TYPE_SINGLE = 1,
	/// 双向好友
	FV2TIM_FRIEND_TYPE_BOTH = 2,
};

/// 好友关系类型
UENUM(BlueprintType)
enum class FV2TIMFriendRelationType:uint8
{
	/// 不是好友
	FV2TIM_FRIEND_RELATION_TYPE_NONE = 0x0,
	/// 对方在我的好友列表中
	FV2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST = 0x1,
	/// 我在对方的好友列表中
	FV2TIM_FRIEND_RELATION_TYPE_IN_OTHER_FRIEND_LIST = 0x2,
	/// 互为好友
	FV2TIM_FRIEND_RELATION_TYPE_BOTH_WAY = 0x3,
};

/// 好友申请接受类型
UENUM(BlueprintType)
enum class FV2TIMFriendAcceptType:uint8
{
	/// 接受加好友（建立单向好友）
	FV2TIM_FRIEND_ACCEPT_AGREE = 0,
	/// 接受加好友并加对方为好友（建立双向好友）
	FV2TIM_FRIEND_ACCEPT_AGREE_AND_ADD = 1,
};


// 用户资料修改标记
UENUM(BlueprintType)
enum class FV2TIMUserInfoModifyFlag:uint8
{
	// 未定义
	FV2TIM_USER_INFO_MODIFY_FLAG_UNKNOWN = 0,
	// 昵称
	FV2TIM_USER_INFO_MODIFY_FLAG_NICK = 1,
	// 头像
	FV2TIM_USER_INFO_MODIFY_FLAG_FACE_URL = 2,
	// 性别
	FV2TIM_USER_INFO_MODIFY_FLAG_GENDER = 3,
	// 生日
	FV2TIM_USER_INFO_MODIFY_FLAG_BIRTHDAY = 4,
	// 修改签名
	FV2TIM_USER_INFO_MODIFY_FLAG_SELF_SIGNATURE = 7,
	// 等级
	FV2TIM_USER_INFO_MODIFY_FLAG_LEVEL = 8,
	// 角色
	FV2TIM_USER_INFO_MODIFY_FLAG_ROLE = 9,
	// 好友验证方式
	FV2TIM_USER_INFO_MODIFY_FLAG_ALLOW_TYPE = 10,
	// 自定义字段
	FV2TIM_USER_INFO_MODIFY_FLAG_CUSTOM = 11,
};

// 好友资料修改标记
UENUM(BlueprintType)
enum class ETIMFriendInfoModifyFlag:uint8
{
	// 未定义
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_UNKNOWN = 0,
	// 好友备注
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_REMARK = 1,
	// 好友自定义字段
	FV2TIM_FRIEND_INFO_MODIFY_FLAG_CUSTOM = 2,
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMUserFullInfo
{
	GENERATED_BODY()

	/// 用户 ID
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	FString userID;
	/// 用户昵称
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	FString nickName;
	/// 用户头像
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	FString faceURL;

	/// 用户签名
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	FString selfSignature;

	/// 用户性别
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	ETIMGender gender;
	/// 用户角色
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	int32 role;
	/// 用户等级
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	int32 level;
	/// 出生日期
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	int32 birthday;
	/// 用户好友验证方式
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	ETIMFriendAllowType allowType;
	/// 用户自定义字段
	/// 首先要在 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 用户自定义字段)
	/// 配置用户自定义字段，然后再调用该接口进行设置，key 值不需要加 Tag_Profile_Custom_ 前缀。

	// FV2TIMCustomInfo customInfo;

	//todo ????
	// UPROPERTY(BlueprintReadWrite,Category=TIMUserFullInfo)
	TMap<FString, V2TIMBuffer> customInfo;

	// 用户资料修改标记位
	// 枚举 V2TIMUserInfoModifyFlag 列出哪些字段支持修改，如果您修改用户资料，请设置这个字段值
	// 支持同时修改多个字段，多个枚举值按位或 | 组合，例如，同时修改用户昵称和用户签名
	// info.nickName = "new nickname";
	// info.selfSignature = "new self signature";
	// info.modifyFlag = V2TIM_USER_INFO_MODIFY_FLAG_NICK | V2TIM_USER_INFO_MODIFY_FLAG_SELF_SIGNATURE;
	UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
	int32 modifyFlag;
};

/// 消息状态
UENUM(BlueprintType)
enum class ETIMMessageStatus:uint8
{
	///< 消息发送中
	V2TIM_MSG_STATUS_SENDING = 0,
	///< 消息发送成功
	V2TIM_MSG_STATUS_SEND_SUCC = 1,
	///< 消息发送失败
	V2TIM_MSG_STATUS_SEND_FAIL = 2,
	///< 消息被删除
	V2TIM_MSG_STATUS_HAS_DELETED = 3,
	///< 导入到本地的消息
	V2TIM_MSG_STATUS_LOCAL_IMPORTED = 4,
	///< 被撤销的消息
	V2TIM_MSG_STATUS_LOCAL_REVOKED = 5,
};

/// 消息类型
UENUM(BlueprintType)
enum class ETIMElemType:uint8
{
	///< 未知消息
	V2TIM_ELEM_TYPE_NONE = 0,
	///< 文本消息
	V2TIM_ELEM_TYPE_TEXT = 1,
	///< 自定义消息
	V2TIM_ELEM_TYPE_CUSTOM = 2,
	///< 图片消息
	V2TIM_ELEM_TYPE_IMAGE = 3,
	///< 语音消息
	V2TIM_ELEM_TYPE_SOUND = 4,
	///< 视频消息
	V2TIM_ELEM_TYPE_VIDEO = 5,
	///< 文件消息
	V2TIM_ELEM_TYPE_FILE = 6,
	///< 地理位置消息
	V2TIM_ELEM_TYPE_LOCATION = 7,
	///< 表情消息
	V2TIM_ELEM_TYPE_FACE = 8,
	///< 群 Tips 消息
	V2TIM_ELEM_TYPE_GROUP_TIPS = 9,
	///< 合并消息
	V2TIM_ELEM_TYPE_MERGER = 10,
};

//高级消息
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMMessage
{
	GENERATED_BODY()

	/// 消息 ID（消息创建的时候为空，消息发送的时候会生成）
	FString msgID;
	/// 消息时间
	/// todo 
	int64 timestamp;
	/// 消息发送者
	FString sender;
	/// 消息发送者昵称
	FString nickName;
	/// 消息发送者好友备注
	FString friendRemark;
	/// 如果是群组消息，nameCard 为发送者的群名片
	FString nameCard;
	/// 消息发送者头像
	/// 在 C2C 场景下，陌生人的头像不会实时更新，如需更新陌生人的头像（如在 UI
	/// 上点击陌生人头像以展示陌生人信息时）， 请调用 V2TIMManager.h -> GetUsersInfo
	/// 接口触发信息的拉取。待拉取成功后，SDK 会更新本地头像信息，即 faceURL 字段的内容。
	/// @note 请不要在收到每条消息后都去 GetUsersInfo，会严重影响程序性能。
	FString faceURL;
	/// 如果是群组消息，groupID 为会话群组 ID，否则为空
	FString groupID;
	/// 如果是单聊消息，userID 为会话用户 ID，否则为空，
	/// 假设自己和 userA 聊天，无论是自己发给 userA 的消息还是 userA 发给自己的消息，这里的 userID
	/// 均为 userA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessage)
	FString userID;
	/// 群聊中的消息序列号云端生成，在群里是严格递增且唯一的,
	/// 单聊中的序列号是本地生成，不能保证严格递增且唯一。
	uint64 seq;
	/// 消息随机码
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessage)
	uint64 random;
	/// 消息发送状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessage)
	ETIMMessageStatus status;
	/// 消息发送者是否是自己
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessage)
	bool isSelf;
	/// 消息自己是否已读
	bool isRead;
	/// 消息对方是否已读（只有 C2C 消息有效）
	bool isPeerRead;
	/// 群消息中被 @ 的用户 UserID 列表（即该消息都 @ 了哪些人）
	V2TIMStringVector groupAtUserList;
	/*消息元素列表
	
	推荐一条消息只存放一个 elem，在收到这条消息时，调用 elemList[0] 获取这个elem，示例代码如下：
	if (1 == message.elemList.Size()) {
	    V2TIMElem *elem = message.elemList[0];
	    switch (elem->elemType) {
	        case V2TIM_ELEM_TYPE_TEXT:
	            V2TIMTextElem *textElem = static_cast<V2TIMTextElem *>(elem);
	            break;
	        case V2TIM_ELEM_TYPE_CUSTOM:
	            V2TIMCustomElem *customElem = static_cast<V2TIMCustomElem *>(elem);
	            break;
	        case V2TIM_ELEM_TYPE_FACE:
	            V2TIMFaceElem *faceElem = static_cast<V2TIMFaceElem *>(elem);
	            break;
	        case V2TIM_ELEM_TYPE_LOCATION:
	            V2TIMLocationElem *locationElem = static_cast<V2TIMLocationElem *>(elem);
	            break;
	        default:
	            break;
	    }
	}
	
	如果一条消息有多个 elem，遍历 elemList 列表，获取全部 elem 元素，示例代码如下：
	for (size_t i = 0; i < message.elemList.Size(); ++i) {
	    V2TIMElem *elem = message.elemList[i];
	    switch (elem->elemType) {
	        case V2TIM_ELEM_TYPE_TEXT:
	            V2TIMTextElem *textElem = static_cast<V2TIMTextElem *>(elem);
	            break;
	        case V2TIM_ELEM_TYPE_CUSTOM:
	            V2TIMCustomElem *customElem = static_cast<V2TIMCustomElem *>(elem);
	            break;
	        case V2TIM_ELEM_TYPE_FACE:
	            V2TIMFaceElem *faceElem = static_cast<V2TIMFaceElem *>(elem);
	            break;
	        case V2TIM_ELEM_TYPE_LOCATION:
	            V2TIMLocationElem *locationElem = static_cast<V2TIMLocationElem *>(elem);
	            break;
	        default:
	            break;
	    }
	}
	
	如果您的一条消息需要多个 elem，可以在创建 Message 对象后，调用 elemList.PushBack 添加新
	elem， 以 V2TIMTextElem 和 V2TIMCustomElem 多 elem 为例，示例代码如下： 
	V2TIMCustomElem *customElem = new V2TIMCustomElem(); 
	customElem->data = buffer; 
	V2TIMMessage message = messageManager.CreateTextMessage("text");
	message.elemList.PushBack(customElem);*/

	TArray<ETIMElemType> elemList;
	/// 消息自定义数据（本地保存，不会发送到对端，程序卸载重装后失效）
	V2TIMBuffer localCustomData;
	/// 消息自定义数据,可以用来标记语音、视频消息是否已经播放（本地保存，不会发送到对端，程序卸载重装后失效）
	int localCustomInt;
	/// 消息自定义数据（云端保存，会发送到对端，程序卸载重装后还能拉取到）
	V2TIMBuffer cloudCustomData;
	/// 消息是否不计入会话未读数：默认为 false，表明需要计入会话未读数，设置为
	/// true，表明不需要计入会话未读数
	bool isExcludedFromUnreadCount;
	/// 消息是否不计入会话 lastMsg：默认为 false，表明需要计入会话 lastMsg，设置为
	/// true，表明不需要计入会话 lastMsg
	bool isExcludedFromLastMessage;
	/// 指定群消息接收成员，即发送群内定向消息（5.9及以上版本支持）
	/// 请注意：
	/// - 发送群 @ 消息时，设置该字段无效
	/// - 向社群（Community）和直播群（AVChatRoom）里发送消息时，设置该字段无效
	/// - 群内定向消息默认不计入群会话的未读计数
	TArray<FString> targetGroupMemberList;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FProgressCallbackDelegate, int64, ConsumedBytes, int64, TotalBytes);

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UTencentDataType : public UObject
{
	GENERATED_BODY()
};
