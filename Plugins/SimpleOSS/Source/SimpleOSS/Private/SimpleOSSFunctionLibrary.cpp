// Copyright (C) RenZhai.2020.All Rights Reserved.


#include "SimpleOSSFunctionLibrary.h"
#include "SimpleOSSManage.h"
#include "Async/TaskGraphInterfaces.h"
#include "SimpleOSSMacro.h"
#include "Misc/ScopeLock.h"

FCriticalSection OSSMutex;

FString USimpleOSSFunctionLibrary::GetEndpoint()
{
	return SIMPLE_OSS.GetEndpoint();
}

void USimpleOSSFunctionLibrary::InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint)
{
	SIMPLE_OSS.InitAccounts(InAccessKeyId, InAccessKeySecret, InEndpoint);
}

void USimpleOSSFunctionLibrary::InitConf(const FClientConfiguration &InConf)
{
	SIMPLE_OSS.InitConf(InConf);
}

bool USimpleOSSFunctionLibrary::DoesBucketExist(const FString &InBucketName)
{
	return SIMPLE_OSS.DoesBucketExist(InBucketName);
}

bool USimpleOSSFunctionLibrary::CreateBucket(const FString &InBucketName, EOSSStorageType OSSStorageType /*= EOSSStorageType::IA*/, EAcl Acl /*= EAcl::PUBLICREAD*/)
{
	return SIMPLE_OSS.CreateBucket(InBucketName, OSSStorageType, Acl);
}

bool USimpleOSSFunctionLibrary::ListBuckets(TArray<FString> &InBucketName)
{
	return SIMPLE_OSS.ListBuckets(InBucketName);
}

bool USimpleOSSFunctionLibrary::GetBucketsInfo(const FString &InBucketName, FBucketInfo &InBucketInfo)
{
	return SIMPLE_OSS.GetBucketsInfo(InBucketName, InBucketInfo);
}

FString USimpleOSSFunctionLibrary::GetBucketLocation(const FString &InBucketName)
{
	return SIMPLE_OSS.GetBucketLocation(InBucketName);
}

//int64 USimpleOSSFunctionLibrary::GetBucketsStorageCapacity(const FString &InBucketName)
//{
//	return SIMPLE_OSS.GetBucketsStorageCapacity(InBucketName);
//}

bool USimpleOSSFunctionLibrary::DeleteBucket(const FString &InBucketName)
{
	return SIMPLE_OSS.DeleteBucket(InBucketName);
}

bool USimpleOSSFunctionLibrary::SetBucketAcl(const FString &InBucketName, EAcl BucketAcl)
{
	return SIMPLE_OSS.SetBucketAcl(InBucketName, BucketAcl);
}

EAcl USimpleOSSFunctionLibrary::GetBucketAcl(const FString InBucketName)
{
	return SIMPLE_OSS.GetBucketAcl(InBucketName);
}

bool USimpleOSSFunctionLibrary::SetBucketReferer(const FString &InBucketName, const TArray<FString> &URLReferer, bool bAllowEmptyRefere /*= false*/)
{
	return SIMPLE_OSS.SetBucketReferer(InBucketName, URLReferer, bAllowEmptyRefere);
}

bool USimpleOSSFunctionLibrary::GetBucketReferer(const FString &InBucketName, TArray<FString> &BucketReferers)
{
	return SIMPLE_OSS.GetBucketReferer(InBucketName, BucketReferers);
}

bool USimpleOSSFunctionLibrary::DeleteBucketReferer(const FString &InBucketName, bool bAllowEmptyRefere /*= true*/)
{
	return SIMPLE_OSS.DeleteBucketReferer(InBucketName, bAllowEmptyRefere);
}

bool USimpleOSSFunctionLibrary::GetObjectMeta(const FString &InBucketName, const FString &InObjectName, FOSSObjectMeta &OSSObjectInfo)
{
	return SIMPLE_OSS.GetObjectMeta(InBucketName, InObjectName, OSSObjectInfo);
}

EAcl USimpleOSSFunctionLibrary::GetObjectAcl(const FString &InBucketName, const FString &InObjectName)
{
	return SIMPLE_OSS.GetObjectAcl(InBucketName, InObjectName);
}

