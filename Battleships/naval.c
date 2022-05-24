#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//@brief initialises the boards so that they just contain '.'s in every cell.
//@param rowCount: height of boards
//@param colCount: width of boards
//@return initialised board (2D char Array)
char **init_board(int rowCount, int colCount)
{
    char **board = malloc(sizeof(char) * 100);
    // Iterate through each cell in  the boards:
    for (int currentRow = 0; currentRow < rowCount; currentRow++)
    {
        board[currentRow] = malloc(colCount * sizeof(char));
        for (int currentCol = 0; currentCol <= colCount; currentCol++)
        {
            // Set cells to be '.'s:
            board[currentRow][currentCol] = '.';
        }
    }
    return board;
}

//@brief The following function takes a board, the starting cordinates
// of the ship defined by startsC (Columns), StartsR (Rows)
// and coords (direction). It then uses these coordinates to draw the ships
// onto the board up to their length.
//@param coords: The compass direction of the ships.
//@param startsC: The starting columns of the ships.
//@param startsR: The starting row number of the ships.
//@param board: The board being drawn on.
//@param shipLengths:
//@return boolean value
bool draw_ships(char *coords, char *startsC, int *startsR,
                char **board, int *shipLengths)
{
    // Define the possible ship ID's from 1 to F:
    char *ships = "123456789ABCDEF";
    char shipID;
    // Iterate through all of the ships, up to the 15th ship (max allowed):
    for (int i = 0; i < 15; i++)
    {
        // Check if ship length is larger then 0 (otherwise ship doesn't exist):
        if (shipLengths[i] > 0)
        {
            // Assign the ShipID to its respective value defined above:
            shipID = ships[i];
            // Check which direction the ship is facing (N, E, S or W):
            if (coords[i] == 'N')
            {
                // Draw the ship up to and including the cell equal to
                // the length of the ship:
                for (int j = 0; j < shipLengths[i]; j++)
                {
                    if (board[startsR[i] - j - 1][(startsC[i] - 'A')] != '.')
                    {
                        return true;
                    }
                    board[startsR[i] - j - 1][(startsC[i] - 'A')] = shipID;
                }
            }
            // Repeat the above steps if direction is not North:
            if (coords[i] == 'E')
            {
                for (int j = 0; j < shipLengths[i]; j++)
                {
                    if (board[startsR[i] - 1][(startsC[i] - 'A' + j)] != '.')
                    {
                        return true;
                    }
                    board[startsR[i] - 1][(startsC[i] - 'A' + j)] = shipID;
                }
            }
            if (coords[i] == 'S')
            {
                for (int j = 0; j < shipLengths[i]; j++)
                {
                    if (board[startsR[i] + j - 1][(startsC[i] - 'A')] != '.')
                    {
                        return true;
                    }
                    board[startsR[i] + j - 1][(startsC[i] - 'A')] = shipID;
                }
            }
            if (coords[i] == 'W')
            {
                for (int j = 0; j < shipLengths[i]; j++)
                {
                    if (board[startsR[i] - 1][(startsC[i] - 'A' - j)] != '.')
                    {
                        return true;
                    }
                    board[startsR[i] - 1][(startsC[i] - 'A' - j)] = shipID;
                }
            }
        }
    }
    return false;
}

//@brief Prints the board cell by cell to the terminal.
//@param board: Current board being manipulated.
//@param seperator: whether a seperator (===) needs to be printed.
//@param rowCount: height of the board.
//@param colCount: width of the board.
//@return None
void draw_boards(char **board, bool seperator, int rowCount, int colCount)
{

    // Define the top border of the game:
    char *borderTop = "   ABCDEFGHIJKLMNOPQRSTUVWRXYZ";

    // Print each value of the top border up to the board width plus the three
    // white spaces:
    for (int i = 0; i < (colCount + 3); i++)
    {
        printf("%c", borderTop[i]);
    }
    // Move to the first line of the board:
    printf("\n");
    // Iterate through each row first printing the number of that row and then
    // each cell for each column in that row:
    for (int currentRow = 0; currentRow < rowCount; currentRow++)
    {
        if (currentRow >= 9)
        {
            printf(" %d", currentRow + 1);
        }
        else
        {
            printf(" %d ", currentRow + 1);
        }
        for (int currentCol = 0; currentCol < colCount; currentCol++)
        {
            printf("%c", board[currentRow][currentCol]);
            if (currentCol == colCount - 1)
            {
                printf("\n");
            }
        }
    }
    // If the cpu board has just been printed the seperator is needed:
    if (seperator)
    {
        printf("===\n");
    }
}

