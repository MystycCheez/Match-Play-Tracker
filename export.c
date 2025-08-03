#include "includes.h"

char* ColorToHexText(Color color)
{
    char* colorText = malloc(7);
    snprintf(colorText + 0, 3, "%02x", color.r);
    snprintf(colorText + 2, 3, "%02x", color.g);
    snprintf(colorText + 4, 3, "%02x", color.b);
    return colorText;
}

void ExportToBBCode(Cell* sheet)
{
    FILE *TestFile = fopen("TestFile.txt", "w");
    FILE *out_bb = fopen("out.bbcode", "w");

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
                colorText[i] = "red";
            } else if (ColorIsEqual(sheet[i].highlight, COLOR_LOSE)) {
                colorText[i] = "limegreen";
            } else {
                colorText[i] = "white";
            }
        }
        fprintf(TestFile, "Cell Text: %s\n", cellText[i]);
        fprintf(TestFile, "Color Text: %s\n", colorText[i]);
        if (i % 3 == 2) {
            fprintf(TestFile, "\n");
        }
    }

    fprintf(out_bb, // Player 1
        "[size=6][color=#%s]%s[/color] VS [color=#", 
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
        "[td][size=4][color=#%s]%s[/color]\n"
        "[/size][/td]\n",
        colorText[1],
        cellText[1]
    );
    fprintf(out_bb, // Player 2
        "[td][size=4][color=#%s]%s[/color]\n"
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
        "[size=14pt][b]%s - %s[/b][/size] Win at %s",
        cellText[CELL_COUNT - 1],
        cellText[CELL_COUNT - 2],
        "Who Knows?\n\n"
    );
    fprintf(out_bb,
        "Put Twitch link here\n"
    );
    fprintf(out_bb,
        "[/spoiler]"
    );

    fclose(TestFile);
    fclose(out_bb);
}