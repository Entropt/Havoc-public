#include <Util/AnsiParser.h>
#include <QRegularExpression>
#include <QStringList>

using namespace HavocNamespace::Util;

QString AnsiParser::AnsiColorToHex(int colorCode, bool bright, bool background)
{
    // Standard ANSI colors (0-7) and bright variants (8-15)
    static const QString fgColors[16] = {
        "#000000",  // Black
        "#800000",  // Red
        "#008000",  // Green
        "#808000",  // Yellow
        "#000080",  // Blue
        "#800080",  // Magenta
        "#008080",  // Cyan
        "#c0c0c0",  // White
        "#808080",  // Bright Black (Gray)
        "#ff0000",  // Bright Red
        "#00ff00",  // Bright Green
        "#ffff00",  // Bright Yellow
        "#0000ff",  // Bright Blue
        "#ff00ff",  // Bright Magenta
        "#00ffff",  // Bright Cyan
        "#ffffff"   // Bright White
    };
    
    static const QString bgColors[16] = {
        "#000000",  // Black
        "#800000",  // Red
        "#008000",  // Green
        "#808000",  // Yellow
        "#000080",  // Blue
        "#800080",  // Magenta
        "#008080",  // Cyan
        "#c0c0c0",  // White
        "#808080",  // Bright Black (Gray)
        "#ff0000",  // Bright Red
        "#00ff00",  // Bright Green
        "#ffff00",  // Bright Yellow
        "#0000ff",  // Bright Blue
        "#ff00ff",  // Bright Magenta
        "#00ffff",  // Bright Cyan
        "#ffffff"   // Bright White
    };
    
    int index = colorCode;
    if (bright && colorCode < 8) {
        index += 8;
    }
    
    if (index >= 0 && index < 16) {
        return background ? bgColors[index] : fgColors[index];
    }
    
    return background ? "#000000" : "#ffffff";
}

void AnsiParser::ProcessAnsiSequence(const QString& sequence, QString& currentFgColor, 
                                    QString& currentBgColor, bool& isBold, bool& isUnderline)
{
    // Extract numeric codes from the sequence
    QStringList codes = sequence.split(';');
    
    for (int i = 0; i < codes.size(); i++)
    {
        bool ok;
        int code = codes[i].toInt(&ok);
        if (!ok) continue;
        
        switch (code)
        {
            case 0:  // Reset
                currentFgColor = "";
                currentBgColor = "";
                isBold = false;
                isUnderline = false;
                break;
                
            case 1:  // Bold
                isBold = true;
                break;
                
            case 4:  // Underline
                isUnderline = true;
                break;
                
            case 22: // Normal intensity (not bold)
                isBold = false;
                break;
                
            case 24: // Not underlined
                isUnderline = false;
                break;
                
            // Foreground colors (30-37)
            case 30: currentFgColor = AnsiColorToHex(0, isBold); break; // Black
            case 31: currentFgColor = AnsiColorToHex(1, isBold); break; // Red
            case 32: currentFgColor = AnsiColorToHex(2, isBold); break; // Green
            case 33: currentFgColor = AnsiColorToHex(3, isBold); break; // Yellow
            case 34: currentFgColor = AnsiColorToHex(4, isBold); break; // Blue
            case 35: currentFgColor = AnsiColorToHex(5, isBold); break; // Magenta
            case 36: currentFgColor = AnsiColorToHex(6, isBold); break; // Cyan
            case 37: currentFgColor = AnsiColorToHex(7, isBold); break; // White
            
            // Background colors (40-47)
            case 40: currentBgColor = AnsiColorToHex(0, false, true); break; // Black
            case 41: currentBgColor = AnsiColorToHex(1, false, true); break; // Red
            case 42: currentBgColor = AnsiColorToHex(2, false, true); break; // Green
            case 43: currentBgColor = AnsiColorToHex(3, false, true); break; // Yellow
            case 44: currentBgColor = AnsiColorToHex(4, false, true); break; // Blue
            case 45: currentBgColor = AnsiColorToHex(5, false, true); break; // Magenta
            case 46: currentBgColor = AnsiColorToHex(6, false, true); break; // Cyan
            case 47: currentBgColor = AnsiColorToHex(7, false, true); break; // White
            
            // Bright foreground colors (90-97)
            case 90: currentFgColor = AnsiColorToHex(8); break;  // Bright Black
            case 91: currentFgColor = AnsiColorToHex(9); break;  // Bright Red
            case 92: currentFgColor = AnsiColorToHex(10); break; // Bright Green
            case 93: currentFgColor = AnsiColorToHex(11); break; // Bright Yellow
            case 94: currentFgColor = AnsiColorToHex(12); break; // Bright Blue
            case 95: currentFgColor = AnsiColorToHex(13); break; // Bright Magenta
            case 96: currentFgColor = AnsiColorToHex(14); break; // Bright Cyan
            case 97: currentFgColor = AnsiColorToHex(15); break; // Bright White
            
            // Bright background colors (100-107)
            case 100: currentBgColor = AnsiColorToHex(8, false, true); break;  // Bright Black
            case 101: currentBgColor = AnsiColorToHex(9, false, true); break;  // Bright Red
            case 102: currentBgColor = AnsiColorToHex(10, false, true); break; // Bright Green
            case 103: currentBgColor = AnsiColorToHex(11, false, true); break; // Bright Yellow
            case 104: currentBgColor = AnsiColorToHex(12, false, true); break; // Bright Blue
            case 105: currentBgColor = AnsiColorToHex(13, false, true); break; // Bright Magenta
            case 106: currentBgColor = AnsiColorToHex(14, false, true); break; // Bright Cyan
            case 107: currentBgColor = AnsiColorToHex(15, false, true); break; // Bright White
            
            // 256 color support (38;5;n for foreground, 48;5;n for background)
            case 38:
                if (i + 2 < codes.size() && codes[i + 1].toInt() == 5) {
                    int colorIndex = codes[i + 2].toInt(&ok);
                    if (ok && colorIndex < 16) {
                        currentFgColor = AnsiColorToHex(colorIndex);
                    }
                    i += 2;
                }
                break;
                
            case 48:
                if (i + 2 < codes.size() && codes[i + 1].toInt() == 5) {
                    int colorIndex = codes[i + 2].toInt(&ok);
                    if (ok && colorIndex < 16) {
                        currentBgColor = AnsiColorToHex(colorIndex, false, true);
                    }
                    i += 2;
                }
                break;
                
            case 39: // Default foreground color
                currentFgColor = "";
                break;
                
            case 49: // Default background color
                currentBgColor = "";
                break;
        }
    }
}

