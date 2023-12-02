#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


#define ROW 18
#define COL 19
#define BUFFER_SIZE 10 // 버퍼 크기

typedef struct {
    int row;    // 세로 위치 (위, 아래)
    int col;    // 가로 위치 (좌, 우)
} location;
    const char* wall = " ■ ";
    const char* road = " □ ";
    const char* road2 = "ㅁ ";
    const char* user = " ♀ ";
    const char* end = " ♥ ";
    const char* GO = " ★ ";
    
location user_location; // 유저 위치를 저장하는 변수
//맵 생성
void maze_print(const char* maze[ROW][COL]);
void readFromArduino(const char* maze[ROW][COL]); // 전방 선언
void maze_print_fog(const char* maze[ROW][COL], int fog);
void maze_print_GO(const char* maze[ROW][COL]) ;
void print_reset();
void print_ending();

int main(void) 
{   
    system("cls");
    location user_location = {2, 2};
 
    const char *maze[ROW][COL] = 
    {
        {wall,road2,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall},
        {wall,road2,wall,wall,road,road,road,road,road,wall,road,road,road2,road2,road2,road2,road2,road2,wall},
        {wall,road2,wall,wall,road,wall,wall,wall,wall,wall,wall,wall,road2,wall,wall,wall,wall,road2,wall},
        {wall,road2,wall,road,road,road,road,road,road,wall,road,road,road2,road2,road,wall,wall,road2,wall},
        {wall,road2,wall,road,wall,wall,wall,wall,road,wall,wall,wall,wall,road2,wall,wall,road,road2,wall},
        {wall,road2,road2,road2,road2,road2,road2,road2,road2,wall,road2,road2,road2,road2,wall,road,road,road2,wall},
        {wall,wall,wall,wall,road,wall,wall,wall,road2,road2,road2,wall,wall,wall,wall,wall,wall,road2,wall},
        {wall,road,road,road,road,road,road,wall,wall,wall,wall,road2,road2,road2,road2,road2,road2,road2,wall},
        {wall,road,wall,wall,wall,wall,road,wall,road2,road2,road2,road2,wall,wall,wall,wall,road,wall,wall},
        {wall,road,wall,wall,wall,wall,road,wall,road2,wall,wall,wall,wall,road,road,wall,road,wall,wall},
        {wall,road,road,road,road,road,road,wall,road2,road,road,road,wall,road,road,wall,road,wall,wall},
        {wall,road,wall,wall,wall,wall,road,wall,road2,wall,wall,road,wall,wall,road,wall,road,wall,wall},        
        {wall,road,road,road,wall,road,road,wall,road2,road,road,road,wall,road,road,wall,road,wall,wall},
        {wall,wall,wall,road,wall,wall,road,wall,road2,wall,wall,road,wall,road,road,road,road,wall,wall},
        {wall,road,road,road,wall,wall,road,wall,road2,road,road,wall,wall,wall,wall,wall,wall,wall,wall},
         {wall,road,road,road,wall,road,road,road,road2,road2,road2,road2,road2,road2,road2,road2,road2,road2,wall},
        {wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,wall,end,wall},
        {" ","@","@","@","@","@","@"," 노랑 = 왼쪽 " ,"/ "," 흰색 = 오른쪽 " ,"/ "," 빨강 = 위 ","/ "," 파랑 = 아래 " ,"@","@","@","@","@"},
    };


    print_reset();
    maze_print_GO(maze);
    maze_print_fog(maze,3); 

    readFromArduino(maze);
    return 0;
}

void maze_print(const char* maze[ROW][COL]) 
{
    system("cls"); // 콘솔 화면 지우기
    for (int i = 0; i < ROW; i++) 
    {
        for (int j = 0; j < COL; j++) 
        {
            if (i == user_location.row && j == user_location.col) 
            {
                printf("%s", user); // 유저 위치에 유저 모양 출력
            }
            else if (maze[i][j] == GO)
            {
                printf("%s",road);
            } 
            else 
            {
                printf("%s", maze[i][j]); // 나머지 위치에는 맵 데이터 출력
            }
        }
        printf("\n");
    }
    Sleep(3000);
    maze_print_fog(maze,4); 
}





