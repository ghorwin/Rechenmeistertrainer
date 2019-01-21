#include "MainDialog.h"
#include "ui_MainDialog.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <set>

#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>

#include "EinMalEinsDialog.h"

// 5 seconds per error
#define ERROR_PENALTY 5

MainDialog::MainDialog(QWidget *parent) :
	QDialog(parent),
	m_playerListModel(new QStringListModel(this)),
	ui(new Ui::MainDialog)
{
	ui->setupUi(this);
	ui->comboBoxPlayer->setModel(m_playerListModel);

	// setup diagram
	QwtPlot & plot = *qobject_cast<QwtPlot*>(ui->plot);
	plot.setTitle( "Bisherige Ergebnisse" );
	plot.setCanvasBackground( Qt::white );
	// create a new curve to be shown in the plot and set some properties
	m_durationCurve = new QwtPlotCurve();
	m_durationCurve->setTitle( "Zeit (+5s je Fehler)" ); // will later be used in legend
	m_durationCurve->setPen( QColor("navy"), 1 ), // color and thickness in pixels
	m_durationCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing}
	m_durationCurve->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
	m_durationCurve->attach( &plot );
	QwtSymbol *symbol = new QwtSymbol(
		QwtSymbol::Ellipse,    // Form
		QBrush( Qt::white ),  // Füllung
		QPen( QColor("navy"), 1 ),    // Rand
		QSize( 4, 4 ) );       // Größe in Pixel
	m_durationCurve->setSymbol( symbol );

	m_completedCurve = new QwtPlotCurve();
	m_completedCurve->setTitle( "gelöste Aufgaben" ); // will later be used in legend
	m_completedCurve->setPen( QColor("darkred"), 1 ), // color and thickness in pixels
	m_completedCurve->setRenderHint( QwtPlotItem::RenderAntialiased, true ); // use antialiasing}
	m_completedCurve->setAxes(QwtPlot::xBottom, QwtPlot::yRight);
	m_completedCurve->attach( &plot );
	symbol = new QwtSymbol(
		QwtSymbol::Rect,    // Form
		QBrush( Qt::white ),  // Füllung
		QPen( QColor("darkred"), 1 ),    // Rand
		QSize( 5, 5 ) );       // Größe in Pixel
	m_completedCurve->setSymbol( symbol );

	QwtLegend * legend = new QwtLegend();
	plot.insertLegend( legend , QwtPlot::BottomLegend);

	plot.setAxisScale( QwtPlot::yRight, 0.0, 100.0 );
	plot.enableAxis(QwtPlot::yRight, true);
	plot.setAxisTitle(QwtPlot::yLeft, QwtText("Zeit [min]"));
	plot.setAxisTitle(QwtPlot::yRight, QwtText("Gelöst"));

	QwtPlotGrid *grid = new QwtPlotGrid();
	grid->setMajorPen(QPen(Qt::DotLine));
	grid->attach( &plot );

	plot.setAutoReplot(true);

	// read old hight score
	QFile in("Rechenmeistertrainer.csv");
	QTextStream strm(&in);
	if (in.open(QFile::ReadOnly)) {
		QString line;
		while (strm.readLineInto(&line)) {
			QStringList tokens = line.split('\t');
			Stat t;
			t.duration = tokens[1].toDouble();
			t.errors = tokens[2].toInt();
			t.date = QDateTime::fromString(tokens[3], Qt::ISODate);
			m_stats[tokens[0]].append(t);
		}
	}

	// populate player list model
	m_playerListModel->setStringList( m_stats.keys());
	if (m_stats.empty())
		m_playerListModel->setStringList(QStringList() << "Henrik");
	ui->comboBoxPlayer->setCurrentIndex(0); /// \todo reselect last player

	connect(ui->comboBoxPlayer->lineEdit(), SIGNAL(editingFinished()), this, SLOT(onComboBoxPlayerEditingFinished()));

	updateHighscore();
}


MainDialog::~MainDialog() {
	delete ui;
}


