#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Data/PlayerData.h"
#include "Data/TeamData.h"
#include "SportsGameMode.generated.h"

class APlayerCharacter;
class AMatchController;
class ARefereeMode;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	PreMatch,
	Kickoff,
	Playing,
	HalfTime,
	SecondHalf,
	FullTime,
	Overtime,
	Paused
};

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	WaitingForKickoff,
	ActivePlay,
	Throw_In,
	Corner,
	GoalKick,
	FreeKick,
	Penalty,
	Replay,
	VideoReview
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int32, TeamA, int32, TeamB);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, EGameState, OldState, EGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseChanged, EGamePhase, OldPhase, EGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_Three(FOnMatchEvent, const FString&, EventType, const FString&, Description, APlayerCharacter*, Player);

USTRUCT(BlueprintType)
struct FMatchStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float Possession;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 Shots;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 ShotsOnTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 Passes;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 Tackles;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 Fouls;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	float PassAccuracy;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 Corners;

	UPROPERTY(BlueprintReadWrite, Category = "Stats")
	int32 OffsideCalls;
};

UCLASS()
class ULTIMATESPORTSGAME_API ASportsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ASportsGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Match Setup
	UFUNCTION(BlueprintCallable, Category = "Game|Match")
	void InitializeMatch(const FTeamData& TeamAData, const FTeamData& TeamBData, bool bUseRefereeMode = false);

	UFUNCTION(BlueprintCallable, Category = "Game|Match")
	void StartMatch();

	UFUNCTION(BlueprintCallable, Category = "Game|Match")
	void PauseMatch();

	UFUNCTION(BlueprintCallable, Category = "Game|Match")
	void ResumeMatch();

	UFUNCTION(BlueprintCallable, Category = "Game|Match")
	void EndMatch();

	// Game State Management
	UFUNCTION(BlueprintCallable, Category = "Game|State")
	void SetGameState(EGameState NewState);

	UFUNCTION(BlueprintCallable, Category = "Game|State")
	void SetGamePhase(EGamePhase NewPhase);

	UFUNCTION(BlueprintCallable, Category = "Game|State")
	EGameState GetGameState() const { return CurrentGameState; }

	UFUNCTION(BlueprintCallable, Category = "Game|State")
	EGamePhase GetGamePhase() const { return CurrentGamePhase; }

	// Scoring
	UFUNCTION(BlueprintCallable, Category = "Game|Score")
	void ScoreGoal(bool bTeamA, APlayerCharacter* Scorer, APlayerCharacter* Assister = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Game|Score")
	void GetScore(int32& OutTeamAScore, int32& OutTeamBScore) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Score")
	int32 GetTeamScore(bool bTeamA) const;

	// Match Time
	UFUNCTION(BlueprintCallable, Category = "Game|Time")
	float GetElapsedTime() const { return ElapsedTime; }

	UFUNCTION(BlueprintCallable, Category = "Game|Time")
	float GetRemainingTime() const;

	UFUNCTION(BlueprintCallable, Category = "Game|Time")
	int32 GetCurrentHalf() const { return CurrentHalf; }

	UFUNCTION(BlueprintCallable, Category = "Game|Time")
	float GetTimeUntilHalfTime() const;

	// Ball Possession
	UFUNCTION(BlueprintCallable, Category = "Game|Possession")
	void SetBallPossession(APlayerCharacter* PlayerWithBall);

	UFUNCTION(BlueprintCallable, Category = "Game|Possession")
	APlayerCharacter* GetPlayerWithBall() const { return PlayerPossessingBall; }

	UFUNCTION(BlueprintCallable, Category = "Game|Possession")
	bool GetTeamPossession(bool bTeamA) const;

	// Statistics
	UFUNCTION(BlueprintCallable, Category = "Game|Stats")
	FMatchStats GetTeamStats(bool bTeamA) const;

	UFUNCTION(BlueprintCallable, Category = "Game|Stats")
	void RecordShot(bool bTeamA, bool bOnTarget);

	UFUNCTION(BlueprintCallable, Category = "Game|Stats")
	void RecordPass(bool bTeamA, bool bSuccessful, float Distance);

	UFUNCTION(BlueprintCallable, Category = "Game|Stats")
	void RecordTackle(bool bTeamA);

	UFUNCTION(BlueprintCallable, Category = "Game|Stats")
	void RecordFoul(bool bTeamA);

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameStateChanged OnGameStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPhaseChanged OnPhaseChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMatchEvent OnMatchEvent;

	// Getters
	AMatchController* GetMatchController() const { return MatchController; }
	ARefereeMode* GetReferee() const { return Referee; }

protected:
	// Match Configuration
	UPROPERTY(BlueprintReadWrite, Category = "Match")
	FTeamData TeamAData;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	FTeamData TeamBData;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	int32 TeamAScore;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	int32 TeamBScore;

	// Match State
	UPROPERTY(BlueprintReadWrite, Category = "Match")
	EGameState CurrentGameState;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	EGamePhase CurrentGamePhase;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	float ElapsedTime;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	int32 CurrentHalf;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	bool bMatchPaused;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	bool bUsingRefereeMode;

	// Ball & Possession
	UPROPERTY(BlueprintReadWrite, Category = "Match")
	APlayerCharacter* PlayerPossessingBall;

	UPROPERTY(BlueprintReadWrite, Category = "Match")
	bool bTeamAPossession;

	// Statistics
	FMatchStats TeamAStats;
	FMatchStats TeamBStats;

	// Controllers
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	AMatchController* MatchController;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	ARefereeMode* Referee;

	// Game Configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	float MatchDuration = 2700.0f; // 45 minutes per half

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game")
	float HalfTimeDuration = 300.0f; // 5 minutes

private:
	void UpdateMatchTime(float DeltaTime);
	void CheckHalfTime();
	void CheckFullTime();
	void HandleHalfTime();
	void HandleFullTime();
	void UpdateStats();
	void BroadcastMatchEvent(const FString& EventType, const FString& Description, APlayerCharacter* Player = nullptr);
};
