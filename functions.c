#include "includes.h"

Font initFont()
{
    const char *font_file = "C:/Windows/Fonts/trebucbd.ttf";
    int fileSize = 0;
    unsigned char *fileData = LoadFileData(font_file, &fileSize);
    if (fileData == NULL) {
        fprintf(stderr, "Failed to load font: %s!\n", font_file);
        exit(1);
    }
    Font font = {0};
    font.baseSize = GVARS.fontSize * 5;
    font.glyphCount = 95;
    font.glyphs = LoadFontData(fileData, fileSize, GVARS.fontSize * 5, 0, 0, FONT_SDF);
    Image atlas = GenImageFontAtlas(font.glyphs, &font.recs, font.glyphCount, GVARS.fontSize * 5, 0, 1);
    font.texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);
    // SetTextureFilter(font.texture, TEXTURE_FILTER_POINT); // Will I need this?
    return font;
}

void reInitGVARS()
{
    GVARS.screenHeight = GetScreenHeight();
    GVARS.screenWidth = GetScreenWidth();
    GVARS.cellHeight = GVARS.screenHeight / ROWS;
    GVARS.cellWidth = GVARS.screenWidth / COLUMNS;
    GVARS.fontSize = GVARS.screenHeight / 44;
}

void initButtons()
{
    GVARS.buttons = malloc(sizeof(Button) * 2);
    GVARS.buttons[BTN_EXIT].pos = (Vector2){GVARS.screenWidth - BUTTON_SIZE - (BUTTON_SIZE / 2), (TOP_BAR_HEIGHT / 2) - 13};
    GVARS.buttons[BTN_EXIT].texture = LoadTextureFromImage(LoadImage("resources/x.png"));
    GVARS.buttons[BTN_EXIT].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(GVARS.buttons[BTN_EXIT].texture, TEXTURE_FILTER_BILINEAR);
    GVARS.buttons[BTN_MINIMIZE].pos = (Vector2){GVARS.screenWidth - (BUTTON_SIZE * 3), (TOP_BAR_HEIGHT / 2) - 13};
    GVARS.buttons[BTN_MINIMIZE].texture = LoadTextureFromImage(LoadImage("resources/minimize.png"));
    GVARS.buttons[BTN_MINIMIZE].state = STATE_BTN_UNHIGHLIGHTED;
    SetTextureFilter(GVARS.buttons[BTN_MINIMIZE].texture, TEXTURE_FILTER_BILINEAR);
}

char **loadLevelText(int game)
{
    char* filename;
    if (game == LEVELS_GE) filename = "resources/levels-ge.txt";
    if (game == LEVELS_PD) filename = "resources/levels-pd.txt";
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Error: Could not open file: %s\n", filename);
        fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    char **levelText = malloc(sizeof(char*) * LEVEL_COUNT);
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
    char* tmp = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    while (fgets(tmp, CELL_TEXT_LENGTH, file_ptr) != NULL) specialCount++;
    free(tmp);
    rewind(file_ptr);
    char **specialText = malloc(sizeof(char*) * specialCount);
    for (size_t i = 0; i < specialCount; i++) {
        specialText[i] = malloc(sizeof(char) * CELL_TEXT_LENGTH);
        fgets(specialText[i], CELL_TEXT_LENGTH, file_ptr);
        memset(strchr(specialText[i], '\n'), 0, 1);
    }
    GVARS.specials.text = specialText;
    GVARS.specials.count = specialCount;
}

Color getStateColor(State_Button state)
{
    if (state == STATE_BTN_PRESSED) return GRAY;
    if (state == STATE_BTN_HIGHLIGHTED) return LIGHTGRAY;
    return WHITE;
}

// Returns TEXT_VETO or TEXT_DNF if found
// Returns TEXT_NA if special text not found
Text_Type CompareSpecialText(char* text)
{
    Text_Type returnVal = TEXT_NA;
    for (size_t i = 0; i < GVARS.specials.count; i++) {
        if (strcmp(text, GVARS.specials.text[i]) == 0) {
            if (i < 4) returnVal = TEXT_VETO;
            if (i > 3)           returnVal = TEXT_DNF;
        } else if (strcmp(text, GVARS.specials.text[i]) == 0) return returnVal;
    }
    return returnVal;
}

