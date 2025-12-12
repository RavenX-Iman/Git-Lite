#   Code work in main class
#  import hashlib

# def compute_sha256(data: str) -> str:
#     """
#     Computes SHA-256 hash of a string.
#     Returns hexadecimal hash.
#     """
#     return hashlib.sha256(data.encode()).hexdigest()


# # Optional: test if run directly
# if __name__ == "__main__":
#     text = "Hello Git-Lite"
#     print("SHA-256:", compute_sha256(text))


# To work on vcs_commit directly
import hashlib
import sys

def compute_sha256(data: str) -> str:
    """Compute SHA-256 hash of the given string."""
    return hashlib.sha256(data.encode()).hexdigest()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        text = sys.argv[1]
        print(compute_sha256(text))
    else:
        print("No input provided")
