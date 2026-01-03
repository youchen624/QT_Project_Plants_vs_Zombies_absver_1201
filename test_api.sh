#!/bin/bash
# Online Leaderboard API Test Script
# This script tests the mock server API endpoints

set -e

echo "╔═══════════════════════════════════════════════════════════╗"
echo "║  Online Leaderboard API Test Suite                       ║"
echo "╚═══════════════════════════════════════════════════════════╝"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Function to test API endpoint
test_endpoint() {
    local test_name="$1"
    local method="$2"
    local endpoint="$3"
    local data="$4"
    local expected_status="$5"
    
    echo -n "Testing: $test_name... "
    
    if [ "$method" = "POST" ]; then
        response=$(curl -s -w "\n%{http_code}" -X POST "$endpoint" \
            -H "Content-Type: application/json" \
            -d "$data")
    else
        response=$(curl -s -w "\n%{http_code}" "$endpoint")
    fi
    
    status_code=$(echo "$response" | tail -n1)
    body=$(echo "$response" | head -n-1)
    
    if [ "$status_code" = "$expected_status" ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
        echo "  Response: $body"
    else
        echo -e "${RED}✗ FAILED${NC}"
        echo "  Expected status: $expected_status, Got: $status_code"
        echo "  Response: $body"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Start test server
echo "Starting mock server..."
cd "$(dirname "$0")"
python3 mock_leaderboard_server.py > /tmp/pvz_server.log 2>&1 &
SERVER_PID=$!
echo "Server started with PID: $SERVER_PID"
sleep 2

# Verify server is running
if ! ps -p $SERVER_PID > /dev/null; then
    echo -e "${RED}Failed to start server. Check /tmp/pvz_server.log${NC}"
    exit 1
fi

echo "Server is running."
echo ""

# Test 1: Submit score - valid data
test_endpoint \
    "Submit valid score" \
    "POST" \
    "http://localhost:8080/api/submitScore" \
    '{
        "playerName": "TestPlayer1",
        "levelId": 1,
        "wavesSurvived": 3,
        "zombiesKilled": 50,
        "plantsPlaced": 20,
        "totalSunCollected": 2000,
        "playTimeSeconds": 300,
        "date": "2024-01-03T15:30:00",
        "isWin": true,
        "score": 2500
    }' \
    "200"

# Test 2: Submit another score
test_endpoint \
    "Submit second score" \
    "POST" \
    "http://localhost:8080/api/submitScore" \
    '{
        "playerName": "TestPlayer2",
        "levelId": 2,
        "wavesSurvived": 5,
        "zombiesKilled": 100,
        "plantsPlaced": 30,
        "totalSunCollected": 3500,
        "playTimeSeconds": 450,
        "date": "2024-01-03T16:00:00",
        "isWin": true,
        "score": 3500
    }' \
    "200"

# Test 3: Submit third score
test_endpoint \
    "Submit third score (highest)" \
    "POST" \
    "http://localhost:8080/api/submitScore" \
    '{
        "playerName": "TopPlayer",
        "levelId": 1,
        "wavesSurvived": 10,
        "zombiesKilled": 200,
        "plantsPlaced": 50,
        "totalSunCollected": 5000,
        "playTimeSeconds": 600,
        "date": "2024-01-03T17:00:00",
        "isWin": true,
        "score": 5000
    }' \
    "200"

# Test 4: Get all leaderboard
test_endpoint \
    "Get complete leaderboard" \
    "GET" \
    "http://localhost:8080/api/leaderboard" \
    "" \
    "200"

# Test 5: Get leaderboard by level
test_endpoint \
    "Get leaderboard for level 1" \
    "GET" \
    "http://localhost:8080/api/leaderboard?levelId=1" \
    "" \
    "200"

# Test 6: Submit invalid score (missing fields)
test_endpoint \
    "Submit invalid score (missing fields)" \
    "POST" \
    "http://localhost:8080/api/submitScore" \
    '{
        "playerName": "InvalidPlayer"
    }' \
    "400"

# Test 7: Invalid endpoint
test_endpoint \
    "Request invalid endpoint" \
    "GET" \
    "http://localhost:8080/api/invalid" \
    "" \
    "404"

# Clean up
echo "Stopping server (PID: $SERVER_PID)..."
kill $SERVER_PID
wait $SERVER_PID 2>/dev/null || true
echo ""

# Summary
echo "╔═══════════════════════════════════════════════════════════╗"
echo "║  Test Summary                                             ║"
echo "╠═══════════════════════════════════════════════════════════╣"
echo -e "║  ${GREEN}Passed: $TESTS_PASSED${NC}                                               ║"
echo -e "║  ${RED}Failed: $TESTS_FAILED${NC}                                               ║"
echo "╚═══════════════════════════════════════════════════════════╝"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed! ✓${NC}\n"
    exit 0
else
    echo -e "\n${RED}Some tests failed! ✗${NC}\n"
    exit 1
fi
