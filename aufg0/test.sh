#!/bin/sh

echo "1: falscher Aufruf"
./bba --input 2>/dev/null || echo OK

echo "\n\n2: Output in Datei + identity-Filter"
./bba --input eingabe.pgm --identity --output test2.pgm 2>/dev/null
diff -w --brief eingabe.pgm test2.pgm && echo OK
rm test2.pgm

echo "\n\n3: Output in Datei"
./bba --input eingabe.pgm --output test3.pgm 2>/dev/null
diff -w --brief eingabe.pgm test3.pgm && echo OK
rm test3.pgm

echo "\n\n4: Bild laden und vergessen"
./bba --input eingabe.pgm 2>/dev/null && echo OK 

echo "\n\n5: Ausgabe auf Standardausgabe"
./bba --input eingabe.pgm --output - > test5.pgm
diff -w --brief eingabe.pgm test5.pgm && echo OK
rm test5.pgm

echo "\n\n6: den Identitäts-Filter auf ein Bild anwenden"
./bba --input eingabe.pgm --identity --output - > test6.pgm
diff -w --brief eingabe.pgm test6.pgm && echo OK
rm test6.pgm

echo "\n\n7: Funktion des Stacks prüfen"
./bba --input eingabe.pgm --input eingabe2.pgm --output - --output - && echo OK

echo "\n\n8: mehrere Filter hintereinander anwenden"
./bba --input eingabe.pgm --identity --identity --output - > test8.pgm
diff -w --brief eingabe.pgm test8.pgm && echo OK
rm test8.pgm

echo "\n\n9: in anderem Dateiformat speichern"
./bba --input eingabe.pgm --output ausgabe.png && echo OK

echo "\n\n10: Funktion des Stacks prüfen"
./bba --input eingabe.pgm --input eingabe2.pgm --output test10a.pgm --output test10b.pgm
diff -w --brief eingabe.pgm test10a.pgm && echo OK
diff -w --brief eingabe2.pgm test10b.pgm && echo OK
rm test10a.pgm
rm test10b.pgm

echo "\n\n11: Bild laden, anzeigen und vergessen"
./bba --input eingabe.pgm --show && echo OK

echo "\n\n12: Bild laden, anzeigen und Ausgeben"
./bba --input eingabe.pgm --show --output - > test12.pgm
diff -w --brief eingabe.pgm test12.pgm && echo OK
rm test12.pgm

echo "\n\n13: Funktion des Stacks prüfen, unterschiedliche dateitypen"
./bba --input eingabe.pgm --input eingabe.pxm --output - --output - && echo OK
