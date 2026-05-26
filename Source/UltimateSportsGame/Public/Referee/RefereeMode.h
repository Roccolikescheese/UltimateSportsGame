#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Data/PlayerData.h"
#include "Data/TeamData.h"
#include "RefereeMode.generated.h"

UENUM(BlueprintType)
enum class EFoulType : uint8
{
	// Soccer/Football Fouls
	Handball UMETA(DisplayName = "Handball"),
	Offsides UMETA(DisplayName = "Offsides"),
	Pushing UMETA(DisplayName = "Pushing"),
	Tripping UMETA(DisplayName = "Tripping"),
	Fouling UMETA(DisplayName = "Fouling"),
	BackPass UMETA(DisplayName = "Back Pass to Keeper"),
	TimeWasting UMETA(DisplayName = "Time Wasting"),
	Diving UMETA(DisplayName = "Diving/Simulation"),
	ExcessiveCelebration UMETA(DisplayName = "Excessive Celebration"),
	
	// Basketball Fouls
	PersonalFoul UMETA(DisplayName = "Personal Foul"),
	TechnicalFoul UMETA(DisplayName = "Technical Foul"),
	FlagrantFoul UMETA(DisplayName = "Flagrant Foul"),
	Charging UMETA(DisplayName = "Charging"),
	Blocking UMETA(DisplayName = "Blocking"),
	DoubleTeam UMETA(DisplayName = "Double Team"),
	
	// Baseball Fouls
	BallStrike UMETA(DisplayName = "Ball/Strike Call"),
	SafeOut UMETA(DisplayName = "Safe/Out Call"),
	Balk UMETA(DisplayName = "Balk"),
	IllegalPitch UMETA(DisplayName = "Illegal Pitch"),
	
	// Hockey Fouls
	Slashing UMETA(DisplayName = "Slashing"),
	HighStick UMETA(DisplayName = "High Sticking"),
	Hooking UMETA(DisplayName = "Hooking"),
	Interference UMETA(DisplayName = "Interference"),
	OffsideHockey UMETA(DisplayName = "Offsides"),
	Icing UMETA(DisplayName = "Icing"),
	Roughing UMETA(DisplayName = "Roughing"),
	
	// Rugby Fouls
	HighTackle UMETA(DisplayName = "High Tackle"),
	LateHit UMETA(DisplayName = "Late Hit"),
	KickingPlayer UMETA(DisplayName = "Kicking Player"),
	Collapsing UMETA(DisplayName = "Collapsing Ruck"),
	OffballPlay UMETA(DisplayName = "Off-ball Play"),
	
	// Cricket Fouls
	NoballBowling UMETA(DisplayName = "No-ball Bowling"),
	WideDelivery UMETA(DisplayName = "Wide Delivery"),
	
	// Tennis Fouls
	FeetFault UMETA(DisplayName = "Foot Fault"),
	CourtViolation UMETA(DisplayName = "Court Violation"),
	BallToss UMETA(DisplayName = "Illegal Ball Toss"),
	
	// Bowling Fouls
	FoulLine UMETA(DisplayName = "Foul Line Violation"),
	