// XY being the top left corner of the cell index
Vector2 indexToXY(size_t index)
{
    Vector2 xy = {0};
    xy.x = (index % COLUMNS) * GVARS.cellWidth;
    xy.y = (index / COLUMNS) * GVARS.cellHeight + TOP_BAR_HEIGHT;
    return xy;
}

// CR being column/row
Vector2 indexToCR(size_t index)
{
    Vector2 cr = {0};
    cr.x = index % COLUMNS;
    cr.y = index / COLUMNS;
    return cr;
}

// XY being anywhere within a cell
size_t xyToIndex(Vector2 xy)
{
    size_t index = 0;
    xy.y = xy.y - TOP_BAR_HEIGHT;
    Vector2 rounded = {xy.x - fmodf(xy.x, GVARS.cellWidth), xy.y - fmodf(xy.y, GVARS.cellHeight)};
    rounded.x = rounded.x / GVARS.cellWidth;
    rounded.y = rounded.y / GVARS.cellHeight;
    index = rounded.x + (rounded.y * COLUMNS);
    return index;
}

// CR being column/row
size_t crToIndex(Vector2 cr)
{
    return (size_t)(cr.x + (cr.y * COLUMNS));
}

Rectangle getButtonRect(Button button)
{
    return (Rectangle){button.pos.x, button.pos.y, BUTTON_SIZE, BUTTON_SIZE};
}

void initSheetText(Cell *sheet, Players players, int game)
{
    char **levelText = loadLevelText(game);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        placeString(&sheet[(i * 3) + 3].gapStr, levelText[i]);
    }
    free(levelText);
    char *s1 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    char *s2 = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(s1, "%lld", players.s1);
    sprintf(s2, "%lld", players.s2);
    placeString(&sheet[0].gapStr, "Stage");
    placeString(&sheet[CELL_COUNT - 3].gapStr, "Points");
    placeString(&sheet[1].gapStr, players.p1);
    placeString(&sheet[2].gapStr, players.p2);
    placeString(&sheet[CELL_COUNT - 2].gapStr, s1);
    placeString(&sheet[CELL_COUNT - 1].gapStr, s2);
}

Cell* initSheet(Players players, int game)
{
    Cell *sheet = malloc(sizeof(Cell) * CELL_COUNT);
    for (size_t i = 0; i < CELL_COUNT; i++) {
        sheet[i].gapStr = initGapStr(CELL_TEXT_LENGTH);
        sheet[i].alignment = ALIGN_CENTER;
        sheet[i].color = WHITE;
        sheet[i].highlight = TRANSPARENT;
        sheet[i].selectable = true;
    }
    sheet[0].selectable = false;
    sheet[CELL_COUNT - 1].selectable = false;
    sheet[CELL_COUNT - 2].selectable = false;
    sheet[CELL_COUNT - 3].selectable = false;
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        sheet[3 + (i * COLUMNS)].alignment = ALIGN_LEFT;
        sheet[3 + (i * COLUMNS)].color = COLOR_LEVEL;
        sheet[3 + (i * COLUMNS)].selectable = false;
    }
    initSheetText(sheet, players, game);
    return sheet;
}

void setBorderPositions(Line *borders)
{
    size_t index = 0;
    for (size_t i = 0; i < ROWS; i++) {
        borders[i] = (Line){0, (GVARS.cellHeight * i) + TOP_BAR_HEIGHT, GVARS.screenWidth, (GVARS.cellHeight * i) + TOP_BAR_HEIGHT};
        index++;
    }
    for (size_t i = 0; i < COLUMNS; i++) {
        borders[i + index] = (Line){GVARS.cellWidth * i, TOP_BAR_HEIGHT, GVARS.cellWidth * i, GVARS.screenHeight};
    }
}

void DrawCellBorders(size_t cellIndex)
{
    if (cellIndex == 0) return;
    Vector2 cellOrigin = indexToXY(cellIndex);
    DrawRectangleLinesEx((Rectangle){cellOrigin.x, cellOrigin.y, GVARS.cellWidth, GVARS.cellHeight}, 3.0, RAYWHITE);
}

// TODO: check if this can utilize xy/cr functions
Vector2 CalcTextPos(Vector2 pos, size_t index)
{
    pos.x = pos.x + (GVARS.cellWidth * (index % 3));
    pos.y = 1 + pos.y + (GVARS.cellHeight * (index / 3) + TOP_BAR_HEIGHT);
    return pos;
}