//@brief handle player input guesses from the terminal
//@param boardC: The cpu board that is being manipulated.
//@param boardH: The hidden cpu that contains the ships.
//@param rowCount: height of the board.
//@return the number of the ship that is hit or 0 if no ship is hit.
//@error 130: Bad guess
int player_guess(char **boardC, char **boardH, int rowCount)
{
    int row, iCol;
    char cCol, current, fail;
    char *input = malloc(sizeof(char) * 100);

    printf("(Your move)>");
    fgets(input, 100, stdin);

    sscanf(input, " %c %d %c", &cCol, &row, &fail);
    // Check for end of player input from file:
    if (feof(stdin))
    {
        fprintf(stderr, "Bad guess\n");
        exit(130);
    }
    // Iterate through input to find invalid symbols etc.
    for (int i = 0; i < (strlen(input) - 1); i++)
    {
        if (!isalnum(input[i]) && input[i] != ' ')
        {
            printf("Bad guess\n");
            return player_guess(boardC, boardH, rowCount);
        }
    }
    free(input);
    row--;
    iCol = cCol - 'A';
    // Check if guess out of bounds:
    if (iCol > strlen(boardH[0]) || row > rowCount || fail)
    {
        printf("Bad guess\n");
        return player_guess(boardC, boardH, rowCount);
    }
    else
    {
        // Check the cell on the board that the player inputs:
        current = boardH[row][iCol];
    }
    // If the cell is a valid ship identifier print hit if it is or miss if not:
    if ((current <= '9' && current >= '0') ||
        (current <= 'F' && current >= 'A'))
    {
        printf("Hit\n");
        int n = boardH[row][iCol] - '0';
        boardC[row][iCol] = '*';
        boardH[row][iCol] = '*';
        return n;
    }
    else if (current == '/' || current == 42)
    {
        // Print repeated guess if the player has made this guess before:
        printf("Repeated guess\n");
        return player_guess(boardC, boardH, rowCount);
    }
    else
    {
        printf("Miss\n");
        boardC[row][iCol] = '/';
        boardH[row][iCol] = '/';
        return 0;
    }
}

//@brief handle CPU input guesses from the current line of cpu.turns
//@param boardP: The player board that is being manipulated.
//@param rowCount: height of the board.
//@return the number of the ship that is hit or 0 if no ship is hit.
//@error 140: CPU player gives up
int cpu_guess(char **boardP, FILE *cGuesses, int rowCount)
{

    int row, iCol;
    char cCol, current;

    char *currentLine = malloc(sizeof(char) * 100);

    fgets(currentLine, 100, cGuesses);

    if (strlen(currentLine) < 2)
    {
        printf("(CPU move)>");
        fprintf(stderr, "CPU player gives up\n");
        exit(140);
    }
    // Assign the first character and int to the coordinate of the guess:
    sscanf(currentLine, " %c %d", &cCol, &row);

    row--;

    iCol = cCol - 'A';
    // If the line is not a comment, check to see if it is valid and then do
    // the same as player_guess():
    if (cCol != '#')
    {
        printf("(CPU move)>%s", currentLine);
        free(currentLine);
        if (iCol >= strlen(boardP[0]) || row > rowCount || row < 0 ||
            row >= rowCount || iCol < 0)
        {
            printf("Bad guess\n");
            return cpu_guess(boardP, cGuesses, rowCount);
        }

        current = boardP[row][iCol];

        if (current <= '9' && current >= '0')
        {
            printf("Hit\n");
            int n = boardP[row][iCol] - '0';
            boardP[row][iCol] = '*';
            return n;
        }
        else if (current == '/' || current == '*')
        {
            printf("Repeated guess\n");
            return cpu_guess(boardP, cGuesses, rowCount);
        }
        else
        {
            printf("Miss\n");
            return 0;
        }
    }
    else
    {
        free(currentLine);
        return cpu_guess(boardP, cGuesses, rowCount);
    }
}

