#ifndef IO_C
#define IO_C

#include "headers.h"

void loadFont()
{
    const char *font_file = "C:/Windows/Fonts/trebuc.ttf";
    UI.font = LoadFontEx(font_file, UI.fontSize, 0, 0);
}

// Returns true if succesfully loaded
bool loadTimes()
{
    char* filename = "times/times.txt";
    
    FILE* file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        printf("Error: Could not open file: %s\n", filename);
        return false;
    }

    ClearTimes(sheet);
    OverwriteStr(&sheet[1].gapStr, "", 0, CELL_TEXT_LENGTH);
    OverwriteStr(&sheet[2].gapStr, "", 0, CELL_TEXT_LENGTH);

    int c = 0;
    
    for (size_t i = 1; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 2) || (i % 3 == 1)) {
            while (c != EOF) {
                c = fgetc(file_ptr);
                if (c == '\n') break;
                placeChar(&sheet[i].gapStr, (char)c);
            }
        }
    }
    fclose(file_ptr);
    return true;
}

char** loadLevelText(int game)
{
    char *filename;
    if (game == LEVELS_GE) filename = "resources/levels-ge.txt";
    if (game == LEVELS_PD) filename = "resources/levels-pd.txt";
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    char **levelText = malloc(sizeof(char *) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        levelText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        fgets(levelText[i], CELL_TEXT_LENGTH, file_ptr);
        memset(strchr(levelText[i], '\n'), 0, 1);
    }

    fclose(file_ptr);
    return levelText;
}

// Loads dnf/veto variations
void loadSpecialText()
{
    FILE *file_ptr = fopen("resources/specials.txt", "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", "specials.txt");
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    size_t specialCount = 0;
    char *tmp = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    while (fgets(tmp, CELL_TEXT_LENGTH, file_ptr) != NULL) 
        specialCount++;
    free(tmp);
    rewind(file_ptr);
    char **specialText = malloc(sizeof(char *) * specialCount);
    for (size_t i = 0; i < specialCount; i++) {
        specialText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        fgets(specialText[i], CELL_TEXT_LENGTH, file_ptr);
        memset(strchr(specialText[i], '\n'), 0, 1);
    }
    GVARS.specials.text = specialText;
    GVARS.specials.count = specialCount;
}

void saveTimes()
{
    MakeDirectory("times");
    char* filename = "times/times.txt";

    FILE* file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t i = 1; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 2) || (i % 3 == 1)) {
            if (strlen(sheet[i].gapStr.str) > 0) {
                fprintf(file_ptr, "%s\n", gapStrToStr(sheet[i].gapStr, CELL_TEXT_LENGTH));
            } else {
                fprintf(file_ptr, "\n");
            }
        }
    }
    fclose(file_ptr);
}

