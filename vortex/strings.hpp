#pragma once

#include "diagnostics.hpp"
#include "mem/utils.hpp"
#include "numbers.hpp"
#include "vortex/diagnostics.hpp"

static usize strlen(const char *const s) {
    usize i = 0;
    for (;; i++)
        if (s [i] == '\0') return i;
}

static int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

static int isdigit(char c) {
    return c >= '0' && c <= '9';
}

static int isalpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static char tolower(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

static i64 stol(const char *str, usize *idx = reinterpret_cast<usize *>(NULL), int base = 10) {
    // Skip leading whitespace
    while (isspace(static_cast<unsigned char>(*str))) {
        str++;
    }

    // Handle optional sign
    int sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    // Initialize result
    long int result = 0;

    // Check for base
    if (base < 2 || base > 36) {
        // Invalid base, treat as base 10
        base = 10;
    }

    // Loop through the string and convert to long int
    while (*str) {
        int digit;

        // Check if character is a valid digit for the base
        if (isdigit(static_cast<unsigned char>(*str))) {
            digit = *str - '0';
        } else if (isalpha(static_cast<unsigned char>(*str))) {
            digit = tolower(static_cast<unsigned char>(*str)) - 'a' + 10;
        } else {
            break; // Invalid character
        }

        if (digit >= base) {
            break; // Digit out of range for the base
        }

        result = result * base + digit;
        str++;
    }

    // Set the index if it's provided
    DIAG_IGNORE(
        {
            if (idx) {
                *idx = str - (str - strlen(str));
            }
        },
        "-Wsign-conversion"
    )

    return sign * result;
}

static isize ltos(i64 value, char *str, usize size) {
    // Handle zero case
    if (value == 0) {
        if (size > 1) {
            str [0] = '0';
            str [1] = '\0';
        }
        return 1;
    }

    long temp         = value;
    usize is_negative = 0;
    usize length      = 0;

    // Check if the number is negative
    if (value < 0) {
        is_negative = 1;
        temp        = -temp; // Make the number positive for further processing
    }

    // Calculate the number of digits
    while (temp > 0) {
        length++;
        temp /= 10;
    }

    // Ensure the buffer is large enough
    if (length + is_negative + 1 > size) {
        // Buffer is not large enough, handle accordingly (e.g., set an error code)
        if (size > 0) {
            str [0] = '\0'; // Set to an empty string
        }
        return -1;
    }

    str [length + is_negative] = '\0'; // Null-terminate the string

    // Fill the string from the end
    while (value != 0) {
        DIAG_IGNORE(
            {
                str [--length + is_negative] = '0' + (is_negative ? -(value % 10) : (value % 10));
                value /= 10;
            },
            "-Wsign-conversion"
        )
    }

    // If negative, add the '-' sign
    if (is_negative) {
        str [0] = '-';
    }

    return static_cast<isize>(length);
}
