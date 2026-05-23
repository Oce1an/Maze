#include "trainer.h"
#include <QStringList>
#include <QMap>
#include <Qt>

Trainer::Trainer()
    : m_pos(0), m_mistakes(0), m_correct(0)
{}

void Trainer::setText(const QString &text)
{
    m_text     = text;
    m_pos      = 0;
    m_mistakes = 0;
    m_correct  = 0;
    m_results.clear();
    m_lastError = false;
}

QString Trainer::text() const { return m_text; }

void Trainer::reset()
{
    m_pos      = 0;
    m_mistakes = 0;
    m_correct  = 0;
    m_results.clear();
    m_lastError = false;
}

bool Trainer::typeChar(QChar ch)
{
    if (m_pos >= m_text.length()) {
        m_lastError = false;
        return false;
    }

    bool ok = (ch == m_text.at(m_pos));

    if (ok) {
        m_results.append({m_text.at(m_pos), true});
        ++m_correct;
        ++m_pos;
        m_lastError = false;
    } else {
        ++m_mistakes;
        m_lastError = true;
    }

    return ok;
}

int  Trainer::position()     const { return m_pos; }
bool Trainer::finished()     const { return m_pos >= m_text.length(); }
int  Trainer::mistakeCount() const { return m_mistakes; }
int  Trainer::correctCount() const { return m_correct; }
bool Trainer::hasLastError() const { return m_lastError; }

double Trainer::accuracy() const
{
    int total = m_correct + m_mistakes;
    if (total == 0) return 100.0;
    return 100.0 * m_correct / total;
}

int Trainer::wordCount(const QString &s) const
{
    if (s.trimmed().isEmpty()) return 0;
    return s.split(' ', Qt::SkipEmptyParts).count();
}

double Trainer::wordsPerMinute(int elapsedMs) const
{
    if (elapsedMs <= 0) return 0.0;
    QString typed   = m_text.left(m_pos);
    double  words   = wordCount(typed);
    double  minutes = elapsedMs / 60000.0;
    return words / minutes;
}

QString Trainer::languageName(Language lang)
{
    switch (lang) {
    case Language::German:     return QStringLiteral("Немецкий");
    case Language::French:     return QStringLiteral("Французский");
    case Language::Arabic:     return QStringLiteral("Арабский");
    case Language::Chinese:    return QStringLiteral("Китайский");
    case Language::Belarusian: return QStringLiteral("Белорусский");
    case Language::Hebrew:     return QStringLiteral("Иврит");
    }
    return {};
}

QStringList Trainer::sampleText(Language lang)
{
    switch (lang) {
    case Language::German:
        return {
            "Das Buch liegt auf dem Tisch neben der Lampe."
        };
    case Language::French:
        return {
            "Le livre est sur la table a cote de la lampe."
        };
    case Language::Arabic:
        return {
            "\u0627\u0644\u0643\u062a\u0627\u0628 \u0639\u0644\u0649 \u0627\u0644\u0637\u0627\u0648\u0644\u0629."
        };
    case Language::Chinese:
        return {
            "\u4e66\u5728\u684c\u5b50\u4e0a\u7684\u706f\u65c1\u3002"
        };
    case Language::Belarusian:
        return {
            "\u041a\u043d\u0456\u0433\u0430 \u043b\u0430\u0436\u044b\u0446\u044c \u043d\u0430 \u0441\u0442\u0430\u043b\u0435 \u043f\u0430\u0440\u0430\u0434 \u043b\u044e\u0441\u0442\u0440\u0430\u043c."
        };
    case Language::Hebrew:
        return {
            "\u05d4\u05e1\u05e4\u05e8 \u05e0\u05de\u05e6\u05d0 \u05e2\u05dc \u05d4\u05e9\u05d5\u05dc\u05d7\u05df \u05dc\u05d9\u05d3 \u05d4\u05de\u05e0\u05d5\u05e8\u05d4."
        };
    }
    return {};
}

