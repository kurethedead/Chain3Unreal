// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Chain3DataSubsystem.generated.h"

/**
 * By default this subsystem will auto load persistent state on game start, and auto save on game end.
 If you want to disable this, extend this class with a blueprint and unset the flags bLoadDataOnStart/bSaveDataOnExit.
 */
UCLASS(DisplayName = "Chain Data Subsystem", Blueprintable)
class CHAIN3UNREAL_API UChain3DataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase &Collection) override;
    virtual void Deinitialize() override;
    virtual bool ShouldCreateSubsystem(UObject *Outer) const override;

    // Loads persistent state from exit.door and shareddata.data
    // By default, this subsystem calls this on Initialize
    UFUNCTION(BlueprintCallable)
    void LoadData();

    // Saves persistent state to exit.door and shareddata.data
    // By default, this subsystem calls this on Deinitialize
    UFUNCTION(BlueprintCallable)
    void SaveData();

    UFUNCTION(BlueprintPure)
    FString GetEnterDoor() { return EnterDoor; }

    UFUNCTION(BlueprintCallable)
    void SetExitDoor(FString _ExitDoor) { ExitDoor = _ExitDoor; }

    UFUNCTION(BlueprintCallable)
    void SetFlag(FString Flag);

    UFUNCTION(BlueprintCallable)
    void ClearFlag(FString Flag);

    UFUNCTION(BlueprintPure)
    bool HasFlag(FString Flag);

    // This function returns a copy of all existing flags.
    // Changes in this array will not be saved unless OverwriteFlags is called later.
    // UFUNCTION(BlueprintPure)
    // TArray<FString> GetAllFlags();

    // This function will delete all existing flags and replace them with the input flags.
    // Use with caution.
    // UFUNCTION(BlueprintCallable)
    // void OverwriteFlags(TArray<FString> Flags);

protected:
    FString GetDataPath();
    FString StripString(FString Input);

    // Should the subsystem auto load data on game start?
    UPROPERTY(EditAnywhere)
    bool bLoadDataOnStart = true;

    // Should the subsystem auto save data on game end?
    UPROPERTY(EditAnywhere)
    bool bSaveDataOnExit = true;

private:
    UPROPERTY()
    FString EnterDoor;

    UPROPERTY()
    FString ExitDoor;

    UPROPERTY()
    TArray<FString> SharedData;
};