//@brief Reads from the rules file char by char extracting all ints.
//@param file: File to be read.
//@return the list of integers found in the file.
//@error 50: Error in rules file
int *rule_reader(FILE *file)
{
    char *line = malloc(sizeof(char) * 4);
    static int values[20];
    int i = 0;
    int j = 0;
    int commentLine = 0;
    char c = fgetc(file);
    // Iterate through each character until end of file occurs, and if the
    // character is an int add it to a temporary line, then add the atoi() of
    // the temporary line to the int list being returned.
    while (c != EOF)
    {
        while (commentLine == 1)
        {
            if (c == '\n')
            {
                commentLine = 0;
            }
            c = fgetc(file);
        }
        if (c == '#')
        {
            commentLine = 1;
        }
        else if (c >= '0' && c <= '9')
        {
            line[i] = c;
            i++;
        }
        else if (c == '\n' || c == ' ')
        {
            if (strlen(line) > 0)
            {
                values[j] = atoi(line);
                j++;
            }
            i = 0;
            line = malloc(sizeof(char) * 4);
        }
        c = fgetc(file);
    }
    // Check that the values are valid:
    if (values[0] == 0 || values[0] > 26 || values[1] == 0 || values[1] > 26)
    {
        fprintf(stderr, "Error in rules file\n");
        exit(50);
    }

    free(line);
    return values;
}

//@brief Handles map error given an input type either "CPU" or "player".
//@param type: Which type of player is causing the error.
//@return None.
//@error 100: Error in player map file
//@error 110: Error in CPU map file
void map_error(char *type)
{
    fprintf(stderr, "Error in %s map file\n", type);
    if (strcmp(type, "player") == 0)
    {
        exit(100);
    }
    else
    {
        exit(110);
    }
}

//@brief Handles out of bounds error given an input player type.
//@param type: Which type of player is causing the error ("CPU" or "player").
//@return None.
//@error 80: Out of bounds in player map file
//@error 90: Out of bounds in CPU map file
void bounds_error(char *type)
{
    fprintf(stderr, "Out of bounds in %s map file\n", type);
    if (strcmp(type, "player") == 0)
    {
        exit(80);
    }
    else
    {
        exit(90);
    }
}

