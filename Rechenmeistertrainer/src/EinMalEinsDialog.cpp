#include "EinMalEinsDialog.h"
#include "ui_EinMalEinsDialog.h"

#include <QGridLayout>
#include <QToolButton>
#include <QTimer>
#include <QMessageBox>
#include <QSound>

#include <cmath>

// time limit in seconds
#define TIME_LIMIT 300

EinMalEinsDialog::EinMalEinsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EinMalEinsDialog),
	m_errors(0)
{
	ui->setupUi(this);

	// create a grid layout 11x11
	QGridLayout * grid = new QGridLayout;

	for (unsigned int i=0; i<11; ++i) {
		if (i==0) {
			// first row, insert labels
			for (unsigned int j=0; j<10; ++j) {
				QLabel * l = new QLabel(this);
				l->setText(QString("%1").arg(j+1));
				grid->addWidget(l, 0, 1+j);
			}
		}
		else {
			// first column is the label
			QLabel * l = new QLabel(this);
			l->setText(QString("%1").arg(i));
			grid->addWidget(l, i, 0);
			for (unsigned int j=0; j<10; ++j) {
				QToolButton * btn = new QToolButton(this);
				// initially, buttons are empty
				btn->setText("");
				QFont f;
				f.setPointSize(18);
				btn->setFont(f);
				btn->setMinimumSize(60,60);
				btn->setMaximumSize(60,60);
				btn->setFocusPolicy(Qt::NoFocus);
				connect(btn, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
				m_buttonMap[btn] = i*(j+1); // store product
				grid->addWidget(btn, i, j+1);
			}

		}

	}
	ui->groupBoxGrid->setLayout(grid);

	// initialize problems
	for (unsigned int i=0; i<10; ++i) {
		for (unsigned int j=0; j<10; ++j) {
			m_remainingProducts.append(QPair<int,int>(i+1,j+1));
		}
	}

	m_buzzerSound = new QSound("://WrongAnswer.wav", this);
	m_startTime = QDateTime::currentDateTime();

	QTimer::singleShot(0, this, SLOT(onNextProblem()));
}


EinMalEinsDialog::~EinMalEinsDialog() {
	delete ui;
}


void EinMalEinsDialog::onButtonClicked() {
	QToolButton * btn = qobject_cast<QToolButton*>(sender());

	// lookup selected factors
	int product = m_buttonMap[btn];

	// check if answer is correct
	if (product == m_product) {
		btn->setText(QString("%1").arg(product));
		QPalette pal;
		pal.setColor(QPalette::Button, QColor("gold"));
		btn->setPalette(pal);
		disconnect(btn, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
		m_remainingProducts.removeAt(m_currentProblemIndex);
		onNextProblem();
	}
	else {
		// remember wrong answer
		++m_errors;
		m_buzzerSound->play();
		QPalette pal;
		pal.setColor(QPalette::WindowText, QColor("red"));
		ui->labelNextNumber->setPalette(pal);
		QTimer::singleShot(1000, this, SLOT(onNextProblem()));
	}
}


void EinMalEinsDialog::onNextProblem() {
	int remainingProducts = m_remainingProducts.count();
	QDateTime endTime = QDateTime::currentDateTime();
	m_duration = m_startTime.secsTo(endTime);
	if (remainingProducts == 0 || m_duration >= TIME_LIMIT) {
		// show result
		QGridLayout * gridLay = qobject_cast<QGridLayout *>(ui->groupBoxGrid->layout());

		int missing = 0;
		for (int i=0; i<10; ++i) {
			for (int j=0; j<10; ++j) {
				QToolButton * btn = qobject_cast<QToolButton*>(gridLay->itemAtPosition(i+1, j+1)->widget());
				if (btn->text().isEmpty()) {
					++missing;
					btn->setText(QString("%1").arg((i+1)*(j+2)));
					QPalette pal;
					pal.setColor(QPalette::Button, QColor("maroon"));
					pal.setColor(QPalette::ButtonText, QColor("white"));
					btn->setPalette(pal);
				}
			}
		}

		m_completed = 100 - missing; // number of solved problems
		if (m_duration >= TIME_LIMIT) {
			QMessageBox::information(this, tr("1x1 Success"), tr("The time is up. In %1:%2 minutes you have solved %3 problems, and "
																"you had %4 errors.")
									 .arg(std::floor((m_duration/60)))
									 .arg((int)std::floor((m_duration/60 - std::floor(m_duration/60))*60),2,10,QLatin1Char('0'))
									 .arg(m_completed)
									 .arg(m_errors));

		}
		else {
			QMessageBox::information(this, tr("1x1 Success"), tr("Great! You solved all problems in %1:%2 minutes, with %3 errors.")
									 .arg(std::floor((m_duration/60)))
									 .arg((int)std::floor((m_duration/60 - std::floor(m_duration/60))*60),2,10,QLatin1Char('0'))
									 .arg(m_errors));
		}

		accept(); // close dialog
		return;
	}

	// randomly generate new request
	m_currentProblemIndex = (int)(qrand()*(double)remainingProducts/RAND_MAX);
	const QPair<int,int> & problem = m_remainingProducts[m_currentProblemIndex];
	m_product = problem.first*problem.second;

	ui->labelNextNumber->setText(QString("%1").arg(m_product));
	QPalette pal;
	ui->labelNextNumber->setPalette(pal);

}