	None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ECardType : uint8
{
	Yellow UMETA(DisplayName = "Yellow Card"),
	Red UMETA(DisplayName = "Red Card"),
	TechnicalYellow UMETA(DisplayName = "Technical Yellow"),
	TechnicalRed UMETA(DisplayName = "Technical Red"),
	None UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class ERefereeDecision : uint8
{
	FreeKick UMETA(DisplayName = "Free Kick"),
	Penalty UMETA(DisplayName = "Penalty"),
	ThrowIn UMETA(DisplayName = "Throw In"),
	GoalKick UMETA(DisplayName = "Goal Kick"),
	CornerKick UMETA(DisplayName = "Corner Kick"),
	Kickoff UMETA(DisplayName = "Kickoff"),
	YellowCard UMETA(DisplayName = "Yellow Card"),
	RedCard UMETA(DisplayName = "Red Card"),
	Advantage UMETA(DisplayName = "Play Advantage"),
	SafeCall UMETA(DisplayName = "Safe"),
	OutCall UMETA(DisplayName = "Out"),
	Goal UMETA(DisplayName = "Goal"),
	NoGoal UMETA(DisplayName = "No Goal"),
	TimeOut UMETA(DisplayName = "Time Out"),
	Substitution UMETA(DisplayName = "Substitution"),
	None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FFoulEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFoulType FoulType = EFoulType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType CardType = ECardType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERefereeDecision Decision = ERefereeDecision::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FouledPlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CommittingPlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CommittingTeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FoulLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Period = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWasContested = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ReplayCount = 0;
};

USTRUCT(BlueprintType)
struct FRefereeStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalYellowCards = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalRedCards = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalFoulsCalledCorrectly = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalFoulsCalledIncorrectly = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalGamesMissed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccuracyRating = 0.0f; // 0-100

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ContestationsAllowed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ConsistencyRating = 0.0f; // 0-100

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GamesRefereed = 0;

	float CalculateOverallRating() const;
};

USTRUCT(BlueprintType)
struct FRefereeProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RefereeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LastName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Nationality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 YearsExperience = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CertificationLevel; // Regional, National, International

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRefereeStats Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> AssignedMatches;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor JerseyColor = FLinearColor::Black;

	FString GetFullName() const { return FirstName + " " + LastName; }
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFoulCalled, const FFoulEvent&, ECardType);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDecisionMade, const FFoulEvent&, ERefereeDecision);
DECLARE_MULTICAST_DELEGATE_One(FOnReplayRequested, const FFoulEvent&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerYellowCard, FString, int32);
DECLARE_MULTICAST_DELEGATE_One(FOnPlayerRedCard, FString);

class ARefereeCamera;

/**
 * Main Referee Mode Controller
 * Places player in first-person referee position during matches
 */
UCLASS()
class ULTIMATESPORTSGAME_API ARefereeMode : public APlayerController
{
	GENERATED_BODY()

public:
	ARefereeMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	// ============ INITIALIZATION ============
	
	/** Initialize referee mode for a specific match */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void InitializeRefereeMode(const FRefereeProfile& RefereeProfile);

	/** Load match data and setup referee for game */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void SetupMatch(const FTeamData& HomeTeam, const FTeamData& AwayTeam);

	// ============ GAMEPLAY CALLS ============

	/** Call a foul and select card type if needed */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void CallFoul(const FFoulEvent& FoulEvent);

	/** Make a decision (free kick, penalty, throw-in, etc.) */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void MakeDecision(ERefereeDecision Decision, const FVector& Location);

	/** Issue yellow card to player */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void IssueYellowCard(const FString& PlayerID, const FString& TeamID);

	/** Issue red card to player (automatic ejection) */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void IssueRedCard(const FString& PlayerID, const FString& TeamID);

	/** Allow player to contest the call (basketball/other sports) */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void AllowContestation(const FString& PlayerID, bool bAllow);

	/** Challenge/Review a call */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void RequestReplay(const FFoulEvent& FoulEvent);

	/** Confirm or overturn a call after replay review */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void ConfirmCallAfterReview(bool bConfirmOriginalCall, const FFoulEvent& FoulEvent);

	// ============ CAMERA & MOVEMENT ============

	/** Move referee to ideal viewing position */
	UFUNCTION(BlueprintCallable, Category = "Referee|Camera")
	void MoveToOptimalPosition(const FVector& ActionLocation);

	/** Rotate camera to follow play */
	UFUNCTION(BlueprintCallable, Category = "Referee|Camera")
	void RotateCameraToAction(const FVector& ActionLocation);

	/** Zoom in for close inspection */
	UFUNCTION(BlueprintCallable, Category = "Referee|Camera")
	void ZoomIn();

	/** Zoom out to see full field */
	UFUNCTION(BlueprintCallable, Category = "Referee|Camera")
	void ZoomOut();

