#ifndef CLOUD_CONTROLLER_SHELL_TASK_LOOP_H
#define CLOUD_CONTROLLER_SHELL_TASK_LOOP_H

#include <QMap>
#include <QPair>
#include <QString>

#include <termios.h>
#include <unistd.h>


#include "global/global.h"

QT_BEGIN_NAMESPACE
class QString;
class QChar;
QT_END_NAMESPACE

namespace cloudcontroller{
namespace shell{

class TaskLoop
{
public:
   enum class SpecialKeyName
   {
      HOME,PAGE_UP,PAGE_DOWN,DEL,END,
      ARROW_UP,ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT,
      ASCII_CODE,
      UNKNOW
   };
   
public:
   TaskLoop();
   void run();
   void updateTerminalWindowSize(int width, int height);
   ~TaskLoop();
protected:
   void setupTerminalAttr();
   void readCommand(QString& command);
   void readUnitCycle(QString& unit, SpecialKeyName keyType);
   SpecialKeyName getKeyTypeName(QString& unit);
protected:
   void arrowCommand(QString& unit, SpecialKeyName keyType);
   void asciiCommand(QString& unit, SpecialKeyName keyType);
   void delKeyCommand(QString& unit, SpecialKeyName keyType);
   void moveToBeginCommand(QString& unit, SpecialKeyName keyType);
   void moveToEndCommand(QString& unit, SpecialKeyName keyType);
protected:
   void removeCharAtCurrentCursorAction();
   void refreshLine();
   QPair<int, int> getCursorPos();
   void saveCycleBeginCursorPos();
   QPair<int, int> getCycleEndCursorPos();
   QPair<int, int> calculateCursorPosByInsertPointer();
   void filterBuffer(char* buffer, QString& ret);
   bool isMultiByteChar(const QChar& unicode);
   int getSpaceCountForChar(const QChar& unicode);
   int calculateCursorStep(const QString& unit);
   int calculateCursorStep(const QString &unit, int end);
   
protected:
   struct termios m_savedTerminalAttr;
   QString m_cmd_buff;
   static QMap<QString, SpecialKeyName> m_specialKeyMap;
   QString m_ps = "cloudcontroller >> ";
   
   int m_insertPos = 0;
   
   int m_cycleBeginX = 0;
   int m_cycleBeginY = 0;
   
   int m_windowWidth;
   int m_windowHeight;
   
   int m_ps_length;
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_LOOP_H