QList<QStringList> Trainer::keyboardLayout(Language lang)
{
    switch (lang) {
    case Language::German:
        return {
            {"^","1","2","3","4","5","6","7","8","9","0","ß","´","←"},
            {"Tab","q","w","e","r","t","z","u","i","o","p","ü","+"},
            {"Caps","a","s","d","f","g","h","j","k","l","ö","ä","#","↵"},
            {"Shift","<","y","x","c","v","b","n","m",",",".","-","Shift"},
            {"Strg","","Alt","Space","AltGr","","Strg"}
        };
    case Language::French:
        return {
            {"²","&","é","\"","'","(","§","è","!","ç","à",")","=","←"},
            {"Tab","a","z","e","r","t","y","u","i","o","p","^","$"},
            {"Caps","q","s","d","f","g","h","j","k","l","m","ù","*","↵"},
            {"Shift","<","w","x","c","v","b","n",",",";",":","!","Shift"},
            {"Ctrl","","Alt","Space","AltGr","","Ctrl"}
        };
    case Language::Arabic:
        return {
            {"ذ","1","2","3","4","5","6","7","8","9","0","-","=","←"},
            {"Tab","ض","ص","ث","ق","ف","غ","ع","ه","خ","ح","ج","د"},
            {"Caps","ش","س","ي","ب","ل","ا","ت","ن","م","ك","ط","ذ","↵"},
            {"Shift","ئ","ء","ؤ","ر","لا","ى","ة","و","ز","ظ","Shift"},
            {"Ctrl","","Alt","Space","AltGr","","Ctrl"}
        };
    case Language::Chinese:
        return {
            {"`","1","2","3","4","5","6","7","8","9","0","-","=","←"},
            {"Tab","q","w","e","r","t","y","u","i","o","p","[","]","\\"},
            {"Caps","a","s","d","f","g","h","j","k","l",";","'","↵"},
            {"Shift","z","x","c","v","b","n","m",",",".","/","Shift"},
            {"Ctrl","","Alt","Space","AltGr","","Ctrl"}
        };
    case Language::Belarusian:
        return {
            {"ё","1","2","3","4","5","6","7","8","9","0","-","=","←"},
            {"Tab","й","ц","у","к","е","н","г","ш","ў","з","х","'"},
            {"Caps","ф","ы","в","а","п","р","о","л","д","ж","э","↵"},
            {"Shift","\\","я","ч","с","м","і","т","ь","б","ю",".","Shift"},
            {"Ctrl","","Alt","Space","AltGr","","Ctrl"}
        };
    case Language::Hebrew:
        return {
            {"`","1","2","3","4","5","6","7","8","9","0","-","=","←"},
            {"Tab","/","'","ק","ר","א","ט","ו","ן","ם","פ","[","]"},
            {"Caps","ש","ד","ג","כ","ע","י","ח","ל","ך","ף",",","↵"},
            {"Shift","ז","ס","ב","ה","נ","מ","צ","ת","ץ",".","Shift"},
            {"Ctrl","","Alt","Space","AltGr","","Ctrl"}
        };
    }
    return {};
}

