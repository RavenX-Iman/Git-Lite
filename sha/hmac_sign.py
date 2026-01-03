"""
HMAC-SHA256 Signature Generator for Git-Lite
Accepts key path and commit data as arguments
"""

import hmac
import hashlib
import sys
import os

def load_key(key_path):
    """Load HMAC key from file"""
    if not os.path.exists(key_path):
        print(f"ERROR: Key file not found: {key_path}", file=sys.stderr)
        sys.exit(1)
    
    with open(key_path, 'rb') as f:
        return f.read()

def sign_commit(commit_data, key):
    """Generate HMAC-SHA256 signature"""
    if isinstance(commit_data, str):
        commit_data = commit_data.encode('utf-8')
    
    h = hmac.new(key, commit_data, hashlib.sha256)
    return h.hexdigest()

if __name__ == "__main__":
    # Accept key path as first arg, commit data from stdin
    if len(sys.argv) < 2:
        print("Usage: python hmac_sign.py <key_path>", file=sys.stderr)
        print("Commit data should be provided via stdin", file=sys.stderr)
        sys.exit(1)
    
    key_path = sys.argv[1]
    
    try:
        # Load secret key
        key = load_key(key_path)
        
        # Read commit data from stdin
        commit_data = sys.stdin.read()
        
        if not commit_data:
            print("ERROR: No commit data received from stdin", file=sys.stderr)
            sys.exit(1)
        
        # Generate signature
        signature = sign_commit(commit_data, key)
        
        # Output ONLY the signature (no extra text)
        print(signature)
        sys.exit(0)
        
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)