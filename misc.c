#ifndef MISC_C
#define MISC_C

#include "includes.h"

// XY being the top left corner of the cell index
Vector2 indexToXY(size_t index)
{
    Vector2 xy = {0};
    xy.x = (index % COLUMNS) * UI.cellWidth;
    xy.y = (index / COLUMNS) * UI.cellHeight + TOP_BAR_HEIGHT;
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
    Vector2 rounded = {xy.x - fmodf(xy.x, UI.cellWidth), xy.y - fmodf(xy.y, UI.cellHeight)};
    rounded.x = rounded.x / UI.cellWidth;
    rounded.y = rounded.y / UI.cellHeight;
    index = rounded.x + (rounded.y * COLUMNS);
    return index;
}

// CR being column/row
size_t crToIndex(Vector2 cr)
{
    return (size_t)(cr.x + (cr.y * COLUMNS));
}

char* ColorToHexText(Color color)
{
    char* colorText = malloc(7);
    snprintf(colorText + 0, 3, "%02x", color.r);
    snprintf(colorText + 2, 3, "%02x", color.g);
    snprintf(colorText + 4, 3, "%02x", color.b);
    return colorText;
}

void chrswap(char* ptr1, char* ptr2)
{
    char tmp = *ptr2;
    *ptr2 = *ptr1;
    *ptr1 = tmp;
}

char* i_toStr(int num)
{
    // TODO: make more portable or just use itoa in place of this
    char* str = malloc(sizeof(char) * CELL_TEXT_LENGTH);
    sprintf(str, "%d", num);
    return str;
}

#endif