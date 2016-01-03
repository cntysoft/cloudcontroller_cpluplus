#include <QString>
#include <QByteArray>
#include <QPair>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QList>
#include <QChar>
#include <QThread>
#include <QTextStream>
#include <QDebug>

#include <iostream>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>

#include "corelib/io/terminal.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/command/history.h"
#include "corelib/kernel/application.h"

#include "abstract_task_container.h"
#include "abstract_task_loop.h"

namespace cclib{
namespace shell{

using sn::corelib::Terminal;
using sn::corelib::TerminalColor;
using sn::corelib::ErrorInfo;
using sn::corelib::Application;

const int ASCII_NUL = 0;
const int ASCII_ESC = 27;
const int UNICODE_RANGE_START = 0x4E00;
const int UNICODE_RANGE_END = 0x9FA5;

AbstractTaskLoop::AbstractTaskLoop()
   : m_exit(false)
{
   QPair<int, int> winSize = Terminal::getWindowSize();
   m_windowWidth = winSize.first;
   m_windowHeight = winSize.second;
   setupTerminalAttr();
}

bool AbstractTaskLoop::isExitRequest()
{
   return m_exit;
}

AbstractTaskLoop& AbstractTaskLoop::exitRequest()
{
   m_exit = true;
   return *this;
}

AbstractTaskLoop& AbstractTaskLoop::setConsolePsText(const QString &psText)
{
   m_ps = psText;
   m_psLength = calculateCursorStep(m_ps);
   if(!m_cmdBuff.isEmpty()){
      refreshLine(0);
   }
   return *this;
}

const QString& AbstractTaskLoop::getConsolePsText()
{
   return m_ps;
}

void AbstractTaskLoop::setupTerminalAttr()
{
   tcgetattr(STDIN_FILENO, &m_savedTerminalAttr);
   struct termios terminalAttr;
   tcgetattr(STDIN_FILENO,&terminalAttr);
   terminalAttr.c_lflag &= ~ICANON;
   terminalAttr.c_lflag &= ~(ECHO);
   terminalAttr.c_cc[VMIN] = 1;
   terminalAttr.c_cc[VTIME] = 0;
   int status = tcsetattr(STDIN_FILENO, TCSANOW, &terminalAttr);
   Q_ASSERT_X(status == 0, "AbstractTaskLoop::setupTerminalAttr()", "tcsetattr error");
}

AbstractTaskContainer& AbstractTaskLoop::getCurrentTaskContainer()
{
   return *m_currentTaskContainer;
}

AbstractTaskLoop& AbstractTaskLoop::enterGlobalTaskContainer()
{
   enterTaskContainer("Global");
   return *this;
}

AbstractTaskContainer* AbstractTaskLoop::getTaskContainer(const QString &name)
{
   if(!m_taskContainerPool.contains(name)){
      return nullptr;
   }else{
      return m_taskContainerPool.value(name);
   }
}

AbstractTaskLoop& AbstractTaskLoop::enterTaskContainer(const QString& name, const QMap<QString, QString>& args)
{
   if(m_currentTaskContainer == nullptr || 
         (m_currentTaskContainer->getName() == "Global" && name != "Global")||
         (m_currentTaskContainer->getName() != "Global" && name == "Global")){
      if(m_taskContainerPool.contains(name)){
         if(nullptr != m_currentTaskContainer){
            m_currentTaskContainer->unloadHandler();
         }
         m_currentTaskContainer = m_taskContainerPool.value(name);
         m_currentTaskContainer->loadHandler();
         if(!m_historyPool.contains(name)){
            Application* app = sn::corelib::get_application_ref();
            m_historyPool[name] = new History(app->getRuntimeDir()+"/history", name);
         }
      }
   }
   return *this;
}

void AbstractTaskLoop::run()
{
   initCommandContainer();
   QString command;
   Terminal::writeText("welcome to use cloud controller system\n", TerminalColor::Cyan);
   enterGlobalTaskContainer();
   while(true){
      Terminal::writeText(m_ps.toLocal8Bit(), TerminalColor::LightBlue);
      saveCycleBeginCursorPos();
      readCommand(command);
      History* history = m_historyPool[m_currentTaskContainer->getName()];
      if(!command.isEmpty()){
         history->addItem(command);
      }
      runCommand(command);
      if(isExitRequest()){
         break;
      }
   }
}

void AbstractTaskLoop::runCommand(const QString &command)
{
   try{
      m_currentTaskContainer->run(command);
   }catch(ErrorInfo errorInfo){
      Terminal::writeText(errorInfo.toString().toLocal8Bit(), TerminalColor::Red);
   }
}


void AbstractTaskLoop::updateTerminalWindowSize(int width, int height)
{
   m_windowWidth = width;
   m_windowHeight = height;
   refreshLine(0);
}

void AbstractTaskLoop::readCommand(QString &command)
{
   fd_set readFds;
   char buf[128];
   memset(buf, ASCII_NUL, 128);
   int nfds = 1;
   int catchedSigNum = -1;
   FD_ZERO(&readFds);
   FD_SET(STDIN_FILENO, &readFds);
LABEL_AGAIN:
   int ret = select(nfds, &readFds, NULL, NULL, NULL);
   if(-1 == ret){
      if(errno == EINTR && isNeedRestartSelectCall()){
         catchedSigNum = Application::instance()->getCatchedSignalNumber();
         if(SIGUSR1 == catchedSigNum){
            nfds = 1;
            strcpy(buf, "\n");
            FD_ZERO(&readFds);
            goto LABEL_SKIP_CURRENT_INPUT;
         }
         goto LABEL_AGAIN;
      }
      if(errno != EINTR){
         throw ErrorInfo("select error");
      }else{
         ::exit(Application::instance()->getCatchedSignalNumber());
      }
   }
LABEL_SKIP_CURRENT_INPUT:
   for(int fd = 0; fd < nfds; fd++){
      if(FD_ISSET(fd, &readFds)){
         read(STDIN_FILENO, &buf, 127);
         QString unit;
         filterBuffer(buf, unit);
         memset(buf, ASCII_NUL, 128);
         if(!unit.isEmpty()){
            SpecialKeyName keyType = getKeyTypeName(unit);
            readUnitCycle(unit, keyType);
            if(keyType == SpecialKeyName::ASCII_CODE && unit == "\n"){
               History* history = m_historyPool[m_currentTaskContainer->getName()];
               history->resetPointer();
               command = m_cmdBuff.trimmed();
               m_cmdBuff.clear();
               m_insertPos = 0;
               std::cout << "\n";
               return;
            }
         }
         goto LABEL_AGAIN;
      }else if(catchedSigNum == SIGUSR1){
         QString unit;
         filterBuffer(buf, unit);
         memset(buf, ASCII_NUL, 128);
         if(!unit.isEmpty()){
            SpecialKeyName keyType = getKeyTypeName(unit);
            readUnitCycle(unit, keyType);
            if(keyType == SpecialKeyName::ASCII_CODE && unit == "\n"){
               History* history = m_historyPool[m_currentTaskContainer->getName()];
               history->resetPointer();
               command = m_cmdBuff.trimmed();
               m_cmdBuff.clear();
               m_insertPos = 0;
               return;
            }
         }
         catchedSigNum = -1;
         goto LABEL_AGAIN;
      }
   }
}

bool AbstractTaskLoop::isNeedRestartSelectCall()
{
   return true;
}

void AbstractTaskLoop::filterBuffer(char* buffer, QString &ret)
{
   QString tmp(buffer);
   if(tmp.at(0) == ASCII_ESC){
      //匹配命令
      QList<QString> fnKeys = m_specialKeyMap.keys();
      QList<QString>::const_iterator it = fnKeys.cbegin();
      while(it != fnKeys.cend()){
         if(tmp.startsWith(*it)){
            ret = *it;
            break;
         }
         it++;
      }
      return;
   }else{
      int escPos = tmp.indexOf(ASCII_ESC);
      if(-1 == escPos){
         ret = tmp;
      }else{
         ret = tmp.mid(0, escPos);
      }
   }
}

void AbstractTaskLoop::readUnitCycle(QString &unit, SpecialKeyName keyType)
{
   switch(keyType){
   case SpecialKeyName::ARROW_UP:
   case SpecialKeyName::ARROW_DOWN:
   {
      historyCommand(unit, keyType);
      break;
   }
   case SpecialKeyName::ARROW_LEFT:
   case SpecialKeyName::ARROW_RIGHT:
   {
      arrowCommand(unit, keyType);
      break;
   }
   case SpecialKeyName::ASCII_CODE:
   {
      asciiCommand(unit, keyType);
      break;
   }
   case SpecialKeyName::DEL:
   {
      delKeyCommand(unit, keyType);
      break;
   }
   case SpecialKeyName::HOME:
   {
      moveToBeginCommand(unit, keyType);
      break;
   }
   case SpecialKeyName::END:
   {
      moveToEndCommand(unit, keyType);
      break;
   }
   default:
   {
      break;   
   }
   }
}

void AbstractTaskLoop::historyCommand(QString &, SpecialKeyName keyType)
{
   History* history = m_historyPool[m_currentTaskContainer->getName()];
   QString command;
   if(keyType == SpecialKeyName::ARROW_UP){
      command = history->prev();
   }else if(keyType == SpecialKeyName::ARROW_DOWN){
      command = history->next();
   }
   command.remove('\n');
   m_cmdBuff = command;
   m_insertPos = command.size();
   refreshLine(0);
}

void AbstractTaskLoop::arrowCommand(QString&, SpecialKeyName keyType)
{
   if(keyType == SpecialKeyName::ARROW_LEFT){
      if(m_insertPos > 0){
         m_insertPos--;
         QPair<int, int> targetPos = calculateCursorPosByInsertPointer(m_insertPos);
         Terminal::setCursorPos(targetPos.first, targetPos.second);
      }
   }else if(keyType == SpecialKeyName::ARROW_RIGHT){
      if(m_insertPos < m_cmdBuff.size()){
         m_insertPos++;
         QPair<int, int> targetPos = calculateCursorPosByInsertPointer(m_insertPos);
         Terminal::setCursorPos(targetPos.first, targetPos.second);
      }
   }
}

void AbstractTaskLoop::asciiCommand(QString &unit, SpecialKeyName)
{
   if(unit == "\177"){
      removeCharAtCurrentCursorAction();
   }else if(unit != "\n"){
      m_cmdBuff.insert(m_insertPos, unit);
      m_insertPos += unit.size();
      refreshLine(m_insertPos - unit.size());
   }
}

void AbstractTaskLoop::delKeyCommand(QString&, SpecialKeyName)
{
   removeCharAtCurrentCursorAction();
}

void AbstractTaskLoop::moveToBeginCommand(QString&, SpecialKeyName)
{
   Terminal::setCursorPos(m_cycleBeginX, m_cycleBeginY);
   m_insertPos = 0;
}

void AbstractTaskLoop::moveToEndCommand(QString&, SpecialKeyName)
{
   m_insertPos = m_cmdBuff.size();
   QPair<int, int> targetPos = calculateCursorPosByInsertPointer();
   Terminal::setCursorPos(targetPos.first, targetPos.second);
}


void AbstractTaskLoop::refreshLine(int startPointer)
{
   int bufSize = m_cmdBuff.size();
   Q_ASSERT_X(startPointer >= 0 && startPointer <= bufSize, "AbstractTaskLoop::refreshLine", "startPointer out of range");
   QPair<int, int> startCleanPointer = calculateCursorPosByInsertPointer(startPointer);
   Terminal::setCursorPos(startCleanPointer.first, startCleanPointer.second);
   std::cout << "\x1b[J";
   std::cout << m_cmdBuff.mid(startPointer, bufSize - startPointer).toStdString() << std::flush;
   QPair<int, int> targetCursorPos = calculateCursorPosByInsertPointer();
   Terminal::setCursorPos(targetCursorPos.first, targetCursorPos.second);
}


QPair<int, int> AbstractTaskLoop::calculateCursorPosByInsertPointer()
{
   return calculateCursorPosByInsertPointer(m_insertPos);
}

QPair<int, int> AbstractTaskLoop::calculateCursorPosByInsertPointer(int insertPointer)
{
   int winWidth = m_windowWidth;
   QString buf(m_ps + m_cmdBuff);
   int totalBufSize = buf.size();
   int x;
   int y = m_cycleBeginY;
   int cycleRoom = winWidth;
   int hits = 0;
   for(int i = 0; i < totalBufSize; i++){
      int space = getSpaceCountForChar(buf[i]);
      if(cycleRoom >= space){
         cycleRoom-= space;
      }else{
         if(cycleRoom == 0){
            cycleRoom = winWidth - space;
         }
         if(cycleRoom == 1){
            cycleRoom = winWidth - 2;
            hits++;
         }
      }
   }
   int totalSize = m_psLength + calculateCursorStep(m_cmdBuff, insertPointer);
   if(hits > 0){
      x = (totalSize + hits)% winWidth + 1;
      y = m_cycleBeginY + (totalSize + hits) / winWidth;
   }else{
      x = totalSize % winWidth + 1;
      y = m_cycleBeginY + totalSize / winWidth;
   }
   return QPair<int, int>(x, y);
}

AbstractTaskLoop::SpecialKeyName AbstractTaskLoop::getKeyTypeName(QString& unit)
{
   //进行一些简单的处理
   SpecialKeyName keyType = SpecialKeyName::UNKNOW;
   if(unit.at(0) == ASCII_ESC && unit.size() > 1){
      if(m_specialKeyMap.contains(unit)){
         keyType = m_specialKeyMap[unit];
      }
   }else{
      keyType = SpecialKeyName::ASCII_CODE;
   }
   return keyType;
}

void AbstractTaskLoop::removeCharAtCurrentCursorAction()
{
   if(m_insertPos > 0){
      m_insertPos--;
      m_cmdBuff.remove(m_insertPos, 1);
      refreshLine(m_insertPos);
   }
}

int AbstractTaskLoop::calculateCursorStep(const QString &unit)
{
   int step = 0;
   for(int i = 0; i < unit.size(); i++){
      if(isMultiByteChar(unit.at(i))){
         step += 2;
      }else{
         step += 1;
      }
   }
   return step;
}

int AbstractTaskLoop::calculateCursorStep(const QString &unit, int end)
{
   int step = 0;
   for(int i = 0; i < end; i++){
      if(isMultiByteChar(unit.at(i))){
         step += 2;
      }else{
         step += 1;
      }
   }
   return step;
}

bool AbstractTaskLoop::isMultiByteChar(const QChar& unicode)
{
   if(unicode.unicode() >= UNICODE_RANGE_START && unicode.unicode() <= UNICODE_RANGE_END){
      return true;
   }
   return false;
}

int AbstractTaskLoop::getSpaceCountForChar(const QChar& unicode)
{
   if(isMultiByteChar(unicode)){
      return 2;
   }
   return 1;
}

QPair<int, int> AbstractTaskLoop::getCursorPos()
{
   char buf[32] = "\033[6n";
   write(STDOUT_FILENO, buf, sizeof(buf));
   memset(buf, 0, sizeof(buf));
   read (STDIN_FILENO ,buf ,sizeof(buf));
   QString ret(buf);
   QRegularExpression regex("\\[(?P<y>\\d+);(?P<x>\\d+)");
   QRegularExpressionMatch match = regex.match(ret);
   QPair<int, int> pos;
   if(match.hasMatch()){
      pos.first = match.captured("x").toInt();
      pos.second = match.captured("y").toInt();
   }
   return pos;
}

void AbstractTaskLoop::saveCycleBeginCursorPos()
{
   QPair<int, int> pair = getCursorPos();
   m_cycleBeginX = pair.first;
   m_cycleBeginY = pair.second;
}

QPair<int, int> AbstractTaskLoop::getCycleEndCursorPos()
{
   int psSize = calculateCursorStep(m_ps);
   int bufSize = calculateCursorStep(m_cmdBuff);
   int deltaY = (psSize + bufSize) / m_windowWidth;
   int deltaX = ((bufSize + psSize) % m_windowWidth) + 1;
   return QPair<int, int>(deltaX, m_cycleBeginY + deltaY);
}

AbstractTaskLoop::~AbstractTaskLoop()
{
   tcsetattr(STDIN_FILENO, TCSANOW, &m_savedTerminalAttr);
   QMap<QString, AbstractTaskContainer*>::iterator it = m_taskContainerPool.begin();
   while(it != m_taskContainerPool.end()){
      delete it.value();
      it++;
   }
   QMap<QString, History*>::iterator hit = m_historyPool.begin();
   while(hit != m_historyPool.end()){
      delete hit.value();
      hit++;
   }
}

QMap<QString, AbstractTaskLoop::SpecialKeyName> AbstractTaskLoop::m_specialKeyMap{
   {"\x1B[H", AbstractTaskLoop::SpecialKeyName::HOME},
   {"\x1B[5~", AbstractTaskLoop::SpecialKeyName::PAGE_UP},
   {"\x1B[6~", AbstractTaskLoop::SpecialKeyName::PAGE_DOWN},
   {"\x1B[3~", AbstractTaskLoop::SpecialKeyName::DEL},
   {"\x1B[F", AbstractTaskLoop::SpecialKeyName::END},
   {"\x1B[A", AbstractTaskLoop::SpecialKeyName::ARROW_UP},
   {"\x1B[B", AbstractTaskLoop::SpecialKeyName::ARROW_DOWN},
   {"\x1B[C", AbstractTaskLoop::SpecialKeyName::ARROW_RIGHT},
   {"\x1B[D", AbstractTaskLoop::SpecialKeyName::ARROW_LEFT},
};

}//shell
}//cclib