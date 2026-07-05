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
        if (language != "cz") {
            return english;
        }

        static const QHash<QString, QString> cs = {
            {"Thumbnail me", "Thumbnail me"},
            {"Thumbnail me 4", "Thumbnail me 4"},
            {"Thumbnail me 4b1", "Thumbnail me 4b1"},
            {"Input / Output", "Vstup / výstup"},
            {"Load", "Načíst"},
            {"Delete Selection", "Smazat výběr"},
            {"Reset", "Reset"},
            {"Config 1", "Konfig 1"},
            {"Config 2", "Konfig 2"},
            {"Config 3", "Konfig 3"},
            {"Output", "Výstup"},
            {"Styles", "Styl"},
            {"Columns:", "Sloupce:"},
            {"Rows:", "Řádky:"},
            {"Width:", "Šířka:"},
            {"Gap:", "Mezera:"},
            {"Timestamp:", "Čas:"},
            {"Seek mode:", "Režim seeku:"},
            {"Show timestamps", "Zobrazit časové značky"},
            {"Fast (Thumbnail me 3.0 style)", "Rychlý (styl Thumbnail me 3.0)"},
            {"Balanced (safer for short videos)", "Vyvážený (bezpečnější pro krátká videa)"},
            {"Accurate (slowest)", "Přesný (nejpomalejší)"},
            {"Start margin:", "Vynechat začátek:"},
            {"End margin:", "Vynechat konec:"},
            {"Skip dark:", "Přeskočit tmavé:"},
            {"Skip similar:", "Přeskočit podobné:"},
            {"Similarity threshold:", "Práh podobnosti:"},
            {"Randomize:", "Náhodně posunout:"},
            {"Randomize amount:", "Míra náhody:"},
            {"Specific timestep:", "Pevný časový krok:"},
            {"Timestep:", "Časový krok:"},
            {"Info text:", "Info text:"},
            {"Info position:", "Pozice info textu:"},
            {"Timestamp position:", "Pozice času:"},
            {"Skip dark / blank frames", "Přeskočit tmavé / prázdné snímky"},
            {"Skip similar frames", "Přeskočit podobné snímky"},
            {"Randomize positions", "Náhodně posunout pozice"},
            {"Use specific timestep", "Použít pevný časový krok"},
            {"Show technical info text", "Zobrazit technický info text"},
            {"Upper Left", "Vlevo nahoře"},
            {"Lower Left", "Vlevo dole"},
            {"Lower Right", "Vpravo dole"},
            {"Upper Right", "Vpravo nahoře"},
            {"Path Output:", "Výstupní cesta:"},
            {"Browse...", "Procházet..."},
            {"Same source folder:", "Stejná složka jako zdroj:"},
            {"Same folder as source video", "Stejná složka jako zdrojové video"},
            {"Output filename suffix:", "Přípona názvu výstupu:"},
            {"Output Format:", "Výstupní formát:"},
            {"Quality:", "Kvalita:"},
            {"JPEG (*.jpg)", "JPEG (*.jpg)"},
            {"PNG (*.png)", "PNG (*.png)"},
            {"WebP (*.webp)", "WebP (*.webp)"},
            {"Choose font...", "Vybrat font..."},
            {"Font:", "Font:"},
            {"Additional title:", "Doplňkový titulek:"},
            {"Show additional title", "Zobrazit doplňkový titulek"},
            {"Title text:", "Text titulku:"},
            {"Background color:", "Barva pozadí:"},
            {"Choose...", "Vybrat..."},
            {"Language:", "Jazyk:"},
            {"English", "English"},
            {"Czech", "Čeština"},
            {"Title text color:", "Barva titulku:"},
            {"Info text color:", "Barva info textu:"},
            {"Count text color:", "Barva počtu snímků:"},
            {"Timestamp text color:", "Barva času:"},
            {"Timestamp background:", "Pozadí času:"},
            {"Tile border:", "Rámeček snímku:"},
            {"Draw tile border", "Kreslit rámeček snímku"},
            {"Tile border color:", "Barva rámečku:"},
            {"Start selected", "Spustit vybrané"},
            {"Start all", "Spustit vše"},
            {"Cancel batch", "Zrušit dávku"},
            {"Preview style", "Náhled stylu"},
            {"Save manually", "Uložit ručně"},
            {"Preview area", "Oblast náhledu"},
            {"File", "Soubor"},
            {"Load video files...", "Načíst video soubory..."},
            {"Open configuration...", "Otevřít konfiguraci..."},
            {"Save configuration", "Uložit konfiguraci"},
            {"Save configuration as...", "Uložit konfiguraci jako..."},
            {"Open Recent Configuration", "Otevřít nedávnou konfiguraci"},
            {"Save selected contact sheet...", "Uložit vybraný contact sheet..."},
            {"Exit", "Konec"},
            {"Edition", "Úpravy"},
            {"Reload settings.ini", "Znovu načíst settings.ini"},
            {"Save settings.ini", "Uložit settings.ini"},
            {"View", "Zobrazení"},
            {"Clear preview", "Vyčistit náhled"},
            {"Predefined Configuration", "Předdefinovaná konfigurace"},
            {"About", "O aplikaci"},
            {"About Thumbnail me 4b1", "O Thumbnail me 4b1"},
            {"Load videos and click Start selected or Start all.", "Načti videa a klikni na Spustit vybrané nebo Spustit vše."},
            {"Language changed. Save settings and restart the application to fully apply it.", "Jazyk změněn. Ulož nastavení a restartuj aplikaci, aby se změna plně projevila."},
            {"Style preview ready", "Náhled stylu je připravený"},
            {"Style preview failed", "Náhled stylu selhal"},
            {"No contact sheet to save", "Není co uložit"},
            {"Generate a contact sheet first.", "Nejdřív vygeneruj contact sheet."},
            {"Save contact sheet", "Uložit contact sheet"},
            {"Save failed", "Uložení selhalo"},
            {"The file could not be saved.", "Soubor se nepodařilo uložit."},
            {"Saved: ", "Uloženo: "},
            {"Add video files first.", "Nejdřív přidej video soubory."},
            {"Batch cancellation requested...", "Požadavek na zrušení dávky..."},
            {"Cancellation requested. Waiting for the current worker to stop...", "Zrušení vyžádáno. Čekám na zastavení aktuálního workeru..."},
            {"Settings saved:\n", "Nastavení uloženo:\n"},
            {"Open configuration", "Otevřít konfiguraci"},
            {"Save configuration as", "Uložit konfiguraci jako"},
            {"Choose background color", "Vybrat barvu pozadí"},
            {"Choose title text color", "Vybrat barvu titulku"},
            {"Choose info text color", "Vybrat barvu info textu"},
            {"Choose count text color", "Vybrat barvu počtu snímků"},
            {"Choose timestamp text color", "Vybrat barvu času"},
            {"Choose timestamp background color", "Vybrat barvu pozadí času"},
            {"Choose tile border color", "Vybrat barvu rámečku"},
            {"Add video files", "Přidat video soubory"},
            {"Added %1 video file(s)", "Přidáno %1 video souborů"},
            {"Selected video file(s) removed", "Vybrané video soubory odstraněny"},
            {"Input list cleared", "Seznam vstupů vyčištěn"},
            {"Contact sheet preview", "Náhled contact sheetu"},
            {"Select output folder", "Vybrat výstupní složku"},
            {"Add and select a video first.", "Nejdřív přidej a vyber video."},
            {"Preset applied: %1 rows × %2 columns", "Preset použit: %1 řádků × %2 sloupců"},
            {"%1 rows × %2 columns", "%1 řádků × %2 sloupců"},
        };

        return cs.value(english, english);
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
    setWindowTitle("Thumbnail me 4b1");
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
    speedModeCombo->setToolTip("Fast is closest to the old mtn behavior. Accurate is safer for sparse keyframes, but slower.");

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

    showInfoTextCheck = new QCheckBox("Show information text", layoutTab2);
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
    languageCombo->addItem(T("English"), "en");
    languageCombo->addItem(T("Czech"), "cz");
    languageCombo->setToolTip("Language selection is saved immediately to settings.ini. Restart the application to fully apply static labels.");

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
        QString("Thumbnail me 4b1\n\n"
                "Milestone 12.7:\n"
                "- Czech/English UI translation foundation\n"
                "- WebP output and style preview preserved\n"
                "- settings.ini / MediaInfo / worker process preserved\n\n%1")
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
    aboutMenu->addAction(uiText("About Thumbnail me 4b1"), this, [this]() {
        QMessageBox::information(
            this,
            "About Thumbnail me 4b1",
            "Thumbnail me 4b1\n\n"
            "Modern Qt6 / FFmpeg / MediaInfo continuation of the original GPL Thumbnail me concept.\n\n"
            "License: GPL-2.0-or-later\n"
            "Uses Qt, FFmpeg, MediaInfo and Qt Image Formats / libwebp.\n"
            "See the licenses folder in the Windows ZIP package for full third-party notices.\n\n"
            "Version 4b1 / beta 1."
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

    statusBar()->showMessage("Thumbnail me 4b1 ready");
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
    QString title = "Thumbnail me 4b1";
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

    if (value == "cz" || value == "cs" || value == "czech" || value == "čeština") {
        return "cz";
    }

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
