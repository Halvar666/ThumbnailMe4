// SPDX-License-Identifier: GPL-2.0-or-later
#include "MainWindow.h"

#include "ContactSheetComposer.h"
#include "FrameGrabber.h"
#include "MediaInfoReader.h"
#include "VideoProbe.h"
#include "VideoTypes.h"

#include <QApplication>
#include <QAbstractItemView>
#include <QAction>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QCoreApplication>
#include <QDir>
#include <QDoubleSpinBox>
#include <QEventLoop>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageWriter>
#include <QFontDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QLinearGradient>
#include <QProcess>
#include <QPushButton>
#include <QResizeEvent>
#include <QSettings>
#include <QSpinBox>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
    QString translateUiText(const QString &language, const QString &english)
    {
        if (language == "en") {
            return english;
        }

        static const QHash<QString, QString> cs = {
            {"%1 rows × %2 columns", "%1 řádků × %2 sloupců"},
            {"About", "O aplikaci"},
            {"About Thumbnail me 4b2", "O Thumbnail me 4b2"},
            {"Accurate (slowest)", "Přesný (nejpomalejší)"},
            {"Add and select a video first.", "Nejdřív přidej a vyber video."},
            {"Add video files", "Přidat video soubory"},
            {"Add video files first.", "Nejdřív přidej video soubory."},
            {"Added %1 video file(s)", "Přidáno %1 video souborů"},
            {"Additional title:", "Doplňkový titulek:"},
            {"Background color:", "Barva pozadí:"},
            {"Balanced (safer for short videos)", "Vyvážený (bezpečnější pro krátká videa)"},
            {"Batch cancellation requested...", "Požadavek na zrušení dávky..."},
            {"Browse...", "Procházet..."},
            {"Cancel batch", "Zrušit dávku"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Zrušení vyžádáno. Čekám na zastavení aktuálního workeru..."},
            {"Choose count text color", "Vybrat barvu počtu snímků"},
            {"Choose font...", "Vybrat font..."},
            {"Choose info text color", "Vybrat barvu info textu"},
            {"Choose tile border color", "Vybrat barvu rámečku"},
            {"Choose timestamp background color", "Vybrat barvu pozadí času"},
            {"Choose timestamp text color", "Vybrat barvu času"},
            {"Choose title text color", "Vybrat barvu titulku"},
            {"Choose...", "Vybrat..."},
            {"Clear preview", "Vyčistit náhled"},
            {"Columns:", "Sloupce:"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Contact sheet preview", "Náhled contact sheetu"},
            {"Count text color:", "Barva počtu snímků:"},
            {"Delete Selection", "Smazat výběr"},
            {"Draw tile border", "Kreslit rámeček snímku"},
            {"Edition", "Úpravy"},
            {"End margin:", "Vynechat konec:"},
            {"Exit", "Konec"},
            {"Fast (Thumbnail me 3.0 style)", "Rychlý (styl Thumbnail me 3.0)"},
            {"File", "Soubor"},
            {"Font:", "Font:"},
            {"Gap:", "Mezera:"},
            {"Generate a contact sheet first.", "Nejdřív vygeneruj contact sheet."},
            {"Info position:", "Pozice info textu:"},
            {"Info text color:", "Barva info textu:"},
            {"Info text:", "Info text:"},
            {"Input / Output", "Vstup / výstup"},
            {"Input list cleared", "Seznam vstupů vyčištěn"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jazyk změněn. Ulož nastavení a restartuj aplikaci, aby se změna plně projevila."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Výběr jazyka se okamžitě uloží do settings.ini. Pro plné přepnutí statických popisků aplikaci restartuj."},
            {"Language:", "Jazyk:"},
            {"Load", "Načíst"},
            {"Load video files...", "Načíst video soubory..."},
            {"Load videos and click Start selected or Start all.", "Načti videa a klikni na Spustit vybrané nebo Spustit vše."},
            {"Lower Left", "Vlevo dole"},
            {"Lower Right", "Vpravo dole"},
            {"No contact sheet to save", "Není co uložit"},
            {"Open Recent Configuration", "Otevřít nedávnou konfiguraci"},
            {"Open configuration...", "Otevřít konfiguraci..."},
            {"Output", "Výstup"},
            {"Output Format:", "Výstupní formát:"},
            {"Output filename suffix:", "Přípona názvu výstupu:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Výstupní cesta:"},
            {"Predefined Configuration", "Předdefinovaná konfigurace"},
            {"Preset applied: %1 rows × %2 columns", "Preset použit: %1 řádků × %2 sloupců"},
            {"Preview area", "Oblast náhledu"},
            {"Preview style", "Náhled stylu"},
            {"Quality:", "Kvalita:"},
            {"Randomize amount:", "Míra náhody:"},
            {"Randomize positions", "Náhodně posunout pozice"},
            {"Randomize:", "Náhodně posunout:"},
            {"Reload settings.ini", "Znovu načíst settings.ini"},
            {"Reset", "Reset"},
            {"Rows:", "Řádky:"},
            {"Same folder as source video", "Stejná složka jako zdrojové video"},
            {"Same source folder:", "Stejná složka jako zdroj:"},
            {"Save configuration", "Uložit konfiguraci"},
            {"Save configuration as...", "Uložit konfiguraci jako..."},
            {"Save contact sheet", "Uložit contact sheet"},
            {"Save failed", "Uložení selhalo"},
            {"Save manually", "Uložit ručně"},
            {"Save selected contact sheet...", "Uložit vybraný contact sheet..."},
            {"Save settings.ini", "Uložit settings.ini"},
            {"Saved: ", "Uloženo: "},
            {"Seek mode:", "Režim seeku:"},
            {"Select output folder", "Vybrat výstupní složku"},
            {"Selected video file(s) removed", "Vybrané video soubory odstraněny"},
            {"Settings saved:\n", "Nastavení uloženo:\n"},
            {"Show additional title", "Zobrazit doplňkový titulek"},
            {"Show technical info text", "Zobrazit technický info text"},
            {"Show timestamps", "Zobrazit časové značky"},
            {"Similarity threshold:", "Práh podobnosti:"},
            {"Skip dark / blank frames", "Přeskočit tmavé / prázdné snímky"},
            {"Skip dark:", "Přeskočit tmavé:"},
            {"Skip similar frames", "Přeskočit podobné snímky"},
            {"Skip similar:", "Přeskočit podobné:"},
            {"Specific timestep:", "Pevný časový krok:"},
            {"Start all", "Spustit vše"},
            {"Start margin:", "Vynechat začátek:"},
            {"Start selected", "Spustit vybrané"},
            {"Style preview failed", "Náhled stylu selhal"},
            {"Style preview ready", "Náhled stylu je připravený"},
            {"Styles", "Styl"},
            {"The file could not be saved.", "Soubor se nepodařilo uložit."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Barva rámečku:"},
            {"Tile border:", "Rámeček snímku:"},
            {"Timestamp background:", "Pozadí času:"},
            {"Timestamp position:", "Pozice času:"},
            {"Timestamp text color:", "Barva času:"},
            {"Timestamp:", "Čas:"},
            {"Timestep:", "Časový krok:"},
            {"Title text color:", "Barva titulku:"},
            {"Title text:", "Text titulku:"},
            {"Upper Left", "Vlevo nahoře"},
            {"Upper Right", "Vpravo nahoře"},
            {"Use specific timestep", "Použít pevný časový krok"},
            {"View", "Zobrazení"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Šířka:"},
            {"English", "English"},
            {"French", "Francouzsky"},
            {"German", "Německy"},
            {"Spanish", "Španělsky"},
            {"Dutch", "Nizozemsky"},
            {"Czech", "Česky"},
            {"Italian", "Italsky"},
            {"Hungarian", "Maďarsky"},
            {"Croatian", "Chorvatsky"},
            {"Portuguese", "Portugalsky"},
            {"Chinese", "Čínsky"},
            {"Russian", "Rusky"}
        };

        static const QHash<QString, QString> fr = {
            {"%1 rows × %2 columns", "%1 lignes × %2 colonnes"},
            {"About", "À propos"},
            {"About Thumbnail me 4b2", "À propos de Thumbnail me 4b2"},
            {"Accurate (slowest)", "Précis (le plus lent)"},
            {"Add and select a video first.", "Ajoutez et sélectionnez d’abord une vidéo."},
            {"Add video files", "Ajouter des fichiers vidéo"},
            {"Add video files first.", "Ajoutez d’abord des fichiers vidéo."},
            {"Added %1 video file(s)", "%1 fichier(s) vidéo ajouté(s)"},
            {"Additional title:", "Titre supplémentaire :"},
            {"Background color:", "Couleur d’arrière-plan :"},
            {"Balanced (safer for short videos)", "Équilibré (plus sûr pour les vidéos courtes)"},
            {"Batch cancellation requested...", "Annulation du lot demandée..."},
            {"Browse...", "Parcourir..."},
            {"Cancel batch", "Annuler le lot"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Annulation demandée. Attente de l’arrêt du worker en cours..."},
            {"Choose count text color", "Choisir la couleur du compteur"},
            {"Choose font...", "Choisir la police..."},
            {"Choose info text color", "Choisir la couleur du texte d’information"},
            {"Choose tile border color", "Choisir la couleur de bordure des vignettes"},
            {"Choose timestamp background color", "Choisir la couleur de fond de l’horodatage"},
            {"Choose timestamp text color", "Choisir la couleur de l’horodatage"},
            {"Choose title text color", "Choisir la couleur du titre"},
            {"Choose...", "Choisir..."},
            {"Clear preview", "Effacer l’aperçu"},
            {"Columns:", "Colonnes :"},
            {"Config 1", "Config 1"},
            {"Config 2", "Config 2"},
            {"Config 3", "Config 3"},
            {"Contact sheet preview", "Aperçu de la planche-contact"},
            {"Count text color:", "Couleur du compteur :"},
            {"Delete Selection", "Supprimer la sélection"},
            {"Draw tile border", "Dessiner la bordure des vignettes"},
            {"Edition", "Édition"},
            {"End margin:", "Marge de fin :"},
            {"Exit", "Quitter"},
            {"Fast (Thumbnail me 3.0 style)", "Rapide (style Thumbnail me 3.0)"},
            {"File", "Fichier"},
            {"Font:", "Police :"},
            {"Gap:", "Espacement :"},
            {"Generate a contact sheet first.", "Générez d’abord une planche-contact."},
            {"Info position:", "Position des infos :"},
            {"Info text color:", "Couleur du texte d’info :"},
            {"Info text:", "Texte d’info :"},
            {"Input / Output", "Entrée / sortie"},
            {"Input list cleared", "Liste d’entrée effacée"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Langue modifiée. Enregistrez les paramètres et redémarrez l’application pour l’appliquer complètement."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "La langue est enregistrée immédiatement dans settings.ini. Redémarrez l’application pour appliquer tous les libellés statiques."},
            {"Language:", "Langue :"},
            {"Load", "Charger"},
            {"Load video files...", "Charger des fichiers vidéo..."},
            {"Load videos and click Start selected or Start all.", "Chargez des vidéos puis cliquez sur Démarrer la sélection ou Tout démarrer."},
            {"Lower Left", "En bas à gauche"},
            {"Lower Right", "En bas à droite"},
            {"No contact sheet to save", "Aucune planche-contact à enregistrer"},
            {"Open Recent Configuration", "Ouvrir une configuration récente"},
            {"Open configuration...", "Ouvrir une configuration..."},
            {"Output", "Sortie"},
            {"Output Format:", "Format de sortie :"},
            {"Output filename suffix:", "Suffixe du nom de fichier :"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Chemin de sortie :"},
            {"Predefined Configuration", "Configuration prédéfinie"},
            {"Preset applied: %1 rows × %2 columns", "Préréglage appliqué : %1 lignes × %2 colonnes"},
            {"Preview area", "Zone d’aperçu"},
            {"Preview style", "Aperçu du style"},
            {"Quality:", "Qualité :"},
            {"Randomize amount:", "Quantité aléatoire :"},
            {"Randomize positions", "Positions aléatoires"},
            {"Randomize:", "Aléatoire :"},
            {"Reload settings.ini", "Recharger settings.ini"},
            {"Reset", "Réinitialiser"},
            {"Rows:", "Lignes :"},
            {"Same folder as source video", "Même dossier que la vidéo source"},
            {"Same source folder:", "Même dossier source :"},
            {"Save configuration", "Enregistrer la configuration"},
            {"Save configuration as...", "Enregistrer la configuration sous..."},
            {"Save contact sheet", "Enregistrer la planche-contact"},
            {"Save failed", "Échec de l’enregistrement"},
            {"Save manually", "Enregistrer manuellement"},
            {"Save selected contact sheet...", "Enregistrer la planche-contact sélectionnée..."},
            {"Save settings.ini", "Enregistrer settings.ini"},
            {"Saved: ", "Enregistré : "},
            {"Seek mode:", "Mode de recherche :"},
            {"Select output folder", "Sélectionner le dossier de sortie"},
            {"Selected video file(s) removed", "Fichier(s) vidéo sélectionné(s) supprimé(s)"},
            {"Settings saved:\n", "Paramètres enregistrés :\n"},
            {"Show additional title", "Afficher le titre supplémentaire"},
            {"Show technical info text", "Afficher les infos techniques"},
            {"Show timestamps", "Afficher les horodatages"},
            {"Similarity threshold:", "Seuil de similarité :"},
            {"Skip dark / blank frames", "Ignorer les images sombres / vides"},
            {"Skip dark:", "Ignorer les sombres :"},
            {"Skip similar frames", "Ignorer les images similaires"},
            {"Skip similar:", "Ignorer similaires :"},
            {"Specific timestep:", "Intervalle fixe :"},
            {"Start all", "Tout démarrer"},
            {"Start margin:", "Marge de début :"},
            {"Start selected", "Démarrer la sélection"},
            {"Style preview failed", "Échec de l’aperçu du style"},
            {"Style preview ready", "Aperçu du style prêt"},
            {"Styles", "Styles"},
            {"The file could not be saved.", "Le fichier n’a pas pu être enregistré."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Couleur de bordure :"},
            {"Tile border:", "Bordure des vignettes :"},
            {"Timestamp background:", "Fond de l’horodatage :"},
            {"Timestamp position:", "Position de l’horodatage :"},
            {"Timestamp text color:", "Couleur de l’horodatage :"},
            {"Timestamp:", "Horodatage :"},
            {"Timestep:", "Intervalle :"},
            {"Title text color:", "Couleur du titre :"},
            {"Title text:", "Texte du titre :"},
            {"Upper Left", "En haut à gauche"},
            {"Upper Right", "En haut à droite"},
            {"Use specific timestep", "Utiliser un intervalle fixe"},
            {"View", "Affichage"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Largeur :"},
            {"English", "Anglais"},
            {"French", "Français"},
            {"German", "Allemand"},
            {"Spanish", "Espagnol"},
            {"Dutch", "Néerlandais"},
            {"Czech", "Tchèque"},
            {"Italian", "Italien"},
            {"Hungarian", "Hongrois"},
            {"Croatian", "Croate"},
            {"Portuguese", "Portugais"},
            {"Chinese", "Chinois"},
            {"Russian", "Russe"}
        };

        static const QHash<QString, QString> de = {
            {"%1 rows × %2 columns", "%1 Zeilen × %2 Spalten"},
            {"About", "Über"},
            {"About Thumbnail me 4b2", "Über Thumbnail me 4b2"},
            {"Accurate (slowest)", "Genau (am langsamsten)"},
            {"Add and select a video first.", "Fügen Sie zuerst ein Video hinzu und wählen Sie es aus."},
            {"Add video files", "Videodateien hinzufügen"},
            {"Add video files first.", "Fügen Sie zuerst Videodateien hinzu."},
            {"Added %1 video file(s)", "%1 Videodatei(en) hinzugefügt"},
            {"Additional title:", "Zusätzlicher Titel:"},
            {"Background color:", "Hintergrundfarbe:"},
            {"Balanced (safer for short videos)", "Ausgewogen (sicherer für kurze Videos)"},
            {"Batch cancellation requested...", "Abbruch der Stapelverarbeitung angefordert..."},
            {"Browse...", "Durchsuchen..."},
            {"Cancel batch", "Stapel abbrechen"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Abbruch angefordert. Warten auf das Beenden des aktuellen Workers..."},
            {"Choose count text color", "Farbe für Zählertext wählen"},
            {"Choose font...", "Schrift wählen..."},
            {"Choose info text color", "Farbe für Infotext wählen"},
            {"Choose tile border color", "Farbe für Kachelrahmen wählen"},
            {"Choose timestamp background color", "Hintergrundfarbe des Zeitstempels wählen"},
            {"Choose timestamp text color", "Farbe des Zeitstempels wählen"},
            {"Choose title text color", "Farbe des Titels wählen"},
            {"Choose...", "Wählen..."},
            {"Clear preview", "Vorschau löschen"},
            {"Columns:", "Spalten:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Kontaktbogen-Vorschau"},
            {"Count text color:", "Farbe des Zählertexts:"},
            {"Delete Selection", "Auswahl löschen"},
            {"Draw tile border", "Kachelrahmen zeichnen"},
            {"Edition", "Bearbeiten"},
            {"End margin:", "Endrand:"},
            {"Exit", "Beenden"},
            {"Fast (Thumbnail me 3.0 style)", "Schnell (Thumbnail-me-3.0-Stil)"},
            {"File", "Datei"},
            {"Font:", "Schrift:"},
            {"Gap:", "Abstand:"},
            {"Generate a contact sheet first.", "Erzeugen Sie zuerst einen Kontaktbogen."},
            {"Info position:", "Infoposition:"},
            {"Info text color:", "Farbe des Infotexts:"},
            {"Info text:", "Infotext:"},
            {"Input / Output", "Eingabe / Ausgabe"},
            {"Input list cleared", "Eingabeliste geleert"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Sprache geändert. Speichern Sie die Einstellungen und starten Sie die Anwendung neu, um sie vollständig anzuwenden."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Die Sprachauswahl wird sofort in settings.ini gespeichert. Starten Sie die Anwendung neu, um alle statischen Beschriftungen zu übernehmen."},
            {"Language:", "Sprache:"},
            {"Load", "Laden"},
            {"Load video files...", "Videodateien laden..."},
            {"Load videos and click Start selected or Start all.", "Laden Sie Videos und klicken Sie auf Auswahl starten oder Alle starten."},
            {"Lower Left", "Unten links"},
            {"Lower Right", "Unten rechts"},
            {"No contact sheet to save", "Kein Kontaktbogen zum Speichern"},
            {"Open Recent Configuration", "Zuletzt verwendete Konfiguration öffnen"},
            {"Open configuration...", "Konfiguration öffnen..."},
            {"Output", "Ausgabe"},
            {"Output Format:", "Ausgabeformat:"},
            {"Output filename suffix:", "Suffix des Ausgabedateinamens:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Ausgabepfad:"},
            {"Predefined Configuration", "Vordefinierte Konfiguration"},
            {"Preset applied: %1 rows × %2 columns", "Voreinstellung angewendet: %1 Zeilen × %2 Spalten"},
            {"Preview area", "Vorschaubereich"},
            {"Preview style", "Stilvorschau"},
            {"Quality:", "Qualität:"},
            {"Randomize amount:", "Zufallsstärke:"},
            {"Randomize positions", "Positionen zufällig wählen"},
            {"Randomize:", "Zufall:"},
            {"Reload settings.ini", "settings.ini neu laden"},
            {"Reset", "Zurücksetzen"},
            {"Rows:", "Zeilen:"},
            {"Same folder as source video", "Gleicher Ordner wie Quellvideo"},
            {"Same source folder:", "Gleicher Quellordner:"},
            {"Save configuration", "Konfiguration speichern"},
            {"Save configuration as...", "Konfiguration speichern unter..."},
            {"Save contact sheet", "Kontaktbogen speichern"},
            {"Save failed", "Speichern fehlgeschlagen"},
            {"Save manually", "Manuell speichern"},
            {"Save selected contact sheet...", "Ausgewählten Kontaktbogen speichern..."},
            {"Save settings.ini", "settings.ini speichern"},
            {"Saved: ", "Gespeichert: "},
            {"Seek mode:", "Suchmodus:"},
            {"Select output folder", "Ausgabeordner auswählen"},
            {"Selected video file(s) removed", "Ausgewählte Videodatei(en) entfernt"},
            {"Settings saved:\n", "Einstellungen gespeichert:\n"},
            {"Show additional title", "Zusätzlichen Titel anzeigen"},
            {"Show technical info text", "Technische Infos anzeigen"},
            {"Show timestamps", "Zeitstempel anzeigen"},
            {"Similarity threshold:", "Ähnlichkeitsschwelle:"},
            {"Skip dark / blank frames", "Dunkle / leere Frames überspringen"},
            {"Skip dark:", "Dunkle überspringen:"},
            {"Skip similar frames", "Ähnliche Frames überspringen"},
            {"Skip similar:", "Ähnliche überspringen:"},
            {"Specific timestep:", "Fester Zeitschritt:"},
            {"Start all", "Alle starten"},
            {"Start margin:", "Startrand:"},
            {"Start selected", "Auswahl starten"},
            {"Style preview failed", "Stilvorschau fehlgeschlagen"},
            {"Style preview ready", "Stilvorschau bereit"},
            {"Styles", "Stile"},
            {"The file could not be saved.", "Die Datei konnte nicht gespeichert werden."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Kachelrahmenfarbe:"},
            {"Tile border:", "Kachelrahmen:"},
            {"Timestamp background:", "Zeitstempel-Hintergrund:"},
            {"Timestamp position:", "Zeitstempelposition:"},
            {"Timestamp text color:", "Zeitstempelfarbe:"},
            {"Timestamp:", "Zeitstempel:"},
            {"Timestep:", "Zeitschritt:"},
            {"Title text color:", "Titelfarbe:"},
            {"Title text:", "Titeltext:"},
            {"Upper Left", "Oben links"},
            {"Upper Right", "Oben rechts"},
            {"Use specific timestep", "Festen Zeitschritt verwenden"},
            {"View", "Ansicht"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Breite:"},
            {"English", "Englisch"},
            {"French", "Französisch"},
            {"German", "Deutsch"},
            {"Spanish", "Spanisch"},
            {"Dutch", "Niederländisch"},
            {"Czech", "Tschechisch"},
            {"Italian", "Italienisch"},
            {"Hungarian", "Ungarisch"},
            {"Croatian", "Kroatisch"},
            {"Portuguese", "Portugiesisch"},
            {"Chinese", "Chinesisch"},
            {"Russian", "Russisch"}
        };

        static const QHash<QString, QString> es = {
            {"%1 rows × %2 columns", "%1 filas × %2 columnas"},
            {"About", "Acerca de"},
            {"About Thumbnail me 4b2", "Acerca de Thumbnail me 4b2"},
            {"Accurate (slowest)", "Preciso (más lento)"},
            {"Add and select a video first.", "Añade y selecciona primero un vídeo."},
            {"Add video files", "Añadir archivos de vídeo"},
            {"Add video files first.", "Añade primero archivos de vídeo."},
            {"Added %1 video file(s)", "%1 archivo(s) de vídeo añadido(s)"},
            {"Additional title:", "Título adicional:"},
            {"Background color:", "Color de fondo:"},
            {"Balanced (safer for short videos)", "Equilibrado (más seguro para vídeos cortos)"},
            {"Batch cancellation requested...", "Cancelación del lote solicitada..."},
            {"Browse...", "Examinar..."},
            {"Cancel batch", "Cancelar lote"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Cancelación solicitada. Esperando a que se detenga el worker actual..."},
            {"Choose count text color", "Elegir color del contador"},
            {"Choose font...", "Elegir fuente..."},
            {"Choose info text color", "Elegir color del texto de información"},
            {"Choose tile border color", "Elegir color del borde de miniatura"},
            {"Choose timestamp background color", "Elegir color de fondo de la marca de tiempo"},
            {"Choose timestamp text color", "Elegir color de la marca de tiempo"},
            {"Choose title text color", "Elegir color del título"},
            {"Choose...", "Elegir..."},
            {"Clear preview", "Limpiar vista previa"},
            {"Columns:", "Columnas:"},
            {"Config 1", "Config. 1"},
            {"Config 2", "Config. 2"},
            {"Config 3", "Config. 3"},
            {"Contact sheet preview", "Vista previa de hoja de contactos"},
            {"Count text color:", "Color del contador:"},
            {"Delete Selection", "Eliminar selección"},
            {"Draw tile border", "Dibujar borde de miniatura"},
            {"Edition", "Edición"},
            {"End margin:", "Margen final:"},
            {"Exit", "Salir"},
            {"Fast (Thumbnail me 3.0 style)", "Rápido (estilo Thumbnail me 3.0)"},
            {"File", "Archivo"},
            {"Font:", "Fuente:"},
            {"Gap:", "Espacio:"},
            {"Generate a contact sheet first.", "Genera primero una hoja de contactos."},
            {"Info position:", "Posición de información:"},
            {"Info text color:", "Color del texto de información:"},
            {"Info text:", "Texto de información:"},
            {"Input / Output", "Entrada / salida"},
            {"Input list cleared", "Lista de entrada vaciada"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Idioma cambiado. Guarda la configuración y reinicia la aplicación para aplicarlo por completo."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "La selección de idioma se guarda inmediatamente en settings.ini. Reinicia la aplicación para aplicar por completo las etiquetas estáticas."},
            {"Language:", "Idioma:"},
            {"Load", "Cargar"},
            {"Load video files...", "Cargar archivos de vídeo..."},
            {"Load videos and click Start selected or Start all.", "Carga vídeos y pulsa Iniciar seleccionados o Iniciar todo."},
            {"Lower Left", "Abajo izquierda"},
            {"Lower Right", "Abajo derecha"},
            {"No contact sheet to save", "No hay hoja de contactos para guardar"},
            {"Open Recent Configuration", "Abrir configuración reciente"},
            {"Open configuration...", "Abrir configuración..."},
            {"Output", "Salida"},
            {"Output Format:", "Formato de salida:"},
            {"Output filename suffix:", "Sufijo del nombre de salida:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Ruta de salida:"},
            {"Predefined Configuration", "Configuración predefinida"},
            {"Preset applied: %1 rows × %2 columns", "Preajuste aplicado: %1 filas × %2 columnas"},
            {"Preview area", "Área de vista previa"},
            {"Preview style", "Vista previa de estilo"},
            {"Quality:", "Calidad:"},
            {"Randomize amount:", "Cantidad aleatoria:"},
            {"Randomize positions", "Aleatorizar posiciones"},
            {"Randomize:", "Aleatorizar:"},
            {"Reload settings.ini", "Recargar settings.ini"},
            {"Reset", "Restablecer"},
            {"Rows:", "Filas:"},
            {"Same folder as source video", "Misma carpeta que el vídeo fuente"},
            {"Same source folder:", "Misma carpeta fuente:"},
            {"Save configuration", "Guardar configuración"},
            {"Save configuration as...", "Guardar configuración como..."},
            {"Save contact sheet", "Guardar hoja de contactos"},
            {"Save failed", "Error al guardar"},
            {"Save manually", "Guardar manualmente"},
            {"Save selected contact sheet...", "Guardar hoja de contactos seleccionada..."},
            {"Save settings.ini", "Guardar settings.ini"},
            {"Saved: ", "Guardado: "},
            {"Seek mode:", "Modo de búsqueda:"},
            {"Select output folder", "Seleccionar carpeta de salida"},
            {"Selected video file(s) removed", "Archivo(s) de vídeo seleccionado(s) eliminado(s)"},
            {"Settings saved:\n", "Configuración guardada:\n"},
            {"Show additional title", "Mostrar título adicional"},
            {"Show technical info text", "Mostrar texto técnico"},
            {"Show timestamps", "Mostrar marcas de tiempo"},
            {"Similarity threshold:", "Umbral de similitud:"},
            {"Skip dark / blank frames", "Omitir fotogramas oscuros / vacíos"},
            {"Skip dark:", "Omitir oscuros:"},
            {"Skip similar frames", "Omitir fotogramas similares"},
            {"Skip similar:", "Omitir similares:"},
            {"Specific timestep:", "Intervalo fijo:"},
            {"Start all", "Iniciar todo"},
            {"Start margin:", "Margen inicial:"},
            {"Start selected", "Iniciar seleccionados"},
            {"Style preview failed", "Falló la vista previa de estilo"},
            {"Style preview ready", "Vista previa de estilo lista"},
            {"Styles", "Estilos"},
            {"The file could not be saved.", "No se pudo guardar el archivo."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Color del borde:"},
            {"Tile border:", "Borde de miniatura:"},
            {"Timestamp background:", "Fondo de marca de tiempo:"},
            {"Timestamp position:", "Posición de marca de tiempo:"},
            {"Timestamp text color:", "Color de marca de tiempo:"},
            {"Timestamp:", "Marca de tiempo:"},
            {"Timestep:", "Intervalo:"},
            {"Title text color:", "Color del título:"},
            {"Title text:", "Texto del título:"},
            {"Upper Left", "Arriba izquierda"},
            {"Upper Right", "Arriba derecha"},
            {"Use specific timestep", "Usar intervalo fijo"},
            {"View", "Ver"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Ancho:"},
            {"English", "Inglés"},
            {"French", "Francés"},
            {"German", "Alemán"},
            {"Spanish", "Español"},
            {"Dutch", "Neerlandés"},
            {"Czech", "Checo"},
            {"Italian", "Italiano"},
            {"Hungarian", "Húngaro"},
            {"Croatian", "Croata"},
            {"Portuguese", "Portugués"},
            {"Chinese", "Chino"},
            {"Russian", "Ruso"}
        };

        static const QHash<QString, QString> nl = {
            {"%1 rows × %2 columns", "%1 rijen × %2 kolommen"},
            {"About", "Over"},
            {"About Thumbnail me 4b2", "Over Thumbnail me 4b2"},
            {"Accurate (slowest)", "Nauwkeurig (traagst)"},
            {"Add and select a video first.", "Voeg eerst een video toe en selecteer die."},
            {"Add video files", "Videobestanden toevoegen"},
            {"Add video files first.", "Voeg eerst videobestanden toe."},
            {"Added %1 video file(s)", "%1 videobestand(en) toegevoegd"},
            {"Additional title:", "Extra titel:"},
            {"Background color:", "Achtergrondkleur:"},
            {"Balanced (safer for short videos)", "Gebalanceerd (veiliger voor korte video’s)"},
            {"Batch cancellation requested...", "Annuleren van batch aangevraagd..."},
            {"Browse...", "Bladeren..."},
            {"Cancel batch", "Batch annuleren"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Annuleren aangevraagd. Wachten tot de huidige worker stopt..."},
            {"Choose count text color", "Kleur van tellertekst kiezen"},
            {"Choose font...", "Lettertype kiezen..."},
            {"Choose info text color", "Kleur van infotekst kiezen"},
            {"Choose tile border color", "Kleur van tegelrand kiezen"},
            {"Choose timestamp background color", "Achtergrondkleur van tijdstempel kiezen"},
            {"Choose timestamp text color", "Kleur van tijdstempel kiezen"},
            {"Choose title text color", "Kleur van titel kiezen"},
            {"Choose...", "Kiezen..."},
            {"Clear preview", "Voorbeeld wissen"},
            {"Columns:", "Kolommen:"},
            {"Config 1", "Config 1"},
            {"Config 2", "Config 2"},
            {"Config 3", "Config 3"},
            {"Contact sheet preview", "Voorbeeld van contactblad"},
            {"Count text color:", "Kleur tellertekst:"},
            {"Delete Selection", "Selectie verwijderen"},
            {"Draw tile border", "Tegelrand tekenen"},
            {"Edition", "Bewerken"},
            {"End margin:", "Eindmarge:"},
            {"Exit", "Afsluiten"},
            {"Fast (Thumbnail me 3.0 style)", "Snel (Thumbnail me 3.0-stijl)"},
            {"File", "Bestand"},
            {"Font:", "Lettertype:"},
            {"Gap:", "Tussenruimte:"},
            {"Generate a contact sheet first.", "Genereer eerst een contactblad."},
            {"Info position:", "Infopositie:"},
            {"Info text color:", "Kleur infotekst:"},
            {"Info text:", "Infotekst:"},
            {"Input / Output", "Invoer / uitvoer"},
            {"Input list cleared", "Invoerlijst gewist"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Taal gewijzigd. Sla de instellingen op en herstart de toepassing om dit volledig toe te passen."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "De taalkeuze wordt direct opgeslagen in settings.ini. Herstart de toepassing om alle statische labels toe te passen."},
            {"Language:", "Taal:"},
            {"Load", "Laden"},
            {"Load video files...", "Videobestanden laden..."},
            {"Load videos and click Start selected or Start all.", "Laad video’s en klik op Geselecteerde starten of Alles starten."},
            {"Lower Left", "Linksonder"},
            {"Lower Right", "Rechtsonder"},
            {"No contact sheet to save", "Geen contactblad om op te slaan"},
            {"Open Recent Configuration", "Recente configuratie openen"},
            {"Open configuration...", "Configuratie openen..."},
            {"Output", "Uitvoer"},
            {"Output Format:", "Uitvoerformaat:"},
            {"Output filename suffix:", "Achtervoegsel uitvoerbestandsnaam:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Uitvoerpad:"},
            {"Predefined Configuration", "Voorgedefinieerde configuratie"},
            {"Preset applied: %1 rows × %2 columns", "Voorinstelling toegepast: %1 rijen × %2 kolommen"},
            {"Preview area", "Voorbeeldgebied"},
            {"Preview style", "Stijlvoorbeeld"},
            {"Quality:", "Kwaliteit:"},
            {"Randomize amount:", "Willekeurige hoeveelheid:"},
            {"Randomize positions", "Posities willekeurig maken"},
            {"Randomize:", "Willekeurig:"},
            {"Reload settings.ini", "settings.ini herladen"},
            {"Reset", "Resetten"},
            {"Rows:", "Rijen:"},
            {"Same folder as source video", "Zelfde map als bronvideo"},
            {"Same source folder:", "Zelfde bronmap:"},
            {"Save configuration", "Configuratie opslaan"},
            {"Save configuration as...", "Configuratie opslaan als..."},
            {"Save contact sheet", "Contactblad opslaan"},
            {"Save failed", "Opslaan mislukt"},
            {"Save manually", "Handmatig opslaan"},
            {"Save selected contact sheet...", "Geselecteerd contactblad opslaan..."},
            {"Save settings.ini", "settings.ini opslaan"},
            {"Saved: ", "Opgeslagen: "},
            {"Seek mode:", "Zoekmodus:"},
            {"Select output folder", "Uitvoermap selecteren"},
            {"Selected video file(s) removed", "Geselecteerde videobestand(en) verwijderd"},
            {"Settings saved:\n", "Instellingen opgeslagen:\n"},
            {"Show additional title", "Extra titel tonen"},
            {"Show technical info text", "Technische infotekst tonen"},
            {"Show timestamps", "Tijdstempels tonen"},
            {"Similarity threshold:", "Drempel voor gelijkenis:"},
            {"Skip dark / blank frames", "Donkere / lege frames overslaan"},
            {"Skip dark:", "Donkere overslaan:"},
            {"Skip similar frames", "Vergelijkbare frames overslaan"},
            {"Skip similar:", "Vergelijkbare overslaan:"},
            {"Specific timestep:", "Vaste tijdstap:"},
            {"Start all", "Alles starten"},
            {"Start margin:", "Startmarge:"},
            {"Start selected", "Geselecteerde starten"},
            {"Style preview failed", "Stijlvoorbeeld mislukt"},
            {"Style preview ready", "Stijlvoorbeeld gereed"},
            {"Styles", "Stijlen"},
            {"The file could not be saved.", "Het bestand kon niet worden opgeslagen."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Kleur tegelrand:"},
            {"Tile border:", "Tegelrand:"},
            {"Timestamp background:", "Achtergrond tijdstempel:"},
            {"Timestamp position:", "Positie tijdstempel:"},
            {"Timestamp text color:", "Kleur tijdstempel:"},
            {"Timestamp:", "Tijdstempel:"},
            {"Timestep:", "Tijdstap:"},
            {"Title text color:", "Kleur titeltekst:"},
            {"Title text:", "Titeltekst:"},
            {"Upper Left", "Linksboven"},
            {"Upper Right", "Rechtsboven"},
            {"Use specific timestep", "Vaste tijdstap gebruiken"},
            {"View", "Beeld"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Breedte:"},
            {"English", "Engels"},
            {"French", "Frans"},
            {"German", "Duits"},
            {"Spanish", "Spaans"},
            {"Dutch", "Nederlands"},
            {"Czech", "Tsjechisch"},
            {"Italian", "Italiaans"},
            {"Hungarian", "Hongaars"},
            {"Croatian", "Kroatisch"},
            {"Portuguese", "Portugees"},
            {"Chinese", "Chinees"},
            {"Russian", "Russisch"}
        };

        static const QHash<QString, QString> it = {
            {"%1 rows × %2 columns", "%1 righe × %2 colonne"},
            {"About", "Informazioni"},
            {"About Thumbnail me 4b2", "Informazioni su Thumbnail me 4b2"},
            {"Accurate (slowest)", "Accurata (più lenta)"},
            {"Add and select a video first.", "Aggiungi e seleziona prima un video."},
            {"Add video files", "Aggiungi file video"},
            {"Add video files first.", "Aggiungi prima dei file video."},
            {"Added %1 video file(s)", "Aggiunti %1 file video"},
            {"Additional title:", "Titolo aggiuntivo:"},
            {"Background color:", "Colore di sfondo:"},
            {"Balanced (safer for short videos)", "Bilanciata (più sicura per video brevi)"},
            {"Batch cancellation requested...", "Annullamento batch richiesto..."},
            {"Browse...", "Sfoglia..."},
            {"Cancel batch", "Annulla batch"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Annullamento richiesto. Attendo l’arresto del worker corrente..."},
            {"Choose count text color", "Scegli colore del conteggio"},
            {"Choose font...", "Scegli carattere..."},
            {"Choose info text color", "Scegli colore del testo info"},
            {"Choose tile border color", "Scegli colore bordo miniatura"},
            {"Choose timestamp background color", "Scegli colore sfondo timestamp"},
            {"Choose timestamp text color", "Scegli colore timestamp"},
            {"Choose title text color", "Scegli colore titolo"},
            {"Choose...", "Scegli..."},
            {"Clear preview", "Pulisci anteprima"},
            {"Columns:", "Colonne:"},
            {"Config 1", "Config 1"},
            {"Config 2", "Config 2"},
            {"Config 3", "Config 3"},
            {"Contact sheet preview", "Anteprima provino"},
            {"Count text color:", "Colore conteggio:"},
            {"Delete Selection", "Elimina selezione"},
            {"Draw tile border", "Disegna bordo miniatura"},
            {"Edition", "Modifica"},
            {"End margin:", "Margine finale:"},
            {"Exit", "Esci"},
            {"Fast (Thumbnail me 3.0 style)", "Veloce (stile Thumbnail me 3.0)"},
            {"File", "File"},
            {"Font:", "Carattere:"},
            {"Gap:", "Spazio:"},
            {"Generate a contact sheet first.", "Genera prima un provino."},
            {"Info position:", "Posizione info:"},
            {"Info text color:", "Colore testo info:"},
            {"Info text:", "Testo info:"},
            {"Input / Output", "Input / output"},
            {"Input list cleared", "Elenco input svuotato"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Lingua cambiata. Salva le impostazioni e riavvia l’applicazione per applicarla completamente."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "La lingua viene salvata subito in settings.ini. Riavvia l’applicazione per applicare tutte le etichette statiche."},
            {"Language:", "Lingua:"},
            {"Load", "Carica"},
            {"Load video files...", "Carica file video..."},
            {"Load videos and click Start selected or Start all.", "Carica i video e fai clic su Avvia selezionati o Avvia tutti."},
            {"Lower Left", "In basso a sinistra"},
            {"Lower Right", "In basso a destra"},
            {"No contact sheet to save", "Nessun provino da salvare"},
            {"Open Recent Configuration", "Apri configurazione recente"},
            {"Open configuration...", "Apri configurazione..."},
            {"Output", "Output"},
            {"Output Format:", "Formato output:"},
            {"Output filename suffix:", "Suffisso nome file output:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Percorso output:"},
            {"Predefined Configuration", "Configurazione predefinita"},
            {"Preset applied: %1 rows × %2 columns", "Preset applicato: %1 righe × %2 colonne"},
            {"Preview area", "Area anteprima"},
            {"Preview style", "Anteprima stile"},
            {"Quality:", "Qualità:"},
            {"Randomize amount:", "Quantità casuale:"},
            {"Randomize positions", "Posizioni casuali"},
            {"Randomize:", "Casuale:"},
            {"Reload settings.ini", "Ricarica settings.ini"},
            {"Reset", "Ripristina"},
            {"Rows:", "Righe:"},
            {"Same folder as source video", "Stessa cartella del video sorgente"},
            {"Same source folder:", "Stessa cartella sorgente:"},
            {"Save configuration", "Salva configurazione"},
            {"Save configuration as...", "Salva configurazione con nome..."},
            {"Save contact sheet", "Salva provino"},
            {"Save failed", "Salvataggio non riuscito"},
            {"Save manually", "Salva manualmente"},
            {"Save selected contact sheet...", "Salva provino selezionato..."},
            {"Save settings.ini", "Salva settings.ini"},
            {"Saved: ", "Salvato: "},
            {"Seek mode:", "Modalità seek:"},
            {"Select output folder", "Seleziona cartella di output"},
            {"Selected video file(s) removed", "File video selezionati rimossi"},
            {"Settings saved:\n", "Impostazioni salvate:\n"},
            {"Show additional title", "Mostra titolo aggiuntivo"},
            {"Show technical info text", "Mostra testo tecnico"},
            {"Show timestamps", "Mostra timestamp"},
            {"Similarity threshold:", "Soglia di somiglianza:"},
            {"Skip dark / blank frames", "Salta fotogrammi scuri / vuoti"},
            {"Skip dark:", "Salta scuri:"},
            {"Skip similar frames", "Salta fotogrammi simili"},
            {"Skip similar:", "Salta simili:"},
            {"Specific timestep:", "Intervallo fisso:"},
            {"Start all", "Avvia tutti"},
            {"Start margin:", "Margine iniziale:"},
            {"Start selected", "Avvia selezionati"},
            {"Style preview failed", "Anteprima stile non riuscita"},
            {"Style preview ready", "Anteprima stile pronta"},
            {"Styles", "Stili"},
            {"The file could not be saved.", "Impossibile salvare il file."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Colore bordo miniatura:"},
            {"Tile border:", "Bordo miniatura:"},
            {"Timestamp background:", "Sfondo timestamp:"},
            {"Timestamp position:", "Posizione timestamp:"},
            {"Timestamp text color:", "Colore timestamp:"},
            {"Timestamp:", "Timestamp:"},
            {"Timestep:", "Intervallo:"},
            {"Title text color:", "Colore titolo:"},
            {"Title text:", "Testo titolo:"},
            {"Upper Left", "In alto a sinistra"},
            {"Upper Right", "In alto a destra"},
            {"Use specific timestep", "Usa intervallo fisso"},
            {"View", "Visualizza"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Larghezza:"},
            {"English", "Inglese"},
            {"French", "Francese"},
            {"German", "Tedesco"},
            {"Spanish", "Spagnolo"},
            {"Dutch", "Olandese"},
            {"Czech", "Ceco"},
            {"Italian", "Italiano"},
            {"Hungarian", "Ungherese"},
            {"Croatian", "Croato"},
            {"Portuguese", "Portoghese"},
            {"Chinese", "Cinese"},
            {"Russian", "Russo"}
        };

        static const QHash<QString, QString> hu = {
            {"%1 rows × %2 columns", "%1 sor × %2 oszlop"},
            {"About", "Névjegy"},
            {"About Thumbnail me 4b2", "Thumbnail me 4b2 névjegye"},
            {"Accurate (slowest)", "Pontos (leglassabb)"},
            {"Add and select a video first.", "Előbb adj hozzá és válassz ki egy videót."},
            {"Add video files", "Videófájlok hozzáadása"},
            {"Add video files first.", "Előbb adj hozzá videófájlokat."},
            {"Added %1 video file(s)", "%1 videófájl hozzáadva"},
            {"Additional title:", "Kiegészítő cím:"},
            {"Background color:", "Háttérszín:"},
            {"Balanced (safer for short videos)", "Kiegyensúlyozott (biztonságosabb rövid videóknál)"},
            {"Batch cancellation requested...", "Kötegelt feldolgozás megszakítása kérve..."},
            {"Browse...", "Tallózás..."},
            {"Cancel batch", "Köteg megszakítása"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Megszakítás kérve. Várakozás az aktuális worker leállására..."},
            {"Choose count text color", "Darabszöveg színének kiválasztása"},
            {"Choose font...", "Betűtípus kiválasztása..."},
            {"Choose info text color", "Infó szöveg színének kiválasztása"},
            {"Choose tile border color", "Csempekeret színének kiválasztása"},
            {"Choose timestamp background color", "Időbélyeg hátterének kiválasztása"},
            {"Choose timestamp text color", "Időbélyeg színének kiválasztása"},
            {"Choose title text color", "Cím színének kiválasztása"},
            {"Choose...", "Kiválasztás..."},
            {"Clear preview", "Előnézet törlése"},
            {"Columns:", "Oszlopok:"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Contact sheet preview", "Kontaktlap előnézete"},
            {"Count text color:", "Darabszöveg színe:"},
            {"Delete Selection", "Kijelölés törlése"},
            {"Draw tile border", "Csempekeret rajzolása"},
            {"Edition", "Szerkesztés"},
            {"End margin:", "Vég margó:"},
            {"Exit", "Kilépés"},
            {"Fast (Thumbnail me 3.0 style)", "Gyors (Thumbnail me 3.0 stílus)"},
            {"File", "Fájl"},
            {"Font:", "Betűtípus:"},
            {"Gap:", "Rés:"},
            {"Generate a contact sheet first.", "Előbb hozz létre egy kontaktlapot."},
            {"Info position:", "Infó pozíciója:"},
            {"Info text color:", "Infó szöveg színe:"},
            {"Info text:", "Infó szöveg:"},
            {"Input / Output", "Bemenet / kimenet"},
            {"Input list cleared", "Bemeneti lista törölve"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "A nyelv megváltozott. Mentsd a beállításokat és indítsd újra az alkalmazást a teljes alkalmazáshoz."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "A nyelvválasztás azonnal mentésre kerül a settings.ini fájlba. Indítsd újra az alkalmazást az összes statikus felirat alkalmazásához."},
            {"Language:", "Nyelv:"},
            {"Load", "Betöltés"},
            {"Load video files...", "Videófájlok betöltése..."},
            {"Load videos and click Start selected or Start all.", "Tölts be videókat, majd kattints a Kijelöltek indítása vagy Összes indítása gombra."},
            {"Lower Left", "Bal alsó"},
            {"Lower Right", "Jobb alsó"},
            {"No contact sheet to save", "Nincs menthető kontaktlap"},
            {"Open Recent Configuration", "Legutóbbi konfiguráció megnyitása"},
            {"Open configuration...", "Konfiguráció megnyitása..."},
            {"Output", "Kimenet"},
            {"Output Format:", "Kimeneti formátum:"},
            {"Output filename suffix:", "Kimeneti fájlnév utótagja:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Kimeneti útvonal:"},
            {"Predefined Configuration", "Előre definiált konfiguráció"},
            {"Preset applied: %1 rows × %2 columns", "Előbeállítás alkalmazva: %1 sor × %2 oszlop"},
            {"Preview area", "Előnézeti terület"},
            {"Preview style", "Stílus előnézete"},
            {"Quality:", "Minőség:"},
            {"Randomize amount:", "Véletlenszerűség mértéke:"},
            {"Randomize positions", "Pozíciók véletlenítése"},
            {"Randomize:", "Véletlenítés:"},
            {"Reload settings.ini", "settings.ini újratöltése"},
            {"Reset", "Alaphelyzet"},
            {"Rows:", "Sorok:"},
            {"Same folder as source video", "Ugyanaz a mappa, mint a forrásvideó"},
            {"Same source folder:", "Ugyanaz a forrásmappa:"},
            {"Save configuration", "Konfiguráció mentése"},
            {"Save configuration as...", "Konfiguráció mentése másként..."},
            {"Save contact sheet", "Kontaktlap mentése"},
            {"Save failed", "Mentés sikertelen"},
            {"Save manually", "Mentés kézzel"},
            {"Save selected contact sheet...", "Kijelölt kontaktlap mentése..."},
            {"Save settings.ini", "settings.ini mentése"},
            {"Saved: ", "Mentve: "},
            {"Seek mode:", "Keresési mód:"},
            {"Select output folder", "Kimeneti mappa kiválasztása"},
            {"Selected video file(s) removed", "Kijelölt videófájl(ok) eltávolítva"},
            {"Settings saved:\n", "Beállítások mentve:\n"},
            {"Show additional title", "Kiegészítő cím megjelenítése"},
            {"Show technical info text", "Technikai infó szöveg megjelenítése"},
            {"Show timestamps", "Időbélyegek megjelenítése"},
            {"Similarity threshold:", "Hasonlósági küszöb:"},
            {"Skip dark / blank frames", "Sötét / üres képkockák kihagyása"},
            {"Skip dark:", "Sötétek kihagyása:"},
            {"Skip similar frames", "Hasonló képkockák kihagyása"},
            {"Skip similar:", "Hasonlók kihagyása:"},
            {"Specific timestep:", "Rögzített időlépés:"},
            {"Start all", "Összes indítása"},
            {"Start margin:", "Kezdő margó:"},
            {"Start selected", "Kijelöltek indítása"},
            {"Style preview failed", "Stílus előnézete sikertelen"},
            {"Style preview ready", "Stílus előnézete kész"},
            {"Styles", "Stílusok"},
            {"The file could not be saved.", "A fájlt nem sikerült menteni."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Csempekeret színe:"},
            {"Tile border:", "Csempekeret:"},
            {"Timestamp background:", "Időbélyeg háttere:"},
            {"Timestamp position:", "Időbélyeg pozíciója:"},
            {"Timestamp text color:", "Időbélyeg színe:"},
            {"Timestamp:", "Időbélyeg:"},
            {"Timestep:", "Időlépés:"},
            {"Title text color:", "Cím színe:"},
            {"Title text:", "Cím szövege:"},
            {"Upper Left", "Bal felső"},
            {"Upper Right", "Jobb felső"},
            {"Use specific timestep", "Rögzített időlépés használata"},
            {"View", "Nézet"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Szélesség:"},
            {"English", "Angol"},
            {"French", "Francia"},
            {"German", "Német"},
            {"Spanish", "Spanyol"},
            {"Dutch", "Holland"},
            {"Czech", "Cseh"},
            {"Italian", "Olasz"},
            {"Hungarian", "Magyar"},
            {"Croatian", "Horvát"},
            {"Portuguese", "Portugál"},
            {"Chinese", "Kínai"},
            {"Russian", "Orosz"}
        };

        static const QHash<QString, QString> hr = {
            {"%1 rows × %2 columns", "%1 redaka × %2 stupca"},
            {"About", "O programu"},
            {"About Thumbnail me 4b2", "O programu Thumbnail me 4b2"},
            {"Accurate (slowest)", "Precizno (najsporije)"},
            {"Add and select a video first.", "Najprije dodajte i odaberite video."},
            {"Add video files", "Dodaj video datoteke"},
            {"Add video files first.", "Najprije dodajte video datoteke."},
            {"Added %1 video file(s)", "Dodano %1 video datoteka"},
            {"Additional title:", "Dodatni naslov:"},
            {"Background color:", "Boja pozadine:"},
            {"Balanced (safer for short videos)", "Uravnoteženo (sigurnije za kratke videozapise)"},
            {"Batch cancellation requested...", "Zatraženo otkazivanje serije..."},
            {"Browse...", "Pregledaj..."},
            {"Cancel batch", "Otkaži seriju"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Otkazivanje zatraženo. Čekam zaustavljanje trenutnog workera..."},
            {"Choose count text color", "Odaberi boju teksta brojača"},
            {"Choose font...", "Odaberi font..."},
            {"Choose info text color", "Odaberi boju info teksta"},
            {"Choose tile border color", "Odaberi boju okvira sličice"},
            {"Choose timestamp background color", "Odaberi boju pozadine vremenske oznake"},
            {"Choose timestamp text color", "Odaberi boju vremenske oznake"},
            {"Choose title text color", "Odaberi boju naslova"},
            {"Choose...", "Odaberi..."},
            {"Clear preview", "Očisti pregled"},
            {"Columns:", "Stupci:"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Contact sheet preview", "Pregled kontaktne ploče"},
            {"Count text color:", "Boja teksta brojača:"},
            {"Delete Selection", "Obriši odabir"},
            {"Draw tile border", "Crtaj okvir sličice"},
            {"Edition", "Uređivanje"},
            {"End margin:", "Završna margina:"},
            {"Exit", "Izlaz"},
            {"Fast (Thumbnail me 3.0 style)", "Brzo (stil Thumbnail me 3.0)"},
            {"File", "Datoteka"},
            {"Font:", "Font:"},
            {"Gap:", "Razmak:"},
            {"Generate a contact sheet first.", "Najprije generirajte kontaktnu ploču."},
            {"Info position:", "Položaj info teksta:"},
            {"Info text color:", "Boja info teksta:"},
            {"Info text:", "Info tekst:"},
            {"Input / Output", "Ulaz / izlaz"},
            {"Input list cleared", "Popis ulaza očišćen"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jezik je promijenjen. Spremite postavke i ponovno pokrenite aplikaciju za potpunu primjenu."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Odabir jezika odmah se sprema u settings.ini. Ponovno pokrenite aplikaciju za potpunu primjenu statičkih oznaka."},
            {"Language:", "Jezik:"},
            {"Load", "Učitaj"},
            {"Load video files...", "Učitaj video datoteke..."},
            {"Load videos and click Start selected or Start all.", "Učitajte videozapise i kliknite Pokreni odabrano ili Pokreni sve."},
            {"Lower Left", "Dolje lijevo"},
            {"Lower Right", "Dolje desno"},
            {"No contact sheet to save", "Nema kontaktne ploče za spremanje"},
            {"Open Recent Configuration", "Otvori nedavnu konfiguraciju"},
            {"Open configuration...", "Otvori konfiguraciju..."},
            {"Output", "Izlaz"},
            {"Output Format:", "Izlazni format:"},
            {"Output filename suffix:", "Sufiks izlaznog naziva datoteke:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Izlazna putanja:"},
            {"Predefined Configuration", "Predefinirana konfiguracija"},
            {"Preset applied: %1 rows × %2 columns", "Predložak primijenjen: %1 redaka × %2 stupca"},
            {"Preview area", "Područje pregleda"},
            {"Preview style", "Pregled stila"},
            {"Quality:", "Kvaliteta:"},
            {"Randomize amount:", "Količina slučajnosti:"},
            {"Randomize positions", "Nasumične pozicije"},
            {"Randomize:", "Nasumično:"},
            {"Reload settings.ini", "Ponovno učitaj settings.ini"},
            {"Reset", "Resetiraj"},
            {"Rows:", "Redci:"},
            {"Same folder as source video", "Ista mapa kao izvorni video"},
            {"Same source folder:", "Ista izvorna mapa:"},
            {"Save configuration", "Spremi konfiguraciju"},
            {"Save configuration as...", "Spremi konfiguraciju kao..."},
            {"Save contact sheet", "Spremi kontaktnu ploču"},
            {"Save failed", "Spremanje nije uspjelo"},
            {"Save manually", "Spremi ručno"},
            {"Save selected contact sheet...", "Spremi odabranu kontaktnu ploču..."},
            {"Save settings.ini", "Spremi settings.ini"},
            {"Saved: ", "Spremljeno: "},
            {"Seek mode:", "Način traženja:"},
            {"Select output folder", "Odaberi izlaznu mapu"},
            {"Selected video file(s) removed", "Odabrane video datoteke uklonjene"},
            {"Settings saved:\n", "Postavke spremljene:\n"},
            {"Show additional title", "Prikaži dodatni naslov"},
            {"Show technical info text", "Prikaži tehnički info tekst"},
            {"Show timestamps", "Prikaži vremenske oznake"},
            {"Similarity threshold:", "Prag sličnosti:"},
            {"Skip dark / blank frames", "Preskoči tamne / prazne kadrove"},
            {"Skip dark:", "Preskoči tamne:"},
            {"Skip similar frames", "Preskoči slične kadrove"},
            {"Skip similar:", "Preskoči slične:"},
            {"Specific timestep:", "Fiksni vremenski korak:"},
            {"Start all", "Pokreni sve"},
            {"Start margin:", "Početna margina:"},
            {"Start selected", "Pokreni odabrano"},
            {"Style preview failed", "Pregled stila nije uspio"},
            {"Style preview ready", "Pregled stila spreman"},
            {"Styles", "Stilovi"},
            {"The file could not be saved.", "Datoteka se nije mogla spremiti."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Boja okvira sličice:"},
            {"Tile border:", "Okvir sličice:"},
            {"Timestamp background:", "Pozadina vremenske oznake:"},
            {"Timestamp position:", "Položaj vremenske oznake:"},
            {"Timestamp text color:", "Boja vremenske oznake:"},
            {"Timestamp:", "Vremenska oznaka:"},
            {"Timestep:", "Vremenski korak:"},
            {"Title text color:", "Boja naslova:"},
            {"Title text:", "Tekst naslova:"},
            {"Upper Left", "Gore lijevo"},
            {"Upper Right", "Gore desno"},
            {"Use specific timestep", "Koristi fiksni vremenski korak"},
            {"View", "Prikaz"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Širina:"},
            {"English", "Engleski"},
            {"French", "Francuski"},
            {"German", "Njemački"},
            {"Spanish", "Španjolski"},
            {"Dutch", "Nizozemski"},
            {"Czech", "Češki"},
            {"Italian", "Talijanski"},
            {"Hungarian", "Mađarski"},
            {"Croatian", "Hrvatski"},
            {"Portuguese", "Portugalski"},
            {"Chinese", "Kineski"},
            {"Russian", "Ruski"}
        };

        static const QHash<QString, QString> pt = {
            {"%1 rows × %2 columns", "%1 linhas × %2 colunas"},
            {"About", "Sobre"},
            {"About Thumbnail me 4b2", "Sobre o Thumbnail me 4b2"},
            {"Accurate (slowest)", "Preciso (mais lento)"},
            {"Add and select a video first.", "Adicione e selecione primeiro um vídeo."},
            {"Add video files", "Adicionar ficheiros de vídeo"},
            {"Add video files first.", "Adicione primeiro ficheiros de vídeo."},
            {"Added %1 video file(s)", "%1 ficheiro(s) de vídeo adicionado(s)"},
            {"Additional title:", "Título adicional:"},
            {"Background color:", "Cor de fundo:"},
            {"Balanced (safer for short videos)", "Equilibrado (mais seguro para vídeos curtos)"},
            {"Batch cancellation requested...", "Cancelamento do lote solicitado..."},
            {"Browse...", "Procurar..."},
            {"Cancel batch", "Cancelar lote"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Cancelamento solicitado. A aguardar que o worker atual pare..."},
            {"Choose count text color", "Escolher cor do texto de contagem"},
            {"Choose font...", "Escolher fonte..."},
            {"Choose info text color", "Escolher cor do texto de informação"},
            {"Choose tile border color", "Escolher cor da borda da miniatura"},
            {"Choose timestamp background color", "Escolher cor de fundo da marca temporal"},
            {"Choose timestamp text color", "Escolher cor da marca temporal"},
            {"Choose title text color", "Escolher cor do título"},
            {"Choose...", "Escolher..."},
            {"Clear preview", "Limpar pré-visualização"},
            {"Columns:", "Colunas:"},
            {"Config 1", "Config 1"},
            {"Config 2", "Config 2"},
            {"Config 3", "Config 3"},
            {"Contact sheet preview", "Pré-visualização da folha de contactos"},
            {"Count text color:", "Cor do texto de contagem:"},
            {"Delete Selection", "Apagar seleção"},
            {"Draw tile border", "Desenhar borda da miniatura"},
            {"Edition", "Editar"},
            {"End margin:", "Margem final:"},
            {"Exit", "Sair"},
            {"Fast (Thumbnail me 3.0 style)", "Rápido (estilo Thumbnail me 3.0)"},
            {"File", "Ficheiro"},
            {"Font:", "Fonte:"},
            {"Gap:", "Espaço:"},
            {"Generate a contact sheet first.", "Gere primeiro uma folha de contactos."},
            {"Info position:", "Posição da informação:"},
            {"Info text color:", "Cor do texto de informação:"},
            {"Info text:", "Texto de informação:"},
            {"Input / Output", "Entrada / saída"},
            {"Input list cleared", "Lista de entrada limpa"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Idioma alterado. Guarde as definições e reinicie a aplicação para aplicar completamente."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "A seleção de idioma é guardada imediatamente em settings.ini. Reinicie a aplicação para aplicar todos os rótulos estáticos."},
            {"Language:", "Idioma:"},
            {"Load", "Carregar"},
            {"Load video files...", "Carregar ficheiros de vídeo..."},
            {"Load videos and click Start selected or Start all.", "Carregue vídeos e clique em Iniciar selecionados ou Iniciar tudo."},
            {"Lower Left", "Inferior esquerdo"},
            {"Lower Right", "Inferior direito"},
            {"No contact sheet to save", "Não há folha de contactos para guardar"},
            {"Open Recent Configuration", "Abrir configuração recente"},
            {"Open configuration...", "Abrir configuração..."},
            {"Output", "Saída"},
            {"Output Format:", "Formato de saída:"},
            {"Output filename suffix:", "Sufixo do nome de ficheiro de saída:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Caminho de saída:"},
            {"Predefined Configuration", "Configuração predefinida"},
            {"Preset applied: %1 rows × %2 columns", "Predefinição aplicada: %1 linhas × %2 colunas"},
            {"Preview area", "Área de pré-visualização"},
            {"Preview style", "Pré-visualizar estilo"},
            {"Quality:", "Qualidade:"},
            {"Randomize amount:", "Quantidade aleatória:"},
            {"Randomize positions", "Aleatorizar posições"},
            {"Randomize:", "Aleatorizar:"},
            {"Reload settings.ini", "Recarregar settings.ini"},
            {"Reset", "Repor"},
            {"Rows:", "Linhas:"},
            {"Same folder as source video", "Mesma pasta do vídeo de origem"},
            {"Same source folder:", "Mesma pasta de origem:"},
            {"Save configuration", "Guardar configuração"},
            {"Save configuration as...", "Guardar configuração como..."},
            {"Save contact sheet", "Guardar folha de contactos"},
            {"Save failed", "Falha ao guardar"},
            {"Save manually", "Guardar manualmente"},
            {"Save selected contact sheet...", "Guardar folha de contactos selecionada..."},
            {"Save settings.ini", "Guardar settings.ini"},
            {"Saved: ", "Guardado: "},
            {"Seek mode:", "Modo de procura:"},
            {"Select output folder", "Selecionar pasta de saída"},
            {"Selected video file(s) removed", "Ficheiro(s) de vídeo selecionado(s) removido(s)"},
            {"Settings saved:\n", "Definições guardadas:\n"},
            {"Show additional title", "Mostrar título adicional"},
            {"Show technical info text", "Mostrar texto técnico"},
            {"Show timestamps", "Mostrar marcas temporais"},
            {"Similarity threshold:", "Limiar de semelhança:"},
            {"Skip dark / blank frames", "Ignorar frames escuros / vazios"},
            {"Skip dark:", "Ignorar escuros:"},
            {"Skip similar frames", "Ignorar frames semelhantes"},
            {"Skip similar:", "Ignorar semelhantes:"},
            {"Specific timestep:", "Intervalo fixo:"},
            {"Start all", "Iniciar tudo"},
            {"Start margin:", "Margem inicial:"},
            {"Start selected", "Iniciar selecionados"},
            {"Style preview failed", "Falha na pré-visualização do estilo"},
            {"Style preview ready", "Pré-visualização do estilo pronta"},
            {"Styles", "Estilos"},
            {"The file could not be saved.", "Não foi possível guardar o ficheiro."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Cor da borda da miniatura:"},
            {"Tile border:", "Borda da miniatura:"},
            {"Timestamp background:", "Fundo da marca temporal:"},
            {"Timestamp position:", "Posição da marca temporal:"},
            {"Timestamp text color:", "Cor da marca temporal:"},
            {"Timestamp:", "Marca temporal:"},
            {"Timestep:", "Intervalo:"},
            {"Title text color:", "Cor do título:"},
            {"Title text:", "Texto do título:"},
            {"Upper Left", "Superior esquerdo"},
            {"Upper Right", "Superior direito"},
            {"Use specific timestep", "Usar intervalo fixo"},
            {"View", "Ver"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Largura:"},
            {"English", "Inglês"},
            {"French", "Francês"},
            {"German", "Alemão"},
            {"Spanish", "Espanhol"},
            {"Dutch", "Neerlandês"},
            {"Czech", "Checo"},
            {"Italian", "Italiano"},
            {"Hungarian", "Húngaro"},
            {"Croatian", "Croata"},
            {"Portuguese", "Português"},
            {"Chinese", "Chinês"},
            {"Russian", "Russo"}
        };

        static const QHash<QString, QString> zh = {
            {"%1 rows × %2 columns", "%1 行 × %2 列"},
            {"About", "关于"},
            {"About Thumbnail me 4b2", "关于 Thumbnail me 4b2"},
            {"Accurate (slowest)", "精确（最慢）"},
            {"Add and select a video first.", "请先添加并选择一个视频。"},
            {"Add video files", "添加视频文件"},
            {"Add video files first.", "请先添加视频文件。"},
            {"Added %1 video file(s)", "已添加 %1 个视频文件"},
            {"Additional title:", "附加标题："},
            {"Background color:", "背景颜色："},
            {"Balanced (safer for short videos)", "平衡（对短视频更安全）"},
            {"Batch cancellation requested...", "已请求取消批处理..."},
            {"Browse...", "浏览..."},
            {"Cancel batch", "取消批处理"},
            {"Cancellation requested. Waiting for the current worker to stop...", "已请求取消，正在等待当前 worker 停止..."},
            {"Choose count text color", "选择计数文字颜色"},
            {"Choose font...", "选择字体..."},
            {"Choose info text color", "选择信息文字颜色"},
            {"Choose tile border color", "选择缩略图边框颜色"},
            {"Choose timestamp background color", "选择时间戳背景颜色"},
            {"Choose timestamp text color", "选择时间戳文字颜色"},
            {"Choose title text color", "选择标题文字颜色"},
            {"Choose...", "选择..."},
            {"Clear preview", "清除预览"},
            {"Columns:", "列："},
            {"Config 1", "配置 1"},
            {"Config 2", "配置 2"},
            {"Config 3", "配置 3"},
            {"Contact sheet preview", "联系表预览"},
            {"Count text color:", "计数文字颜色："},
            {"Delete Selection", "删除所选"},
            {"Draw tile border", "绘制缩略图边框"},
            {"Edition", "编辑"},
            {"End margin:", "结尾边距："},
            {"Exit", "退出"},
            {"Fast (Thumbnail me 3.0 style)", "快速（Thumbnail me 3.0 风格）"},
            {"File", "文件"},
            {"Font:", "字体："},
            {"Gap:", "间距："},
            {"Generate a contact sheet first.", "请先生成联系表。"},
            {"Info position:", "信息位置："},
            {"Info text color:", "信息文字颜色："},
            {"Info text:", "信息文字："},
            {"Input / Output", "输入 / 输出"},
            {"Input list cleared", "输入列表已清空"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "语言已更改。请保存设置并重启应用以完全应用。"},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "语言选择会立即保存到 settings.ini。请重启应用以完全应用静态标签。"},
            {"Language:", "语言："},
            {"Load", "加载"},
            {"Load video files...", "加载视频文件..."},
            {"Load videos and click Start selected or Start all.", "加载视频后点击开始所选或开始全部。"},
            {"Lower Left", "左下"},
            {"Lower Right", "右下"},
            {"No contact sheet to save", "没有可保存的联系表"},
            {"Open Recent Configuration", "打开最近配置"},
            {"Open configuration...", "打开配置..."},
            {"Output", "输出"},
            {"Output Format:", "输出格式："},
            {"Output filename suffix:", "输出文件名后缀："},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "输出路径："},
            {"Predefined Configuration", "预定义配置"},
            {"Preset applied: %1 rows × %2 columns", "已应用预设：%1 行 × %2 列"},
            {"Preview area", "预览区域"},
            {"Preview style", "预览样式"},
            {"Quality:", "质量："},
            {"Randomize amount:", "随机幅度："},
            {"Randomize positions", "随机位置"},
            {"Randomize:", "随机："},
            {"Reload settings.ini", "重新加载 settings.ini"},
            {"Reset", "重置"},
            {"Rows:", "行："},
            {"Same folder as source video", "与源视频相同文件夹"},
            {"Same source folder:", "相同源文件夹："},
            {"Save configuration", "保存配置"},
            {"Save configuration as...", "配置另存为..."},
            {"Save contact sheet", "保存联系表"},
            {"Save failed", "保存失败"},
            {"Save manually", "手动保存"},
            {"Save selected contact sheet...", "保存所选联系表..."},
            {"Save settings.ini", "保存 settings.ini"},
            {"Saved: ", "已保存："},
            {"Seek mode:", "定位模式："},
            {"Select output folder", "选择输出文件夹"},
            {"Selected video file(s) removed", "已移除所选视频文件"},
            {"Settings saved:\n", "设置已保存：\n"},
            {"Show additional title", "显示附加标题"},
            {"Show technical info text", "显示技术信息文字"},
            {"Show timestamps", "显示时间戳"},
            {"Similarity threshold:", "相似度阈值："},
            {"Skip dark / blank frames", "跳过暗帧 / 空白帧"},
            {"Skip dark:", "跳过暗帧："},
            {"Skip similar frames", "跳过相似帧"},
            {"Skip similar:", "跳过相似："},
            {"Specific timestep:", "固定时间步长："},
            {"Start all", "开始全部"},
            {"Start margin:", "开始边距："},
            {"Start selected", "开始所选"},
            {"Style preview failed", "样式预览失败"},
            {"Style preview ready", "样式预览就绪"},
            {"Styles", "样式"},
            {"The file could not be saved.", "无法保存文件。"},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "缩略图边框颜色："},
            {"Tile border:", "缩略图边框："},
            {"Timestamp background:", "时间戳背景："},
            {"Timestamp position:", "时间戳位置："},
            {"Timestamp text color:", "时间戳文字颜色："},
            {"Timestamp:", "时间戳："},
            {"Timestep:", "时间步长："},
            {"Title text color:", "标题文字颜色："},
            {"Title text:", "标题文字："},
            {"Upper Left", "左上"},
            {"Upper Right", "右上"},
            {"Use specific timestep", "使用固定时间步长"},
            {"View", "查看"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "宽度："},
            {"English", "英语"},
            {"French", "法语"},
            {"German", "德语"},
            {"Spanish", "西班牙语"},
            {"Dutch", "荷兰语"},
            {"Czech", "捷克语"},
            {"Italian", "意大利语"},
            {"Hungarian", "匈牙利语"},
            {"Croatian", "克罗地亚语"},
            {"Portuguese", "葡萄牙语"},
            {"Chinese", "中文"},
            {"Russian", "俄语"}
        };

        static const QHash<QString, QString> ru = {
            {"%1 rows × %2 columns", "%1 строк × %2 столбцов"},
            {"About", "О программе"},
            {"About Thumbnail me 4b2", "О Thumbnail me 4b2"},
            {"Accurate (slowest)", "Точный (самый медленный)"},
            {"Add and select a video first.", "Сначала добавьте и выберите видео."},
            {"Add video files", "Добавить видеофайлы"},
            {"Add video files first.", "Сначала добавьте видеофайлы."},
            {"Added %1 video file(s)", "Добавлено видеофайлов: %1"},
            {"Additional title:", "Дополнительный заголовок:"},
            {"Background color:", "Цвет фона:"},
            {"Balanced (safer for short videos)", "Сбалансированный (безопаснее для коротких видео)"},
            {"Batch cancellation requested...", "Запрошена отмена пакетной обработки..."},
            {"Browse...", "Обзор..."},
            {"Cancel batch", "Отменить пакет"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Запрошена отмена. Ожидание остановки текущего worker..."},
            {"Choose count text color", "Выбрать цвет текста счётчика"},
            {"Choose font...", "Выбрать шрифт..."},
            {"Choose info text color", "Выбрать цвет информационного текста"},
            {"Choose tile border color", "Выбрать цвет рамки миниатюры"},
            {"Choose timestamp background color", "Выбрать цвет фона времени"},
            {"Choose timestamp text color", "Выбрать цвет времени"},
            {"Choose title text color", "Выбрать цвет заголовка"},
            {"Choose...", "Выбрать..."},
            {"Clear preview", "Очистить предпросмотр"},
            {"Columns:", "Столбцы:"},
            {"Config 1", "Конфиг 1"},
            {"Config 2", "Конфиг 2"},
            {"Config 3", "Конфиг 3"},
            {"Contact sheet preview", "Предпросмотр листа миниатюр"},
            {"Count text color:", "Цвет текста счётчика:"},
            {"Delete Selection", "Удалить выбранное"},
            {"Draw tile border", "Рисовать рамку миниатюры"},
            {"Edition", "Правка"},
            {"End margin:", "Отступ в конце:"},
            {"Exit", "Выход"},
            {"Fast (Thumbnail me 3.0 style)", "Быстрый (стиль Thumbnail me 3.0)"},
            {"File", "Файл"},
            {"Font:", "Шрифт:"},
            {"Gap:", "Интервал:"},
            {"Generate a contact sheet first.", "Сначала создайте лист миниатюр."},
            {"Info position:", "Позиция информации:"},
            {"Info text color:", "Цвет инфотекста:"},
            {"Info text:", "Инфотекст:"},
            {"Input / Output", "Ввод / вывод"},
            {"Input list cleared", "Список ввода очищен"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Язык изменён. Сохраните настройки и перезапустите приложение для полного применения."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Выбор языка сразу сохраняется в settings.ini. Перезапустите приложение, чтобы применить все статические подписи."},
            {"Language:", "Язык:"},
            {"Load", "Загрузить"},
            {"Load video files...", "Загрузить видеофайлы..."},
            {"Load videos and click Start selected or Start all.", "Загрузите видео и нажмите Запустить выбранные или Запустить всё."},
            {"Lower Left", "Снизу слева"},
            {"Lower Right", "Снизу справа"},
            {"No contact sheet to save", "Нет листа миниатюр для сохранения"},
            {"Open Recent Configuration", "Открыть недавнюю конфигурацию"},
            {"Open configuration...", "Открыть конфигурацию..."},
            {"Output", "Вывод"},
            {"Output Format:", "Формат вывода:"},
            {"Output filename suffix:", "Суффикс имени выходного файла:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Путь вывода:"},
            {"Predefined Configuration", "Предустановленная конфигурация"},
            {"Preset applied: %1 rows × %2 columns", "Пресет применён: %1 строк × %2 столбцов"},
            {"Preview area", "Область предпросмотра"},
            {"Preview style", "Предпросмотр стиля"},
            {"Quality:", "Качество:"},
            {"Randomize amount:", "Степень случайности:"},
            {"Randomize positions", "Случайные позиции"},
            {"Randomize:", "Случайно:"},
            {"Reload settings.ini", "Перезагрузить settings.ini"},
            {"Reset", "Сброс"},
            {"Rows:", "Строки:"},
            {"Same folder as source video", "Та же папка, что и исходное видео"},
            {"Same source folder:", "Та же исходная папка:"},
            {"Save configuration", "Сохранить конфигурацию"},
            {"Save configuration as...", "Сохранить конфигурацию как..."},
            {"Save contact sheet", "Сохранить лист миниатюр"},
            {"Save failed", "Сохранение не удалось"},
            {"Save manually", "Сохранить вручную"},
            {"Save selected contact sheet...", "Сохранить выбранный лист миниатюр..."},
            {"Save settings.ini", "Сохранить settings.ini"},
            {"Saved: ", "Сохранено: "},
            {"Seek mode:", "Режим поиска:"},
            {"Select output folder", "Выбрать папку вывода"},
            {"Selected video file(s) removed", "Выбранные видеофайлы удалены"},
            {"Settings saved:\n", "Настройки сохранены:\n"},
            {"Show additional title", "Показывать дополнительный заголовок"},
            {"Show technical info text", "Показывать техническую информацию"},
            {"Show timestamps", "Показывать время"},
            {"Similarity threshold:", "Порог сходства:"},
            {"Skip dark / blank frames", "Пропускать тёмные / пустые кадры"},
            {"Skip dark:", "Пропускать тёмные:"},
            {"Skip similar frames", "Пропускать похожие кадры"},
            {"Skip similar:", "Пропускать похожие:"},
            {"Specific timestep:", "Фиксированный шаг времени:"},
            {"Start all", "Запустить всё"},
            {"Start margin:", "Отступ в начале:"},
            {"Start selected", "Запустить выбранные"},
            {"Style preview failed", "Предпросмотр стиля не удался"},
            {"Style preview ready", "Предпросмотр стиля готов"},
            {"Styles", "Стили"},
            {"The file could not be saved.", "Не удалось сохранить файл."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Цвет рамки миниатюры:"},
            {"Tile border:", "Рамка миниатюры:"},
            {"Timestamp background:", "Фон времени:"},
            {"Timestamp position:", "Позиция времени:"},
            {"Timestamp text color:", "Цвет времени:"},
            {"Timestamp:", "Время:"},
            {"Timestep:", "Шаг времени:"},
            {"Title text color:", "Цвет заголовка:"},
            {"Title text:", "Текст заголовка:"},
            {"Upper Left", "Сверху слева"},
            {"Upper Right", "Сверху справа"},
            {"Use specific timestep", "Использовать фиксированный шаг времени"},
            {"View", "Вид"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Ширина:"},
            {"English", "Английский"},
            {"French", "Французский"},
            {"German", "Немецкий"},
            {"Spanish", "Испанский"},
            {"Dutch", "Нидерландский"},
            {"Czech", "Чешский"},
            {"Italian", "Итальянский"},
            {"Hungarian", "Венгерский"},
            {"Croatian", "Хорватский"},
            {"Portuguese", "Португальский"},
            {"Chinese", "Китайский"},
            {"Russian", "Русский"}
        };

        static const QHash<QString, QString> bg = {
            {"%1 rows × %2 columns", "%1 реда × %2 колони"},
            {"About", "За програмата"},
            {"About Thumbnail me 4b2", "За Thumbnail me 4b2"},
            {"Accurate (slowest)", "Точен (най-бавен)"},
            {"Add and select a video first.", "Първо добавете и изберете видео."},
            {"Add video files", "Добавяне на видео файлове"},
            {"Add video files first.", "Първо добавете видео файлове."},
            {"Added %1 video file(s)", "Добавени са %1 видео файл(а)"},
            {"Additional title:", "Допълнително заглавие:"},
            {"Background color:", "Цвят на фона:"},
            {"Balanced (safer for short videos)", "Балансиран (по-безопасен за кратки видеа)"},
            {"Batch cancellation requested...", "Заявенo е отменяне на партидата..."},
            {"Browse...", "Преглед..."},
            {"Cancel batch", "Отмени пакетната обработка"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Заявенo е отменяне. Изчаква се текущият процес да спре..."},
            {"Choose count text color", "Избор на цвят за текста на брояча"},
            {"Choose font...", "Избор на шрифт..."},
            {"Choose info text color", "Избор на цвят за информационния текст"},
            {"Choose tile border color", "Избор на цвят на рамката на плочката"},
            {"Choose timestamp background color", "Избор на цвят на фона на времевия маркер"},
            {"Choose timestamp text color", "Избор на цвят на текста на времевия маркер"},
            {"Choose title text color", "Избор на цвят на текста на заглавието"},
            {"Choose...", "Избери..."},
            {"Clear preview", "Изчистване на прегледа"},
            {"Columns:", "Колони:"},
            {"Config 1", "Конфиг. 1"},
            {"Config 2", "Конфиг. 2"},
            {"Config 3", "Конфиг. 3"},
            {"Contact sheet preview", "Преглед на контактния лист"},
            {"Count text color:", "Цвят на текста на брояча:"},
            {"Delete Selection", "Изтрий избраното"},
            {"Draw tile border", "Рисуване на рамка на плочката"},
            {"Edition", "Редактиране"},
            {"End margin:", "Краен отстъп:"},
            {"Exit", "Изход"},
            {"Fast (Thumbnail me 3.0 style)", "Бърз (стил Thumbnail me 3.0)"},
            {"File", "Файл"},
            {"Font:", "Шрифт:"},
            {"Gap:", "Разстояние:"},
            {"Generate a contact sheet first.", "Първо генерирайте контактен лист."},
            {"Info position:", "Позиция на информацията:"},
            {"Info text color:", "Цвят на информационния текст:"},
            {"Info text:", "Информационен текст:"},
            {"Input / Output", "Вход / изход"},
            {"Input list cleared", "Списъкът с входни файлове е изчистен"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Езикът е променен. Запазете настройките и рестартирайте приложението, за да се приложи напълно."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Изборът на език се записва веднага в settings.ini. Рестартирайте приложението, за да се приложат напълно статичните етикети."},
            {"Language:", "Език:"},
            {"Load", "Зареди"},
            {"Load video files...", "Зареди видео файлове..."},
            {"Load videos and click Start selected or Start all.", "Заредете видеата и щракнете Стартирай избраните или Стартирай всички."},
            {"Lower Left", "Долу вляво"},
            {"Lower Right", "Долу вдясно"},
            {"No contact sheet to save", "Няма контактен лист за запис"},
            {"Open Recent Configuration", "Отваряне на скорошна конфигурация"},
            {"Open configuration...", "Отваряне на конфигурация..."},
            {"Output", "Изход"},
            {"Output Format:", "Формат на изхода:"},
            {"Output filename suffix:", "Суфикс на името на изходния файл:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Път на изхода:"},
            {"Predefined Configuration", "Предварително зададена конфигурация"},
            {"Preset applied: %1 rows × %2 columns", "Приложена е настройка: %1 реда × %2 колони"},
            {"Preview area", "Област за преглед"},
            {"Preview style", "Преглед на стила"},
            {"Quality:", "Качество:"},
            {"Randomize amount:", "Степен на случайност:"},
            {"Randomize positions", "Случайно разместване на позициите"},
            {"Randomize:", "Случайно:"},
            {"Reload settings.ini", "Презареждане на settings.ini"},
            {"Reset", "Нулиране"},
            {"Rows:", "Редове:"},
            {"Same folder as source video", "Същата папка като изходното видео"},
            {"Same source folder:", "Същата изходна папка:"},
            {"Save configuration", "Запазване на конфигурацията"},
            {"Save configuration as...", "Запазване на конфигурацията като..."},
            {"Save contact sheet", "Запазване на контактния лист"},
            {"Save failed", "Запазването е неуспешно"},
            {"Save manually", "Запази ръчно"},
            {"Save selected contact sheet...", "Запазване на избрания контактен лист..."},
            {"Save settings.ini", "Запазване на settings.ini"},
            {"Saved: ", "Запазено: "},
            {"Seek mode:", "Режим на търсене:"},
            {"Select output folder", "Избор на изходна папка"},
            {"Selected video file(s) removed", "Избраните видео файлове са премахнати"},
            {"Settings saved:\n", "Настройките са запазени:\n"},
            {"Show additional title", "Показване на допълнително заглавие"},
            {"Show technical info text", "Показване на технически информационен текст"},
            {"Show timestamps", "Показвай времеви маркери"},
            {"Similarity threshold:", "Праг на сходство:"},
            {"Skip dark / blank frames", "Пропускане на тъмни / празни кадри"},
            {"Skip dark:", "Пропускане на тъмни:"},
            {"Skip similar frames", "Пропускане на подобни кадри"},
            {"Skip similar:", "Пропускане на подобни:"},
            {"Specific timestep:", "Конкретна времева стъпка:"},
            {"Start all", "Стартирай всички"},
            {"Start margin:", "Начален отстъп:"},
            {"Start selected", "Стартирай избраните"},
            {"Style preview failed", "Прегледът на стила е неуспешен"},
            {"Style preview ready", "Прегледът на стила е готов"},
            {"Styles", "Стилове"},
            {"The file could not be saved.", "Файлът не можа да бъде записан."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Цвят на рамката на плочката:"},
            {"Tile border:", "Рамка на плочката:"},
            {"Timestamp background:", "Фон на времевия маркер:"},
            {"Timestamp position:", "Позиция на времевия маркер:"},
            {"Timestamp text color:", "Цвят на текста на времевия маркер:"},
            {"Timestamp:", "Времеви маркер:"},
            {"Timestep:", "Времева стъпка:"},
            {"Title text color:", "Цвят на текста на заглавието:"},
            {"Title text:", "Текст на заглавието:"},
            {"Upper Left", "Горе вляво"},
            {"Upper Right", "Горе вдясно"},
            {"Use specific timestep", "Използване на конкретна времева стъпка"},
            {"View", "Изглед"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Ширина:"},
            {"English", "Английски"},
            {"French", "Френски"},
            {"German", "Немски"},
            {"Spanish", "Испански"},
            {"Dutch", "Нидерландски"},
            {"Czech", "Чешки"},
            {"Italian", "Италиански"},
            {"Hungarian", "Унгарски"},
            {"Croatian", "Хърватски"},
            {"Portuguese", "Португалски"},
            {"Chinese", "Китайски"},
            {"Russian", "Руски"}
        };

        static const QHash<QString, QString> da = {
            {"%1 rows × %2 columns", "%1 rækker × %2 kolonner"},
            {"About", "Om"},
            {"About Thumbnail me 4b2", "Om Thumbnail me 4b2"},
            {"Accurate (slowest)", "Præcis (langsomst)"},
            {"Add and select a video first.", "Tilføj og vælg først en video."},
            {"Add video files", "Tilføj videofiler"},
            {"Add video files first.", "Tilføj videofiler først."},
            {"Added %1 video file(s)", "Tilføjede %1 videofil(er)"},
            {"Additional title:", "Ekstra titel:"},
            {"Background color:", "Baggrundsfarve:"},
            {"Balanced (safer for short videos)", "Balanceret (sikrere til korte videoer)"},
            {"Batch cancellation requested...", "Annullering af batch er anmodet..."},
            {"Browse...", "Gennemse..."},
            {"Cancel batch", "Annuller batch"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Annullering anmodet. Venter på at den aktuelle worker stopper..."},
            {"Choose count text color", "Vælg farve til tællertekst"},
            {"Choose font...", "Vælg skrifttype..."},
            {"Choose info text color", "Vælg farve til infotekst"},
            {"Choose tile border color", "Vælg farve til miniaturekant"},
            {"Choose timestamp background color", "Vælg baggrundsfarve til tidsstempel"},
            {"Choose timestamp text color", "Vælg tekstfarve til tidsstempel"},
            {"Choose title text color", "Vælg tekstfarve til titel"},
            {"Choose...", "Vælg..."},
            {"Clear preview", "Ryd forhåndsvisning"},
            {"Columns:", "Kolonner:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Forhåndsvisning af kontaktark"},
            {"Count text color:", "Farve på tællertekst:"},
            {"Delete Selection", "Slet markering"},
            {"Draw tile border", "Tegn miniaturekant"},
            {"Edition", "Redigering"},
            {"End margin:", "Slutmargen:"},
            {"Exit", "Afslut"},
            {"Fast (Thumbnail me 3.0 style)", "Hurtig (Thumbnail me 3.0-stil)"},
            {"File", "Filer"},
            {"Font:", "Skrifttype:"},
            {"Gap:", "Mellemrum:"},
            {"Generate a contact sheet first.", "Generér først et kontaktark."},
            {"Info position:", "Infoposition:"},
            {"Info text color:", "Farve på infotekst:"},
            {"Info text:", "Infotekst:"},
            {"Input / Output", "Input / output"},
            {"Input list cleared", "Inputlisten er ryddet"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Sproget er ændret. Gem indstillingerne og genstart programmet for at anvende det fuldt ud."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Sprogvalget gemmes straks i settings.ini. Genstart programmet for at anvende statiske etiketter fuldt ud."},
            {"Language:", "Sprog:"},
            {"Load", "Indlæs"},
            {"Load video files...", "Indlæs videofiler..."},
            {"Load videos and click Start selected or Start all.", "Indlæs videoer og klik Start valgte eller Start alle."},
            {"Lower Left", "Nederst til venstre"},
            {"Lower Right", "Nederst til højre"},
            {"No contact sheet to save", "Intet kontaktark at gemme"},
            {"Open Recent Configuration", "Åbn seneste konfiguration"},
            {"Open configuration...", "Åbn konfiguration..."},
            {"Output", "Output"},
            {"Output Format:", "Outputformat:"},
            {"Output filename suffix:", "Suffiks til outputfilnavn:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Outputsti:"},
            {"Predefined Configuration", "Foruddefineret konfiguration"},
            {"Preset applied: %1 rows × %2 columns", "Forvalg anvendt: %1 rækker × %2 kolonner"},
            {"Preview area", "Forhåndsvisningsområde"},
            {"Preview style", "Forhåndsvis stil"},
            {"Quality:", "Kvalitet:"},
            {"Randomize amount:", "Mængde af tilfældighed:"},
            {"Randomize positions", "Tilfældiggør positioner"},
            {"Randomize:", "Tilfældiggør:"},
            {"Reload settings.ini", "Genindlæs settings.ini"},
            {"Reset", "Nulstil"},
            {"Rows:", "Rækker:"},
            {"Same folder as source video", "Samme mappe som kildevideo"},
            {"Same source folder:", "Samme kildemappe:"},
            {"Save configuration", "Gem konfiguration"},
            {"Save configuration as...", "Gem konfiguration som..."},
            {"Save contact sheet", "Gem kontaktark"},
            {"Save failed", "Gemning mislykkedes"},
            {"Save manually", "Gem manuelt"},
            {"Save selected contact sheet...", "Gem valgt kontaktark..."},
            {"Save settings.ini", "Gem settings.ini"},
            {"Saved: ", "Gemt: "},
            {"Seek mode:", "Søgetilstand:"},
            {"Select output folder", "Vælg outputmappe"},
            {"Selected video file(s) removed", "Valgte videofil(er) fjernet"},
            {"Settings saved:\n", "Indstillinger gemt:\n"},
            {"Show additional title", "Vis ekstra titel"},
            {"Show technical info text", "Vis teknisk infotekst"},
            {"Show timestamps", "Vis tidsstempler"},
            {"Similarity threshold:", "Lighedstærskel:"},
            {"Skip dark / blank frames", "Spring mørke / tomme billeder over"},
            {"Skip dark:", "Spring mørke over:"},
            {"Skip similar frames", "Spring lignende billeder over"},
            {"Skip similar:", "Spring lignende over:"},
            {"Specific timestep:", "Specifikt tidsinterval:"},
            {"Start all", "Start alle"},
            {"Start margin:", "Startmargen:"},
            {"Start selected", "Start valgte"},
            {"Style preview failed", "Stilforhåndsvisning mislykkedes"},
            {"Style preview ready", "Stilforhåndsvisning klar"},
            {"Styles", "Stilarter"},
            {"The file could not be saved.", "Filen kunne ikke gemmes."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Farve på miniaturekant:"},
            {"Tile border:", "Miniaturekant:"},
            {"Timestamp background:", "Tidsstempelbaggrund:"},
            {"Timestamp position:", "Tidsstempelposition:"},
            {"Timestamp text color:", "Tekstfarve til tidsstempel:"},
            {"Timestamp:", "Tidsstempel:"},
            {"Timestep:", "Tidsinterval:"},
            {"Title text color:", "Tekstfarve til titel:"},
            {"Title text:", "Titeltekst:"},
            {"Upper Left", "Øverst til venstre"},
            {"Upper Right", "Øverst til højre"},
            {"Use specific timestep", "Brug specifikt tidsinterval"},
            {"View", "Vis"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Bredde:"},
            {"English", "Engelsk"},
            {"French", "Fransk"},
            {"German", "Tysk"},
            {"Spanish", "Spansk"},
            {"Dutch", "Nederlandsk"},
            {"Czech", "Tjekkisk"},
            {"Italian", "Italiensk"},
            {"Hungarian", "Ungarsk"},
            {"Croatian", "Kroatisk"},
            {"Portuguese", "Portugisisk"},
            {"Chinese", "Kinesisk"},
            {"Russian", "Russisk"}
        };

        static const QHash<QString, QString> et = {
            {"%1 rows × %2 columns", "%1 rida × %2 veergu"},
            {"About", "Teave"},
            {"About Thumbnail me 4b2", "Teave Thumbnail me 4b2 kohta"},
            {"Accurate (slowest)", "Täpne (aeglaseim)"},
            {"Add and select a video first.", "Lisa ja vali esmalt video."},
            {"Add video files", "Lisa videofaile"},
            {"Add video files first.", "Lisa esmalt videofailid."},
            {"Added %1 video file(s)", "Lisati %1 videofaili"},
            {"Additional title:", "Lisapealkiri:"},
            {"Background color:", "Taustavärv:"},
            {"Balanced (safer for short videos)", "Tasakaalustatud (lühikeste videote jaoks turvalisem)"},
            {"Batch cancellation requested...", "Pakettülesande tühistamist taotleti..."},
            {"Browse...", "Sirvi..."},
            {"Cancel batch", "Tühista pakktöö"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Tühistamist taotleti. Oodatakse praeguse tööprotsessi peatumist..."},
            {"Choose count text color", "Vali loenduriteksti värv"},
            {"Choose font...", "Vali font..."},
            {"Choose info text color", "Vali infoteksti värv"},
            {"Choose tile border color", "Vali pisipildi äärise värv"},
            {"Choose timestamp background color", "Vali ajatemplite taustavärv"},
            {"Choose timestamp text color", "Vali ajatemplite teksti värv"},
            {"Choose title text color", "Vali pealkirja teksti värv"},
            {"Choose...", "Vali..."},
            {"Clear preview", "Tühjenda eelvaade"},
            {"Columns:", "Veerud:"},
            {"Config 1", "Seadistus 1"},
            {"Config 2", "Seadistus 2"},
            {"Config 3", "Seadistus 3"},
            {"Contact sheet preview", "Kontaktlehe eelvaade"},
            {"Count text color:", "Loenduriteksti värv:"},
            {"Delete Selection", "Kustuta valik"},
            {"Draw tile border", "Joonista pisipildi ääris"},
            {"Edition", "Redigeerimine"},
            {"End margin:", "Lõpumarginaal:"},
            {"Exit", "Välju"},
            {"Fast (Thumbnail me 3.0 style)", "Kiire (Thumbnail me 3.0 stiil)"},
            {"File", "Fail"},
            {"Font:", "Font:"},
            {"Gap:", "Vahe:"},
            {"Generate a contact sheet first.", "Genereeri esmalt kontaktleht."},
            {"Info position:", "Info asukoht:"},
            {"Info text color:", "Infoteksti värv:"},
            {"Info text:", "Infotekst:"},
            {"Input / Output", "Sisend / väljund"},
            {"Input list cleared", "Sisendloend tühjendati"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Keel muudeti. Salvesta sätted ja taaskäivita rakendus, et muudatus täielikult rakenduks."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Keelevalik salvestatakse kohe faili settings.ini. Staatiliste siltide täielikuks rakendamiseks taaskäivita rakendus."},
            {"Language:", "Keel:"},
            {"Load", "Laadi"},
            {"Load video files...", "Laadi videofaile..."},
            {"Load videos and click Start selected or Start all.", "Laadi videod ja klõpsa Käivita valitud või Käivita kõik."},
            {"Lower Left", "All vasakul"},
            {"Lower Right", "All paremal"},
            {"No contact sheet to save", "Salvestatavat kontaktlehte pole"},
            {"Open Recent Configuration", "Ava hiljutine konfiguratsioon"},
            {"Open configuration...", "Ava konfiguratsioon..."},
            {"Output", "Väljund"},
            {"Output Format:", "Väljundvorming:"},
            {"Output filename suffix:", "Väljundfaili nime järelliide:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Väljundi asukoht:"},
            {"Predefined Configuration", "Eelmääratud konfiguratsioon"},
            {"Preset applied: %1 rows × %2 columns", "Eelseade rakendatud: %1 rida × %2 veergu"},
            {"Preview area", "Eelvaate ala"},
            {"Preview style", "Stiili eelvaade"},
            {"Quality:", "Kvaliteet:"},
            {"Randomize amount:", "Juhuslikustamise määr:"},
            {"Randomize positions", "Juhuslikusta asukohad"},
            {"Randomize:", "Juhuslikustamine:"},
            {"Reload settings.ini", "Laadi settings.ini uuesti"},
            {"Reset", "Lähtesta"},
            {"Rows:", "Read:"},
            {"Same folder as source video", "Sama kaust kui lähtevideol"},
            {"Same source folder:", "Sama lähtekaust:"},
            {"Save configuration", "Salvesta konfiguratsioon"},
            {"Save configuration as...", "Salvesta konfiguratsioon kui..."},
            {"Save contact sheet", "Salvesta kontaktleht"},
            {"Save failed", "Salvestamine nurjus"},
            {"Save manually", "Salvesta käsitsi"},
            {"Save selected contact sheet...", "Salvesta valitud kontaktleht..."},
            {"Save settings.ini", "Salvesta settings.ini"},
            {"Saved: ", "Salvestatud: "},
            {"Seek mode:", "Otsimisrežiim:"},
            {"Select output folder", "Vali väljundkaust"},
            {"Selected video file(s) removed", "Valitud videofailid eemaldati"},
            {"Settings saved:\n", "Seaded salvestatud:\n"},
            {"Show additional title", "Näita lisapealkirja"},
            {"Show technical info text", "Näita tehnilist infoteksti"},
            {"Show timestamps", "Kuva ajatemplid"},
            {"Similarity threshold:", "Sarnasuse lävi:"},
            {"Skip dark / blank frames", "Jäta tumedad / tühjad kaadrid vahele"},
            {"Skip dark:", "Jäta tumedad vahele:"},
            {"Skip similar frames", "Jäta sarnased kaadrid vahele"},
            {"Skip similar:", "Jäta sarnased vahele:"},
            {"Specific timestep:", "Konkreetne ajasamm:"},
            {"Start all", "Käivita kõik"},
            {"Start margin:", "Algusmarginaal:"},
            {"Start selected", "Käivita valitud"},
            {"Style preview failed", "Stiili eelvaade ebaõnnestus"},
            {"Style preview ready", "Stiili eelvaade valmis"},
            {"Styles", "Stiilid"},
            {"The file could not be saved.", "Faili ei saanud salvestada."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Pisipildi äärise värv:"},
            {"Tile border:", "Pisipildi ääris:"},
            {"Timestamp background:", "Ajatemplite taust:"},
            {"Timestamp position:", "Ajatemplite asukoht:"},
            {"Timestamp text color:", "Ajatemplite teksti värv:"},
            {"Timestamp:", "Ajatempel:"},
            {"Timestep:", "Ajasamm:"},
            {"Title text color:", "Pealkirja teksti värv:"},
            {"Title text:", "Pealkirja tekst:"},
            {"Upper Left", "Ülal vasakul"},
            {"Upper Right", "Ülal paremal"},
            {"Use specific timestep", "Kasuta konkreetset ajasammu"},
            {"View", "Vaade"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Laius:"},
            {"English", "Inglise"},
            {"French", "Prantsuse"},
            {"German", "Saksa"},
            {"Spanish", "Hispaania"},
            {"Dutch", "Hollandi"},
            {"Czech", "Tšehhi"},
            {"Italian", "Itaalia"},
            {"Hungarian", "Ungari"},
            {"Croatian", "Horvaadi"},
            {"Portuguese", "Portugali"},
            {"Chinese", "Hiina"},
            {"Russian", "Vene"}
        };

        static const QHash<QString, QString> el = {
            {"%1 rows × %2 columns", "%1 γραμμές × %2 στήλες"},
            {"About", "Σχετικά"},
            {"About Thumbnail me 4b2", "Σχετικά με το Thumbnail me 4b2"},
            {"Accurate (slowest)", "Ακριβής (πιο αργή)"},
            {"Add and select a video first.", "Προσθέστε και επιλέξτε πρώτα ένα βίντεο."},
            {"Add video files", "Προσθήκη αρχείων βίντεο"},
            {"Add video files first.", "Προσθέστε πρώτα αρχεία βίντεο."},
            {"Added %1 video file(s)", "Προστέθηκαν %1 αρχεία βίντεο"},
            {"Additional title:", "Πρόσθετος τίτλος:"},
            {"Background color:", "Χρώμα φόντου:"},
            {"Balanced (safer for short videos)", "Ισορροπημένη (ασφαλέστερη για σύντομα βίντεο)"},
            {"Batch cancellation requested...", "Ζητήθηκε ακύρωση παρτίδας..."},
            {"Browse...", "Περιήγηση..."},
            {"Cancel batch", "Ακύρωση παρτίδας"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Ζητήθηκε ακύρωση. Αναμονή να σταματήσει η τρέχουσα εργασία..."},
            {"Choose count text color", "Επιλογή χρώματος κειμένου μετρητή"},
            {"Choose font...", "Επιλογή γραμματοσειράς..."},
            {"Choose info text color", "Επιλογή χρώματος κειμένου πληροφοριών"},
            {"Choose tile border color", "Επιλογή χρώματος περιγράμματος πλακιδίου"},
            {"Choose timestamp background color", "Επιλογή χρώματος φόντου χρονικής σήμανσης"},
            {"Choose timestamp text color", "Επιλογή χρώματος κειμένου χρονικής σήμανσης"},
            {"Choose title text color", "Επιλογή χρώματος κειμένου τίτλου"},
            {"Choose...", "Επιλογή..."},
            {"Clear preview", "Καθαρισμός προεπισκόπησης"},
            {"Columns:", "Στήλες:"},
            {"Config 1", "Ρύθμιση 1"},
            {"Config 2", "Ρύθμιση 2"},
            {"Config 3", "Ρύθμιση 3"},
            {"Contact sheet preview", "Προεπισκόπηση φύλλου επαφής"},
            {"Count text color:", "Χρώμα κειμένου μετρητή:"},
            {"Delete Selection", "Διαγραφή επιλογής"},
            {"Draw tile border", "Σχεδίαση περιγράμματος πλακιδίου"},
            {"Edition", "Επεξεργασία"},
            {"End margin:", "Περιθώριο τέλους:"},
            {"Exit", "Έξοδος"},
            {"Fast (Thumbnail me 3.0 style)", "Γρήγορη (στυλ Thumbnail me 3.0)"},
            {"File", "Αρχείο"},
            {"Font:", "Γραμματοσειρά:"},
            {"Gap:", "Κενό:"},
            {"Generate a contact sheet first.", "Δημιουργήστε πρώτα ένα φύλλο επαφής."},
            {"Info position:", "Θέση πληροφοριών:"},
            {"Info text color:", "Χρώμα κειμένου πληροφοριών:"},
            {"Info text:", "Κείμενο πληροφοριών:"},
            {"Input / Output", "Είσοδος / έξοδος"},
            {"Input list cleared", "Η λίστα εισόδου καθαρίστηκε"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Η γλώσσα άλλαξε. Αποθηκεύστε τις ρυθμίσεις και επανεκκινήστε την εφαρμογή για πλήρη εφαρμογή."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Η επιλογή γλώσσας αποθηκεύεται αμέσως στο settings.ini. Επανεκκινήστε την εφαρμογή για πλήρη εφαρμογή των στατικών ετικετών."},
            {"Language:", "Γλώσσα:"},
            {"Load", "Φόρτωση"},
            {"Load video files...", "Φόρτωση αρχείων βίντεο..."},
            {"Load videos and click Start selected or Start all.", "Φορτώστε βίντεο και πατήστε Έναρξη επιλεγμένων ή Έναρξη όλων."},
            {"Lower Left", "Κάτω αριστερά"},
            {"Lower Right", "Κάτω δεξιά"},
            {"No contact sheet to save", "Δεν υπάρχει φύλλο επαφής για αποθήκευση"},
            {"Open Recent Configuration", "Άνοιγμα πρόσφατης διαμόρφωσης"},
            {"Open configuration...", "Άνοιγμα διαμόρφωσης..."},
            {"Output", "Έξοδος"},
            {"Output Format:", "Μορφή εξόδου:"},
            {"Output filename suffix:", "Κατάληξη ονόματος αρχείου εξόδου:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Διαδρομή εξόδου:"},
            {"Predefined Configuration", "Προκαθορισμένη διαμόρφωση"},
            {"Preset applied: %1 rows × %2 columns", "Εφαρμόστηκε προρύθμιση: %1 γραμμές × %2 στήλες"},
            {"Preview area", "Περιοχή προεπισκόπησης"},
            {"Preview style", "Προεπισκόπηση στυλ"},
            {"Quality:", "Ποιότητα:"},
            {"Randomize amount:", "Ποσότητα τυχαιότητας:"},
            {"Randomize positions", "Τυχαίες θέσεις"},
            {"Randomize:", "Τυχαιότητα:"},
            {"Reload settings.ini", "Επαναφόρτωση settings.ini"},
            {"Reset", "Επαναφορά"},
            {"Rows:", "Γραμμές:"},
            {"Same folder as source video", "Ίδιος φάκελος με το αρχικό βίντεο"},
            {"Same source folder:", "Ίδιος φάκελος πηγής:"},
            {"Save configuration", "Αποθήκευση διαμόρφωσης"},
            {"Save configuration as...", "Αποθήκευση διαμόρφωσης ως..."},
            {"Save contact sheet", "Αποθήκευση φύλλου επαφής"},
            {"Save failed", "Η αποθήκευση απέτυχε"},
            {"Save manually", "Χειροκίνητη αποθήκευση"},
            {"Save selected contact sheet...", "Αποθήκευση επιλεγμένου φύλλου επαφής..."},
            {"Save settings.ini", "Αποθήκευση settings.ini"},
            {"Saved: ", "Αποθηκεύτηκε: "},
            {"Seek mode:", "Λειτουργία αναζήτησης:"},
            {"Select output folder", "Επιλογή φακέλου εξόδου"},
            {"Selected video file(s) removed", "Τα επιλεγμένα αρχεία βίντεο αφαιρέθηκαν"},
            {"Settings saved:\n", "Οι ρυθμίσεις αποθηκεύτηκαν:\n"},
            {"Show additional title", "Εμφάνιση πρόσθετου τίτλου"},
            {"Show technical info text", "Εμφάνιση τεχνικού κειμένου πληροφοριών"},
            {"Show timestamps", "Εμφάνιση χρονικών σημάνσεων"},
            {"Similarity threshold:", "Κατώφλι ομοιότητας:"},
            {"Skip dark / blank frames", "Παράλειψη σκοτεινών / κενών καρέ"},
            {"Skip dark:", "Παράλειψη σκοτεινών:"},
            {"Skip similar frames", "Παράλειψη παρόμοιων καρέ"},
            {"Skip similar:", "Παράλειψη παρόμοιων:"},
            {"Specific timestep:", "Συγκεκριμένο χρονικό βήμα:"},
            {"Start all", "Έναρξη όλων"},
            {"Start margin:", "Περιθώριο αρχής:"},
            {"Start selected", "Έναρξη επιλεγμένων"},
            {"Style preview failed", "Η προεπισκόπηση στυλ απέτυχε"},
            {"Style preview ready", "Η προεπισκόπηση στυλ είναι έτοιμη"},
            {"Styles", "Στυλ"},
            {"The file could not be saved.", "Το αρχείο δεν μπόρεσε να αποθηκευτεί."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Χρώμα περιγράμματος πλακιδίου:"},
            {"Tile border:", "Περίγραμμα πλακιδίου:"},
            {"Timestamp background:", "Φόντο χρονικής σήμανσης:"},
            {"Timestamp position:", "Θέση χρονικής σήμανσης:"},
            {"Timestamp text color:", "Χρώμα κειμένου χρονικής σήμανσης:"},
            {"Timestamp:", "Χρονική σήμανση:"},
            {"Timestep:", "Χρονικό βήμα:"},
            {"Title text color:", "Χρώμα κειμένου τίτλου:"},
            {"Title text:", "Κείμενο τίτλου:"},
            {"Upper Left", "Πάνω αριστερά"},
            {"Upper Right", "Πάνω δεξιά"},
            {"Use specific timestep", "Χρήση συγκεκριμένου χρονικού βήματος"},
            {"View", "Προβολή"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Πλάτος:"},
            {"English", "Αγγλικά"},
            {"French", "Γαλλικά"},
            {"German", "Γερμανικά"},
            {"Spanish", "Ισπανικά"},
            {"Dutch", "Ολλανδικά"},
            {"Czech", "Τσεχικά"},
            {"Italian", "Ιταλικά"},
            {"Hungarian", "Ουγγρικά"},
            {"Croatian", "Κροατικά"},
            {"Portuguese", "Πορτογαλικά"},
            {"Chinese", "Κινεζικά"},
            {"Russian", "Ρωσικά"}
        };

        static const QHash<QString, QString> ga = {
            {"%1 rows × %2 columns", "%1 ró × %2 colún"},
            {"About", "Maidir leis"},
            {"About Thumbnail me 4b2", "Maidir le Thumbnail me 4b2"},
            {"Accurate (slowest)", "Cruinn (is moille)"},
            {"Add and select a video first.", "Cuir físeán leis agus roghnaigh é ar dtús."},
            {"Add video files", "Cuir comhaid físe leis"},
            {"Add video files first.", "Cuir comhaid físe leis ar dtús."},
            {"Added %1 video file(s)", "Cuireadh %1 comhad físe leis"},
            {"Additional title:", "Teideal breise:"},
            {"Background color:", "Dath cúlra:"},
            {"Balanced (safer for short videos)", "Cothromaithe (níos sábháilte do fhíseáin ghearra)"},
            {"Batch cancellation requested...", "Iarradh cealú baisce..."},
            {"Browse...", "Brabhsáil..."},
            {"Cancel batch", "Cealaigh baisc"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Iarradh cealú. Ag fanacht leis an oibrí reatha stopadh..."},
            {"Choose count text color", "Roghnaigh dath théacs an chomhairimh"},
            {"Choose font...", "Roghnaigh cló..."},
            {"Choose info text color", "Roghnaigh dath an téacs eolais"},
            {"Choose tile border color", "Roghnaigh dath imeall na tíle"},
            {"Choose timestamp background color", "Roghnaigh dath cúlra an stampa ama"},
            {"Choose timestamp text color", "Roghnaigh dath théacs an stampa ama"},
            {"Choose title text color", "Roghnaigh dath théacs an teidil"},
            {"Choose...", "Roghnaigh..."},
            {"Clear preview", "Glan an réamhamharc"},
            {"Columns:", "Colúin:"},
            {"Config 1", "Cumraíocht 1"},
            {"Config 2", "Cumraíocht 2"},
            {"Config 3", "Cumraíocht 3"},
            {"Contact sheet preview", "Réamhamharc ar bhileog theagmhála"},
            {"Count text color:", "Dath théacs an chomhairimh:"},
            {"Delete Selection", "Scrios an roghnú"},
            {"Draw tile border", "Tarraing imeall tíle"},
            {"Edition", "Eagrú"},
            {"End margin:", "Imeall deiridh:"},
            {"Exit", "Scoir"},
            {"Fast (Thumbnail me 3.0 style)", "Tapa (stíl Thumbnail me 3.0)"},
            {"File", "Comhad"},
            {"Font:", "Cló:"},
            {"Gap:", "Bearna:"},
            {"Generate a contact sheet first.", "Cruthaigh bileog theagmhála ar dtús."},
            {"Info position:", "Suíomh eolais:"},
            {"Info text color:", "Dath an téacs eolais:"},
            {"Info text:", "Téacs eolais:"},
            {"Input / Output", "Ionchur / aschur"},
            {"Input list cleared", "Glanadh an liosta ionchuir"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Athraíodh an teanga. Sábháil na socruithe agus atosaigh an feidhmchlár chun í a chur i bhfeidhm go hiomlán."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Sábháiltear an rogha teanga láithreach i settings.ini. Atosaigh an feidhmchlár chun na lipéid statacha a chur i bhfeidhm go hiomlán."},
            {"Language:", "Teanga:"},
            {"Load", "Luchtaigh"},
            {"Load video files...", "Luchtaigh comhaid físe..."},
            {"Load videos and click Start selected or Start all.", "Luchtaigh físeáin agus cliceáil Tosaigh roghnaithe nó Tosaigh uile."},
            {"Lower Left", "Íochtar ar chlé"},
            {"Lower Right", "Íochtar ar dheis"},
            {"No contact sheet to save", "Níl bileog theagmhála le sábháil"},
            {"Open Recent Configuration", "Oscail cumraíocht le déanaí"},
            {"Open configuration...", "Oscail cumraíocht..."},
            {"Output", "Aschur"},
            {"Output Format:", "Formáid aschuir:"},
            {"Output filename suffix:", "Iarmhír ainm comhaid aschuir:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Conair aschuir:"},
            {"Predefined Configuration", "Cumraíocht réamhshocraithe"},
            {"Preset applied: %1 rows × %2 columns", "Cuireadh réamhshocrú i bhfeidhm: %1 ró × %2 colún"},
            {"Preview area", "Limistéar réamhamhairc"},
            {"Preview style", "Réamhamharc stíle"},
            {"Quality:", "Cáilíocht:"},
            {"Randomize amount:", "Méid randamaithe:"},
            {"Randomize positions", "Randamaigh suíomhanna"},
            {"Randomize:", "Randamú:"},
            {"Reload settings.ini", "Athluchtaigh settings.ini"},
            {"Reset", "Athshocraigh"},
            {"Rows:", "Rónna:"},
            {"Same folder as source video", "An fillteán céanna leis an bhfíseán foinse"},
            {"Same source folder:", "Fillteán foinse céanna:"},
            {"Save configuration", "Sábháil cumraíocht"},
            {"Save configuration as...", "Sábháil cumraíocht mar..."},
            {"Save contact sheet", "Sábháil bileog theagmhála"},
            {"Save failed", "Theip ar shábháil"},
            {"Save manually", "Sábháil de láimh"},
            {"Save selected contact sheet...", "Sábháil an bhileog theagmhála roghnaithe..."},
            {"Save settings.ini", "Sábháil settings.ini"},
            {"Saved: ", "Sábháilte: "},
            {"Seek mode:", "Mód cuardaigh:"},
            {"Select output folder", "Roghnaigh fillteán aschuir"},
            {"Selected video file(s) removed", "Baineadh na comhaid físe roghnaithe"},
            {"Settings saved:\n", "Sábháladh na socruithe:\n"},
            {"Show additional title", "Taispeáin teideal breise"},
            {"Show technical info text", "Taispeáin téacs eolais theicniúil"},
            {"Show timestamps", "Taispeáin stampaí ama"},
            {"Similarity threshold:", "Tairseach cosúlachta:"},
            {"Skip dark / blank frames", "Scipeáil frámaí dorcha / bána"},
            {"Skip dark:", "Scipeáil dorcha:"},
            {"Skip similar frames", "Scipeáil frámaí cosúla"},
            {"Skip similar:", "Scipeáil cosúla:"},
            {"Specific timestep:", "Céim ama shonrach:"},
            {"Start all", "Tosaigh iad uile"},
            {"Start margin:", "Imeall tosaigh:"},
            {"Start selected", "Tosaigh na cinn roghnaithe"},
            {"Style preview failed", "Theip ar réamhamharc stíle"},
            {"Style preview ready", "Tá réamhamharc stíle réidh"},
            {"Styles", "Stíleanna"},
            {"The file could not be saved.", "Níorbh fhéidir an comhad a shábháil."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Dath imeall na tíle:"},
            {"Tile border:", "Imeall tíle:"},
            {"Timestamp background:", "Cúlra stampa ama:"},
            {"Timestamp position:", "Suíomh stampa ama:"},
            {"Timestamp text color:", "Dath théacs stampa ama:"},
            {"Timestamp:", "Stampa ama:"},
            {"Timestep:", "Céim ama:"},
            {"Title text color:", "Dath théacs an teidil:"},
            {"Title text:", "Téacs teidil:"},
            {"Upper Left", "Barr ar chlé"},
            {"Upper Right", "Barr ar dheis"},
            {"Use specific timestep", "Úsáid céim ama shonrach"},
            {"View", "Amharc"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Leithead:"},
            {"English", "Béarla"},
            {"French", "Fraincis"},
            {"German", "Gearmáinis"},
            {"Spanish", "Spáinnis"},
            {"Dutch", "Ollainnis"},
            {"Czech", "Seicis"},
            {"Italian", "Iodáilis"},
            {"Hungarian", "Ungáiris"},
            {"Croatian", "Cróitis"},
            {"Portuguese", "Portaingéilis"},
            {"Chinese", "Sínis"},
            {"Russian", "Rúisis"}
        };

        static const QHash<QString, QString> lv = {
            {"%1 rows × %2 columns", "%1 rindas × %2 kolonnas"},
            {"About", "Par programmu"},
            {"About Thumbnail me 4b2", "Par Thumbnail me 4b2"},
            {"Accurate (slowest)", "Precīzs (vislēnākais)"},
            {"Add and select a video first.", "Vispirms pievienojiet un izvēlieties video."},
            {"Add video files", "Pievienot video failus"},
            {"Add video files first.", "Vispirms pievienojiet video failus."},
            {"Added %1 video file(s)", "Pievienoti %1 video faili"},
            {"Additional title:", "Papildu virsraksts:"},
            {"Background color:", "Fona krāsa:"},
            {"Balanced (safer for short videos)", "Līdzsvarots (drošāks īsiem video)"},
            {"Batch cancellation requested...", "Pieprasīta paketes atcelšana..."},
            {"Browse...", "Pārlūkot..."},
            {"Cancel batch", "Atcelt paketi"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Pieprasīta atcelšana. Gaida, līdz pašreizējais process apstāsies..."},
            {"Choose count text color", "Izvēlieties skaita teksta krāsu"},
            {"Choose font...", "Izvēlieties fontu..."},
            {"Choose info text color", "Izvēlieties informācijas teksta krāsu"},
            {"Choose tile border color", "Izvēlieties sīktēla apmales krāsu"},
            {"Choose timestamp background color", "Izvēlieties laika zīmoga fona krāsu"},
            {"Choose timestamp text color", "Izvēlieties laika zīmoga teksta krāsu"},
            {"Choose title text color", "Izvēlieties virsraksta teksta krāsu"},
            {"Choose...", "Izvēlēties..."},
            {"Clear preview", "Notīrīt priekšskatījumu"},
            {"Columns:", "Kolonnas:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Kontaktlapas priekšskatījums"},
            {"Count text color:", "Skaita teksta krāsa:"},
            {"Delete Selection", "Dzēst atlasi"},
            {"Draw tile border", "Zīmēt sīktēla apmali"},
            {"Edition", "Rediģēšana"},
            {"End margin:", "Beigu mala:"},
            {"Exit", "Iziet"},
            {"Fast (Thumbnail me 3.0 style)", "Ātrs (Thumbnail me 3.0 stils)"},
            {"File", "Fails"},
            {"Font:", "Fonts:"},
            {"Gap:", "Atstarpe:"},
            {"Generate a contact sheet first.", "Vispirms izveidojiet kontaktlapu."},
            {"Info position:", "Informācijas pozīcija:"},
            {"Info text color:", "Informācijas teksta krāsa:"},
            {"Info text:", "Informācijas teksts:"},
            {"Input / Output", "Ievade / izvade"},
            {"Input list cleared", "Ievades saraksts notīrīts"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Valoda mainīta. Saglabājiet iestatījumus un restartējiet lietotni, lai to pilnībā piemērotu."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Valodas izvēle nekavējoties tiek saglabāta settings.ini. Restartējiet lietotni, lai pilnībā piemērotu statiskās etiķetes."},
            {"Language:", "Valoda:"},
            {"Load", "Ielādēt"},
            {"Load video files...", "Ielādēt video failus..."},
            {"Load videos and click Start selected or Start all.", "Ielādējiet video un noklikšķiniet Sākt izvēlētos vai Sākt visus."},
            {"Lower Left", "Apakšā pa kreisi"},
            {"Lower Right", "Apakšā pa labi"},
            {"No contact sheet to save", "Nav kontaktlapas, ko saglabāt"},
            {"Open Recent Configuration", "Atvērt nesenu konfigurāciju"},
            {"Open configuration...", "Atvērt konfigurāciju..."},
            {"Output", "Izvade"},
            {"Output Format:", "Izvades formāts:"},
            {"Output filename suffix:", "Izvades faila nosaukuma sufikss:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Izvades ceļš:"},
            {"Predefined Configuration", "Iepriekš definēta konfigurācija"},
            {"Preset applied: %1 rows × %2 columns", "Sagatave piemērota: %1 rindas × %2 kolonnas"},
            {"Preview area", "Priekšskatījuma zona"},
            {"Preview style", "Stila priekšskatījums"},
            {"Quality:", "Kvalitāte:"},
            {"Randomize amount:", "Nejaušības apjoms:"},
            {"Randomize positions", "Nejaušināt pozīcijas"},
            {"Randomize:", "Nejaušināšana:"},
            {"Reload settings.ini", "Pārlādēt settings.ini"},
            {"Reset", "Atiestatīt"},
            {"Rows:", "Rindas:"},
            {"Same folder as source video", "Tā pati mape kā avota video"},
            {"Same source folder:", "Tā pati avota mape:"},
            {"Save configuration", "Saglabāt konfigurāciju"},
            {"Save configuration as...", "Saglabāt konfigurāciju kā..."},
            {"Save contact sheet", "Saglabāt kontaktlapu"},
            {"Save failed", "Saglabāšana neizdevās"},
            {"Save manually", "Saglabāt manuāli"},
            {"Save selected contact sheet...", "Saglabāt izvēlēto kontaktlapu..."},
            {"Save settings.ini", "Saglabāt settings.ini"},
            {"Saved: ", "Saglabāts: "},
            {"Seek mode:", "Meklēšanas režīms:"},
            {"Select output folder", "Izvēlēties izvades mapi"},
            {"Selected video file(s) removed", "Izvēlētie video faili noņemti"},
            {"Settings saved:\n", "Iestatījumi saglabāti:\n"},
            {"Show additional title", "Rādīt papildu virsrakstu"},
            {"Show technical info text", "Rādīt tehnisko informācijas tekstu"},
            {"Show timestamps", "Rādīt laika zīmogus"},
            {"Similarity threshold:", "Līdzības slieksnis:"},
            {"Skip dark / blank frames", "Izlaist tumšus / tukšus kadrus"},
            {"Skip dark:", "Izlaist tumšos:"},
            {"Skip similar frames", "Izlaist līdzīgus kadrus"},
            {"Skip similar:", "Izlaist līdzīgos:"},
            {"Specific timestep:", "Konkrēts laika solis:"},
            {"Start all", "Sākt visu"},
            {"Start margin:", "Sākuma mala:"},
            {"Start selected", "Sākt atlasītos"},
            {"Style preview failed", "Stila priekšskatījums neizdevās"},
            {"Style preview ready", "Stila priekšskatījums gatavs"},
            {"Styles", "Stili"},
            {"The file could not be saved.", "Failu nevarēja saglabāt."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Sīktēla apmales krāsa:"},
            {"Tile border:", "Sīktēla apmale:"},
            {"Timestamp background:", "Laika zīmoga fons:"},
            {"Timestamp position:", "Laika zīmoga pozīcija:"},
            {"Timestamp text color:", "Laika zīmoga teksta krāsa:"},
            {"Timestamp:", "Laika zīmogs:"},
            {"Timestep:", "Laika solis:"},
            {"Title text color:", "Virsraksta teksta krāsa:"},
            {"Title text:", "Virsraksta teksts:"},
            {"Upper Left", "Augšā pa kreisi"},
            {"Upper Right", "Augšā pa labi"},
            {"Use specific timestep", "Izmantot konkrētu laika soli"},
            {"View", "Skats"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Platums:"},
            {"English", "Angļu"},
            {"French", "Franču"},
            {"German", "Vācu"},
            {"Spanish", "Spāņu"},
            {"Dutch", "Nīderlandiešu"},
            {"Czech", "Čehu"},
            {"Italian", "Itāļu"},
            {"Hungarian", "Ungāru"},
            {"Croatian", "Horvātu"},
            {"Portuguese", "Portugāļu"},
            {"Chinese", "Ķīniešu"},
            {"Russian", "Krievu"}
        };

        static const QHash<QString, QString> lt = {
            {"%1 rows × %2 columns", "%1 eilutės × %2 stulpeliai"},
            {"About", "Apie"},
            {"About Thumbnail me 4b2", "Apie Thumbnail me 4b2"},
            {"Accurate (slowest)", "Tikslus (lėčiausias)"},
            {"Add and select a video first.", "Pirmiausia pridėkite ir pasirinkite vaizdo įrašą."},
            {"Add video files", "Pridėti vaizdo failus"},
            {"Add video files first.", "Pirmiausia pridėkite vaizdo failus."},
            {"Added %1 video file(s)", "Pridėta %1 vaizdo failų"},
            {"Additional title:", "Papildomas pavadinimas:"},
            {"Background color:", "Fono spalva:"},
            {"Balanced (safer for short videos)", "Subalansuotas (saugesnis trumpiems vaizdo įrašams)"},
            {"Batch cancellation requested...", "Paprašyta atšaukti paketą..."},
            {"Browse...", "Naršyti..."},
            {"Cancel batch", "Atšaukti paketą"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Paprašyta atšaukti. Laukiama, kol dabartinis procesas sustos..."},
            {"Choose count text color", "Pasirinkite skaičiaus teksto spalvą"},
            {"Choose font...", "Pasirinkite šriftą..."},
            {"Choose info text color", "Pasirinkite informacinio teksto spalvą"},
            {"Choose tile border color", "Pasirinkite plytelės rėmelio spalvą"},
            {"Choose timestamp background color", "Pasirinkite laiko žymos fono spalvą"},
            {"Choose timestamp text color", "Pasirinkite laiko žymos teksto spalvą"},
            {"Choose title text color", "Pasirinkite pavadinimo teksto spalvą"},
            {"Choose...", "Pasirinkti..."},
            {"Clear preview", "Išvalyti peržiūrą"},
            {"Columns:", "Stulpeliai:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Kontaktinio lapo peržiūra"},
            {"Count text color:", "Skaičiaus teksto spalva:"},
            {"Delete Selection", "Ištrinti pasirinkimą"},
            {"Draw tile border", "Piešti plytelės rėmelį"},
            {"Edition", "Redagavimas"},
            {"End margin:", "Pabaigos paraštė:"},
            {"Exit", "Išeiti"},
            {"Fast (Thumbnail me 3.0 style)", "Greitas (Thumbnail me 3.0 stilius)"},
            {"File", "Failas"},
            {"Font:", "Šriftas:"},
            {"Gap:", "Tarpas:"},
            {"Generate a contact sheet first.", "Pirmiausia sugeneruokite kontaktinį lapą."},
            {"Info position:", "Informacijos padėtis:"},
            {"Info text color:", "Informacinio teksto spalva:"},
            {"Info text:", "Informacijos tekstas:"},
            {"Input / Output", "Įvestis / išvestis"},
            {"Input list cleared", "Įvesties sąrašas išvalytas"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Kalba pakeista. Išsaugokite nustatymus ir paleiskite programą iš naujo, kad pakeitimas būtų visiškai pritaikytas."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Kalbos pasirinkimas iškart įrašomas į settings.ini. Paleiskite programą iš naujo, kad statinės etiketės būtų visiškai pritaikytos."},
            {"Language:", "Kalba:"},
            {"Load", "Įkelti"},
            {"Load video files...", "Įkelti vaizdo failus..."},
            {"Load videos and click Start selected or Start all.", "Įkelkite vaizdo įrašus ir spustelėkite Pradėti pasirinktus arba Pradėti visus."},
            {"Lower Left", "Apačioje kairėje"},
            {"Lower Right", "Apačioje dešinėje"},
            {"No contact sheet to save", "Nėra kontaktinio lapo, kurį būtų galima išsaugoti"},
            {"Open Recent Configuration", "Atidaryti naujausią konfigūraciją"},
            {"Open configuration...", "Atidaryti konfigūraciją..."},
            {"Output", "Išvestis"},
            {"Output Format:", "Išvesties formatas:"},
            {"Output filename suffix:", "Išvesties failo vardo priesaga:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Išvesties kelias:"},
            {"Predefined Configuration", "Iš anksto nustatyta konfigūracija"},
            {"Preset applied: %1 rows × %2 columns", "Nustatymas pritaikytas: %1 eilutės × %2 stulpeliai"},
            {"Preview area", "Peržiūros sritis"},
            {"Preview style", "Stiliaus peržiūra"},
            {"Quality:", "Kokybė:"},
            {"Randomize amount:", "Atsitiktinumo kiekis:"},
            {"Randomize positions", "Atsitiktinai keisti padėtis"},
            {"Randomize:", "Atsitiktinumas:"},
            {"Reload settings.ini", "Iš naujo įkelti settings.ini"},
            {"Reset", "Atstatyti"},
            {"Rows:", "Eilutės:"},
            {"Same folder as source video", "Tas pats aplankas kaip šaltinio vaizdo įrašas"},
            {"Same source folder:", "Tas pats šaltinio aplankas:"},
            {"Save configuration", "Išsaugoti konfigūraciją"},
            {"Save configuration as...", "Išsaugoti konfigūraciją kaip..."},
            {"Save contact sheet", "Išsaugoti kontaktinį lapą"},
            {"Save failed", "Išsaugoti nepavyko"},
            {"Save manually", "Išsaugoti rankiniu būdu"},
            {"Save selected contact sheet...", "Išsaugoti pasirinktą kontaktinį lapą..."},
            {"Save settings.ini", "Išsaugoti settings.ini"},
            {"Saved: ", "Išsaugota: "},
            {"Seek mode:", "Paieškos režimas:"},
            {"Select output folder", "Pasirinkti išvesties aplanką"},
            {"Selected video file(s) removed", "Pasirinkti vaizdo failai pašalinti"},
            {"Settings saved:\n", "Nustatymai išsaugoti:\n"},
            {"Show additional title", "Rodyti papildomą pavadinimą"},
            {"Show technical info text", "Rodyti techninį informacinį tekstą"},
            {"Show timestamps", "Rodyti laiko žymas"},
            {"Similarity threshold:", "Panašumo slenkstis:"},
            {"Skip dark / blank frames", "Praleisti tamsius / tuščius kadrus"},
            {"Skip dark:", "Praleisti tamsius:"},
            {"Skip similar frames", "Praleisti panašius kadrus"},
            {"Skip similar:", "Praleisti panašius:"},
            {"Specific timestep:", "Konkretus laiko žingsnis:"},
            {"Start all", "Paleisti visus"},
            {"Start margin:", "Pradžios paraštė:"},
            {"Start selected", "Paleisti pasirinktus"},
            {"Style preview failed", "Stiliaus peržiūra nepavyko"},
            {"Style preview ready", "Stiliaus peržiūra paruošta"},
            {"Styles", "Stiliai"},
            {"The file could not be saved.", "Failo nepavyko išsaugoti."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Plytelės rėmelio spalva:"},
            {"Tile border:", "Plytelės rėmelis:"},
            {"Timestamp background:", "Laiko žymos fonas:"},
            {"Timestamp position:", "Laiko žymos padėtis:"},
            {"Timestamp text color:", "Laiko žymos teksto spalva:"},
            {"Timestamp:", "Laiko žyma:"},
            {"Timestep:", "Laiko žingsnis:"},
            {"Title text color:", "Pavadinimo teksto spalva:"},
            {"Title text:", "Antraštės tekstas:"},
            {"Upper Left", "Viršuje kairėje"},
            {"Upper Right", "Viršuje dešinėje"},
            {"Use specific timestep", "Naudoti konkretų laiko žingsnį"},
            {"View", "Rodinys"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Plotis:"},
            {"English", "Anglų"},
            {"French", "Prancūzų"},
            {"German", "Vokiečių"},
            {"Spanish", "Ispanų"},
            {"Dutch", "Nyderlandų"},
            {"Czech", "Čekų"},
            {"Italian", "Italų"},
            {"Hungarian", "Vengrų"},
            {"Croatian", "Kroatų"},
            {"Portuguese", "Portugalų"},
            {"Chinese", "Kinų"},
            {"Russian", "Rusų"}
        };

        static const QHash<QString, QString> mt = {
            {"%1 rows × %2 columns", "%1 ringieli × %2 kolonni"},
            {"About", "Dwar"},
            {"About Thumbnail me 4b2", "Dwar Thumbnail me 4b2"},
            {"Accurate (slowest)", "Preċiż (l-aktar bil-mod)"},
            {"Add and select a video first.", "Żid u agħżel vidjo l-ewwel."},
            {"Add video files", "Żid fajls tal-vidjo"},
            {"Add video files first.", "Żid fajls tal-vidjo l-ewwel."},
            {"Added %1 video file(s)", "Żdiedu %1 fajl(s) tal-vidjo"},
            {"Additional title:", "Titlu addizzjonali:"},
            {"Background color:", "Kulur tal-isfond:"},
            {"Balanced (safer for short videos)", "Ibbilanċjat (aktar sigur għal vidjows qosra)"},
            {"Batch cancellation requested...", "Intalbet il-kanċellazzjoni tal-lott..."},
            {"Browse...", "Fittex..."},
            {"Cancel batch", "Ikkanċella l-lott"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Intalbet kanċellazzjoni. Qed nistennew li l-proċess kurrenti jieqaf..."},
            {"Choose count text color", "Agħżel il-kulur tat-test tal-għadd"},
            {"Choose font...", "Agħżel font..."},
            {"Choose info text color", "Agħżel il-kulur tat-test tal-informazzjoni"},
            {"Choose tile border color", "Agħżel il-kulur tal-bordura tal-maduma"},
            {"Choose timestamp background color", "Agħżel il-kulur tal-isfond tat-timbru tal-ħin"},
            {"Choose timestamp text color", "Agħżel il-kulur tat-test tat-timbru tal-ħin"},
            {"Choose title text color", "Agħżel il-kulur tat-test tat-titlu"},
            {"Choose...", "Agħżel..."},
            {"Clear preview", "Naddaf il-previżjoni"},
            {"Columns:", "Kolonni:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Previżjoni tal-folja ta’ kuntatt"},
            {"Count text color:", "Kulur tat-test tal-għadd:"},
            {"Delete Selection", "Ħassar l-għażla"},
            {"Draw tile border", "Pinġi bordura tal-maduma"},
            {"Edition", "Editjar"},
            {"End margin:", "Marġni tat-tmiem:"},
            {"Exit", "Oħroġ"},
            {"Fast (Thumbnail me 3.0 style)", "Mgħaġġel (stil Thumbnail me 3.0)"},
            {"File", "Fajl"},
            {"Font:", "Font:"},
            {"Gap:", "Spazju:"},
            {"Generate a contact sheet first.", "Iġġenera folja ta’ kuntatt l-ewwel."},
            {"Info position:", "Pożizzjoni tal-informazzjoni:"},
            {"Info text color:", "Kulur tat-test tal-informazzjoni:"},
            {"Info text:", "Test taʼ informazzjoni:"},
            {"Input / Output", "Input / output"},
            {"Input list cleared", "Il-lista tad-dħul tnaddfet"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Il-lingwa nbidlet. Issejvja s-settings u erġa’ ibda l-applikazzjoni biex tapplikaha kompletament."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "L-għażla tal-lingwa tiġi ssejvjata minnufih f’settings.ini. Erġa’ ibda l-applikazzjoni biex tapplika t-tikketti statiċi kompletament."},
            {"Language:", "Lingwa:"},
            {"Load", "Iftaħ"},
            {"Load video files...", "Iftaħ fajls tal-vidjo..."},
            {"Load videos and click Start selected or Start all.", "Għabbi l-vidjows u ikklikkja Ibda magħżula jew Ibda kollha."},
            {"Lower Left", "Isfel xellug"},
            {"Lower Right", "Isfel lemin"},
            {"No contact sheet to save", "M’hemm l-ebda folja ta’ kuntatt biex tissejvja"},
            {"Open Recent Configuration", "Iftaħ konfigurazzjoni riċenti"},
            {"Open configuration...", "Iftaħ konfigurazzjoni..."},
            {"Output", "Output"},
            {"Output Format:", "Format tal-output:"},
            {"Output filename suffix:", "Suffiss tal-isem tal-fajl tal-output:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Mogħdija tal-output:"},
            {"Predefined Configuration", "Konfigurazzjoni predefinita"},
            {"Preset applied: %1 rows × %2 columns", "Preset applikat: %1 ringieli × %2 kolonni"},
            {"Preview area", "Żona tal-previżjoni"},
            {"Preview style", "Previżjoni tal-istil"},
            {"Quality:", "Kwalità:"},
            {"Randomize amount:", "Ammont ta’ każwalità:"},
            {"Randomize positions", "Każwalizza l-pożizzjonijiet"},
            {"Randomize:", "Każwalizza:"},
            {"Reload settings.ini", "Erġa’ għabbi settings.ini"},
            {"Reset", "Irrisettja"},
            {"Rows:", "Ringieli:"},
            {"Same folder as source video", "L-istess folder bħall-vidjo sors"},
            {"Same source folder:", "L-istess folder tas-sors:"},
            {"Save configuration", "Issejvja konfigurazzjoni"},
            {"Save configuration as...", "Issejvja konfigurazzjoni bħala..."},
            {"Save contact sheet", "Issejvja folja ta’ kuntatt"},
            {"Save failed", "Is-sejvjar falla"},
            {"Save manually", "Issejvja manwalment"},
            {"Save selected contact sheet...", "Issejvja l-folja ta’ kuntatt magħżula..."},
            {"Save settings.ini", "Issejvja settings.ini"},
            {"Saved: ", "Issejvjat: "},
            {"Seek mode:", "Mod ta’ tfittxija:"},
            {"Select output folder", "Agħżel folder tal-output"},
            {"Selected video file(s) removed", "Il-fajls tal-vidjo magħżula tneħħew"},
            {"Settings saved:\n", "Is-settings ġew issejvjati:\n"},
            {"Show additional title", "Uri titlu addizzjonali"},
            {"Show technical info text", "Uri test ta’ informazzjoni teknika"},
            {"Show timestamps", "Uri marki tal-ħin"},
            {"Similarity threshold:", "Limitu ta’ xebh:"},
            {"Skip dark / blank frames", "Aqbeż frejms skuri / vojta"},
            {"Skip dark:", "Aqbeż skuri:"},
            {"Skip similar frames", "Aqbeż frejms simili"},
            {"Skip similar:", "Aqbeż simili:"},
            {"Specific timestep:", "Pass ta’ ħin speċifiku:"},
            {"Start all", "Ibda kollha"},
            {"Start margin:", "Marġni tal-bidu:"},
            {"Start selected", "Ibda l-magħżula"},
            {"Style preview failed", "Il-previżjoni tal-istil falliet"},
            {"Style preview ready", "Il-previżjoni tal-istil lesta"},
            {"Styles", "Stili"},
            {"The file could not be saved.", "Il-fajl ma setax jiġi ssejvjat."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Kulur tal-bordura tal-maduma:"},
            {"Tile border:", "Bordura tal-maduma:"},
            {"Timestamp background:", "Sfond tat-timbru tal-ħin:"},
            {"Timestamp position:", "Pożizzjoni tat-timbru tal-ħin:"},
            {"Timestamp text color:", "Kulur tat-test tat-timbru tal-ħin:"},
            {"Timestamp:", "Timbru tal-ħin:"},
            {"Timestep:", "Pass ta’ ħin:"},
            {"Title text color:", "Kulur tat-test tat-titlu:"},
            {"Title text:", "Test tat-titlu:"},
            {"Upper Left", "Fuq xellug"},
            {"Upper Right", "Fuq lemin"},
            {"Use specific timestep", "Uża pass ta’ ħin speċifiku"},
            {"View", "Ara"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Wisaʼ:"},
            {"English", "Ingliż"},
            {"French", "Franċiż"},
            {"German", "Ġermaniż"},
            {"Spanish", "Spanjol"},
            {"Dutch", "Olandiż"},
            {"Czech", "Ċek"},
            {"Italian", "Taljan"},
            {"Hungarian", "Ungeriż"},
            {"Croatian", "Kroat"},
            {"Portuguese", "Portugiż"},
            {"Chinese", "Ċiniż"},
            {"Russian", "Russu"}
        };

        static const QHash<QString, QString> pl = {
            {"%1 rows × %2 columns", "%1 wierszy × %2 kolumn"},
            {"About", "O programie"},
            {"About Thumbnail me 4b2", "O Thumbnail me 4b2"},
            {"Accurate (slowest)", "Dokładny (najwolniejszy)"},
            {"Add and select a video first.", "Najpierw dodaj i wybierz film."},
            {"Add video files", "Dodaj pliki wideo"},
            {"Add video files first.", "Najpierw dodaj pliki wideo."},
            {"Added %1 video file(s)", "Dodano %1 plików wideo"},
            {"Additional title:", "Dodatkowy tytuł:"},
            {"Background color:", "Kolor tła:"},
            {"Balanced (safer for short videos)", "Zrównoważony (bezpieczniejszy dla krótkich filmów)"},
            {"Batch cancellation requested...", "Zażądano anulowania kolejki..."},
            {"Browse...", "Przeglądaj..."},
            {"Cancel batch", "Anuluj kolejkę"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Zażądano anulowania. Oczekiwanie na zatrzymanie bieżącego procesu..."},
            {"Choose count text color", "Wybierz kolor tekstu licznika"},
            {"Choose font...", "Wybierz czcionkę..."},
            {"Choose info text color", "Wybierz kolor tekstu informacji"},
            {"Choose tile border color", "Wybierz kolor obramowania kafelka"},
            {"Choose timestamp background color", "Wybierz kolor tła znacznika czasu"},
            {"Choose timestamp text color", "Wybierz kolor tekstu znacznika czasu"},
            {"Choose title text color", "Wybierz kolor tekstu tytułu"},
            {"Choose...", "Wybierz..."},
            {"Clear preview", "Wyczyść podgląd"},
            {"Columns:", "Kolumny:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Podgląd arkusza miniatur"},
            {"Count text color:", "Kolor tekstu licznika:"},
            {"Delete Selection", "Usuń zaznaczenie"},
            {"Draw tile border", "Rysuj obramowanie kafelka"},
            {"Edition", "Edycja"},
            {"End margin:", "Margines końcowy:"},
            {"Exit", "Zakończ"},
            {"Fast (Thumbnail me 3.0 style)", "Szybki (styl Thumbnail me 3.0)"},
            {"File", "Plik"},
            {"Font:", "Czcionka:"},
            {"Gap:", "Odstęp:"},
            {"Generate a contact sheet first.", "Najpierw wygeneruj arkusz miniatur."},
            {"Info position:", "Pozycja informacji:"},
            {"Info text color:", "Kolor tekstu informacji:"},
            {"Info text:", "Tekst informacji:"},
            {"Input / Output", "Wejście / wyjście"},
            {"Input list cleared", "Lista wejściowa wyczyszczona"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Język został zmieniony. Zapisz ustawienia i uruchom aplikację ponownie, aby w pełni zastosować zmianę."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Wybór języka jest natychmiast zapisywany w settings.ini. Uruchom aplikację ponownie, aby w pełni zastosować statyczne etykiety."},
            {"Language:", "Język:"},
            {"Load", "Wczytaj"},
            {"Load video files...", "Wczytaj pliki wideo..."},
            {"Load videos and click Start selected or Start all.", "Wczytaj filmy i kliknij Start zaznaczone lub Start wszystkie."},
            {"Lower Left", "Lewy dolny"},
            {"Lower Right", "Prawy dolny"},
            {"No contact sheet to save", "Brak arkusza miniatur do zapisania"},
            {"Open Recent Configuration", "Otwórz ostatnią konfigurację"},
            {"Open configuration...", "Otwórz konfigurację..."},
            {"Output", "Wyjście"},
            {"Output Format:", "Format wyjściowy:"},
            {"Output filename suffix:", "Sufiks nazwy pliku wyjściowego:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Ścieżka wyjściowa:"},
            {"Predefined Configuration", "Konfiguracja predefiniowana"},
            {"Preset applied: %1 rows × %2 columns", "Zastosowano preset: %1 wierszy × %2 kolumn"},
            {"Preview area", "Obszar podglądu"},
            {"Preview style", "Podgląd stylu"},
            {"Quality:", "Jakość:"},
            {"Randomize amount:", "Poziom losowości:"},
            {"Randomize positions", "Losuj pozycje"},
            {"Randomize:", "Losowość:"},
            {"Reload settings.ini", "Przeładuj settings.ini"},
            {"Reset", "Resetuj"},
            {"Rows:", "Wiersze:"},
            {"Same folder as source video", "Ten sam folder co film źródłowy"},
            {"Same source folder:", "Ten sam folder źródłowy:"},
            {"Save configuration", "Zapisz konfigurację"},
            {"Save configuration as...", "Zapisz konfigurację jako..."},
            {"Save contact sheet", "Zapisz arkusz miniatur"},
            {"Save failed", "Zapisywanie nie powiodło się"},
            {"Save manually", "Zapisz ręcznie"},
            {"Save selected contact sheet...", "Zapisz wybrany arkusz miniatur..."},
            {"Save settings.ini", "Zapisz settings.ini"},
            {"Saved: ", "Zapisano: "},
            {"Seek mode:", "Tryb wyszukiwania:"},
            {"Select output folder", "Wybierz folder wyjściowy"},
            {"Selected video file(s) removed", "Usunięto wybrane pliki wideo"},
            {"Settings saved:\n", "Ustawienia zapisane:\n"},
            {"Show additional title", "Pokaż dodatkowy tytuł"},
            {"Show technical info text", "Pokaż techniczny tekst informacji"},
            {"Show timestamps", "Pokaż znaczniki czasu"},
            {"Similarity threshold:", "Próg podobieństwa:"},
            {"Skip dark / blank frames", "Pomiń ciemne / puste klatki"},
            {"Skip dark:", "Pomiń ciemne:"},
            {"Skip similar frames", "Pomiń podobne klatki"},
            {"Skip similar:", "Pomiń podobne:"},
            {"Specific timestep:", "Określony krok czasu:"},
            {"Start all", "Uruchom wszystko"},
            {"Start margin:", "Margines początkowy:"},
            {"Start selected", "Uruchom wybrane"},
            {"Style preview failed", "Podgląd stylu nie powiódł się"},
            {"Style preview ready", "Podgląd stylu gotowy"},
            {"Styles", "Style"},
            {"The file could not be saved.", "Nie można zapisać pliku."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Kolor obramowania kafelka:"},
            {"Tile border:", "Obramowanie kafelka:"},
            {"Timestamp background:", "Tło znacznika czasu:"},
            {"Timestamp position:", "Pozycja znacznika czasu:"},
            {"Timestamp text color:", "Kolor tekstu znacznika czasu:"},
            {"Timestamp:", "Znacznik czasu:"},
            {"Timestep:", "Krok czasu:"},
            {"Title text color:", "Kolor tekstu tytułu:"},
            {"Title text:", "Tekst tytułu:"},
            {"Upper Left", "Lewy górny"},
            {"Upper Right", "Prawy górny"},
            {"Use specific timestep", "Użyj określonego kroku czasu"},
            {"View", "Widok"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Szerokość:"},
            {"English", "Angielski"},
            {"French", "Francuski"},
            {"German", "Niemiecki"},
            {"Spanish", "Hiszpański"},
            {"Dutch", "Niderlandzki"},
            {"Czech", "Czeski"},
            {"Italian", "Włoski"},
            {"Hungarian", "Węgierski"},
            {"Croatian", "Chorwacki"},
            {"Portuguese", "Portugalski"},
            {"Chinese", "Chiński"},
            {"Russian", "Rosyjski"}
        };

        static const QHash<QString, QString> ro = {
            {"%1 rows × %2 columns", "%1 rânduri × %2 coloane"},
            {"About", "Despre"},
            {"About Thumbnail me 4b2", "Despre Thumbnail me 4b2"},
            {"Accurate (slowest)", "Precis (cel mai lent)"},
            {"Add and select a video first.", "Adaugă și selectează mai întâi un videoclip."},
            {"Add video files", "Adaugă fișiere video"},
            {"Add video files first.", "Adaugă mai întâi fișiere video."},
            {"Added %1 video file(s)", "Au fost adăugate %1 fișier(e) video"},
            {"Additional title:", "Titlu suplimentar:"},
            {"Background color:", "Culoare fundal:"},
            {"Balanced (safer for short videos)", "Echilibrat (mai sigur pentru videoclipuri scurte)"},
            {"Batch cancellation requested...", "S-a solicitat anularea lotului..."},
            {"Browse...", "Răsfoiește..."},
            {"Cancel batch", "Anulează lotul"},
            {"Cancellation requested. Waiting for the current worker to stop...", "S-a solicitat anularea. Se așteaptă oprirea procesului curent..."},
            {"Choose count text color", "Alege culoarea textului contorului"},
            {"Choose font...", "Alege fontul..."},
            {"Choose info text color", "Alege culoarea textului informativ"},
            {"Choose tile border color", "Alege culoarea chenarului plăcii"},
            {"Choose timestamp background color", "Alege culoarea fundalului marcajului temporal"},
            {"Choose timestamp text color", "Alege culoarea textului marcajului temporal"},
            {"Choose title text color", "Alege culoarea textului titlului"},
            {"Choose...", "Alege..."},
            {"Clear preview", "Golește previzualizarea"},
            {"Columns:", "Coloane:"},
            {"Config 1", "Config. 1"},
            {"Config 2", "Config. 2"},
            {"Config 3", "Config. 3"},
            {"Contact sheet preview", "Previzualizarea foii de contact"},
            {"Count text color:", "Culoarea textului contorului:"},
            {"Delete Selection", "Șterge selecția"},
            {"Draw tile border", "Desenează chenarul plăcii"},
            {"Edition", "Editare"},
            {"End margin:", "Margine finală:"},
            {"Exit", "Ieșire"},
            {"Fast (Thumbnail me 3.0 style)", "Rapid (stil Thumbnail me 3.0)"},
            {"File", "Fișier"},
            {"Font:", "Font:"},
            {"Gap:", "Spațiu:"},
            {"Generate a contact sheet first.", "Generează mai întâi o foaie de contact."},
            {"Info position:", "Poziția informațiilor:"},
            {"Info text color:", "Culoarea textului informativ:"},
            {"Info text:", "Text informații:"},
            {"Input / Output", "Intrare / ieșire"},
            {"Input list cleared", "Lista de intrare a fost golită"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Limba a fost schimbată. Salvează setările și repornește aplicația pentru aplicare completă."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Selecția limbii este salvată imediat în settings.ini. Repornește aplicația pentru a aplica complet etichetele statice."},
            {"Language:", "Limbă:"},
            {"Load", "Încarcă"},
            {"Load video files...", "Încarcă fișiere video..."},
            {"Load videos and click Start selected or Start all.", "Încarcă videoclipuri și apasă Pornește selectate sau Pornește toate."},
            {"Lower Left", "Stânga jos"},
            {"Lower Right", "Dreapta jos"},
            {"No contact sheet to save", "Nu există foaie de contact de salvat"},
            {"Open Recent Configuration", "Deschide configurația recentă"},
            {"Open configuration...", "Deschide configurația..."},
            {"Output", "Ieșire"},
            {"Output Format:", "Format de ieșire:"},
            {"Output filename suffix:", "Sufixul numelui fișierului de ieșire:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Calea de ieșire:"},
            {"Predefined Configuration", "Configurație predefinită"},
            {"Preset applied: %1 rows × %2 columns", "Presetare aplicată: %1 rânduri × %2 coloane"},
            {"Preview area", "Zonă de previzualizare"},
            {"Preview style", "Previzualizare stil"},
            {"Quality:", "Calitate:"},
            {"Randomize amount:", "Cantitate aleatorizare:"},
            {"Randomize positions", "Aleatorizează pozițiile"},
            {"Randomize:", "Aleatorizare:"},
            {"Reload settings.ini", "Reîncarcă settings.ini"},
            {"Reset", "Resetare"},
            {"Rows:", "Rânduri:"},
            {"Same folder as source video", "Același folder ca videoclipul sursă"},
            {"Same source folder:", "Același folder sursă:"},
            {"Save configuration", "Salvează configurația"},
            {"Save configuration as...", "Salvează configurația ca..."},
            {"Save contact sheet", "Salvează foaia de contact"},
            {"Save failed", "Salvarea a eșuat"},
            {"Save manually", "Salvează manual"},
            {"Save selected contact sheet...", "Salvează foaia de contact selectată..."},
            {"Save settings.ini", "Salvează settings.ini"},
            {"Saved: ", "Salvat: "},
            {"Seek mode:", "Mod de căutare:"},
            {"Select output folder", "Selectează folderul de ieșire"},
            {"Selected video file(s) removed", "Fișierele video selectate au fost eliminate"},
            {"Settings saved:\n", "Setările au fost salvate:\n"},
            {"Show additional title", "Afișează titlul suplimentar"},
            {"Show technical info text", "Afișează textul tehnic informativ"},
            {"Show timestamps", "Afișează marcaje temporale"},
            {"Similarity threshold:", "Prag de similaritate:"},
            {"Skip dark / blank frames", "Sari peste cadre întunecate / goale"},
            {"Skip dark:", "Sari peste întunecate:"},
            {"Skip similar frames", "Sari peste cadre similare"},
            {"Skip similar:", "Sari peste similare:"},
            {"Specific timestep:", "Pas de timp specific:"},
            {"Start all", "Pornește tot"},
            {"Start margin:", "Margine de început:"},
            {"Start selected", "Pornește selectate"},
            {"Style preview failed", "Previzualizarea stilului a eșuat"},
            {"Style preview ready", "Previzualizarea stilului este gata"},
            {"Styles", "Stiluri"},
            {"The file could not be saved.", "Fișierul nu a putut fi salvat."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Culoarea chenarului plăcii:"},
            {"Tile border:", "Chenarul plăcii:"},
            {"Timestamp background:", "Fundal marcaj temporal:"},
            {"Timestamp position:", "Poziția marcajului temporal:"},
            {"Timestamp text color:", "Culoarea textului marcajului temporal:"},
            {"Timestamp:", "Marcaj temporal:"},
            {"Timestep:", "Pas de timp:"},
            {"Title text color:", "Culoarea textului titlului:"},
            {"Title text:", "Text titlu:"},
            {"Upper Left", "Stânga sus"},
            {"Upper Right", "Dreapta sus"},
            {"Use specific timestep", "Folosește pas de timp specific"},
            {"View", "Vizualizare"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Lățime:"},
            {"English", "Engleză"},
            {"French", "Franceză"},
            {"German", "Germană"},
            {"Spanish", "Spaniolă"},
            {"Dutch", "Neerlandeză"},
            {"Czech", "Cehă"},
            {"Italian", "Italiană"},
            {"Hungarian", "Maghiară"},
            {"Croatian", "Croată"},
            {"Portuguese", "Portugheză"},
            {"Chinese", "Chineză"},
            {"Russian", "Rusă"}
        };

        static const QHash<QString, QString> sk = {
            {"%1 rows × %2 columns", "%1 riadkov × %2 stĺpcov"},
            {"About", "O aplikácii"},
            {"About Thumbnail me 4b2", "O Thumbnail me 4b2"},
            {"Accurate (slowest)", "Presný (najpomalší)"},
            {"Add and select a video first.", "Najprv pridaj a vyber video."},
            {"Add video files", "Pridať video súbory"},
            {"Add video files first.", "Najprv pridaj video súbory."},
            {"Added %1 video file(s)", "Pridaných %1 video súborov"},
            {"Additional title:", "Doplnkový titulok:"},
            {"Background color:", "Farba pozadia:"},
            {"Balanced (safer for short videos)", "Vyvážený (bezpečnejší pre krátke videá)"},
            {"Batch cancellation requested...", "Vyžiadané zrušenie dávky..."},
            {"Browse...", "Prehľadávať..."},
            {"Cancel batch", "Zrušiť dávku"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Zrušenie vyžiadané. Čakám na zastavenie aktuálneho procesu..."},
            {"Choose count text color", "Vybrať farbu textu počítadla"},
            {"Choose font...", "Vybrať písmo..."},
            {"Choose info text color", "Vybrať farbu informačného textu"},
            {"Choose tile border color", "Vybrať farbu okraja dlaždice"},
            {"Choose timestamp background color", "Vybrať farbu pozadia časovej značky"},
            {"Choose timestamp text color", "Vybrať farbu textu časovej značky"},
            {"Choose title text color", "Vybrať farbu textu titulku"},
            {"Choose...", "Vybrať..."},
            {"Clear preview", "Vyčistiť náhľad"},
            {"Columns:", "Stĺpce:"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Contact sheet preview", "Náhľad kontaktnej mozaiky"},
            {"Count text color:", "Farba textu počítadla:"},
            {"Delete Selection", "Odstrániť výber"},
            {"Draw tile border", "Kresliť okraj dlaždice"},
            {"Edition", "Úpravy"},
            {"End margin:", "Koncový okraj:"},
            {"Exit", "Koniec"},
            {"Fast (Thumbnail me 3.0 style)", "Rýchly (štýl Thumbnail me 3.0)"},
            {"File", "Súbor"},
            {"Font:", "Písmo:"},
            {"Gap:", "Medzera:"},
            {"Generate a contact sheet first.", "Najprv vygeneruj kontaktnú mozaiku."},
            {"Info position:", "Pozícia informácií:"},
            {"Info text color:", "Farba informačného textu:"},
            {"Info text:", "Informačný text:"},
            {"Input / Output", "Vstup / výstup"},
            {"Input list cleared", "Zoznam vstupov vyčistený"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jazyk bol zmenený. Ulož nastavenia a reštartuj aplikáciu, aby sa zmena úplne prejavila."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Výber jazyka sa okamžite uloží do settings.ini. Reštartuj aplikáciu, aby sa statické popisky úplne prejavili."},
            {"Language:", "Jazyk:"},
            {"Load", "Načítať"},
            {"Load video files...", "Načítať video súbory..."},
            {"Load videos and click Start selected or Start all.", "Načítaj videá a klikni Spustiť vybrané alebo Spustiť všetko."},
            {"Lower Left", "Vľavo dole"},
            {"Lower Right", "Vpravo dole"},
            {"No contact sheet to save", "Nie je čo uložiť"},
            {"Open Recent Configuration", "Otvoriť nedávnu konfiguráciu"},
            {"Open configuration...", "Otvoriť konfiguráciu..."},
            {"Output", "Výstup"},
            {"Output Format:", "Výstupný formát:"},
            {"Output filename suffix:", "Prípona názvu výstupného súboru:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Výstupná cesta:"},
            {"Predefined Configuration", "Preddefinovaná konfigurácia"},
            {"Preset applied: %1 rows × %2 columns", "Predvoľba použitá: %1 riadkov × %2 stĺpcov"},
            {"Preview area", "Oblasť náhľadu"},
            {"Preview style", "Náhľad štýlu"},
            {"Quality:", "Kvalita:"},
            {"Randomize amount:", "Miera náhodnosti:"},
            {"Randomize positions", "Náhodne posunúť pozície"},
            {"Randomize:", "Náhodnosť:"},
            {"Reload settings.ini", "Znovu načítať settings.ini"},
            {"Reset", "Resetovať"},
            {"Rows:", "Riadky:"},
            {"Same folder as source video", "Rovnaký priečinok ako zdrojové video"},
            {"Same source folder:", "Rovnaký zdrojový priečinok:"},
            {"Save configuration", "Uložiť konfiguráciu"},
            {"Save configuration as...", "Uložiť konfiguráciu ako..."},
            {"Save contact sheet", "Uložiť kontaktnú mozaiku"},
            {"Save failed", "Uloženie zlyhalo"},
            {"Save manually", "Uložiť ručne"},
            {"Save selected contact sheet...", "Uložiť vybranú kontaktnú mozaiku..."},
            {"Save settings.ini", "Uložiť settings.ini"},
            {"Saved: ", "Uložené: "},
            {"Seek mode:", "Režim vyhľadávania:"},
            {"Select output folder", "Vybrať výstupný priečinok"},
            {"Selected video file(s) removed", "Vybrané video súbory odstránené"},
            {"Settings saved:\n", "Nastavenia uložené:\n"},
            {"Show additional title", "Zobraziť doplnkový titulok"},
            {"Show technical info text", "Zobraziť technický informačný text"},
            {"Show timestamps", "Zobraziť časové značky"},
            {"Similarity threshold:", "Prah podobnosti:"},
            {"Skip dark / blank frames", "Preskočiť tmavé / prázdne snímky"},
            {"Skip dark:", "Preskočiť tmavé:"},
            {"Skip similar frames", "Preskočiť podobné snímky"},
            {"Skip similar:", "Preskočiť podobné:"},
            {"Specific timestep:", "Konkrétny časový krok:"},
            {"Start all", "Spustiť všetko"},
            {"Start margin:", "Počiatočný okraj:"},
            {"Start selected", "Spustiť vybrané"},
            {"Style preview failed", "Náhľad štýlu zlyhal"},
            {"Style preview ready", "Náhľad štýlu je pripravený"},
            {"Styles", "Štýly"},
            {"The file could not be saved.", "Súbor sa nepodarilo uložiť."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Farba okraja dlaždice:"},
            {"Tile border:", "Okraj dlaždice:"},
            {"Timestamp background:", "Pozadie časovej značky:"},
            {"Timestamp position:", "Pozícia časovej značky:"},
            {"Timestamp text color:", "Farba textu časovej značky:"},
            {"Timestamp:", "Časová značka:"},
            {"Timestep:", "Časový krok:"},
            {"Title text color:", "Farba textu titulku:"},
            {"Title text:", "Text titulku:"},
            {"Upper Left", "Vľavo hore"},
            {"Upper Right", "Vpravo hore"},
            {"Use specific timestep", "Použiť konkrétny časový krok"},
            {"View", "Zobrazenie"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Šírka:"},
            {"English", "Angličtina"},
            {"French", "Francúzština"},
            {"German", "Nemčina"},
            {"Spanish", "Španielčina"},
            {"Dutch", "Holandčina"},
            {"Czech", "Čeština"},
            {"Italian", "Taliančina"},
            {"Hungarian", "Maďarčina"},
            {"Croatian", "Chorvátčina"},
            {"Portuguese", "Portugalčina"},
            {"Chinese", "Čínština"},
            {"Russian", "Ruština"}
        };

        static const QHash<QString, QString> sl = {
            {"%1 rows × %2 columns", "%1 vrstic × %2 stolpcev"},
            {"About", "O programu"},
            {"About Thumbnail me 4b2", "O Thumbnail me 4b2"},
            {"Accurate (slowest)", "Natančno (najpočasnejše)"},
            {"Add and select a video first.", "Najprej dodajte in izberite video."},
            {"Add video files", "Dodaj video datoteke"},
            {"Add video files first.", "Najprej dodajte video datoteke."},
            {"Added %1 video file(s)", "Dodanih %1 video datotek"},
            {"Additional title:", "Dodatni naslov:"},
            {"Background color:", "Barva ozadja:"},
            {"Balanced (safer for short videos)", "Uravnoteženo (varnejše za kratke videe)"},
            {"Batch cancellation requested...", "Zahtevana je preklic serije..."},
            {"Browse...", "Prebrskaj..."},
            {"Cancel batch", "Prekliči paket"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Zahtevan je preklic. Čakanje, da se trenutni proces ustavi..."},
            {"Choose count text color", "Izberi barvo besedila števca"},
            {"Choose font...", "Izberi pisavo..."},
            {"Choose info text color", "Izberi barvo informativnega besedila"},
            {"Choose tile border color", "Izberi barvo obrobe ploščice"},
            {"Choose timestamp background color", "Izberi barvo ozadja časovnega žiga"},
            {"Choose timestamp text color", "Izberi barvo besedila časovnega žiga"},
            {"Choose title text color", "Izberi barvo besedila naslova"},
            {"Choose...", "Izberi..."},
            {"Clear preview", "Počisti predogled"},
            {"Columns:", "Stolpci:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Predogled kontaktnega lista"},
            {"Count text color:", "Barva besedila števca:"},
            {"Delete Selection", "Izbriši izbor"},
            {"Draw tile border", "Nariši obrobo ploščice"},
            {"Edition", "Urejanje"},
            {"End margin:", "Končni rob:"},
            {"Exit", "Izhod"},
            {"Fast (Thumbnail me 3.0 style)", "Hitro (slog Thumbnail me 3.0)"},
            {"File", "Datoteka"},
            {"Font:", "Pisava:"},
            {"Gap:", "Razmik:"},
            {"Generate a contact sheet first.", "Najprej ustvari kontaktni list."},
            {"Info position:", "Položaj informacij:"},
            {"Info text color:", "Barva informativnega besedila:"},
            {"Info text:", "Informacijsko besedilo:"},
            {"Input / Output", "Vhod / izhod"},
            {"Input list cleared", "Seznam vhodov je počiščen"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jezik je spremenjen. Shranite nastavitve in znova zaženite program, da se sprememba v celoti uporabi."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Izbira jezika se takoj shrani v settings.ini. Znova zaženite program, da se statične oznake v celoti uporabijo."},
            {"Language:", "Jezik:"},
            {"Load", "Naloži"},
            {"Load video files...", "Naloži video datoteke..."},
            {"Load videos and click Start selected or Start all.", "Naloži videe in klikni Zaženi izbrane ali Zaženi vse."},
            {"Lower Left", "Spodaj levo"},
            {"Lower Right", "Spodaj desno"},
            {"No contact sheet to save", "Ni kontaktnega lista za shranjevanje"},
            {"Open Recent Configuration", "Odpri nedavno konfiguracijo"},
            {"Open configuration...", "Odpri konfiguracijo..."},
            {"Output", "Izhod"},
            {"Output Format:", "Izhodni format:"},
            {"Output filename suffix:", "Pripona imena izhodne datoteke:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Izhodna pot:"},
            {"Predefined Configuration", "Prednastavljena konfiguracija"},
            {"Preset applied: %1 rows × %2 columns", "Prednastavitev uporabljena: %1 vrstic × %2 stolpcev"},
            {"Preview area", "Območje predogleda"},
            {"Preview style", "Predogled sloga"},
            {"Quality:", "Kakovost:"},
            {"Randomize amount:", "Količina naključnosti:"},
            {"Randomize positions", "Naključno razporedi položaje"},
            {"Randomize:", "Naključno:"},
            {"Reload settings.ini", "Znova naloži settings.ini"},
            {"Reset", "Ponastavi"},
            {"Rows:", "Vrstice:"},
            {"Same folder as source video", "Ista mapa kot izvorni video"},
            {"Same source folder:", "Ista izvorna mapa:"},
            {"Save configuration", "Shrani konfiguracijo"},
            {"Save configuration as...", "Shrani konfiguracijo kot..."},
            {"Save contact sheet", "Shrani kontaktni list"},
            {"Save failed", "Shranjevanje ni uspelo"},
            {"Save manually", "Shrani ročno"},
            {"Save selected contact sheet...", "Shrani izbrani kontaktni list..."},
            {"Save settings.ini", "Shrani settings.ini"},
            {"Saved: ", "Shranjeno: "},
            {"Seek mode:", "Način iskanja:"},
            {"Select output folder", "Izberi izhodno mapo"},
            {"Selected video file(s) removed", "Izbrane video datoteke odstranjene"},
            {"Settings saved:\n", "Nastavitve shranjene:\n"},
            {"Show additional title", "Prikaži dodatni naslov"},
            {"Show technical info text", "Prikaži tehnično informativno besedilo"},
            {"Show timestamps", "Prikaži časovne žige"},
            {"Similarity threshold:", "Prag podobnosti:"},
            {"Skip dark / blank frames", "Preskoči temne / prazne sličice"},
            {"Skip dark:", "Preskoči temne:"},
            {"Skip similar frames", "Preskoči podobne sličice"},
            {"Skip similar:", "Preskoči podobne:"},
            {"Specific timestep:", "Določen časovni korak:"},
            {"Start all", "Zaženi vse"},
            {"Start margin:", "Začetni rob:"},
            {"Start selected", "Zaženi izbrane"},
            {"Style preview failed", "Predogled sloga ni uspel"},
            {"Style preview ready", "Predogled sloga je pripravljen"},
            {"Styles", "Slogi"},
            {"The file could not be saved.", "Datoteke ni bilo mogoče shraniti."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Barva obrobe ploščice:"},
            {"Tile border:", "Obroba ploščice:"},
            {"Timestamp background:", "Ozadje časovnega žiga:"},
            {"Timestamp position:", "Položaj časovnega žiga:"},
            {"Timestamp text color:", "Barva besedila časovnega žiga:"},
            {"Timestamp:", "Časovni žig:"},
            {"Timestep:", "Časovni korak:"},
            {"Title text color:", "Barva besedila naslova:"},
            {"Title text:", "Besedilo naslova:"},
            {"Upper Left", "Zgoraj levo"},
            {"Upper Right", "Zgoraj desno"},
            {"Use specific timestep", "Uporabi določen časovni korak"},
            {"View", "Pogled"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Širina:"},
            {"English", "Angleščina"},
            {"French", "Francoščina"},
            {"German", "Nemščina"},
            {"Spanish", "Španščina"},
            {"Dutch", "Nizozemščina"},
            {"Czech", "Češčina"},
            {"Italian", "Italijanščina"},
            {"Hungarian", "Madžarščina"},
            {"Croatian", "Hrvaščina"},
            {"Portuguese", "Portugalščina"},
            {"Chinese", "Kitajščina"},
            {"Russian", "Ruščina"}
        };

        static const QHash<QString, QString> fi = {
            {"%1 rows × %2 columns", "%1 riviä × %2 saraketta"},
            {"About", "Tietoja"},
            {"About Thumbnail me 4b2", "Tietoja Thumbnail me 4b2:stä"},
            {"Accurate (slowest)", "Tarkka (hitain)"},
            {"Add and select a video first.", "Lisää ja valitse ensin video."},
            {"Add video files", "Lisää videotiedostoja"},
            {"Add video files first.", "Lisää ensin videotiedostot."},
            {"Added %1 video file(s)", "Lisätty %1 videotiedosto(a)"},
            {"Additional title:", "Lisäotsikko:"},
            {"Background color:", "Taustaväri:"},
            {"Balanced (safer for short videos)", "Tasapainoinen (turvallisempi lyhyille videoille)"},
            {"Batch cancellation requested...", "Eräajon peruutusta pyydetty..."},
            {"Browse...", "Selaa..."},
            {"Cancel batch", "Peruuta erä"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Peruutusta pyydetty. Odotetaan nykyisen työn pysähtymistä..."},
            {"Choose count text color", "Valitse laskuritekstin väri"},
            {"Choose font...", "Valitse fontti..."},
            {"Choose info text color", "Valitse infoteksin väri"},
            {"Choose tile border color", "Valitse ruudun reunan väri"},
            {"Choose timestamp background color", "Valitse aikaleiman taustaväri"},
            {"Choose timestamp text color", "Valitse aikaleiman tekstiväri"},
            {"Choose title text color", "Valitse otsikkotekstin väri"},
            {"Choose...", "Valitse..."},
            {"Clear preview", "Tyhjennä esikatselu"},
            {"Columns:", "Sarakkeet:"},
            {"Config 1", "Asetus 1"},
            {"Config 2", "Asetus 2"},
            {"Config 3", "Asetus 3"},
            {"Contact sheet preview", "Kontaktilevyn esikatselu"},
            {"Count text color:", "Laskuritekstin väri:"},
            {"Delete Selection", "Poista valinta"},
            {"Draw tile border", "Piirrä ruudun reuna"},
            {"Edition", "Muokkaus"},
            {"End margin:", "Loppumarginaali:"},
            {"Exit", "Lopeta"},
            {"Fast (Thumbnail me 3.0 style)", "Nopea (Thumbnail me 3.0 -tyyli)"},
            {"File", "Tiedosto"},
            {"Font:", "Fontti:"},
            {"Gap:", "Väli:"},
            {"Generate a contact sheet first.", "Luo ensin kontaktilevy."},
            {"Info position:", "Infon sijainti:"},
            {"Info text color:", "Infotekstin väri:"},
            {"Info text:", "Tietoteksti:"},
            {"Input / Output", "Syöte / tuloste"},
            {"Input list cleared", "Syöteluettelo tyhjennetty"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Kieli vaihdettu. Tallenna asetukset ja käynnistä sovellus uudelleen, jotta muutos tulee kokonaan voimaan."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Kielivalinta tallennetaan heti settings.ini-tiedostoon. Käynnistä sovellus uudelleen, jotta staattiset tekstit päivittyvät kokonaan."},
            {"Language:", "Kieli:"},
            {"Load", "Lataa"},
            {"Load video files...", "Lataa videotiedostoja..."},
            {"Load videos and click Start selected or Start all.", "Lataa videot ja napsauta Käynnistä valitut tai Käynnistä kaikki."},
            {"Lower Left", "Alhaalla vasemmalla"},
            {"Lower Right", "Alhaalla oikealla"},
            {"No contact sheet to save", "Ei tallennettavaa kontaktilevyä"},
            {"Open Recent Configuration", "Avaa viimeisin kokoonpano"},
            {"Open configuration...", "Avaa kokoonpano..."},
            {"Output", "Tuloste"},
            {"Output Format:", "Tulostemuoto:"},
            {"Output filename suffix:", "Tulostetiedoston nimen pääte:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Tulostepolku:"},
            {"Predefined Configuration", "Esimääritetty kokoonpano"},
            {"Preset applied: %1 rows × %2 columns", "Esiasetus käytetty: %1 riviä × %2 saraketta"},
            {"Preview area", "Esikatselualue"},
            {"Preview style", "Esikatsele tyyli"},
            {"Quality:", "Laatu:"},
            {"Randomize amount:", "Satunnaisuuden määrä:"},
            {"Randomize positions", "Satunnaista sijainnit"},
            {"Randomize:", "Satunnaistus:"},
            {"Reload settings.ini", "Lataa settings.ini uudelleen"},
            {"Reset", "Palauta"},
            {"Rows:", "Rivit:"},
            {"Same folder as source video", "Sama kansio kuin lähdevideolla"},
            {"Same source folder:", "Sama lähdekansio:"},
            {"Save configuration", "Tallenna kokoonpano"},
            {"Save configuration as...", "Tallenna kokoonpano nimellä..."},
            {"Save contact sheet", "Tallenna kontaktilevy"},
            {"Save failed", "Tallennus epäonnistui"},
            {"Save manually", "Tallenna käsin"},
            {"Save selected contact sheet...", "Tallenna valittu kontaktilevy..."},
            {"Save settings.ini", "Tallenna settings.ini"},
            {"Saved: ", "Tallennettu: "},
            {"Seek mode:", "Hakutila:"},
            {"Select output folder", "Valitse tulostekansio"},
            {"Selected video file(s) removed", "Valitut videotiedostot poistettu"},
            {"Settings saved:\n", "Asetukset tallennettu:\n"},
            {"Show additional title", "Näytä lisäotsikko"},
            {"Show technical info text", "Näytä tekninen infoteksti"},
            {"Show timestamps", "Näytä aikaleimat"},
            {"Similarity threshold:", "Samankaltaisuuden kynnys:"},
            {"Skip dark / blank frames", "Ohita tummat / tyhjät ruudut"},
            {"Skip dark:", "Ohita tummat:"},
            {"Skip similar frames", "Ohita samankaltaiset ruudut"},
            {"Skip similar:", "Ohita samankaltaiset:"},
            {"Specific timestep:", "Määritetty aika-askel:"},
            {"Start all", "Käynnistä kaikki"},
            {"Start margin:", "Alkumarginaali:"},
            {"Start selected", "Käynnistä valitut"},
            {"Style preview failed", "Tyyliesikatselu epäonnistui"},
            {"Style preview ready", "Tyyliesikatselu valmis"},
            {"Styles", "Tyylit"},
            {"The file could not be saved.", "Tiedostoa ei voitu tallentaa."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Ruudun reunan väri:"},
            {"Tile border:", "Ruudun reuna:"},
            {"Timestamp background:", "Aikaleiman tausta:"},
            {"Timestamp position:", "Aikaleiman sijainti:"},
            {"Timestamp text color:", "Aikaleiman tekstiväri:"},
            {"Timestamp:", "Aikaleima:"},
            {"Timestep:", "Aika-askel:"},
            {"Title text color:", "Otsikkotekstin väri:"},
            {"Title text:", "Otsikkoteksti:"},
            {"Upper Left", "Ylhäällä vasemmalla"},
            {"Upper Right", "Ylhäällä oikealla"},
            {"Use specific timestep", "Käytä määritettyä aika-askelta"},
            {"View", "Näytä"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Leveys:"},
            {"English", "Englanti"},
            {"French", "Ranska"},
            {"German", "Saksa"},
            {"Spanish", "Espanja"},
            {"Dutch", "Hollanti"},
            {"Czech", "Tšekki"},
            {"Italian", "Italia"},
            {"Hungarian", "Unkari"},
            {"Croatian", "Kroatia"},
            {"Portuguese", "Portugali"},
            {"Chinese", "Kiina"},
            {"Russian", "Venäjä"}
        };

        static const QHash<QString, QString> sv = {
            {"%1 rows × %2 columns", "%1 rader × %2 kolumner"},
            {"About", "Om"},
            {"About Thumbnail me 4b2", "Om Thumbnail me 4b2"},
            {"Accurate (slowest)", "Exakt (långsammast)"},
            {"Add and select a video first.", "Lägg till och välj först en video."},
            {"Add video files", "Lägg till videofiler"},
            {"Add video files first.", "Lägg först till videofiler."},
            {"Added %1 video file(s)", "Lade till %1 videofil(er)"},
            {"Additional title:", "Ytterligare titel:"},
            {"Background color:", "Bakgrundsfärg:"},
            {"Balanced (safer for short videos)", "Balanserad (säkrare för korta videor)"},
            {"Batch cancellation requested...", "Batchavbrott begärt..."},
            {"Browse...", "Bläddra..."},
            {"Cancel batch", "Avbryt batch"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Avbrott begärt. Väntar på att den aktuella processen ska stoppas..."},
            {"Choose count text color", "Välj färg för räknartext"},
            {"Choose font...", "Välj teckensnitt..."},
            {"Choose info text color", "Välj färg för informationstext"},
            {"Choose tile border color", "Välj färg för bildrutans kant"},
            {"Choose timestamp background color", "Välj bakgrundsfärg för tidsstämpel"},
            {"Choose timestamp text color", "Välj textfärg för tidsstämpel"},
            {"Choose title text color", "Välj textfärg för titel"},
            {"Choose...", "Välj..."},
            {"Clear preview", "Rensa förhandsvisning"},
            {"Columns:", "Kolumner:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Förhandsvisning av kontaktark"},
            {"Count text color:", "Färg för räknartext:"},
            {"Delete Selection", "Ta bort markering"},
            {"Draw tile border", "Rita bildrutans kant"},
            {"Edition", "Redigering"},
            {"End margin:", "Slutmarginal:"},
            {"Exit", "Avsluta"},
            {"Fast (Thumbnail me 3.0 style)", "Snabb (Thumbnail me 3.0-stil)"},
            {"File", "Arkiv"},
            {"Font:", "Teckensnitt:"},
            {"Gap:", "Mellanrum:"},
            {"Generate a contact sheet first.", "Generera först ett kontaktark."},
            {"Info position:", "Informationsposition:"},
            {"Info text color:", "Färg för informationstext:"},
            {"Info text:", "Infotext:"},
            {"Input / Output", "Indata / utdata"},
            {"Input list cleared", "Indatalistan rensad"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Språket har ändrats. Spara inställningarna och starta om programmet för att tillämpa det helt."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Språkvalet sparas omedelbart i settings.ini. Starta om programmet för att tillämpa statiska etiketter helt."},
            {"Language:", "Språk:"},
            {"Load", "Läs in"},
            {"Load video files...", "Läs in videofiler..."},
            {"Load videos and click Start selected or Start all.", "Ladda videor och klicka på Starta markerade eller Starta alla."},
            {"Lower Left", "Nedre vänster"},
            {"Lower Right", "Nedre höger"},
            {"No contact sheet to save", "Inget kontaktark att spara"},
            {"Open Recent Configuration", "Öppna senaste konfiguration"},
            {"Open configuration...", "Öppna konfiguration..."},
            {"Output", "Utdata"},
            {"Output Format:", "Utdataformat:"},
            {"Output filename suffix:", "Suffix för utdatafilnamn:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Utdatasökväg:"},
            {"Predefined Configuration", "Fördefinierad konfiguration"},
            {"Preset applied: %1 rows × %2 columns", "Förval tillämpat: %1 rader × %2 kolumner"},
            {"Preview area", "Förhandsvisningsområde"},
            {"Preview style", "Förhandsvisa stil"},
            {"Quality:", "Kvalitet:"},
            {"Randomize amount:", "Slumpningsgrad:"},
            {"Randomize positions", "Slumpa positioner"},
            {"Randomize:", "Slumpning:"},
            {"Reload settings.ini", "Läs om settings.ini"},
            {"Reset", "Återställ"},
            {"Rows:", "Rader:"},
            {"Same folder as source video", "Samma mapp som källvideon"},
            {"Same source folder:", "Samma källmapp:"},
            {"Save configuration", "Spara konfiguration"},
            {"Save configuration as...", "Spara konfiguration som..."},
            {"Save contact sheet", "Spara kontaktark"},
            {"Save failed", "Sparandet misslyckades"},
            {"Save manually", "Spara manuellt"},
            {"Save selected contact sheet...", "Spara markerat kontaktark..."},
            {"Save settings.ini", "Spara settings.ini"},
            {"Saved: ", "Sparat: "},
            {"Seek mode:", "Sökläge:"},
            {"Select output folder", "Välj utdatamapp"},
            {"Selected video file(s) removed", "Markerade videofiler borttagna"},
            {"Settings saved:\n", "Inställningarna sparades:\n"},
            {"Show additional title", "Visa ytterligare titel"},
            {"Show technical info text", "Visa teknisk informationstext"},
            {"Show timestamps", "Visa tidsstämplar"},
            {"Similarity threshold:", "Likhetströskel:"},
            {"Skip dark / blank frames", "Hoppa över mörka / tomma bildrutor"},
            {"Skip dark:", "Hoppa över mörka:"},
            {"Skip similar frames", "Hoppa över liknande bildrutor"},
            {"Skip similar:", "Hoppa över liknande:"},
            {"Specific timestep:", "Specifikt tidssteg:"},
            {"Start all", "Starta alla"},
            {"Start margin:", "Startmarginal:"},
            {"Start selected", "Starta markerade"},
            {"Style preview failed", "Stilförhandsvisning misslyckades"},
            {"Style preview ready", "Stilförhandsvisning klar"},
            {"Styles", "Stilar"},
            {"The file could not be saved.", "Filen kunde inte sparas."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Färg på bildrutans kant:"},
            {"Tile border:", "Bildrutans kant:"},
            {"Timestamp background:", "Tidsstämpelbakgrund:"},
            {"Timestamp position:", "Tidsstämpelposition:"},
            {"Timestamp text color:", "Textfärg för tidsstämpel:"},
            {"Timestamp:", "Tidsstämpel:"},
            {"Timestep:", "Tidssteg:"},
            {"Title text color:", "Textfärg för titel:"},
            {"Title text:", "Titeltext:"},
            {"Upper Left", "Övre vänster"},
            {"Upper Right", "Övre höger"},
            {"Use specific timestep", "Använd specifikt tidssteg"},
            {"View", "Visa"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Bredd:"},
            {"English", "Engelska"},
            {"French", "Franska"},
            {"German", "Tyska"},
            {"Spanish", "Spanska"},
            {"Dutch", "Nederländska"},
            {"Czech", "Tjeckiska"},
            {"Italian", "Italienska"},
            {"Hungarian", "Ungerska"},
            {"Croatian", "Kroatiska"},
            {"Portuguese", "Portugisiska"},
            {"Chinese", "Kinesiska"},
            {"Russian", "Ryska"}
        };

                static const QHash<QString, QString> sq = {
            {"%1 rows × %2 columns", "%1 rreshta × %2 kolona"},
            {"About", "Rreth programit"},
            {"About Thumbnail me 4b2", "Rreth Thumbnail me 4b2"},
            {"Accurate (slowest)", "I saktë (më i ngadalti)"},
            {"Add and select a video first.", "Shto dhe zgjidh më parë një video."},
            {"Add video files", "Shto skedarë video"},
            {"Add video files first.", "Shto më parë skedarë video."},
            {"Added %1 video file(s)", "U shtuan %1 skedarë video"},
            {"Additional title:", "Titull shtesë:"},
            {"Background color:", "Ngjyra e sfondit:"},
            {"Balanced (safer for short videos)", "I balancuar (më i sigurt për video të shkurtra)"},
            {"Batch cancellation requested...", "U kërkua anulimi i grupit..."},
            {"Browse...", "Shfleto..."},
            {"Cancel batch", "Anulo grupin"},
            {"Cancellation requested. Waiting for the current worker to stop...", "U kërkua anulimi. Po pritet ndalimi i punëtorit aktual..."},
            {"Choose count text color", "Zgjidh ngjyrën e tekstit të numërimit"},
            {"Choose font...", "Zgjidh shkronjën..."},
            {"Choose info text color", "Zgjidh ngjyrën e tekstit informues"},
            {"Choose tile border color", "Zgjidh ngjyrën e kornizës së pllakës"},
            {"Choose timestamp background color", "Zgjidh ngjyrën e sfondit të vulës kohore"},
            {"Choose timestamp text color", "Zgjidh ngjyrën e tekstit të vulës kohore"},
            {"Choose title text color", "Zgjidh ngjyrën e tekstit të titullit"},
            {"Choose...", "Zgjidh..."},
            {"Clear preview", "Pastro pamjen paraprake"},
            {"Columns:", "Kolonat:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Pamje paraprake e fletës së miniaturave"},
            {"Count text color:", "Ngjyra e tekstit të numërimit:"},
            {"Delete Selection", "Fshi përzgjedhjen"},
            {"Draw tile border", "Vizato kornizën e pllakës"},
            {"Edition", "Redaktim"},
            {"End margin:", "Margjina e fundit:"},
            {"Exit", "Dil"},
            {"Fast (Thumbnail me 3.0 style)", "I shpejtë (stil Thumbnail me 3.0)"},
            {"File", "Skedar"},
            {"Font:", "Shkronja:"},
            {"Gap:", "Hapësira:"},
            {"Generate a contact sheet first.", "Gjenero më parë një fletë miniaturash."},
            {"Info position:", "Pozicioni i informacionit:"},
            {"Info text color:", "Ngjyra e tekstit informues:"},
            {"Info text:", "Tekst informues:"},
            {"Input / Output", "Hyrje / dalje"},
            {"Input list cleared", "Lista e hyrjeve u pastrua"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Gjuha u ndryshua. Ruaj cilësimet dhe rinis aplikacionin për ta zbatuar plotësisht."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Zgjedhja e gjuhës ruhet menjëherë në settings.ini. Rinis aplikacionin për të zbatuar plotësisht etiketat statike."},
            {"Language:", "Gjuha:"},
            {"Load", "Ngarko"},
            {"Load video files...", "Ngarko skedarë video..."},
            {"Load videos and click Start selected or Start all.", "Ngarko videot dhe kliko Nis të përzgjedhurat ose Nis të gjitha."},
            {"Lower Left", "Poshtë majtas"},
            {"Lower Right", "Poshtë djathtas"},
            {"No contact sheet to save", "Nuk ka fletë miniaturash për t’u ruajtur"},
            {"Open Recent Configuration", "Hap konfigurimin e fundit"},
            {"Open configuration...", "Hap konfigurimin..."},
            {"Output", "Dalje"},
            {"Output Format:", "Formati i daljes:"},
            {"Output filename suffix:", "Prapashtesa e emrit të daljes:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Shtegu i daljes:"},
            {"Predefined Configuration", "Konfigurim i paracaktuar"},
            {"Preset applied: %1 rows × %2 columns", "Paracaktimi u zbatua: %1 rreshta × %2 kolona"},
            {"Preview area", "Zona e pamjes paraprake"},
            {"Preview style", "Pamje paraprake e stilit"},
            {"Quality:", "Cilësia:"},
            {"Randomize amount:", "Sasia e rastësimit:"},
            {"Randomize positions", "Rastëso pozicionet"},
            {"Randomize:", "Rastëso:"},
            {"Reload settings.ini", "Ringarko settings.ini"},
            {"Reset", "Rivendos"},
            {"Rows:", "Rreshtat:"},
            {"Same folder as source video", "E njëjta dosje si video burimore"},
            {"Same source folder:", "E njëjta dosje burimore:"},
            {"Save configuration", "Ruaj konfigurimin"},
            {"Save configuration as...", "Ruaj konfigurimin si..."},
            {"Save contact sheet", "Ruaj fletën e miniaturave"},
            {"Save failed", "Ruajtja dështoi"},
            {"Save manually", "Ruaj manualisht"},
            {"Save selected contact sheet...", "Ruaj fletën e përzgjedhur të miniaturave..."},
            {"Save settings.ini", "Ruaj settings.ini"},
            {"Saved: ", "U ruajt: "},
            {"Seek mode:", "Modaliteti i kërkimit:"},
            {"Select output folder", "Zgjidh dosjen e daljes"},
            {"Selected video file(s) removed", "Skedarët e përzgjedhur video u hoqën"},
            {"Settings saved:\n", "Cilësimet u ruajtën:\n"},
            {"Show additional title", "Shfaq titull shtesë"},
            {"Show technical info text", "Shfaq tekstin teknik informues"},
            {"Show timestamps", "Shfaq vulat kohore"},
            {"Similarity threshold:", "Pragu i ngjashmërisë:"},
            {"Skip dark / blank frames", "Kapërce korniza të errëta / bosh"},
            {"Skip dark:", "Kapërce të errëtat:"},
            {"Skip similar frames", "Kapërce korniza të ngjashme"},
            {"Skip similar:", "Kapërce të ngjashmet:"},
            {"Specific timestep:", "Hap kohor i caktuar:"},
            {"Start all", "Nis të gjitha"},
            {"Start margin:", "Margjina e fillimit:"},
            {"Start selected", "Nis të përzgjedhurat"},
            {"Style preview failed", "Pamja paraprake e stilit dështoi"},
            {"Style preview ready", "Pamja paraprake e stilit është gati"},
            {"Styles", "Stilet"},
            {"The file could not be saved.", "Skedari nuk mund të ruhej."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Ngjyra e kornizës së pllakës:"},
            {"Tile border:", "Korniza e pllakës:"},
            {"Timestamp background:", "Sfondi i vulës kohore:"},
            {"Timestamp position:", "Pozicioni i vulës kohore:"},
            {"Timestamp text color:", "Ngjyra e tekstit të vulës kohore:"},
            {"Timestamp:", "Vula kohore:"},
            {"Timestep:", "Hapi kohor:"},
            {"Title text color:", "Ngjyra e tekstit të titullit:"},
            {"Title text:", "Teksti i titullit:"},
            {"Upper Left", "Lart majtas"},
            {"Upper Right", "Lart djathtas"},
            {"Use specific timestep", "Përdor hap kohor të caktuar"},
            {"View", "Pamje"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Gjerësia:"},
            {"English", "Anglisht"},
            {"French", "Frëngjisht"},
            {"German", "Gjermanisht"},
            {"Spanish", "Spanjisht"},
            {"Dutch", "Holandisht"},
            {"Czech", "Çekisht"},
            {"Italian", "Italisht"},
            {"Hungarian", "Hungarisht"},
            {"Croatian", "Kroatisht"},
            {"Portuguese", "Portugalisht"},
            {"Chinese", "Kinezisht"},
            {"Russian", "Rusisht"}
        };

                static const QHash<QString, QString> be = {
            {"%1 rows × %2 columns", "%1 радкоў × %2 слупкоў"},
            {"About", "Пра праграму"},
            {"About Thumbnail me 4b2", "Пра Thumbnail me 4b2"},
            {"Accurate (slowest)", "Дакладны (самы павольны)"},
            {"Add and select a video first.", "Спачатку дадайце і выберыце відэа."},
            {"Add video files", "Дадаць відэафайлы"},
            {"Add video files first.", "Спачатку дадайце відэафайлы."},
            {"Added %1 video file(s)", "Дададзена %1 відэафайлаў"},
            {"Additional title:", "Дадатковы загаловак:"},
            {"Background color:", "Колер фону:"},
            {"Balanced (safer for short videos)", "Збалансаваны (бяспечней для кароткіх відэа)"},
            {"Batch cancellation requested...", "Запытана скасаванне пакета..."},
            {"Browse...", "Агляд..."},
            {"Cancel batch", "Скасаваць пакет"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Скасаванне запытана. Чаканне спынення бягучага працэсу..."},
            {"Choose count text color", "Выбраць колер тэксту лічыльніка"},
            {"Choose font...", "Выбраць шрыфт..."},
            {"Choose info text color", "Выбраць колер інфармацыйнага тэксту"},
            {"Choose tile border color", "Выбраць колер рамкі мініяцюры"},
            {"Choose timestamp background color", "Выбраць колер фону часавай меткі"},
            {"Choose timestamp text color", "Выбраць колер тэксту часавай меткі"},
            {"Choose title text color", "Выбраць колер тэксту загалоўка"},
            {"Choose...", "Выбраць..."},
            {"Clear preview", "Ачысціць папярэдні прагляд"},
            {"Columns:", "Слупкі:"},
            {"Config 1", "Канфіг. 1"},
            {"Config 2", "Канфіг. 2"},
            {"Config 3", "Канфіг. 3"},
            {"Contact sheet preview", "Папярэдні прагляд ліста мініяцюр"},
            {"Count text color:", "Колер тэксту лічыльніка:"},
            {"Delete Selection", "Выдаліць выбранае"},
            {"Draw tile border", "Маляваць рамку мініяцюры"},
            {"Edition", "Рэдагаванне"},
            {"End margin:", "Канцавае поле:"},
            {"Exit", "Выйсці"},
            {"Fast (Thumbnail me 3.0 style)", "Хуткі (стыль Thumbnail me 3.0)"},
            {"File", "Файл"},
            {"Font:", "Шрыфт:"},
            {"Gap:", "Прамежак:"},
            {"Generate a contact sheet first.", "Спачатку стварыце ліст мініяцюр."},
            {"Info position:", "Пазіцыя інфармацыі:"},
            {"Info text color:", "Колер інфармацыйнага тэксту:"},
            {"Info text:", "Інфармацыйны тэкст:"},
            {"Input / Output", "Увод / вывад"},
            {"Input list cleared", "Спіс уводу ачышчаны"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Мова зменена. Захавайце налады і перазапусціце праграму, каб цалкам ужыць змены."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Выбар мовы адразу захоўваецца ў settings.ini. Перазапусціце праграму, каб цалкам ужыць статычныя надпісы."},
            {"Language:", "Мова:"},
            {"Load", "Загрузіць"},
            {"Load video files...", "Загрузіць відэафайлы..."},
            {"Load videos and click Start selected or Start all.", "Загрузіце відэа і націсніце Запусціць выбранае або Запусціць усё."},
            {"Lower Left", "Унізе злева"},
            {"Lower Right", "Унізе справа"},
            {"No contact sheet to save", "Няма ліста мініяцюр для захавання"},
            {"Open Recent Configuration", "Адкрыць нядаўнюю канфігурацыю"},
            {"Open configuration...", "Адкрыць канфігурацыю..."},
            {"Output", "Вывад"},
            {"Output Format:", "Фармат вываду:"},
            {"Output filename suffix:", "Суфікс імя вываднога файла:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Шлях вываду:"},
            {"Predefined Configuration", "Папярэдне зададзеная канфігурацыя"},
            {"Preset applied: %1 rows × %2 columns", "Профіль ужыты: %1 радкоў × %2 слупкоў"},
            {"Preview area", "Вобласць папярэдняга прагляду"},
            {"Preview style", "Папярэдні прагляд стылю"},
            {"Quality:", "Якасць:"},
            {"Randomize amount:", "Ступень выпадковасці:"},
            {"Randomize positions", "Выпадкова змяняць пазіцыі"},
            {"Randomize:", "Выпадкова:"},
            {"Reload settings.ini", "Перазагрузіць settings.ini"},
            {"Reset", "Скінуць"},
            {"Rows:", "Радкі:"},
            {"Same folder as source video", "Тая ж папка, што і зыходнае відэа"},
            {"Same source folder:", "Тая ж зыходная папка:"},
            {"Save configuration", "Захаваць канфігурацыю"},
            {"Save configuration as...", "Захаваць канфігурацыю як..."},
            {"Save contact sheet", "Захаваць ліст мініяцюр"},
            {"Save failed", "Не ўдалося захаваць"},
            {"Save manually", "Захаваць уручную"},
            {"Save selected contact sheet...", "Захаваць выбраны ліст мініяцюр..."},
            {"Save settings.ini", "Захаваць settings.ini"},
            {"Saved: ", "Захавана: "},
            {"Seek mode:", "Рэжым пошуку:"},
            {"Select output folder", "Выбраць папку вываду"},
            {"Selected video file(s) removed", "Выбраныя відэафайлы выдалены"},
            {"Settings saved:\n", "Налады захаваны:\n"},
            {"Show additional title", "Паказваць дадатковы загаловак"},
            {"Show technical info text", "Паказваць тэхнічны інфармацыйны тэкст"},
            {"Show timestamps", "Паказваць часавыя меткі"},
            {"Similarity threshold:", "Парог падабенства:"},
            {"Skip dark / blank frames", "Прапускаць цёмныя / пустыя кадры"},
            {"Skip dark:", "Прапускаць цёмныя:"},
            {"Skip similar frames", "Прапускаць падобныя кадры"},
            {"Skip similar:", "Прапускаць падобныя:"},
            {"Specific timestep:", "Пэўны крок часу:"},
            {"Start all", "Запусціць усё"},
            {"Start margin:", "Пачатковае поле:"},
            {"Start selected", "Запусціць выбранае"},
            {"Style preview failed", "Папярэдні прагляд стылю не ўдаўся"},
            {"Style preview ready", "Папярэдні прагляд стылю гатовы"},
            {"Styles", "Стылі"},
            {"The file could not be saved.", "Файл не ўдалося захаваць."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Колер рамкі мініяцюры:"},
            {"Tile border:", "Рамка мініяцюры:"},
            {"Timestamp background:", "Фон часавай меткі:"},
            {"Timestamp position:", "Пазіцыя часавай меткі:"},
            {"Timestamp text color:", "Колер тэксту часавай меткі:"},
            {"Timestamp:", "Часавая метка:"},
            {"Timestep:", "Крок часу:"},
            {"Title text color:", "Колер тэксту загалоўка:"},
            {"Title text:", "Тэкст загалоўка:"},
            {"Upper Left", "Уверсе злева"},
            {"Upper Right", "Уверсе справа"},
            {"Use specific timestep", "Выкарыстоўваць пэўны крок часу"},
            {"View", "Выгляд"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Шырыня:"},
            {"English", "Англійская"},
            {"French", "Французская"},
            {"German", "Нямецкая"},
            {"Spanish", "Іспанская"},
            {"Dutch", "Нідэрландская"},
            {"Czech", "Чэшская"},
            {"Italian", "Італьянская"},
            {"Hungarian", "Венгерская"},
            {"Croatian", "Харвацкая"},
            {"Portuguese", "Партугальская"},
            {"Chinese", "Кітайская"},
            {"Russian", "Руская"}
        };

                static const QHash<QString, QString> bs = {
            {"%1 rows × %2 columns", "%1 redaka × %2 stupca"},
            {"About", "O programu"},
            {"About Thumbnail me 4b2", "O Thumbnail me 4b2"},
            {"Accurate (slowest)", "Precizno (najsporije)"},
            {"Add and select a video first.", "Najprije dodajte i odaberite video."},
            {"Add video files", "Dodaj video datoteke"},
            {"Add video files first.", "Prvo dodaj video datoteke."},
            {"Added %1 video file(s)", "Dodano %1 video datoteka"},
            {"Additional title:", "Dodatni naslov:"},
            {"Background color:", "Boja pozadine:"},
            {"Balanced (safer for short videos)", "Uravnoteženo (sigurnije za kratke videozapise)"},
            {"Batch cancellation requested...", "Zatraženo otkazivanje serije..."},
            {"Browse...", "Pregledaj..."},
            {"Cancel batch", "Otkaži seriju"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Otkazivanje zatraženo. Čekam zaustavljanje trenutnog workera..."},
            {"Choose count text color", "Odaberi boju teksta brojača"},
            {"Choose font...", "Odaberi font..."},
            {"Choose info text color", "Odaberi boju info teksta"},
            {"Choose tile border color", "Odaberi boju okvira sličice"},
            {"Choose timestamp background color", "Odaberi boju pozadine vremenske oznake"},
            {"Choose timestamp text color", "Odaberi boju vremenske oznake"},
            {"Choose title text color", "Odaberi boju naslova"},
            {"Choose...", "Izaberi..."},
            {"Clear preview", "Očisti pregled"},
            {"Columns:", "Kolone:"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Contact sheet preview", "Pregled kontaktne liste"},
            {"Count text color:", "Boja teksta brojača:"},
            {"Delete Selection", "Obriši izbor"},
            {"Draw tile border", "Crtaj okvir sličice"},
            {"Edition", "Uređivanje"},
            {"End margin:", "Završna margina:"},
            {"Exit", "Izlaz"},
            {"Fast (Thumbnail me 3.0 style)", "Brzo (stil Thumbnail me 3.0)"},
            {"File", "Datoteka"},
            {"Font:", "Font:"},
            {"Gap:", "Razmak:"},
            {"Generate a contact sheet first.", "Najprije generirajte kontaktnu ploču."},
            {"Info position:", "Položaj info teksta:"},
            {"Info text color:", "Boja info teksta:"},
            {"Info text:", "Info tekst:"},
            {"Input / Output", "Ulaz / izlaz"},
            {"Input list cleared", "Popis ulaza očišćen"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jezik je promijenjen. Spremite postavke i ponovno pokrenite aplikaciju za potpunu primjenu."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Odabir jezika odmah se sprema u settings.ini. Ponovno pokrenite aplikaciju za potpunu primjenu statičkih oznaka."},
            {"Language:", "Jezik:"},
            {"Load", "Učitaj"},
            {"Load video files...", "Učitaj video datoteke..."},
            {"Load videos and click Start selected or Start all.", "Učitajte videozapise i kliknite Pokreni odabrano ili Pokreni sve."},
            {"Lower Left", "Dolje lijevo"},
            {"Lower Right", "Dolje desno"},
            {"No contact sheet to save", "Nema kontaktne ploče za spremanje"},
            {"Open Recent Configuration", "Otvori nedavnu konfiguraciju"},
            {"Open configuration...", "Otvori konfiguraciju..."},
            {"Output", "Izlaz"},
            {"Output Format:", "Izlazni format:"},
            {"Output filename suffix:", "Sufiks izlaznog naziva datoteke:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Izlazna putanja:"},
            {"Predefined Configuration", "Predefinirana konfiguracija"},
            {"Preset applied: %1 rows × %2 columns", "Predložak primijenjen: %1 redaka × %2 stupca"},
            {"Preview area", "Područje pregleda"},
            {"Preview style", "Pregled stila"},
            {"Quality:", "Kvalitet:"},
            {"Randomize amount:", "Količina slučajnosti:"},
            {"Randomize positions", "Nasumične pozicije"},
            {"Randomize:", "Nasumično:"},
            {"Reload settings.ini", "Ponovno učitaj settings.ini"},
            {"Reset", "Poništi"},
            {"Rows:", "Redovi:"},
            {"Same folder as source video", "Ista mapa kao izvorni video"},
            {"Same source folder:", "Ista izvorna mapa:"},
            {"Save configuration", "Spremi konfiguraciju"},
            {"Save configuration as...", "Spremi konfiguraciju kao..."},
            {"Save contact sheet", "Spremi kontaktnu ploču"},
            {"Save failed", "Spremanje nije uspjelo"},
            {"Save manually", "Spremi ručno"},
            {"Save selected contact sheet...", "Spremi odabranu kontaktnu ploču..."},
            {"Save settings.ini", "Spremi settings.ini"},
            {"Saved: ", "Spremljeno: "},
            {"Seek mode:", "Način traženja:"},
            {"Select output folder", "Odaberi izlaznu mapu"},
            {"Selected video file(s) removed", "Odabrane video datoteke uklonjene"},
            {"Settings saved:\n", "Postavke su spremljene:\n"},
            {"Show additional title", "Prikaži dodatni naslov"},
            {"Show technical info text", "Prikaži tehnički info tekst"},
            {"Show timestamps", "Prikaži vremenske oznake"},
            {"Similarity threshold:", "Prag sličnosti:"},
            {"Skip dark / blank frames", "Preskoči tamne / prazne kadrove"},
            {"Skip dark:", "Preskoči tamne:"},
            {"Skip similar frames", "Preskoči slične kadrove"},
            {"Skip similar:", "Preskoči slične:"},
            {"Specific timestep:", "Fiksni vremenski korak:"},
            {"Start all", "Pokreni sve"},
            {"Start margin:", "Početna margina:"},
            {"Start selected", "Pokreni odabrano"},
            {"Style preview failed", "Pregled stila nije uspio"},
            {"Style preview ready", "Pregled stila spreman"},
            {"Styles", "Stilovi"},
            {"The file could not be saved.", "Datoteka se nije mogla spremiti."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Boja okvira sličice:"},
            {"Tile border:", "Okvir sličice:"},
            {"Timestamp background:", "Pozadina vremenske oznake:"},
            {"Timestamp position:", "Položaj vremenske oznake:"},
            {"Timestamp text color:", "Boja vremenske oznake:"},
            {"Timestamp:", "Vremenska oznaka:"},
            {"Timestep:", "Vremenski korak:"},
            {"Title text color:", "Boja naslova:"},
            {"Title text:", "Tekst naslova:"},
            {"Upper Left", "Gore lijevo"},
            {"Upper Right", "Gore desno"},
            {"Use specific timestep", "Koristi fiksni vremenski korak"},
            {"View", "Prikaz"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Širina:"},
            {"English", "Engleski"},
            {"French", "Francuski"},
            {"German", "Njemački"},
            {"Spanish", "Španski"},
            {"Dutch", "Nizozemski"},
            {"Czech", "Češki"},
            {"Italian", "Italijanski"},
            {"Hungarian", "Mađarski"},
            {"Croatian", "Hrvatski"},
            {"Portuguese", "Portugalski"},
            {"Chinese", "Kineski"},
            {"Russian", "Ruski"}
        };

                static const QHash<QString, QString> sr = {
            {"%1 rows × %2 columns", "%1 редова × %2 колоне"},
            {"About", "О програму"},
            {"About Thumbnail me 4b2", "О Thumbnail me 4b2"},
            {"Accurate (slowest)", "Прецизно (најспорије)"},
            {"Add and select a video first.", "Прво додај и изабери видео."},
            {"Add video files", "Додај видео датотеке"},
            {"Add video files first.", "Прво додај видео датотеке."},
            {"Added %1 video file(s)", "Додато је %1 видео датотека"},
            {"Additional title:", "Додатни наслов:"},
            {"Background color:", "Боја позадине:"},
            {"Balanced (safer for short videos)", "Уравнотежено (безбедније за кратке видео записе)"},
            {"Batch cancellation requested...", "Затражено је отказивање пакета..."},
            {"Browse...", "Прегледај..."},
            {"Cancel batch", "Откажи пакет"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Затражено је отказивање. Чека се заустављање тренутног процеса..."},
            {"Choose count text color", "Изабери боју текста бројача"},
            {"Choose font...", "Изабери фонт..."},
            {"Choose info text color", "Изабери боју инфо текста"},
            {"Choose tile border color", "Изабери боју оквира плочице"},
            {"Choose timestamp background color", "Изабери боју позадине временске ознаке"},
            {"Choose timestamp text color", "Изабери боју текста временске ознаке"},
            {"Choose title text color", "Изабери боју текста наслова"},
            {"Choose...", "Изабери..."},
            {"Clear preview", "Очисти преглед"},
            {"Columns:", "Колоне:"},
            {"Config 1", "Конфиг. 1"},
            {"Config 2", "Конфиг. 2"},
            {"Config 3", "Конфиг. 3"},
            {"Contact sheet preview", "Преглед контакт листе"},
            {"Count text color:", "Боја текста бројача:"},
            {"Delete Selection", "Обриши избор"},
            {"Draw tile border", "Исцртај оквир плочице"},
            {"Edition", "Уређивање"},
            {"End margin:", "Крајња маргина:"},
            {"Exit", "Излаз"},
            {"Fast (Thumbnail me 3.0 style)", "Брзо (стил Thumbnail me 3.0)"},
            {"File", "Датотека"},
            {"Font:", "Фонт:"},
            {"Gap:", "Размак:"},
            {"Generate a contact sheet first.", "Прво генериши контакт листу."},
            {"Info position:", "Положај информација:"},
            {"Info text color:", "Боја инфо текста:"},
            {"Info text:", "Инфо текст:"},
            {"Input / Output", "Улаз / излаз"},
            {"Input list cleared", "Листа улаза је очишћена"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Језик је промењен. Сачувај подешавања и поново покрени апликацију да би се промена у потпуности применила."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Избор језика се одмах чува у settings.ini. Поново покрени апликацију да би се статичке ознаке у потпуности примениле."},
            {"Language:", "Језик:"},
            {"Load", "Учитај"},
            {"Load video files...", "Учитај видео датотеке..."},
            {"Load videos and click Start selected or Start all.", "Учитај видео записе и кликни Покрени изабрано или Покрени све."},
            {"Lower Left", "Доле лево"},
            {"Lower Right", "Доле десно"},
            {"No contact sheet to save", "Нема контакт листе за чување"},
            {"Open Recent Configuration", "Отвори недавну конфигурацију"},
            {"Open configuration...", "Отвори конфигурацију..."},
            {"Output", "Излаз"},
            {"Output Format:", "Излазни формат:"},
            {"Output filename suffix:", "Суфикс имена излазне датотеке:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Излазна путања:"},
            {"Predefined Configuration", "Унапред дефинисана конфигурација"},
            {"Preset applied: %1 rows × %2 columns", "Поставка примењена: %1 редова × %2 колоне"},
            {"Preview area", "Област прегледа"},
            {"Preview style", "Преглед стила"},
            {"Quality:", "Квалитет:"},
            {"Randomize amount:", "Количина насумичности:"},
            {"Randomize positions", "Насумично помери позиције"},
            {"Randomize:", "Насумично:"},
            {"Reload settings.ini", "Поново учитај settings.ini"},
            {"Reset", "Ресетуј"},
            {"Rows:", "Редови:"},
            {"Same folder as source video", "Иста фасцикла као изворни видео"},
            {"Same source folder:", "Иста изворна фасцикла:"},
            {"Save configuration", "Сачувај конфигурацију"},
            {"Save configuration as...", "Сачувај конфигурацију као..."},
            {"Save contact sheet", "Сачувај контакт листу"},
            {"Save failed", "Чување није успело"},
            {"Save manually", "Сачувај ручно"},
            {"Save selected contact sheet...", "Сачувај изабрану контакт листу..."},
            {"Save settings.ini", "Сачувај settings.ini"},
            {"Saved: ", "Сачувано: "},
            {"Seek mode:", "Режим премотавања:"},
            {"Select output folder", "Изабери излазну фасциклу"},
            {"Selected video file(s) removed", "Изабране видео датотеке су уклоњене"},
            {"Settings saved:\n", "Подешавања су сачувана:\n"},
            {"Show additional title", "Прикажи додатни наслов"},
            {"Show technical info text", "Прикажи технички инфо текст"},
            {"Show timestamps", "Прикажи временске ознаке"},
            {"Similarity threshold:", "Праг сличности:"},
            {"Skip dark / blank frames", "Прескочи тамне / празне кадрове"},
            {"Skip dark:", "Прескочи тамне:"},
            {"Skip similar frames", "Прескочи сличне кадрове"},
            {"Skip similar:", "Прескочи сличне:"},
            {"Specific timestep:", "Одређени временски корак:"},
            {"Start all", "Покрени све"},
            {"Start margin:", "Почетна маргина:"},
            {"Start selected", "Покрени изабрано"},
            {"Style preview failed", "Преглед стила није успео"},
            {"Style preview ready", "Преглед стила је спреман"},
            {"Styles", "Стилови"},
            {"The file could not be saved.", "Датотека није могла бити сачувана."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Боја оквира плочице:"},
            {"Tile border:", "Оквир плочице:"},
            {"Timestamp background:", "Позадина временске ознаке:"},
            {"Timestamp position:", "Положај временске ознаке:"},
            {"Timestamp text color:", "Боја текста временске ознаке:"},
            {"Timestamp:", "Временска ознака:"},
            {"Timestep:", "Временски корак:"},
            {"Title text color:", "Боја текста наслова:"},
            {"Title text:", "Текст наслова:"},
            {"Upper Left", "Горе лево"},
            {"Upper Right", "Горе десно"},
            {"Use specific timestep", "Користи одређени временски корак"},
            {"View", "Приказ"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Ширина:"},
            {"English", "Енглески"},
            {"French", "Француски"},
            {"German", "Немачки"},
            {"Spanish", "Шпански"},
            {"Dutch", "Холандски"},
            {"Czech", "Чешки"},
            {"Italian", "Италијански"},
            {"Hungarian", "Мађарски"},
            {"Croatian", "Хрватски"},
            {"Portuguese", "Португалски"},
            {"Chinese", "Кинески"},
            {"Russian", "Руски"}
        };

                static const QHash<QString, QString> mk = {
            {"%1 rows × %2 columns", "%1 редови × %2 колони"},
            {"About", "За програмата"},
            {"About Thumbnail me 4b2", "За Thumbnail me 4b2"},
            {"Accurate (slowest)", "Точен (најбавен)"},
            {"Add and select a video first.", "Прво додај и избери видео."},
            {"Add video files", "Додај видео датотеки"},
            {"Add video files first.", "Прво додај видео датотеки."},
            {"Added %1 video file(s)", "Додадени се %1 видео датотеки"},
            {"Additional title:", "Дополнителен наслов:"},
            {"Background color:", "Боја на позадина:"},
            {"Balanced (safer for short videos)", "Балансиран (побезбеден за кратки видеа)"},
            {"Batch cancellation requested...", "Побарано е откажување на серијата..."},
            {"Browse...", "Прелистај..."},
            {"Cancel batch", "Откажи серија"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Побарано е откажување. Се чека тековниот процес да запре..."},
            {"Choose count text color", "Избери боја на текстот за бројачот"},
            {"Choose font...", "Избери фонт..."},
            {"Choose info text color", "Избери боја на инфо текстот"},
            {"Choose tile border color", "Избери боја на рамката на сликичката"},
            {"Choose timestamp background color", "Избери боја на позадината на временската ознака"},
            {"Choose timestamp text color", "Избери боја на текстот на временската ознака"},
            {"Choose title text color", "Избери боја на текстот на насловот"},
            {"Choose...", "Избери..."},
            {"Clear preview", "Исчисти преглед"},
            {"Columns:", "Колони:"},
            {"Config 1", "Конфиг. 1"},
            {"Config 2", "Конфиг. 2"},
            {"Config 3", "Конфиг. 3"},
            {"Contact sheet preview", "Преглед на лист со сликички"},
            {"Count text color:", "Боја на текстот за бројачот:"},
            {"Delete Selection", "Избриши избор"},
            {"Draw tile border", "Исцртај рамка на сликичката"},
            {"Edition", "Уредување"},
            {"End margin:", "Крајна маргина:"},
            {"Exit", "Излез"},
            {"Fast (Thumbnail me 3.0 style)", "Брз (стил Thumbnail me 3.0)"},
            {"File", "Датотека"},
            {"Font:", "Фонт:"},
            {"Gap:", "Растојание:"},
            {"Generate a contact sheet first.", "Прво генерирај лист со сликички."},
            {"Info position:", "Позиција на информациите:"},
            {"Info text color:", "Боја на инфо текстот:"},
            {"Info text:", "Инфо текст:"},
            {"Input / Output", "Влез / излез"},
            {"Input list cleared", "Листата на влезови е исчистена"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Јазикот е променет. Зачувај ги поставките и рестартирај ја апликацијата за целосно да се примени."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Изборот на јазик веднаш се зачувува во settings.ini. Рестартирај ја апликацијата за целосно да се применат статичките ознаки."},
            {"Language:", "Јазик:"},
            {"Load", "Вчитај"},
            {"Load video files...", "Вчитај видео датотеки..."},
            {"Load videos and click Start selected or Start all.", "Вчитај видеа и кликни Стартувај избрано или Стартувај ги сите."},
            {"Lower Left", "Долу лево"},
            {"Lower Right", "Долу десно"},
            {"No contact sheet to save", "Нема лист со сликички за зачувување"},
            {"Open Recent Configuration", "Отвори неодамнешна конфигурација"},
            {"Open configuration...", "Отвори конфигурација..."},
            {"Output", "Излез"},
            {"Output Format:", "Излезен формат:"},
            {"Output filename suffix:", "Наставка на името на излезната датотека:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Излезна патека:"},
            {"Predefined Configuration", "Предефинирана конфигурација"},
            {"Preset applied: %1 rows × %2 columns", "Поставката е применета: %1 редови × %2 колони"},
            {"Preview area", "Област за преглед"},
            {"Preview style", "Преглед на стил"},
            {"Quality:", "Квалитет:"},
            {"Randomize amount:", "Степен на случајност:"},
            {"Randomize positions", "Случајно помести позиции"},
            {"Randomize:", "Случајно:"},
            {"Reload settings.ini", "Повторно вчитај settings.ini"},
            {"Reset", "Ресетирај"},
            {"Rows:", "Редови:"},
            {"Same folder as source video", "Иста папка како изворното видео"},
            {"Same source folder:", "Иста изворна папка:"},
            {"Save configuration", "Зачувај конфигурација"},
            {"Save configuration as...", "Зачувај конфигурација како..."},
            {"Save contact sheet", "Зачувај лист со сликички"},
            {"Save failed", "Зачувувањето не успеа"},
            {"Save manually", "Зачувај рачно"},
            {"Save selected contact sheet...", "Зачувај избран лист со сликички..."},
            {"Save settings.ini", "Зачувај settings.ini"},
            {"Saved: ", "Зачувано: "},
            {"Seek mode:", "Режим на барање:"},
            {"Select output folder", "Избери излезна папка"},
            {"Selected video file(s) removed", "Избраните видео датотеки се отстранети"},
            {"Settings saved:\n", "Поставките се зачувани:\n"},
            {"Show additional title", "Прикажи дополнителен наслов"},
            {"Show technical info text", "Прикажи технички инфо текст"},
            {"Show timestamps", "Прикажи временски ознаки"},
            {"Similarity threshold:", "Праг на сличност:"},
            {"Skip dark / blank frames", "Прескокни темни / празни кадри"},
            {"Skip dark:", "Прескокни темни:"},
            {"Skip similar frames", "Прескокни слични кадри"},
            {"Skip similar:", "Прескокни слични:"},
            {"Specific timestep:", "Одреден временски чекор:"},
            {"Start all", "Стартувај ги сите"},
            {"Start margin:", "Почетна маргина:"},
            {"Start selected", "Стартувај избрано"},
            {"Style preview failed", "Прегледот на стил не успеа"},
            {"Style preview ready", "Прегледот на стил е подготвен"},
            {"Styles", "Стилови"},
            {"The file could not be saved.", "Датотеката не можеше да се зачува."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Боја на рамката на сликичката:"},
            {"Tile border:", "Рамка на сликичката:"},
            {"Timestamp background:", "Позадина на временската ознака:"},
            {"Timestamp position:", "Позиција на временската ознака:"},
            {"Timestamp text color:", "Боја на текстот на временската ознака:"},
            {"Timestamp:", "Временска ознака:"},
            {"Timestep:", "Временски чекор:"},
            {"Title text color:", "Боја на текстот на насловот:"},
            {"Title text:", "Текст на насловот:"},
            {"Upper Left", "Горе лево"},
            {"Upper Right", "Горе десно"},
            {"Use specific timestep", "Користи одреден временски чекор"},
            {"View", "Приказ"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Ширина:"},
            {"English", "Англиски"},
            {"French", "Француски"},
            {"German", "Германски"},
            {"Spanish", "Шпански"},
            {"Dutch", "Холандски"},
            {"Czech", "Чешки"},
            {"Italian", "Италијански"},
            {"Hungarian", "Унгарски"},
            {"Croatian", "Хрватски"},
            {"Portuguese", "Португалски"},
            {"Chinese", "Кинески"},
            {"Russian", "Руски"}
        };

                static const QHash<QString, QString> cnr = {
            {"%1 rows × %2 columns", "%1 redaka × %2 stupca"},
            {"About", "O programu"},
            {"About Thumbnail me 4b2", "O Thumbnail me 4b2"},
            {"Accurate (slowest)", "Precizno (najsporije)"},
            {"Add and select a video first.", "Najprije dodajte i odaberite video."},
            {"Add video files", "Dodaj video fajlove"},
            {"Add video files first.", "Prvo dodaj video fajlove."},
            {"Added %1 video file(s)", "Dodano %1 video datoteka"},
            {"Additional title:", "Dodatni naslov:"},
            {"Background color:", "Boja pozadine:"},
            {"Balanced (safer for short videos)", "Uravnoteženo (sigurnije za kratke videozapise)"},
            {"Batch cancellation requested...", "Zatraženo otkazivanje serije..."},
            {"Browse...", "Pregledaj..."},
            {"Cancel batch", "Otkaži seriju"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Otkazivanje zatraženo. Čekam zaustavljanje trenutnog workera..."},
            {"Choose count text color", "Odaberi boju teksta brojača"},
            {"Choose font...", "Odaberi font..."},
            {"Choose info text color", "Odaberi boju info teksta"},
            {"Choose tile border color", "Odaberi boju okvira sličice"},
            {"Choose timestamp background color", "Odaberi boju pozadine vremenske oznake"},
            {"Choose timestamp text color", "Odaberi boju vremenske oznake"},
            {"Choose title text color", "Odaberi boju naslova"},
            {"Choose...", "Izaberi..."},
            {"Clear preview", "Očisti pregled"},
            {"Columns:", "Kolone:"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Contact sheet preview", "Pregled kontakt liste"},
            {"Count text color:", "Boja teksta brojača:"},
            {"Delete Selection", "Obriši izbor"},
            {"Draw tile border", "Crtaj okvir sličice"},
            {"Edition", "Uređivanje"},
            {"End margin:", "Završna margina:"},
            {"Exit", "Izlaz"},
            {"Fast (Thumbnail me 3.0 style)", "Brzo (stil Thumbnail me 3.0)"},
            {"File", "Fajl"},
            {"Font:", "Font:"},
            {"Gap:", "Razmak:"},
            {"Generate a contact sheet first.", "Najprije generirajte kontaktnu ploču."},
            {"Info position:", "Položaj info teksta:"},
            {"Info text color:", "Boja info teksta:"},
            {"Info text:", "Info tekst:"},
            {"Input / Output", "Ulaz / izlaz"},
            {"Input list cleared", "Popis ulaza očišćen"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jezik je promijenjen. Spremite postavke i ponovno pokrenite aplikaciju za potpunu primjenu."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Odabir jezika odmah se sprema u settings.ini. Ponovno pokrenite aplikaciju za potpunu primjenu statičkih oznaka."},
            {"Language:", "Jezik:"},
            {"Load", "Učitaj"},
            {"Load video files...", "Učitaj video fajlove..."},
            {"Load videos and click Start selected or Start all.", "Učitajte videozapise i kliknite Pokreni odabrano ili Pokreni sve."},
            {"Lower Left", "Dolje lijevo"},
            {"Lower Right", "Dolje desno"},
            {"No contact sheet to save", "Nema kontaktne ploče za spremanje"},
            {"Open Recent Configuration", "Otvori nedavnu konfiguraciju"},
            {"Open configuration...", "Otvori konfiguraciju..."},
            {"Output", "Izlaz"},
            {"Output Format:", "Izlazni format:"},
            {"Output filename suffix:", "Sufiks izlaznog naziva datoteke:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Izlazna putanja:"},
            {"Predefined Configuration", "Predefinirana konfiguracija"},
            {"Preset applied: %1 rows × %2 columns", "Predložak primijenjen: %1 redaka × %2 stupca"},
            {"Preview area", "Područje pregleda"},
            {"Preview style", "Pregled stila"},
            {"Quality:", "Kvalitet:"},
            {"Randomize amount:", "Količina slučajnosti:"},
            {"Randomize positions", "Nasumične pozicije"},
            {"Randomize:", "Nasumično:"},
            {"Reload settings.ini", "Ponovno učitaj settings.ini"},
            {"Reset", "Resetuj"},
            {"Rows:", "Redovi:"},
            {"Same folder as source video", "Ista mapa kao izvorni video"},
            {"Same source folder:", "Ista izvorna mapa:"},
            {"Save configuration", "Spremi konfiguraciju"},
            {"Save configuration as...", "Spremi konfiguraciju kao..."},
            {"Save contact sheet", "Spremi kontaktnu ploču"},
            {"Save failed", "Čuvanje nije uspjelo"},
            {"Save manually", "Sačuvaj ručno"},
            {"Save selected contact sheet...", "Spremi odabranu kontaktnu ploču..."},
            {"Save settings.ini", "Spremi settings.ini"},
            {"Saved: ", "Sačuvano: "},
            {"Seek mode:", "Način traženja:"},
            {"Select output folder", "Odaberi izlaznu mapu"},
            {"Selected video file(s) removed", "Odabrane video datoteke uklonjene"},
            {"Settings saved:\n", "Postavke su spremljene:\n"},
            {"Show additional title", "Prikaži dodatni naslov"},
            {"Show technical info text", "Prikaži tehnički info tekst"},
            {"Show timestamps", "Prikaži vremenske oznake"},
            {"Similarity threshold:", "Prag sličnosti:"},
            {"Skip dark / blank frames", "Preskoči tamne / prazne kadrove"},
            {"Skip dark:", "Preskoči tamne:"},
            {"Skip similar frames", "Preskoči slične kadrove"},
            {"Skip similar:", "Preskoči slične:"},
            {"Specific timestep:", "Fiksni vremenski korak:"},
            {"Start all", "Pokreni sve"},
            {"Start margin:", "Početna margina:"},
            {"Start selected", "Pokreni izabrano"},
            {"Style preview failed", "Pregled stila nije uspio"},
            {"Style preview ready", "Pregled stila spreman"},
            {"Styles", "Stilovi"},
            {"The file could not be saved.", "Datoteka se nije mogla spremiti."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Boja okvira sličice:"},
            {"Tile border:", "Okvir sličice:"},
            {"Timestamp background:", "Pozadina vremenske oznake:"},
            {"Timestamp position:", "Položaj vremenske oznake:"},
            {"Timestamp text color:", "Boja vremenske oznake:"},
            {"Timestamp:", "Vremenska oznaka:"},
            {"Timestep:", "Vremenski korak:"},
            {"Title text color:", "Boja naslova:"},
            {"Title text:", "Tekst naslova:"},
            {"Upper Left", "Gore lijevo"},
            {"Upper Right", "Gore desno"},
            {"Use specific timestep", "Koristi fiksni vremenski korak"},
            {"View", "Prikaz"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Širina:"},
            {"English", "Engleski"},
            {"French", "Francuski"},
            {"German", "Njemački"},
            {"Spanish", "Španski"},
            {"Dutch", "Holandski"},
            {"Czech", "Češki"},
            {"Italian", "Italijanski"},
            {"Hungarian", "Mađarski"},
            {"Croatian", "Hrvatski"},
            {"Portuguese", "Portugalski"},
            {"Chinese", "Kineski"},
            {"Russian", "Ruski"}
        };

                static const QHash<QString, QString> isl = {
            {"%1 rows × %2 columns", "%1 raðir × %2 dálkar"},
            {"About", "Um forritið"},
            {"About Thumbnail me 4b2", "Um Thumbnail me 4b2"},
            {"Accurate (slowest)", "Nákvæmt (hægast)"},
            {"Add and select a video first.", "Bættu fyrst við myndbandi og veldu það."},
            {"Add video files", "Bæta við myndbandsskrám"},
            {"Add video files first.", "Bættu fyrst við myndbandsskrám."},
            {"Added %1 video file(s)", "%1 myndbandsskrá(m) bætt við"},
            {"Additional title:", "Viðbótartitill:"},
            {"Background color:", "Bakgrunnslitur:"},
            {"Balanced (safer for short videos)", "Jafnvægi (öruggara fyrir stutt myndbönd)"},
            {"Batch cancellation requested...", "Beðið um að hætta við runu..."},
            {"Browse...", "Fletta..."},
            {"Cancel batch", "Hætta við runu"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Beðið um að hætta við. Bíð eftir að núverandi vinnsluþráður stöðvist..."},
            {"Choose count text color", "Velja lit á talningartexta"},
            {"Choose font...", "Velja letur..."},
            {"Choose info text color", "Velja lit á upplýsingatexta"},
            {"Choose tile border color", "Velja lit á ramma smámyndar"},
            {"Choose timestamp background color", "Velja bakgrunnslit tímamerkis"},
            {"Choose timestamp text color", "Velja textalit tímamerkis"},
            {"Choose title text color", "Velja textalit titils"},
            {"Choose...", "Velja..."},
            {"Clear preview", "Hreinsa forskoðun"},
            {"Columns:", "Dálkar:"},
            {"Config 1", "Stilling 1"},
            {"Config 2", "Stilling 2"},
            {"Config 3", "Stilling 3"},
            {"Contact sheet preview", "Forskoðun smámyndablaðs"},
            {"Count text color:", "Litur talningartexta:"},
            {"Delete Selection", "Eyða vali"},
            {"Draw tile border", "Teikna ramma smámyndar"},
            {"Edition", "Breyta"},
            {"End margin:", "Lokajaðar:"},
            {"Exit", "Hætta"},
            {"Fast (Thumbnail me 3.0 style)", "Hratt (Thumbnail me 3.0-stíll)"},
            {"File", "Skrá"},
            {"Font:", "Letur:"},
            {"Gap:", "Bil:"},
            {"Generate a contact sheet first.", "Búðu fyrst til smámyndablað."},
            {"Info position:", "Staðsetning upplýsinga:"},
            {"Info text color:", "Litur upplýsingatexta:"},
            {"Info text:", "Upplýsingatexti:"},
            {"Input / Output", "Inntak / úttak"},
            {"Input list cleared", "Inntakslisti hreinsaður"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Tungumáli breytt. Vistaðu stillingar og endurræstu forritið til að virkja breytinguna að fullu."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Tungumálsval er vistað strax í settings.ini. Endurræstu forritið til að virkja fasta texta að fullu."},
            {"Language:", "Tungumál:"},
            {"Load", "Hlaða"},
            {"Load video files...", "Hlaða myndbandsskrám..."},
            {"Load videos and click Start selected or Start all.", "Hladdu myndböndum og smelltu á Ræsa valið eða Ræsa allt."},
            {"Lower Left", "Neðst til vinstri"},
            {"Lower Right", "Neðst til hægri"},
            {"No contact sheet to save", "Ekkert smámyndablað til að vista"},
            {"Open Recent Configuration", "Opna nýlega stillingu"},
            {"Open configuration...", "Opna stillingu..."},
            {"Output", "Úttak"},
            {"Output Format:", "Úttakssnið:"},
            {"Output filename suffix:", "Viðskeyti úttaksskrár:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Úttaksslóð:"},
            {"Predefined Configuration", "Forstillt stilling"},
            {"Preset applied: %1 rows × %2 columns", "Forstilling virkjuð: %1 raðir × %2 dálkar"},
            {"Preview area", "Forskoðunarsvæði"},
            {"Preview style", "Forskoða stíl"},
            {"Quality:", "Gæði:"},
            {"Randomize amount:", "Magn slembni:"},
            {"Randomize positions", "Slembiraða staðsetningum"},
            {"Randomize:", "Slembi:"},
            {"Reload settings.ini", "Endurhlaða settings.ini"},
            {"Reset", "Endurstilla"},
            {"Rows:", "Raðir:"},
            {"Same folder as source video", "Sama mappa og upprunamyndband"},
            {"Same source folder:", "Sama upprunamappa:"},
            {"Save configuration", "Vista stillingu"},
            {"Save configuration as...", "Vista stillingu sem..."},
            {"Save contact sheet", "Vista smámyndablað"},
            {"Save failed", "Vistun mistókst"},
            {"Save manually", "Vista handvirkt"},
            {"Save selected contact sheet...", "Vista valið smámyndablað..."},
            {"Save settings.ini", "Vista settings.ini"},
            {"Saved: ", "Vistað: "},
            {"Seek mode:", "Leitarhamur:"},
            {"Select output folder", "Velja úttaksmöppu"},
            {"Selected video file(s) removed", "Valdar myndbandsskrár fjarlægðar"},
            {"Settings saved:\n", "Stillingar vistaðar:\n"},
            {"Show additional title", "Sýna viðbótartitil"},
            {"Show technical info text", "Sýna tæknilegan upplýsingatexta"},
            {"Show timestamps", "Sýna tímamerki"},
            {"Similarity threshold:", "Þröskuldur líkinda:"},
            {"Skip dark / blank frames", "Sleppa dökkum / tómum römmum"},
            {"Skip dark:", "Sleppa dökkum:"},
            {"Skip similar frames", "Sleppa svipuðum römmum"},
            {"Skip similar:", "Sleppa svipuðum:"},
            {"Specific timestep:", "Ákveðið tímaskref:"},
            {"Start all", "Ræsa allt"},
            {"Start margin:", "Upphafsjaðar:"},
            {"Start selected", "Ræsa valið"},
            {"Style preview failed", "Forskoðun stíls mistókst"},
            {"Style preview ready", "Forskoðun stíls tilbúin"},
            {"Styles", "Stílar"},
            {"The file could not be saved.", "Ekki var hægt að vista skrána."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Litur ramma smámyndar:"},
            {"Tile border:", "Rammi smámyndar:"},
            {"Timestamp background:", "Bakgrunnur tímamerkis:"},
            {"Timestamp position:", "Staðsetning tímamerkis:"},
            {"Timestamp text color:", "Textalitur tímamerkis:"},
            {"Timestamp:", "Tímamerki:"},
            {"Timestep:", "Tímaskref:"},
            {"Title text color:", "Textalitur titils:"},
            {"Title text:", "Titiltexti:"},
            {"Upper Left", "Efst til vinstri"},
            {"Upper Right", "Efst til hægri"},
            {"Use specific timestep", "Nota ákveðið tímaskref"},
            {"View", "Skoða"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Breidd:"},
            {"English", "Enska"},
            {"French", "Franska"},
            {"German", "Þýska"},
            {"Spanish", "Spænska"},
            {"Dutch", "Hollenska"},
            {"Czech", "Tékkneska"},
            {"Italian", "Ítalska"},
            {"Hungarian", "Ungverska"},
            {"Croatian", "Króatíska"},
            {"Portuguese", "Portúgalska"},
            {"Chinese", "Kínverska"},
            {"Russian", "Rússneska"}
        };

                static const QHash<QString, QString> nor = {
            {"%1 rows × %2 columns", "%1 rader × %2 kolonner"},
            {"About", "Om"},
            {"About Thumbnail me 4b2", "Om Thumbnail me 4b2"},
            {"Accurate (slowest)", "Nøyaktig (tregest)"},
            {"Add and select a video first.", "Legg til og velg en video først."},
            {"Add video files", "Legg til videofiler"},
            {"Add video files first.", "Legg til videofiler først."},
            {"Added %1 video file(s)", "La til %1 videofil(er)"},
            {"Additional title:", "Ekstra tittel:"},
            {"Background color:", "Bakgrunnsfarge:"},
            {"Balanced (safer for short videos)", "Balansert (tryggere for korte videoer)"},
            {"Batch cancellation requested...", "Avbrytelse av kø er forespurt..."},
            {"Browse...", "Bla gjennom..."},
            {"Cancel batch", "Avbryt kø"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Avbrytelse forespurt. Venter på at gjeldende arbeider skal stoppe..."},
            {"Choose count text color", "Velg farge for telle­tekst"},
            {"Choose font...", "Velg skrift..."},
            {"Choose info text color", "Velg farge for infotekst"},
            {"Choose tile border color", "Velg farge for miniatyrramme"},
            {"Choose timestamp background color", "Velg bakgrunnsfarge for tidsstempel"},
            {"Choose timestamp text color", "Velg tekstfarge for tidsstempel"},
            {"Choose title text color", "Velg tekstfarge for tittel"},
            {"Choose...", "Velg..."},
            {"Clear preview", "Tøm forhåndsvisning"},
            {"Columns:", "Kolonner:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Forhåndsvisning av kontaktark"},
            {"Count text color:", "Farge for telle­tekst:"},
            {"Delete Selection", "Slett utvalg"},
            {"Draw tile border", "Tegn miniatyrramme"},
            {"Edition", "Redigering"},
            {"End margin:", "Sluttmargin:"},
            {"Exit", "Avslutt"},
            {"Fast (Thumbnail me 3.0 style)", "Rask (Thumbnail me 3.0-stil)"},
            {"File", "Fil"},
            {"Font:", "Skrift:"},
            {"Gap:", "Mellomrom:"},
            {"Generate a contact sheet first.", "Generer et kontaktark først."},
            {"Info position:", "Infoposisjon:"},
            {"Info text color:", "Farge for infotekst:"},
            {"Info text:", "Infotekst:"},
            {"Input / Output", "Inndata / utdata"},
            {"Input list cleared", "Inndatalisten er tømt"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Språket er endret. Lagre innstillingene og start programmet på nytt for å ta endringen helt i bruk."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Språkvalget lagres umiddelbart i settings.ini. Start programmet på nytt for å ta i bruk alle statiske etiketter."},
            {"Language:", "Språk:"},
            {"Load", "Last inn"},
            {"Load video files...", "Last inn videofiler..."},
            {"Load videos and click Start selected or Start all.", "Last inn videoer og klikk Start valgte eller Start alle."},
            {"Lower Left", "Nede til venstre"},
            {"Lower Right", "Nede til høyre"},
            {"No contact sheet to save", "Ingen kontaktark å lagre"},
            {"Open Recent Configuration", "Åpne nylig konfigurasjon"},
            {"Open configuration...", "Åpne konfigurasjon..."},
            {"Output", "Utdata"},
            {"Output Format:", "Utdataformat:"},
            {"Output filename suffix:", "Suffiks for utdatafilnavn:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Utdatabane:"},
            {"Predefined Configuration", "Forhåndsdefinert konfigurasjon"},
            {"Preset applied: %1 rows × %2 columns", "Forhåndsvalg brukt: %1 rader × %2 kolonner"},
            {"Preview area", "Forhåndsvisningsområde"},
            {"Preview style", "Forhåndsvis stil"},
            {"Quality:", "Kvalitet:"},
            {"Randomize amount:", "Grad av tilfeldig variasjon:"},
            {"Randomize positions", "Tilfeldiggjør posisjoner"},
            {"Randomize:", "Tilfeldig:"},
            {"Reload settings.ini", "Last inn settings.ini på nytt"},
            {"Reset", "Tilbakestill"},
            {"Rows:", "Rader:"},
            {"Same folder as source video", "Samme mappe som kildevideoen"},
            {"Same source folder:", "Samme kildemappe:"},
            {"Save configuration", "Lagre konfigurasjon"},
            {"Save configuration as...", "Lagre konfigurasjon som..."},
            {"Save contact sheet", "Lagre kontaktark"},
            {"Save failed", "Lagring mislyktes"},
            {"Save manually", "Lagre manuelt"},
            {"Save selected contact sheet...", "Lagre valgt kontaktark..."},
            {"Save settings.ini", "Lagre settings.ini"},
            {"Saved: ", "Lagret: "},
            {"Seek mode:", "Søkemodus:"},
            {"Select output folder", "Velg utdatamappe"},
            {"Selected video file(s) removed", "Valgte videofil(er) fjernet"},
            {"Settings saved:\n", "Innstillinger lagret:\n"},
            {"Show additional title", "Vis ekstra tittel"},
            {"Show technical info text", "Vis teknisk infotekst"},
            {"Show timestamps", "Vis tidsstempler"},
            {"Similarity threshold:", "Likhetsterskel:"},
            {"Skip dark / blank frames", "Hopp over mørke / tomme bilder"},
            {"Skip dark:", "Hopp over mørke:"},
            {"Skip similar frames", "Hopp over like bilder"},
            {"Skip similar:", "Hopp over like:"},
            {"Specific timestep:", "Bestemt tidssteg:"},
            {"Start all", "Start alle"},
            {"Start margin:", "Startmargin:"},
            {"Start selected", "Start valgte"},
            {"Style preview failed", "Stilforhåndsvisning mislyktes"},
            {"Style preview ready", "Stilforhåndsvisning klar"},
            {"Styles", "Stiler"},
            {"The file could not be saved.", "Filen kunne ikke lagres."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Farge på miniatyrramme:"},
            {"Tile border:", "Miniatyrramme:"},
            {"Timestamp background:", "Bakgrunn for tidsstempel:"},
            {"Timestamp position:", "Posisjon for tidsstempel:"},
            {"Timestamp text color:", "Tekstfarge for tidsstempel:"},
            {"Timestamp:", "Tidsstempel:"},
            {"Timestep:", "Tidssteg:"},
            {"Title text color:", "Tekstfarge for tittel:"},
            {"Title text:", "Titteltekst:"},
            {"Upper Left", "Oppe til venstre"},
            {"Upper Right", "Oppe til høyre"},
            {"Use specific timestep", "Bruk bestemt tidssteg"},
            {"View", "Vis"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Bredde:"},
            {"English", "Engelsk"},
            {"French", "Fransk"},
            {"German", "Tysk"},
            {"Spanish", "Spansk"},
            {"Dutch", "Nederlandsk"},
            {"Czech", "Tsjekkisk"},
            {"Italian", "Italiensk"},
            {"Hungarian", "Ungarsk"},
            {"Croatian", "Kroatisk"},
            {"Portuguese", "Portugisisk"},
            {"Chinese", "Kinesisk"},
            {"Russian", "Russisk"}
        };

                static const QHash<QString, QString> uk = {
            {"%1 rows × %2 columns", "%1 рядків × %2 стовпці"},
            {"About", "Про програму"},
            {"About Thumbnail me 4b2", "Про Thumbnail me 4b2"},
            {"Accurate (slowest)", "Точний (найповільніший)"},
            {"Add and select a video first.", "Спочатку додайте та виберіть відео."},
            {"Add video files", "Додати відеофайли"},
            {"Add video files first.", "Спочатку додайте відеофайли."},
            {"Added %1 video file(s)", "Додано %1 відеофайл(ів)"},
            {"Additional title:", "Додатковий заголовок:"},
            {"Background color:", "Колір фону:"},
            {"Balanced (safer for short videos)", "Збалансований (безпечніший для коротких відео)"},
            {"Batch cancellation requested...", "Запитано скасування пакета..."},
            {"Browse...", "Огляд..."},
            {"Cancel batch", "Скасувати пакет"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Скасування запитано. Очікування зупинки поточного процесу..."},
            {"Choose count text color", "Вибрати колір тексту лічильника"},
            {"Choose font...", "Вибрати шрифт..."},
            {"Choose info text color", "Вибрати колір інформаційного тексту"},
            {"Choose tile border color", "Вибрати колір рамки мініатюри"},
            {"Choose timestamp background color", "Вибрати колір фону часової мітки"},
            {"Choose timestamp text color", "Вибрати колір тексту часової мітки"},
            {"Choose title text color", "Вибрати колір тексту заголовка"},
            {"Choose...", "Вибрати..."},
            {"Clear preview", "Очистити попередній перегляд"},
            {"Columns:", "Стовпці:"},
            {"Config 1", "Конфіг. 1"},
            {"Config 2", "Конфіг. 2"},
            {"Config 3", "Конфіг. 3"},
            {"Contact sheet preview", "Попередній перегляд аркуша мініатюр"},
            {"Count text color:", "Колір тексту лічильника:"},
            {"Delete Selection", "Видалити вибране"},
            {"Draw tile border", "Малювати рамку мініатюри"},
            {"Edition", "Редагування"},
            {"End margin:", "Кінцеве поле:"},
            {"Exit", "Вихід"},
            {"Fast (Thumbnail me 3.0 style)", "Швидкий (стиль Thumbnail me 3.0)"},
            {"File", "Файл"},
            {"Font:", "Шрифт:"},
            {"Gap:", "Проміжок:"},
            {"Generate a contact sheet first.", "Спочатку створіть аркуш мініатюр."},
            {"Info position:", "Позиція інформації:"},
            {"Info text color:", "Колір інформаційного тексту:"},
            {"Info text:", "Інформаційний текст:"},
            {"Input / Output", "Вхід / вихід"},
            {"Input list cleared", "Список входу очищено"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Мову змінено. Збережіть налаштування й перезапустіть програму, щоб повністю застосувати зміну."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Вибір мови одразу зберігається в settings.ini. Перезапустіть програму, щоб повністю застосувати статичні написи."},
            {"Language:", "Мова:"},
            {"Load", "Завантажити"},
            {"Load video files...", "Завантажити відеофайли..."},
            {"Load videos and click Start selected or Start all.", "Завантажте відео й натисніть Запустити вибрані або Запустити всі."},
            {"Lower Left", "Внизу ліворуч"},
            {"Lower Right", "Внизу праворуч"},
            {"No contact sheet to save", "Немає аркуша мініатюр для збереження"},
            {"Open Recent Configuration", "Відкрити нещодавню конфігурацію"},
            {"Open configuration...", "Відкрити конфігурацію..."},
            {"Output", "Вихід"},
            {"Output Format:", "Формат виходу:"},
            {"Output filename suffix:", "Суфікс імені вихідного файлу:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Шлях виходу:"},
            {"Predefined Configuration", "Попередньо визначена конфігурація"},
            {"Preset applied: %1 rows × %2 columns", "Пресет застосовано: %1 рядків × %2 стовпці"},
            {"Preview area", "Область попереднього перегляду"},
            {"Preview style", "Попередній перегляд стилю"},
            {"Quality:", "Якість:"},
            {"Randomize amount:", "Ступінь випадковості:"},
            {"Randomize positions", "Випадково змінювати позиції"},
            {"Randomize:", "Випадково:"},
            {"Reload settings.ini", "Перезавантажити settings.ini"},
            {"Reset", "Скинути"},
            {"Rows:", "Рядки:"},
            {"Same folder as source video", "Та сама папка, що й вихідне відео"},
            {"Same source folder:", "Та сама вихідна папка:"},
            {"Save configuration", "Зберегти конфігурацію"},
            {"Save configuration as...", "Зберегти конфігурацію як..."},
            {"Save contact sheet", "Зберегти аркуш мініатюр"},
            {"Save failed", "Не вдалося зберегти"},
            {"Save manually", "Зберегти вручну"},
            {"Save selected contact sheet...", "Зберегти вибраний аркуш мініатюр..."},
            {"Save settings.ini", "Зберегти settings.ini"},
            {"Saved: ", "Збережено: "},
            {"Seek mode:", "Режим пошуку:"},
            {"Select output folder", "Вибрати вихідну папку"},
            {"Selected video file(s) removed", "Вибрані відеофайли вилучено"},
            {"Settings saved:\n", "Налаштування збережено:\n"},
            {"Show additional title", "Показувати додатковий заголовок"},
            {"Show technical info text", "Показувати технічний інформаційний текст"},
            {"Show timestamps", "Показувати часові мітки"},
            {"Similarity threshold:", "Поріг схожості:"},
            {"Skip dark / blank frames", "Пропускати темні / порожні кадри"},
            {"Skip dark:", "Пропускати темні:"},
            {"Skip similar frames", "Пропускати схожі кадри"},
            {"Skip similar:", "Пропускати схожі:"},
            {"Specific timestep:", "Певний часовий крок:"},
            {"Start all", "Запустити всі"},
            {"Start margin:", "Початкове поле:"},
            {"Start selected", "Запустити вибрані"},
            {"Style preview failed", "Попередній перегляд стилю не вдався"},
            {"Style preview ready", "Попередній перегляд стилю готовий"},
            {"Styles", "Стилі"},
            {"The file could not be saved.", "Не вдалося зберегти файл."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Колір рамки мініатюри:"},
            {"Tile border:", "Рамка мініатюри:"},
            {"Timestamp background:", "Фон часової мітки:"},
            {"Timestamp position:", "Позиція часової мітки:"},
            {"Timestamp text color:", "Колір тексту часової мітки:"},
            {"Timestamp:", "Часова мітка:"},
            {"Timestep:", "Часовий крок:"},
            {"Title text color:", "Колір тексту заголовка:"},
            {"Title text:", "Текст заголовка:"},
            {"Upper Left", "Угорі ліворуч"},
            {"Upper Right", "Угорі праворуч"},
            {"Use specific timestep", "Використовувати певний часовий крок"},
            {"View", "Перегляд"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Ширина:"},
            {"English", "Англійська"},
            {"French", "Французька"},
            {"German", "Німецька"},
            {"Spanish", "Іспанська"},
            {"Dutch", "Нідерландська"},
            {"Czech", "Чеська"},
            {"Italian", "Італійська"},
            {"Hungarian", "Угорська"},
            {"Croatian", "Хорватська"},
            {"Portuguese", "Португальська"},
            {"Chinese", "Китайська"},
            {"Russian", "Російська"}
        };

                static const QHash<QString, QString> tr = {
            {"%1 rows × %2 columns", "%1 satır × %2 sütun"},
            {"About", "Hakkında"},
            {"About Thumbnail me 4b2", "Thumbnail me 4b2 hakkında"},
            {"Accurate (slowest)", "Doğru (en yavaş)"},
            {"Add and select a video first.", "Önce bir video ekleyip seçin."},
            {"Add video files", "Video dosyaları ekle"},
            {"Add video files first.", "Önce video dosyaları ekleyin."},
            {"Added %1 video file(s)", "%1 video dosyası eklendi"},
            {"Additional title:", "Ek başlık:"},
            {"Background color:", "Arka plan rengi:"},
            {"Balanced (safer for short videos)", "Dengeli (kısa videolar için daha güvenli)"},
            {"Batch cancellation requested...", "Toplu işlemin iptali istendi..."},
            {"Browse...", "Gözat..."},
            {"Cancel batch", "Toplu işlemi iptal et"},
            {"Cancellation requested. Waiting for the current worker to stop...", "İptal istendi. Geçerli işçinin durması bekleniyor..."},
            {"Choose count text color", "Sayaç metni rengini seç"},
            {"Choose font...", "Yazı tipini seç..."},
            {"Choose info text color", "Bilgi metni rengini seç"},
            {"Choose tile border color", "Küçük resim kenarlığı rengini seç"},
            {"Choose timestamp background color", "Zaman damgası arka plan rengini seç"},
            {"Choose timestamp text color", "Zaman damgası metin rengini seç"},
            {"Choose title text color", "Başlık metni rengini seç"},
            {"Choose...", "Seç..."},
            {"Clear preview", "Önizlemeyi temizle"},
            {"Columns:", "Sütunlar:"},
            {"Config 1", "Yapılandırma 1"},
            {"Config 2", "Yapılandırma 2"},
            {"Config 3", "Yapılandırma 3"},
            {"Contact sheet preview", "Kontak sayfası önizlemesi"},
            {"Count text color:", "Sayaç metni rengi:"},
            {"Delete Selection", "Seçimi sil"},
            {"Draw tile border", "Küçük resim kenarlığını çiz"},
            {"Edition", "Düzenleme"},
            {"End margin:", "Bitiş boşluğu:"},
            {"Exit", "Çıkış"},
            {"Fast (Thumbnail me 3.0 style)", "Hızlı (Thumbnail me 3.0 tarzı)"},
            {"File", "Dosya"},
            {"Font:", "Yazı tipi:"},
            {"Gap:", "Boşluk:"},
            {"Generate a contact sheet first.", "Önce bir kontak sayfası oluşturun."},
            {"Info position:", "Bilgi konumu:"},
            {"Info text color:", "Bilgi metni rengi:"},
            {"Info text:", "Bilgi metni:"},
            {"Input / Output", "Giriş / çıkış"},
            {"Input list cleared", "Giriş listesi temizlendi"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Dil değiştirildi. Tam olarak uygulamak için ayarları kaydedip uygulamayı yeniden başlatın."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Dil seçimi hemen settings.ini dosyasına kaydedilir. Statik etiketlerin tamamen uygulanması için uygulamayı yeniden başlatın."},
            {"Language:", "Dil:"},
            {"Load", "Yükle"},
            {"Load video files...", "Video dosyalarını yükle..."},
            {"Load videos and click Start selected or Start all.", "Videoları yükleyin ve Seçilenleri başlat ya da Tümünü başlat düğmesine tıklayın."},
            {"Lower Left", "Sol alt"},
            {"Lower Right", "Sağ alt"},
            {"No contact sheet to save", "Kaydedilecek kontak sayfası yok"},
            {"Open Recent Configuration", "Son yapılandırmayı aç"},
            {"Open configuration...", "Yapılandırmayı aç..."},
            {"Output", "Çıkış"},
            {"Output Format:", "Çıkış biçimi:"},
            {"Output filename suffix:", "Çıkış dosya adı son eki:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Çıkış yolu:"},
            {"Predefined Configuration", "Önceden tanımlı yapılandırma"},
            {"Preset applied: %1 rows × %2 columns", "Ön ayar uygulandı: %1 satır × %2 sütun"},
            {"Preview area", "Önizleme alanı"},
            {"Preview style", "Stil önizlemesi"},
            {"Quality:", "Kalite:"},
            {"Randomize amount:", "Rastgelelik miktarı:"},
            {"Randomize positions", "Konumları rastgeleleştir"},
            {"Randomize:", "Rastgele:"},
            {"Reload settings.ini", "settings.ini dosyasını yeniden yükle"},
            {"Reset", "Sıfırla"},
            {"Rows:", "Satırlar:"},
            {"Same folder as source video", "Kaynak video ile aynı klasör"},
            {"Same source folder:", "Aynı kaynak klasör:"},
            {"Save configuration", "Yapılandırmayı kaydet"},
            {"Save configuration as...", "Yapılandırmayı farklı kaydet..."},
            {"Save contact sheet", "Kontak sayfasını kaydet"},
            {"Save failed", "Kaydetme başarısız"},
            {"Save manually", "Elle kaydet"},
            {"Save selected contact sheet...", "Seçili kontak sayfasını kaydet..."},
            {"Save settings.ini", "settings.ini dosyasını kaydet"},
            {"Saved: ", "Kaydedildi: "},
            {"Seek mode:", "Arama modu:"},
            {"Select output folder", "Çıkış klasörünü seç"},
            {"Selected video file(s) removed", "Seçili video dosyaları kaldırıldı"},
            {"Settings saved:\n", "Ayarlar kaydedildi:\n"},
            {"Show additional title", "Ek başlığı göster"},
            {"Show technical info text", "Teknik bilgi metnini göster"},
            {"Show timestamps", "Zaman damgalarını göster"},
            {"Similarity threshold:", "Benzerlik eşiği:"},
            {"Skip dark / blank frames", "Karanlık / boş kareleri atla"},
            {"Skip dark:", "Karanlıkları atla:"},
            {"Skip similar frames", "Benzer kareleri atla"},
            {"Skip similar:", "Benzerleri atla:"},
            {"Specific timestep:", "Belirli zaman adımı:"},
            {"Start all", "Tümünü başlat"},
            {"Start margin:", "Başlangıç boşluğu:"},
            {"Start selected", "Seçilenleri başlat"},
            {"Style preview failed", "Stil önizlemesi başarısız"},
            {"Style preview ready", "Stil önizlemesi hazır"},
            {"Styles", "Stiller"},
            {"The file could not be saved.", "Dosya kaydedilemedi."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Küçük resim kenarlığı rengi:"},
            {"Tile border:", "Küçük resim kenarlığı:"},
            {"Timestamp background:", "Zaman damgası arka planı:"},
            {"Timestamp position:", "Zaman damgası konumu:"},
            {"Timestamp text color:", "Zaman damgası metin rengi:"},
            {"Timestamp:", "Zaman damgası:"},
            {"Timestep:", "Zaman adımı:"},
            {"Title text color:", "Başlık metni rengi:"},
            {"Title text:", "Başlık metni:"},
            {"Upper Left", "Sol üst"},
            {"Upper Right", "Sağ üst"},
            {"Use specific timestep", "Belirli zaman adımını kullan"},
            {"View", "Görünüm"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Genişlik:"},
            {"English", "İngilizce"},
            {"French", "Fransızca"},
            {"German", "Almanca"},
            {"Spanish", "İspanyolca"},
            {"Dutch", "Felemenkçe"},
            {"Czech", "Çekçe"},
            {"Italian", "İtalyanca"},
            {"Hungarian", "Macarca"},
            {"Croatian", "Hırvatça"},
            {"Portuguese", "Portekizce"},
            {"Chinese", "Çince"},
            {"Russian", "Rusça"}
        };

                static const QHash<QString, QString> ca = {
            {"%1 rows × %2 columns", "%1 files × %2 columnes"},
            {"About", "Quant a"},
            {"About Thumbnail me 4b2", "Quant a Thumbnail me 4b2"},
            {"Accurate (slowest)", "Precís (el més lent)"},
            {"Add and select a video first.", "Afegeix i selecciona primer un vídeo."},
            {"Add video files", "Afegeix fitxers de vídeo"},
            {"Add video files first.", "Afegeix primer fitxers de vídeo."},
            {"Added %1 video file(s)", "S’han afegit %1 fitxer(s) de vídeo"},
            {"Additional title:", "Títol addicional:"},
            {"Background color:", "Color de fons:"},
            {"Balanced (safer for short videos)", "Equilibrat (més segur per a vídeos curts)"},
            {"Batch cancellation requested...", "S’ha sol·licitat la cancel·lació del lot..."},
            {"Browse...", "Navega..."},
            {"Cancel batch", "Cancel·la el lot"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Cancel·lació sol·licitada. S’espera que s’aturi el procés actual..."},
            {"Choose count text color", "Tria el color del text del comptador"},
            {"Choose font...", "Tria el tipus de lletra..."},
            {"Choose info text color", "Tria el color del text informatiu"},
            {"Choose tile border color", "Tria el color de la vora de la miniatura"},
            {"Choose timestamp background color", "Tria el color de fons de la marca de temps"},
            {"Choose timestamp text color", "Tria el color del text de la marca de temps"},
            {"Choose title text color", "Tria el color del text del títol"},
            {"Choose...", "Tria..."},
            {"Clear preview", "Neteja la previsualització"},
            {"Columns:", "Columnes:"},
            {"Config 1", "Config. 1"},
            {"Config 2", "Config. 2"},
            {"Config 3", "Config. 3"},
            {"Contact sheet preview", "Previsualització del full de miniatures"},
            {"Count text color:", "Color del text del comptador:"},
            {"Delete Selection", "Suprimeix la selecció"},
            {"Draw tile border", "Dibuixa la vora de la miniatura"},
            {"Edition", "Edició"},
            {"End margin:", "Marge final:"},
            {"Exit", "Surt"},
            {"Fast (Thumbnail me 3.0 style)", "Ràpid (estil Thumbnail me 3.0)"},
            {"File", "Fitxer"},
            {"Font:", "Tipus de lletra:"},
            {"Gap:", "Espai:"},
            {"Generate a contact sheet first.", "Genera primer un full de miniatures."},
            {"Info position:", "Posició de la informació:"},
            {"Info text color:", "Color del text informatiu:"},
            {"Info text:", "Text informatiu:"},
            {"Input / Output", "Entrada / sortida"},
            {"Input list cleared", "S’ha netejat la llista d’entrada"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "L’idioma ha canviat. Desa la configuració i reinicia l’aplicació per aplicar-lo completament."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "La selecció d’idioma es desa immediatament a settings.ini. Reinicia l’aplicació per aplicar completament les etiquetes estàtiques."},
            {"Language:", "Idioma:"},
            {"Load", "Carrega"},
            {"Load video files...", "Carrega fitxers de vídeo..."},
            {"Load videos and click Start selected or Start all.", "Carrega vídeos i fes clic a Inicia la selecció o Inicia-ho tot."},
            {"Lower Left", "Inferior esquerra"},
            {"Lower Right", "Inferior dreta"},
            {"No contact sheet to save", "No hi ha cap full de miniatures per desar"},
            {"Open Recent Configuration", "Obre la configuració recent"},
            {"Open configuration...", "Obre la configuració..."},
            {"Output", "Sortida"},
            {"Output Format:", "Format de sortida:"},
            {"Output filename suffix:", "Sufix del nom del fitxer de sortida:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Camí de sortida:"},
            {"Predefined Configuration", "Configuració predefinida"},
            {"Preset applied: %1 rows × %2 columns", "Predefinit aplicat: %1 files × %2 columnes"},
            {"Preview area", "Àrea de previsualització"},
            {"Preview style", "Previsualitza l’estil"},
            {"Quality:", "Qualitat:"},
            {"Randomize amount:", "Quantitat d’aleatorietat:"},
            {"Randomize positions", "Aleatoritza les posicions"},
            {"Randomize:", "Aleatoritza:"},
            {"Reload settings.ini", "Recarrega settings.ini"},
            {"Reset", "Restableix"},
            {"Rows:", "Files:"},
            {"Same folder as source video", "La mateixa carpeta que el vídeo d’origen"},
            {"Same source folder:", "La mateixa carpeta d’origen:"},
            {"Save configuration", "Desa la configuració"},
            {"Save configuration as...", "Desa la configuració com a..."},
            {"Save contact sheet", "Desa el full de miniatures"},
            {"Save failed", "No s’ha pogut desar"},
            {"Save manually", "Desa manualment"},
            {"Save selected contact sheet...", "Desa el full de miniatures seleccionat..."},
            {"Save settings.ini", "Desa settings.ini"},
            {"Saved: ", "Desat: "},
            {"Seek mode:", "Mode de cerca:"},
            {"Select output folder", "Selecciona la carpeta de sortida"},
            {"Selected video file(s) removed", "S’han eliminat els fitxers de vídeo seleccionats"},
            {"Settings saved:\n", "Configuració desada:\n"},
            {"Show additional title", "Mostra el títol addicional"},
            {"Show technical info text", "Mostra el text tècnic informatiu"},
            {"Show timestamps", "Mostra marques de temps"},
            {"Similarity threshold:", "Llindar de similitud:"},
            {"Skip dark / blank frames", "Omet fotogrames foscos / buits"},
            {"Skip dark:", "Omet foscos:"},
            {"Skip similar frames", "Omet fotogrames similars"},
            {"Skip similar:", "Omet similars:"},
            {"Specific timestep:", "Interval de temps específic:"},
            {"Start all", "Inicia-ho tot"},
            {"Start margin:", "Marge inicial:"},
            {"Start selected", "Inicia la selecció"},
            {"Style preview failed", "Ha fallat la previsualització de l’estil"},
            {"Style preview ready", "La previsualització de l’estil és a punt"},
            {"Styles", "Estils"},
            {"The file could not be saved.", "No s’ha pogut desar el fitxer."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Color de la vora de la miniatura:"},
            {"Tile border:", "Vora de la miniatura:"},
            {"Timestamp background:", "Fons de la marca de temps:"},
            {"Timestamp position:", "Posició de la marca de temps:"},
            {"Timestamp text color:", "Color del text de la marca de temps:"},
            {"Timestamp:", "Marca de temps:"},
            {"Timestep:", "Interval de temps:"},
            {"Title text color:", "Color del text del títol:"},
            {"Title text:", "Text del títol:"},
            {"Upper Left", "Superior esquerra"},
            {"Upper Right", "Superior dreta"},
            {"Use specific timestep", "Utilitza un interval de temps específic"},
            {"View", "Visualitza"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Amplada:"},
            {"English", "Anglès"},
            {"French", "Francès"},
            {"German", "Alemany"},
            {"Spanish", "Espanyol"},
            {"Dutch", "Neerlandès"},
            {"Czech", "Txec"},
            {"Italian", "Italià"},
            {"Hungarian", "Hongarès"},
            {"Croatian", "Croat"},
            {"Portuguese", "Portuguès"},
            {"Chinese", "Xinès"},
            {"Russian", "Rus"}
        };

                static const QHash<QString, QString> lb = {
            {"%1 rows × %2 columns", "%1 Reien × %2 Kolonnen"},
            {"About", "Iwwer"},
            {"About Thumbnail me 4b2", "Iwwer Thumbnail me 4b2"},
            {"Accurate (slowest)", "Genee (am luessten)"},
            {"Add and select a video first.", "Setzt fir d’éischt e Video dobäi a wielt en aus."},
            {"Add video files", "Videodateien dobäisetzen"},
            {"Add video files first.", "Setzt fir d’éischt Videodateien dobäi."},
            {"Added %1 video file(s)", "%1 Videodatei(en) dobäigesat"},
            {"Additional title:", "Zousätzlechen Titel:"},
            {"Background color:", "Hannergrondfaarf:"},
            {"Balanced (safer for short videos)", "Ausgeglach (méi sécher fir kuerz Videoen)"},
            {"Batch cancellation requested...", "Ofbrieche vum Batch ugefrot..."},
            {"Browse...", "Duerchsichen..."},
            {"Cancel batch", "Batch ofbriechen"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Ofbriechen ugefrot. Waarde bis den aktuelle Worker stoppt..."},
            {"Choose count text color", "Faarf vum Zielttext wielen"},
            {"Choose font...", "Schrëft wielen..."},
            {"Choose info text color", "Faarf vum Infotext wielen"},
            {"Choose tile border color", "Faarf vum Miniatur-Rand wielen"},
            {"Choose timestamp background color", "Hannergrondfaarf vum Zäitstempel wielen"},
            {"Choose timestamp text color", "Textfaarf vum Zäitstempel wielen"},
            {"Choose title text color", "Textfaarf vum Titel wielen"},
            {"Choose...", "Wielen..."},
            {"Clear preview", "Virschau läschen"},
            {"Columns:", "Kolonnen:"},
            {"Config 1", "Konfig. 1"},
            {"Config 2", "Konfig. 2"},
            {"Config 3", "Konfig. 3"},
            {"Contact sheet preview", "Virschau vum Kontaktblat"},
            {"Count text color:", "Faarf vum Zielttext:"},
            {"Delete Selection", "Auswiel läschen"},
            {"Draw tile border", "Miniatur-Rand zeechnen"},
            {"Edition", "Änneren"},
            {"End margin:", "Enn-Marge:"},
            {"Exit", "Erausgoen"},
            {"Fast (Thumbnail me 3.0 style)", "Séier (Thumbnail me 3.0-Stil)"},
            {"File", "Fichier"},
            {"Font:", "Schrëft:"},
            {"Gap:", "Ofstand:"},
            {"Generate a contact sheet first.", "Erstellt fir d’éischt e Kontaktblat."},
            {"Info position:", "Positioun vun den Informatiounen:"},
            {"Info text color:", "Faarf vum Infotext:"},
            {"Info text:", "Infotext:"},
            {"Input / Output", "Input / Output"},
            {"Input list cleared", "Input-Lëscht geläscht"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "D’Sprooch gouf geännert. Späichert d’Astellungen a start d’Applikatioun nei fir alles voll z’applizéieren."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "D’Sproochwal gëtt direkt an settings.ini gespäichert. Start d’Applikatioun nei fir statesch Beschrëftunge voll z’applizéieren."},
            {"Language:", "Sprooch:"},
            {"Load", "Lueden"},
            {"Load video files...", "Videodateien lueden..."},
            {"Load videos and click Start selected or Start all.", "Lued Videoen a klickt Ausgewielt starten oder Alles starten."},
            {"Lower Left", "Lénks ënnen"},
            {"Lower Right", "Riets ënnen"},
            {"No contact sheet to save", "Kee Kontaktblat fir ze späicheren"},
            {"Open Recent Configuration", "Rezent Konfiguratioun opmaachen"},
            {"Open configuration...", "Konfiguratioun opmaachen..."},
            {"Output", "Output"},
            {"Output Format:", "Output-Format:"},
            {"Output filename suffix:", "Suffix vum Output-Dateinumm:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Output-Wee:"},
            {"Predefined Configuration", "Virdefinéiert Konfiguratioun"},
            {"Preset applied: %1 rows × %2 columns", "Virastellung applizéiert: %1 Reien × %2 Kolonnen"},
            {"Preview area", "Virschau-Beräich"},
            {"Preview style", "Stil-Virschau"},
            {"Quality:", "Qualitéit:"},
            {"Randomize amount:", "Zoufallsquantitéit:"},
            {"Randomize positions", "Positiounen zoufälleg maachen"},
            {"Randomize:", "Zoufälleg:"},
            {"Reload settings.ini", "settings.ini nei lueden"},
            {"Reset", "Zerécksetzen"},
            {"Rows:", "Reien:"},
            {"Same folder as source video", "Selwechten Dossier wéi de Quellvideo"},
            {"Same source folder:", "Selwechte Quelldossier:"},
            {"Save configuration", "Konfiguratioun späicheren"},
            {"Save configuration as...", "Konfiguratioun späicheren als..."},
            {"Save contact sheet", "Kontaktblat späicheren"},
            {"Save failed", "Späicheren ass feelgeschloen"},
            {"Save manually", "Manuell späicheren"},
            {"Save selected contact sheet...", "Ausgewielt Kontaktblat späicheren..."},
            {"Save settings.ini", "settings.ini späicheren"},
            {"Saved: ", "Gespäichert: "},
            {"Seek mode:", "Seek-Modus:"},
            {"Select output folder", "Output-Dossier auswielen"},
            {"Selected video file(s) removed", "Ausgewielt Videodatei(en) geläscht"},
            {"Settings saved:\n", "Astellunge gespäichert:\n"},
            {"Show additional title", "Zousätzlechen Titel weisen"},
            {"Show technical info text", "Techneschen Infotext weisen"},
            {"Show timestamps", "Zäitstempel weisen"},
            {"Similarity threshold:", "Ähnlechkeets-Schwell:"},
            {"Skip dark / blank frames", "Donkel / eidel Frames iwwersprangen"},
            {"Skip dark:", "Donkel iwwersprangen:"},
            {"Skip similar frames", "Ähnlech Frames iwwersprangen"},
            {"Skip similar:", "Ähnlech iwwersprangen:"},
            {"Specific timestep:", "Bestëmmte Zäitstuf:"},
            {"Start all", "Alles starten"},
            {"Start margin:", "Start-Marge:"},
            {"Start selected", "Ausgewielt starten"},
            {"Style preview failed", "Stil-Virschau feelgeschloen"},
            {"Style preview ready", "Stil-Virschau fäerdeg"},
            {"Styles", "Stiler"},
            {"The file could not be saved.", "De Fichier konnt net gespäichert ginn."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Faarf vum Miniatur-Rand:"},
            {"Tile border:", "Miniatur-Rand:"},
            {"Timestamp background:", "Hannergrond vum Zäitstempel:"},
            {"Timestamp position:", "Positioun vum Zäitstempel:"},
            {"Timestamp text color:", "Textfaarf vum Zäitstempel:"},
            {"Timestamp:", "Zäitstempel:"},
            {"Timestep:", "Zäitstuf:"},
            {"Title text color:", "Textfaarf vum Titel:"},
            {"Title text:", "Titeltext:"},
            {"Upper Left", "Lénks uewen"},
            {"Upper Right", "Riets uewen"},
            {"Use specific timestep", "Bestëmmte Zäitstuf benotzen"},
            {"View", "Usiicht"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Breet:"},
            {"English", "Englesch"},
            {"French", "Franséisch"},
            {"German", "Däitsch"},
            {"Spanish", "Spuenesch"},
            {"Dutch", "Hollännesch"},
            {"Czech", "Tschechesch"},
            {"Italian", "Italienesch"},
            {"Hungarian", "Ungaresch"},
            {"Croatian", "Kroatesch"},
            {"Portuguese", "Portugisesch"},
            {"Chinese", "Chinesesch"},
            {"Russian", "Russesch"}
        };

                static const QHash<QString, QString> hy = {
            {"%1 rows × %2 columns", "%1 տող × %2 սյունակ"},
            {"About", "Ծրագրի մասին"},
            {"About Thumbnail me 4b2", "Thumbnail me 4b2-ի մասին"},
            {"Accurate (slowest)", "Ճշգրիտ (ամենադանդաղ)"},
            {"Add and select a video first.", "Նախ ավելացրեք և ընտրեք վիդեո։"},
            {"Add video files", "Ավելացնել վիդեո ֆայլեր"},
            {"Add video files first.", "Նախ ավելացրեք վիդեո ֆայլեր։"},
            {"Added %1 video file(s)", "Ավելացվել է %1 վիդեո ֆայլ"},
            {"Additional title:", "Լրացուցիչ վերնագիր:"},
            {"Background color:", "Ֆոնի գույն:"},
            {"Balanced (safer for short videos)", "Հավասարակշռված (ավելի ապահով կարճ վիդեոների համար)"},
            {"Batch cancellation requested...", "Փաթեթի չեղարկումը պահանջվել է..."},
            {"Browse...", "Թերթել..."},
            {"Cancel batch", "Չեղարկել փաթեթը"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Չեղարկումը պահանջվել է։ Սպասում է ընթացիկ գործընթացի կանգնեցմանը..."},
            {"Choose count text color", "Ընտրել հաշվիչի տեքստի գույնը"},
            {"Choose font...", "Ընտրել տառատեսակ..."},
            {"Choose info text color", "Ընտրել տեղեկատվական տեքստի գույնը"},
            {"Choose tile border color", "Ընտրել մանրապատկերի եզրագծի գույնը"},
            {"Choose timestamp background color", "Ընտրել ժամանակային նշանի ֆոնի գույնը"},
            {"Choose timestamp text color", "Ընտրել ժամանակային նշանի տեքստի գույնը"},
            {"Choose title text color", "Ընտրել վերնագրի տեքստի գույնը"},
            {"Choose...", "Ընտրել..."},
            {"Clear preview", "Մաքրել նախադիտումը"},
            {"Columns:", "Սյունակներ:"},
            {"Config 1", "Կոնֆիգ. 1"},
            {"Config 2", "Կոնֆիգ. 2"},
            {"Config 3", "Կոնֆիգ. 3"},
            {"Contact sheet preview", "Մանրապատկերների թերթի նախադիտում"},
            {"Count text color:", "Հաշվիչի տեքստի գույն:"},
            {"Delete Selection", "Ջնջել ընտրվածը"},
            {"Draw tile border", "Նկարել մանրապատկերի եզրագիծը"},
            {"Edition", "Խմբագրում"},
            {"End margin:", "Վերջի լուսանցք:"},
            {"Exit", "Ելք"},
            {"Fast (Thumbnail me 3.0 style)", "Արագ (Thumbnail me 3.0 ոճ)"},
            {"File", "Ֆայլ"},
            {"Font:", "Տառատեսակ:"},
            {"Gap:", "Բացատ:"},
            {"Generate a contact sheet first.", "Նախ ստեղծեք մանրապատկերների թերթ։"},
            {"Info position:", "Տեղեկության դիրք:"},
            {"Info text color:", "Տեղեկատվական տեքստի գույն:"},
            {"Info text:", "Տեղեկատվական տեքստ:"},
            {"Input / Output", "Մուտք / ելք"},
            {"Input list cleared", "Մուտքային ցուցակը մաքրված է"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Լեզուն փոխվել է։ Պահպանեք կարգավորումները և վերագործարկեք ծրագիրը՝ լիարժեք կիրառելու համար։"},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Լեզվի ընտրությունը անմիջապես պահվում է settings.ini-ում։ Վերագործարկեք ծրագիրը՝ ստատիկ պիտակները լիարժեք կիրառելու համար։"},
            {"Language:", "Լեզու:"},
            {"Load", "Բեռնել"},
            {"Load video files...", "Բեռնել վիդեո ֆայլեր..."},
            {"Load videos and click Start selected or Start all.", "Բեռնեք վիդեոները և սեղմեք Սկսել ընտրվածը կամ Սկսել բոլորը։"},
            {"Lower Left", "Ներքև ձախ"},
            {"Lower Right", "Ներքև աջ"},
            {"No contact sheet to save", "Պահպանելու մանրապատկերների թերթ չկա"},
            {"Open Recent Configuration", "Բացել վերջին կոնֆիգուրացիան"},
            {"Open configuration...", "Բացել կոնֆիգուրացիա..."},
            {"Output", "Ելք"},
            {"Output Format:", "Ելքի ձևաչափ:"},
            {"Output filename suffix:", "Ելքային ֆայլի անվան վերջածանց:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Ելքի ուղի:"},
            {"Predefined Configuration", "Նախապես սահմանված կոնֆիգուրացիա"},
            {"Preset applied: %1 rows × %2 columns", "Նախադրվածը կիրառվեց՝ %1 տող × %2 սյունակ"},
            {"Preview area", "Նախադիտման տարածք"},
            {"Preview style", "Ոճի նախադիտում"},
            {"Quality:", "Որակ:"},
            {"Randomize amount:", "Պատահականացման չափը:"},
            {"Randomize positions", "Պատահականացնել դիրքերը"},
            {"Randomize:", "Պատահականացնել:"},
            {"Reload settings.ini", "Վերբեռնել settings.ini"},
            {"Reset", "Վերակայել"},
            {"Rows:", "Տողեր:"},
            {"Same folder as source video", "Նույն պանակը, ինչ աղբյուր վիդեոն"},
            {"Same source folder:", "Նույն աղբյուր պանակը:"},
            {"Save configuration", "Պահպանել կոնֆիգուրացիան"},
            {"Save configuration as...", "Պահպանել կոնֆիգուրացիան որպես..."},
            {"Save contact sheet", "Պահպանել մանրապատկերների թերթը"},
            {"Save failed", "Պահպանումը ձախողվեց"},
            {"Save manually", "Պահպանել ձեռքով"},
            {"Save selected contact sheet...", "Պահպանել ընտրված մանրապատկերների թերթը..."},
            {"Save settings.ini", "Պահպանել settings.ini"},
            {"Saved: ", "Պահված է: "},
            {"Seek mode:", "Փնտրման ռեժիմ:"},
            {"Select output folder", "Ընտրել ելքային պանակը"},
            {"Selected video file(s) removed", "Ընտրված վիդեո ֆայլերը հեռացվել են"},
            {"Settings saved:\n", "Կարգավորումները պահպանված են:\n"},
            {"Show additional title", "Ցույց տալ լրացուցիչ վերնագիրը"},
            {"Show technical info text", "Ցույց տալ տեխնիկական տեղեկատվական տեքստը"},
            {"Show timestamps", "Ցույց տալ ժամանակային նշանները"},
            {"Similarity threshold:", "Նմանության շեմ:"},
            {"Skip dark / blank frames", "Բաց թողնել մութ / դատարկ կադրերը"},
            {"Skip dark:", "Բաց թողնել մութերը:"},
            {"Skip similar frames", "Բաց թողնել նման կադրերը"},
            {"Skip similar:", "Բաց թողնել նմանները:"},
            {"Specific timestep:", "Հատուկ ժամանակային քայլ:"},
            {"Start all", "Սկսել բոլորը"},
            {"Start margin:", "Սկզբի լուսանցք:"},
            {"Start selected", "Սկսել ընտրվածը"},
            {"Style preview failed", "Ոճի նախադիտումը ձախողվեց"},
            {"Style preview ready", "Ոճի նախադիտումը պատրաստ է"},
            {"Styles", "Ոճեր"},
            {"The file could not be saved.", "Ֆայլը չհաջողվեց պահպանել։"},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Մանրապատկերի եզրագծի գույն:"},
            {"Tile border:", "Մանրապատկերի եզրագիծ:"},
            {"Timestamp background:", "Ժամանակային նշանի ֆոն:"},
            {"Timestamp position:", "Ժամանակային նշանի դիրք:"},
            {"Timestamp text color:", "Ժամանակային նշանի տեքստի գույն:"},
            {"Timestamp:", "Ժամանակային նշան:"},
            {"Timestep:", "Ժամանակային քայլ:"},
            {"Title text color:", "Վերնագրի տեքստի գույն:"},
            {"Title text:", "Վերնագրի տեքստ:"},
            {"Upper Left", "Վերև ձախ"},
            {"Upper Right", "Վերև աջ"},
            {"Use specific timestep", "Օգտագործել հատուկ ժամանակային քայլ"},
            {"View", "Տեսք"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "Լայնություն:"},
            {"English", "Անգլերեն"},
            {"French", "Ֆրանսերեն"},
            {"German", "Գերմաներեն"},
            {"Spanish", "Իսպաներեն"},
            {"Dutch", "Նիդերլանդերեն"},
            {"Czech", "Չեխերեն"},
            {"Italian", "Իտալերեն"},
            {"Hungarian", "Հունգարերեն"},
            {"Croatian", "Խորվաթերեն"},
            {"Portuguese", "Պորտուգալերեն"},
            {"Chinese", "Չինարեն"},
            {"Russian", "Ռուսերեն"}
        };

                static const QHash<QString, QString> ka = {
            {"%1 rows × %2 columns", "%1 სტრიქონი × %2 სვეტი"},
            {"About", "პროგრამის შესახებ"},
            {"About Thumbnail me 4b2", "Thumbnail me 4b2-ის შესახებ"},
            {"Accurate (slowest)", "ზუსტი (ყველაზე ნელი)"},
            {"Add and select a video first.", "ჯერ დაამატეთ და აირჩიეთ ვიდეო."},
            {"Add video files", "ვიდეო ფაილების დამატება"},
            {"Add video files first.", "ჯერ დაამატეთ ვიდეო ფაილები."},
            {"Added %1 video file(s)", "დამატებულია %1 ვიდეო ფაილი"},
            {"Additional title:", "დამატებითი სათაური:"},
            {"Background color:", "ფონის ფერი:"},
            {"Balanced (safer for short videos)", "დაბალანსებული (უფრო უსაფრთხო მოკლე ვიდეოებისთვის)"},
            {"Batch cancellation requested...", "პაკეტის გაუქმება მოთხოვნილია..."},
            {"Browse...", "დათვალიერება..."},
            {"Cancel batch", "პაკეტის გაუქმება"},
            {"Cancellation requested. Waiting for the current worker to stop...", "გაუქმება მოთხოვნილია. მიმდინარეობს მიმდინარე პროცესის გაჩერების ლოდინი..."},
            {"Choose count text color", "აირჩიეთ მთვლელის ტექსტის ფერი"},
            {"Choose font...", "აირჩიეთ შრიფტი..."},
            {"Choose info text color", "აირჩიეთ საინფორმაციო ტექსტის ფერი"},
            {"Choose tile border color", "აირჩიეთ მინიატურის ჩარჩოს ფერი"},
            {"Choose timestamp background color", "აირჩიეთ დროის ნიშნის ფონის ფერი"},
            {"Choose timestamp text color", "აირჩიეთ დროის ნიშნის ტექსტის ფერი"},
            {"Choose title text color", "აირჩიეთ სათაურის ტექსტის ფერი"},
            {"Choose...", "არჩევა..."},
            {"Clear preview", "წინასწარი ხედის გასუფთავება"},
            {"Columns:", "სვეტები:"},
            {"Config 1", "კონფიგ. 1"},
            {"Config 2", "კონფიგ. 2"},
            {"Config 3", "კონფიგ. 3"},
            {"Contact sheet preview", "კონტაქტური ფურცლის წინასწარი ხედი"},
            {"Count text color:", "მთვლელის ტექსტის ფერი:"},
            {"Delete Selection", "მონიშნულის წაშლა"},
            {"Draw tile border", "მინიატურის ჩარჩოს დახატვა"},
            {"Edition", "რედაქტირება"},
            {"End margin:", "ბოლო მინდორი:"},
            {"Exit", "გასვლა"},
            {"Fast (Thumbnail me 3.0 style)", "სწრაფი (Thumbnail me 3.0 სტილი)"},
            {"File", "ფაილი"},
            {"Font:", "შრიფტი:"},
            {"Gap:", "შუალედი:"},
            {"Generate a contact sheet first.", "ჯერ შექმენით კონტაქტური ფურცელი."},
            {"Info position:", "ინფორმაციის პოზიცია:"},
            {"Info text color:", "საინფორმაციო ტექსტის ფერი:"},
            {"Info text:", "საინფორმაციო ტექსტი:"},
            {"Input / Output", "შეყვანა / გამოტანა"},
            {"Input list cleared", "შეყვანის სია გასუფთავებულია"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "ენა შეიცვალა. სრულად გამოსაყენებლად შეინახეთ პარამეტრები და გადატვირთეთ აპლიკაცია."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "ენის არჩევანი დაუყოვნებლივ ინახება settings.ini-ში. სტატიკური წარწერების სრულად გამოსაყენებლად გადატვირთეთ აპლიკაცია."},
            {"Language:", "ენა:"},
            {"Load", "ჩატვირთვა"},
            {"Load video files...", "ვიდეო ფაილების ჩატვირთვა..."},
            {"Load videos and click Start selected or Start all.", "ჩატვირთეთ ვიდეოები და დააწკაპუნეთ მონიშნულის გაშვებას ან ყველას გაშვებას."},
            {"Lower Left", "ქვედა მარცხენა"},
            {"Lower Right", "ქვედა მარჯვენა"},
            {"No contact sheet to save", "შესანახი კონტაქტური ფურცელი არ არის"},
            {"Open Recent Configuration", "ბოლო კონფიგურაციის გახსნა"},
            {"Open configuration...", "კონფიგურაციის გახსნა..."},
            {"Output", "გამოტანა"},
            {"Output Format:", "გამოტანის ფორმატი:"},
            {"Output filename suffix:", "გამოსატანი ფაილის სახელის სუფიქსი:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "გამოტანის გზა:"},
            {"Predefined Configuration", "წინასწარ განსაზღვრული კონფიგურაცია"},
            {"Preset applied: %1 rows × %2 columns", "წინასწარი პარამეტრი გამოყენებულია: %1 სტრიქონი × %2 სვეტი"},
            {"Preview area", "წინასწარი ხედის არე"},
            {"Preview style", "სტილის წინასწარი ხედი"},
            {"Quality:", "ხარისხი:"},
            {"Randomize amount:", "შემთხვევითობის რაოდენობა:"},
            {"Randomize positions", "პოზიციების შემთხვევითად შეცვლა"},
            {"Randomize:", "შემთხვევითად:"},
            {"Reload settings.ini", "settings.ini-ის ხელახლა ჩატვირთვა"},
            {"Reset", "გადატვირთვა"},
            {"Rows:", "სტრიქონები:"},
            {"Same folder as source video", "იგივე საქაღალდე, რაც საწყისი ვიდეო"},
            {"Same source folder:", "იგივე საწყისი საქაღალდე:"},
            {"Save configuration", "კონფიგურაციის შენახვა"},
            {"Save configuration as...", "კონფიგურაციის შენახვა როგორც..."},
            {"Save contact sheet", "კონტაქტური ფურცლის შენახვა"},
            {"Save failed", "შენახვა ვერ მოხერხდა"},
            {"Save manually", "ხელით შენახვა"},
            {"Save selected contact sheet...", "მონიშნული კონტაქტური ფურცლის შენახვა..."},
            {"Save settings.ini", "settings.ini-ის შენახვა"},
            {"Saved: ", "შენახულია: "},
            {"Seek mode:", "ძიების რეჟიმი:"},
            {"Select output folder", "აირჩიეთ გამოსატანი საქაღალდე"},
            {"Selected video file(s) removed", "მონიშნული ვიდეო ფაილები წაშლილია"},
            {"Settings saved:\n", "პარამეტრები შენახულია:\n"},
            {"Show additional title", "დამატებითი სათაურის ჩვენება"},
            {"Show technical info text", "ტექნიკური საინფორმაციო ტექსტის ჩვენება"},
            {"Show timestamps", "დროის ნიშნების ჩვენება"},
            {"Similarity threshold:", "მსგავსების ზღვარი:"},
            {"Skip dark / blank frames", "მუქი / ცარიელი კადრების გამოტოვება"},
            {"Skip dark:", "მუქების გამოტოვება:"},
            {"Skip similar frames", "მსგავსი კადრების გამოტოვება"},
            {"Skip similar:", "მსგავსების გამოტოვება:"},
            {"Specific timestep:", "კონკრეტული დროის ნაბიჯი:"},
            {"Start all", "ყველას გაშვება"},
            {"Start margin:", "საწყისი მინდორი:"},
            {"Start selected", "მონიშნულის გაშვება"},
            {"Style preview failed", "სტილის წინასწარი ხედი ვერ შესრულდა"},
            {"Style preview ready", "სტილის წინასწარი ხედი მზადაა"},
            {"Styles", "სტილები"},
            {"The file could not be saved.", "ფაილის შენახვა ვერ მოხერხდა."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "მინიატურის ჩარჩოს ფერი:"},
            {"Tile border:", "მინიატურის ჩარჩო:"},
            {"Timestamp background:", "დროის ნიშნის ფონი:"},
            {"Timestamp position:", "დროის ნიშნის პოზიცია:"},
            {"Timestamp text color:", "დროის ნიშნის ტექსტის ფერი:"},
            {"Timestamp:", "დროის ნიშანი:"},
            {"Timestep:", "დროის ნაბიჯი:"},
            {"Title text color:", "სათაურის ტექსტის ფერი:"},
            {"Title text:", "სათაურის ტექსტი:"},
            {"Upper Left", "ზედა მარცხენა"},
            {"Upper Right", "ზედა მარჯვენა"},
            {"Use specific timestep", "კონკრეტული დროის ნაბიჯის გამოყენება"},
            {"View", "ხედი"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "სიგანე:"},
            {"English", "ინგლისური"},
            {"French", "ფრანგული"},
            {"German", "გერმანული"},
            {"Spanish", "ესპანური"},
            {"Dutch", "ნიდერლანდური"},
            {"Czech", "ჩეხური"},
            {"Italian", "იტალიური"},
            {"Hungarian", "უნგრული"},
            {"Croatian", "ხორვატული"},
            {"Portuguese", "პორტუგალიური"},
            {"Chinese", "ჩინური"},
            {"Russian", "რუსული"}
        };

                static const QHash<QString, QString> az = {
            {"%1 rows × %2 columns", "%1 sətir × %2 sütun"},
            {"About", "Haqqında"},
            {"About Thumbnail me 4b2", "Thumbnail me 4b2 haqqında"},
            {"Accurate (slowest)", "Dəqiq (ən yavaş)"},
            {"Add and select a video first.", "Əvvəlcə video əlavə edib seçin."},
            {"Add video files", "Video faylları əlavə et"},
            {"Add video files first.", "Əvvəlcə video faylları əlavə edin."},
            {"Added %1 video file(s)", "%1 video faylı əlavə edildi"},
            {"Additional title:", "Əlavə başlıq:"},
            {"Background color:", "Fon rəngi:"},
            {"Balanced (safer for short videos)", "Balanslı (qısa videolar üçün daha təhlükəsiz)"},
            {"Batch cancellation requested...", "Toplu işin ləğvi istəndi..."},
            {"Browse...", "Gözdən keçir..."},
            {"Cancel batch", "Toplu işi ləğv et"},
            {"Cancellation requested. Waiting for the current worker to stop...", "Ləğv istəndi. Cari işçinin dayanması gözlənilir..."},
            {"Choose count text color", "Sayğac mətninin rəngini seç"},
            {"Choose font...", "Şrifti seç..."},
            {"Choose info text color", "Məlumat mətninin rəngini seç"},
            {"Choose tile border color", "Miniatür çərçivəsinin rəngini seç"},
            {"Choose timestamp background color", "Zaman damğasının fon rəngini seç"},
            {"Choose timestamp text color", "Zaman damğası mətninin rəngini seç"},
            {"Choose title text color", "Başlıq mətninin rəngini seç"},
            {"Choose...", "Seç..."},
            {"Clear preview", "Önizləməni təmizlə"},
            {"Columns:", "Sütunlar:"},
            {"Config 1", "Konfiq. 1"},
            {"Config 2", "Konfiq. 2"},
            {"Config 3", "Konfiq. 3"},
            {"Contact sheet preview", "Miniatür vərəqinin önizləməsi"},
            {"Count text color:", "Sayğac mətninin rəngi:"},
            {"Delete Selection", "Seçiləni sil"},
            {"Draw tile border", "Miniatür çərçivəsini çək"},
            {"Edition", "Redaktə"},
            {"End margin:", "Son boşluq:"},
            {"Exit", "Çıxış"},
            {"Fast (Thumbnail me 3.0 style)", "Sürətli (Thumbnail me 3.0 üslubu)"},
            {"File", "Fayl"},
            {"Font:", "Şrift:"},
            {"Gap:", "Boşluq:"},
            {"Generate a contact sheet first.", "Əvvəlcə miniatür vərəqi yaradın."},
            {"Info position:", "Məlumat mövqeyi:"},
            {"Info text color:", "Məlumat mətninin rəngi:"},
            {"Info text:", "Məlumat mətni:"},
            {"Input / Output", "Giriş / çıxış"},
            {"Input list cleared", "Giriş siyahısı təmizləndi"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"Language changed. Save settings and restart the application to fully apply it.", "Dil dəyişdirildi. Tam tətbiq etmək üçün ayarları saxlayın və tətbiqi yenidən başladın."},
            {"Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.", "Dil seçimi dərhal settings.ini faylına saxlanılır. Statik yazıların tam tətbiqi üçün tətbiqi yenidən başladın."},
            {"Language:", "Dil:"},
            {"Load", "Yüklə"},
            {"Load video files...", "Video faylları yüklə..."},
            {"Load videos and click Start selected or Start all.", "Videoları yükləyin və Seçilənləri başlat və ya Hamısını başlat düyməsinə klikləyin."},
            {"Lower Left", "Aşağı sol"},
            {"Lower Right", "Aşağı sağ"},
            {"No contact sheet to save", "Saxlanacaq miniatür vərəqi yoxdur"},
            {"Open Recent Configuration", "Son konfiqurasiyanı aç"},
            {"Open configuration...", "Konfiqurasiyanı aç..."},
            {"Output", "Çıxış"},
            {"Output Format:", "Çıxış formatı:"},
            {"Output filename suffix:", "Çıxış fayl adının sonluğu:"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"Path Output:", "Çıxış yolu:"},
            {"Predefined Configuration", "Əvvəlcədən təyin edilmiş konfiqurasiya"},
            {"Preset applied: %1 rows × %2 columns", "Əvvəlcədən ayar tətbiq edildi: %1 sətir × %2 sütun"},
            {"Preview area", "Önizləmə sahəsi"},
            {"Preview style", "Stilin önizləməsi"},
            {"Quality:", "Keyfiyyət:"},
            {"Randomize amount:", "Təsadüfilik miqdarı:"},
            {"Randomize positions", "Mövqeləri təsadüfiləşdir"},
            {"Randomize:", "Təsadüfi:"},
            {"Reload settings.ini", "settings.ini faylını yenidən yüklə"},
            {"Reset", "Sıfırla"},
            {"Rows:", "Sətirlər:"},
            {"Same folder as source video", "Mənbə video ilə eyni qovluq"},
            {"Same source folder:", "Eyni mənbə qovluğu:"},
            {"Save configuration", "Konfiqurasiyanı saxla"},
            {"Save configuration as...", "Konfiqurasiyanı belə saxla..."},
            {"Save contact sheet", "Miniatür vərəqini saxla"},
            {"Save failed", "Saxlama alınmadı"},
            {"Save manually", "Əl ilə saxla"},
            {"Save selected contact sheet...", "Seçilmiş miniatür vərəqini saxla..."},
            {"Save settings.ini", "settings.ini faylını saxla"},
            {"Saved: ", "Saxlanıldı: "},
            {"Seek mode:", "Axtarış rejimi:"},
            {"Select output folder", "Çıxış qovluğunu seç"},
            {"Selected video file(s) removed", "Seçilmiş video faylları silindi"},
            {"Settings saved:\n", "Ayarlar saxlanıldı:\n"},
            {"Show additional title", "Əlavə başlığı göstər"},
            {"Show technical info text", "Texniki məlumat mətnini göstər"},
            {"Show timestamps", "Zaman damğalarını göstər"},
            {"Similarity threshold:", "Oxşarlıq həddi:"},
            {"Skip dark / blank frames", "Tünd / boş kadrları burax"},
            {"Skip dark:", "Tündləri burax:"},
            {"Skip similar frames", "Oxşar kadrları burax"},
            {"Skip similar:", "Oxşarları burax:"},
            {"Specific timestep:", "Müəyyən zaman addımı:"},
            {"Start all", "Hamısını başlat"},
            {"Start margin:", "Başlanğıc boşluğu:"},
            {"Start selected", "Seçilənləri başlat"},
            {"Style preview failed", "Stil önizləməsi alınmadı"},
            {"Style preview ready", "Stil önizləməsi hazırdır"},
            {"Styles", "Stillər"},
            {"The file could not be saved.", "Faylı saxlamaq mümkün olmadı."},
            {"Thumbnail me", "Thumbnail me"},
            {"Tile border color:", "Miniatür çərçivəsinin rəngi:"},
            {"Tile border:", "Miniatür çərçivəsi:"},
            {"Timestamp background:", "Zaman damğasının fonu:"},
            {"Timestamp position:", "Zaman damğasının mövqeyi:"},
            {"Timestamp text color:", "Zaman damğası mətninin rəngi:"},
            {"Timestamp:", "Zaman damğası:"},
            {"Timestep:", "Zaman addımı:"},
            {"Title text color:", "Başlıq mətninin rəngi:"},
            {"Title text:", "Başlıq mətni:"},
            {"Upper Left", "Yuxarı sol"},
            {"Upper Right", "Yuxarı sağ"},
            {"Use specific timestep", "Müəyyən zaman addımından istifadə et"},
            {"View", "Görünüş"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Width:", "En:"},
            {"English", "İngiliscə"},
            {"French", "Fransızca"},
            {"German", "Almanca"},
            {"Spanish", "İspanca"},
            {"Dutch", "Niderlandca"},
            {"Czech", "Çexcə"},
            {"Italian", "İtalyanca"},
            {"Hungarian", "Macarca"},
            {"Croatian", "Xorvatca"},
            {"Portuguese", "Portuqalca"},
            {"Chinese", "Çincə"},
            {"Russian", "Rusca"}
        };

        const QHash<QString, QString> *translations = nullptr;
        if (language == "cs") translations = &cs;
        else if (language == "fr") translations = &fr;
        else if (language == "de") translations = &de;
        else if (language == "es") translations = &es;
        else if (language == "nl") translations = &nl;
        else if (language == "it") translations = &it;
        else if (language == "hu") translations = &hu;
        else if (language == "hr") translations = &hr;
        else if (language == "pt") translations = &pt;
        else if (language == "zh") translations = &zh;
        else if (language == "ru") translations = &ru;
        else if (language == "bg") translations = &bg;
        else if (language == "da") translations = &da;
        else if (language == "et") translations = &et;
        else if (language == "el") translations = &el;
        else if (language == "ga") translations = &ga;
        else if (language == "lv") translations = &lv;
        else if (language == "lt") translations = &lt;
        else if (language == "mt") translations = &mt;
        else if (language == "pl") translations = &pl;
        else if (language == "ro") translations = &ro;
        else if (language == "sk") translations = &sk;
        else if (language == "sl") translations = &sl;
        else if (language == "fi") translations = &fi;
        else if (language == "sv") translations = &sv;
        else if (language == "sq") translations = &sq;
        else if (language == "be") translations = &be;
        else if (language == "bs") translations = &bs;
        else if (language == "sr") translations = &sr;
        else if (language == "mk") translations = &mk;
        else if (language == "cnr") translations = &cnr;
        else if (language == "is") translations = &isl;
        else if (language == "no") translations = &nor;
        else if (language == "uk") translations = &uk;
        else if (language == "tr") translations = &tr;
        else if (language == "ca") translations = &ca;
        else if (language == "lb") translations = &lb;
        else if (language == "hy") translations = &hy;
        else if (language == "ka") translations = &ka;
        else if (language == "az") translations = &az;

        return translations ? translations->value(english, english) : english;
    }

    QString supportedVideoFilter()
    {
        return QString(
            "Supported video files ("
            "*.mkv *.mk3d "
            "*.mp4 *.m4v *.mov *.qt "
            "*.webm *.ivf "
            "*.avi *.divx *.xvid "
            "*.wmv *.asf "
            "*.flv *.f4v "
            "*.ogv *.ogg "
            "*.ts *.mts *.m2ts *.m2t "
            "*.mpg *.mpeg *.m1v *.m2v *.vob "
            "*.3gp *.3g2 "
            "*.rm *.rmvb "
            "*.mxf *.nut *.y4m "
            "*.h264 *.264 *.avc "
            "*.h265 *.265 *.hevc "
            "*.av1 *.obu"
            ");;"
            "Modern containers (*.mkv *.mp4 *.m4v *.mov *.webm);;"
            "HEVC / H.265 raw streams (*.h265 *.265 *.hevc);;"
            "AVC / H.264 raw streams (*.h264 *.264 *.avc);;"
            "AV1 streams (*.av1 *.obu *.ivf);;"
            "Transport streams (*.ts *.mts *.m2ts *.m2t);;"
            "Legacy video files (*.avi *.wmv *.asf *.flv *.f4v *.mpg *.mpeg *.vob *.3gp *.3g2 *.ogv *.rm *.rmvb);;"
            "All files (*.*)"
        );
    }
}


MainWindow::MainWindow()
{
    setWindowTitle("Thumbnail me 4b2");
    resize(1500, 850);

    selectedFont = font();
    selectedFont.setPointSize(10);

    currentLanguage = savedLanguage();

    auto T = [this](const QString &text) {
        return uiText(text);
    };

    auto *mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);

    auto *leftPanel = new QWidget(mainSplitter);
    leftPanel->setMinimumWidth(560);
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(6, 6, 6, 6);
    leftLayout->setSpacing(6);

    auto *filesBox = new QGroupBox(T("Input / Output"), leftPanel);
    auto *filesLayout = new QVBoxLayout(filesBox);

    inputList = new QListWidget(filesBox);
    inputList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    inputList->setAlternatingRowColors(true);
    inputList->setMinimumHeight(150);

    auto *fileButtonsLayout = new QHBoxLayout();
    auto *addButton = new QPushButton(T("Load"), filesBox);
    auto *removeButton = new QPushButton(T("Delete Selection"), filesBox);
    auto *clearButton = new QPushButton(T("Reset"), filesBox);

    fileButtonsLayout->addWidget(addButton);
    fileButtonsLayout->addWidget(removeButton);
    fileButtonsLayout->addWidget(clearButton);

    filesLayout->addWidget(inputList);
    filesLayout->addLayout(fileButtonsLayout);

    auto *settingsTabs = new QTabWidget(leftPanel);
    settingsTabs->setMinimumWidth(540);
    settingsTabs->setUsesScrollButtons(false);
    settingsTabs->setElideMode(Qt::ElideNone);

    auto *layoutTab = new QWidget(settingsTabs);
    auto *layoutForm = new QFormLayout(layoutTab);

    columnsSpin = new QSpinBox(layoutTab);
    columnsSpin->setRange(1, 50);
    columnsSpin->setValue(4);
    columnsSpin->setToolTip("Columns = number of thumbnails horizontally.");

    rowsSpin = new QSpinBox(layoutTab);
    rowsSpin->setRange(1, 50);
    rowsSpin->setValue(4);
    rowsSpin->setToolTip("Rows = number of thumbnails vertically.");

    thumbWidthSpin = new QSpinBox(layoutTab);
    thumbWidthSpin->setRange(120, 1200);
    thumbWidthSpin->setSingleStep(20);
    thumbWidthSpin->setValue(320);
    thumbWidthSpin->setSuffix(" px");

    gapSpin = new QSpinBox(layoutTab);
    gapSpin->setRange(0, 80);
    gapSpin->setValue(8);
    gapSpin->setSuffix(" px");

    timestampsCheck = new QCheckBox(T("Show timestamps"), layoutTab);
    timestampsCheck->setChecked(true);

    speedModeCombo = new QComboBox(layoutTab);
    speedModeCombo->addItem(T("Fast (Thumbnail me 3.0 style)"), "fast");
    speedModeCombo->addItem(T("Balanced (safer for short videos)"), "balanced");
    speedModeCombo->addItem(T("Accurate (slowest)"), "accurate");
    speedModeCombo->setToolTip("Fast is closest to the old Thumbnail me 3 behavior. Accurate is safer for sparse keyframes, but slower.");

    layoutForm->addRow(T("Columns:"), columnsSpin);
    layoutForm->addRow(T("Rows:"), rowsSpin);
    layoutForm->addRow(T("Width:"), thumbWidthSpin);
    layoutForm->addRow(T("Gap:"), gapSpin);
    layoutForm->addRow(T("Timestamp:"), timestampsCheck);
    layoutForm->addRow(T("Seek mode:"), speedModeCombo);

    auto *layoutTab2 = new QWidget(settingsTabs);
    auto *layoutForm2 = new QFormLayout(layoutTab2);

    startMarginSpin = new QDoubleSpinBox(layoutTab2);
    startMarginSpin->setRange(0.0, 45.0);
    startMarginSpin->setDecimals(1);
    startMarginSpin->setSingleStep(1.0);
    startMarginSpin->setValue(7.0);
    startMarginSpin->setSuffix(" %");
    startMarginSpin->setToolTip("Ignore the beginning of the video when choosing thumbnails.");

    endMarginSpin = new QDoubleSpinBox(layoutTab2);
    endMarginSpin->setRange(0.0, 45.0);
    endMarginSpin->setDecimals(1);
    endMarginSpin->setSingleStep(1.0);
    endMarginSpin->setValue(7.0);
    endMarginSpin->setSuffix(" %");
    endMarginSpin->setToolTip("Ignore the end of the video when choosing thumbnails.");

    skipDarkFramesCheck = new QCheckBox(T("Skip dark / blank frames"), layoutTab2);
    skipDarkFramesCheck->setChecked(false);
    skipDarkFramesCheck->setToolTip("Try nearby timestamps when a captured frame is probably black or blank.");

    skipSimilarFramesCheck = new QCheckBox(T("Skip similar frames"), layoutTab2);
    skipSimilarFramesCheck->setChecked(false);
    skipSimilarFramesCheck->setToolTip("Try nearby timestamps when a captured frame is too similar to the previously accepted thumbnail.");

    similarThresholdSpin = new QDoubleSpinBox(layoutTab2);
    similarThresholdSpin->setRange(80.0, 99.5);
    similarThresholdSpin->setDecimals(1);
    similarThresholdSpin->setSingleStep(1.0);
    similarThresholdSpin->setValue(92.0);
    similarThresholdSpin->setSuffix(" %");
    similarThresholdSpin->setToolTip("Higher values are stricter. 92% is a safe default.");

    randomizePositionsCheck = new QCheckBox(T("Randomize positions"), layoutTab2);
    randomizePositionsCheck->setChecked(false);

    randomizePercentSpin = new QDoubleSpinBox(layoutTab2);
    randomizePercentSpin->setRange(1.0, 50.0);
    randomizePercentSpin->setDecimals(1);
    randomizePercentSpin->setSingleStep(1.0);
    randomizePercentSpin->setValue(10.0);
    randomizePercentSpin->setSuffix(" %");
    randomizePercentSpin->setToolTip("Randomize each evenly selected timestamp within this percentage of its spacing.");

    specificTimestepCheck = new QCheckBox(T("Use specific timestep"), layoutTab2);
    specificTimestepCheck->setChecked(false);

    timestepSpin = new QDoubleSpinBox(layoutTab2);
    timestepSpin->setRange(1.0, 3600.0);
    timestepSpin->setDecimals(1);
    timestepSpin->setSingleStep(5.0);
    timestepSpin->setValue(60.0);
    timestepSpin->setSuffix(" s");
    timestepSpin->setToolTip("Generate thumbnails at a fixed time interval until the grid is full.");

    showInfoTextCheck = new QCheckBox(T("Show technical info text"), layoutTab2);
    showInfoTextCheck->setChecked(true);
    showInfoTextCheck->setToolTip("Show or hide the MediaInfo-style technical text in the contact sheet.");

    infoTextPositionCombo = new QComboBox(layoutTab2);
    infoTextPositionCombo->addItem(T("Upper Left"), "upper-left");
    infoTextPositionCombo->addItem(T("Lower Left"), "lower-left");
    infoTextPositionCombo->addItem(T("Lower Right"), "lower-right");
    infoTextPositionCombo->addItem(T("Upper Right"), "upper-right");
    infoTextPositionCombo->setToolTip("Where to place the technical information text in the generated image.");

    timestampPositionCombo = new QComboBox(layoutTab2);
    timestampPositionCombo->addItem(T("Upper Left"), "upper-left");
    timestampPositionCombo->addItem(T("Lower Left"), "lower-left");
    timestampPositionCombo->addItem(T("Lower Right"), "lower-right");
    timestampPositionCombo->addItem(T("Upper Right"), "upper-right");
    timestampPositionCombo->setCurrentIndex(1);
    timestampPositionCombo->setToolTip("Where to place timestamps inside every thumbnail.");

    layoutForm2->addRow(T("Start margin:"), startMarginSpin);
    layoutForm2->addRow(T("End margin:"), endMarginSpin);
    layoutForm2->addRow(T("Skip dark:"), skipDarkFramesCheck);
    layoutForm2->addRow(T("Skip similar:"), skipSimilarFramesCheck);
    layoutForm2->addRow(T("Similarity threshold:"), similarThresholdSpin);
    layoutForm2->addRow(T("Randomize:"), randomizePositionsCheck);
    layoutForm2->addRow(T("Randomize amount:"), randomizePercentSpin);
    layoutForm2->addRow(T("Specific timestep:"), specificTimestepCheck);
    layoutForm2->addRow(T("Timestep:"), timestepSpin);
    layoutForm2->addRow(T("Info text:"), showInfoTextCheck);
    layoutForm2->addRow(T("Info position:"), infoTextPositionCombo);
    layoutForm2->addRow(T("Timestamp position:"), timestampPositionCombo);

    auto *outputTab = new QWidget(settingsTabs);
    auto *outputForm = new QFormLayout(outputTab);

    outputFolderEdit = new QLineEdit(outputTab);
    outputFolderEdit->setText(QDir::homePath());

    auto *browseButton = new QPushButton(T("Browse..."), outputTab);
    auto *folderLayout = new QHBoxLayout();
    folderLayout->addWidget(outputFolderEdit, 1);
    folderLayout->addWidget(browseButton);

    sameSourceFolderCheck = new QCheckBox(T("Same folder as source video"), outputTab);
    sameSourceFolderCheck->setToolTip("When enabled, generated images are saved next to each source video.");

    filenameSuffixEdit = new QLineEdit(outputTab);
    filenameSuffixEdit->setPlaceholderText("_thumbs (optional)");
    filenameSuffixEdit->setToolTip("Optional text inserted before the output extension, for example _thumbs or -screens.");

    formatCombo = new QComboBox(outputTab);
    formatCombo->addItem(T("JPEG (*.jpg)"), "jpg");
    formatCombo->addItem(T("PNG (*.png)"), "png");
    formatCombo->addItem(T("WebP (*.webp)"), "webp");
    formatCombo->setToolTip("Choose the output image format: JPEG, PNG or WebP.");

    qualitySpin = new QSpinBox(outputTab);
    qualitySpin->setRange(1, 100);
    qualitySpin->setValue(95);
    qualitySpin->setSuffix(" %");

    outputForm->addRow(T("Path Output:"), folderLayout);
    outputForm->addRow(T("Same source folder:"), sameSourceFolderCheck);
    outputForm->addRow(T("Output filename suffix:"), filenameSuffixEdit);
    outputForm->addRow(T("Output Format:"), formatCombo);
    outputForm->addRow(T("Quality:"), qualitySpin);

    auto *appearanceTab = new QWidget(settingsTabs);
    auto *appearanceForm = new QFormLayout(appearanceTab);

    fontPreviewLabel = new QLabel(appearanceTab);
    fontPreviewLabel->setText("Default font");
    fontPreviewLabel->setMinimumHeight(28);

    auto *fontButton = new QPushButton(T("Choose font..."), appearanceTab);

    additionalTitleCheck = new QCheckBox(T("Show additional title"), appearanceTab);
    additionalTitleCheck->setChecked(true);
    additionalTitleCheck->setToolTip("Optional custom text shown in the contact sheet, similar to Thumbnail me 3.0 Additional Title.");

    additionalTitleEdit = new QLineEdit(appearanceTab);
    additionalTitleEdit->setText("Generated by Thumbnail me");

    backgroundColorEdit = new QLineEdit(appearanceTab);
    backgroundColorEdit->setText("#141414");
    backgroundColorEdit->setToolTip("Contact sheet background color. Use a hex color such as #141414.");

    auto *backgroundColorButton = new QPushButton(T("Choose..."), appearanceTab);

    auto *backgroundColorLayout = new QHBoxLayout();
    backgroundColorLayout->addWidget(backgroundColorEdit, 1);
    backgroundColorLayout->addWidget(backgroundColorButton);

    appearanceForm->addRow(T("Font:"), fontPreviewLabel);
    appearanceForm->addRow("", fontButton);
    appearanceForm->addRow(T("Additional title:"), additionalTitleCheck);
    appearanceForm->addRow(T("Title text:"), additionalTitleEdit);
    appearanceForm->addRow(T("Background color:"), backgroundColorLayout);

    languageCombo = new QComboBox(appearanceTab);
    languageCombo->addItem("English", "en");
    languageCombo->addItem("Français", "fr");
    languageCombo->addItem("Deutsch", "de");
    languageCombo->addItem("Español", "es");
    languageCombo->addItem("Nederlands", "nl");
    languageCombo->addItem("Česky", "cs");
    languageCombo->addItem("Italiano", "it");
    languageCombo->addItem("Magyar", "hu");
    languageCombo->addItem("Hrvatski", "hr");
    languageCombo->addItem("Português", "pt");
    languageCombo->addItem("中文", "zh");
    languageCombo->addItem("Русский", "ru");
    languageCombo->addItem("Български", "bg");
    languageCombo->addItem("Dansk", "da");
    languageCombo->addItem("Eesti", "et");
    languageCombo->addItem("Ελληνικά", "el");
    languageCombo->addItem("Gaeilge", "ga");
    languageCombo->addItem("Latviešu", "lv");
    languageCombo->addItem("Lietuvių", "lt");
    languageCombo->addItem("Malti", "mt");
    languageCombo->addItem("Polski", "pl");
    languageCombo->addItem("Română", "ro");
    languageCombo->addItem("Slovenčina", "sk");
    languageCombo->addItem("Slovenščina", "sl");
    languageCombo->addItem("Suomi", "fi");
    languageCombo->addItem("Svenska", "sv");
    languageCombo->addItem("Shqip", "sq");
    languageCombo->addItem("Беларуская", "be");
    languageCombo->addItem("Bosanski", "bs");
    languageCombo->addItem("Српски", "sr");
    languageCombo->addItem("Македонски", "mk");
    languageCombo->addItem("Crnogorski", "cnr");
    languageCombo->addItem("Íslenska", "is");
    languageCombo->addItem("Norsk", "no");
    languageCombo->addItem("Українська", "uk");
    languageCombo->addItem("Türkçe", "tr");
    languageCombo->addItem("Català", "ca");
    languageCombo->addItem("Lëtzebuergesch", "lb");
    languageCombo->addItem("Հայերեն", "hy");
    languageCombo->addItem("ქართული", "ka");
    languageCombo->addItem("Azərbaycanca", "az");
    languageCombo->setToolTip(T("Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels."));

    const int startupLanguageIndex = languageCombo->findData(currentLanguage);
    if (startupLanguageIndex >= 0) {
        languageCombo->setCurrentIndex(startupLanguageIndex);
    }

    appearanceForm->addRow(T("Language:"), languageCombo);

    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        currentLanguage = normalizedLanguage(languageCombo->currentData().toString());
        saveLanguageSetting(currentLanguage);
        statusBar()->showMessage(uiText("Language changed. Save settings and restart the application to fully apply it."));
    });

    auto *configuration3Tab = new QWidget(settingsTabs);
    auto *configuration3Form = new QFormLayout(configuration3Tab);

    auto makeColorRow = [this, configuration3Tab](const QString &defaultColor, const QString &dialogTitle, QLineEdit **editOut) {
        auto *edit = new QLineEdit(configuration3Tab);
        edit->setText(defaultColor);
        edit->setToolTip("Use a hex color such as #ffffff.");

        auto *button = new QPushButton(uiText("Choose..."), configuration3Tab);

        auto *layout = new QHBoxLayout();
        layout->addWidget(edit, 1);
        layout->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, edit, dialogTitle]() {
            const QColor initial(edit->text().trimmed());
            const QColor selected = QColorDialog::getColor(
                initial.isValid() ? initial : QColor(255, 255, 255),
                this,
                dialogTitle
            );

            if (selected.isValid()) {
                edit->setText(selected.name(QColor::HexRgb));
            }
        });

        *editOut = edit;
        return layout;
    };

    configuration3Form->addRow(T("Title text color:"), makeColorRow("#ffffff", T("Choose title text color"), &titleTextColorEdit));
    configuration3Form->addRow(T("Info text color:"), makeColorRow("#e6e6e6", T("Choose info text color"), &infoTextColorEdit));
    configuration3Form->addRow(T("Count text color:"), makeColorRow("#d2d2d2", T("Choose count text color"), &countTextColorEdit));
    configuration3Form->addRow(T("Timestamp text color:"), makeColorRow("#ffffff", T("Choose timestamp text color"), &timestampTextColorEdit));
    configuration3Form->addRow(T("Timestamp background:"), makeColorRow("#000000", T("Choose timestamp background color"), &timestampBackgroundColorEdit));

    tileBorderCheck = new QCheckBox(T("Draw tile border"), configuration3Tab);
    tileBorderCheck->setChecked(true);
    configuration3Form->addRow(T("Tile border:"), tileBorderCheck);
    configuration3Form->addRow(T("Tile border color:"), makeColorRow("#464646", T("Choose tile border color"), &tileBorderColorEdit));

        settingsTabs->addTab(layoutTab, T("Config 1"));
    settingsTabs->addTab(layoutTab2, T("Config 2"));
    settingsTabs->addTab(configuration3Tab, T("Config 3"));
    settingsTabs->addTab(outputTab, T("Output"));
    settingsTabs->addTab(appearanceTab, T("Styles"));

    auto *actionButtonsLayout = new QHBoxLayout();
    generateButton = new QPushButton(T("Start selected"), leftPanel);
    generateAllButton = new QPushButton(T("Start all"), leftPanel);
    cancelBatchButton = new QPushButton(T("Cancel batch"), leftPanel);
    cancelBatchButton->setEnabled(false);
    previewStyleButton = new QPushButton(T("Preview style"), leftPanel);
    previewStyleButton->setToolTip("Create a style preview instantly, without decoding the whole video.");
    saveButton = new QPushButton(T("Save manually"), leftPanel);

    actionButtonsLayout->addWidget(generateButton);
    actionButtonsLayout->addWidget(generateAllButton);
    actionButtonsLayout->addWidget(cancelBatchButton);
    actionButtonsLayout->addWidget(previewStyleButton);
    actionButtonsLayout->addWidget(saveButton);

    output = new QTextEdit(leftPanel);
    output->setReadOnly(true);
    output->setText(
        QString("Thumbnail me 4b2\n\n"
                "Beta 2 build:\n"
                "- Broad multilingual UI translation set\n"
                "- Optimized FFmpeg runtime size\n"
                "- Smaller Windows ZIP package\n"
                "- MediaInfo audio/video stream bitrates when available\n"
                "- WebP output, settings.ini and worker process preserved\n\n%1")
            .arg(T("Load videos and click Start selected or Start all."))
    );

    leftLayout->addWidget(filesBox);
    leftLayout->addWidget(settingsTabs);
    leftLayout->addLayout(actionButtonsLayout);
    leftLayout->addWidget(output, 1);

    preview = new QLabel(mainSplitter);
    preview->setAlignment(Qt::AlignCenter);
    preview->setText(uiText("Preview area"));
    preview->setMinimumSize(760, 540);
    preview->setStyleSheet("QLabel { background: #b8b8b8; color: #333333; border: 1px solid #9a9a9a; }");

    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(preview);
    mainSplitter->setStretchFactor(0, 0);
    mainSplitter->setStretchFactor(1, 1);
    mainSplitter->setSizes({560, 940});

    setCentralWidget(mainSplitter);

    setStyleSheet(
        "QMainWindow { background: #c0c0c0; }"
        "QSplitter { background: #c0c0c0; }"
        "QGroupBox { background: #efefef; border: 1px solid #a8a8a8; border-radius: 6px; margin-top: 18px; padding-top: 14px; }"
        "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 10px; top: 3px; padding: 0 4px; background: #efefef; }"
        "QTabWidget::pane { border: 1px solid #a8a8a8; background: #efefef; }"
        "QTextEdit, QListWidget, QLineEdit { background: #ffffff; color: #000000; }"
        "QListWidget::item { color: #000000; padding: 3px; }"
        "QListWidget::item:selected { background: #d7e8ff; color: #000000; }"
        "QListWidget::item:hover { background: #eef5ff; color: #000000; }"
    );

    auto *fileMenu = menuBar()->addMenu(uiText("File"));
    fileMenu->addAction(uiText("Load video files..."), this, [this]() { addVideos(); });
    fileMenu->addSeparator();
    fileMenu->addAction(uiText("Open configuration..."), this, [this]() { loadConfiguration(); });
    fileMenu->addAction(uiText("Save configuration"), this, [this]() { saveConfiguration(); });
    fileMenu->addAction(uiText("Save configuration as..."), this, [this]() { saveConfigurationAs(); });
    recentConfigMenu = fileMenu->addMenu(uiText("Open Recent Configuration"));
    fileMenu->addSeparator();
    fileMenu->addAction(uiText("Start selected"), this, [this]() { generateSelected(); });
    fileMenu->addAction(uiText("Start all"), this, [this]() { generateAll(); });
    fileMenu->addAction(uiText("Save selected contact sheet..."), this, [this]() { saveContactSheet(); });
    fileMenu->addSeparator();
    fileMenu->addAction(uiText("Exit"), this, [this]() { close(); });

    auto *editionMenu = menuBar()->addMenu(uiText("Edition"));
    editionMenu->addAction(uiText("Reload settings.ini"), this, [this]() { loadSettings(); });
    editionMenu->addAction(uiText("Save settings.ini"), this, [this]() { saveSettings(); });

    auto *viewMenu = menuBar()->addMenu(uiText("View"));
    viewMenu->addAction(uiText("Clear preview"), this, [this]() {
        currentSheet = QImage();
        preview->setPixmap(QPixmap());
        preview->setText(uiText("Preview area"));
    });

    auto *presetMenu = menuBar()->addMenu(uiText("Predefined Configuration"));

    auto applyOriginalPreset = [this](int rows, int columns) {
        rowsSpin->setValue(rows);
        columnsSpin->setValue(columns);
        if (speedModeCombo) {
            const int fastIndex = speedModeCombo->findData("fast");
            if (fastIndex >= 0) {
                speedModeCombo->setCurrentIndex(fastIndex);
            }
        }
        statusBar()->showMessage(uiText("Preset applied: %1 rows × %2 columns").arg(rows).arg(columns));
    };

    auto addOriginalPreset = [this, presetMenu, applyOriginalPreset](const QString &label, int rows, int columns) {
        QAction *action = presetMenu->addAction(label);
        action->setToolTip(uiText("%1 rows × %2 columns").arg(rows).arg(columns));
        connect(action, &QAction::triggered, this, [applyOriginalPreset, rows, columns]() {
            applyOriginalPreset(rows, columns);
        });
    };

    // Original Thumbnail me 3.0 predefined configurations.
    // The old menu text is interpreted as Rows × Columns.
    addOriginalPreset("1 x 2", 1, 2);
    addOriginalPreset("1 x 3", 1, 3);
    addOriginalPreset("1 x 4", 1, 4);
    addOriginalPreset("1 x 5", 1, 5);
    presetMenu->addSeparator();

    addOriginalPreset("2 x 2", 2, 2);
    addOriginalPreset("2 x 3", 2, 3);
    addOriginalPreset("2 x 4", 2, 4);
    addOriginalPreset("2 x 5", 2, 5);
    presetMenu->addSeparator();

    addOriginalPreset("3 x 2", 3, 2);
    addOriginalPreset("3 x 3", 3, 3);
    addOriginalPreset("3 x 4", 3, 4);
    addOriginalPreset("3 x 5", 3, 5);
    presetMenu->addSeparator();

    addOriginalPreset("2 x 1", 2, 1);
    addOriginalPreset("3 x 1", 3, 1);
    addOriginalPreset("4 x 1", 4, 1);
    addOriginalPreset("5 x 1", 5, 1);
    presetMenu->addSeparator();

    addOriginalPreset("2 x 2", 2, 2);
    addOriginalPreset("3 x 2", 3, 2);
    addOriginalPreset("4 x 2", 4, 2);
    addOriginalPreset("5 x 2", 5, 2);
    presetMenu->addSeparator();

    addOriginalPreset("2 x 3", 2, 3);
    addOriginalPreset("3 x 3", 3, 3);
    addOriginalPreset("4 x 3", 4, 3);
    addOriginalPreset("5 x 3", 5, 3);

    auto *aboutMenu = menuBar()->addMenu(uiText("About"));
    aboutMenu->addAction(uiText("About Thumbnail me 4b2"), this, [this]() {
        QMessageBox::information(
            this,
            "About Thumbnail me 4b2",
            "Thumbnail me 4b2\n\n"
            "Modern Qt6 / FFmpeg / MediaInfo continuation of the original GPL Thumbnail me concept.\n\n"
            "License: GPL-2.0-or-later\n"
            "Uses Qt, FFmpeg, MediaInfo and Qt Image Formats / libwebp.\n"
            "See the licenses folder in the Windows ZIP package for full third-party notices.\n\n"
            "Version 4b2 / beta 2."
        );
    });

    connect(addButton, &QPushButton::clicked, this, [this]() { addVideos(); });
    connect(removeButton, &QPushButton::clicked, this, [this]() { removeSelectedVideos(); });
    connect(clearButton, &QPushButton::clicked, this, [this]() { clearVideos(); });
    connect(browseButton, &QPushButton::clicked, this, [this]() { browseOutputFolder(); });
    connect(sameSourceFolderCheck, &QCheckBox::toggled, this, [this, browseButton](bool checked) {
        if (outputFolderEdit) {
            outputFolderEdit->setEnabled(!checked);
        }
        browseButton->setEnabled(!checked);
    });
    connect(skipSimilarFramesCheck, &QCheckBox::toggled, similarThresholdSpin, &QDoubleSpinBox::setEnabled);
    connect(randomizePositionsCheck, &QCheckBox::toggled, randomizePercentSpin, &QDoubleSpinBox::setEnabled);
    connect(specificTimestepCheck, &QCheckBox::toggled, timestepSpin, &QDoubleSpinBox::setEnabled);
    connect(showInfoTextCheck, &QCheckBox::toggled, infoTextPositionCombo, &QComboBox::setEnabled);
    similarThresholdSpin->setEnabled(skipSimilarFramesCheck->isChecked());
    randomizePercentSpin->setEnabled(randomizePositionsCheck->isChecked());
    timestepSpin->setEnabled(specificTimestepCheck->isChecked());
    infoTextPositionCombo->setEnabled(showInfoTextCheck->isChecked());

    connect(additionalTitleCheck, &QCheckBox::toggled, additionalTitleEdit, &QLineEdit::setEnabled);
    additionalTitleEdit->setEnabled(additionalTitleCheck->isChecked());

    connect(tileBorderCheck, &QCheckBox::toggled, tileBorderColorEdit, &QLineEdit::setEnabled);
    tileBorderColorEdit->setEnabled(tileBorderCheck->isChecked());

    connect(backgroundColorButton, &QPushButton::clicked, this, [this]() {
        const QColor initial = backgroundColor();
        const QColor selected = QColorDialog::getColor(
            initial.isValid() ? initial : QColor(20, 20, 20),
            this,
            "Choose background color"
        );

        if (selected.isValid() && backgroundColorEdit) {
            backgroundColorEdit->setText(selected.name(QColor::HexRgb));
        }
    });

    connect(fontButton, &QPushButton::clicked, this, [this]() { chooseFont(); });
    connect(generateButton, &QPushButton::clicked, this, [this]() { generateSelected(); });
    connect(generateAllButton, &QPushButton::clicked, this, [this]() { generateAll(); });
    connect(cancelBatchButton, &QPushButton::clicked, this, [this]() { cancelBatch(); });
    connect(previewStyleButton, &QPushButton::clicked, this, [this]() { previewStyle(); });
    connect(saveButton, &QPushButton::clicked, this, [this]() { saveContactSheet(); });
    connect(inputList, &QListWidget::itemDoubleClicked, this, [this]() { generateSelected(); });

    updateFontLabel();
    loadSettings();
    updateRecentConfigurationsMenu();
    updateWindowTitle();

    statusBar()->showMessage("Thumbnail me 4b2 ready");
}

QString MainWindow::settingsFilePath() const
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("settings.ini");
}

void MainWindow::loadSettings()
{
    const QString path = settingsFilePath();
    if (!QFileInfo::exists(path)) {
        currentLanguage = "en";
        updateFontLabel();
        updateRecentConfigurationsMenu();
        return;
    }

    QSettings settings(path, QSettings::IniFormat);
    applySettingsFrom(settings);

    updateFontLabel();
    updateRecentConfigurationsMenu();
    statusBar()->showMessage("Loaded: " + QDir::toNativeSeparators(path));
}

void MainWindow::saveSettings()
{
    const QString path = settingsFilePath();
    QSettings settings(path, QSettings::IniFormat);

    writeSettingsTo(settings, false);

    settings.sync();
    statusBar()->showMessage("Saved: " + QDir::toNativeSeparators(path));
    QMessageBox::information(this, uiText("Thumbnail me"), uiText("Settings saved:\n") + QDir::toNativeSeparators(path));
}

void MainWindow::applySettingsFrom(QSettings &settings)
{
    settings.beginGroup("thumbnails");
    columnsSpin->setValue(settings.value("columns", columnsSpin->value()).toInt());
    rowsSpin->setValue(settings.value("rows", rowsSpin->value()).toInt());
    thumbWidthSpin->setValue(settings.value("thumb_width", thumbWidthSpin->value()).toInt());
    gapSpin->setValue(settings.value("gap", gapSpin->value()).toInt());
    timestampsCheck->setChecked(settings.value("timestamps", timestampsCheck->isChecked()).toBool());
    if (speedModeCombo) {
        const QString seekMode = settings.value("seek_mode", "fast").toString();
        const int seekIndex = speedModeCombo->findData(seekMode);
        if (seekIndex >= 0) {
            speedModeCombo->setCurrentIndex(seekIndex);
        }
    }
    settings.endGroup();

    settings.beginGroup("frame_selection");
    if (startMarginSpin) {
        startMarginSpin->setValue(settings.value("start_margin_percent", startMarginSpin->value()).toDouble());
    }
    if (endMarginSpin) {
        endMarginSpin->setValue(settings.value("end_margin_percent", endMarginSpin->value()).toDouble());
    }
    if (skipDarkFramesCheck) {
        skipDarkFramesCheck->setChecked(settings.value("skip_dark_frames", false).toBool());
    }
    if (skipSimilarFramesCheck) {
        skipSimilarFramesCheck->setChecked(settings.value("skip_similar_frames", false).toBool());
    }
    if (similarThresholdSpin) {
        similarThresholdSpin->setValue(settings.value("similar_threshold_percent", similarThresholdSpin->value()).toDouble());
    }
    if (randomizePositionsCheck) {
        randomizePositionsCheck->setChecked(settings.value("randomize_positions", false).toBool());
    }
    if (randomizePercentSpin) {
        randomizePercentSpin->setValue(settings.value("randomize_percent", randomizePercentSpin->value()).toDouble());
    }
    if (specificTimestepCheck) {
        specificTimestepCheck->setChecked(settings.value("use_specific_timestep", false).toBool());
    }
    if (timestepSpin) {
        timestepSpin->setValue(settings.value("timestep_seconds", timestepSpin->value()).toDouble());
    }
    if (showInfoTextCheck) {
        showInfoTextCheck->setChecked(settings.value("show_info_text", true).toBool());
    }
    if (infoTextPositionCombo) {
        const QString infoPosition = settings.value("info_text_position", "upper-left").toString();
        const int infoPositionIndex = infoTextPositionCombo->findData(infoPosition);
        if (infoPositionIndex >= 0) {
            infoTextPositionCombo->setCurrentIndex(infoPositionIndex);
        }
    }
    if (timestampPositionCombo) {
        const QString tsPosition = settings.value("timestamp_position", "lower-left").toString();
        const int tsPositionIndex = timestampPositionCombo->findData(tsPosition);
        if (tsPositionIndex >= 0) {
            timestampPositionCombo->setCurrentIndex(tsPositionIndex);
        }
    }
    settings.endGroup();

    settings.beginGroup("output");
    outputFolderEdit->setText(settings.value("folder", outputFolderEdit->text()).toString());
    if (sameSourceFolderCheck) {
        sameSourceFolderCheck->setChecked(settings.value("same_source_folder", false).toBool());
    }
    if (filenameSuffixEdit) {
        filenameSuffixEdit->setText(settings.value("filename_suffix", filenameSuffixEdit->text()).toString());
    }

    const QString format = settings.value("format", currentOutputExtension()).toString();
    const int formatIndex = formatCombo->findData(format);
    if (formatIndex >= 0) {
        formatCombo->setCurrentIndex(formatIndex);
    }

    qualitySpin->setValue(settings.value("jpeg_quality", qualitySpin->value()).toInt());
    settings.endGroup();

    settings.beginGroup("appearance");
    QFont loadedFont;
    if (loadedFont.fromString(settings.value("font", selectedFont.toString()).toString())) {
        selectedFont = loadedFont;
    }

    if (additionalTitleCheck) {
        additionalTitleCheck->setChecked(settings.value("additional_title_enabled", true).toBool());
    }
    if (additionalTitleEdit) {
        additionalTitleEdit->setText(settings.value("additional_title_text", "Generated by Thumbnail me").toString());
    }
    if (backgroundColorEdit) {
        backgroundColorEdit->setText(settings.value("background_color", "#141414").toString());
    }
    if (titleTextColorEdit) {
        titleTextColorEdit->setText(settings.value("title_text_color", "#ffffff").toString());
    }
    if (infoTextColorEdit) {
        infoTextColorEdit->setText(settings.value("info_text_color", "#e6e6e6").toString());
    }
    if (countTextColorEdit) {
        countTextColorEdit->setText(settings.value("count_text_color", "#d2d2d2").toString());
    }
    if (timestampTextColorEdit) {
        timestampTextColorEdit->setText(settings.value("timestamp_text_color", "#ffffff").toString());
    }
    if (timestampBackgroundColorEdit) {
        timestampBackgroundColorEdit->setText(settings.value("timestamp_background_color", "#000000").toString());
    }
    if (tileBorderCheck) {
        tileBorderCheck->setChecked(settings.value("tile_border_enabled", true).toBool());
    }
    if (tileBorderColorEdit) {
        tileBorderColorEdit->setText(settings.value("tile_border_color", "#464646").toString());
    }

    const QString language = normalizedLanguage(settings.value("language", currentLanguage).toString());
    currentLanguage = language;
    if (languageCombo) {
        const int languageIndex = languageCombo->findData(language);
        if (languageIndex >= 0 && languageCombo->currentIndex() != languageIndex) {
            const bool oldBlocked = languageCombo->blockSignals(true);
            languageCombo->setCurrentIndex(languageIndex);
            languageCombo->blockSignals(oldBlocked);
        }
    }
    settings.endGroup();
}

void MainWindow::writeSettingsTo(QSettings &settings, bool includeProfileMetadata) const
{
    if (includeProfileMetadata) {
        settings.beginGroup("profile");
        settings.setValue("program", "thumbnailme4");
        settings.setValue("version", 4);
        settings.endGroup();
    }

    settings.beginGroup("thumbnails");
    settings.setValue("columns", columnsSpin->value());
    settings.setValue("rows", rowsSpin->value());
    settings.setValue("thumb_width", thumbWidthSpin->value());
    settings.setValue("gap", gapSpin->value());
    settings.setValue("timestamps", timestampsCheck->isChecked());
    settings.setValue("seek_mode", currentSeekMode());
    settings.endGroup();

    settings.beginGroup("frame_selection");
    settings.setValue("start_margin_percent", startMarginPercent());
    settings.setValue("end_margin_percent", endMarginPercent());
    settings.setValue("skip_dark_frames", skipDarkFrames());
    settings.setValue("skip_similar_frames", skipSimilarFrames());
    settings.setValue("similar_threshold_percent", similarThresholdPercent());
    settings.setValue("randomize_positions", randomizePositions());
    settings.setValue("randomize_percent", randomizePercent());
    settings.setValue("use_specific_timestep", useSpecificTimestep());
    settings.setValue("timestep_seconds", timestepSeconds());
    settings.setValue("show_info_text", showInfoText());
    settings.setValue("info_text_position", infoTextPosition());
    settings.setValue("timestamp_position", timestampPosition());
    settings.endGroup();

    settings.beginGroup("output");
    settings.setValue("folder", outputFolderEdit->text());
    settings.setValue("same_source_folder", sameSourceFolderCheck ? sameSourceFolderCheck->isChecked() : false);
    settings.setValue("filename_suffix", outputFilenameSuffix());
    settings.setValue("format", currentOutputExtension());
    settings.setValue("jpeg_quality", qualitySpin->value());
    settings.endGroup();

    settings.beginGroup("appearance");
    settings.setValue("font", selectedFont.toString());
    settings.setValue("additional_title_enabled", showAdditionalTitle());
    settings.setValue("additional_title_text", additionalTitleText());
    settings.setValue("background_color", backgroundColorName());
    settings.setValue("title_text_color", titleTextColorName());
    settings.setValue("info_text_color", infoTextColorName());
    settings.setValue("count_text_color", countTextColorName());
    settings.setValue("timestamp_text_color", timestampTextColorName());
    settings.setValue("timestamp_background_color", timestampBackgroundColorName());
    settings.setValue("tile_border_enabled", tileBorderEnabled());
    settings.setValue("tile_border_color", tileBorderColorName());
    if (languageCombo) {
        settings.setValue("language", normalizedLanguage(languageCombo->currentData().toString()));
    } else {
        settings.setValue("language", normalizedLanguage(currentLanguage));
    }
    settings.endGroup();
}

void MainWindow::loadConfiguration()
{
    const QString startFolder = recentConfigurations().isEmpty()
        ? QDir::homePath()
        : QFileInfo(recentConfigurations().first()).absolutePath();

    const QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open configuration",
        startFolder,
        "Thumbnail me 4 configuration (*.ini);;All INI files (*.ini);;All files (*.*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    loadConfigurationFromFile(fileName, true);
}

void MainWindow::saveConfiguration()
{
    if (currentConfigFile.isEmpty()) {
        saveConfigurationAs();
        return;
    }

    saveConfigurationToFile(currentConfigFile);
}

void MainWindow::saveConfigurationAs()
{
    QString startPath = QDir(QCoreApplication::applicationDirPath()).filePath("thumbnailme4.ini");
    if (!currentConfigFile.isEmpty()) {
        startPath = currentConfigFile;
    } else if (!recentConfigurations().isEmpty()) {
        startPath = recentConfigurations().first();
    }

    const QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save configuration as",
        startPath,
        "Thumbnail me 4 configuration (*.ini);;All INI files (*.ini);;All files (*.*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    QString finalName = fileName;
    if (QFileInfo(finalName).suffix().isEmpty()) {
        finalName += ".ini";
    }

    saveConfigurationToFile(finalName);
}

bool MainWindow::loadConfigurationFromFile(const QString &fileName, bool remember)
{
    if (fileName.isEmpty() || !QFileInfo::exists(fileName)) {
        QMessageBox::warning(this, "Thumbnail me", "Configuration file was not found.");
        return false;
    }

    QSettings settings(fileName, QSettings::IniFormat);
    const QString program = settings.value("profile/program").toString();
    if (!program.isEmpty() && program != "thumbnailme4") {
        QMessageBox::warning(
            this,
            "Thumbnail me",
            "This INI file is not a Thumbnail me 4 configuration."
        );
        return false;
    }

    applySettingsFrom(settings);
    currentConfigFile = fileName;
    updateFontLabel();
    updateWindowTitle();

    if (remember) {
        addRecentConfiguration(fileName);
        updateRecentConfigurationsMenu();
    }

    statusBar()->showMessage("Configuration loaded: " + QDir::toNativeSeparators(fileName));
    return true;
}

bool MainWindow::saveConfigurationToFile(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return false;
    }

    QSettings settings(fileName, QSettings::IniFormat);
    settings.clear();
    writeSettingsTo(settings, true);
    settings.sync();

    currentConfigFile = fileName;
    addRecentConfiguration(fileName);
    updateRecentConfigurationsMenu();
    updateWindowTitle();

    statusBar()->showMessage("Configuration saved: " + QDir::toNativeSeparators(fileName));
    return true;
}

QStringList MainWindow::recentConfigurations() const
{
    QSettings settings(settingsFilePath(), QSettings::IniFormat);
    settings.beginGroup("recent");
    QStringList files = settings.value("configurations").toStringList();
    settings.endGroup();

    QStringList result;
    for (const QString &fileName : files) {
        if (fileName.isEmpty()) {
            continue;
        }

        const QString nativeName = QDir::toNativeSeparators(fileName);
        if (!result.contains(nativeName, Qt::CaseInsensitive)) {
            result << nativeName;
        }
    }

    return result;
}

void MainWindow::addRecentConfiguration(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return;
    }

    const QString nativeName = QDir::toNativeSeparators(QFileInfo(fileName).absoluteFilePath());
    QStringList files = recentConfigurations();
    files.removeAll(nativeName);
    files.prepend(nativeName);

    while (files.size() > 4) {
        files.removeLast();
    }

    QSettings settings(settingsFilePath(), QSettings::IniFormat);
    settings.beginGroup("recent");
    settings.setValue("configurations", files);
    settings.endGroup();
    settings.sync();
}

void MainWindow::updateRecentConfigurationsMenu()
{
    if (!recentConfigMenu) {
        return;
    }

    recentConfigMenu->clear();

    const QStringList files = recentConfigurations();
    if (files.isEmpty()) {
        QAction *emptyAction = recentConfigMenu->addAction("(No recent configurations)");
        emptyAction->setEnabled(false);
        return;
    }

    for (const QString &fileName : files) {
        QAction *action = recentConfigMenu->addAction(QFileInfo(fileName).fileName());
        action->setToolTip(QDir::toNativeSeparators(fileName));
        connect(action, &QAction::triggered, this, [this, fileName]() {
            loadConfigurationFromFile(fileName, true);
        });
    }
}

void MainWindow::updateWindowTitle()
{
    QString title = "Thumbnail me 4b2";
    if (!currentConfigFile.isEmpty()) {
        title += " - " + QFileInfo(currentConfigFile).fileName();
    }
    setWindowTitle(title);
}

void MainWindow::chooseFont()
{
    bool ok = false;
    const QFont font = QFontDialog::getFont(&ok, selectedFont, this, "Choose output font");
    if (!ok) {
        return;
    }

    selectedFont = font;
    updateFontLabel();
}

void MainWindow::updateFontLabel()
{
    if (!fontPreviewLabel) {
        return;
    }

    fontPreviewLabel->setFont(selectedFont);
    fontPreviewLabel->setText(QString("%1, %2 pt")
        .arg(selectedFont.family())
        .arg(selectedFont.pointSize() > 0 ? selectedFont.pointSize() : 10));
}

bool MainWindow::containsInputFile(const QString &fileName) const
{
    if (!inputList) {
        return false;
    }

    const QString normalized = QDir::toNativeSeparators(fileName);
    for (int i = 0; i < inputList->count(); ++i) {
        QListWidgetItem *item = inputList->item(i);
        if (item && QDir::toNativeSeparators(item->data(Qt::UserRole).toString()) == normalized) {
            return true;
        }
    }

    return false;
}

QString MainWindow::uiText(const QString &english) const
{
    return translateUiText(currentLanguage, english);
}

QString MainWindow::normalizedLanguage(const QString &language) const
{
    const QString value = language.trimmed().toLower();

    if (value == "fr" || value == "french" || value == "français") return "fr";
    if (value == "de" || value == "german" || value == "deutsch") return "de";
    if (value == "es" || value == "spanish" || value == "español") return "es";
    if (value == "nl" || value == "dutch" || value == "nederlands") return "nl";
    if (value == "cz" || value == "cs" || value == "czech" || value == "česky" || value == "čeština" || value == "cesky" || value == "cestina") return "cs";
    if (value == "it" || value == "italian" || value == "italiano") return "it";
    if (value == "hu" || value == "hungarian" || value == "magyar") return "hu";
    if (value == "hr" || value == "croatian" || value == "hrvatski") return "hr";
    if (value == "pt" || value == "portuguese" || value == "português") return "pt";
    if (value == "cn" || value == "zh" || value == "chinese" || value == "中文") return "zh";
    if (value == "ru" || value == "russian" || value == "русский") return "ru";
    if (value == "bg" || value == "bulgarian" || value == "български") return "bg";
    if (value == "da" || value == "danish" || value == "dansk") return "da";
    if (value == "et" || value == "estonian" || value == "eesti") return "et";
    if (value == "el" || value == "greek" || value == "ελληνικά" || value == "ellinika") return "el";
    if (value == "ga" || value == "irish" || value == "gaeilge") return "ga";
    if (value == "lv" || value == "latvian" || value == "latviešu" || value == "latviesu") return "lv";
    if (value == "lt" || value == "lithuanian" || value == "lietuvių" || value == "lietuviu") return "lt";
    if (value == "mt" || value == "maltese" || value == "malti") return "mt";
    if (value == "pl" || value == "polish" || value == "polski") return "pl";
    if (value == "ro" || value == "romanian" || value == "română" || value == "romana") return "ro";
    if (value == "sk" || value == "slovak" || value == "slovenčina" || value == "slovencina") return "sk";
    if (value == "sl" || value == "slovenian" || value == "slovenščina" || value == "slovenscina") return "sl";
    if (value == "fi" || value == "finnish" || value == "suomi") return "fi";
    if (value == "sv" || value == "swedish" || value == "svenska") return "sv";
    if (value == "sq" || value == "albanian" || value == "shqip") return "sq";
    if (value == "be" || value == "belarusian" || value == "беларуская" || value == "bielaruskaja") return "be";
    if (value == "bs" || value == "bosnian" || value == "bosanski") return "bs";
    if (value == "sr" || value == "serbian" || value == "српски" || value == "srpski") return "sr";
    if (value == "mk" || value == "macedonian" || value == "македонски" || value == "makedonski") return "mk";
    if (value == "me" || value == "cnr" || value == "montenegrin" || value == "crnogorski") return "cnr";
    if (value == "is" || value == "icelandic" || value == "íslenska" || value == "islenska") return "is";
    if (value == "no" || value == "nb" || value == "nn" || value == "norwegian" || value == "norsk") return "no";
    if (value == "uk" || value == "ua" || value == "ukrainian" || value == "українська" || value == "ukrayinska") return "uk";
    if (value == "tr" || value == "turkish" || value == "türkçe" || value == "turkce") return "tr";
    if (value == "ca" || value == "catalan" || value == "català" || value == "catala") return "ca";
    if (value == "lb" || value == "luxembourgish" || value == "lëtzebuergesch" || value == "letzebuergesch") return "lb";
    if (value == "hy" || value == "armenian" || value == "հայերեն" || value == "hayeren") return "hy";
    if (value == "ka" || value == "georgian" || value == "ქართული" || value == "kartuli") return "ka";
    if (value == "az" || value == "azerbaijani" || value == "azərbaycanca" || value == "azerbaycanca") return "az";

    return "en";
}

QString MainWindow::savedLanguage() const
{
    QSettings settings(settingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("appearance");
    QString language = settings.value("language").toString();
    settings.endGroup();

    if (language.isEmpty()) {
        language = settings.value("language", "en").toString();
    }

    return normalizedLanguage(language);
}

void MainWindow::saveLanguageSetting(const QString &language)
{
    QSettings settings(settingsFilePath(), QSettings::IniFormat);
    settings.beginGroup("appearance");
    settings.setValue("language", normalizedLanguage(language));
    settings.endGroup();
    settings.sync();
}

void MainWindow::addVideos()
{
    const QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        uiText("Add video files"),
        QDir::homePath(),
        supportedVideoFilter()
    );

    if (fileNames.isEmpty()) {
        return;
    }

    int added = 0;
    for (const QString &fileName : fileNames) {
        if (fileName.isEmpty() || containsInputFile(fileName)) {
            continue;
        }

        auto *item = new QListWidgetItem(QFileInfo(fileName).fileName());
        item->setToolTip(QDir::toNativeSeparators(fileName));
        item->setData(Qt::UserRole, fileName);
        inputList->addItem(item);
        ++added;
    }

    if (inputList->count() > 0 && inputList->currentRow() < 0) {
        inputList->setCurrentRow(0);
    }

    statusBar()->showMessage(uiText("Added %1 video file(s)").arg(added));
}

void MainWindow::removeSelectedVideos()
{
    const QList<QListWidgetItem *> selected = inputList->selectedItems();
    for (QListWidgetItem *item : selected) {
        delete inputList->takeItem(inputList->row(item));
    }

    statusBar()->showMessage(uiText("Selected video file(s) removed"));
}

void MainWindow::clearVideos()
{
    inputList->clear();
    currentSheet = QImage();
    currentVideoFile.clear();
    preview->setText(uiText("Contact sheet preview"));
    statusBar()->showMessage(uiText("Input list cleared"));
}

void MainWindow::browseOutputFolder()
{
    const QString folder = QFileDialog::getExistingDirectory(
        this,
        uiText("Select output folder"),
        outputFolderEdit ? outputFolderEdit->text() : QDir::homePath()
    );

    if (!folder.isEmpty() && outputFolderEdit) {
        outputFolderEdit->setText(QDir::toNativeSeparators(folder));
    }
}

void MainWindow::generateSelected()
{
    QListWidgetItem *item = inputList->currentItem();
    if (!item) {
        QMessageBox::information(this, uiText("Thumbnail me"), uiText("Add and select a video first."));
        return;
    }

    const QString fileName = item->data(Qt::UserRole).toString();
    if (fileName.isEmpty()) {
        return;
    }

    cancelBatchRequested = false;
    generateForFile(fileName, true, false);
}

void MainWindow::cancelBatch()
{
    cancelBatchRequested = true;
    statusBar()->showMessage(uiText("Batch cancellation requested..."));
    if (output) {
        output->append(uiText("Cancellation requested. Waiting for the current worker to stop..."));
    }
}

void MainWindow::setQueueItemStatus(QListWidgetItem *item, const QString &status, const QString &detail)
{
    if (!item) {
        return;
    }

    const QString fileName = item->data(Qt::UserRole).toString();
    const QString shortName = QFileInfo(fileName).fileName();

    item->setText(QString("[%1] %2").arg(status, shortName));

    QString tooltip = QDir::toNativeSeparators(fileName);
    if (!detail.isEmpty()) {
        tooltip += "\n" + detail;
    }
    item->setToolTip(tooltip);
}

void MainWindow::generateAll()
{
    if (!inputList || inputList->count() <= 0) {
        QMessageBox::information(this, uiText("Thumbnail me"), uiText("Add video files first."));
        return;
    }

    const QMessageBox::StandardButton answer = QMessageBox::question(
        this,
        "Thumbnail me",
        QString("Generate and automatically save contact sheets for %1 file(s)?").arg(inputList->count()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes
    );

    if (answer != QMessageBox::Yes) {
        return;
    }

    cancelBatchRequested = false;

    generateButton->setEnabled(false);
    generateAllButton->setEnabled(false);
    saveButton->setEnabled(false);
    cancelBatchButton->setEnabled(true);
    QApplication::setOverrideCursor(Qt::BusyCursor);

    int okCount = 0;
    int failCount = 0;
    int cancelledCount = 0;

    for (int i = 0; i < inputList->count(); ++i) {
        setQueueItemStatus(inputList->item(i), "Waiting");
    }

    QString batchLog;
    batchLog += QString("Batch queue: %1 file(s)\n").arg(inputList->count());
    batchLog += QString("Output mode: %1\n").arg(sameSourceFolderCheck && sameSourceFolderCheck->isChecked()
        ? QString("same folder as source")
        : outputFolderPath());
    batchLog += "Statuses are also shown in the input list.\n\n";
    output->setPlainText(batchLog);

    for (int i = 0; i < inputList->count(); ++i) {
        QListWidgetItem *item = inputList->item(i);
        if (!item) {
            continue;
        }

        if (cancelBatchRequested) {
            setQueueItemStatus(item, "Cancelled");
            ++cancelledCount;
            continue;
        }

        inputList->setCurrentRow(i);

        const QString fileName = item->data(Qt::UserRole).toString();
        const QString shortName = QFileInfo(fileName).fileName();

        QString savedPath;
        QString workerMessage;

        setQueueItemStatus(item, "Processing");
        batchLog += QString("[%1/%2] Processing: %3\n")
            .arg(i + 1)
            .arg(inputList->count())
            .arg(shortName);

        output->setPlainText(batchLog);
        statusBar()->showMessage(
            QString("Batch %1/%2: %3").arg(i + 1).arg(inputList->count()).arg(shortName)
        );

        QApplication::processEvents();

        const bool ok = runWorkerForFile(fileName, workerMessage, &savedPath);

        if (cancelBatchRequested && !ok) {
            setQueueItemStatus(item, "Cancelled");
            ++cancelledCount;
            batchLog += "  CANCELLED\n";
        } else if (ok) {
            ++okCount;
            setQueueItemStatus(item, "Done", "Saved: " + QDir::toNativeSeparators(savedPath));
            batchLog += "  DONE\n";
            batchLog += "  Saved: " + QDir::toNativeSeparators(savedPath) + "\n";
        } else {
            ++failCount;
            setQueueItemStatus(item, "Failed", workerMessage.trimmed());
            batchLog += "  FAILED\n";
        }

        if (!workerMessage.isEmpty()) {
            batchLog += workerMessage.trimmed() + "\n";
        }

        batchLog += "\n";
        output->setPlainText(batchLog);
        QApplication::processEvents();
    }

    if (cancelBatchRequested) {
        batchLog += QString("Cancelled. Done: %1, Failed: %2, Cancelled/waiting: %3\n")
            .arg(okCount)
            .arg(failCount)
            .arg(cancelledCount);
    } else {
        batchLog += QString("Done. OK: %1, Failed: %2\n").arg(okCount).arg(failCount);
    }

    output->setPlainText(batchLog);

    QApplication::restoreOverrideCursor();
    generateButton->setEnabled(true);
    generateAllButton->setEnabled(true);
    saveButton->setEnabled(true);
    cancelBatchButton->setEnabled(false);

    statusBar()->showMessage(cancelBatchRequested
        ? QString("Batch cancelled. Done: %1, failed: %2").arg(okCount).arg(failCount)
        : QString("Batch finished. OK: %1, failed: %2").arg(okCount).arg(failCount));

    cancelBatchRequested = false;
}

bool MainWindow::runWorkerForFile(const QString &fileName, QString &message, QString *savedOutputPath)
{
    const QString program = QDir(QCoreApplication::applicationDirPath()).filePath("ThumbnailMeWorker.exe");
    const QString outputPath = automaticOutputPathFor(fileName);
    if (savedOutputPath) {
        *savedOutputPath = outputPath;
    }

    QStringList arguments;
    arguments << "--input" << fileName;
    arguments << "--output" << outputPath;
    arguments << "--columns" << QString::number(columnsSpin->value());
    arguments << "--rows" << QString::number(rowsSpin->value());
    arguments << "--thumb-width" << QString::number(thumbWidthSpin->value());
    arguments << "--gap" << QString::number(gapSpin->value());
    arguments << "--timestamps" << (timestampsCheck && timestampsCheck->isChecked() ? "1" : "0");
    arguments << "--quality" << QString::number(qualitySpin->value());
    arguments << "--font" << selectedFont.toString();
    arguments << "--seek-mode" << currentSeekMode();
    arguments << "--start-margin" << QString::number(startMarginPercent(), 'f', 1);
    arguments << "--end-margin" << QString::number(endMarginPercent(), 'f', 1);
    arguments << "--skip-dark" << (skipDarkFrames() ? "1" : "0");
    arguments << "--skip-similar" << (skipSimilarFrames() ? "1" : "0");
    arguments << "--similar-threshold" << QString::number(similarThresholdPercent(), 'f', 1);
    arguments << "--randomize" << (randomizePositions() ? "1" : "0");
    arguments << "--randomize-percent" << QString::number(randomizePercent(), 'f', 1);
    arguments << "--specific-timestep" << (useSpecificTimestep() ? "1" : "0");
    arguments << "--timestep" << QString::number(timestepSeconds(), 'f', 1);
    arguments << "--show-info" << (showInfoText() ? "1" : "0");
    arguments << "--info-position" << infoTextPosition();
    arguments << "--timestamp-position" << timestampPosition();
    arguments << "--additional-title-enabled" << (showAdditionalTitle() ? "1" : "0");
    arguments << "--additional-title" << additionalTitleText();
    arguments << "--background-color" << backgroundColorName();
    arguments << "--title-text-color" << titleTextColorName();
    arguments << "--info-text-color" << infoTextColorName();
    arguments << "--count-text-color" << countTextColorName();
    arguments << "--timestamp-text-color" << timestampTextColorName();
    arguments << "--timestamp-background-color" << timestampBackgroundColorName();
    arguments << "--tile-border" << (tileBorderEnabled() ? "1" : "0");
    arguments << "--tile-border-color" << tileBorderColorName();

    QProcess process;
    process.setProgram(program);
    process.setArguments(arguments);
    process.setProcessChannelMode(QProcess::MergedChannels);

    process.start();

    if (!process.waitForStarted(5000)) {
        message = QString("Could not start worker: %1").arg(QDir::toNativeSeparators(program));
        return false;
    }

    while (!process.waitForFinished(250)) {
        QApplication::processEvents();

        if (cancelBatchRequested) {
            process.kill();
            process.waitForFinished(3000);
            message = "Cancelled.";
            return false;
        }
    }

    const QString workerOutput = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();

    if (!workerOutput.isEmpty()) {
        message = workerOutput;
    }

    const bool ok =
        process.exitStatus() == QProcess::NormalExit &&
        process.exitCode() == 0 &&
        QFileInfo::exists(outputPath);

    if (ok) {
        if (!message.isEmpty()) {
            message += "\n";
        }
        message += "Saved: " + QDir::toNativeSeparators(outputPath);
    } else {
        if (!message.isEmpty()) {
            message += "\n";
        }
        message += QString("Worker failed. exitCode=%1").arg(process.exitCode());
    }

    return ok;
}

QString MainWindow::videoHeaderInfo(const QString &fileName, const VideoInfo &info) const
{
    return MediaInfoReader::summaryForFile(fileName, info);
}

bool MainWindow::generateForFile(const QString &fileName, bool autoSave, bool batchMode)
{
    const int columns = columnsSpin->value();
    const int rows = rowsSpin->value();
    const int count = columns * rows;
    const int thumbWidth = thumbWidthSpin->value();
    const int gap = gapSpin->value();
    const bool drawTimestamps = timestampsCheck ? timestampsCheck->isChecked() : true;

    if (count > 200 && !batchMode) {
        const QMessageBox::StandardButton answer = QMessageBox::question(
            this,
            "Thumbnail me",
            QString("This configuration creates %1 thumbnails. It may take a while. Continue?").arg(count),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

        if (answer != QMessageBox::Yes) {
            return false;
        }
    }

    const bool manageUi = !batchMode;

    if (manageUi) {
        generateButton->setEnabled(false);
        generateAllButton->setEnabled(false);
        saveButton->setEnabled(false);
        QApplication::setOverrideCursor(Qt::BusyCursor);
    }

    auto finish = [this, manageUi](bool result) {
        if (manageUi) {
            QApplication::restoreOverrideCursor();
            generateButton->setEnabled(true);
            generateAllButton->setEnabled(true);
            saveButton->setEnabled(true);
        }
        return result;
    };

    if (autoSave && !batchMode) {
        QString workerMessage;
        QString savedPath;

        statusBar()->showMessage("Running worker...");
        if (!output->toPlainText().isEmpty()) {
            output->setPlainText(QString("Generating with %1 seek mode...\n").arg(currentSeekMode()));
        }

        const bool ok = runWorkerForFile(fileName, workerMessage, &savedPath);

        if (!workerMessage.isEmpty()) {
            output->setPlainText(workerMessage);
        }

        if (ok) {
            currentVideoFile = fileName;
            currentSheet = QImage(savedPath);
            if (!currentSheet.isNull()) {
                showCurrentSheet();
            }
            statusBar()->showMessage("Contact sheet generated and saved");
        } else {
            preview->setText("Contact sheet failed");
            statusBar()->showMessage("Worker failed");
        }

        return finish(ok);
    }

    statusBar()->showMessage("Probing video...");
    const VideoInfo info = VideoProbe::probe(fileName);

    QString log;
    log += info.toText();

    log += "\n\nContact sheet settings:\n";
    log += QString("Columns / horizontal: %1\n").arg(columns);
    log += QString("Rows / vertical: %1\n").arg(rows);
    log += QString("Total thumbnails: %1\n").arg(count);
    log += QString("Thumbnail width: %1 px\n").arg(thumbWidth);
    log += QString("Gap: %1 px\n").arg(gap);
    log += QString("Timestamps: %1\n").arg(drawTimestamps ? "yes" : "no");
    log += QString("Start margin: %1 %%\n").arg(startMarginPercent(), 0, 'f', 1);
    log += QString("End margin: %1 %%\n").arg(endMarginPercent(), 0, 'f', 1);
    log += QString("Skip dark frames: %1\n").arg(skipDarkFrames() ? "yes" : "no");
    log += QString("Skip similar frames: %1\n").arg(skipSimilarFrames() ? QString("yes, %1 %").arg(similarThresholdPercent(), 0, 'f', 1) : QString("no"));
    log += QString("Randomize positions: %1\n").arg(randomizePositions() ? "yes" : "no");
    log += QString("Specific timestep: %1\n").arg(useSpecificTimestep() ? QString("%1 s").arg(timestepSeconds(), 0, 'f', 1) : QString("no"));
    log += QString("Info text: %1\n").arg(showInfoText() ? "yes" : "no");
    log += QString("Info text position: %1\n").arg(infoTextPosition());
    log += QString("Timestamp position: %1\n").arg(timestampPosition());
    log += QString("Additional title: %1\n").arg(showAdditionalTitle() ? additionalTitleText() : QString("off"));
    log += QString("Background color: %1\n").arg(backgroundColorName());
    log += QString("Title text color: %1\n").arg(titleTextColorName());
    log += QString("Info text color: %1\n").arg(infoTextColorName());
    log += QString("Timestamp text color: %1\n").arg(timestampTextColorName());
    log += QString("Tile border: %1\n").arg(tileBorderEnabled() ? tileBorderColorName() : QString("off"));
    log += QString("Format: %1\n").arg(currentOutputExtension().toUpper());
    log += QString("JPEG quality: %1 %%\n").arg(qualitySpin->value());
    log += QString("Font: %1, %2 pt\n")
        .arg(selectedFont.family())
        .arg(selectedFont.pointSize() > 0 ? selectedFont.pointSize() : 10);

    if (!info.ok || info.durationSeconds <= 0.0) {
        if (!batchMode) {
            output->setPlainText(log + "\n\nCould not determine video duration.");
            statusBar()->showMessage("Probe failed");
        }
        return finish(false);
    }

    QVector<double> times = ContactSheetComposer::makeEvenTimes(
        info.durationSeconds,
        count,
        startMarginPercent(),
        endMarginPercent(),
        randomizePositions(),
        randomizePercent(),
        useSpecificTimestep(),
        timestepSeconds()
    );
    QVector<QImage> frames;
    frames.reserve(times.size());

    log += "\nGenerating contact sheet...\n";
    if (!batchMode) {
        output->setPlainText(log);
    }

    for (int i = 0; i < times.size(); ++i) {
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

        statusBar()->showMessage(
            QString("Decoding frame %1/%2...").arg(i + 1).arg(times.size())
        );

        QImage frame;
        QString frameLog;

        const FrameGrabber::SeekMode frameSeekMode =
            currentSeekMode() == "accurate" ? FrameGrabber::SeekMode::Accurate : FrameGrabber::SeekMode::Fast;

        if (FrameGrabber::grabFrameAt(fileName, times[i], frame, frameLog, frameSeekMode)) {
            frames.append(frame);
            if (!batchMode) {
                log += QString("[%1/%2] %3 OK\n")
                    .arg(i + 1)
                    .arg(times.size())
                    .arg(secondsToTimeText(times[i]));
            }
        } else {
            if (!batchMode) {
                log += QString("[%1/%2] %3 FAILED: %4\n")
                    .arg(i + 1)
                    .arg(times.size())
                    .arg(secondsToTimeText(times[i]), frameLog);
            }
        }
    }

    if (frames.isEmpty()) {
        if (!batchMode) {
            output->setPlainText(log + "\nCould not decode any frame.");
            preview->setText("Contact sheet failed");
            statusBar()->showMessage("Failed");
        }
        return finish(false);
    }

    const QString title = QFileInfo(fileName).fileName();

    QImage generatedSheet = ContactSheetComposer::compose(
        frames,
        times,
        columns,
        rows,
        thumbWidth,
        gap,
        title,
        videoHeaderInfo(fileName, info),
        drawTimestamps,
        selectedFont,
        showInfoText(),
        infoTextPosition(),
        timestampPosition(),
        showAdditionalTitle(),
        additionalTitleText(),
        backgroundColor(),
        titleTextColor(),
        infoTextColor(),
        countTextColor(),
        timestampTextColor(),
        timestampBackgroundColor(),
        tileBorderEnabled(),
        tileBorderColor()
    );

    frames.clear();

    if (generatedSheet.isNull()) {
        if (!batchMode) {
            output->setPlainText(log + "\nCould not create contact sheet.");
            preview->setText("Contact sheet failed");
            statusBar()->showMessage("Failed");
        }
        return finish(false);
    }

    log += "\nContact sheet OK\n";
    log += QString("Output size: %1 × %2 px\n")
        .arg(generatedSheet.width())
        .arg(generatedSheet.height());

    if (autoSave) {
        const QString outputPath = automaticOutputPathFor(fileName);
        const QString extension = QFileInfo(outputPath).suffix().toLower();
        const int quality = extension == "png" ? -1 : (qualitySpin ? qualitySpin->value() : 95);

        QByteArray imageFormat = extension.toUtf8().toUpper();
        if (extension == "jpg" || extension == "jpeg") {
            imageFormat = "JPG";
        } else if (extension == "webp") {
            imageFormat = "WEBP";
        } else if (extension == "png") {
            imageFormat = "PNG";
        }

        QImageWriter writer(outputPath, imageFormat);
        if (quality >= 0) {
            writer.setQuality(quality);
        }

        if (writer.write(generatedSheet)) {
            log += QString("Automatically saved:\n%1\n").arg(QDir::toNativeSeparators(outputPath));
        } else {
            if (!batchMode) {
                log += QString("Automatic save failed:\n%1\n").arg(QDir::toNativeSeparators(outputPath));
                output->setPlainText(log);
                statusBar()->showMessage("Auto-save failed");
            }
            return finish(false);
        }

        if (!batchMode) {
            currentSheet = generatedSheet;
            currentVideoFile = fileName;
            showCurrentSheet();
        }
    } else {
        currentSheet = generatedSheet;
        currentVideoFile = fileName;
        showCurrentSheet();
        log += QString("Suggested output:\n%1\n").arg(suggestedOutputPath());
    }

    if (!batchMode) {
        output->setPlainText(log);
        statusBar()->showMessage(autoSave ? "Contact sheet generated and saved" : "Contact sheet ready");
    }

    return finish(true);
}

QString MainWindow::currentSeekMode() const
{
    if (!speedModeCombo) {
        return "fast";
    }

    const QString mode = speedModeCombo->currentData().toString().trimmed().toLower();
    return mode.isEmpty() ? QString("fast") : mode;
}

QString MainWindow::outputFilenameSuffix() const
{
    return filenameSuffixEdit ? filenameSuffixEdit->text() : QString();
}

QString MainWindow::sanitizedOutputFilenameSuffix() const
{
    QString suffix = outputFilenameSuffix();

    const QString invalidCharacters = "\\/:*?\"<>|";
    for (const QChar ch : invalidCharacters) {
        suffix.replace(ch, '_');
    }

    return suffix.trimmed();
}

QString MainWindow::outputFolderForVideo(const QString &videoFileName) const
{
    if (sameSourceFolderCheck && sameSourceFolderCheck->isChecked()) {
        const QString sourceFolder = QFileInfo(videoFileName).absolutePath();
        if (!sourceFolder.isEmpty()) {
            return sourceFolder;
        }
    }

    return outputFolderPath();
}

double MainWindow::startMarginPercent() const
{
    return startMarginSpin ? startMarginSpin->value() : 7.0;
}

double MainWindow::endMarginPercent() const
{
    return endMarginSpin ? endMarginSpin->value() : 7.0;
}

bool MainWindow::skipDarkFrames() const
{
    return skipDarkFramesCheck && skipDarkFramesCheck->isChecked();
}

bool MainWindow::skipSimilarFrames() const
{
    return skipSimilarFramesCheck && skipSimilarFramesCheck->isChecked();
}

double MainWindow::similarThresholdPercent() const
{
    return similarThresholdSpin ? similarThresholdSpin->value() : 92.0;
}

bool MainWindow::randomizePositions() const
{
    return randomizePositionsCheck && randomizePositionsCheck->isChecked();
}

double MainWindow::randomizePercent() const
{
    return randomizePercentSpin ? randomizePercentSpin->value() : 10.0;
}

bool MainWindow::useSpecificTimestep() const
{
    return specificTimestepCheck && specificTimestepCheck->isChecked();
}

double MainWindow::timestepSeconds() const
{
    return timestepSpin ? timestepSpin->value() : 60.0;
}

bool MainWindow::showInfoText() const
{
    return !showInfoTextCheck || showInfoTextCheck->isChecked();
}

QString MainWindow::infoTextPosition() const
{
    if (!infoTextPositionCombo) {
        return "upper-left";
    }

    const QString position = infoTextPositionCombo->currentData().toString().trimmed().toLower();
    if (position == "upper-right" || position == "lower-left" || position == "lower-right") {
        return position;
    }

    return "upper-left";
}

QString MainWindow::timestampPosition() const
{
    if (!timestampPositionCombo) {
        return "lower-left";
    }

    const QString position = timestampPositionCombo->currentData().toString().trimmed().toLower();
    if (position == "upper-left" || position == "upper-right" || position == "lower-right") {
        return position;
    }

    return "lower-left";
}

bool MainWindow::showAdditionalTitle() const
{
    return !additionalTitleCheck || additionalTitleCheck->isChecked();
}

QString MainWindow::additionalTitleText() const
{
    return additionalTitleEdit ? additionalTitleEdit->text() : QString("Generated by Thumbnail me");
}

QColor MainWindow::backgroundColor() const
{
    const QColor color(backgroundColorEdit ? backgroundColorEdit->text().trimmed() : QString("#141414"));
    return color.isValid() ? color : QColor(20, 20, 20);
}

QString MainWindow::backgroundColorName() const
{
    return backgroundColor().name(QColor::HexRgb);
}

QColor MainWindow::titleTextColor() const
{
    const QColor color(titleTextColorEdit ? titleTextColorEdit->text().trimmed() : QString("#ffffff"));
    return color.isValid() ? color : QColor(255, 255, 255);
}

QString MainWindow::titleTextColorName() const
{
    return titleTextColor().name(QColor::HexRgb);
}

QColor MainWindow::infoTextColor() const
{
    const QColor color(infoTextColorEdit ? infoTextColorEdit->text().trimmed() : QString("#e6e6e6"));
    return color.isValid() ? color : QColor(230, 230, 230);
}

QString MainWindow::infoTextColorName() const
{
    return infoTextColor().name(QColor::HexRgb);
}

QColor MainWindow::countTextColor() const
{
    const QColor color(countTextColorEdit ? countTextColorEdit->text().trimmed() : QString("#d2d2d2"));
    return color.isValid() ? color : QColor(210, 210, 210);
}

QString MainWindow::countTextColorName() const
{
    return countTextColor().name(QColor::HexRgb);
}

QColor MainWindow::timestampTextColor() const
{
    const QColor color(timestampTextColorEdit ? timestampTextColorEdit->text().trimmed() : QString("#ffffff"));
    return color.isValid() ? color : QColor(255, 255, 255);
}

QString MainWindow::timestampTextColorName() const
{
    return timestampTextColor().name(QColor::HexRgb);
}

QColor MainWindow::timestampBackgroundColor() const
{
    QColor color(timestampBackgroundColorEdit ? timestampBackgroundColorEdit->text().trimmed() : QString("#000000"));
    if (!color.isValid()) {
        color = QColor(0, 0, 0);
    }
    color.setAlpha(170);
    return color;
}

QString MainWindow::timestampBackgroundColorName() const
{
    QColor color = timestampBackgroundColor();
    color.setAlpha(255);
    return color.name(QColor::HexRgb);
}

bool MainWindow::tileBorderEnabled() const
{
    return !tileBorderCheck || tileBorderCheck->isChecked();
}

QColor MainWindow::tileBorderColor() const
{
    const QColor color(tileBorderColorEdit ? tileBorderColorEdit->text().trimmed() : QString("#464646"));
    return color.isValid() ? color : QColor(70, 70, 70);
}

QString MainWindow::tileBorderColorName() const
{
    return tileBorderColor().name(QColor::HexRgb);
}

QString MainWindow::currentOutputExtension() const
{
    if (!formatCombo) {
        return "jpg";
    }

    const QString extension = formatCombo->currentData().toString();
    return extension.isEmpty() ? QString("jpg") : extension;
}

QString MainWindow::outputFolderPath() const
{
    QString folder = outputFolderEdit ? outputFolderEdit->text().trimmed() : QString();

    if (folder.isEmpty()) {
        folder = QDir::homePath();
    }

    QDir dir(folder);

    if (dir.isRelative()) {
        dir = QDir(QCoreApplication::applicationDirPath()).filePath(folder);
    }

    const QString absolutePath = QDir::cleanPath(dir.absolutePath());

    QDir outputDir(absolutePath);
    if (!outputDir.exists()) {
        outputDir.mkpath(".");
    }

    return absolutePath;
}

QString MainWindow::suggestedOutputPath() const
{
    QString baseName = "contact_sheet";
    QString folder = outputFolderPath();

    if (!currentVideoFile.isEmpty()) {
        baseName = QFileInfo(currentVideoFile).completeBaseName();
        folder = outputFolderForVideo(currentVideoFile);
    }

    const QString extension = currentOutputExtension();
    return QDir(folder).filePath(baseName + sanitizedOutputFilenameSuffix() + "." + extension);
}

QString MainWindow::automaticOutputPathFor(const QString &videoFileName) const
{
    QDir dir(outputFolderForVideo(videoFileName));

    const QString baseName = QFileInfo(videoFileName).completeBaseName();
    const QString extension = currentOutputExtension();
    return uniqueFilePath(dir.filePath(baseName + sanitizedOutputFilenameSuffix() + "." + extension));
}

QString MainWindow::uniqueFilePath(const QString &filePath) const
{
    QFileInfo info(filePath);
    if (!info.exists()) {
        return filePath;
    }

    const QString folder = info.absolutePath();
    const QString baseName = info.completeBaseName();
    const QString suffix = info.suffix();

    for (int i = 1; i < 10000; ++i) {
        const QString candidate = QDir(folder).filePath(
            QString("%1_%2.%3").arg(baseName).arg(i).arg(suffix)
        );

        if (!QFileInfo::exists(candidate)) {
            return candidate;
        }
    }

    return filePath;
}


QImage MainWindow::createStylePreviewSheet() const
{
    const int columns = columnsSpin ? columnsSpin->value() : 4;
    const int rows = rowsSpin ? rowsSpin->value() : 4;
    const int count = qMax(1, columns * rows);
    const int thumbWidth = thumbWidthSpin ? thumbWidthSpin->value() : 320;
    const int gap = gapSpin ? gapSpin->value() : 8;

    QVector<QImage> frames;
    QVector<double> times;
    frames.reserve(count);
    times.reserve(count);

    const int thumbHeight = qMax(120, static_cast<int>(thumbWidth * 9.0 / 16.0));
    const QStringList labels = {
        "Scene A", "Scene B", "Close-up", "Night", "Action", "Dialogue",
        "Landscape", "Warm", "Cold", "Studio", "Outdoor", "Finale"
    };

    for (int i = 0; i < count; ++i) {
        QImage frame(thumbWidth, thumbHeight, QImage::Format_RGB32);
        QLinearGradient gradient(0, 0, thumbWidth, thumbHeight);

        const int hueA = (i * 29) % 360;
        const int hueB = (hueA + 45) % 360;
        gradient.setColorAt(0.0, QColor::fromHsv(hueA, 170, 95));
        gradient.setColorAt(1.0, QColor::fromHsv(hueB, 130, 210));

        QPainter painter(&frame);
        painter.fillRect(frame.rect(), gradient);

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QColor(255, 255, 255, 110));
        painter.drawEllipse(QPoint(thumbWidth / 2, thumbHeight / 2), thumbHeight / 4, thumbHeight / 4);

        painter.setPen(QColor(255, 255, 255, 185));
        QFont overlayFont = selectedFont;
        overlayFont.setPointSize(qMax(10, selectedFont.pointSize()));
        overlayFont.setBold(true);
        painter.setFont(overlayFont);
        painter.drawText(QRect(14, 12, thumbWidth - 28, 28), Qt::AlignLeft | Qt::AlignTop, labels[i % labels.size()]);

        QPen diagPen(QColor(255, 255, 255, 80));
        diagPen.setWidth(2);
        painter.setPen(diagPen);
        painter.drawLine(0, thumbHeight - 1, thumbWidth - 1, 0);

        painter.end();

        frames.append(frame);
        times.append(45.0 + (i * 37.0));
    }

    QString title = "Style preview";
    if (!currentVideoFile.trimmed().isEmpty()) {
        title = QFileInfo(currentVideoFile).fileName();
    } else if (inputList && inputList->currentItem()) {
        title = inputList->currentItem()->text();
    }

    const QString headerInfo = "Preview mode | no full video decoding | colors, font, text placement and borders";
    return ContactSheetComposer::compose(
        frames,
        times,
        columns,
        rows,
        thumbWidth,
        gap,
        title,
        headerInfo,
        timestampsCheck ? timestampsCheck->isChecked() : true,
        selectedFont,
        showInfoText(),
        infoTextPosition(),
        timestampPosition(),
        showAdditionalTitle(),
        additionalTitleText(),
        backgroundColor(),
        titleTextColor(),
        infoTextColor(),
        countTextColor(),
        timestampTextColor(),
        timestampBackgroundColor(),
        tileBorderEnabled(),
        tileBorderColor()
    );
}

void MainWindow::previewStyle()
{
    currentSheet = createStylePreviewSheet();

    if (currentSheet.isNull()) {
        statusBar()->showMessage(uiText("Style preview failed"));
        if (output) {
            output->append(uiText("Style preview failed") + ".");
        }
        return;
    }

    showCurrentSheet();
    statusBar()->showMessage(uiText("Style preview ready"));
    if (output) {
        output->append(QString("Style preview ready (%1 x %2, %3 thumbnails, format %4).")
            .arg(columnsSpin ? columnsSpin->value() : 4)
            .arg(rowsSpin ? rowsSpin->value() : 4)
            .arg((columnsSpin ? columnsSpin->value() : 4) * (rowsSpin ? rowsSpin->value() : 4))
            .arg(currentOutputExtension().toUpper()));
    }
}


void MainWindow::saveContactSheet()
{
    if (currentSheet.isNull()) {
        statusBar()->showMessage(uiText("No contact sheet to save"));
        QMessageBox::information(this, uiText("Thumbnail me"), uiText("Generate a contact sheet first."));
        return;
    }

    QString suggestedPath = suggestedOutputPath();
    const QString extension = currentOutputExtension();
    QString filter;
    if (extension == "png") {
        filter = "PNG image (*.png)";
    } else if (extension == "webp") {
        filter = "WebP image (*.webp)";
    } else {
        filter = "JPEG image (*.jpg *.jpeg)";
    }

    const QString fileName = QFileDialog::getSaveFileName(
        this,
        uiText("Save contact sheet"),
        suggestedPath,
        filter + ";;All files (*.*)"
    );

    if (fileName.isEmpty()) {
        return;
    }

    if (saveSheetToFile(fileName)) {
        statusBar()->showMessage(uiText("Saved: ") + fileName);
    } else {
        statusBar()->showMessage(uiText("Save failed"));
        QMessageBox::warning(this, uiText("Thumbnail me"), uiText("The file could not be saved."));
    }
}

bool MainWindow::saveSheetToFile(const QString &fileName)
{
    if (currentSheet.isNull() || fileName.isEmpty()) {
        return false;
    }

    const QString extension = QFileInfo(fileName).suffix().toLower();
    const int quality = extension == "png" ? -1 : (qualitySpin ? qualitySpin->value() : 95);

    QByteArray imageFormat = extension.toUtf8().toUpper();
    if (extension == "jpg" || extension == "jpeg") {
        imageFormat = "JPG";
    } else if (extension == "webp") {
        imageFormat = "WEBP";
    } else if (extension == "png") {
        imageFormat = "PNG";
    }

    QImageWriter writer(fileName, imageFormat);
    if (quality >= 0) {
        writer.setQuality(quality);
    }

    return writer.write(currentSheet);
}

void MainWindow::showCurrentSheet()
{
    if (currentSheet.isNull()) {
        return;
    }

    const QPixmap pixmap = QPixmap::fromImage(currentSheet);
    preview->setPixmap(
        pixmap.scaled(
            preview->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    showCurrentSheet();
}
