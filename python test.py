import sys
sys.set_int_max_str_digits(20000000)
import os
import time

# Define the list of test files, mirroring the C++ array
files = [
    "addition/less_than_1000_chars.txt",
    "addition/1000_to_10000_chars.txt",
    "addition/10000_to_100000_chars.txt",
    "addition/100000_plus_chars.txt",
    "substraction/less_than_1000_chars.txt",
    "substraction/1000_to_10000_chars.txt",
    "substraction/10000_to_100000_chars.txt",
    "substraction/100000_plus_chars.txt",
    "multiplication/less_than_1000_chars.txt",
    "multiplication/1000_to_10000_chars.txt",
    "multiplication/10000_to_100000_chars.txt",
    "multiplication/100000_plus_chars.txt"
]

# Base directory for test cases
TEST_CASES_DIR = "large_number_test_cases/"

print("Starting Python Test Execution...")
print("-" * 40)

total_files = len(files)
current_file_index = 0

for file_path_suffix in files:
    total_start_time = time.time()
    current_file_index += 1
    full_file_path = os.path.join(TEST_CASES_DIR, file_path_suffix)

    print(f"Processing file {current_file_index}/{total_files}: {full_file_path}")

    try:
        with open(full_file_path, 'r') as f:
            test_count = 0
            for line in f:
                test_count += 1
                if(test_count == 11):
                    break
                # Split the line into components
                parts = line.strip().split()

                if len(parts) != 4:
                    print(f"Error: Skipping malformed line {test_count} in {file_path_suffix}: {line.strip()}")
                    continue

                a_str, operation, b_str, expected_result_str = parts

                try:
                    # Convert strings to Python integers
                    # Python's int handles arbitrary precision
                    number = int(a_str)
                    number2 = int(b_str)

                    # Perform the operation
                    for i in range(1000):
                        if operation == "+":
                            Cont = number + number2
                        elif operation == "-":
                            Cont = number - number2
                        elif operation == "*":
                            Cont = number * number2
                        else:
                            print(f"Error: Unknown operation '{operation}' in line {test_count} of {file_path_suffix}")
                            continue # Skip this test case

                    # Convert the result back to a string for comparison
                    actual_result_str = str(Cont)

                    # Compare actual result with expected result
                    if actual_result_str == expected_result_str:
                        # print(f"{test_count}: OK") # Uncomment for verbose output
                        pass # Keep output clean, only print errors
                    else:
                        print(f"Error in {file_path_suffix}, line {test_count}:")
                        print(f"  Input: {a_str} {operation} {b_str}")
                        print(f"  Expected: {expected_result_str}")
                        print(f"  Actual:   {actual_result_str}")

                except ValueError as e:
                    print(f"Error parsing numbers in line {test_count} of {file_path_suffix}: {e}")
                except Exception as e:
                    print(f"An unexpected error occurred processing line {test_count} in {file_path_suffix}: {e}")

    except FileNotFoundError:
        print(f"Error: Test file not found: {full_file_path}")
    except Exception as e:
        print(f"An error occurred while reading file {full_file_path}: {e}")
    category_end_time = time.time()
    print(f"  Finished '{file_path_suffix}' in {category_end_time - total_start_time:.4f} seconds")

print("-" * 40)
print("Python Test Execution Finished.")
