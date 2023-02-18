#pragma once
#include <QWidget>

class Board : public QWidget
{
public:
	Board(int rows, int columns, QWidget* parent);
	~Board();
};