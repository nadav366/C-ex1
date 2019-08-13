#include <stdio.h>
#include <stdlib.h> // Imported for "EXIT_FAILURE" and "EXIT_SUCCESS"
#include <string.h> // Imported for "strtok"
#include <math.h>  // Imported for "fmax", "floor", "ceil"

/**
 * @brief Defines actions on exit from the program due to fault - fault printing and return value
 */
#define ERROR fprintf(stderr, "ERROR\n"); return EXIT_FAILURE;
/**
 * @brief The delimiters used to separate the input
 */
#define INPUT_DELIMS " \t\r\n"
/**
 * @brief Sets a divider by 2 and returns a floor value
 */
#define FLOOR_DIV_BY_TWO(size) (int)(floor((size) / 2.0))
/**
 * @brief Sets a division operation by 2 and returns a top value
 */
#define CEIL_DIV_BY_TWO(size) (int)(ceil(size / 2.0))
/**
 * @brief Defines an operation in checking that character is a digit
 */
#define IS_DIG(curChar) (START_DIG <= curChar) && (curChar <= END_DIG)
/**
 * @brief Defines the maximum line length.
 */
#define LINE_MAX_SIZE 1024
/**
 * @brief Defines the maximum number of cells in an array
 */
#define ARR_MAX_SIZE 100


/**
 * @brief A constant that marks a TRUE boolean value
 */
const int TRUE = 1;
/**
 * @brief A constant that marks a FALSE boolean value
 */
const int FALSE = 0;
/**
 * @brief Fixed signaling invalid return value
 */
const int ILLEGAL = -1;
/**
 * @brief A constant that marks an empty cell in the array
 */
const double EMPTY_CELL = 0.0f;
/**
 * @brief A constant that marks an empty cell in the array
 */
const double_t EPSILON = 0.00000001;
/**
 * @brief A constant that marks an order of magnitude between digits in a number
 */
const int SCALE = 10;
/**
 * @brief A constant that marks the maximum number of characters in a number
 */
const int STRING_MAX_SIZE = 9;
/**
 * @brief A constant that marks the maximum number of characters in a number
 */
const int PRECISION = 1000;
/**
 * @brief A constant that marks the maximum number of stars in the histogram
 */
const int STAR_NUM = 20;
/**
 * @brief A constant that marks the first cell index
 */
const int FIRST_CELL = 0;
/**
 * @brief A constant that marks the first cell index
 */
const int SEC_CELL = 1;

/**
 * @brief The character is a star
 */
const char STAR = '*';
/**
 * @brief The character that represents the last digit
 */
const char START_DIG = '0';
/**
 * @brief The character that represents the last digit
 */
const char END_DIG = '9';
/**
 * @brief The character that represents the last digit
 */
const char DOT = '.';

// Helper functions

/**
 * @brief A function that converts a character string to a double number
 * @param value Pointer to a set of characters representing the string
 * @param len The length of the string
 * @return The number, if the input is valid. -1 otherwise.
 */
double strToDouble(const char *value, size_t len)
{
    double num = 0.0f;
    int franc = FALSE; // Flag, the mother was already a point of a fraction

    for (int i = 0; i < (int) len; ++i)
    {
        if (!franc) // Literature before the point
        {
            if (value[i] == DOT)
            {
                franc = TRUE;
            }
                // Check that the number does not start at zero
            else if ((i == FIRST_CELL) && (value[i] == START_DIG) && ((int) len != SEC_CELL) &&
                     (value[SEC_CELL] != DOT))
            {
                return ILLEGAL;
            }
            else if (IS_DIG(value[i]))
            {
                num *= SCALE;
                num += value[i] - START_DIG;
            }
            else // not digit
            {
                return ILLEGAL;
            }
        }
        else // The digit is after the point
        {
            if (IS_DIG(value[i]))
            {
                franc *= SCALE;
                num += (value[i] - START_DIG) / (double) franc;
            }
            else
            {
                return ILLEGAL;
            }
        }
    } // for loop
    return num;
}

/**
 * @brief An indexing parameterization function, by array size.
 * @param index The index it is trying to access
 * @return The index after the parameterization if it is within the array boundaries. -1 otherwise.
 */
int parm(int index)
{
    int newInex = index + FLOOR_DIV_BY_TWO(ARR_MAX_SIZE);
    if ((newInex >= FIRST_CELL) && (newInex < ARR_MAX_SIZE))
    {
        return newInex;
    }
    return ILLEGAL;
}

/**
 * A function that receives from the user the number of convocations that need to be done
 * @return The number of convulsions, if the input is valid. -1 otherwise.
 */
int getN()
{
    char nStr[LINE_MAX_SIZE];
    fgets(nStr, LINE_MAX_SIZE, stdin);
    for (int i = 0; i < (int) strlen(nStr) - 1; ++i)
    {
        if (strchr(INPUT_DELIMS, nStr[i]))
        {
            return ILLEGAL;
        }
    }

    char *value = strtok(nStr, INPUT_DELIMS);
    if (value == NULL)
    {
        return ILLEGAL;
    }
    double dounleN = strToDouble(value, strlen(value));
    int n = (int) dounleN;
    if (((double) n != dounleN) || (n < FIRST_CELL)) // If the number is not int or invalid input
    {
        return ILLEGAL;
    }
    return n;
}

/**
 * @brief A function that receives a set of numbers from the user
 * @param doubleArr Pointer to a array of numbers, fill the input from the user
 * @return
 */
