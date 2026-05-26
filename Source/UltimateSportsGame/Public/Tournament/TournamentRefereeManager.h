#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/TeamData.h"
#include "Tournament/MarchMadness.h"
#include "TournamentRefereeManager.generated.h"

UENUM(BlueprintType)
enum class ETournamentType : uint8
{
	MarchMadness UMETA(DisplayName = "March Madness (NCAA)"),
	ConferenceTournament UMETA(DisplayName = "Conference Tournament"),
	NFLPlayoffs UMETA(DisplayName = "NFL Playoffs"),
	NBCPlayoffs UMETA(DisplayName = "NBA Playoffs"),
	MLBPlayoffs UMETA(DisplayName = "MLB Playoffs"),
	NHLPlayoffs UMETA(DisplayName = "NHL Playoffs"),
	WorldCup UMETA(DisplayName = "World Cup"),
	Wimbledon UMETA(DisplayName = "Wimbledon"),
	CricketWorldCup UMETA(DisplayName = "Cricket World Cup"),
	RugbyWorldCup UMETA(DisplayName = "Rugby World Cup")
};

UENUM(BlueprintType)
enum class ETournamentRound : uint8
{
	PlayIn UMETA(DisplayName = "Play-In"),
	FirstFour UMETA(DisplayName = "First Four"),
	RoundOf64 UMETA(DisplayName = "Round of 64"),
	RoundOf32 UMETA(DisplayName = "Round of 32"),
	SweetSixteen UMETA(DisplayName = "Sweet 16"),
	EliteEight UMETA(DisplayName = "Elite Eight"),
	FinalFour UMETA(DisplayName = "Final Four"),
	Championship UMETA(DisplayName = "Championship")
};

USTRUCT(BlueprintType)
struct FTournamentGameAssignment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GameID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData HomeTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTeamData AwayTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RefereeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETournamentRound Round = ETournamentRound::RoundOf64;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HomeTeamScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AwayTeamScore = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RefereePerformanceRating = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCompleted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime GameDateTime;
};

USTRUCT(BlueprintType)
struct FTournamentBracket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETournamentType TournamentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Year = 2024;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> AllGames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> PlayInGames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> FirstFourGames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> RoundOf64Games;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> SweetSixteenGames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> EliteEightGames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTournamentGameAssignment> FinalFourGames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTournamentGameAssignment ChampionshipGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETournamentRound CurrentRound = ETournamentRound::PlayIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalTeams = 68;
};

DECLARE_MULTICAST_DELEGATE_Two(FOnTournamentStarted, ETournamentType, int32); // Type, Year
DECLARE_MULTICAST_DELEGATE_Three(FOnTournamentGameAssigned, const FTournamentGameAssignment&, const FString&, ETournamentRound); // Game, RefereeID, Round
DECLARE_MULTICAST_DELEGATE_Two(FOnTournamentGameCompleted, const FTournamentGameAssignment&, ETournamentRound); // Game, Round
DECLARE_MULTICAST_DELEGATE_Two(FOnRefereesPromotedRound, const TArray<FString>&, ETournamentRound); // Referee IDs, Next Round
DECLARE_MULTICAST_DELEGATE_One(FOnChampionshipRefereeSelected, const FString&); // RefereeID

/**
 * Tournament Referee Manager - Handles March Madness and other tournament referee assignments
 */
