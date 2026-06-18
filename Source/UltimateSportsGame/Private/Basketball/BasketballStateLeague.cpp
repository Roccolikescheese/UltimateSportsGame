#include "Basketball/BasketballStateLeague.h"
#include "Engine/World.h"

ABasketballStateLeague::ABasketballStateLeague()
{
	PrimaryActorTick.bCanEverTick = true;

	StateName = TEXT("Default State");
	Division = EBasketballDivision::Division_1;
	Conference = EBasketballConference::Conference_North;
	MaxTeams = 16;
	RegularSeasonWeeks = 18;
	PlayoffTeams = 4;
	CurrentWeek = 0;
	bSeasonActive = false;
	bPlayoffsActive = false;
}

void ABasketballStateLeague::BeginPlay()
{
	Super::BeginPlay();
}

void ABasketballStateLeague::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasketballStateLeague::AddTeamToLeague(ABasketballTeam* InTeam)
{
	if (InTeam && Teams.Num() < MaxTeams)
	{
		Teams.Add(InTeam);
		UE_LOG(LogTemp, Warning, TEXT("Team %s added to %s %s"), 
			*InTeam->GetTeamName(), 
			*StateName, 
			*FString::FromInt((int32)Division));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot add team - league is full or team is invalid"));
	}
}

void ABasketballStateLeague::RemoveTeamFromLeague(ABasketballTeam* InTeam)
{
	Teams.Remove(InTeam);
	UE_LOG(LogTemp, Warning, TEXT("Team %s removed from league"), *InTeam->GetTeamName());
}

void ABasketballStateLeague::InitializeRegularSeason()
{
	if (Teams.Num() < 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough teams to start season"));
		return;
	}

	// Initialize standings
	Standings.Empty();
	for (ABasketballTeam* Team : Teams)
	{
		FBasketballLeagueStanding NewStanding;
		NewStanding.Team = Team;
		NewStanding.Wins = 0;
		NewStanding.Losses = 0;
		NewStanding.WinPercentage = 0.0f;
		NewStanding.PointsFor = 0;
		NewStanding.PointsAgainst = 0;
		NewStanding.Ranking = 0;
		NewStanding.bMadePlayoffs = false;

		Standings.Add(NewStanding);
	}

	CurrentWeek = 1;
	bSeasonActive = true;
	bPlayoffsActive = false;

	UE_LOG(LogTemp, Warning, TEXT("Regular season initialized for %s"), *StateName);
}

void ABasketballStateLeague::AdvanceWeek()
{
	if (!bSeasonActive || bPlayoffsActive)
	{
		return;
	}

	CurrentWeek++;

	if (CurrentWeek > RegularSeasonWeeks)
	{
		UE_LOG(LogTemp, Warning, TEXT("Regular season ended"));
		bSeasonActive = false;
		InitializePlayoffs();
	}

	UE_LOG(LogTemp, Warning, TEXT("Advanced to week %d"), CurrentWeek);
}

void ABasketballStateLeague::UpdateStandings()
{
	// Sort standings by wins/losses
	SortStandings();

	for (int32 i = 0; i < Standings.Num(); ++i)
	{
		int32 TotalGames = Standings[i].Wins + Standings[i].Losses;
		if (TotalGames > 0)
		{
			Standings[i].WinPercentage = (float)Standings[i].Wins / (float)TotalGames;
		}
		Standings[i].Ranking = i + 1;
	}
}

void ABasketballStateLeague::SortStandings()
{
	Standings.Sort([](const FBasketballLeagueStanding& A, const FBasketballLeagueStanding& B)
	{
		// Sort by wins first
		if (A.Wins != B.Wins)
		{
			return A.Wins > B.Wins;
		}

		// Then by win percentage
		if (A.WinPercentage != B.WinPercentage)
		{
			return A.WinPercentage > B.WinPercentage;
		}

		// Then by point differential
		int32 ADiff = A.PointsFor - A.PointsAgainst;
		int32 BDiff = B.PointsFor - B.PointsAgainst;
		return ADiff > BDiff;
	});
}

void ABasketballStateLeague::InitializePlayoffs()
{
	DeterminePlayoffTeams();
	bPlayoffsActive = true;

	UE_LOG(LogTemp, Warning, TEXT("Playoffs initialized for %s"), *StateName);
}

void ABasketballStateLeague::DeterminePlayoffTeams()
{
	UpdateStandings();

	// Clear playoff status
	for (FBasketballLeagueStanding& Standing : Standings)
	{
		Standing.bMadePlayoffs = false;
	}

	// Top teams make playoffs
	int32 PlayoffCount = FMath::Min(PlayoffTeams, Standings.Num());
	for (int32 i = 0; i < PlayoffCount; ++i)
	{
		Standings[i].bMadePlayoffs = true;
		UE_LOG(LogTemp, Warning, TEXT("Team %s made playoffs as seed %d"), 
			*Standings[i].Team->GetTeamName(), i + 1);
	}
}

TArray<FBasketballLeagueStanding> ABasketballStateLeague::GetCurrentStandings() const
{
	return Standings;
}

FBasketballLeagueStanding ABasketballStateLeague::GetTeamStanding(ABasketballTeam* InTeam) const
{
	for (const FBasketballLeagueStanding& Standing : Standings)
	{
		if (Standing.Team == InTeam)
		{
			return Standing;
		}
	}

	FBasketballLeagueStanding EmptyStanding;
	EmptyStanding.Team = nullptr;
	return EmptyStanding;
}

void ABasketballStateLeague::EndSeason()
{
	bSeasonActive = false;
	bPlayoffsActive = false;
	CurrentWeek = 0;

	UE_LOG(LogTemp, Warning, TEXT("Season ended for %s"), *StateName);
}

void ABasketballStateLeague::PrintStandings() const
{
	UE_LOG(LogTemp, Warning, TEXT("=== %s %s Standings ==="), 
		*StateName, 
		*FString::FromInt((int32)Division));

	for (const FBasketballLeagueStanding& Standing : Standings)
	{
		if (Standing.Team)
		{
			UE_LOG(LogTemp, Warning, 
				TEXT("%d. %s - %d-%d (%.3f) PF: %d PA: %d %s"),
				Standing.Ranking,
				*Standing.Team->GetTeamName(),
				Standing.Wins,
				Standing.Losses,
				Standing.WinPercentage,
				Standing.PointsFor,
				Standing.PointsAgainst,
				Standing.bMadePlayoffs ? TEXT("[PLAYOFFS]") : TEXT(""));
		}
	}
}
