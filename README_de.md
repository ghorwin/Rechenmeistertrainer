# Rechenmeistertrainer
Ein Mathetrainer für die Grundschule (1x1 Kopfrechenübung).

## Wie funktioniert der?

1. Programm starten (siehe unten)
2. Namen eingeben
3. Ein-mal-Eins Aufgabe starten
4. Nach 5 Minuten oder wenn alle 100 Aufgaben gelöst sind, wird die Auswertung angezeigt und man sieht ein Diagramm der Lernleistung... mehr Übung = mehr Aufgaben richtig gelöst = mehr Punkte!

### Ausgabe beim Test
![Ausgabe beim Test](doc/1x1_screen.png)

### Lernfortschritt
![Lernfortschritt](doc/progress_screen.png)

Weitere Screenshots im doc-Verzeichnis.

### Highscore und Statistik

Nach jedem Lauf (max 5 Minuten) wird eine Highscore nach folgender Formel berechnet:

```
duration_with_penalty = duration_in_seconds + not_completed*ERROR_PENALTY
score = 100000/duration_with_penalty - 124
```

`not_completed` ist die Anzahl der noch nicht korrekt beantworteten Fragen/leeren Felder (max 100). 
`ERROR_PENALTY` ist eine Zeitstrafe (5 Sekunden). Damit ist duration_with_penalty  maximal 800 = (300 + 5*100) und 100000/800 = 125 -> score = 1

Für jeden Testlauf werden Namen, Datum/Zeit, Dauer und Anzahl noch freier Felder abgelegt. Die Information wird in einer csv-Datei (Tab-getrennt) abgelegt, und zwar 

- entweder im benutzerspezifischen Datenverzeichnis, also z.B. unter `~/.local/share/kalwiz (calculation wizard)
- oder global für alle Anwender (unter Windows, in /Users/public/...)

Den Ort der gemeinschaftlichen Statistikdatei kann man im Einstellungsdialog anpassen.


## Download/Installation

### Windows
- zip-Datei herunterladen, entpacken und `Rechenmeistertrainer.exe` starten

### Linux

#### Ubuntu 16.04 oder neuer
- tar.gz-Datei herunterladen, Paketabhängigkeiten installieren (qt5, qt5 multimedia und qwt, siehe INSTALL-Datei im Archiv)
- Rechenmeistertrainer binary starten

#### Allgemein

- qt5 und qwt dev Pakete installieren
- QtCreator verwenden, Projektdatei `build/Qt/Rechenmeistertrainer.pro` öffnen und bauen

oder in der Kommandozeile bauen:

```bash
# in Rechenmeistertrainer root dir
$ cd build/Qt
$ qmake Rechenmeistertrainer.pro
$ make
```

