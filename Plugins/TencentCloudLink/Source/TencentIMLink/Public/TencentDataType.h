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

	// UPROPERTY(BlueprintReadWrite, Category=TIMUserFullInfo)
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

/// 消息接收选项
UENUM(Blueprintable, BlueprintType)
enum class ETIMReceiveMessageOpt:uint8
{
	///< 在线正常接收消息，离线时会进行 APNs 推送
	V2TIM_RECEIVE_MESSAGE = 0,
	///< 不会接收到消息，离线不会有推送通知
	V2TIM_NOT_RECEIVE_MESSAGE = 1,
	///< 在线正常接收消息，离线不会有推送通知
	V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE = 2,
};


USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMReceiveMessageOptInfo
{
	GENERATED_BODY()

	/// 用户 ID
	UPROPERTY(EditAnywhere, Category=TIMReceiveMessageOptInfo)
	FString userID;
	/// 消息接收选项
	UPROPERTY(EditAnywhere, Category=TIMReceiveMessageOptInfo)
	ETIMReceiveMessageOpt receiveOpt;

	// TIMReceiveMessageOptInfo();
	// TIMReceiveMessageOptInfo(const TIMReceiveMessageOptInfo &);
	// ~TIMReceiveMessageOptInfo();
};

// 消息拉取方式
UENUM(Blueprintable, BlueprintType)
enum class ETIMMessageGetType:uint8
{
	///< 获取云端更老的消息
	V2TIM_GET_CLOUD_OLDER_MSG = 0,
	///< 获取云端更新的消息
	V2TIM_GET_CLOUD_NEWER_MSG = 1,
	///< 获取本地更老的消息
	V2TIM_GET_LOCAL_OLDER_MSG = 2,
	///< 获取本地更新的消息
	V2TIM_GET_LOCAL_NEWER_MSG = 3,
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMMessageListGetOption
{
	/**
	 * 拉取消息类型，可以设置拉取本地、云端更老或者更新的消息
	 *
	 * @note 请注意
	 * <p>当设置从云端拉取时，会将本地存储消息列表与云端存储消息列表合并后返回。如果无网络，则直接返回本地消息列表。
	 * <p>关于 getType、拉取消息的起始消息、拉取消息的时间范围 的使用说明：
	 * - getType 可以用来表示拉取的方向：往消息时间更老的方向 或者 往消息时间更新的方向；
	 * - lastMsg/lastMsgSeq 用来表示拉取时的起点，第一次拉取时可以不填或者填 0；
	 * - getTimeBegin/getTimePeriod
	 * 用来表示拉取消息的时间范围，时间范围的起止时间点与拉取方向(getType)有关；
	 * -
	 * 当起始消息和时间范围都存在时，结果集可理解成：「单独按起始消息拉取的结果」与「单独按时间范围拉取的结果」
	 * 取交集；
	 * - 当起始消息和时间范围都不存在时，结果集可理解成：从当前会话最新的一条消息开始，按照 getType
	 * 所指定的方向和拉取方式拉取。
	 */
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	ETIMMessageGetType getType;

	/// 拉取单聊历史消息
	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	FString userID;

	/// 拉取群组历史消息
	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	FString groupID;

	/// 拉取消息数量
	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	int64 count;

	/**
	 * 拉取消息的起始消息
	 *
	 * @note 请注意，
	 * <p>拉取 C2C 消息，只能使用 lastMsg 作为消息的拉取起点；如果没有指定
	 * lastMsg，默认使用会话的最新消息作为拉取起点。 <p>拉取 Group 消息时，除了可以使用 lastMsg
	 * 作为消息的拉取起点外，也可以使用 lastMsgSeq 来指定消息的拉取起点，二者的区别在于：
	 * - 使用 lastMsg 作为消息的拉取起点时，返回的消息列表里不包含当前设置的 lastMsg；
	 * - 使用 lastMsgSeq 作为消息拉取起点时，返回的消息列表里包含当前设置的 lastMsgSeq
	 * 所表示的消息。
	 *
	 * @note 在拉取 Group 消息时，
	 * <p>如果同时指定了 lastMsg 和 lastMsgSeq，SDK 优先使用 lastMsg 作为消息的拉取起点。
	 * <p>如果 lastMsg 和 lastMsgSeq 都未指定，消息的拉取起点分为如下两种情况：
	 * -  如果设置了拉取的时间范围，SDK 会根据 @getTimeBegin 所描述的时间点作为拉取起点；
	 * -  如果未设置拉取的时间范围，SDK 默认使用会话的最新消息作为拉取起点。
	 */
	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	FTIMMessage lastMsg;

	//todo to fstring
	uint64_t lastMsgSeq;

	/**
	 * 拉取消息的时间范围
	 * @getTimeBegin  表示时间范围的起点；默认为 0，表示从现在开始拉取；UTC 时间戳，单位：秒
	 * @getTimePeriod 表示时间范围的长度；默认为 0，表示不限制时间范围；单位：秒
	 *
	 * @note
	 * <p> 时间范围的方向由参数 getType 决定
	 * <p> 如果 getType 取 V2TIM_GET_CLOUD_OLDER_MSG/V2TIM_GET_LOCAL_OLDER_MSG，表示从 getTimeBegin
	 * 开始，过去的一段时间，时间长度由 getTimePeriod 决定 <p> 如果 getType 取
	 * V2TIM_GET_CLOUD_NEWER_MSG/V2TIM_GET_LOCAL_NEWER_MSG，表示从 getTimeBegin
	 * 开始，未来的一段时间，时间长度由 getTimePeriod 决定 <p>
	 * 取值范围区间为闭区间，包含起止时间，二者关系如下：
	 * - 如果 getType 指定了朝消息时间更老的方向拉取，则时间范围表示为 [getTimeBegin-getTimePeriod,
	 * getTimeBegin]
	 * - 如果 getType 指定了朝消息时间更新的方向拉取，则时间范围表示为 [getTimeBegin,
	 * getTimeBegin+getTimePeriod]
	 */


	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	int64 getTimeBegin;

	UPROPERTY(EditAnywhere, Category=TIMMessageListGetOption)
	int64 getTimePeriod;

	// V2TIMMessageListGetOption();
	// V2TIMMessageListGetOption(const V2TIMMessageListGetOption&);
	// V2TIMMessageListGetOption& operator=(const V2TIMMessageListGetOption&);
	// ~V2TIMMessageListGetOption();
};


// /// 消息接收选项
// UENUM(Blueprintable, BlueprintType)
// enum class ETIMReceiveMessageOpt:uint8
// {
// 	///< 在线正常接收消息，离线时会进行 APNs 推送
// 	V2TIM_RECEIVE_MESSAGE = 0,
// 	///< 不会接收到消息，离线不会有推送通知
// 	V2TIM_NOT_RECEIVE_MESSAGE = 1,
// 	///< 在线正常接收消息，离线不会有推送通知
// 	V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE = 2,
// };

/// 消息搜索关键字匹配类型
UENUM(Blueprintable, BlueprintType)
enum class ETIMKeywordListMatchType:uint8
{
	V2TIM_KEYWORD_LIST_MATCH_TYPE_OR = 0,
	V2TIM_KEYWORD_LIST_MATCH_TYPE_AND = 1
};


/// 消息搜索参数
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMMessageSearchParam
{
	GENERATED_BODY()

	/**
	 * 关键字列表，最多支持5个。当消息发送者以及消息类型均未指定时，关键字列表必须非空；否则，关键字列表可以为空。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	TArray<FString> keywordList;

	/**
	 * 指定关键字列表匹配类型，可设置为“或”关系搜索或者“与”关系搜索.
	 * 取值分别为 V2TIM_KEYWORD_LIST_MATCH_TYPE_OR 和
	 * V2TIM_KEYWORD_LIST_MATCH_TYPE_AND，默认为“或”关系搜索。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	ETIMKeywordListMatchType keywordListMatchType;

	/**
	 * 指定 userID 发送的消息，最多支持5个。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	TArray<FString> senderUserIDList;

	/// 指定搜索的消息类型集合，传 nil 表示搜索支持的全部类型消息（V2TIMFaceElem 和
	/// V2TIMGroupTipsElem 不支持）取值详见 @V2TIMElemType。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	TArray<ETIMElemType> messageTypeList;

	/**
	 * 搜索“全部会话”还是搜索“指定的会话”：
	 * <p> 如果设置 conversationID == nil，代表搜索全部会话。
	 * <p> 如果设置 conversationID != nil，代表搜索指定会话。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	FString conversationID;

	/// 搜索的起始时间点。默认为0即代表从现在开始搜索。UTC 时间戳，单位：秒
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	int64 searchTimePosition;

	/// 从起始时间点开始的过去时间范围，单位秒。默认为0即代表不限制时间范围，传24x60x60代表过去一天。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	int64 searchTimePeriod;

	/**
	 * 分页的页号：用于分页展示查找结果，从零开始起步。
	 * 比如：您希望每页展示 10 条结果，请按照如下规则调用：
	 * - 首次调用：通过参数 pageSize = 10, pageIndex = 0 调用 searchLocalMessage，从结果回调中的
	 * totalCount 可以获知总共有多少条结果。
	 * - 计算页数：可以获知总页数：totalPage = (totalCount % pageSize == 0) ? (totalCount /
	 * pageSize) : (totalCount / pageSize + 1) 。
	 * - 再次调用：可以通过指定参数 pageIndex （pageIndex < totalPage）返回后续页号的结果。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	int64 pageIndex;

	/// 每页结果数量：用于分页展示查找结果，如不希望分页可将其设置成
	/// 0，但如果结果太多，可能会带来性能问题。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	int64 pageSize;
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMMessageSearchResultItem
{
	GENERATED_BODY()
	/// 会话ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	FString conversationID;
	/// 当前会话一共搜索到了多少条符合要求的消息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	int64 messageCount;

	/**
	 * 满足搜索条件的消息列表
	 * <p>如果您本次搜索【指定会话】，那么 messageList
	 * 中装载的是本会话中所有满足搜索条件的消息列表。 <p>如果您本次搜索【全部会话】，那么
	 * messageList 中装载的消息条数会有如下两种可能：
	 * - 如果某个会话中匹配到的消息条数 > 1，则 messageList 为空，您可以在 UI 上显示“ messageCount
	 * 条相关记录”。
	 * - 如果某个会话中匹配到的消息条数 = 1，则 messageList 为匹配到的那条消息，您可以在 UI
	 * 上显示之，并高亮匹配关键词。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	TArray<FTIMMessage> messageList;

	// V2TIMMessageSearchResultItem();
	// V2TIMMessageSearchResultItem(const V2TIMMessageSearchResultItem &);
	// ~V2TIMMessageSearchResultItem();
};


USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMMessageSearchResult
{
	GENERATED_BODY()

	/**
	 * 如果您本次搜索【指定会话】，那么返回满足搜索条件的消息总数量；
	 * 如果您本次搜索【全部会话】，那么返回满足搜索条件的消息所在的所有会话总数量。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	int64 totalCount;

	/**
	 * 如果您本次搜索【指定会话】，那么返回结果列表只包含该会话结果；
	 * 如果您本次搜索【全部会话】，那么对满足搜索条件的消息根据会话 ID 分组，分页返回分组结果；
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMMessageSearchParam)
	TArray<FTIMMessageSearchResultItem> messageSearchResultItems;

	// V2TIMMessageSearchResult();
	// V2TIMMessageSearchResult(const V2TIMMessageSearchResult &);
	// V2TIMMessageSearchResult &operator=(const V2TIMMessageSearchResult &);
	// ~V2TIMMessageSearchResult();
};

/// 加群选项
UENUM(Blueprintable, BlueprintType)
enum class ETIMGroupAddOpt:uint8
{
	/// 禁止加群
	V2TIM_GROUP_ADD_FORBID = 0,
	/// 需要管理员审批
	V2TIM_GROUP_ADD_AUTH = 1,
	/// 任何人可以加入
	V2TIM_GROUP_ADD_ANY = 2,
};

// UENUM(Blueprintable, BlueprintType)
// enum class EReceiveMessageOpt:uint8
// {
// 	///< 在线正常接收消息，离线时会进行 APNs 推送
// 	V2TIM_RECEIVE_MESSAGE = 0,
// 	///< 不会接收到消息，离线不会有推送通知
// 	V2TIM_NOT_RECEIVE_MESSAGE = 1,
// 	///< 在线正常接收消息，离线不会有推送通知
// 	V2TIM_RECEIVE_NOT_NOTIFY_MESSAGE = 2,
// };


USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMGroupInfo
{
	GENERATED_BODY()
	/// 群组 ID
	/// 自定义群组 ID 必须为可打印 ASCII 字符（0x20-0x7e），最长48个字节，且前缀不能为
	/// @TGS#（避免与默认分配的群组 ID 混淆）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMGroupInfo)
	FString groupID;
	/// 群类型
	FString groupType;
	/// 群名称
	/// 群名称最长30字节
	FString groupName;
	/// 群公告
	/// 群公告最长300字节
	FString notification;
	/// 群简介
	/// 群简介最长240字节
	FString introduction;
	/// 群头像
	/// 群头像 URL 最长100字节
	FString faceURL;
	/// 是否全员禁言
	bool allMuted;
	/// 设置群自定义字段需要两个步骤：
	/// 1.在 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 群自定义字段)
	/// 配置群自定义字段的 key 值，Key 为 V2TIMString 类型，长度不超过 16 字节。 2.调用 SetGroupInfo
	/// 接口设置该字段，value 为 V2TIMSBuffer 数据，长度不超过 512 字节。
	V2TIMCustomInfo customInfo;
	/// 群创建人/管理员
	FString owner;
	/// 群创建时间
	uint32_t createTime;
	/// 加群是否需要管理员审批，工作群（Work）不能主动加入，不支持此设置项
	ETIMGroupAddOpt groupAddOpt;
	/// 群最近一次群资料修改时间
	uint32_t lastInfoTime;
	/// 群最近一次发消息时间
	uint32_t lastMessageTime;
	/// 已加入的群成员数量
	uint32_t memberCount;
	/// 在线的群成员数量
	uint32_t onlineCount;
	/// 最多允许加入的群成员数量
	/// 各类群成员人数限制详见:
	/// https://cloud.tencent.com/document/product/269/1502#.E7.BE.A4.E7.BB.84.E9.99.90.E5.88.B6.E5.B7.AE.E5.BC.82
	uint32_t memberMaxCount;
	/// 当前用户在此群组中的角色，切换角色请调用 setGroupMemberRole 接口
	uint32_t role;
	/// 当前用户在此群组中的消息接收选项,修改群消息接收选项请调用 SetGroupReceiveMessageOpt 接口
	ETIMReceiveMessageOpt recvOpt;
	/// 当前用户在此群中的加入时间，不支持设置，系统自动生成
	uint32_t joinTime;
	// 群资料修改标记位
	// 枚举 V2TIMGroupInfoModifyFlag 列出哪些字段支持修改，如果您修改群资料，请设置这个字段值
	// 如果您同时修改多个字段，多个枚举值按位或 | 组合，例如，同时修改群名称和头像
	// info.groupName = "new group name";
	// info.faceURL = "new face url";
	// info.modifyFlag = V2TIM_GROUP_INFO_MODIFY_FLAG_GROUP_NAME |
	// V2TIM_GROUP_INFO_MODIFY_FLAG_FACE_URL;
	uint32_t modifyFlag;

	// V2TIMGroupInfo();
	// V2TIMGroupInfo(const V2TIMGroupInfo& groupInfo);
	// ~V2TIMGroupInfo();
};

/// 创建群时指定群成员

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMCreateGroupMemberInfo
{
	GENERATED_BODY()

	// 被操作成员
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMGroupInfo)
	FString userID;
	/// 群成员类型，需要注意一下事项：
	/// 1. role 不设置或则设置为 V2TIM_GROUP_MEMBER_UNDEFINED，进群后默认为群成员。
	/// 2. 工作群（Work）不支持设置 role 为管理员。
	/// 3. 所有的群都不支持设置 role 为群主。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMGroupInfo)
	int64 role;


};


/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UTencentDataType : public UObject
{
	GENERATED_BODY()
};