void DrawCursor(Cell *sheet, size_t cellIndex, Font font)
{
    if (cellIndex == 0) return;
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    float span = MeasureTextEx(font, gapStrToStr(sheet[cellIndex].gapStr, CELL_TEXT_LENGTH), GVARS.fontSize, 1).x;
    float offset = MeasureTextEx(font, sheet[cellIndex].gapStr.str, GVARS.fontSize, 1).x;
    pos.x += (GVARS.cellWidth / 2) - (span / 2) + offset + 1;
    DrawLineEx(pos, (Vector2){pos.x, pos.y + GVARS.cellHeight}, 1.0, RED);
}

void DrawTextCentered(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + (GVARS.cellWidth / 2) - (size.x / 2);
    pos.y = pos.y + (GVARS.cellHeight / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
}

void DrawTextLeftAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell)
{
    char *text = gapStrToStr(cell.gapStr, CELL_TEXT_LENGTH);
    Vector2 size = MeasureTextEx(font, text, fontSize, spacing);

    pos.x = pos.x + fontSize / 2;
    pos.y = pos.y + (GVARS.cellHeight / 2) - (size.y / 2);

    DrawTextEx(font, text, pos, fontSize, spacing, cell.color);
}

void DrawTextAligned(Font font, Vector2 pos, float fontSize, float spacing, Cell cell, size_t i)
{
    switch (cell.alignment) {
    case ALIGN_LEFT:
        DrawTextLeftAligned(font, CalcTextPos(pos, i), fontSize, spacing, cell);
        break;
    case ALIGN_CENTER:
        DrawTextCentered(font, CalcTextPos(pos, i), fontSize, spacing, cell);
        break;
    case ALIGN_RIGHT:
        assert(!"TODO: ALIGN_RIGHT");
        break;
    }
}

void DrawTextHighlight(Cell *sheet, size_t cellIndex, Selection selection, Font font)
{
    Vector2 pos = {0};
    pos = CalcTextPos(pos, cellIndex);
    size_t selectionLen = selection.end - selection.start;
    char *selectedText = gapStrToStr(sheet[cellIndex].gapStr, CELL_TEXT_LENGTH) + selection.start;
    selectedText[selectionLen] = 0;
    float selectionSpan = MeasureTextEx(font, selectedText, GVARS.fontSize, 1).x;
    
    float cellTextSpan = MeasureTextEx(font, gapStrToStr(sheet[cellIndex].gapStr, CELL_TEXT_LENGTH), GVARS.fontSize, 1).x;
    float offset = MeasureTextEx(font, gapStrToStr(sheet[cellIndex].gapStr, selection.start), GVARS.fontSize, 1).x;

    pos.x += (GVARS.cellWidth / 2) - (cellTextSpan / 2) + offset;
    DrawRectangleRec((Rectangle){pos.x, pos.y, selectionSpan + 2, GVARS.cellHeight}, COLOR_HIGHLIGHT);
}

// Expects format: "mm:ss"
size_t timeToSecs(char *time)
{
    size_t timeLen = strlen(time);
    assert(timeLen < 6 && "use filterCellText");
    if (timeLen < 3) {
        return atoi(time);
    } else {
        char *a = malloc(sizeof(char) * timeLen - 2);
        char *b = malloc(sizeof(char) * 2);
        strncpy(a, time, timeLen - 2);
        strcpy(b, time + timeLen - 2);
        size_t minutes = atoi(a);
        size_t seconds = atoi(b);
        return seconds + (minutes * 60);
    }
}

// Formats to mm:ss or 0:ss / Expects total number of seconds
char *secsToTime(size_t totalSecs)
{
    if (totalSecs == 0) return "\0";
    char *time = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    memset(time, 0, CELL_TEXT_LENGTH);
    size_t minutes = totalSecs / 60 < 100 ? totalSecs / 60 : 59;
    size_t secs = totalSecs % 60;
    sprintf(time, "%lld:%02lld", minutes, secs);
    return time;
}

// Count how many times char c appears in text limited by len
size_t countChars(char *text, char c, size_t len)
{
    size_t count = 0;
    char *p;
    p = memchr(text, c, len);
    while (p != NULL) {
        p = memchr(p + 1, c, len);
        count++;
    }
    return count;
}

