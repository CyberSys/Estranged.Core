// Estranged is a trade mark of Alan Edwardes.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EstMusic.h"
#include "EstGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FEstMenuVisibilityContext
{
	GENERATED_BODY()

	FEstMenuVisibilityContext()
		: FEstMenuVisibilityContext(false)
	{
	}

	FEstMenuVisibilityContext(bool bInIsMenuVisible)
		: FEstMenuVisibilityContext(bInIsMenuVisible, NAME_None)
	{
	}

	FEstMenuVisibilityContext(bool bInIsMenuVisible, FName InRedirectToMenu)
		: FEstMenuVisibilityContext(bInIsMenuVisible, false, InRedirectToMenu)
	{
	}

	FEstMenuVisibilityContext(bool bInIsMenuVisible, bool bInIsMenuVisibleForever)
		: FEstMenuVisibilityContext(bInIsMenuVisible, bInIsMenuVisibleForever, NAME_None)
	{
	}

	FEstMenuVisibilityContext(bool bInIsMenuVisible, bool bInIsMenuVisibleForever, FName InMenuSection)
	{
		bIsMenuVisible = bInIsMenuVisible;
		bIsMenuVisibleForever = bInIsMenuVisibleForever;
		MenuSection = InMenuSection;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMenuVisibleForever;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMenuVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MenuSection;
};

UCLASS()
class ESTCORE_API UEstGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	virtual void Shutdown() override;

	virtual void PreLoadMap(const FString & InMapName);

	UFUNCTION(BlueprintCallable, Category = Music)
	virtual void FadeMusic();

	UFUNCTION(BlueprintCallable, Category = Music)
	virtual void StopMusic();

	UFUNCTION(BlueprintCallable, Category = Music)
	virtual void PlayMusic(FEstMusic Music);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Music)
	class USoundClass* MusicSoundClass;

	UFUNCTION(BlueprintPure, Category = Music)
	virtual FEstMusic GetMusic();

	UFUNCTION(BlueprintPure, Category = UI)
	virtual bool GetMenuVisibleForever() { return VisibilityContext.bIsMenuVisibleForever; };

	UFUNCTION(BlueprintCallable, Category = UI)
	virtual void SetMenuVisibility(FEstMenuVisibilityContext InVisibilityContext);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> MenuWidgetType;

	TSharedPtr<SWidget> GetRawMenuWidget() { return MenuSlateWidget; }
private:
	class UAudioComponent* AudioComponent;

	virtual float GetPlayPosition();
	virtual void PlayMusicInternal(FEstMusic Music);
	virtual bool Tick(float DeltaTime);
	bool LazilyCreateAudioComponent();

	FDelegateHandle TickDelegateHandle;
	TOptional<FEstMusic> NextMusic;
	float MusicStartTime;
	float GameInstanceTime;
	float MusicFadeCompleteTime;
	bool bWasFadingOut;

	TSharedPtr<SWidget> MenuSlateWidget;
	class UEstMenuWidget* MenuUserWidget;
	FEstMenuVisibilityContext VisibilityContext;
};