//@brief Reads and interprets the given map file
//@param file: File to be read.
//@param coords: list of compass directions read from map file
//@param startsCol: list of starting columns read from map file
//@param startsRow: list of starting rows read from map file
//@param type: Which type of player is causing the error ("CPU" or "player").
//@return None.
void map_reader(FILE *file, char *coords, char *startsCol,
                int *startsRow, char *type)
{
    int i = 0;
    char col, coord, hash;
    int row;
    char fail;

    char *currentLine = malloc(sizeof(char) * 100);

    // Read the file line by line and if the first character is not a hash then
    // Pull the first character, integer and compass direction from the line and
    // add them to their respective lists. Then move to the next element in the
    // lists.
    while (fgets(currentLine, 100, file) != NULL)
    {
        sscanf(currentLine, " %c", &hash);

        if (hash != '#')
        {
            for (int j = 0; j < strlen(currentLine); j++)
            {
                if ((j + 1) < strlen(currentLine))
                {
                    if (currentLine[j] != ' ' && currentLine[j + 1] == ' ')
                    {
                        free(currentLine);
                        map_error(type);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            sscanf(currentLine, " %c %d %c %c \n", &col, &row, &coord, &fail);
            if (col < 'A' || col > 'Z' || row > 26 || fail)
            {
                map_error(type);
            }
            if (coord != 'S' && coord != 'N' && coord != 'E' && coord != 'W')
            {
                bounds_error(type);
            }

            startsCol[i] = col;
            startsRow[i] = row;
            coords[i] = coord;

            i++;
        }
    }
}

//@brief Checks the boards to see if either player has won the game
//@param boardC: the CPU board
//@param boardP: the Player board
//@param rows: height of the boards
//@param cols: width of the boards
//@return 1 if player wins, 2 if cpu wins or 0 if the game is not over
int check_end(char **boardC, char **boardP, int rows, int cols)
{

    char c;
    int count = 0;

    // Check each cell in the cpu board and if no ships are present return 1:
    for (int i = 0; i < rows; i++)
    {

        for (int j = 0; j < cols; j++)
        {

            c = boardC[i][j];
            if (c >= '0' && c <= '9')
            {
                count++;
            }
            if (c >= 'A' && c <= 'F')
            {
                count++;
            }
        }
    }

    if (count == 0)
    {
        return 1;
    }
    count = 0;

    // Do the same as above but for the player board and return 2 if count == 0:
    for (int i = 0; i < rows; i++)
    {

        for (int j = 0; j < cols; j++)
        {

            c = boardP[i][j];
            if (c >= '0' && c <= '9')
            {
                count++;
            }
        }
    }

    if (count == 0)
    {
        return 2;
    }

    return 0;
}

//@brief Checks the board to see if the given ship has sunk
//@param board: the CPU board
//@param rows: height of the boards
//@param cols: width of the boards
//@param currentShip: the ship id that is being searched for on the board
//@return None
void check_sunk(char **board, int rows, int cols, int currentShip)
{
    int count = 0;
    // Iterate through the board, if currentShip's ID is found add to a count.
    // If this count is zero after the loop, then the ship has sunk.
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (board[i][j] - '0' == currentShip)
            {
                count++;
            }
        }
    }
    if (count == 0)
    {
        printf("Ship sunk\n");
    }
}

//@brief Checks that the ships are all in  the bounds of the board
//@param plr: whether the map file is the "player" map file or "CPU" map file.
//@param rows: height of the boards
//@param cols: width of the boards
//@param coords: The compass direction of the ships.
//@param startsC: The starting columns of the ships.
//@param startsR: The starting row number of the ships.
//@param shipLengths: The lengths of the ships.
//@return None
void check_bounds(char *plr, int rows, int cols, int *startsR, char *coords,
                  char *startsC, int *shipLengths)
{
    // Iterate through the ship lengths using a for loop:
    for (int i = 0; i < sizeof(shipLengths); i++)
    {
        int row = startsR[i];
        int col = startsC[i] - 'A';
        int length = shipLengths[i];
        int colIncrement, rowIncrement;

        // Set increment value for the for loop below depending on the direction
        // of the ship. If north it goes up so the rows decrease etc.
        switch (coords[i])
        {
        case 'N':
            rowIncrement = -1;
            colIncrement = 0;
            break;

        case 'S':
            rowIncrement = 1;
            colIncrement = 0;
            break;

        case 'E':
            rowIncrement = 0;
            colIncrement = 1;
            break;
        case 'W':
            rowIncrement = 0;
            colIncrement = -1;
            break;
        }
        // Iterate through the ship's cells for its length using the increments
        // defined above, and check to make sure the full length of the ship is
        // in the bounds of the board else call bounds_error():
        for (int j = 0; j < length; j++)
        {
            if (row > rows || row <= 0)
            {
                bounds_error(plr);
            }
            if (col >= cols || col < 0)
            {
                bounds_error(plr);
            }
            row += rowIncrement;
            col += colIncrement;
        }
    }
}

int main(int argc, char *argv[])
{

    char **boardP, **boardC, **boardH;

    int *shipLengths = malloc(sizeof(int) * 100);

    int *values;

    int width, height;

    char *extractedPCoords = malloc(150 * sizeof(char));

    char *extractedCCoords = malloc(150 * sizeof(char));

    char *extractedPStartsCol = malloc(150 * sizeof(char));

    char *extractedCStartsCol = malloc(150 * sizeof(char));

    int *extractedPStartsRow = malloc(150 * sizeof(int));

    int *extractedCStartsRow = malloc(150 * sizeof(int));

    // Check if not enough input files:
    if (argc < 5)
    {
        fprintf(stderr, "Usage: naval rules playermap cpumap turns\n");
        exit(10);
    }

    FILE *rules = fopen(argv[1], "r");
    // Check if rules file is unreadable:
    if (rules == 0)
    {
        fprintf(stderr, "Missing rules file\n");
        exit(20);
    }

    values = rule_reader(rules);

    width = values[0];
    height = values[1];
    int count = 0;
    for (int i = 0; i < 15; i++)
    {

        shipLengths[i] = values[i + 3];

        if (values[i + 3] >= 1)
        {
            count++;
        }
    }
    fclose(rules);

    if (count != values[2])
    {
        fprintf(stderr, "Error in rules file\n");
        exit(50);
    }

    FILE *pMap = fopen(argv[2], "r");
    // Check if player map file unreadable:
    if (pMap == 0)
    {
        fprintf(stderr, "Missing player map file\n");
        exit(30);
    }

    map_reader(pMap, extractedPCoords, extractedPStartsCol,
               extractedPStartsRow, "player");

    fclose(pMap);

    FILE *cMap = fopen(argv[3], "r");
    // Check if cpu map file unreadable:
    if (cMap == 0)
    {
        fprintf(stderr, "Missing CPU map file\n");
        exit(31);
    }

    map_reader(cMap, extractedCCoords, extractedCStartsCol,
               extractedCStartsRow, "CPU");

    fclose(cMap);

    boardC = init_board(height, width);
    boardH = init_board(height, width);
    boardP = init_board(height, width);

    check_bounds("player", height, width, extractedPStartsRow,
                 extractedPCoords, extractedPStartsCol, shipLengths);

    check_bounds("CPU", height, width, extractedCStartsRow,
                 extractedCCoords, extractedCStartsCol, shipLengths);

    bool overlap;

    overlap = draw_ships(extractedPCoords, extractedPStartsCol,
                         extractedPStartsRow, boardP, shipLengths);
    // Through overlap error if draw_ships returns true:
    if (overlap)
    {
        fprintf(stderr, "Overlap in player map file\n");
        exit(60);
    }

    overlap = draw_ships(extractedCCoords, extractedCStartsCol,
                         extractedCStartsRow, boardH, shipLengths);

    if (overlap)
    {
        fprintf(stderr, "Overlap in CPU map file\n");
        exit(70);
    }

    FILE *cGuesses = fopen(argv[4], "r");
    // Check if cpu turns file is unreadable:
    if (cGuesses == 0)
    {
        fprintf(stderr, "Missing CPU turns file\n");
        exit(40);
    }

    // Main loop for the game. Runs in the following order:
    // 1. Draw boards
    // 2. Take player input and check if player sinks ship
    // 3. Take cpu input and check if cpu sinks ship
    // 4. Check if the game is now over because of the player or cpu's turn.
    // 5. If game isn't over repeat.
    while (1)
    {

        draw_boards(boardC, true, height, width);
        draw_boards(boardP, false, height, width);

        int ship;
        ship = player_guess(boardC, boardH, height);

        if (ship != 0)
        {
            check_sunk(boardH, width, height, ship);
        }

        if (check_end(boardH, boardP, height, width) != 0)
        {
            break;
        }

        ship = cpu_guess(boardP, cGuesses, height);

        if (ship != 0)
        {
            check_sunk(boardP, width, height, ship);
        }

        if (check_end(boardH, boardP, height, width) != 0)
        {
            break;
        }
        fflush(NULL);
    }

    // If the above loop breaks check which player won the game:
    if (check_end(boardH, boardP, height, width) == 1)
    {

        printf("Game over - you win\n");
    }

    if (check_end(boardH, boardP, height, width) == 2)
    {
        printf("Game over - you lose\n");
    }

    fclose(cGuesses);

    // Free each row of the boards:
    for (int i = 0; i < height; i++)
    {
        free(boardP[i]);
        free(boardC[i]);
        free(boardH[i]);
    }

    // Free all malloc arrays initialised in main:
    free(boardP);
    free(boardC);
    free(boardH);
    free(shipLengths);
    free(extractedPCoords);
    free(extractedCCoords);
    free(extractedPStartsCol);
    free(extractedCStartsCol);
    free(extractedPStartsRow);
    free(extractedCStartsRow);

    return 0;
}
