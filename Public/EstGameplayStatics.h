#pragma once

#include "EstImpactEffect.h"
#include "EstSave.h"
#include "EstGameplayStatics.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBulletHitDelegate, FHitResult, HitResult);

UCLASS()
class ESTCORE_API UEstGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Calculate a new rotation with a random spread under MaxSpread. */
	UFUNCTION(BlueprintPure, Category = Projectiles)
	static FRotator RandomProjectileSpread(FRotator InRot, float MaxSpread);

	/** Deploy impact effects at a specified location. */
	UFUNCTION(BlueprintCallable, Category = Projectiles)
	static void DeployImpactEffect(const FEstImpactEffect ImpactEffect, const FVector ImpactPoint, const FVector ImpactNormal, class USceneComponent* Component, const float Scale = 1.0f, class USoundAttenuation* AttenuationOverride = nullptr);

	/** Slower version of DeployImpactEffect with extra checks to ensure any pointers are still valid in memory */
	static void DeployImpactEffectChecked(const FEstImpactEffect ImpactEffect, const FVector ImpactPoint, const FVector ImpactNormal, TWeakObjectPtr<class USceneComponent> Component, const float Scale = 1.0f, TWeakObjectPtr<class USoundAttenuation> AttenuationOverride = nullptr);

	/** Deploy impact effects at a specified location with a delay. */
	UFUNCTION(BlueprintCallable, Category = Projectiles)
	static void DeployImpactEffectDelayed(const FEstImpactEffect ImpactEffect, const FVector ImpactPoint, const FVector ImpactNormal, class USceneComponent* Component, const float Delay, const float Scale = 1.0f, class USoundAttenuation* AttenuationOverride = nullptr);

	/** Find physical impact effects based on a material with an optional default. */
	UFUNCTION(BlueprintPure, Category = Projectiles)
	static struct FEstImpactEffect FindImpactEffect(const class UEstImpactManifest* ImpactEffects, const class UPhysicalMaterial* PhysicalMaterial);

	UFUNCTION(BlueprintPure, Category = Projectiles)
	static FName FindClosestBoneName(USceneComponent * Component, FVector Location);

	/** Pause or unpause all local player controllers. */
	UFUNCTION(BlueprintCallable, Category = Game)
	static void SetPause(bool bIsPaused);

	/** Send a command to all local player controllers */
	UFUNCTION(BlueprintCallable, Category = Game)
	static void ConsoleCommand(const FString& Command);

	UFUNCTION(BlueprintCallable, Category = Game)
	static AActor* MoveActorToLevel(AActor* Actor, ULevel* Level);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "PlayerController", DefaultToSelf = "PlayerController"))
	static void ListActionMappings(const APlayerController* PlayerController, TArray<FName> &SortedActionNames, TMap<FName, FInputChord> &Mappings);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "PlayerController", DefaultToSelf = "PlayerController"))
	static FKey FindBestKeyForAction(APlayerController* PlayerController, FName ActionName, bool bForGamepad);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "PlayerController", DefaultToSelf = "PlayerController"))
	static FKey FindBestKeyForAxis(APlayerController* PlayerController, FName AxisNames, bool bForGamepad);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "PlayerController", DefaultToSelf = "PlayerController"))
	static void AddAxisMapping(APlayerController* PlayerController, FName AxisName, FKey InputKey, float Scale, bool bForGamepad);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "PlayerController", DefaultToSelf = "PlayerController"))
	static void AddActionMapping(APlayerController* PlayerController, FName ActionName, FInputChord InputKey, bool bForGamepad);

	UFUNCTION(BlueprintCallable, Category = Game)
	static FString GetKeyDisplayName(FKey Key);

	/** Get the Estranged player controller for a specified player index. */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static class AEstPlayerController* GetEstPlayerController(UObject* WorldContextObject, int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool IsEditor(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Game")
	static bool IsSlowerMachine();

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static AActor* CreateTransientActor(UObject* WorldContextObject, class UClass* ActorClass);

	UFUNCTION(BlueprintCallable, Category = "Saving")
	static TArray<FString> ListSaveGames(FString Directory);

	UFUNCTION(BlueprintCallable, Category = "Debugging")
	static TArray<FString> GetValidMemoryDumpPaths();

	UFUNCTION(BlueprintPure, Category = Math)
	static bool IsLocationInsideActor(FVector Location, AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Internationalization")
	static TArray<FString> GetAllCultures();

	UFUNCTION(BlueprintCallable, Category = "Input")
	static bool IsUsingGamepad(const APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Sound", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetTransientMasterVolume(UObject* WorldContextObject, const float InTransientMasterVolume);

	UFUNCTION(BlueprintPure, Category = "Saving")
	static FString GenerateSaveFileName() { return FDateTime::Now().ToString(); };

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static AActor* SpawnActor(UObject* WorldContextObject, UClass* ActorClass, FName ActorName, const FTransform &ActorTransform);

	UFUNCTION(BlueprintCallable, Category = "Game")
	static AActor* FindActorByNameAndClassInLevel(const ULevel* Level, FName ActorName, UClass* ActorClass);

	UFUNCTION(BlueprintCallable, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static AActor* FindActorByName(UObject* WorldContextObject, FName ActorName, UClass* ActorClass = nullptr);

	UFUNCTION(BlueprintPure, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static class AEstGameMode* GetEstGameMode(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Game", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static class UEstGameInstance* GetEstGameInstance(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Game")
	static bool CanHumanPickUpActor(ACharacter* Character, AActor * ActorToHold, float MaxMass = 100.f, float MaxRadius = 100.f);

	UFUNCTION(BlueprintCallable, Category = "Game")
	static void TraceBullet(const USceneComponent* SourceComponent, const FVector ExitLocation, const FRotator ExitRotation, const float MaxSpread, const FOnBulletHitDelegate &OnBulletHit, FRotator &AdjustedRotation);

	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static class AEstPlayer* GetEstPlayerPawn(const UObject* WorldContextObject, int32 PlayerIndex);
};