// Filters string to be converted into time / Outputs "mm:ss" or "m:ss"
char* filterText(char* text)
{
    // Do these need to be static?
    static char* dummy = "\0";
    static char* text_veto = "Veto";
    static char* text_dnf = "DNF";

    char *filtered = malloc(sizeof(char) * 5);

    int special = CompareSpecialText(text);
    if (special == TEXT_VETO) return text_veto;
    if (special == TEXT_DNF)  return text_dnf;
    
    size_t textLen = strlen(text);

    if (strpbrk(text, ":1234567890") == NULL) return dummy;

    size_t cCount = countChars(text, ':', textLen);
    if (cCount > 2) return dummy;
    switch (cCount) {
    case 0:
        switch (textLen) {
        case 1: // x
            sprintf(filtered, "0:0%s", text);
            break;
        case 2: // xx
            sprintf(filtered, "0:%s", text);
            break;
        case 3: // xxx
            sprintf(filtered, "%c:%s", text[0], text + 1);
            break;
        case 4: // xxxx
            if (text[0] != '0') {
                sprintf(filtered, "%c%c:%s", text[0], text[1], text + 2);
            } else sprintf(filtered, "%c:%s", text[1], text + 2);
            break;
        default:
            return dummy;
        }
        return secsToTime(timeToSecs(filtered));
    case 1:
        if (text[textLen - 3] != ':') return dummy;
        switch (textLen) {
        case 3: // :xx
            sprintf(filtered, "0%s", text);
            break;
        case 4: // x:xx
            sprintf(filtered, "%s", text);
            break;
        case 5: // xx:xx
            sprintf(filtered, "%s", text);
            break;
        default:
            return dummy;
        }
        return secsToTime(timeToSecs(filtered));
    case 2: // TODO:
        if (text[textLen - 6] != ':') return dummy;
        if (!(textLen <= 8 && textLen >= 6)) return dummy;
        sprintf(filtered, "%s", text + textLen - 5);
        // TODO: Is this a memory leak?
        return filterText(filtered);
    default:
        return dummy;
    }    
}

// Compares two cells in a row and returns a pair of ints representing who won or lost
// Also checks for special cases
Int2 CompareTimes(size_t row, Cell *cells)
{
    size_t cellL = crToIndex((Vector2){1, (float)row});
    size_t cellR = crToIndex((Vector2){2, (float)row});
    
    Text_Type specialL = CompareSpecialText(cells[cellL].gapStr.str);
    Text_Type specialR = CompareSpecialText(cells[cellR].gapStr.str);

    size_t timeL = -1;
    size_t timeR = -1;
    if (specialL == -1) timeL = timeToSecs(cells[cellL].gapStr.str);
    if (specialR == -1) timeR = timeToSecs(cells[cellR].gapStr.str);

    if (cells[cellL].gapStr.str[0] == 0 || cells[cellR].gapStr.str[0] == 0 || specialL == TEXT_VETO || specialR == TEXT_VETO) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){-1, -1}; // Indicates Veto
    }
    if ((timeL == timeR) || (specialL == TEXT_DNF && specialR == TEXT_DNF)) {
        cells[cellL].highlight = TRANSPARENT;
        cells[cellR].highlight = TRANSPARENT;
        return (Int2){0, 0}; // Indicates Tie
    } 
    if ((timeL < timeR) || (specialR == TEXT_DNF)) { // Why doesn't this work????
        cells[cellL].highlight = COLOR_WIN;
        cells[cellR].highlight = COLOR_LOSE;
        return (Int2){1, 0};
    }
    if ((timeL > timeR) || (specialL == TEXT_DNF)) {
        cells[cellL].highlight = COLOR_LOSE;
        cells[cellR].highlight = COLOR_WIN;
        return (Int2){0, 1};
    }
    // TODO: player gets WR as time and is highlighted GOLD (unlikely, but would like this feature) - UNTIEDS too
    // fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
    fprintf(stderr, "Shouldn't have reached here!");
    exit(-1);
}

