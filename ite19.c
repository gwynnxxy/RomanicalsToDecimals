#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Roman numeral values
int romanValues[] = {1, 5, 10, 50, 100, 500, 1000};
char romanSymbols[] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};

// Function prototypes
int romanNumeralsToDeci(char *roman);
void deciToWords(int num, char *words);
void doInput(FILE *inputFile, FILE *outputFile);
void doArithmetic(int num1, int num2, char operator, FILE *outputFile);
int isRoman(char *str);

int main() {
    FILE *inputFile = fopen("Input.txt", "r");
    FILE *outputFile = fopen("Output.txt", "w");

    if (!inputFile || !outputFile) {
        printf("File cannot open.\n");
        return 1;
    }

    doInput(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);

    // Notify the user that output has been converted
    printf("Output is converted, check the Output.txt file.\n");

    return 0;
}

// Convert Roman numeral to decimal
int romanNumeralsToDeci(char *roman) {
    int total = 0, length = strlen(roman);
    for (int i = 0; i < length; i++) {
        int currentVal = 0, nextVal = 0;

        for (int j = 0; j < 7; j++) {
            if (roman[i] == romanSymbols[j]) currentVal = romanValues[j];
            if (i + 1 < length && roman[i + 1] == romanSymbols[j]) nextVal = romanValues[j];
        }

        total += (currentVal < nextVal) ? -currentVal : currentVal;
    }
    return total;
}

// Convert decimal number to words
void deciToWords(int num, char *words) {
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million", "Billion"};

    if (num == 0) {
        strcpy(words, "Zero");
        return;
    }

    int partCount = 0;
    char result[1024] = "";

    while (num > 0) {
        if (num % 1000 != 0) {
            char part[256] = "";
            int n = num % 1000;
            int hundreds = n / 100;
            int tensUnit = n % 100;
            int ones = tensUnit % 10;
            int ten = tensUnit / 10;

            if (hundreds) {
                strcat(part, units[hundreds]);
                strcat(part, " Hundred ");
            }

            if (tensUnit >= 10 && tensUnit < 20) {
                strcat(part, teens[tensUnit - 10]);
            } else {
                if (ten) {
                    strcat(part, tens[ten]);
                    strcat(part, " ");
                }
                if (ones) {
                    strcat(part, units[ones]);
                }
            }

            if (thousands[partCount][0] != '\0') {
                strcat(part, " ");
                strcat(part, thousands[partCount]);
            }

            // Add the part to the result, ensuring proper space between parts
            if (strlen(result) > 0) {
                strcat(result, " ");
            }
            strcat(result, part);
        }

        num /= 1000;
        partCount++;
    }

    // Remove extra space at the end of the result (only if there's extra space)
    if (strlen(result) > 0 && result[strlen(result) - 1] == ' ') {
        result[strlen(result) - 1] = '\0';
    }

    strcpy(words, result);
}

// Process input and handle multiple operations
void doInput(FILE *inputFile, FILE *outputFile) {
    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        char roman1[50], roman2[50];
        char operator;
        int num1 = 0, num2 = 0;

        // Read the line, split based on spaces
        if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3 && isRoman(roman1) && isRoman(roman2)) {
            num1 = romanNumeralsToDeci(roman1);
            num2 = romanNumeralsToDeci(roman2);
            doArithmetic(num1, num2, operator, outputFile);
        } else {
            fprintf(outputFile, "Invalid Roman Numerals.\n");
        }
    }
}

// Check if the string contains valid Roman numerals
int isRoman(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (strchr("IVXLCDM", str[i]) == NULL) {
            return 0; // Invalid Roman numeral
        }
    }
    return 1;
}

// Perform arithmetic operations
void doArithmetic(int num1, int num2, char operator, FILE *outputFile) {
    int result;
    switch (operator) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                fprintf(outputFile, "Error: Division by zero.\n");
                return;
            }
            result = num1 / num2; break;
        default:
            fprintf(outputFile, "Invalid operation.\n");
            return;
    }

    char words[1024];
    deciToWords(result, words);
    fprintf(outputFile, "%s\n", words);
}
