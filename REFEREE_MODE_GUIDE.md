# 🏀 Ultimate Sports Game - Referee Mode Documentation

## Complete Referee System Overview

This comprehensive referee mode system places players in the shoes of a professional referee across all 9 sports, with full support for **Season Mode** and **Tournament Mode** (including March Madness).

---

## 🎯 Core Systems

### 1. **RefereeMode.h** - Main Referee Controller
The central hub for all refereeing activities.

#### Key Features:
- ✅ **25+ Foul Types** across all 9 sports
- ✅ **Yellow/Red Card System** with suspension tracking
- ✅ **Replay Review System** (challenge decisions)
- ✅ **First-Person Camera** for immersive experience
- ✅ **Performance Tracking** (accuracy, consistency ratings)
- ✅ **Multi-Sport Support** (call different fouls per sport)

#### Main Functions:

```cpp
// Foul Management
CallFoul(FFoulEvent FoulEvent)              // Call a foul
IssueYellowCard(PlayerID, TeamID)          // 2 yellows = ejection
IssueRedCard(PlayerID, TeamID)             // Direct ejection

// Decisions
MakeDecision(ERefereeDecision Decision)     // Free kick, penalty, etc.
RequestReplay(FFoulEvent FoulEvent)         // Challenge a call
ConfirmCallAfterReview(bool bConfirm)       // After replay review

// Camera Control
MoveToOptimalPosition(ActionLocation)       // Auto-position ref
ZoomIn() / ZoomOut()                       // Inspect play closely
RotateCameraToAction(ActionLocation)       // Follow the action

// Statistics
GetAccuracyRating()                        // How many correct calls?
GetConsistencyRating()                     // Are penalties consistent?
GetOverallPerformance()                    // Combined score
```

#### Enum: EFoulType (25+ Types)
- **Soccer:** Handball, Offsides, Pushing, Tripping, Time Wasting, Diving
- **Basketball:** Personal Foul, Technical, Flagrant, Charging, Blocking
- **Baseball:** Ball/Strike, Safe/Out, Balk
- **Hockey:** Slashing, High Sticking, Hooking, Icing, Roughing
- **Rugby:** High Tackle, Late Hit, Collapsing Ruck
- **Cricket:** No-ball, Wide Delivery
- **Tennis:** Foot Fault, Court Violation
- **Bowling:** Foul Line Violation

---

### 2. **SeasonModeManager.h** - Full Season Play
Manage 82-game NBA season, 17-game NFL season, etc. with referee assignments.

#### Season Types Supported:
- 🏀 **NBA** (82 games)
- 🏈 **NFL** (17 games)
- ⚾ **MLB** (162 games)
- 🏒 **NHL** (82 games)
- ⚽ **MLS** (34 games)
- 🎓 **NCAA** (30-35 games)
- 🏏 **Cricket** (50+ matches)
- 🎾 **Tennis** (ATP/WTA Season)
- 🎳 **Bowling** (PBA Tour)
- 🏉 **Rugby** (26+ matches)

#### Key Features:
- ✅ **Full Season Scheduling** (no conflicts, realistic dates)
- ✅ **Referee Rotation** (prevents bias, balances workload)
- ✅ **Career Progression** (promotions based on performance)
- ✅ **Playoff Systems** (top seeds, wildcards, brackets)
- ✅ **Standing Tracking** (wins/losses, rankings)
- ✅ **Performance Rating System** (accuracy, consistency, overall)

#### Main Functions:

```cpp
// Season Management
CreateSeason(ESportSeason SportType, int32 Year)  // Start new season
StartSeason(FSeason Season)                       // Begin play
GenerateSeasonSchedule(FSeason& OutSeason)        // Create all games

// Referee Assignments
AssignRefereeToGame(Referee, Game)                // Single assignment
AssignRefereeRotation(int32 NumGames)             // Batch assignments
GetAvailableRefereesForDate(DateTime)             // Get available refs

// Game Progression
PlayGame(FGameSchedule Game)                      // Play specific game
CompleteGame(Game, HomeScore, AwayScore)         // Record result
SimulateRemainingGames()                          // Fast-forward

// Career Advancement
UpdateRefereeStats(RefereeID, PerformanceRating) // Track performance
PromoteRefereeToPlayoffs(RefereeID)              // Earn playoff spots
PromoteRefereeToChampionship(RefereeID)          // Select for finals

// Playoffs
StartPlayoffs()                                   // Begin playoff season
GeneratePlayoffBracket()                          // Create bracket
GetChampionshipGame()                             // Get championship game
```

#### Referee Career Path:
1. **Regular Season** (82 games) → Earn assignments based on performance
2. **Conference Tournaments** → Top-performing refs get spots
3. **Playoff Games** → More selective assignments
4. **Conference Finals** → Only the best
5. **Championship Game** → 1 ref chosen for the entire season