// Compares times in each row and calculates the scores, overwriting the text
void UpdateScores(Cell *cells)
{
    size_t tieCounter = 0;
    OverwriteStr(&cells[CELL_COUNT - 2].gapStr, "0", CELL_TEXT_LENGTH);
    OverwriteStr(&cells[CELL_COUNT - 1].gapStr, "0", CELL_TEXT_LENGTH);
    Int2 *wins = malloc(sizeof(Int2) * LEVEL_COUNT);
    for (size_t i = 0; i < LEVEL_COUNT; i++) {
        wins[i] = CompareTimes(i + 1, cells);
        if (wins[i].a == 0 && wins[i].b == 0) {
            tieCounter++;
        }
        if (wins[i].a > 0 && tieCounter > 0) {
            OverwriteStr(&cells[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(cells[CELL_COUNT - 2].gapStr.str) + tieCounter), CELL_TEXT_LENGTH);
            tieCounter = 0;
        } else if (wins[i].b > 0 && tieCounter > 0) {
            OverwriteStr(&cells[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(cells[CELL_COUNT - 1].gapStr.str) + tieCounter), CELL_TEXT_LENGTH);
            tieCounter = 0;
        } else if (wins[i].a > 0 || wins[i].b > 0){
            OverwriteStr(&cells[CELL_COUNT - 2].gapStr, i_toStr(wins[i].a + atoi(cells[CELL_COUNT - 2].gapStr.str)), CELL_TEXT_LENGTH);
            OverwriteStr(&cells[CELL_COUNT - 1].gapStr, i_toStr(wins[i].b + atoi(cells[CELL_COUNT - 1].gapStr.str)), CELL_TEXT_LENGTH);
        }
    }
}

// TODO: Add save reminder dialog
void ExitHandler()
{
    GVARS.shouldExit = true;
}

void MouseTitleBarHandler(CollisionMap Collision, MouseState Mouse, Vector2 mousePos, Vector2 windowPos)
{
    static bool buttonLeft = false;
    static bool windowDrag = false;
    static CollisionMap Drag = {false};
    static Vector2 dragOffset = {0};

    for (size_t i = 0; i < 2; i++) {
        GVARS.buttons[i].state = ((bool*)(&Collision))[i] ? STATE_BTN_HIGHLIGHTED : STATE_BTN_UNHIGHLIGHTED;
    }
    if (Mouse.down) {
        if (Collision.exit && !Drag.titleBar) {
            Drag.titleBar = true;
            GVARS.buttons[BTN_EXIT].state = STATE_BTN_PRESSED;
        }
        if (Collision.minimize && !Drag.minimize) {
            Drag.minimize = true;
            GVARS.buttons[BTN_MINIMIZE].state = STATE_BTN_PRESSED;
        }
        if (!(Collision.exit || Collision.minimize) && (Drag.titleBar || Drag.minimize)) {
            buttonLeft = true;
        } else buttonLeft = false;
    } else {
        Drag.titleBar = Drag.minimize = false;
    }

    if (Mouse.down && !windowDrag && !(Drag.titleBar || Drag.minimize)) {
        if (Collision.titleBar && !(Collision.exit || Collision.minimize)) {
            windowDrag = true;
            dragOffset = mousePos;
        } else windowDrag = false;
    }
    if (windowDrag) {
        windowPos.x += mousePos.x - dragOffset.x;
        windowPos.y += mousePos.y - dragOffset.y;
        SetWindowPosition(windowPos.x, windowPos.y);
        if (!Mouse.down) windowDrag = false;
    }
    if (Mouse.released && !windowDrag && !(Drag.titleBar || Drag.minimize) && !buttonLeft) {
        if (Collision.exit) {
            GVARS.buttons[BTN_EXIT].state = STATE_BTN_PRESSED;
            ExitHandler();
        }
        if (Collision.minimize) {
            GVARS.buttons[BTN_MINIMIZE].state = STATE_BTN_PRESSED;
            MinimizeWindow();
        }
    }
}

void MouseSheetHandler(CollisionMap Collision, Vector2 mousePos, Cell *sheet, size_t *cellIndex, bool *selectionState)
{
    if (Collision.sheet) {
        *cellIndex = xyToIndex(mousePos);
        *selectionState = sheet[*cellIndex].selectable;
        cellIndex = *selectionState ? cellIndex : 0;
    } else {
        *selectionState = 0;
        cellIndex = 0;
    }
}

