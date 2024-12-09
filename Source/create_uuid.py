#!/usr/bin/env python3
# -*- coding:utf-8 -*-
#
# Use to create 32 bit identifiers for types.
#
import uuid

def generate_uuid():
	return str(uuid.uuid4()) # Generate UUID, convert to string

def extract_first_hex(uuid_str: str):
	if not uuid_str:
		return "", ""

	first_hex = uuid_str.split('-')[0]
	return uuid_str, first_hex.upper()

def main():
	uuid_str, first_hex = extract_first_hex(generate_uuid())
	print(f"Generated UUID: {uuid_str}")
	print(f"First hexadecimal number: 0x{first_hex}")

if __name__ == "__main__":
	main()
