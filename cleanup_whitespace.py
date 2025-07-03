#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script to clean up trailing whitespace and blank lines in code files.
"""

import os
from pathlib import Path

def cleanup_file(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        new_lines = [line.rstrip() + '\n' if line.strip() else '\n' for line in lines]
        if new_lines != lines:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.writelines(new_lines)
            print(f"Cleaned: {file_path}")
            return True
        else:
            print(f"No cleanup needed: {file_path}")
            return False
    except Exception as e:
        print(f"Failed to process {file_path}: {e}")
        return False

def cleanup_dir(root):
    total = 0
    cleaned = 0
    for ext in ['*.cpp', '*.h', '*.hpp', '*.c', '*.cc', '*.cxx']:
        for path in Path(root).rglob(ext):
            total += 1
            if cleanup_file(str(path)):
                cleaned += 1
    return total, cleaned

def main():
    print("Start cleaning whitespace...")
    print("=" * 50)
    current_dir = Path.cwd()
    print(f"Working directory: {current_dir}")
    total, cleaned = cleanup_dir(current_dir)
    print("=" * 50)
    print(f"Summary:")
    print(f"   Total files: {total}")
    print(f"   Files cleaned: {cleaned}")
    print(f"   No cleanup needed: {total - cleaned}")
    if cleaned > 0:
        print("Whitespace cleanup finished!")
    else:
        print("No files needed cleaning.")

if __name__ == "__main__":
    main() 