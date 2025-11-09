#ifndef HAVOC_ANSIPARSER_H
#define HAVOC_ANSIPARSER_H

#include <global.hpp>
#include <QString>
#include <QRegularExpression>

namespace HavocNamespace::Util
{
    class AnsiParser
    {
    public:
        // Convert ANSI escape codes to HTML formatting
        static QString ParseAnsiToHtml(const QString& text);
        
    private:
        // Map ANSI color codes to hex colors
        static QString AnsiColorToHex(int colorCode, bool bright = false, bool background = false);
        
        // Parse a single ANSI escape sequence
        static void ProcessAnsiSequence(const QString& sequence, QString& currentFgColor, 
                                       QString& currentBgColor, bool& isBold, bool& isUnderline);
        
        // Convert text to HTML-safe format while preserving newlines
        static QString ToHtmlWithNewlines(const QString& text);
    };
}

#endif // HAVOC_ANSIPARSER_H
