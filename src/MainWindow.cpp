#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QGridlayout>
#include <QPushButton>

#include "MainWindow.h"
#include "Board.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	Board* board = new Board(10, 10, 20, this);

	m_mainWidget = new QWidget(this);

	m_layout = new QGridLayout();

	m_layout->setSpacing(0);
	m_layout->addWidget(board);
	m_mainWidget->setLayout(m_layout);

	setCentralWidget(m_mainWidget);
}

MainWindow::~MainWindow()
{
	delete m_mainWidget, m_layout;
}
