#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QMap>
#include <QList>
#include <QStringListModel>
#include <QDateTime>

class QwtPlotCurve;

namespace Ui {
	class MainDialog;
}

class MainDialog : public QDialog {
	Q_OBJECT

public:
	explicit MainDialog(QWidget *parent = nullptr);
	~MainDialog();

private slots:
	void on_pushButtonStart_clicked();

	void onComboBoxPlayerEditingFinished();

	void on_comboBoxPlayer_currentIndexChanged(int index);

private:
	/*! Returns platform specific path to user statistics file. */
	QString statsFile() const;

	struct Stat {
		QDateTime date;
		int errors;
		double duration;
	};

	void updateDiagram(const QString & playerName);
	void updateHighscore();


	/*! Statistics of all players/runs. */
	QMap<QString, QList<Stat> >	m_stats;

	QwtPlotCurve				*m_durationCurve;
	QwtPlotCurve				*m_completedCurve;

	/*! Player list model. */
	QStringListModel			*m_playerListModel;

	Ui::MainDialog				*ui;
};

#endif // MAINDIALOG_H
