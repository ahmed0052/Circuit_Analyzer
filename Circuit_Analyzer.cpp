/*
 * Advanced Circuit Analyzer
 * --------------------------
 * This program reads a string description of an electrical circuit
 * and calculates the total equivalent resistance.
 *
 * Input format:
 *   - 'S' or 's' : Series connection
 *   - 'P' or 'p' : Parallel connection
 *   - 'e'        : Ends a connection block
 *   - Numbers    : Resistor values (integer or float)
 *   - Separators : Spaces or commas between elements
 *
 * Example: "P 1 2 S 1 1 e e" means:
 *   Parallel(1, 2, Series(1,1)) = Parallel(1, 2, 2) = 0.5 ohms
 */

#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

bool check(string x); // Function prototype: validates the input string
float Req(string c);  // Function prototype: calculates total resistance

int main()
{
    string connection; // Stores the circuit description entered by the user
    int choice;        // Stores the user's continue/exit choice

    // Keep running until the user chooses to exit
    do
    {
        // Prompt the user to enter a circuit connection string
        cout << "Please enter a connection: ";
        getline(cin, connection);

        // Validate the input; if invalid, print error and ask again
        if (check(connection))
        {
            // Error message already printed inside check()
            // Fall through to ask the user if they want to try again
        }
        else
        {
            // Input is valid -> calculate and display the total resistance
            float z = Req(connection);
            cout << "The total resistance = " << z;
        }

        // Ask the user whether to continue or exit
        cout << "\n\nPress 1 to enter another connection, or 2 to exit: ";
        cin >> choice;

        // Clear the newline left in the buffer after reading the integer
        // so the next getline() works correctly
        cin.ignore();

        cout << "\n"; // Blank line for readability

    } while (choice == 1); // Repeat only if the user pressed 1

    cout << "Program ended. Goodbye!";

    return 0;
}

/*
 * check() - Validates the circuit description string
 * ---------------------------------------------------
 * Returns true  if the input is invalid (and prints an error message)
 * Returns false if the input is valid
 *
 * Error messages:
 *   "Wrong Description" -> an unsupported character was used
 *   "Incorrect Input"   -> valid characters but wrong structure/count
 */
bool check(string x)
{
    int l = x.length();   // Total length of the input string
    int count1 = 0;       // Counts the number of S/P connection keywords
    int count4 = 0;       // Counts the number of 'e' (end) markers

    // -------------------------------------------------------------------
    // Rule: The string must start with a valid connection type (S or P)
    // -------------------------------------------------------------------
    if (x[0] != 'p' && x[0] != 'P' && x[0] != 'S' && x[0] != 's')
    {
        cout << "Incorrect Input";
        return true;
    }

    // -------------------------------------------------------------------
    // Scan every character in the string one by one
    // -------------------------------------------------------------------
    for (int i = 0; i < l; i++)
    {
        char check1 = x[i]; // Current character being examined

        // ---------------------------------------------------------------
        // Rule: Only digits, S, P, e, dot, comma, and space are allowed.
        //       Anything else is an invalid/unsupported character.
        // ---------------------------------------------------------------
        if (!isdigit(check1) && check1 != 'P' && check1 != 'p' &&
            check1 != 'e'   && check1 != 'S' && check1 != 's' &&
            check1 != '.'   && check1 != ',' && check1 != 32)
        {
            cout << "Wrong Description";
            return true;
        }

        // ---------------------------------------------------------------
        // Rule: Total number of connection blocks (S/P) must be < 20
        // ---------------------------------------------------------------
        if (check1 == 'p' || check1 == 'P' || check1 == 's' || check1 == 'S')
        {
            count1++;
            if (count1 >= 20)
            {
                cout << "Incorrect Input";
                return true;
            }
        }

        // ---------------------------------------------------------------
        // Rule: A dot must be followed by a digit to form a valid float
        //       (e.g., "3.4" is valid, "3." alone is not)
        //       Also guards against out-of-bounds access on x[i+1]
        // ---------------------------------------------------------------
        if (check1 == '.' && (i + 1 >= l || !isdigit(x[i + 1])))
        {
            cout << "Incorrect Input";
            return true;
        }

        // ---------------------------------------------------------------
        // Count every 'e' end marker found in the string
        // ---------------------------------------------------------------
        if (check1 == 'e')
        {
            count4++;
        }

        // ---------------------------------------------------------------
        // Rule: Series connection must have AT LEAST 1 resistor or child.
        //       Skip spaces/commas after 'S' and check the next real char.
        //       If it is 'e', then no resistors were given -> invalid.
        // ---------------------------------------------------------------
        if (check1 == 's' || check1 == 'S')
        {
            int next = i + 1;

            // Move past any spaces or commas to find the next real token
            while (next < l && (x[next] == 32 || x[next] == ','))
                next++;

            // If the next real character is 'e', series block is empty
            if (next >= l || x[next] == 'e')
            {
                cout << "Incorrect Input";
                return true;
            }
        }

        // ---------------------------------------------------------------
        // Rule: Parallel connection must have AT LEAST 2 resistors/children.
        //       Count direct resistors and nested S/P blocks separately.
        //       Use a depth counter to avoid counting inside nested blocks.
        // ---------------------------------------------------------------
        if (check1 == 'p' || check1 == 'P')
        {
            int j = 0;     // Number of elements (resistors + nested blocks) found
            int depth = 0; // Nesting depth: tracks how deep we are inside child blocks

            for (int r = i + 1; r < l; r++)
            {
                // A space or comma followed by a digit means a direct resistor value
                if ((x[r] == 32 || x[r] == ',') && r + 1 < l && isdigit(x[r + 1]))
                    j++;

                // A nested S/P block at depth 0 counts as one element of this parallel block
                if ((x[r] == 's' || x[r] == 'S' || x[r] == 'p' || x[r] == 'P') && depth == 0)
                {
                    j++;
                    depth++; // We are now inside a child block; increase depth
                }

                // An 'e' either closes a child block or closes this parallel block
                if (x[r] == 'e')
                {
                    if (depth > 0)
                        depth--;   // Closing a nested child block
                    else
                        break;     // Closing this parallel block; stop counting
                }
            }

            // Parallel needs at least 2 elements
            if (j <= 1)
            {
                cout << "Incorrect Input";
                return true;
            }
        }
    }

    // -------------------------------------------------------------------
    // Rule: Every S/P connection must have exactly one matching 'e' end.
    //       If counts differ, the structure is malformed.
    // -------------------------------------------------------------------
    if (count1 != count4)
    {
        cout << "Wrong Description";
        return true;
    }

    return false; // Input passed all checks; it is valid
}

