#!/usr/bin/env python3
"""
Mock HTTP Server for Testing Online Leaderboard
This server provides test endpoints for the Plants vs Zombies leaderboard feature.

Usage:
    python3 mock_leaderboard_server.py

Endpoints:
    POST /api/submitScore - Accept score submissions
    GET  /api/leaderboard?levelId=N - Return leaderboard data
"""

from http.server import HTTPServer, BaseHTTPRequestHandler
import json
from datetime import datetime
import threading
import time

# In-memory leaderboard storage
leaderboard_data = []

class LeaderboardHandler(BaseHTTPRequestHandler):
    def _set_headers(self, status_code=200):
        self.send_response(status_code)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
    
    def do_OPTIONS(self):
        self._set_headers()
    
    def do_POST(self):
        if self.path == '/api/submitScore':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            
            try:
                score_data = json.loads(post_data.decode('utf-8'))
                
                # Validate required fields
                required_fields = ['playerName', 'levelId', 'wavesSurvived', 'zombiesKilled']
                if all(field in score_data for field in required_fields):
                    # Add server timestamp
                    score_data['serverTimestamp'] = datetime.now().isoformat()
                    
                    # Add to leaderboard
                    leaderboard_data.append(score_data)
                    
                    # Sort by score (descending)
                    leaderboard_data.sort(key=lambda x: x.get('score', 0), reverse=True)
                    
                    # Keep only top 100
                    if len(leaderboard_data) > 100:
                        leaderboard_data[:] = leaderboard_data[:100]
                    
                    self._set_headers(200)
                    response = {
                        'success': True,
                        'message': 'Score submitted successfully',
                        'rank': leaderboard_data.index(score_data) + 1,
                        'totalScores': len(leaderboard_data)
                    }
                    self.wfile.write(json.dumps(response).encode('utf-8'))
                    print(f"✅ Score submitted: {score_data.get('playerName')} - {score_data.get('score')} points")
                else:
                    self._set_headers(400)
                    response = {'success': False, 'message': 'Missing required fields'}
                    self.wfile.write(json.dumps(response).encode('utf-8'))
                    print(f"❌ Invalid score data: missing fields")
            
            except json.JSONDecodeError:
                self._set_headers(400)
                response = {'success': False, 'message': 'Invalid JSON'}
                self.wfile.write(json.dumps(response).encode('utf-8'))
                print(f"❌ Invalid JSON received")
        else:
            self._set_headers(404)
            self.wfile.write(json.dumps({'error': 'Not found'}).encode('utf-8'))
    
    def do_GET(self):
        if self.path.startswith('/api/leaderboard'):
            # Parse query parameters
            level_id = None
            if '?' in self.path:
                query = self.path.split('?')[1]
                params = dict(param.split('=') for param in query.split('&') if '=' in param)
                level_id = int(params.get('levelId', 0))
            
            # Filter by level if specified
            filtered_scores = leaderboard_data
            if level_id and level_id > 0:
                filtered_scores = [s for s in leaderboard_data if s.get('levelId') == level_id]
            
            self._set_headers(200)
            response = {
                'version': '1.0',
                'scores': filtered_scores[:50],  # Return top 50
                'totalCount': len(filtered_scores),
                'levelId': level_id if level_id else 'all'
            }
            self.wfile.write(json.dumps(response).encode('utf-8'))
            print(f"📋 Leaderboard requested: level {level_id if level_id else 'all'} ({len(filtered_scores)} scores)")
        else:
            self._set_headers(404)
            self.wfile.write(json.dumps({'error': 'Not found'}).encode('utf-8'))
    
    def log_message(self, format, *args):
        # Suppress default logging to avoid clutter
        pass

def run_server(port=8080):
    server_address = ('', port)
    httpd = HTTPServer(server_address, LeaderboardHandler)
    print(f"""
╔══════════════════════════════════════════════════════════════╗
║  Mock Leaderboard Server for Plants vs Zombies              ║
╠══════════════════════════════════════════════════════════════╣
║  Server running on http://localhost:{port}/api                 ║
║                                                              ║
║  Endpoints:                                                  ║
║    POST /api/submitScore  - Submit a score                  ║
║    GET  /api/leaderboard  - Get all scores                  ║
║    GET  /api/leaderboard?levelId=N - Get scores for level  ║
║                                                              ║
║  Press Ctrl+C to stop the server                            ║
╚══════════════════════════════════════════════════════════════╝
    """)
    
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n\n🛑 Server stopped by user")
        httpd.shutdown()

if __name__ == '__main__':
    run_server()