UCLASS()
class ULTIMATESPORTSGAME_API UTournamentRefereeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// ============ TOURNAMENT SETUP ============

	/** Start a new tournament */
	UFUNCTION(BlueprintCallable, Category = "Tournament")
	void StartTournament(ETournamentType TournamentType);

	/** Setup March Madness 68-team bracket */
	UFUNCTION(BlueprintCallable, Category = "Tournament|MarchMadness")
	void SetupMarchMadness();

	/** Setup conference tournament */
	UFUNCTION(BlueprintCallable, Category = "Tournament")
	void SetupConferenceTournament(const FString& ConferenceName);

	/** Setup professional playoffs */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Playoffs")
	void SetupPlayoffTournament(ETournamentType TournamentType);

	/** Get current tournament */
	UFUNCTION(BlueprintCallable, Category = "Tournament")
	FTournamentBracket GetCurrentTournament() const { return CurrentBracket; }

	/** Get current tournament round */
	UFUNCTION(BlueprintCallable, Category = "Tournament")
	ETournamentRound GetCurrentRound() const { return CurrentBracket.CurrentRound; }

	// ============ REFEREE ASSIGNMENT ============

	/** Assign referees for first round */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Referees")
	void AssignFirstRoundReferees();

	/** Assign referees for specific round */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Referees")
	void AssignRoundReferees(ETournamentRound Round);

	/** Assign referee to specific game */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Referees")
	void AssignRefereeToGame(const FString& RefereeID, FTournamentGameAssignment& OutGame);

	/** Get referees assigned to a round */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Referees")
	TArray<FString> GetRoundReferees(ETournamentRound Round) const;

	/** Get referee's tournament games */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Referees")
	TArray<FTournamentGameAssignment> GetRefereeGames(const FString& RefereeID) const;

	// ============ TOURNAMENT PROGRESSION ============

	/** Promote top referees to next round */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Progression")
	void PromoteRefereesToNextRound(ETournamentRound CurrentRound);

	/** Complete tournament game */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Games")
	void CompleteTournamentGame(FTournamentGameAssignment& Game, int32 HomeScore, int32 AwayScore, float RefereeRating);

	/** Update game result and advance winner */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Games")
	void UpdateGameResult(FTournamentGameAssignment& Game, const FTeamData& WinnerTeam, int32 WinnerScore, int32 LoserScore);

	/** Advance to next tournament round */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Progression")
	void AdvanceToNextRound();

	// ============ GAME ACCESS ============

	/** Get all games for a round */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Games")
	TArray<FTournamentGameAssignment> GetRoundGames(ETournamentRound Round) const;

	/** Get March Madness bracket */
	UFUNCTION(BlueprintCallable, Category = "Tournament|MarchMadness")
	FTournamentBracket GetMarchMadnessBracket() const { return CurrentBracket; }

	/** Get Sweet 16 games */
	UFUNCTION(BlueprintCallable, Category = "Tournament|MarchMadness")
	TArray<FTournamentGameAssignment> GetSweet16Games() const { return CurrentBracket.SweetSixteenGames; }

	/** Get Elite Eight games */
	UFUNCTION(BlueprintCallable, Category = "Tournament|MarchMadness")
	TArray<FTournamentGameAssignment> GetEliteEightGames() const { return CurrentBracket.EliteEightGames; }

	/** Get Final Four games */
	UFUNCTION(BlueprintCallable, Category = "Tournament|MarchMadness")
	TArray<FTournamentGameAssignment> GetFinalFourGames() const { return CurrentBracket.FinalFourGames; }

	/** Get championship game */
	UFUNCTION(BlueprintCallable, Category = "Tournament|MarchMadness")
	FTournamentGameAssignment GetChampionshipGame() const { return CurrentBracket.ChampionshipGame; }

	// ============ CHAMPIONSHIP ============

	/** Select referee for championship game */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Championship")
	void AssignChampionshipReferee();

	/** Get championship referee */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Championship")
	FString GetChampionshipRefereeID() const { return ChampionshipRefereeID; }

	// ============ STATISTICS ============

	/** Get referee tournament stats */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Stats")
	float GetRefereeTournamentRating(const FString& RefereeID) const;

	/** Get tournament leaderboard */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Stats")
	TMap<FString, float> GetTournamentLeaderboard() const;

	/** Get referee's ranking */
	UFUNCTION(BlueprintCallable, Category = "Tournament|Stats")
	int32 GetRefereeRanking(const FString& RefereeID) const;

	// ============ TOURNAMENT END ============

	/** End tournament */
	UFUNCTION(BlueprintCallable, Category = "Tournament")
	void EndTournament();

	/** Save tournament results */
	UFUNCTION(BlueprintCallable, Category = "Tournament")
	void SaveTournamentResults();

	// ============ EVENTS ============

	UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
	FOnTournamentStarted OnTournamentStarted;

	UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
	FOnTournamentGameAssigned OnTournamentGameAssigned;

	UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
	FOnTournamentGameCompleted OnTournamentGameCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
	FOnRefereesPromotedRound OnRefereesPromotedRound;

	UPROPERTY(BlueprintAssignable, Category = "Tournament|Events")
	FOnChampionshipRefereeSelected OnChampionshipRefereeSelected;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTournamentBracket CurrentBracket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, float> RefereePerformanceRatings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> AdvancedReferees;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString ChampionshipRefereeID;

	// March Madness setup
	void CreateMarchMadnessBracket();
	void AssignPlayInReferees();
	void AssignPlayInGameReferees();

	// Referee promotion logic
	TArray<FString> SelectTopPerformingReferees(int32 Count);
	void PromoteTopReferees(ETournamentRound CurrentRound);

	// Bracket management
	void SortTeamsBySeeding();
	void GenerateMatchups();
};
