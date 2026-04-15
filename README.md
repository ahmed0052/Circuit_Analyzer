# Advanced Circuit Analyzer

A C++ program that calculates the total equivalent resistance of an electrical circuit from a single‑string description. Supports series and parallel connections, nested circuits, and floating‑point resistor values.

## Features

- Parse circuit descriptions from a **single string** input
- Handle **Series (S/s)** and **Parallel (P/p)** connections
- Support **nested connections** (circuit inside a circuit)
- Accept integer or floating‑point resistor values (e.g., `1`, `2.5`, `3.14`)
- Spaces **or** commas as separators between elements
- Full input validation with meaningful error messages
- Interactive loop – evaluate multiple circuits in one run

## Input Format

| Symbol | Meaning |
|--------|---------|
| `S` or `s` | Series connection |
| `P` or `p` | Parallel connection |
| `e` | End of the current connection block |
| number | Resistor value (integer or float) |
| space or comma | Separator between tokens |

**Example:** `S 1 2.5 3 e` → three resistors in series.

## Examples

### 1. Simple Series
Input: `S 1 2 3 e` →
Output: The total resistance = 6

### 2. Simple Parallel 
Input: `P 2 4 e` → 
Output: The total resistance = 1.33333

### 3. Nested Circuit
Input: `P 1 2 S 1 1 e e` → 
Output: The total resistance = 0.5

*(Parallel of 1Ω, 2Ω, and Series(1Ω+1Ω=2Ω) → Parallel(1,2,2) = 0.5Ω)*

### 4. Mixed with Floating Points
Input: `S 1.5 2.3 4 e` → 
Output: The total resistance = 7.8

### 5. Using Commas as Separators 
Input: `P 1,2,3 e` → 
Output: The total resistance = 0.545455


## Error Handling

| Error Message | Cause |
|---------------|-------|
| `Incorrect Input` | Series connection with **no** resistor |
| `Incorrect Input` | Parallel connection with **only one** resistor/block |
| `Incorrect Input` | Invalid number format (e.g., `3.` without trailing digit) |
| `Wrong Description` | Unrecognised character (e.g., `w`, `x`) |
| `Wrong Description` | Unmatched `e` – more/less `e` than connection blocks |

## How It Works

1. **Validation** (`check()`)
   - Scans the entire string character by character.
   - Ensures only allowed characters: digits, `S`/`s`, `P`/`p`, `e`, `.`, space, comma.
   - Checks that every `S` block has at least one resistor.
   - Checks that every `P` block has at least two resistors or nested blocks.
   - Verifies that each `S`/`P` has a matching `e`.

2. **Calculation** (`Req()`)
   - Repeatedly finds the **innermost** connection block (last `S`/`P` before the first `e`).
   - Computes its equivalent resistance:
     - **Series** → sum of all resistor values
     - **Parallel** → `1 / (sum of 1/Ri)`
   - Replaces the whole block with the computed value.
   - Repeats until only a single number remains.

3. **Formulas**
   - Series:  `R_total = R₁ + R₂ + ... + Rₙ`
   - Parallel: `1/R_total = 1/R₁ + 1/R₂ + ... + 1/Rₙ`

### Prerequisites
- C++ compiler (C++11 or later)
- Terminal / command line


