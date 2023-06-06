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
	: QMainWindow(parent), m_board(nullptr), m_mainWidget(nullptr), m_header(nullptr), m_layout(nullptr)
{
	Board* board = new Board(8, 8, 10, 20, this);

	m_mainWidget = new QWidget(this);
	m_header = new Header(8*20, this, this);

	m_layout = new QGridLayout();
	m_layout->setSpacing(0);
	m_layout->addWidget(m_header, 0, 0, Qt::AlignCenter);
	m_layout->addWidget(board, 1, 0);
	m_mainWidget->setLayout(m_layout);

	setCentralWidget(m_mainWidget);
}

MainWindow::~MainWindow()
{
	delete m_mainWidget, m_layout;
}

void MainWindow::restartGame()
{

}