	/** Reset camera to default position */
	UFUNCTION(BlueprintCallable, Category = "Referee|Camera")
	void ResetCamera();

	// ============ UI & HUD ============

	/** Update referee HUD with foul information */
	UFUNCTION(BlueprintCallable, Category = "Referee|UI")
	void UpdateHUD();

	/** Show player cards and stats */
	UFUNCTION(BlueprintCallable, Category = "Referee|UI")
	void ShowPlayerStats(const FString& PlayerID);

	/** Display foul history for current match */
	UFUNCTION(BlueprintCallable, Category = "Referee|UI")
	void ShowFoulHistory();

	/** Display team statistics and fouls */
	UFUNCTION(BlueprintCallable, Category = "Referee|UI")
	void ShowTeamStats(const FTeamData& Team);

	// ============ STATISTICS & PERFORMANCE ============

	/** Get referee's accuracy rating for this match */
	UFUNCTION(BlueprintCallable, Category = "Referee|Stats")
	float GetAccuracyRating() const { return CurrentRefereeProfile.Stats.AccuracyRating; }

	/** Get referee's consistency rating */
	UFUNCTION(BlueprintCallable, Category = "Referee|Stats")
	float GetConsistencyRating() const { return CurrentRefereeProfile.Stats.ConsistencyRating; }

	/** Get overall referee performance score */
	UFUNCTION(BlueprintCallable, Category = "Referee|Stats")
	float GetOverallPerformance() const;

	/** End referee mode and save statistics */
	UFUNCTION(BlueprintCallable, Category = "Referee")
	void EndRefereeMode();

	// ============ EVENTS ============

	UPROPERTY(BlueprintAssignable, Category = "Referee|Events")
	FOnFoulCalled OnFoulCalled;

	UPROPERTY(BlueprintAssignable, Category = "Referee|Events")
	FOnDecisionMade OnDecisionMade;

	UPROPERTY(BlueprintAssignable, Category = "Referee|Events")
	FOnReplayRequested OnReplayRequested;

	UPROPERTY(BlueprintAssignable, Category = "Referee|Events")
	FOnPlayerYellowCard OnPlayerYellowCard;

	UPROPERTY(BlueprintAssignable, Category = "Referee|Events")
	FOnPlayerRedCard OnPlayerRedCard;

	// ============ GETTERS ============

	UFUNCTION(BlueprintCallable, Category = "Referee")
	FRefereeProfile GetCurrentReferee() const { return CurrentRefereeProfile; }

	UFUNCTION(BlueprintCallable, Category = "Referee")
	TArray<FFoulEvent> GetFoulHistory() const { return FoulHistory; }

	UFUNCTION(BlueprintCallable, Category = "Referee")
	int32 GetYellowCardsIssued(const FString& TeamID) const;

	UFUNCTION(BlueprintCallable, Category = "Referee")
	int32 GetRedCardsIssued(const FString& TeamID) const;

protected:
	// Current match data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRefereeProfile CurrentRefereeProfile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTeamData HomeTeam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTeamData AwayTeam;

	// Match tracking
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FFoulEvent> FoulHistory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, int32> PlayerYellowCards; // PlayerID -> Count

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> EjectedPlayers; // Red-carded players

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ARefereeCamera* RefereeCamera;

	// Referee camera reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Referee")
	TSubclassOf<ARefereeCamera> RefereeCameraClass;

	// Calculate accuracy based on AI evaluation
	void CalculateAccuracy();

	// Check if call was correct (AI-driven)
	bool WasCallCorrect(const FFoulEvent& FoulEvent);

	// Input callbacks
	void OnCallFoulPressed();
	void OnMakeDecisionPressed();
	void OnRequestReplayPressed();
	void OnZoomInPressed();
	void OnZoomOutPressed();

	// Camera movement
	FVector OptimalViewPosition;
	float CurrentZoom = 1.0f;
	const float MaxZoom = 3.0f;
	const float MinZoom = 0.5f;
};