void ExportToBBCode()
{
    MakeDirectory("times");
    char* bbFileName = "times/results.bbcode";

    FILE *out_bb = fopen(bbFileName, "w");
    if (bbFileName == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", bbFileName);
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    char* cellText[CELL_COUNT];
    char* colorText[CELL_COUNT];

    for (size_t i = 0; i < CELL_COUNT; i++) {
        cellText[i] = malloc(CELL_COUNT * sizeof(char));
        colorText[i] = malloc(10 * sizeof(char));
        cellText[i] = gapStrToStr(sheet[i].gapStr, CELL_TEXT_LENGTH);
        if (i < 3) {
            colorText[i] = ColorToHexText(sheet[i].color);
        } else {
            if (ColorIsEqual(sheet[i].highlight, COLOR_WIN)) {
                colorText[i] = "limegreen";
            } else if (ColorIsEqual(sheet[i].highlight, COLOR_LOSE)) {
                colorText[i] = "red";
            } else {
                colorText[i] = "white";
            }
            if (GVARS.vetoFlag) {
                if (CompareSpecialText(gapStrToStr(sheet[i].gapStr, CELL_TEXT_LENGTH)) == TEXT_VETO) {
                    colorText[i] = ColorToHexText(COLOR_LEVEL);
                }
            }
        }
    }

    fprintf(out_bb, // Player 1
        "[size=6][color=%s]%s[/color] VS [color=", 
        colorText[1], 
        cellText[1]
    );
    fprintf(out_bb, // Player 2
        "%s]%s[/color] results:[/size]\n",
        colorText[2],
        cellText[2]
    );
    fprintf(out_bb, 
        "[spoiler]\n"
        "[table][tr][td][size=4][color=white]Level[/color]\n"
        "[/size][/td]\n"
    );
    fprintf(out_bb, // Player 1
        "[td][size=4][color=%s]%s[/color]\n"
        "[/size][/td]\n",
        colorText[1],
        cellText[1]
    );
    fprintf(out_bb, // Player 2
        "[td][size=4][color=%s]%s[/color]\n"
        "[/size][/td]\n",
        colorText[2],
        cellText[2]
    );
    fprintf(out_bb,
        "[/tr]\n"
        "[tr][td][size=4][color=#fc0]Dam\n"
        "Facility\n"
        "Runway\n"
        "Surface 1\n"
        "Bunker 1\n"
        "Silo\n"
        "Frigate\n"
        "Surface 2\n"
        "Bunker 2\n"
        "Statue\n"
        "Archives\n"
        "Streets\n"
        "Depot\n"
        "Train\n"
        "Jungle\n"
        "Control\n"
        "Caverns\n"
        "Cradle"
    );
    if (strlen(sheet[CELL_COUNT - 8].gapStr.str) > 0) {
        fprintf(out_bb,
            "\n"
            "Aztec"
        );
    }
    if (strlen(sheet[CELL_COUNT - 5].gapStr.str) > 0) {
        fprintf(out_bb,
            "\n"
            "Egypt"
        );
    }
    
    fprintf(out_bb,
        "[/color]\n"
        "[/size][/td]\n"
        "[td][size=4]"
    );
    for (size_t i = 4; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 1))
        fprintf(out_bb, // Player 1 times
            "[color=%s]%s[/color]\n",
            colorText[i],
            cellText[i]
        );
    }
    fprintf(out_bb,
        "[/size][/td]\n"
        "[td][size=4]"
    );
    for (size_t i = 5; i < CELL_COUNT - 3; i++) {
        if ((i % 3 == 2))
        fprintf(out_bb, // Player 2 times
            "[color=%s]%s[/color]\n",
            colorText[i],
            cellText[i]
        );
    }
    fprintf(out_bb,
        "[/size][/td]\n"
        "[/tr]\n"
        "[/table]\n"
    );
    fprintf(out_bb, // Results
        "[size=14pt][b]%s - %s[/b][/size] Win at %s\n\n",
        cellText[CELL_COUNT - 2],
        cellText[CELL_COUNT - 1],
        GVARS.level_win
    );
    fprintf(out_bb,
        "Put Twitch link here\n"
    );
    fprintf(out_bb,
        "[/spoiler]"
    );

    fclose(out_bb);

    out_bb = fopen(bbFileName, "r");

    fseek(out_bb, 0, SEEK_END);
    long out_len = ftell(out_bb);
    char* copy_bb = malloc(out_len);
    fseek(out_bb, 0, SEEK_SET);
    if (copy_bb) {
        fread(copy_bb, 1, out_len, out_bb);
    }
    SetClipboardText(copy_bb);

    fclose(out_bb);
}

void ExportActionTable()
{
    char* filename = "ACTIONS.md";
    FILE* file_ptr = fopen(filename, "w");
    if (file_ptr == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    fprintf(file_ptr, "# Generated from %s in %s() at Line %d\n\n", __FILE__, __func__, __LINE__);
    fprintf(file_ptr, "|                KEY COMBO |                     OVERVIEW |                        SHEET |                         CELL |\n");
    fprintf(file_ptr, "| ------------------------ | ---------------------------- | ---------------------------- | ---------------------------- |\n");
    for (size_t y = 0; y < KEY_COMBO_COUNT; y++) {
        if (
            (ActionTable[0][y] == A_DONOTHING) && 
            (ActionTable[1][y] == A_DONOTHING) && 
            (ActionTable[2][y] == A_DONOTHING)
        ) continue;
        fprintf(file_ptr, "| %24s ", GetKeyComboText(y));
        for (size_t x = 0; x < 3; x++) {
            fprintf(file_ptr, "| %28s ", GetHumanReadableActionText(ActionTable[x][y]));
        }
        fprintf(file_ptr, "|\n");
    }
    fclose(file_ptr);
}

#endif