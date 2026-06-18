#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/PlayerData.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Running UMETA(DisplayName = "Running"),
	Sprinting UMETA(DisplayName = "Sprinting"),
	Jumping UMETA(DisplayName = "Jumping"),
	Shooting UMETA(DisplayName = "Shooting"),
	Passing UMETA(DisplayName = "Passing"),
	Defending UMETA(DisplayName = "Defending"),
	Tackling UMETA(DisplayName = "Tackling"),
	Injured UMETA(DisplayName = "Injured"),
	Tired UMETA(DisplayName = "Tired")
};

USTRUCT(BlueprintType)
struct FPlayerGameplayStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GoalsScored = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Assists = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TacklesMade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PassesCompleted = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PassesAttempted = 0;
};

DECLARE_MULTICAST_DELEGATE_One(FOnPlayerStateChanged, EPlayerState);
DECLARE_MULTICAST_DELEGATE_One(FOnStaminaDepleted, float);
DECLARE_MULTICAST_DELEGATE_Two(FOnPlayerDamaged, float, FVector);
DECLARE_MULTICAST_DELEGATE_One(FOnPlayerScored, int32);

/**
 * In-game player character with full gameplay mechanics
 */
ACLASS()
class ULTIMATESPORTSGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ============ INITIALIZATION ============

	UFUNCTION(BlueprintCallable, Category = "Player")
	void InitializePlayer(const FPlayerData& InPlayerData);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void LoadPlayerModel();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void ApplyPlayerAppearance();

	// ============ MOVEMENT ============

	UFUNCTION(BlueprintCallable, Category = "Player|Movement")
	void Run(const FVector& Direction);

	UFUNCTION(BlueprintCallable, Category = "Player|Movement")
	void Sprint(const FVector& Direction);

	UFUNCTION(BlueprintCallable, Category = "Player|Movement")
	void StopMovement();

	UFUNCTION(BlueprintCallable, Category = "Player|Movement")
	void Jump() override;

	// ============ GAMEPLAY ACTIONS ============

	UFUNCTION(BlueprintCallable, Category = "Player|Action")
	void Shoot(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Player|Action")
	void PassBall(AActor* Teammate);

	UFUNCTION(BlueprintCallable, Category = "Player|Action")
	void Defend(AActor* Opponent);

	UFUNCTION(BlueprintCallable, Category = "Player|Action")
	void Tackle(AActor* Opponent);

	UFUNCTION(BlueprintCallable, Category = "Player|Action")
	void ReceiveBall();

	// ============ STAMINA SYSTEM ============

	UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
	float GetCurrentStamina() const { return GameplayStats.CurrentStamina; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
	void DrainStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
	void RestoreStamina(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Player|Stamina")
	float GetStaminaPercentage() const;

	// ============ HEALTH SYSTEM ============

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	float GetCurrentHealth() const { return GameplayStats.CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Player|Health")
	bool IsAlive() const { return GameplayStats.CurrentHealth > 0.0f; }

	// ============ STATS ============

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	FPlayerData GetPlayerData() const { return PlayerData; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	FPlayerGameplayStats GetGameplayStats() const { return GameplayStats; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	EPlayerState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	int32 GetGoalsScored() const { return GameplayStats.GoalsScored; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	int32 GetAssists() const { return GameplayStats.Assists; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	int32 GetTacklesMade() const { return GameplayStats.TacklesMade; }

	UFUNCTION(BlueprintCallable, Category = "Player|Stats")
	float GetPassCompletionPercentage() const;

	// ============ ANIMATIONS ============

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void PlayShootingAnimation();

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void PlayPassingAnimation();

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void PlayDefendingAnimation();

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void PlayInjuryAnimation();

	UFUNCTION(BlueprintCallable, Category = "Player|Animation")
	void PlayIdleAnimation();

	// ============ EVENTS ============

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnPlayerStateChanged OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnStaminaDepleted OnStaminaDepleted;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnPlayerDamaged OnPlayerDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Player|Events")
	FOnPlayerScored OnPlayerScored;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPlayerData PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPlayerGameplayStats GameplayStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerState CurrentState = EPlayerState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float RunSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina")
	float StaminaDrainRate = 0.5f; // Sprint

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina")
	float StaminaDrainRateRun = 0.25f; // Running

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Stamina")
	float StaminaRestoreRate = 0.3f; // Idle

	// Position-specific settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Position")
	FString PositionAnimationBlueprintPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Appearance")
	USkeletalMeshComponent* CharacterMesh;

	// Internal functions
	void UpdateState(EPlayerState NewState);
	void UpdateStaminaSystem(float DeltaTime);
	void ApplyStatModifiers();
	void SetupMeshAndMaterial();
	bool CanPerformAction() const;
};
