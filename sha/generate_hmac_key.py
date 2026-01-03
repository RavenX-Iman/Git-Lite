"""
HMAC Key Generator for Git-Lite
Generates 256-bit random key for commit signing
"""

import os
import sys

def generate_key(key_path):
    """Generate 32-byte (256-bit) random key"""
    try:
        # Generate cryptographically secure random bytes
        key = os.urandom(32)
        
        # Ensure directory exists
        key_dir = os.path.dirname(key_path)
        if key_dir and not os.path.exists(key_dir):
            os.makedirs(key_dir)
        
        # Write key in binary mode
        with open(key_path, 'wb') as f:
            f.write(key)
        
        print(f"HMAC key generated: {key_path}")
        
    except Exception as e:
        print(f"ERROR: Failed to generate key: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python generate_hmac_key.py <key_path>")
        sys.exit(1)
    
    key_path = sys.argv[1]
    generate_key(key_path)