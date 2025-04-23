// Fill out your copyright notice in the Description page of Project Settings.

#include "Chain3DataSubsystem.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"
#include "Engine/World.h"
#include "Misc/Paths.h"

bool UChain3DataSubsystem::ShouldCreateSubsystem(UObject *Outer) const
{
    // Only create an instance if there is no override implementation defined elsewhere
    TArray<UClass *> ChildClasses;
    GetDerivedClasses(GetClass(), ChildClasses, false);
    if (ChildClasses.Num() > 0)
    {
        return false;
    }

    return Outer->GetWorld()->GetNetMode() < NM_Client;
}

void UChain3DataSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
    if (bLoadDataOnStart)
    {
        LoadData();
    }
}

void UChain3DataSubsystem::Deinitialize() { Super::Deinitialize(); }

FString UChain3DataSubsystem::GetDataPath()
{
#if WITH_EDITOR
    return FPaths::ProjectDir() + "StreamingAssets/";
#else
    FString Path = FPaths::ProjectDir();
    Path.RemoveFromEnd("/");
    FString PathPart, FilenamePart, ExtensionPart;
    FPaths::Split(Path, PathPart, FilenamePart, ExtensionPart);
    return PathPart + "/StreamingAssets/";
#endif
}

FString UChain3DataSubsystem::StripString(FString Input)
{
    return UKismetStringLibrary::Trim(UKismetStringLibrary::TrimTrailing(Input));
}

void UChain3DataSubsystem::LoadData()
{
    FString DataPath = GetDataPath();

    FString EnterString;
    if (FFileHelper::LoadFileToString(EnterString, *(DataPath + "enter.door")))
    {
        EnterDoor = StripString(EnterString);
    }

    // FString ExitString;
    // if (FFileHelper::LoadFileToString(ExitString, *(DataPath + "exit.door")))
    //{
    //     ExitDoor = StripString(ExitString);
    // }

    FString SharedString;
    if (FFileHelper::LoadFileToString(SharedString, *(DataPath + "shareddata.data")))
    {
        TArray<FString> SharedStringArray;
        SharedString.ParseIntoArray(SharedStringArray, TEXT("\n"));

        for (FString S : SharedStringArray)
        {
            FString StrippedString = StripString(S);
            if (!StrippedString.IsEmpty())
            {
                SharedData.Add(StrippedString);
            }
        }
    }
}

void UChain3DataSubsystem::SaveSharedData()
{
    FString DataPath = GetDataPath();
    FFileHelper::SaveStringArrayToFile(SharedData, *(DataPath + "shareddata.data"));
}

void UChain3DataSubsystem::SetFlag(FString Flag)
{
    FString StrippedFlag = StripString(Flag);
    if (!StrippedFlag.IsEmpty() && !SharedData.Contains(StrippedFlag))
    {
        SharedData.Add(StrippedFlag);
    }
}

void UChain3DataSubsystem::ClearFlag(FString Flag) { SharedData.Remove(StripString(Flag)); }

bool UChain3DataSubsystem::HasFlag(FString Flag) { return SharedData.Contains(StripString(Flag)); }

void UChain3DataSubsystem::ExitDoor(FString _ExitDoor)
{
    FString DataPath = GetDataPath();
    FFileHelper::SaveStringToFile(_ExitDoor, *(DataPath + "exit.door"));
    FFileHelper::SaveStringArrayToFile(SharedData, *(DataPath + "shareddata.data"));
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void UChain3DataSubsystem::DeleteFiles(bool bDeleteEnterFile, bool bDeleteExitFile, bool bDeleteSharedDataFile)
{
    FString DataPath = GetDataPath();
    FString EnterPath = DataPath + "enter.door";
    FString ExitPath = DataPath + "exit.door";
    FString SharedDataPath = DataPath + "shareddata.data";
    IFileManager &FileManager = IFileManager::Get();

    if (bDeleteEnterFile && FPaths::ValidatePath(EnterPath) && FPaths::FileExists(EnterPath))
    {
        FileManager.Delete(*EnterPath);
    }
    if (bDeleteExitFile && FPaths::ValidatePath(ExitPath) && FPaths::FileExists(ExitPath))
    {
        FileManager.Delete(*ExitPath);
    }
    if (bDeleteSharedDataFile && FPaths::ValidatePath(SharedDataPath) && FPaths::FileExists(SharedDataPath))
    {
        FileManager.Delete(*SharedDataPath);
    }
}