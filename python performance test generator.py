import random
import os
import time # Keep time for overall generation time
import sys
sys.set_int_max_str_digits(20000000)

# --- Configuration ---
# Number of test cases per operation and length category
NUM_TESTS_PER_CATEGORY = 1000 # Increased to 1000 as requested

# Define number length categories (min_length, max_length)
# Note: max_length is inclusive
LENGTH_CATEGORIES = {
    "less_than_1000_chars": (1, 999),
    "1000_to_10000_chars": (1000, 9999),
    "10000_to_100000_chars": (10000, 99999),
    "100000_plus_chars": (100000, 150000), # Cap at 150k for practical reasons, adjust if needed
}

# Operations to generate tests for
OPERATIONS = {
    '+': 'addition',
    '-': 'subtraction',
    '*': 'multiplication'
}

# Output directory for test files
OUTPUT_DIR = "large_number_test_cases"

# --- Helper Functions ---

def generate_number_string(min_length, max_length, allow_negative=False):
    """Generates a random number string within the specified length range."""
    length = random.randint(min_length, max_length)
    if length == 0:
        return "0"

    # Ensure the first digit is not '0' for numbers > 1 digit
    first_digit = str(random.randint(1, 9)) if length > 1 else str(random.randint(0, 9))
    rest_digits = ''.join(random.choice('0123456789') for _ in range(length - 1))
    number_str = first_digit + rest_digits

    if allow_negative and random.random() < 0.3: # 30% chance of being negative if allowed
         return '-' + number_str
    return number_str


def calculate_expected_result(num1_str, operator_str, num2_str):
    """Calculates the expected result using Python's arbitrary-precision integers."""
    try:
        num1 = int(num1_str)
        num2 = int(num2_str)

        if operator_str == '+':
            return str(num1 + num2)
        elif operator_str == '-':
            return str(num1 - num2)
        elif operator_str == '*':
            return str(num1 * num2)
        else:
            # This should not happen with the defined OPERATIONS dictionary
            raise ValueError(f"Unsupported operator: {operator_str}")
    except ValueError as e:
        print(f"Error calculating expected result for {num1_str} {operator_str} {num2_str}: {e}")
        return "ERROR" # Indicate an error in the expected result


# --- Main Generation Logic ---

def generate_and_save_tests():
    """Generates test cases and saves them to files."""
    print("Starting Large Number Test Case Generation...")
    print("-" * 40)

    # Create output directory structure
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    total_start_time = time.time()
    total_cases_generated = 0

    for op_symbol, op_name in OPERATIONS.items():
        op_dir = os.path.join(OUTPUT_DIR, op_name)
        if not os.path.exists(op_dir):
            os.makedirs(op_dir)
        # print(f"\n--- Generating Tests for Operation: {op_symbol} ({op_name}) ---")

        for category_name, (min_len, max_len) in LENGTH_CATEGORIES.items():
            file_path = os.path.join(op_dir, f"{category_name}.txt")
            # print(f"  Generating {NUM_TESTS_PER_CATEGORY} cases for '{category_name}' into '{file_path}'...")

            category_start_time = time.time()

            with open(file_path, 'w') as f:
                for i in range(NUM_TESTS_PER_CATEGORY):
                    total_cases_generated += 1

                    # Generate numbers - allow negative for subtraction
                    allow_negative = (op_symbol == '-')
                    num1_str = generate_number_string(min_len, max_len, allow_negative)
                    num2_str = generate_number_string(min_len, max_len, allow_negative)

                    # Calculate expected result
                    expected_result = calculate_expected_result(num1_str, op_symbol, num2_str)

                    # Write test case to file: num1 operator num2 expected_result
                    f.write(f"{num1_str} {op_symbol} {num2_str} {expected_result}\n")

            category_end_time = time.time()
            print(f"  Finished '{category_name}' in {category_end_time - category_start_time:.4f} seconds")

    total_end_time = time.time()
    print("-" * 40)
    print("Generation Summary:")
    print(f"Total Test Cases Generated: {total_cases_generated}")
    print(f"Output Directory: {os.path.abspath(OUTPUT_DIR)}")
    print(f"Total Time Taken: {total_end_time - total_start_time:.4f} seconds")
    print("-" * 40)

# --- Run the generation ---
if __name__ == "__main__":
    generate_and_save_tests()
