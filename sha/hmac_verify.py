"""
HMAC-SHA256 Signature Verifier for Git-Lite
Accepts key path, commit data from stdin, and signature as argument
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

def verify_commit(commit_data, stored_signature, key):
    """Verify HMAC signature using constant-time comparison"""
    if isinstance(commit_data, str):
        commit_data = commit_data.encode('utf-8')
    
    h = hmac.new(key, commit_data, hashlib.sha256)
    computed_signature = h.hexdigest()
    
    # Constant-time comparison
    return hmac.compare_digest(computed_signature, stored_signature)

if __name__ == "__main__":
    # Accept: key_path (arg1), commit_data (stdin), signature (arg2)
    if len(sys.argv) != 3:
        print("Usage: python hmac_verify.py <key_path> <signature>", file=sys.stderr)
        print("Commit data should be provided via stdin", file=sys.stderr)
        sys.exit(1)
    
    key_path = sys.argv[1]
    stored_signature = sys.argv[2]
    
    try:
        # Load secret key
        key = load_key(key_path)
        
        # Read commit data from stdin
        commit_data = sys.stdin.read()
        
        if not commit_data:
            print("ERROR: No commit data received from stdin", file=sys.stderr)
            sys.exit(1)
        
        # Verify signature
        if verify_commit(commit_data, stored_signature, key):
            print("VALID")
            sys.exit(0)
        else:
            print("INVALID")
            sys.exit(1)
            
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(2)