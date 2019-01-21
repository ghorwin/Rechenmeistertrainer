#ifndef EINMALEINSDIALOG_H
#define EINMALEINSDIALOG_H

#include <QDialog>
#include <QPair>
#include <QMap>
#include <QDateTime>

class QToolButton;
class QSound;

namespace Ui {
class EinMalEinsDialog;
}

class EinMalEinsDialog : public QDialog {
	Q_OBJECT
public:
	explicit EinMalEinsDialog(QWidget *parent = nullptr);
	~EinMalEinsDialog();

	int m_completed; // number of completed problems
	double m_duration; // elapsed time in seconds

private slots:

	void onButtonClicked();

	void onNextProblem();


private:
	Ui::EinMalEinsDialog *ui;

	QMap<QToolButton*, int > m_buttonMap;

	/*! Map that contains all remaining products to be found. */
	QList< QPair<int, int> > m_remainingProducts;

	QDateTime	m_startTime;
	/*! Current product. */
	int m_product;
	/*! Index of current problem - to remove problem when correct answer was given. */
	int m_currentProblemIndex;

	/*! Counts number of invalid answers. */
	int m_errors;

	QSound * m_buzzerSound;
};

#endif // EINMALEINSDIALOG_H