bool USimpleOSSFunctionLibrary::SetObjectAcl(const FString &InBucketName, const FString &InObjectName, EAcl InAcl)
{
	return SIMPLE_OSS.SetObjectAcl(InBucketName, InObjectName, InAcl);
}

bool USimpleOSSFunctionLibrary::DoesObjectExist(const FString &InBucketName, const FString &InObjectName)
{
	return SIMPLE_OSS.DoesObjectExist(InBucketName, InObjectName);
}

bool USimpleOSSFunctionLibrary::ListObjects(const FString &InBucketName, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	return SIMPLE_OSS.ListObjects(InBucketName, ObjectName, MaxNumber);
}

bool USimpleOSSFunctionLibrary::ListObjectsByFilePaths(const FString &InBucketName, const FString &ServerFileRelativePaths, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	return SIMPLE_OSS.ListObjectsByPrefix(InBucketName, ServerFileRelativePaths,ObjectName, MaxNumber);
}

bool USimpleOSSFunctionLibrary::RecursiveListObjectsByFilePaths(const FString &InBucketName, const FString &ServerFileRelativePaths, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	return SIMPLE_OSS.RecursiveListObjectsByPrefix(InBucketName, ServerFileRelativePaths, ObjectName, MaxNumber);
}
//
//bool USimpleOSSFunctionLibrary::SetObjectStorageType(const FString &InBucketName, const FString &InObjectName, EOSSStorageType OSSStorageType)
//{
//	return SIMPLE_OSS.SetObjectStorageType(InBucketName, InObjectName, OSSStorageType);
//}
//
//EOSSStorageType USimpleOSSFunctionLibrary::GetObjectStorageType(const FString &InBucketName, const FString &InObjectName)
//{
//	return SIMPLE_OSS.GetObjectStorageType(InBucketName, InObjectName);
//}

bool USimpleOSSFunctionLibrary::CreateSymlink(const FString &InBucketName, const FString &InObjectName, const FString &InLinkObjectName)
{
	return SIMPLE_OSS.CreateSymlink(InBucketName, InObjectName, InLinkObjectName);
}

FString USimpleOSSFunctionLibrary::GetSymlink(const FString &InBucketName, const FString &InLinkName)
{
	return SIMPLE_OSS.GetSymlink(InBucketName, InLinkName);
}

bool USimpleOSSFunctionLibrary::GetObjectToLocal(const FString& InBucketName, const FString& InObjectName, const FString& InLocalPaths, const FRange& Range)
{
	return SIMPLE_OSS.GetObjectToLocal(InBucketName, InObjectName, InLocalPaths, Range, nullptr);
}

bool USimpleOSSFunctionLibrary::GetObjectToLocalByURL(const FString& URL, const FString& InLocalPaths, FRange Range)
{
	return SIMPLE_OSS.GetObjectToLocalByURL(URL, InLocalPaths, Range);
}