---

### 3. **TournamentRefereeManager.h** - Tournament Play
Specialized system for March Madness, Conference Tournaments, and Playoffs.

#### Tournament Types:
- 🏀 **March Madness** (68-team NCAA bracket)
- 🏀 **Conference Tournaments** (32 conferences)
- 🏈 **NFL Playoffs** (16 teams, 3-game series)
- ⚽ **World Cup** (32 teams, group stage + knockout)
- 🎾 **Wimbledon** (single elimination)
- ⚾ **World Series** (7-game series)
- 🏏 **Cricket World Cup** (10 teams)
- 🏉 **Rugby World Cup** (20 teams)
- 🎳 **PBA Championship** (6-game series)

#### March Madness Specific:
- ✅ **68-Team Bracket** (4 #1 seeds, 4 #2 seeds, etc.)
- ✅ **Play-In Games** (4 games: bubble teams)
- ✅ **First Four** (4 additional play-in games)
- ✅ **RPI Seeding** (realistic ranking system)
- ✅ **Automatic Bids** (32 conference tournament winners)
- ✅ **At-Large Bids** (36 best remaining teams)
- ✅ **Regional Brackets** (South, Midwest, East, West)

#### Tournament Rounds:
1. **Play-In/First Four** (4 games)
2. **Round of 64** (32 games)
3. **Round of 32** (16 games) = Sweet 16
4. **Elite Eight** (8 games)
5. **Final Four** (4 games)
6. **Championship** (1 game)

#### Key Functions:

```cpp
// Tournament Setup
StartTournament(ETournamentType Type)             // Initialize
SetupMarchMadness()                               // 68-team bracket
SetupConferenceTournament(ConferenceName)         // Conf. tourney
SetupPlayoffTournament(TournamentType)            // Pro playoffs

// Referee Assignment
AssignFirstRoundReferees()                        // 64/32/16 refs
AssignRoundReferees(ETournamentRound Round)       // Next round refs
PromoteRefereesToNextRound(CurrentRound)          // Advance top refs
AssignChampionshipReferee()                       // Select champion ref

// Bracket Access
GetMarchMadnessBracket()                          // Full 68-team bracket
GetSweet16Games()                                 // 16 teams left
GetEliteEightGames()                              // 8 teams left
GetFinalFourGames()                               // 4 teams left
GetChampionshipGame()                             // Final game

// Statistics & Leaderboard
GetTournamentRefereeStats(RefereeID)              // Individual stats
GetTournamentLeaderboard()                        // Top refs ranked
GetRefereeRanking(RefereeID)                      // Ref's rank
```

#### Tournament Referee Progression:
1. **Round of 64** → All ~32 refs assigned
2. **Sweet 16** → Top-performing 16 refs advance
3. **Elite Eight** → Best 8 refs continue
4. **Final Four** → Top 4 refs make semifinals
5. **Championship** → #1 ranked ref gets final game

---

### 4. **RefereeCameraController.h** - First-Person Camera
Immersive first-person camera system for referee positioning.

#### Camera Views:

```cpp
// Positioning
MoveToOptimalPosition(ActionLocation)     // AI positioning
MoveToSidelineView()                      // Side angle
MoveToEndZoneView()                       // End zone view
MoveToCenterFieldView()                   // Center view

// Following
FollowPlayer(AActor* Player)              // Track specific player
StopFollowingPlayer()                     // Return to free cam

// Zoom
ZoomIn()                                  // Inspect closely (3.0x)
ZoomOut()                                 // Full field (0.5x)
SetZoomLevel(float Zoom)                  // Custom zoom (0.5-3.0)

// Special Views
EnableImmersiveView()                     // First-person immersion
EnableTacticalView()                      // Overhead tactical
EnableBroadcastView()                     // TV broadcast angle
```

#### Referee Positioning Algorithm:
- ✅ Stays ~15 feet from action
- ✅ Maintains clear sight lines to ball
- ✅ Positions based on game situation
- ✅ Smooth camera transitions (prevents motion sickness)
- ✅ Constrained to valid field positions
- ✅ Respects off-field boundaries

---

## 📊 Referee Statistics

### Tracked Metrics:

```cpp
struct FRefereeStats {
    // Calls
    int32 TotalYellowCards;
    int32 TotalRedCards;
    int32 TotalFoulsCalledCorrectly;
    int32 TotalFoulsCalledIncorrectly;
    
    // Ratings (0-100)
    float AccuracyRating;          // Correct call %
    float ConsistencyRating;       // Penalty consistency
    
    // Career
    int32 GamesRefereed;
    int32 ContestationsAllowed;
    float OverallPerformance;
};
```

### Promotion/Demotion System:
- **90+ Rating**: Promoted to playoffs/championship
- **70-89 Rating**: Remains in regular season
- **Below 70 Rating**: Demoted or benched

---

## 🎮 Gameplay Flow

### Season Mode Referee Gameplay:

```
1. Season Start
   ↓
2. Referee Assignment (2-3 games/week)
   ↓
3. Play Game as Referee
   ├─ Make foul calls
   ├─ Issue cards
   ├─ Make decisions
   └─ Get performance rating
   ↓
4. Statistics Updated
   ├─ Accuracy rating
   ├─ Consistency rating
   └─ Overall performance
   ↓
5. Next Assignment (if promoted)
   ↓
6. Season Ends
   └─ Top refs promoted to playoffs
```

### Tournament Mode Referee Gameplay:

```
March Madness Setup
├─ Play-In Games (4 games)
│  └─ All ~32 refs assigned
│
├─ Round of 64 (32 games)
│  └─ All refs assigned
│
├─ Sweet 16 (16 games)
│  └─ Top 16 refs advance
│
├─ Elite Eight (8 games)
│  └─ Best 8 refs continue
│
├─ Final Four (4 games)
│  └─ Top 4 refs compete
│
└─ Championship (1 game)
   └─ #1 ranked ref selected
```

---

## 🔧 Implementation Example

```cpp
// Start a new NBA season
USeasonModeManager* SeasonMgr = GetGameInstance()->GetSubsystem<USeasonModeManager>();
FSeason NBASeason = SeasonMgr->CreateSeason(ESportSeason::NBA, 2024);
SeasonMgr->StartSeason(NBAseason);

// Assign referees for next week's games
SeasonMgr->AssignRefereeRotation(5);

// Play a game as referee
FGameSchedule NextGame = SeasonMgr->GetNextGame();
ARefereeMode* RefereeMode = GetWorld()->SpawnActor<ARefereeMode>();
RefereeMode->InitializeRefereeMode(RefereeProfile);
RefereeMode->SetupMatch(NextGame.HomeTeam, NextGame.AwayTeam);

// During game: Call foul
FFoulEvent Foul;
Foul.FoulType = EFoulType::Pushing;
Foul.CommittingPlayerID = "PLAYER_123";
RefereeMode->CallFoul(Foul);
RefereeMode->IssueYellowCard("PLAYER_123", "TEAM_A");

// Complete game
SeasonMgr->CompleteGame(NextGame, 85, 92, 87.5f); // 87.5 rating

// Check promotion to playoffs
SeasonMgr->CheckRefereePerformance(RefereeProfile.RefereeID);

// For March Madness
UTournamentRefereeManager* TourneyMgr = GetGameInstance()->GetSubsystem<UTournamentRefereeManager>();
TourneyMgr->StartTournament(ETournamentType::MarchMadness);
TourneyMgr->AssignFirstRoundReferees();

// Play tournament game
FTournamentGameAssignment TourneyGame = /* Get game */;
TourneyMgr->PlayTournamentGameAsReferee(TourneyGame);

// Promote to next round
TourneyMgr->PromoteRefereesToNextRound(ETournamentRound::RoundOf64);

// Championship assignment
FRefereeProfile ChampionshipRef;
TourneyMgr->AssignChampionshipReferee(ChampionshipRef);
```

---

## 📋 Files Created

```
Source/UltimateSportsGame/
├── Public/
│   ├── Referee/
│   │   ├── RefereeMode.h              ← Main ref controller
│   │   └── RefereeCameraController.h  ← Camera system
│   │
│   └── Season/
│       └── SeasonModeManager.h        ← Season management
│
│   └── Tournament/
│       └── TournamentRefereeManager.h ← Tournament referee system
│
└── Private/
    ├── Referee/
    │   ├── RefereeMode.cpp
    │   └── RefereeCameraController.cpp
    │
    ├── Season/
    │   └── SeasonModeManager.cpp
    │
    └── Tournament/
        └── TournamentRefereeManager.cpp
```

---

## 🚀 Next Steps

1. ✅ **Implement Core Referee Logic** (foul validation, card tracking)
2. ✅ **Connect to Match System** (receive game events)
3. ✅ **Build Referee UI/HUD** (foul history, card display)
4. ✅ **Add AI Evaluation** (accuracy calculation)
5. ✅ **Season Simulation** (simulate non-played games)
6. ✅ **Playoff Progression** (bracket advancement)
7. ✅ **Leaderboard System** (rank all referees)
8. ✅ **Save/Load System** (persist progress)

---

**This referee system provides a complete, immersive experience where players can build a referee career across season and tournament play!** 🎮🏀

