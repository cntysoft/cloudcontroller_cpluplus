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

class AbstractTaskContainer;

class TaskLoop
{
   Q_DISABLE_COPY(TaskLoop)
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
   void runCommand(const QString& command);
   void updateTerminalWindowSize(int width, int height);
   AbstractTaskContainer& getCurrentTaskContainer();
   const QString& getConsolePsText();
   TaskLoop& setConsolePsText(const QString& psText);
   ~TaskLoop();
public:
   TaskLoop& enterTaskContainer(const QString& name);
   TaskLoop& enterGlobalTaskContainer();
protected:
   //命令容器相关的
   void initCommandContainer();
protected:
   void setupTerminalAttr();
   void readCommand(QString& command);
   void readUnitCycle(QString& unit, SpecialKeyName keyType);
   SpecialKeyName getKeyTypeName(QString& unit);
protected:
   //逻辑处理
   void arrowCommand(QString& unit, SpecialKeyName keyType);
   void asciiCommand(QString& unit, SpecialKeyName keyType);
   void delKeyCommand(QString& unit, SpecialKeyName keyType);
   void moveToBeginCommand(QString& unit, SpecialKeyName keyType);
   void moveToEndCommand(QString& unit, SpecialKeyName keyType);
protected:
   //命令识别
   void removeCharAtCurrentCursorAction();
   void refreshLine(int startPointer);
   QPair<int, int> getCursorPos();
   void saveCycleBeginCursorPos();
   QPair<int, int> getCycleEndCursorPos();
   QPair<int, int> calculateCursorPosByInsertPointer(int insertPointer);
   QPair<int, int> calculateCursorPosByInsertPointer();
   void filterBuffer(char* buffer, QString& ret);
   bool isMultiByteChar(const QChar& unicode);
   int getSpaceCountForChar(const QChar& unicode);
   int calculateCursorStep(const QString& unit);
   int calculateCursorStep(const QString &unit, int end);
protected:
   AbstractTaskContainer* m_currentTaskContainer = nullptr;
   QMap<QString, AbstractTaskContainer*> m_taskContainerPool;
protected:
   //底层命令识别部分
   struct termios m_savedTerminalAttr;
   QString m_cmdBuff;
   static QMap<QString, SpecialKeyName> m_specialKeyMap;
   QString m_ps;
   int m_insertPos = 0;
   int m_cycleBeginX = 0;
   int m_cycleBeginY = 0;
   int m_windowWidth;
   int m_windowHeight;
   int m_psLength;
};



}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_LOOP_H