// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "VideoTypes.h"

#include <QColor>
#include <QFont>
#include <QImage>
#include <QLabel>
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QTextEdit>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
class QPushButton;
class QResizeEvent;
class QSettings;
class QSpinBox;

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:
    QString uiText(const QString &english) const;
    QString savedLanguage() const;
    QString normalizedLanguage(const QString &language) const;
    void saveLanguageSetting(const QString &language);

    void addVideos();
    void removeSelectedVideos();
    void clearVideos();
    void browseOutputFolder();
    void chooseFont();
    void updateFontLabel();
    void previewStyle();
    QImage createStylePreviewSheet() const;

    void generateSelected();
    void generateAll();
    void cancelBatch();
    void setQueueItemStatus(QListWidgetItem *item, const QString &status, const QString &detail = QString());
    bool generateForFile(const QString &fileName, bool autoSave, bool batchMode = false);
    bool runWorkerForFile(const QString &fileName, QString &message, QString *savedOutputPath = nullptr);

    void saveContactSheet();
    bool saveSheetToFile(const QString &fileName);
    void showCurrentSheet();
    void resizeEvent(QResizeEvent *event) override;

    void loadSettings();
    void saveSettings();
    void applySettingsFrom(QSettings &settings);
    void writeSettingsTo(QSettings &settings, bool includeProfileMetadata) const;

    void loadConfiguration();
    void saveConfiguration();
    void saveConfigurationAs();
    bool loadConfigurationFromFile(const QString &fileName, bool remember);
    bool saveConfigurationToFile(const QString &fileName);
    void updateRecentConfigurationsMenu();
    QStringList recentConfigurations() const;
    void addRecentConfiguration(const QString &fileName);
    void updateWindowTitle();

    QString settingsFilePath() const;

    QString currentSeekMode() const;
    double startMarginPercent() const;
    double endMarginPercent() const;
    bool skipDarkFrames() const;
    bool skipSimilarFrames() const;
    double similarThresholdPercent() const;
    bool randomizePositions() const;
    double randomizePercent() const;
    bool useSpecificTimestep() const;
    double timestepSeconds() const;
    bool showInfoText() const;
    QString infoTextPosition() const;
    QString timestampPosition() const;
    bool showAdditionalTitle() const;
    QString additionalTitleText() const;
    QColor backgroundColor() const;
    QString backgroundColorName() const;
    QColor titleTextColor() const;
    QString titleTextColorName() const;
    QColor infoTextColor() const;
    QString infoTextColorName() const;
    QColor countTextColor() const;
    QString countTextColorName() const;
    QColor timestampTextColor() const;
    QString timestampTextColorName() const;
    QColor timestampBackgroundColor() const;
    QString timestampBackgroundColorName() const;
    bool tileBorderEnabled() const;
    QColor tileBorderColor() const;
    QString tileBorderColorName() const;
    QString currentOutputExtension() const;
    QString outputFilenameSuffix() const;
    QString sanitizedOutputFilenameSuffix() const;
    QString outputFolderForVideo(const QString &videoFileName) const;
    QString outputFolderPath() const;
    QString suggestedOutputPath() const;
    QString automaticOutputPathFor(const QString &videoFileName) const;
    QString uniqueFilePath(const QString &filePath) const;
    QString videoHeaderInfo(const QString &fileName, const VideoInfo &info) const;
    bool containsInputFile(const QString &fileName) const;

private:
    QListWidget *inputList = nullptr;
    QTextEdit *output = nullptr;
    QLabel *preview = nullptr;

    QSpinBox *columnsSpin = nullptr;
    QSpinBox *rowsSpin = nullptr;
    QSpinBox *thumbWidthSpin = nullptr;
    QSpinBox *gapSpin = nullptr;
    QSpinBox *qualitySpin = nullptr;
    QComboBox *formatCombo = nullptr;
    QComboBox *speedModeCombo = nullptr;
    QComboBox *languageCombo = nullptr;
    QCheckBox *timestampsCheck = nullptr;
    QDoubleSpinBox *startMarginSpin = nullptr;
    QDoubleSpinBox *endMarginSpin = nullptr;
    QCheckBox *skipDarkFramesCheck = nullptr;
    QCheckBox *skipSimilarFramesCheck = nullptr;
    QDoubleSpinBox *similarThresholdSpin = nullptr;
    QCheckBox *randomizePositionsCheck = nullptr;
    QDoubleSpinBox *randomizePercentSpin = nullptr;
    QCheckBox *specificTimestepCheck = nullptr;
    QDoubleSpinBox *timestepSpin = nullptr;
    QCheckBox *showInfoTextCheck = nullptr;
    QComboBox *infoTextPositionCombo = nullptr;
    QComboBox *timestampPositionCombo = nullptr;
    QCheckBox *sameSourceFolderCheck = nullptr;
    QLineEdit *outputFolderEdit = nullptr;
    QLineEdit *filenameSuffixEdit = nullptr;
    QCheckBox *additionalTitleCheck = nullptr;
    QLineEdit *additionalTitleEdit = nullptr;
    QLineEdit *backgroundColorEdit = nullptr;
    QLineEdit *titleTextColorEdit = nullptr;
    QLineEdit *infoTextColorEdit = nullptr;
    QLineEdit *countTextColorEdit = nullptr;
    QLineEdit *timestampTextColorEdit = nullptr;
    QLineEdit *timestampBackgroundColorEdit = nullptr;
    QCheckBox *tileBorderCheck = nullptr;
    QLineEdit *tileBorderColorEdit = nullptr;
    QPushButton *generateButton = nullptr;
    QPushButton *generateAllButton = nullptr;
    QPushButton *cancelBatchButton = nullptr;
    QPushButton *previewStyleButton = nullptr;
    QPushButton *saveButton = nullptr;
    QLabel *fontPreviewLabel = nullptr;
    QMenu *recentConfigMenu = nullptr;

    QImage currentSheet;
    QString currentConfigFile;
    QString currentVideoFile;
    QFont selectedFont;
    QString currentLanguage = "en";
    bool cancelBatchRequested = false;
};
