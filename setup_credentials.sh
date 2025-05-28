#!/bin/bash

set -e

# Script to help set up AWS credentials for the AWS SDK for C++

# Default values
PROFILE="default"
REGION="us-west-2"
OUTPUT_FORMAT="json"
FORCE=0

# Function to show usage
show_usage() {
    echo "AWS Credentials Setup Helper"
    echo "==========================="
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  --profile NAME      AWS profile name (default: default)"
    echo "  --region REGION     AWS region (default: us-west-2)"
    echo "  --output FORMAT     Output format (json, text, table; default: json)"
    echo "  --force             Overwrite existing credentials without prompting"
    echo "  --help              Show this help message"
    echo ""
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --profile)
      PROFILE="$2"
      shift
      shift
      ;;
    --region)
      REGION="$2"
      shift
      shift
      ;;
    --output)
      OUTPUT_FORMAT="$2"
      shift
      shift
      ;;
    --force)
      FORCE=1
      shift
      ;;
    --help)
      show_usage
      exit 0
      ;;
    *)
      echo "Error: Unknown option: $1"
      echo "Use --help for usage information"
      exit 1
      ;;
  esac
done

echo "AWS Credentials Setup Helper"
echo "==========================="
echo "Profile: $PROFILE"
echo "Region: $REGION"
echo "Output format: $OUTPUT_FORMAT"
echo ""

# Check if ~/.aws directory exists
if [ ! -d "$HOME/.aws" ]; then
    echo "Creating ~/.aws directory..."
    mkdir -p "$HOME/.aws"
fi

# Check for existing credentials
if [ -f "$HOME/.aws/credentials" ] && [ -f "$HOME/.aws/config" ]; then
    if grep -q "\[$PROFILE\]" "$HOME/.aws/credentials" && [ $FORCE -ne 1 ]; then
        echo "AWS credentials for profile '$PROFILE' already exist."
        read -p "Do you want to overwrite them? (y/n): " overwrite
        if [ "$overwrite" != "y" ]; then
            echo "Keeping existing credentials file."
            exit 0
        fi
    fi
fi

# Function to validate AWS access key format
validate_access_key() {
    local key=$1
    if [[ ! $key =~ ^[A-Z0-9]{20}$ ]]; then
        echo "Warning: AWS Access Key ID should be 20 characters long and contain only uppercase letters and numbers."
        read -p "Continue anyway? (y/n): " continue_anyway
        if [ "$continue_anyway" != "y" ]; then
            return 1
        fi
    fi
    return 0
}

# Function to validate AWS secret key format
validate_secret_key() {
    local key=$1
    if [[ ! $key =~ ^[A-Za-z0-9/+=]{40}$ ]]; then
        echo "Warning: AWS Secret Access Key should be 40 characters long."
        read -p "Continue anyway? (y/n): " continue_anyway
        if [ "$continue_anyway" != "y" ]; then
            return 1
        fi
    fi
    return 0
}

# Function to validate AWS region format
validate_region() {
    local region=$1
    if [[ ! $region =~ ^[a-z]+-[a-z]+-[0-9]+$ ]]; then
        echo "Warning: AWS Region should be in format like 'us-west-2'."
        read -p "Continue anyway? (y/n): " continue_anyway
        if [ "$continue_anyway" != "y" ]; then
            return 1
        fi
    fi
    return 0
}

# Function to validate output format
validate_output_format() {
    local format=$1
    if [[ ! "$format" =~ ^(json|text|table)$ ]]; then
        echo "Error: Output format must be one of: json, text, table"
        return 1
    fi
    return 0
}

# Validate output format
if ! validate_output_format "$OUTPUT_FORMAT"; then
    exit 1
fi

# Get and validate user input
echo "Please enter your AWS credentials:"
while true; do
    read -p "AWS Access Key ID: " AWS_ACCESS_KEY
    if [ -z "$AWS_ACCESS_KEY" ]; then
        echo "Error: AWS Access Key ID cannot be empty"
        continue
    fi
    validate_access_key "$AWS_ACCESS_KEY" && break
done

while true; do
    read -p "AWS Secret Access Key: " AWS_SECRET_KEY
    if [ -z "$AWS_SECRET_KEY" ]; then
        echo "Error: AWS Secret Access Key cannot be empty"
        continue
    fi
    validate_secret_key "$AWS_SECRET_KEY" && break
done

read -p "Default region (press Enter for $REGION): " user_region
if [ ! -z "$user_region" ]; then
    REGION="$user_region"
fi
validate_region "$REGION"

# Create credentials file
echo "Creating/updating credentials file..."
if [ ! -f "$HOME/.aws/credentials" ]; then
    touch "$HOME/.aws/credentials"
else
    # Remove existing profile if it exists
    sed -i "/\[$PROFILE\]/,/^\[/d" "$HOME/.aws/credentials" 2>/dev/null || true
fi

cat >> "$HOME/.aws/credentials" << EOF
[$PROFILE]
aws_access_key_id = $AWS_ACCESS_KEY
aws_secret_access_key = $AWS_SECRET_KEY

EOF

# Create config file
echo "Creating/updating config file..."
if [ ! -f "$HOME/.aws/config" ]; then
    touch "$HOME/.aws/config"
else
    # Remove existing profile if it exists
    sed -i "/\[profile $PROFILE\]/,/^\[/d" "$HOME/.aws/config" 2>/dev/null || true
fi

cat >> "$HOME/.aws/config" << EOF
[profile $PROFILE]
region = $REGION
output = $OUTPUT_FORMAT

EOF

# Set permissions
chmod 600 "$HOME/.aws/credentials"
chmod 600 "$HOME/.aws/config"

echo ""
echo "✓ AWS credentials have been set up successfully!"
echo "  Credentials file: $HOME/.aws/credentials"
echo "  Config file: $HOME/.aws/config"
echo "  Profile: $PROFILE"
echo ""
echo "You can now build and run the AWS SDK for C++ examples."
