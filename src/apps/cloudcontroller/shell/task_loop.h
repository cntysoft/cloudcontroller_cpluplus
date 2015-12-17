#ifndef CLOUD_CONTROLLER_SHELL_TASK_LOOP_H
#define CLOUD_CONTROLLER_SHELL_TASK_LOOP_H

#include <QMap>
#include <QByteArray>
#include <QPair>

#include <termios.h>
#include <unistd.h>


#include "global/global.h"

QT_BEGIN_NAMESPACE
class QString;
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
   void readUnitCycle(QByteArray& unit, SpecialKeyName keyType);
   SpecialKeyName getKeyTypeName(QByteArray& unit);
protected:
   void arrowCommand(QByteArray& unit, SpecialKeyName keyType);
   void asciiCommand(QByteArray& unit, SpecialKeyName keyType);
   void refreshLine();
   QPair<int, int> getCursorPos();
   void saveCycleBeginCursorPos();
   QPair<int, int> getCycleEndCursorPos();
protected:
   struct termios m_savedTerminalAttr;
   QString m_cmd_buff;
   static QMap<QByteArray, SpecialKeyName> m_specialKeyMap;
   QString m_ps = "cloudcontroller >> ";
   
   int m_insertPos = 0;
   
   int m_cycleBeginX = 0;
   int m_cycleBeginY = 0;
   
   int m_windowWidth;
   int m_windowHeight;
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_LOOP_H