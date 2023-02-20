#pragma once
#include <QWidget>

#include <vector>
#include <memory>

#include "Tile.h"

class Board : public QWidget
{
public:
	Board(int rows, int columns, int tileSize, QWidget* parent = nullptr);
	~Board();

private:
	int m_rows, m_columns, m_tileSize;
	std::array<std::string, 3> m_styleSheets;
	std::vector<Tile*> m_tiles;

	void loadStyleSheets(const std::string& dirPath);
};