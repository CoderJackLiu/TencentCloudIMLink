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
	TArray<FString> groupAtUserList;
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
	/// 配置群自定义字段的 key 值，Key 为 FString 类型，长度不超过 16 字节。 2.调用 SetGroupInfo
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


UENUM()
enum class ETIMConversationType:uint8
{
	///< 单聊
	V2TIM_C2C = 1,
	///< 群聊
	V2TIM_GROUP = 2,
};

/// @ 类型
UENUM()
enum class ETIMGroupAtType:uint8
{
	///< @ 我
	V2TIM_AT_ME = 1,
	///< @ 群里所有人
	V2TIM_AT_ALL = 2,
	///< @ 群里所有人并且单独 @ 我
	V2TIM_AT_ALL_AT_ME = 3,
};


USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMGroupAtInfo
{
	GENERATED_BODY()
	/// 消息序列号，即带有 “@我” 或者 “@所有人” 标记的消息的序列号
	/// uint64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=V2TIMConversationResult)
	FString seq;
	/// @ 提醒类型，分成 “@我” 、“@所有人” 以及 “@我并@所有人” 三类
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=V2TIMConversationResult)
	ETIMGroupAtType atType;

	// V2TIMGroupAtInfo();
	// V2TIMGroupAtInfo(const V2TIMGroupAtInfo& groupAtInfo);
	// V2TIMGroupAtInfo& operator=(const V2TIMGroupAtInfo& conversation);
	// ~V2TIMGroupAtInfo();
};


