// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Chain3DataSubsystem.generated.h"

/**
 *
 This subsystem handles interacting with the CHAIN3 persistent data system (doors + shared data).
 Persistent data is stored in <MyProject>/StreamingAssets/. The saved files are:
 
    - enter.door: Contains a string indicating which room you entered from (ex. M1)
    - exit.door: Contains a string indicating which room you are exiting to (ex. K1)
    - shareddata.data: Contains list of strings (separated by newlines)
        indicating flags that keep track of shared game state.
        The convention is to prefix flags by the game the flag originates from.
        (ex. K1_unlocked_green)

 The CHAIN launcher will write the enter.door and shareddata.data files before your game starts,
 and read exit.door and shareddata.data after your game ends.
 This plugin provides an interface to interact with this system.
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
    UFUNCTION(Category = "Chain Data")
    void LoadData();

    // Saves shared data to disk.
    // This is already called by ExitDoor(), so it doesn't need to be called manually unless you are worried about game
    // crashes.
    UFUNCTION(Category = "Chain Data")
    void SaveSharedData();

    // Gets the door you entered from (enter.door)
    UFUNCTION(BlueprintPure, Category = "Chain Data")
    FString GetEnterDoor() { return EnterDoor; }

    // Exit through a door.
    // This will save the shared data and exit door files, then quit the game.
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void ExitDoor(FString _ExitDoor);

    // Set a shared data flag. This will only be saved to disk once you exit through a door (ExitDoor())
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void SetFlag(FString Flag);

    // Clear (remove) a shared data flag. This will only be saved to disk once you exit through a door (ExitDoor())
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void ClearFlag(FString Flag);

    // Check if the shared data contains a specified flag.
    UFUNCTION(BlueprintPure, Category = "Chain Data")
    bool HasFlag(FString Flag);

    // Debug method to clear save files (enter.door, exit.door, shareddata.data)
    UFUNCTION(BlueprintCallable, Category = "Chain Data")
    void DeleteFiles(bool bDeleteEnterFile = true, bool bDeleteExitFile = true, bool bDeleteSharedDataFile = true);

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

private:
    UPROPERTY()
    FString EnterDoor;

    UPROPERTY()
    TArray<FString> SharedData;
};