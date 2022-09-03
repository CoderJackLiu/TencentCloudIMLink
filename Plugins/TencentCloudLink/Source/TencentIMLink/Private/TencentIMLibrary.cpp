// Fill out your copyright notice in the Description page of Project Settings.


#include "TencentIMLink/Public/TencentIMLibrary.h"

#include <string>

#include "Misc/Paths.h"
#include <string>

FString UTencentIMLibrary::GetSDKVersion()
{
	const std::string stdStrTemp2(V2TIMManager::GetInstance()->GetVersion().CString());
	FString tempUserText{stdStrTemp2.c_str()};
	return 	tempUserText;
}

int64 UTencentIMLibrary::GetServerTime()
{
	return V2TIMManager::GetInstance()->GetServerTime();
}

void UTencentIMLibrary::LogIn(const FString& UserId)
{
	
}