/*
 * Req() - Calculates the total equivalent resistance
 * ---------------------------------------------------
 * Strategy: repeatedly find the INNERMOST connection block
 * (the last S/P keyword before the first 'e'), compute its
 * resistance, replace the whole block with the result, and
 * repeat until only a single number remains in the string.
 *
 * Series formula   : R_total = R1 + R2 + ... + Rn
 * Parallel formula : 1/R_total = 1/R1 + 1/R2 + ... + 1/Rn
 */
float Req(string c)
{
    // Keep reducing the string until only a plain number is left
    while (!isdigit(c[0]))
    {
        int startindex = 0; // Index of the innermost S/P keyword
        int endindex   = 0; // Index of the first 'e' after that keyword

        // ---------------------------------------------------------------
        // Find the innermost block:
        //   - Scan left to right, updating startindex on every S/P found.
        //   - Stop at the first 'e' encountered; that 'e' matches the
        //     last S/P we recorded (the innermost one).
        // ---------------------------------------------------------------
        for (int i = 0; i < (int)c.length(); i++)
        {
            if (c[i] == 'p' || c[i] == 'P' || c[i] == 's' || c[i] == 'S')
            {
                startindex = i; // Update to the latest S/P seen so far
            }
            else if (c[i] == 'e')
            {
                endindex = i;   // This 'e' closes the innermost block
                break;
            }
        }

        // Extract the innermost block as a substring (e.g., "S 1 2 e")
        string startcalc = c.substr(startindex, endindex - startindex + 1);

        // Remove the extracted block from the main string
        c.erase(startindex, endindex - startindex + 1);

        // ---------------------------------------------------------------
        // PARALLEL block: compute 1/Req = sum of (1/Ri) for each resistor
        // ---------------------------------------------------------------
        if (startcalc[0] == 'p' || startcalc[0] == 'P')
        {
            float Rp = 0.0;   // Accumulates the sum of reciprocals (1/R1 + 1/R2 + ...)
            string Rpp = "";  // Builds each resistor value character by character

            // Start at index 1 to skip the 'P' keyword itself
            for (int i = 1; i < (int)startcalc.length(); i++)
            {
                if (isdigit(startcalc[i]) || startcalc[i] == '.')
                {
                    // Build up the resistor value digit by digit
                    Rpp += startcalc[i];
                }
                else if (!Rpp.empty())
                {
                    // Non-digit encountered and we have a complete number -> add 1/R
                    Rp += 1.0f / stof(Rpp);
                    Rpp = ""; // Reset for the next resistor
                }
            }

            // Flush the last resistor value if it wasn't already processed
            // (this handles the case where the last number ends right before 'e')
            if (!Rpp.empty())
                Rp += 1.0f / stof(Rpp);

            // Compute the equivalent parallel resistance: Req = 1 / (sum of 1/Ri)
            float parallelResistance = (Rp > 0) ? 1.0f / Rp : 0;

            // Insert result back into the string surrounded by spaces
            // so it doesn't accidentally merge with neighbouring characters
            c.insert(startindex, " " + to_string(parallelResistance) + " ");
        }

        // ---------------------------------------------------------------
        // SERIES block: compute Req = R1 + R2 + ... + Rn
        // ---------------------------------------------------------------
        else if (startcalc[0] == 's' || startcalc[0] == 'S')
        {
            float Rs = 0.0;   // Accumulates the sum of all resistor values
            string Rss = "";  // Builds each resistor value character by character

            // Start at index 1 to skip the 'S' keyword itself
            for (int i = 1; i < (int)startcalc.length(); i++)
            {
                if (isdigit(startcalc[i]) || startcalc[i] == '.')
                {
                    // Build up the resistor value digit by digit
                    Rss += startcalc[i];
                }
                else if (!Rss.empty())
                {
                    // Non-digit encountered and we have a complete number -> add to sum
                    Rs += stof(Rss);
                    Rss = ""; // Reset for the next resistor
                }
            }

            // Flush the last resistor value if it wasn't already processed
            if (!Rss.empty())
                Rs += stof(Rss);

            // Insert result back into the string surrounded by spaces
            c.insert(startindex, " " + to_string(Rs) + " ");
        }
    }

    // The string has been fully reduced to a single number -> return it
    return stof(c);
}
