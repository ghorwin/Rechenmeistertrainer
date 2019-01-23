# Rechenmeistertrainer
Ein Mathetrainer für die Grundschule (1x1 und sowas).

## Wie funktioniert der?

1. Programm starten (siehe unten)
2. Namen eingeben
3. Ein-mal-Eins Aufgabe starten
4. Nach 5 Minuten oder wenn alle 100 Aufgaben gelöst sind, wird die Auswertung angezeigt und man sieht ein Diagramm der Lernleistung... mehr Übung = mehr Aufgaben richtig gelöst = mehr Punkte!

## Download/Installation

### Windows
- zip-Datei Herunterladen, entpacken und `Rechenmeistertrainer.exe` starten

### Linux

#### Ubuntu 16.04

1. Qwt-Bibliothek installieren:

```bash
> sudo apt get install libqwt-qt5-dev
```

2. Qt 5.10 oder neuer von Qt herunterladen und installieren (wegen Multimedia-support)

3. Quelltext übersetzen und Programm starten

```bash
# in ~/Rechenmeistertrainer 
> cd build/Qt
> export PATH=/path/to/Qt5.10/bin:$PATH
> qmake Rechenmeistertrainer.pro
> make
```
