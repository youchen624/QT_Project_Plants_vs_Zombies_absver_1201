# Leaderboard Feature Test Plan

## Manual Testing Checklist

### 1. Main Menu Integration
- [x] Launch the game
- [ ] Verify "排行榜" (Leaderboard) button appears on main menu
- [ ] Click the button to open leaderboard dialog
- [ ] Verify leaderboard dialog opens with proper styling

### 2. Empty Leaderboard
- [ ] On first launch, leaderboard should be empty
- [ ] Verify "No scores yet" or empty table displays properly
- [ ] Test filter dropdown (should show all level options)

### 3. Score Recording - Win Scenario
- [ ] Start a game and complete a level successfully
- [ ] Verify player name dialog appears
- [ ] Enter a player name (e.g., "TestPlayer1")
- [ ] Click "確定" (OK) button
- [ ] Verify game returns to main menu

### 4. Score Recording - Lose Scenario
- [ ] Start a game and let zombies reach the house
- [ ] Verify player name dialog appears
- [ ] Enter a different name (e.g., "TestPlayer2")
- [ ] Click "確定" (OK) button
- [ ] Verify game returns to main menu

### 5. View Leaderboard
- [ ] Open leaderboard from main menu
- [ ] Verify both scores appear in the table
- [ ] Check all columns display correct data:
  - Rank (排名)
  - Player name (玩家)
  - Level (關卡)
  - Result (結果) - should show ✔ for win, ✖ for loss
  - Waves (波數)
  - Zombies killed (殭屍擊殺)
  - Plants placed (植物種植)
  - Sun collected (陽光收集)
  - Play time (遊玩時間)

### 6. Score Calculation Verification
- [ ] Win score should be higher than loss score
- [ ] First place should have gold background
- [ ] Second place should have silver background
- [ ] Third place should have bronze background

### 7. Filtering by Level
- [ ] Select "全部關卡" (All levels) - should show all scores
- [ ] Select "關卡 1" (Level 1) - should show only level 1 scores
- [ ] Select other levels - should filter accordingly

### 8. Refresh Functionality
- [ ] Play another game and submit score
- [ ] Click "🔄 重新整理" (Refresh) button
- [ ] Verify new score appears in leaderboard

### 9. Data Persistence
- [ ] Close the game completely
- [ ] Relaunch the game
- [ ] Open leaderboard
- [ ] Verify all previous scores are still there

### 10. Skip Score Submission
- [ ] Complete a game
- [ ] When player name dialog appears, click "跳過" (Skip)
- [ ] Verify score is NOT added to leaderboard

### 11. Edge Cases
- [ ] Test with empty player name (should default to "Anonymous")
- [ ] Test with very long player name (should be limited to 20 chars)
- [ ] Play multiple games quickly (scores should accumulate)
- [ ] Test endless mode (levelId = 999)

## Statistics Tracking Verification

During gameplay, verify these statistics are tracked:
- [ ] m_zombiesKilled increments when zombies are destroyed
- [ ] m_plantsPlaced increments when plants are placed
- [ ] m_totalSunCollected increments when sun is collected
- [ ] m_gameStartTime is set when game starts
- [ ] Play time is calculated correctly on game end

## UI/UX Testing

### Leaderboard Dialog
- [ ] Dialog has appropriate size (900x600)
- [ ] Background gradient displays properly
- [ ] Table is readable with good contrast
- [ ] Buttons are styled consistently with main menu
- [ ] Dialog can be closed with "✖ 關閉" button

### Player Name Dialog
- [ ] Dialog has appropriate size (400x200)
- [ ] Text input is focused on open
- [ ] Enter key submits the form
- [ ] Placeholder text is visible
- [ ] Buttons work properly

## Performance Testing
- [ ] Game performance unchanged with leaderboard feature
- [ ] Leaderboard loads quickly (<1 second)
- [ ] No lag when submitting scores
- [ ] JSON file size stays reasonable with 100+ entries

## Code Quality Checks
- [x] All new files added to Qt_pvz.pro
- [x] No memory leaks (proper use of Qt parent-child relationship)
- [x] Singleton pattern correctly implemented
- [x] JSON serialization/deserialization works correctly
- [x] Error handling for file operations

## Expected Results

### Score Calculation Example
For a winning game with:
- 3 waves survived
- 25 zombies killed
- 15 plants placed
- 2000 sun collected
- 8 minutes play time

Score = (3 × 100) + (25 × 10) + (15 × 5) + (2000 ÷ 10) + 1000 = 1825

### File Location
The leaderboard.json file should be created at:
- Windows: `%APPDATA%/QT_Project_Plants_vs_Zombies/leaderboard.json`
- Linux: `~/.local/share/QT_Project_Plants_vs_Zombies/leaderboard.json`
- macOS: `~/Library/Application Support/QT_Project_Plants_vs_Zombies/leaderboard.json`

## Known Limitations
- Maximum 100 scores stored (older scores are removed)
- Local storage only (no cloud sync)
- No score validation (players could manipulate local file)