QMap<int, QString> Trainer::keyMapping(Language lang)
{
    QMap<int, QString> map;
    
    map[Qt::Key_0] = "0"; map[Qt::Key_1] = "1"; map[Qt::Key_2] = "2";
    map[Qt::Key_3] = "3"; map[Qt::Key_4] = "4"; map[Qt::Key_5] = "5";
    map[Qt::Key_6] = "6"; map[Qt::Key_7] = "7"; map[Qt::Key_8] = "8";
    map[Qt::Key_9] = "9";
    map[Qt::Key_Minus] = "-"; map[Qt::Key_Equal] = "=";
    map[Qt::Key_Backslash] = "\\";
    map[Qt::Key_Space] = " ";
    
    switch (lang) {
    case Language::German:
        map[Qt::Key_Q] = "q"; map[Qt::Key_W] = "w"; map[Qt::Key_E] = "e";
        map[Qt::Key_R] = "r"; map[Qt::Key_T] = "t"; map[Qt::Key_Z] = "z";
        map[Qt::Key_U] = "u"; map[Qt::Key_I] = "i"; map[Qt::Key_O] = "o";
        map[Qt::Key_P] = "p"; map[Qt::Key_A] = "a"; map[Qt::Key_S] = "s";
        map[Qt::Key_D] = "d"; map[Qt::Key_F] = "f"; map[Qt::Key_G] = "g";
        map[Qt::Key_H] = "h"; map[Qt::Key_J] = "j"; map[Qt::Key_K] = "k";
        map[Qt::Key_L] = "l"; map[Qt::Key_Y] = "y"; map[Qt::Key_X] = "x";
        map[Qt::Key_C] = "c"; map[Qt::Key_V] = "v"; map[Qt::Key_B] = "b";
        map[Qt::Key_N] = "n"; map[Qt::Key_M] = "m";

        map[Qt::Key_Udiaeresis] = QString::fromUtf8("ü");
        map[Qt::Key_Odiaeresis] = QString::fromUtf8("ö");
        map[Qt::Key_Adiaeresis] = QString::fromUtf8("ä");
        map[Qt::Key_ssharp] = QString::fromUtf8("ß");
        map[Qt::Key_BracketLeft] = QString::fromUtf8("ü");
        map[Qt::Key_BracketRight] = "+";
        map[Qt::Key_Semicolon] = QString::fromUtf8("ö");
        map[Qt::Key_Apostrophe] = QString::fromUtf8("ä");
        map[Qt::Key_Comma] = ",";
        map[Qt::Key_Period] = ".";
        map[Qt::Key_Slash] = "-";
        break;
        
    case Language::Belarusian:
        map[Qt::Key_Q] = QString::fromUtf8("й");
        map[Qt::Key_W] = QString::fromUtf8("ц");
        map[Qt::Key_E] = QString::fromUtf8("у");
        map[Qt::Key_R] = QString::fromUtf8("к");
        map[Qt::Key_T] = QString::fromUtf8("е");
        map[Qt::Key_Y] = QString::fromUtf8("н");
        map[Qt::Key_U] = QString::fromUtf8("г");
        map[Qt::Key_I] = QString::fromUtf8("ш");
        map[Qt::Key_O] = QString::fromUtf8("ў");
        map[Qt::Key_P] = QString::fromUtf8("з");
        map[Qt::Key_BracketLeft] = QString::fromUtf8("х");
        map[Qt::Key_BracketRight] = "'";
        map[Qt::Key_A] = QString::fromUtf8("ф");
        map[Qt::Key_S] = QString::fromUtf8("ы");
        map[Qt::Key_D] = QString::fromUtf8("в");
        map[Qt::Key_F] = QString::fromUtf8("а");
        map[Qt::Key_G] = QString::fromUtf8("п");
        map[Qt::Key_H] = QString::fromUtf8("р");
        map[Qt::Key_J] = QString::fromUtf8("о");
        map[Qt::Key_K] = QString::fromUtf8("л");
        map[Qt::Key_L] = QString::fromUtf8("д");
        map[Qt::Key_Semicolon] = QString::fromUtf8("ж");
        map[Qt::Key_Apostrophe] = QString::fromUtf8("э");
        map[Qt::Key_Z] = QString::fromUtf8("я");
        map[Qt::Key_X] = QString::fromUtf8("ч");
        map[Qt::Key_C] = QString::fromUtf8("с");
        map[Qt::Key_V] = QString::fromUtf8("м");
        map[Qt::Key_B] = QString::fromUtf8("і");
        map[Qt::Key_N] = QString::fromUtf8("т");
        map[Qt::Key_M] = QString::fromUtf8("ь");
        map[Qt::Key_Comma] = QString::fromUtf8("б");
        map[Qt::Key_Period] = QString::fromUtf8("ю");
        map[Qt::Key_Slash] = ".";
        map[Qt::Key_QuoteLeft] = QString::fromUtf8("ё");
        break;
        
    case Language::French:
        map[Qt::Key_A] = "a"; map[Qt::Key_Z] = "z"; map[Qt::Key_E] = "e";
        map[Qt::Key_R] = "r"; map[Qt::Key_T] = "t"; map[Qt::Key_Y] = "y";
        map[Qt::Key_U] = "u"; map[Qt::Key_I] = "i"; map[Qt::Key_O] = "o";
        map[Qt::Key_P] = "p"; map[Qt::Key_Q] = "q"; map[Qt::Key_S] = "s";
        map[Qt::Key_D] = "d"; map[Qt::Key_F] = "f"; map[Qt::Key_G] = "g";
        map[Qt::Key_H] = "h"; map[Qt::Key_J] = "j"; map[Qt::Key_K] = "k";
        map[Qt::Key_L] = "l"; map[Qt::Key_M] = "m";
        map[Qt::Key_W] = "w"; map[Qt::Key_X] = "x"; map[Qt::Key_C] = "c";
        map[Qt::Key_V] = "v"; map[Qt::Key_B] = "b"; map[Qt::Key_N] = "n";
        map[Qt::Key_Comma] = ",";
        map[Qt::Key_Period] = ".";
        map[Qt::Key_Semicolon] = "m";
        map[Qt::Key_Apostrophe] = QString::fromUtf8("ù");
        break;
        
    case Language::Hebrew:
        map[Qt::Key_Q] = "/"; map[Qt::Key_W] = "'"; 
        map[Qt::Key_E] = QString::fromUtf8("ק");
        map[Qt::Key_R] = QString::fromUtf8("ר");
        map[Qt::Key_T] = QString::fromUtf8("א");
        map[Qt::Key_Y] = QString::fromUtf8("ט");
        map[Qt::Key_U] = QString::fromUtf8("ו");
        map[Qt::Key_I] = QString::fromUtf8("ן");
        map[Qt::Key_O] = QString::fromUtf8("ם");
        map[Qt::Key_P] = QString::fromUtf8("פ");
        map[Qt::Key_A] = QString::fromUtf8("ש");
        map[Qt::Key_S] = QString::fromUtf8("ד");
        map[Qt::Key_D] = QString::fromUtf8("ג");
        map[Qt::Key_F] = QString::fromUtf8("כ");
        map[Qt::Key_G] = QString::fromUtf8("ע");
        map[Qt::Key_H] = QString::fromUtf8("י");
        map[Qt::Key_J] = QString::fromUtf8("ח");
        map[Qt::Key_K] = QString::fromUtf8("ל");
        map[Qt::Key_L] = QString::fromUtf8("ך");
        map[Qt::Key_Semicolon] = QString::fromUtf8("ף");
        map[Qt::Key_Comma] = QString::fromUtf8("ת");
        map[Qt::Key_Z] = QString::fromUtf8("ז");
        map[Qt::Key_X] = QString::fromUtf8("ס");
        map[Qt::Key_C] = QString::fromUtf8("ב");
        map[Qt::Key_V] = QString::fromUtf8("ה");
        map[Qt::Key_B] = QString::fromUtf8("נ");
        map[Qt::Key_N] = QString::fromUtf8("מ");
        map[Qt::Key_M] = QString::fromUtf8("צ");
        map[Qt::Key_Period] = QString::fromUtf8("ץ");
        break;
        
    case Language::Arabic:
        map[Qt::Key_Q] = QString::fromUtf8("ض");
        map[Qt::Key_W] = QString::fromUtf8("ص");
        map[Qt::Key_E] = QString::fromUtf8("ث");
        map[Qt::Key_R] = QString::fromUtf8("ق");
        map[Qt::Key_T] = QString::fromUtf8("ف");
        map[Qt::Key_Y] = QString::fromUtf8("غ");
        map[Qt::Key_U] = QString::fromUtf8("ع");
        map[Qt::Key_I] = QString::fromUtf8("ه");
        map[Qt::Key_O] = QString::fromUtf8("خ");
        map[Qt::Key_P] = QString::fromUtf8("ح");
        map[Qt::Key_BracketLeft] = QString::fromUtf8("ج");
        map[Qt::Key_BracketRight] = QString::fromUtf8("د");
        map[Qt::Key_A] = QString::fromUtf8("ش");
        map[Qt::Key_S] = QString::fromUtf8("س");
        map[Qt::Key_D] = QString::fromUtf8("ي");
        map[Qt::Key_F] = QString::fromUtf8("ب");
        map[Qt::Key_G] = QString::fromUtf8("ل");
        map[Qt::Key_H] = QString::fromUtf8("ا");
        map[Qt::Key_J] = QString::fromUtf8("ت");
        map[Qt::Key_K] = QString::fromUtf8("ن");
        map[Qt::Key_L] = QString::fromUtf8("م");
        map[Qt::Key_Semicolon] = QString::fromUtf8("ك");
        map[Qt::Key_Apostrophe] = QString::fromUtf8("ط");
        map[Qt::Key_Z] = QString::fromUtf8("ئ");
        map[Qt::Key_X] = QString::fromUtf8("ء");
        map[Qt::Key_C] = QString::fromUtf8("ؤ");
        map[Qt::Key_V] = QString::fromUtf8("ر");
        map[Qt::Key_B] = QString::fromUtf8("لا");
        map[Qt::Key_N] = QString::fromUtf8("ى");
        map[Qt::Key_M] = QString::fromUtf8("ة");
        map[Qt::Key_Comma] = QString::fromUtf8("و");
        map[Qt::Key_Period] = QString::fromUtf8("ز");
        map[Qt::Key_Slash] = QString::fromUtf8("ظ");
        break;
        
    case Language::Chinese:
        for (int key = Qt::Key_A; key <= Qt::Key_Z; ++key) {
            map[key] = QChar(key).toLower();
        }
        map[Qt::Key_Comma] = ",";
        map[Qt::Key_Period] = ".";
        map[Qt::Key_Slash] = "/";
        map[Qt::Key_Semicolon] = ";";
        map[Qt::Key_Apostrophe] = "'";
        map[Qt::Key_BracketLeft] = "[";
        map[Qt::Key_BracketRight] = "]";
        break;
    }
    
    return map;
}