int getLine(double *doubleArr)
{
    char line[LINE_MAX_SIZE];
    char *value = NULL;

    /* Reads the line from the stdin */
    if (fgets(line, LINE_MAX_SIZE, stdin) == NULL)
    {
        return ILLEGAL;
    }

    value = strtok(line, INPUT_DELIMS);
    if (value == NULL)
    {
        return ILLEGAL;
    }

    int i = 0;

    /* Handle every value and read the next value */
    while (value != NULL)
    {
        /* Read the value and print it */
        size_t length = strlen(value);

        /* Validity check of value */
        if ((i >= ARR_MAX_SIZE) || ((int) length > STRING_MAX_SIZE) ||
            ((doubleArr[i] = strToDouble(value, length)) < FIRST_CELL))
        {
            return ILLEGAL;
        }
        i++;

        /* Read the next value */
        value = strtok(NULL, INPUT_DELIMS);
    }
    return i;
}

// Different steps in software

/**
 * @brief A function that prints the  histogram of number array
 * @param resArr Pointer to array numbers
 */
void histogram(double *resArr)
{
    /* Search the maximum value in the array */
    double maxArr = -1;
    for (int i = 0; i < ARR_MAX_SIZE; ++i)
    {
        resArr[i] = (round(PRECISION * resArr[i]) / PRECISION);
        maxArr = fmax(maxArr, resArr[i]);
    }
    if (maxArr <= EPSILON) // If no prerequisite for printing exists
    {
        return;
    }

    /* Printing the histogram */
    for (int i = 0; i < ARR_MAX_SIZE; ++i)
    {
        printf("%0.3f: ", resArr[i]);
        for (int j = 0; j < (int) ((resArr[i] / maxArr) * STAR_NUM); ++j)
        {
            printf("%c", STAR);
        }
        printf("\n");
    }
}

/**
 * @brief Normalize a number array
 * @param arr Pointer to a number array
 */
void normalization(double *arr)
{
    double sum = 0.0f;
    for (int i = 0; i < ARR_MAX_SIZE; ++i)
    {
        sum += arr[i];
    }
    if (sum > EPSILON)
    {
        for (int i = 0; i < ARR_MAX_SIZE; ++i)
        {
            arr[i] = arr[i] / sum;
        }
    }
}

/**
 * @brief Centering a number array
 * @param arr Pointer to a number array
 * @param size Size of the array
 */
void centering(double *arr, int size)
{
    //Calculate the number of zeros on each side
    int empySize = FLOOR_DIV_BY_TWO(ARR_MAX_SIZE - size);
    for (int i = 1; i <= empySize; i++)
    {
        arr[ARR_MAX_SIZE - i] = EMPTY_CELL;
    }
    for (int i = size; i >= 0; i--)
    {
        arr[empySize + i] = arr[i];
    }
    for (int i = 0; i < empySize; i++)
    {
        arr[i] = EMPTY_CELL;
    }
}

/**
 * @brief A function that do Convolution between two number arrays
 * @param gArrP Pointer to first number set
 * @param hArrP Pointer to second number set
 * @param convP Pointer to a number set for the calculation result
 * @return  pointer to the result set
 */
double *convolution(double *gArrP, const double *hArrP, double *convP)
{
    /* Variables for index retention, after parameterization */
    int parmT;
    int parmM;
    int parmOpezM;

    for (int t = -(CEIL_DIV_BY_TWO(ARR_MAX_SIZE) + 1); t <= FLOOR_DIV_BY_TWO (ARR_MAX_SIZE); t++)
    {
        if ((parmT = parm(t + 1)) >= FIRST_CELL)
        {
            convP[parmT] = EMPTY_CELL;
            for (int m = -(CEIL_DIV_BY_TWO(ARR_MAX_SIZE));
                 m <= FLOOR_DIV_BY_TWO (ARR_MAX_SIZE); m++)
            {
                if (((parmM = parm(m)) >= FIRST_CELL) &&
                    (((parmOpezM = parm(t - m)) >= FIRST_CELL)))
                {
                    convP[parmT] += gArrP[parmOpezM] * hArrP[parmM];
                }
            }
        }
    }
    normalization(convP);
    for (int j = 0; j < ARR_MAX_SIZE; ++j)
    {
        gArrP[j] = convP[j];
    }

    return gArrP;
}

// Main function

/**
 * @brief The main function that runs the program.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main()
{
    double gArr[ARR_MAX_SIZE], hArr[ARR_MAX_SIZE];
    int gSize, hSize;

    // Receiving arrays g, h from the user, and checking their integrity -
    if ((gSize = getLine(gArr)) == ILLEGAL || (hSize = getLine(hArr)) == ILLEGAL || (hSize > gSize))
    {
        ERROR
    }

    // Receiving the number n from the user and checking their integrity-
    int n = getN();
    if (n < 0)
    {
        ERROR
    }

    // Centering and normalizing the arrays-
    centering(gArr, gSize);
    centering(hArr, hSize);
    normalization(gArr);
    normalization(hArr);

    // Centering and normalizing the arrays
    double conv[ARR_MAX_SIZE];
    double *resArr = gArr;
    for (int i = 0; i < n; i++)
    {
        resArr = convolution(resArr, hArr, conv);
    }

    // Histogram printing and ending
    histogram(resArr);
    return EXIT_SUCCESS;
}