void readFromArduino(const char* maze[ROW][COL]) {
    HANDLE hSerial;
    // 시리얼 포트 핸들을 엽니다.
    hSerial = CreateFile("COM4", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            // 시리얼 포트를 찾을 수 없습니다.
            printf("시리얼 포트를 찾을 수 없습니다.\n");
        }
        printf("오류 발생\n");
    }
    DWORD bytesRead;
    char buffer[BUFFER_SIZE];
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600; // 시리얼 통신 속도 설정 (9600 bps)
    dcbSerialParams.ByteSize = 8;        // 데이터 비트 설정
    dcbSerialParams.StopBits = ONESTOPBIT;// 정지 비트 설정
    dcbSerialParams.Parity = NOPARITY;   // 패리티 설정

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        // 시리얼 포트 설정을 변경할 수 없습니다.
        printf("시리얼 포트 설정을 변경할 수 없습니다.\n");
        CloseHandle(hSerial);
        return;
    }

      while (1) {
        if (ReadFile(hSerial, buffer, BUFFER_SIZE - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                for (DWORD i = 0; i < bytesRead; ++i) {
                    if (buffer[i] > '1' && buffer[i] <= '5') {
                        printf("들어온값:%c\n", buffer[i]);
                        if (buffer[i] == '2' && user_location.col + 1 < COL && strcmp(maze[user_location.row][user_location.col + 1], wall) != 0) 
                        {
                            // 유저 위치 변경 (오른쪽으로 이동=흰색)
                            user_location.col += 1;
                        }
                        else if (buffer[i] == '4' && user_location.row - 1 >= 0 && strcmp(maze[user_location.row - 1][user_location.col], wall) != 0) 
                        {
                            // 유저 위치 변경 (위쪽으로 이동=빨강)
                            user_location.row -= 1;
                        }
                        else if (buffer[i] == '3' && user_location.row + 1 < ROW && strcmp(maze[user_location.row + 1][user_location.col], wall) != 0) 
                        {
                            // 유저 위치 변경 (아래쪽으로 이동=파랑)
                            user_location.row += 1;
                        }
                          else if (buffer[i] == '5' && user_location.col - 1 >= 0 && strcmp(maze[user_location.row ][user_location.col-1], wall) != 0) 
                        {
                            // 유저 위치 변경 (왼쪽으로 이동=노랑)
                            user_location.col -= 1;
                        }
                        else
                        {
                            const char* data = "1";
                            DWORD bytes_written;
                            WriteFile(hSerial, data, strlen(data), &bytes_written, NULL);
                        }

                    }
                }
                system("cls");
                maze_print_fog(maze,4); // 맵을 새로 출력하여 유저의 새 위치 반영
            }
        }
    }
    
    CloseHandle(hSerial); // 시리얼 포트 닫기
}


void maze_print_fog(const char* maze[ROW][COL], int fog) {
    system("cls"); // 콘솔 화면 지우기
    for (int i = user_location.row - fog; i <= user_location.row + fog; i++) 
    {
        for (int j = user_location.col - fog; j <= user_location.col + fog; j++) 
        {
            if (i >= 0 && i < ROW && j >= 0 && j < COL) 
            {
                if (i == user_location.row && j == user_location.col)
                {
                    if (strcmp(maze[i][j], end) == 0) {
                        print_ending(); // 미로의 끝에 도달했을 때 end 함수 호출
                        return;
                    }
                    printf("\x1b[31m%s\x1b[0m", user);
                } 
                else
                 {
                    printf("%s", maze[i][j]); // 주위 3칸에 해당하는 맵 데이터 출력
                }
            }
           
        }
        printf("\n");
    }
    // 출력을 수정하여 보다 명확하게 나타내기
    printf("초록 = 왼쪽 / 파랑 = 오른쪽 / 노랑 = 위 / 빨강 = 아래\n");
}

void maze_print_GO(const char* maze[ROW][COL]) 
{
    system("cls"); // 콘솔 화면 지우기
    for (int i = 0; i < ROW; i++) 
    {
        for (int j = 0; j < COL; j++) 
        {
            if (i == user_location.row && j == user_location.col) 
            {
               printf("\x1b[31m%s\x1b[0m", user); // 유저 위치에 유저 모양 출력
            }
            else if (strcmp(maze[i][j], road2) == 0)
            {
                printf("%s", GO); // road2 위치에 GO 출력
            }
            else 
            {
                printf("%s", maze[i][j]); // 나머지 위치에는 맵 데이터 출력
            }
        }
        printf("\n");
        Sleep(100);
    }
    
    printf("별을 따라 가세요!!");
    Sleep(3000);
    maze_print_fog(maze, 3); 
}

void print_reset()
{

    system("cls"); 
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("길을 알려줄게요!!");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("별을 따라 가면 됩니다.!!");
    Sleep(3000);
}
void print_ending()
{
system("cls"); 
printf("★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ \n");
printf("\n");
printf( "           미로를 찾았습니다 !!\n");
printf("\n");
printf("★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ ★ ☆ \n");
exit(0);

}