void MainDialog::on_pushButtonStart_clicked() {
	if (ui->comboBoxPlayer->currentText().trimmed().isEmpty()) {
		QMessageBox::critical(this, QString(), tr("Bitte einen Namen eingeben!"));
		return;
	}
	EinMalEinsDialog dlg(this);

	int res = dlg.exec();

	// get results
	if (res == QDialog::Accepted) {
		Stat t;
		t.duration = dlg.m_duration;
		t.errors = 100-dlg.m_completed;
		t.date = QDateTime::currentDateTime();
		// write and show statistics
		m_stats[ui->comboBoxPlayer->currentText()].append(t);

		QFile in("Rechenmeistertrainer.csv");
		in.open(QFile::WriteOnly | QFile::Truncate);
		QTextStream strm(&in);

		for (QMap<QString, QList<Stat> >::const_iterator it = m_stats.begin(); it != m_stats.end(); ++it) {
			for (QList<Stat>::const_iterator stit = it->begin(); stit != it->end(); ++stit) {
				strm << it.key() << '\t' << stit->duration << '\t' << stit->errors << '\t' << stit->date.toString(Qt::ISODate) << '\n';
			}
		}
		updateDiagram(ui->comboBoxPlayer->currentText());
		updateHighscore();
	}

}


void MainDialog::onComboBoxPlayerEditingFinished() {
	// insert IP address into ComboBox if not yet present
	QString player = ui->comboBoxPlayer->currentText().trimmed();
	if (m_playerListModel->stringList().indexOf(player) == -1) {
		m_playerListModel->insertRow(0);
		m_playerListModel->setData(m_playerListModel->index(0,0), player);
	}

	updateDiagram(player);
}


void MainDialog::updateDiagram(const QString & playerName) {
	// get data of a given run
	QList<Stat> data = m_stats[playerName];
	std::vector<double> x,y,y2;
	for (int i=0; i<data.size(); ++i) {
		x.push_back(i);
		y.push_back( (data[i].duration + data[i].errors*ERROR_PENALTY)/60);
		y2.push_back(100-data[i].errors);
	}
	if (data.isEmpty()) {
		m_durationCurve->setSamples(QVector<QPointF>());
		m_completedCurve->setSamples(QVector<QPointF>());
	}
	else {
		m_durationCurve->setSamples(&x[0], &y[0], data.size());
		m_completedCurve->setSamples(&x[0], &y2[0], data.size());
	}
}


void MainDialog::on_comboBoxPlayer_currentIndexChanged(int) {
	updateDiagram(ui->comboBoxPlayer->currentText());
}

struct Score {
	int points;
	QString name;
};

bool operator<(const Score& lhs, const Score& rhs) {
	return (lhs.points < rhs.points);
}

void MainDialog::updateHighscore() {
	ui->tableWidget->clear();
	ui->tableWidget->verticalHeader()->setVisible(false);
	ui->tableWidget->verticalHeader()->setDefaultSectionSize(18);
	ui->tableWidget->setColumnCount(2);
	ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Punkte" << "Spieler");
	// create sorted list
	std::set<Score> scoreList;
	for (QMap<QString, QList<Stat> >::const_iterator it = m_stats.begin(); it != m_stats.end(); ++it) {
		for (QList<Stat>::const_iterator stit = it->begin(); stit != it->end(); ++stit) {
			Score s;
			int duration_with_penalty = stit->duration + stit->errors*ERROR_PENALTY;
			// Good value: 100000/4 min = 100000/240 ~ 417
			// Bad value: 100000(5 min mit 25 Fehlern) = 100000/425 =
			// Worst value: 100000/5 min + 100 Fehler) = 100000/125

			s.points = std::floor(100000/duration_with_penalty) - 124;
			s.name = QString("%1 [%2]").arg(it.key()).arg(stit->date.toString("dd.MM.yyyy hh:mm"));
			scoreList.insert(s);
		}
	}
	ui->tableWidget->setRowCount(scoreList.size());
	int i=0;
	for (std::set<Score>::const_reverse_iterator it = scoreList.rbegin(); it != scoreList.rend(); ++it, ++i) {
		QTableWidgetItem * item = new QTableWidgetItem(QString("%1").arg(it->points));
		item->setFlags(Qt::ItemIsEnabled);
		ui->tableWidget->setItem(i,0, item);
		item = new QTableWidgetItem(it->name);
		item->setFlags(Qt::ItemIsEnabled);
		ui->tableWidget->setItem(i,1, item);
	}
	ui->tableWidget->resizeColumnToContents(0);
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}
