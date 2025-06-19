import os
import sys

sys.set_int_max_str_digits(1000000)

def verify_extreme_numbers_output(filename="Numbers.txt"):
    """
    Reads the output file from the C++ program and verifies the calculations
    using Python's arbitrary-precision integers.
    """
    if not os.path.exists(filename):
        print(f"Error: File '{filename}' not found. Please run the C++ program first.")
        return

    print(f"Reading '{filename}' and verifying calculations...")

    with open(filename, 'r') as f:
        file_lines = [line.strip() for line in f if line.strip()] # Read non-empty lines

    # Replicate the C++ logic
    number = 9
    number2 = 1
    calculated_outputs = []

    # The outer loop runs 3 times (i = 1 to 3)
    for i in range(1, 4):
        cont = number
        # The inner loop runs j from 1 up to the value of number2
        # In the first outer loop iteration, number2 is 1, so inner loop runs once (j=1)
        # In subsequent iterations, number2 becomes the large number calculated in the previous inner loop
        # We need to be careful here. The C++ loop condition is `j <= Number2`.
        # In the C++ code, `Number2` is an ExtremeNumber. The comparison `j <= Number2`
        # likely relies on the overloaded `<=` operator for ExtremeNumber and int.
        # The inner loop runs as long as the integer `j` is less than or equal to the value represented by `Number2`.
        # So, the loop should run `int(number2)` times, with `j` going from 1 up to `int(number2)`.
        # Let's check the C++ code again: `for(int j = 1; j<=Number2; ++j)`
        # This means the loop iterates `Number2` times, starting with j=1.
        # So, the range should be from 1 up to `int(number2) + 1` (exclusive end).

        # Ensure number2 is treated as an integer for the loop range
        number2_int = int(number2)

        for j in range(1, number2_int + 1):
            if(j==10000):
                break
            number = number2 * number
            calculated_outputs.append(str(number))
            print(j)
            # print(f"Calculated j={j}, number={number}") # Optional: for debugging

        # After inner loop
        number2 = number
        # print(f"After inner loop i={i}, number2 updated to {number2}") # Optional: for debugging
        number = cont
        # print(f"After inner loop i={i}, number restored to {number}") # Optional: for debugging


    # Compare calculated outputs with file contents
    if len(calculated_outputs) != len(file_lines):
        print(f"Mismatch: Number of calculated lines ({len(calculated_outputs)}) does not match file lines ({len(file_lines)}).")
        return

    mismatches = 0
    for k in range(len(calculated_outputs)):
        if calculated_outputs[k] != file_lines[k]:
            print(f"Mismatch on line {k+1}:")
            print(f"  File:      {file_lines[k]}")
            print(f"  Calculated:{calculated_outputs[k]}")
            mismatches += 1

    if mismatches == 0:
        print("Verification successful: All calculated numbers match the file content.")
    else:
        print(f"Verification failed: Found {mismatches} mismatches.")

# Run the verification
verify_extreme_numbers_output()
