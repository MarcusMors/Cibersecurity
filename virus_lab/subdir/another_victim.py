#!/usr/bin/env python3
import os
import sys

# === PAYLOAD ===
# This is the malicious part. In a real virus, this could be anything.
# For our educational purposes, it just prints a message.
def payload():
    print("!!! PAYLOAD TRIGGERED: File has been infected! !!!")

# === REPLICATION (INFECTION) LOGIC ===

print("I am in a subdirectory.")