/// 会话对象
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMConversation
{
	GENERATED_BODY()
	/// 会话类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversation)
	ETIMConversationType type;
	/// 会话唯一 ID，如果是 C2C 单聊，组成方式为 c2c_userID，如果是群聊，组成方式为 group_groupID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString conversationID;
	/// 如果会话类型为 C2C 单聊，userID 会存储对方的用户ID，否则为空字符串
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString userID;
	/// 如果会话类型为群聊，groupID 会存储当前群的群 ID，否则为空字符串
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString groupID;
	/// 如果会话类型为群聊，groupType 为当前群类型，否则为空字符串
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString groupType;
	/// 会话展示名称（群组：群名称 >> 群 ID；C2C：对方好友备注 >> 对方昵称 >> 对方的 userID）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString showName;
	/// 会话展示头像（群组：群头像；C2C：对方头像）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString faceUrl;
	/// 会话未读消息数量,直播群（AVChatRoom）不支持未读计数，默认为 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	int32 unreadCount;
	/// 消息接收选项（接收 | 接收但不提醒 | 不接收）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	ETIMReceiveMessageOpt recvOpt;
	/// 会话最后一条消息，如果会话没有消息，lastMessage 字段为 NULL
	/// 5.5.892 以前版本，请您使用 lastMessage -> timestamp 对会话做排序，timestamp 越大，会话越靠前
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FTIMMessage lastMessage;
	/// 群会话 @ 信息列表，用于展示 “有人@我” 或 “@所有人” 这两种提醒状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	TArray<FTIMGroupAtInfo> groupAtInfolist;

	/// 草稿信息，设置草稿信息请调用 SetConversationDraft() 接口
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString draftText;

	/// 草稿编辑时间，草稿设置的时候自动生成   uint64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString draftTimestamp;

	/// 是否置顶
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	bool isPinned;

	// 排序字段（5.5.892 及以后版本支持）
	// @note
	// - 排序字段 orderKey
	// 是按照会话的激活时间线性递增的一个数字（注意：不是时间戳，因为同一时刻可能会有多个会话被同时激活）
	// - 5.5.892 及其以后版本，推荐您使用该字段对所有会话进行排序，orderKey
	// 值越大，代表该会话排序越靠前
	// - 当您 “清空会话所有消息” 或者 “逐个删除会话的所有消息” 之后，会话的 lastMessage
	// 变为空，但会话的 orderKey
	// 不会改变；如果想保持会话的排序位置不变，可以使用该字段对所有会话进行排序
	//uint64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString orderKey;

	// V2TIMConversation();
	// V2TIMConversation(const V2TIMConversation& conversation);
	// V2TIMConversation& operator=(const V2TIMConversation& conversation);
	// ~V2TIMConversation();
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMConversationResult
{
	GENERATED_BODY()
	/// 获取下一次分页拉取的游标   uint64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	FString nextSeq;
	/// 会话列表是否已经拉取完毕
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	bool isFinished;
	/// 获取会话列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TIMConversationResult)
	TArray<FTIMConversation> conversationList;

	// V2TIMConversationResult();
	// V2TIMConversationResult(const V2TIMConversationResult& conversationResult);
	// ~V2TIMConversationResult();
	// V2TIMConversationResult& operator=(const V2TIMConversationResult& conversationResult);
};


/// 用户基本资料
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMUserInfo
{
	GENERATED_BODY()
	/// 用户 ID
	FString userID;
	/// 用户昵称
	FString nickName;
	/// 用户头像
	FString faceURL;
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FBuffer
{
	GENERATED_BODY()
	TArray<uint8> Buffer;
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendInfo
{
	GENERATED_BODY()
	/// 好友 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString userID;
	/// 好友备注
	/// 备注长度最长不得超过 96 个字节;
	/// 字段描述详见
	/// [控制台](https://cloud.tencent.com/document/product/269/1501#.E6.A0.87.E9.85.8D.E5.A5.BD.E5.8F.8B.E5.AD.97.E6.AE.B5)。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString friendRemark;
	/// 好友自定义字段
	/// 首先要在 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 好友自定义字段)
	/// 配置好友自定义字段，然后再调用该接口进行设置，key 值不需要加 Tag_SNS_Custom_ 前缀。
	/// Map<FString, V2TIMBuffer>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	TMap<FString, FBuffer> friendCustomInfo;
	/// 好友所在分组列表
	/// - 最多支持 32 个分组；
	/// - 不允许分组名为空；
	/// - 分组名长度不得超过 30 个字节；
	/// - 同一个好友可以有多个不同的分组。
	/// - 字段描述详见
	/// [控制台](https://cloud.tencent.com/document/product/269/1501#.E6.A0.87.E9.85.8D.E5.A5.BD.E5.8F.8B.E5.AD.97.E6.AE.B5)。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	TArray<FString> friendGroups;
	/// 好友个人资料
	/// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FTIMUserFullInfo userFullInfo;
	// 用户资料修改标记位
	// 枚举 V2TIMFriendInfoModifyFlag 列出哪些字段支持修改，如果您修改好友资料，请设置这个字段值
	// 支持同时修改多个字段，多个枚举值按位或 | 组合，例如，同时修改好友备注和好友自定义字段
	// info.friendRemark = "new friend remark";
	// info.friendCustomInfo = friendCustomInfo;
	// info.modifyFlag = V2TIM_FRIEND_INFO_MODIFY_FLAG_REMARK | V2TIM_FRIEND_INFO_MODIFY_FLAG_CUSTOM;
	//uint32_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString modifyFlag;
};


/// 群成员基本资料

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMGroupMemberInfo
{
	GENERATED_BODY()
	/// 用户 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString userID;
	/// 用户昵称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString nickName;
	/// 用户好友备注
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString friendRemark;
	/// 群成员名片
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString nameCard;
	/// 用户头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	FString faceURL;
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMGroupMemberFullInfo : public FTIMGroupMemberInfo
{
	GENERATED_BODY()
	/// 群成员自定义字段
	/// 首先要在 [控制台](https://console.cloud.tencent.com/im) (功能配置 -> 群成员自定义字段)
	/// 配置用户自定义字段，然后再调用该接口进行设置。

	//todo finish
	// V2TIMCustomInfo customInfo;
	TMap<FString, V2TIMBuffer> customInfo;
	/// 群成员角色,修改群成员角色请调用 V2TIMManagerGroup.h -> SetGroupMemberRole 接口
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	int64 role;
	/// 群成员禁言结束时间戳，禁言用户请调用 V2TIMManagerGroup.h -> MuteGroupMember 接口
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	int64 muteUntil;
	/// 群成员入群时间，自动生成，不可修改
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	int64 joinTime;
	// 群成员资料修改标记位
	// 枚举 V2TIMGroupMemberInfoModifyFlag 列出哪些字段支持修改，如果您修改群成员资料，请设置这个字段值
	// 支持同时修改多个字段，多个枚举值按位或 | 组合，例如，同时修改群成员名片和群成员角色
	// info.nameCard = "new name card";
	// info.role = V2TIM_GROUP_MEMBER_ROLE_ADMIN;
	// info.modifyFlag = V2TIM_GROUP_MEMBER_INFO_MODIFY_FLAG_NAME_CARD | V2TIM_GROUP_MEMBER_INFO_MODIFY_FLAG_MEMBER_ROLE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FV2TIMFriendInfo)
	int64 modifyFlag;
};


/// 好友关系类型
UENUM(BlueprintType)
enum class ETIMFriendRelationType:uint8
{
	/// 不是好友
	V2TIM_FRIEND_RELATION_TYPE_NONE = 0x0,
	/// 对方在我的好友列表中
	V2TIM_FRIEND_RELATION_TYPE_IN_MY_FRIEND_LIST = 0x1,
	/// 我在对方的好友列表中
	V2TIM_FRIEND_RELATION_TYPE_IN_OTHER_FRIEND_LIST = 0x2,
	/// 互为好友
	V2TIM_FRIEND_RELATION_TYPE_BOTH_WAY = 0x3,
};

/// 好友资料获取结果
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendInfoResult
{
	GENERATED_BODY()
	/// 返回码
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendInfoResult)
	int32 resultCode;
	/// 返结果表述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendInfoResult)
	FString resultInfo;
	/// 好友类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendInfoResult)
	ETIMFriendRelationType relation;
	/// 好友个人资料，如果不是好友，除了 userID 字段，其他字段都为空
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendInfoResult)
	FTIMFriendInfo friendInfo;
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendSearchParam
{
	GENERATED_BODY()
	/// 搜索的关键字列表，关键字列表最多支持 5 个
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendSearchParam)
	TArray<FString> keywordList;
	/// 设置是否搜索 userID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendSearchParam)
	bool isSearchUserID;
	/// 是否设置搜索昵称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendSearchParam)
	bool isSearchNickName;
	/// 是否设置搜索备注
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendSearchParam)
	bool isSearchRemark;
};

/// 好友类型
UENUM(BlueprintType)
enum class ETIMFriendType:uint8
{
	None=0,
	/// 单向好友
	V2TIM_FRIEND_TYPE_SINGLE = 1,
	/// 双向好友
	V2TIM_FRIEND_TYPE_BOTH = 2,
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendAddApplication
{
	GENERATED_BODY()
	/// 用户 userID（必填）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendAddApplication)
	FString userID;
	/// 备注（备注最大96字节）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendAddApplication)
	FString friendRemark;
	/// 预分组名（最大96字节）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendAddApplication)
	FString friendGroup;
	/// 请求说明（最大120字节）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendAddApplication)
	FString addWording;
	/// 添加来源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendAddApplication)
	FString addSource;
	/// 加好友方式
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendAddApplication)
	ETIMFriendType addType;
};

/// 好友操作结果（添加、删除、加黑名单、添加分组等）
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendOperationResult
{
	GENERATED_BODY()
	/// 用户Id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString userID;
	/// 返回码
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	int32 resultCode;
	/// 返回信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString resultInfo;
};

/// 好友关系链检查结果
USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendCheckResult
{
	GENERATED_BODY()
	/// 用户id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString userID;
	/// 返回码
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	int32 resultCode;
	/// 返回信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString resultInfo;
	/// 检查结果
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	ETIMFriendRelationType relationType;
};

/// 好友申请类型
UENUM(BlueprintType)
enum class ETIMFriendApplicationType:uint8
{
	None=0,
	/// 别人发给我的
	V2TIM_FRIEND_APPLICATION_COME_IN = 1,
	/// 我发给别人的
	V2TIM_FRIEND_APPLICATION_SEND_OUT = 2,
	/// 别人发给我的 和 我发给别人的。仅拉取时有效
	V2TIM_FRIEND_APPLICATION_BOTH = 3,
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendApplication
{
	GENERATED_BODY()
	/// 用户标识
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString userID;
	/// 用户昵称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString nickName;
	/// 用户头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString faceUrl;
	/// 添加时间   uint64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString addTime;
	/// 来源
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString addSource;
	/// 加好友附言
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString addWording;
	/// 好友申请类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	ETIMFriendApplicationType type;
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendApplicationResult
{
	GENERATED_BODY()
	/// 好友申请未读数量   uint64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	FString unreadCount;
	/// 好友申请列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendOperationResult)
	TArray<FTIMFriendApplication> applicationList;
};

/// 好友申请接受类型
UENUM(BlueprintType)
enum class ETIMFriendAcceptType:uint8
{
	/// 接受加好友（建立单向好友）
	V2TIM_FRIEND_ACCEPT_AGREE = 0,
	/// 接受加好友并加对方为好友（建立双向好友）
	V2TIM_FRIEND_ACCEPT_AGREE_AND_ADD = 1,
};

USTRUCT(Blueprintable, BlueprintType)
struct TENCENTIMLINK_API FTIMFriendGroup
{
	GENERATED_BODY()
	/// 好友分组名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendGroup)
	FString groupName;
	/// 分组成员数量 uinn64_t
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendGroup)
	FString userCount;
	/// 分组成员列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FTIMFriendGroup)
	TArray<FString> friendList;
};

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UTencentDataType : public UObject
{
	GENERATED_BODY()
};
