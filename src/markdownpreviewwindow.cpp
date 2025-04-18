// markdownpreviewwindow.cpp
#include "markdownpreviewwindow.h"
#include <QHBoxLayout>
#include <QFile>
#include <QEvent>

MarkdownPreviewWindow::MarkdownPreviewWindow(QWidget *parent)
    : QWidget(parent),
      m_currentTheme(Theme::Light)
{
    setWindowTitle(tr("Markdown Preview"));
    resize(800, 600);
    
    auto *layout = new QHBoxLayout(this);
    m_previewBrowser = new QTextBrowser(this);
    m_previewBrowser->setOpenExternalLinks(true);
    m_previewBrowser->setOpenLinks(true);
    
    layout->addWidget(m_previewBrowser);
    setLayout(layout);
    
    // 设置样式表
    updateStyleSheet();
    
    // 使用定时器来避免频繁更新
    m_updateTimer.setInterval(300);
    m_updateTimer.setSingleShot(true);
    connect(&m_updateTimer, &QTimer::timeout, this, [this]() {
        m_previewBrowser->setMarkdown(m_currentText);
    });
}

void MarkdownPreviewWindow::closeEvent(QCloseEvent *event) {
    emit windowClosedorMinimized();
    QWidget::closeEvent(event);
}

void MarkdownPreviewWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            emit windowClosedorMinimized();  // Window was minimized
        } else if (isVisible()) {
            emit windowRestored();   // Window was restored
        }
    }
    QWidget::changeEvent(event);
}

void MarkdownPreviewWindow::setMarkdownText(const QString &text)
{
    m_currentText = text;
    m_updateTimer.start();
}

void MarkdownPreviewWindow::setTheme(Theme::Value theme)
{
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        updateStyleSheet();
    }
}

void MarkdownPreviewWindow::updateStyleSheet()
{
    QString styleSheet;
    QFile file;
    
    switch (m_currentTheme) {
    case Theme::Dark:
        file.setFileName(":/styles/markdown-dark.css");
        break;
    case Theme::Sepia:
        file.setFileName(":/styles/markdown-sepia.css");
        break;
    case Theme::Light:
    default:
        file.setFileName(":/styles/markdown-light.css");
        break;
    }
    
    if (file.open(QIODevice::ReadOnly)) {
        styleSheet = QString::fromUtf8(file.readAll());
        file.close();
    }
    
    m_previewBrowser->setStyleSheet(styleSheet);
}