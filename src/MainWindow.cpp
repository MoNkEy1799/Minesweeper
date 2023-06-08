#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QGridlayout>
#include <QPushButton>

#include <string>

#include "MainWindow.h"
#include "HeaderWidget.h"
#include "Board.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), board(nullptr), m_mainWidget(nullptr), header(nullptr), m_layout(nullptr),
	m_rowSetting(24), m_colSetting(30), m_mineSetting(10)
{
	header = new Header(m_colSetting * 20 + 4, this, this);
	header->changeMineCount(m_mineSetting);
	board = new Board(m_rowSetting, m_colSetting, m_mineSetting, 20, this, this);

	m_layout = new QGridLayout();
	m_layout->addWidget(header, 0, 0, Qt::AlignCenter);
	m_layout->addWidget(board, 1, 0);

	m_mainWidget = new QWidget(this);
	m_mainWidget->setLayout(m_layout);
	m_mainWidget->setStyleSheet(StyleSheet::OUTER.c_str());
	m_mainWidget->setFixedSize(m_colSetting * 20 + 24, m_rowSetting * 20 + 84);
	setFixedSize(m_colSetting * 20 + 24, m_rowSetting * 20 + 84);
	setCentralWidget(m_mainWidget);
}

void MainWindow::restartGame()
{
	board->hide();
	board->deleteLater();
	board = new Board(m_rowSetting, m_colSetting, m_mineSetting, 20, this, this);
	m_layout->addWidget(board, 1, 0);
	header->changeSmiley(true, true);
}