bool USimpleOSSFunctionLibrary::GetImageObjectByResize(const FVector2D &Size, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectByResize(Size, InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageObjectByCrop(const FVector4 &Size, int32 Ratio, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectByCrop(Size, Ratio,InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageObjectBySharpen(const int32 Sharpen, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectBySharpen(Sharpen, InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageObjectByWaterMark(const FString WaterMarkObjectName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectByWaterMark(WaterMarkObjectName, InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageObjectByRotate(const int32 Rotate, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectByRotate(Rotate, InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageObjectByStyle(const FString OSSStyleName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectByStyle(OSSStyleName, InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageObjectByCascade(const TArray<FString> &CascadeCommand, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageObjectByCascade(CascadeCommand, InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::GetImageInfo(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	return SIMPLE_OSS.GetImageInfo(InBucketName, InObjectName, InFileNametoSave);
}

bool USimpleOSSFunctionLibrary::AbortMultipartUpload(const FString &InBucketName, const FString &InObjectName, const FString &InUploadId)
{
	return SIMPLE_OSS.AbortMultipartUpload(InBucketName, InObjectName, InUploadId);
}

bool USimpleOSSFunctionLibrary::GeneratePresignedUrl(FString &URL, const FString &InBucketName, const FString &InObjectName)
{
	return SIMPLE_OSS.GeneratePresignedUrl(URL, InBucketName, InObjectName);
}

bool USimpleOSSFunctionLibrary::UploadPart(FString &InUploadId, const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize /*= 1024 * 1024 * 10*/, const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	if (PartSize == 0)
	{
		PartSize = 1024 * 1024 * 10;
	}
	return SIMPLE_OSS.UploadPart(InUploadId, InBucketName, InObjectName, InLocalPaths, PartSize,OSSMeta);
}

bool USimpleOSSFunctionLibrary::PutByteObjectMemoryByURL(const FString& URL, const TArray<uint8>& Data, const TMap<FString, FString>& OSSMeta)
{
	return SIMPLE_OSS.PutObjectMemoryByURL(URL, Data, OSSMeta);
}

bool USimpleOSSFunctionLibrary::ResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, int64 PartSize, const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	if (PartSize == 0)
	{
		PartSize = 1024 * 1024 * 10;
	}
	return SIMPLE_OSS.ResumableUploadObject(InBucketName, InObjectName, InUploadFilePath, PartSize, nullptr, OSSMeta);
}

//bool USimpleOSSFunctionLibrary::AppendObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName /*=""*/)
//{
//	return SIMPLE_OSS.AppendObject(InBucketName,InLocalPaths,InObjectName);
//}

bool USimpleOSSFunctionLibrary::PutObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName /*= FString()*/, const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	return SIMPLE_OSS.PutObject(InBucketName, InLocalPaths, InObjectName,nullptr,OSSMeta);
}

bool USimpleOSSFunctionLibrary::PutObjectByURL(const FString& URL, const FString& InLocalPaths, const TMap<FString, FString> &OSSMeta)
{
	return SIMPLE_OSS.PutObjectByURL(URL, InLocalPaths, OSSMeta);
}

bool USimpleOSSFunctionLibrary::PutStringObjectByMemory(const FString &InBucketName, const FString &InObjectName, const FString &Data, const TMap<FString, FString> &OSSMeta)
{
	return SIMPLE_OSS.PutObjectByMemory(InBucketName,InObjectName, Data,nullptr, OSSMeta);
}

bool USimpleOSSFunctionLibrary::PutByteObjectByMemory(const FString& InBucketName, const FString& InObjectName, const TArray<uint8>& Data, const TMap<FString, FString>& OSSMeta)
{
	return SIMPLE_OSS.PutObjectByMemory(InBucketName, InObjectName, Data, nullptr, OSSMeta);
}

bool USimpleOSSFunctionLibrary::PutStringObjectMemoryByURL(const FString& URL, const FString& Data, const TMap<FString, FString>& OSSMeta)
{
	return SIMPLE_OSS.PutObjectMemoryByURL(URL, Data, OSSMeta);
}

bool USimpleOSSFunctionLibrary::GetObjectStringToMemory(const FString& InBucketName, const FString& InObjectName, FString& ContextString, FRange Range)
{
	return SIMPLE_OSS.GetObjectToMemory(InBucketName, InObjectName, ContextString, Range, nullptr);
}

bool USimpleOSSFunctionLibrary::GetObjectByteToMemory(const FString& InBucketName, const FString& InObjectName, TArray<uint8>& OutData, FRange Range)
{
	return SIMPLE_OSS.GetObjectToMemory(InBucketName, InObjectName, OutData, Range, nullptr);
}

bool USimpleOSSFunctionLibrary::GetObjectStringToMemoryByURL(const FString& URL, FString& ContextString, FRange Range)
{
	return SIMPLE_OSS.GetObjectToMemoryByURL(URL, ContextString, Range);
}

bool USimpleOSSFunctionLibrary::GetObjectByteToMemoryByURL(const FString& URL, TArray<uint8>& OutBytes, FRange Range)
{
	return SIMPLE_OSS.GetObjectToMemoryByURL(URL, OutBytes, Range);
}

bool USimpleOSSFunctionLibrary::ResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, int32 PartSize /*= 1024 * 1024 * 10*/)
{
	if (PartSize == 0)
	{
		PartSize = 1024 * 1024 * 10;
	}

	return SIMPLE_OSS.ResumableDownloadObject(InBucketName, InObjectName, InDownloadFilePath, PartSize, NULL);
}

bool USimpleOSSFunctionLibrary::CopyObject(const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName)
{
	return SIMPLE_OSS.CopyObject(InSourceBucketName, InSourceObjectName, InCopyBucketName, InCopyObjectName);
}
//
//bool USimpleOSSFunctionLibrary::CopyPart(FString &UploadID, const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName)
//{
//	return SIMPLE_OSS.CopyPart(UploadID, InSourceBucketName, InSourceObjectName, InCopyBucketName, InCopyObjectName);
//}

bool USimpleOSSFunctionLibrary::DeleteObject(const FString &InBucketName, const FString &InObjectName)
{
	return SIMPLE_OSS.DeleteObject(InBucketName, InObjectName);
}

bool USimpleOSSFunctionLibrary::DeleteObjectByPrefix(const FString &InBucketName, const FString &InPrefix)
{
	return SIMPLE_OSS.DeleteObjectByPrefix(InBucketName, InPrefix);
}

bool USimpleOSSFunctionLibrary::DeleteObjectByFilePath(const FString &InBucketName, const FString &InServerFilePath)
{
	return SIMPLE_OSS.DeleteObjectByPrefix(InBucketName, InServerFilePath);
}

bool USimpleOSSFunctionLibrary::DeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName)
{
	return SIMPLE_OSS.DeleteObjects(InBucketName, InObjectName);
}

bool USimpleOSSFunctionLibrary::ListMultipartUploads(const FString &InBucketName, TArray<FMultipartPartsUploadsInfo> &MultipartPartsInfo, int32 MaxParts /*= 1000*/)
{
	return SIMPLE_OSS.ListMultipartUploads(InBucketName, MultipartPartsInfo, MaxParts);
}

DECLARATION_CALLBACK_DELEGATE(AsyncGetObjectToLocal)
void USimpleOSSFunctionLibrary::AsyncGetObjectToLocal(const FString &InBucketName, const FString& InObjectName, const FString& InLocalPaths, FRange Range, FProgressCallbackDelegate InProgressCallback)
{
	AsyncGetObjectToLocal_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncGetObjectToLocal(InBucketName, InObjectName, InLocalPaths, Range, AsyncGetObjectToLocal_Local);
}

DECLARATION_CALLBACK_DELEGATE(AsyncGetObjectToLocalByURL)
DECLARATION_END_CALLBACK_DELEGATE(AsyncGetObjectToLocalByURL)
DECLARATION_START_CALLBACK_DELEGATE(AsyncGetObjectToLocalByURL)
void USimpleOSSFunctionLibrary::AsyncGetObjectToLocalByURL(const FString& URL, const FString& InLocalPaths, FRange Range, FProgressCallbackDelegate InProgressCallback, FBufferByOSSCallbackDelegate InStartCallback, FBufferByOSSCallbackDelegate InEndCallback)
{
	AsyncGetObjectToLocalByURL_Delegate = InProgressCallback;
	AsyncGetObjectToLocalByURL_Start_Delegate = InStartCallback;
	AsyncGetObjectToLocalByURL_End_Delegate = InEndCallback;
	SIMPLE_OSS.AsyncGetObjectToLocalByURL(URL, InLocalPaths, Range, AsyncGetObjectToLocalByURL_Local, AsyncGetObjectToLocalByURL__Start_Delegate,AsyncGetObjectToLocalByURL__End_Delegate);
}

DECLARATION_CALLBACK_DELEGATE(AsyncGetObjectStringToMemory)
DECLARATION_END_CALLBACK_DELEGATE(AsyncGetObjectStringToMemory)
DECLARATION_START_CALLBACK_DELEGATE(AsyncGetObjectStringToMemory)
void USimpleOSSFunctionLibrary::AsyncGetObjectStringToMemory(const FString& InBucketName, const FString& InObjectName, FRange Range, FProgressCallbackDelegate InProgressCallback, FBufferByOSSCallbackDelegate InStartCallback, FBufferByOSSCallbackDelegate InEndCallback)
{
	AsyncGetObjectStringToMemory_Delegate = InProgressCallback;
	AsyncGetObjectStringToMemory_Start_Delegate = InStartCallback;
	AsyncGetObjectStringToMemory_End_Delegate = InEndCallback;
	SIMPLE_OSS.AsyncGetObjectToMemory(InBucketName, InObjectName, Range, AsyncGetObjectStringToMemory_Local, AsyncGetObjectStringToMemory__Start_Delegate, AsyncGetObjectStringToMemory__End_Delegate);
}

DECLARATION_CALLBACK_DELEGATE(AsyncGetObjectBytesToMemory)
DECLARATION_END_BYTE_CALLBACK_DELEGATE(AsyncGetObjectBytesToMemory)
DECLARATION_START_CALLBACK_DELEGATE(AsyncGetObjectBytesToMemory)
void USimpleOSSFunctionLibrary::AsyncGetObjectBytesToMemory(const FString& InBucketName, const FString& InObjectName, FRange Range, FProgressCallbackDelegate InProgressCallback, FBufferByOSSCallbackDelegate InStartCallback, FBytesBufferByOSSCallbackDelegate InEndCallback)
{
	AsyncGetObjectBytesToMemory_Delegate = InProgressCallback;
	AsyncGetObjectBytesToMemory_Start_Delegate = InStartCallback;
	AsyncGetObjectBytesToMemory_End_Delegate = InEndCallback;
	SIMPLE_OSS.AsyncGetObjectToMemory(InBucketName, InObjectName, Range, AsyncGetObjectBytesToMemory_Local, AsyncGetObjectBytesToMemory__Start_Delegate, AsyncGetObjectBytesToMemory__End_Delegate);
}

DECLARATION_CALLBACK_DELEGATE(AsyncGetObjectStringToMemoryByURL)
DECLARATION_END_CALLBACK_DELEGATE(AsyncGetObjectStringToMemoryByURL)
DECLARATION_START_CALLBACK_DELEGATE(AsyncGetObjectStringToMemoryByURL)
void USimpleOSSFunctionLibrary::AsyncGetObjectStringToMemoryByURL(const FString& URL, FRange Range, FProgressCallbackDelegate InProgressCallback, FBufferByOSSCallbackDelegate InStartCallback, FBufferByOSSCallbackDelegate InEndCallback)
{
	AsyncGetObjectStringToMemoryByURL_Delegate = InProgressCallback;
	AsyncGetObjectStringToMemoryByURL_End_Delegate = InEndCallback;
	AsyncGetObjectStringToMemoryByURL_Start_Delegate = InStartCallback;
	SIMPLE_OSS.AsyncGetObjectToMemoryByURL(URL, Range, AsyncGetObjectStringToMemoryByURL_Local, AsyncGetObjectStringToMemoryByURL__Start_Delegate, AsyncGetObjectStringToMemoryByURL__End_Delegate);
}

DECLARATION_CALLBACK_DELEGATE(AsyncGetObjectByteToMemoryByURL)
DECLARATION_END_BYTE_CALLBACK_DELEGATE(AsyncGetObjectByteToMemoryByURL)
DECLARATION_START_CALLBACK_DELEGATE(AsyncGetObjectByteToMemoryByURL)
void USimpleOSSFunctionLibrary::AsyncGetObjectByteToMemoryByURL(const FString& URL, FRange Range, FProgressCallbackDelegate InProgressCallback, FBufferByOSSCallbackDelegate InStartCallback, FBytesBufferByOSSCallbackDelegate InEndCallback)
{
	AsyncGetObjectByteToMemoryByURL_Delegate = InProgressCallback;
	AsyncGetObjectByteToMemoryByURL_End_Delegate = InEndCallback;
	AsyncGetObjectByteToMemoryByURL_Start_Delegate = InStartCallback;
	SIMPLE_OSS.AsyncGetObjectToMemoryByURL(URL, Range, AsyncGetObjectByteToMemoryByURL_Local, AsyncGetObjectByteToMemoryByURL__Start_Delegate, AsyncGetObjectByteToMemoryByURL__End_Delegate);
}

DECLARATION_CALLBACK_DELEGATE(AsyncResumableDownloadObject)
void USimpleOSSFunctionLibrary::AsyncResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, int32 PartSize,FProgressCallbackDelegate InProgressCallback)
{
	if (PartSize == 0)
	{
		PartSize = 1024 * 10;
	}
	AsyncResumableDownloadObject_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncResumableDownloadObject(InBucketName, InObjectName, InDownloadFilePath, PartSize, AsyncResumableDownloadObject_Local);
}

DECLARATION_CALLBACK_DELEGATE_PART(AsyncUploadPart)
FString USimpleOSSFunctionLibrary::AsyncUploadPart(const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize, FCallbackUploadPartDelegate InProgressCallback, const TMap<FString, FString> &OSSMeta)
{
	if (PartSize == 0)
	{
		PartSize = 1024 * 1024 * 10;
	}
	AsyncUploadPart_Delegate = InProgressCallback;
	return SIMPLE_OSS.AsyncUploadPart(InBucketName, InObjectName, InLocalPaths, PartSize, AsyncUploadPart_Local, OSSMeta);
}

DECLARATION_CALLBACK_DELEGATE(AsyncPutObject)
void USimpleOSSFunctionLibrary::AsyncPutObject(const FString &InBucketName,  const FString &InObjectName /*= FString()*/,const FString &InLocalPaths, FProgressCallbackDelegate InProgressCallback /*= nullptr*/, const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	AsyncPutObject_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncPutObject(InBucketName, InLocalPaths,InObjectName, AsyncPutObject_Local, OSSMeta);
}

DECLARATION_CALLBACK_DELEGATE(AsyncPutObjectByURL)
void USimpleOSSFunctionLibrary::AsyncPutObjectByURL(const FString& URL, const FString& InLocalPaths, FProgressCallbackDelegate InProgressCallback, const TMap<FString, FString> &OSSMeta)
{
	AsyncPutObjectByURL_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncPutObjectByURL(URL, InLocalPaths, OSSMeta, AsyncPutObjectByURL_Local);
}

DECLARATION_CALLBACK_DELEGATE(AsyncPutObjectStringByMemory)
void USimpleOSSFunctionLibrary::AsyncPutObjectStringByMemory(const FString& InBucketName, const FString& InObjectName, const FString& Data, FProgressCallbackDelegate InProgressCallback, const TMap<FString, FString>& OSSMeta)
{
	AsyncPutObjectStringByMemory_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncPutObjectByMemory(InBucketName, InObjectName, Data, AsyncPutObjectStringByMemory_Local, OSSMeta);
}

DECLARATION_CALLBACK_DELEGATE(AsyncPutObjectBytesByMemory)
void USimpleOSSFunctionLibrary::AsyncPutObjectBytesByMemory(const FString& InBucketName, const FString& InObjectName, const TArray<uint8>& Data, FProgressCallbackDelegate InProgressCallback, const TMap<FString, FString>& OSSMeta)
{
	AsyncPutObjectBytesByMemory_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncPutObjectByMemory(InBucketName, InObjectName, Data, AsyncPutObjectBytesByMemory_Local, OSSMeta);
}

DECLARATION_CALLBACK_DELEGATE(AsyncPutObjectStringMemoryByURL)
void USimpleOSSFunctionLibrary::AsyncPutObjectStringMemoryByURL(const FString& URL, const FString& Data, FProgressCallbackDelegate InProgressCallback, const TMap<FString, FString>& OSSMeta)
{
	AsyncPutObjectStringMemoryByURL_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncPutObjectMemoryByURL(URL, Data, OSSMeta, AsyncPutObjectStringMemoryByURL_Local);
}

DECLARATION_CALLBACK_DELEGATE(AsyncPutObjectByteMemoryByURL)
void USimpleOSSFunctionLibrary::AsyncPutObjectByteMemoryByURL(const FString& URL, const TArray<uint8>& Data, FProgressCallbackDelegate InProgressCallback, const TMap<FString, FString>& OSSMeta)
{
	AsyncPutObjectByteMemoryByURL_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncPutObjectMemoryByURL(URL, Data, OSSMeta,AsyncPutObjectByteMemoryByURL_Local);
}

DECLARATION_CALLBACK_DELEGATE(AsyncResumableUploadObject)
void USimpleOSSFunctionLibrary::AsyncResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, int64 PartSize, FProgressCallbackDelegate InProgressCallback /*= nullptr*/, const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	if (PartSize == 0)
	{
		PartSize = 1024 * 1024 * 10;
	}
	AsyncResumableUploadObject_Delegate = InProgressCallback;
	SIMPLE_OSS.AsyncResumableUploadObject(InBucketName, InObjectName, InUploadFilePath, PartSize,AsyncResumableUploadObject_Local, OSSMeta);
}