QString AnsiParser::ToHtmlWithNewlines(const QString& text)
{
    // First escape HTML entities
    QString escaped = text.toHtmlEscaped();
    
    // Replace consecutive spaces with non-breaking spaces to preserve formatting
    // Replace all spaces with non-breaking spaces to preserve indentation
    escaped.replace(" ", "&nbsp;");
    
    // Replace tabs with 4 non-breaking spaces
    escaped.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;");
    
    // Then replace newlines with <br> tags
    escaped.replace("\n", "<br>");
    
    return escaped;
}

QString AnsiParser::ParseAnsiToHtml(const QString& text)
{
    QString result;
    QString currentFgColor;
    QString currentBgColor;
    bool isBold = false;
    bool isUnderline = false;
    bool inSpan = false;
    
    // Regex to match ANSI escape sequences
    // Matches: ESC [ <codes> m
    QRegularExpression ansiRegex("\\x1b\\[([0-9;]*)m");
    
    int lastPos = 0;
    QRegularExpressionMatchIterator it = ansiRegex.globalMatch(text);
    
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        int matchPos = match.capturedStart();
        
        // Add text before this escape sequence
        if (matchPos > lastPos)
        {
            QString textSegment = text.mid(lastPos, matchPos - lastPos);
            
            // Close previous span if needed
            if (inSpan)
            {
                result += "</span>";
                inSpan = false;
            }
            
            // Open new span with current formatting
            if (!currentFgColor.isEmpty() || !currentBgColor.isEmpty() || isBold || isUnderline)
            {
                result += "<span style=\"";
                if (!currentFgColor.isEmpty())
                    result += "color: " + currentFgColor + ";";
                if (!currentBgColor.isEmpty())
                    result += "background-color: " + currentBgColor + ";";
                if (isBold)
                    result += "font-weight: bold;";
                if (isUnderline)
                    result += "text-decoration: underline;";
                result += "\">";
                inSpan = true;
            }
            
            result += ToHtmlWithNewlines(textSegment);
        }
        
        // Process the ANSI escape sequence
        QString sequence = match.captured(1);
        ProcessAnsiSequence(sequence, currentFgColor, currentBgColor, isBold, isUnderline);
        
        lastPos = match.capturedEnd();
    }
    
    // Add remaining text
    if (lastPos < text.length())
    {
        QString textSegment = text.mid(lastPos);
        
        // Close previous span if needed
        if (inSpan)
        {
            result += "</span>";
            inSpan = false;
        }
        
        // Open new span with current formatting
        if (!currentFgColor.isEmpty() || !currentBgColor.isEmpty() || isBold || isUnderline)
        {
            result += "<span style=\"";
            if (!currentFgColor.isEmpty())
                result += "color: " + currentFgColor + ";";
            if (!currentBgColor.isEmpty())
                result += "background-color: " + currentBgColor + ";";
            if (isBold)
                result += "font-weight: bold;";
            if (isUnderline)
                result += "text-decoration: underline;";
            result += "\">";
            inSpan = true;
        }
        
        result += ToHtmlWithNewlines(textSegment);
    }
    
    // Close any open span
    if (inSpan)
    {
        result += "</span>";
    }
    
    return result;
}
