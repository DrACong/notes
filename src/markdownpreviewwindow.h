// markdownpreviewwindow.h
#ifndef MARKDOWNPREVIEWWINDOW_H
#define MARKDOWNPREVIEWWINDOW_H

#include <QWidget>
#include <QTextBrowser>
#include <QSplitter>
#include <QTimer>
#include "editorsettingsoptions.h"

class MarkdownPreviewWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MarkdownPreviewWindow(QWidget *parent = nullptr);
    void setMarkdownText(const QString &text);
    void setTheme(Theme::Value theme);

private:
    QTextBrowser *m_previewBrowser;
    QTimer m_updateTimer;
    QString m_currentText;
    Theme::Value m_currentTheme;

    void updateStyleSheet();

    signals:
    void windowClosedorMinimized();  // Emitted when the window is closed (X button) and minimized
    void windowRestored();   // Emitted when restored from minimized state

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;  // Detect minimize/restore
};

#endif // MARKDOWNPREVIEWWINDOW_H