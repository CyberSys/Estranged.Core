#include "EstCore.h"
#include "UnrealNetwork.h"
#include "EstResourceComponent.h"

UEstResourceComponent::UEstResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = .1f;
	MaxResource = 100.f;
	Resource = 100.f;

	SetIsReplicated(true);
}

void UEstResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->ActorHasTag(TAG_DEAD))
	{
		return;
	}

	if (FMath::IsNearlyZero(ResourceChangePerSecond))
	{
		return;
	}

	ChangeResource(ResourceChangePerSecond * DeltaTime, GetOwner());
}

void UEstResourceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UEstResourceComponent, IsFrozen);
	DOREPLIFETIME(UEstResourceComponent, Resource);
	DOREPLIFETIME(UEstResourceComponent, MaxResource);
}

bool UEstResourceComponent::ChangeResource(float Amount, AActor* Instigator)
{
	if (IsFrozen)
	{
		return false;
	}

	if (Amount > 0.f && IsFull())
	{
		return false;
	}

	if (Amount < 0.f && IsDepleted())
	{
		return false;
	}

	Resource = FMath::Clamp(Resource + Amount, 0.f, MaxResource);

	if (IsDepleted())
	{
		OnDepleted.Broadcast(Instigator);
	}

	return true;
}