void MouseHandler(Cell *sheet, size_t *cellIndex, bool *selectionState)
{
    Vector2 mousePos = GetMousePosition();
    Vector2 windowPos = GetWindowPosition();

    MouseState Mouse = {
        IsMouseButtonDown(MOUSE_BUTTON_LEFT),
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)
    };

    CollisionMap Collision = {
        CheckCollisionPointRec(mousePos, getButtonRect(GVARS.buttons[BTN_EXIT])),
        CheckCollisionPointRec(mousePos, getButtonRect(GVARS.buttons[BTN_MINIMIZE])),
        CheckCollisionPointRec(mousePos, (Rectangle){0, 0, GVARS.screenWidth, TOP_BAR_HEIGHT}),
        CheckCollisionPointRec(mousePos, (Rectangle){0, TOP_BAR_HEIGHT, GVARS.screenWidth, DEFAULT_SHEET_HEIGHT})
    };

    MouseTitleBarHandler(Collision, Mouse, mousePos, windowPos);
    if (Mouse.pressed) {
        MouseSheetHandler(Collision, mousePos, sheet, cellIndex, selectionState);
    }
}

void CellInputHandler(Cell *sheet, size_t *cellIndex, bool *selectionState, bool *textChanged, KeyMap key, Selection *selection)
{
    int key_char = GetCharPressed();
    bool tmpSelectState = *selectionState;

    // Check if more characters have been pressed on the same frame
    while (key_char > 0) {
        if ((key_char >= 32) && (key_char <= 125))
            placeChar(&sheet[*cellIndex].gapStr, (char)key_char);
        key_char = GetCharPressed(); // Check next character in the queue
        *textChanged = true;
    }
    if (!key.shift) {
        if (key.ctrl) { // TODO: do it by token
            if (key.left) GapStrGotoIndex(&sheet[*cellIndex].gapStr, 0);
            if (key.right) GapStrGotoIndex(&sheet[*cellIndex].gapStr, strlen(gapStrToStr(sheet[*cellIndex].gapStr, CELL_TEXT_LENGTH)));
            Deselect(selection);
        } else if (!key.ctrl) {
            if (key.left) if (cursorLeft(&sheet[*cellIndex].gapStr)) Deselect(selection);
            if (key.right) if (cursorRight(&sheet[*cellIndex].gapStr)) Deselect(selection);
        }
        if (IsKeyPressed(KEY_BACKSPACE)) {
            deleteChar(&sheet[*cellIndex].gapStr);
            *textChanged = true;
        }
    } else if (key.shift) {
        if (key.ctrl) {
            // if (key.left) selectLeftToken();
            // if (key.right) selectRightToken();
        } else if (!key.ctrl) {
            if (key.left) selectChar(selection, &sheet[*cellIndex].gapStr, DIR_LEFT);
            if (key.right) selectChar(selection, &sheet[*cellIndex].gapStr, DIR_RIGHT);
        }
    }
    if (IsKeyPressed(KEY_ENTER)) {
        if (*cellIndex > 2 && *cellIndex < CELL_COUNT - 3) {
            char *filteredText = filterText(sheet[*cellIndex].gapStr.str);
            if (*textChanged == true)
                OverwriteStr(&sheet[*cellIndex].gapStr, filteredText, CELL_TEXT_LENGTH);
            if (*cellIndex > CELL_COUNT - 6) {
                *cellIndex = 0;
                tmpSelectState = false;
            } else *cellIndex = *cellIndex + 3;
        } else {
            *cellIndex = 0;
            tmpSelectState = false;
        }
        if (*textChanged == true)
            UpdateScores(sheet);
    }
    *selectionState = tmpSelectState;
}

void KeyPressHandler(KeyMap key, size_t *cellIndex, bool *selectionState)
{
    // TODO: escape should have multiple purposes
    // The case here is one, but there are more
    // If text is selected, escape should deselect // TODO: Text Selection
    if (key.escape) {
        *selectionState = false;
        *cellIndex = 0;
    }
    if (key.ctrl) {
        // TODO: undo/redo
        // TODO: save/load
        // TODO: select all
    }
    // TODO: shift selection
}

void InputHandler(Cell *sheet, size_t *cellIndex, bool *selectionState, bool *textChanged, Selection *selection)
{
    KeyMap key = {false};

    key.ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    key.shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    key.left = IsKeyPressed(KEY_LEFT);
    key.right = IsKeyPressed(KEY_RIGHT);
    key.escape = IsKeyPressed(KEY_ESCAPE);

    MouseHandler(sheet, cellIndex, selectionState);

    if (*selectionState) CellInputHandler(sheet, cellIndex, selectionState, textChanged, key, selection);
}