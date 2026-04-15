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
Input: `S 1 2 3 e` 
Output: The total resistance = 6
