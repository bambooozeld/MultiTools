#include <stdio.h>
#include <windows.h>
#include "Chat.h"


void Chat();
void Init();
void ClearScreen();

int main() {
    Init();
    return 0;
}

void Init() {

    char input[10];
    int *ptr_input = malloc(sizeof(int));

    while (1) {
        printf("Multi-Use App\n");
        printf("*************\n");
        printf("1 - Chat App\n");
        printf("2 - File Transfer App\n");
        printf("3 - Placeholder App\n");

        printf("\nPlease choose which of the above programs, you want to run, enter the number: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Handle error or EOF
            break;
        }
        if (input[strlen(input) - 1] != '\n') {
            printf("Input too long. Please try again.\n");
            // Clear the input buffer using fgets
            fgets(input, sizeof(input), stdin);
            continue;
        }
        // Remove the newline character from the input string
        input[strlen(input) - 1] = '\0';
        // Convert the input string to an integer
        char *end_ptr = 0;
        long num = strtol(input, &end_ptr, 10);
        if (num == 1 || num == 2 || num == 3) {
            *ptr_input = (int)num;
            break;
        }
        printf("Invalid input. Please try again.\n");
        printf("(Restarting)...");
        Sleep(2500);
        ClearScreen();
    }

    switch (*ptr_input) {
        case 1:
            Chat();
            break;
        case 2:
        case 3:
            printf("\nCurrently under development, retry later!");
            printf("\nRestarting....");
            Sleep(2500);
            ClearScreen();
            Init();
            break;
        default:
            break;
    }
}
void Chat(){
    ChatMain();
}


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
void ClearScreen()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR) ' ',
            cellCount,
            homeCoords,
            &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition( hStdOut, homeCoords );
}

#else // !_WIN32
#include <unistd.h>
#include <term.h>

void ClearScreen()
{
  if (!cur_term)
  {
     int result;
     setupterm( NULL, STDOUT_FILENO, &result );
     if (result <= 0) return;
  }

   putp( tigetstr( "clear" ) );
}
#endif
