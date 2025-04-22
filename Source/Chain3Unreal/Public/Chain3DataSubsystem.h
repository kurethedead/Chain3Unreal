// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Chain3DataSubsystem.generated.h"

/**
 * This subsystem handles interacting with the CHAIN3 persistent data system (doors + shared data).
 */
UCLASS(DisplayName = "Chain Data Subsystem", Blueprintable)
class CHAIN3UNREAL_API UChain3DataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase &Collection) override;
    virtual void Deinitialize() override;
    virtual bool ShouldCreateSubsystem(UObject *Outer) const override;

    // Loads persistent state from enter.door and shareddata.data
    // By default, this subsystem calls this when the game starts.
    // If you want to disable this behaviour, extend this class with a blueprint and set member variable bLoadDataOnStart = false.
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void LoadData();

    // Saves persistent state to exit.door and shareddata.data
    // By default, this subsystem calls this when the game ends.
    // If you want to disable this, extend this class with a blueprint and set member variable bSaveDataOnExit = false.
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void SaveData();

    // Gets the door you entered from enter.door
    UFUNCTION(BlueprintPure, Category = "Chain Data")
    FString GetEnterDoor() { return EnterDoor; }

    // Sets the door you will exit from (writes to exit.door)
    // You will still need to save the data
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void SetExitDoor(FString _ExitDoor) { ExitDoor = _ExitDoor; }

    // Set a shared data flag.
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void SetFlag(FString Flag);

    // Clear (remove) a shared data flag.
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void ClearFlag(FString Flag);

    // Check if the shared data contains a specified flag.
    UFUNCTION(BlueprintPure, Category = "Chain Data")
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