#ifndef IO_H
#define IO_H

#include <stdbool.h>

void loadFont();
bool loadTimes();
char **loadLevelText(int game);
void loadSpecialText();
void saveTimes();
void ExportToBBCode();
void ExportActionTable();

#endif