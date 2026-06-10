#!/bin/bash

# Function to display and execute commands
run_cmd() {
    echo "\$ $*"
    eval "$@"
    # sleep 2;
}

# chmod +x task_6.sh
# gcc cert_verifier.c -o cert_verifier -lssl -lcrypto

# Task 6: Manually Verify X.509 Certificate
# Usage: ./task_6.sh <website>
# Example: ./task_6.sh www.example.org

# Websites
# www.google.com
# www.amazon.com
# www.microsoft.com
# www.cloudflare.com
# www.github.com

run_cmd "WEBSITE=\${1:-www.amazon.com}"

echo "=== Task 6: Manually Verifying X.509 Certificate ==="
echo "Website: $WEBSITE"

# Step 1: Download certificate chain
echo -e "\n[Step 1] Downloading certificate chain..."
run_cmd "openssl s_client -connect $WEBSITE:443 -showcerts </dev/null 2>/dev/null | awk '/BEGIN CERTIFICATE/,/END CERTIFICATE/ {print}' > cert_chain.pem"

# Extract server cert (first) and issuer cert (second)
# issuer cert = the Certificate Authority's certificate that contains the public key needed to verify the signature on the server cert (first)
# c0.pem = server cert
# c1.pem = issuer cert
# signature_from_c0^e_from_c1 mod n_from_c1 == hash_of_c0_body
run_cmd "awk '/BEGIN CERTIFICATE/ {n++} n==1 {print > \"c0.pem\"} n==2 {print > \"c1.pem\"}' cert_chain.pem"

if [ ! -s c0.pem ] || [ ! -s c1.pem ]; then
    run_cmd "echo \"✗ Error: Could not extract certificates\"; exit 1"
fi

# Step 2: Extract issuer's public key
echo -e "\n[Step 2] Extracting issuer's public key..."
# Get modulus n
run_cmd "openssl x509 -in c1.pem -noout -modulus | sed 's/Modulus=//' > n.hex"
# Get exponent e and convert to hex
run_cmd "openssl x509 -in c1.pem -text -noout | grep -A1 \"Exponent:\" | tail -1 | xargs | cut -d' ' -f1 > e.dec"

run_cmd "printf \"%X\" \$(cat e.dec) > e.hex"

# Step 3: Extract signature from server certificate
echo -e "\n[Step 3] Extracting signature..."
run_cmd "openssl x509 -in c0.pem -text -noout | sed -n '/Signature Algorithm:/,\$p' | grep -v \"Signature Algorithm:\" | tr -d ' \t\n\r:' > signature.hex"

# Step 4: Extract certificate body and compute hash
echo -e "\n[Step 4] Extracting certificate body..."
run_cmd "openssl asn1parse -i -in c0.pem -strparse 4 -out c0_body.bin -noout 2>/dev/null"
echo "Computing SHA256 hash of certificate body..."
run_cmd "sha256sum c0_body.bin | awk '{print \$1}' > hash.hex"

# Step 5: Verify signature
echo -e "\n[Step 5] Running RSA verification..."
# Compile verifier if needed
if [ ! -f cert_verifier ]; then
    echo "Compiling cert_verifier..."
    run_cmd "gcc cert_verifier.c -o cert_verifier -lssl -lcrypto"
fi

run_cmd "./cert_verifier n.hex e.hex signature.hex hash.hex"

# Cleanup
run_cmd "rm -f cert_chain.pem c0.pem c1.pem n.hex e.dec e.hex signature.hex c0_body.bin hash.hex"

echo -e "\n=== Verification Complete ==="