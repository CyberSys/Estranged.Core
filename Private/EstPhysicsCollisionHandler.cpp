// Estranged is a trade mark of Alan Edwardes.

#include "EstCore.h"
#include "EstImpactManifest.h"
#include "PhysicsPublic.h"
#include "EstPhysicsCollisionHandler.h"

void UEstPhysicsCollisionHandler::HandlePhysicsCollisions_AssumesLocked(TArray<FCollisionNotifyInfo>& PendingCollisionNotifies)
{
	// Fire any collision notifies in the queue.
	for (int32 i = 0; i<PendingCollisionNotifies.Num(); i++)
	{
		// If it hasn't been long enough since our last sound, just bail out
		const float TimeSinceLastImpact = GetWorld()->GetTimeSeconds() - LastImpactSoundTime;
		if (TimeSinceLastImpact < ImpactReFireDelay)
		{
			break;
		}

		// See if this impact is between 2 valid actors
		const FCollisionNotifyInfo& NotifyInfo = PendingCollisionNotifies[i];
		if (NotifyInfo.IsValidForNotify() &&
			NotifyInfo.RigidCollisionData.ContactInfos.Num() > 0)
		{
			CustomHandleCollision_AssumesLocked(NotifyInfo.Info0, NotifyInfo.Info1, NotifyInfo.RigidCollisionData);
		}
	}
}

UPhysicalMaterial* UEstPhysicsCollisionHandler::GetPhysicalMaterialFromCollision(const FRigidBodyCollisionInfo CollisionInfo)
{
	const TWeakObjectPtr<UPrimitiveComponent> Component = CollisionInfo.Component;
	if (Component == nullptr)
	{
		return nullptr;
	}

	const UMaterialInterface* Material = Component->GetMaterial(0);
	if (Material == nullptr)
	{
		return nullptr;
	}

	return Material->GetPhysicalMaterial();
}

void UEstPhysicsCollisionHandler::CustomHandleCollision_AssumesLocked(const FRigidBodyCollisionInfo& MyInfo, const FRigidBodyCollisionInfo& OtherInfo, const FCollisionImpactData& RigidCollisionData)
{
	FRigidBodyContactInfo ContactInfo = RigidCollisionData.ContactInfos[0];

	FBodyInstance* BodyInst0 = MyInfo.GetBodyInstance();
	FBodyInstance* BodyInst1 = OtherInfo.GetBodyInstance();

	if (BodyInst0 && BodyInst1)
	{
		// Find relative velocity.
		FVector Velocity0 = BodyInst0->GetUnrealWorldVelocityAtPoint_AssumesLocked(ContactInfo.ContactPosition);
		FVector AngularVel0 = BodyInst0->GetUnrealWorldAngularVelocityInRadians_AssumesLocked();

		FVector Velocity1 = BodyInst1->GetUnrealWorldVelocityAtPoint_AssumesLocked(ContactInfo.ContactPosition);
		FVector AngularVel1 = BodyInst1->GetUnrealWorldAngularVelocityInRadians_AssumesLocked();

		const FVector RelVel = Velocity1 - Velocity0;

		// Then project along contact normal, and take magnitude.
		float ImpactVelMag = FMath::Abs(RelVel | ContactInfo.ContactNormal);

		if (ImpactVelMag < ImpactThreshold)
		{
			return;
		}

		UWorld* World = GetWorld();
		if (World == nullptr)
		{
			return;
		}

		FEstImpactEffect DefaultImpactEffect;
		DefaultImpactEffect.Sound = DefaultImpactSound;

		UPhysicalMaterial* MyMaterial = GetPhysicalMaterialFromCollision(MyInfo);
		if (MyMaterial != nullptr)
		{
			const FEstImpactEffect ImpactEffect = UEstGameplayStatics::FindImpactEffect(ImpactManifest->ImpactEffects, MyMaterial, DefaultImpactEffect);
			UEstGameplayStatics::DeployImpactEffect(ImpactEffect, ContactInfo.ContactPosition, ContactInfo.ContactNormal, MyInfo.Component.Get());
		}

		UPhysicalMaterial* OtherMaterial = GetPhysicalMaterialFromCollision(OtherInfo);
		if (OtherMaterial != nullptr)
		{
			const FEstImpactEffect ImpactEffect = UEstGameplayStatics::FindImpactEffect(ImpactManifest->ImpactEffects, OtherMaterial, DefaultImpactEffect);
			UEstGameplayStatics::DeployImpactEffect(ImpactEffect, ContactInfo.ContactPosition, ContactInfo.ContactNormal, OtherInfo.Component.Get());
		}

		LastImpactSoundTime = World->GetTimeSeconds();
	}
}
