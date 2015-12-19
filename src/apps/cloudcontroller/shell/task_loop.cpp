#include <QString>
#include <QByteArray>
#include <QPair>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QList>
#include <QChar>
#include <QDebug>

#include <cstdio>
#include <iostream>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>


#include "task_loop.h"
#include "io/terminal.h"
#include "kernel/errorinfo.h"

namespace cloudcontroller{
namespace shell{

using sn::corelib::Terminal;
using sn::corelib::TerminalColor;
using sn::corelib::ErrorInfo;

const int ASCII_NUL = 0;
const int ASCII_ESC = 27;
const int UNICODE_RANGE_START = 0x4E00;
const int UNICODE_RANGE_END = 0x9FA5;
TaskLoop::TaskLoop()
{
   QPair<int, int> winSize = Terminal::getWindowSize();
   m_windowWidth = winSize.first;
   m_windowHeight = winSize.second;
   setupTerminalAttr();
   m_psLength = calculateCursorStep(m_ps);
}

void TaskLoop::setupTerminalAttr()
{
   tcgetattr(STDIN_FILENO, &m_savedTerminalAttr);
   struct termios terminalAttr;
   tcgetattr(STDIN_FILENO,&terminalAttr);
   terminalAttr.c_lflag &= ~ICANON;
   terminalAttr.c_lflag &= ~(ECHO);
   terminalAttr.c_cc[VMIN] = 1;
   terminalAttr.c_cc[VTIME] = 0;
   int status = tcsetattr(STDIN_FILENO, TCSANOW, &terminalAttr);
   Q_ASSERT_X(status == 0, "TaskLoop::setupTerminalAttr()", "tcsetattr error");
   
}

void TaskLoop::run()
{
   QString command;
   Terminal::writeText("welcome to use cloud controller system\n", TerminalColor::Cyan);
   Terminal::writeText(m_ps.toLocal8Bit(), TerminalColor::LightBlue);
   saveCycleBeginCursorPos();
   readCommand(command);
   while(command != "quit"){
      Terminal::writeText(m_ps.toLocal8Bit(), TerminalColor::LightBlue);
      saveCycleBeginCursorPos();
      readCommand(command);
   }
}

void TaskLoop::updateTerminalWindowSize(int width, int height)
{
   m_windowWidth = width;
   m_windowHeight = height;
   refreshLine(0);
}

void TaskLoop::readCommand(QString &command)
{
   fd_set readFds;
   char buf[128];
   memset(buf, ASCII_NUL, 128);
   int nfds = 1;
   FD_ZERO(&readFds);
   FD_SET(STDIN_FILENO, &readFds);
LABEL_AGAIN:
   int ret = select(nfds, &readFds, NULL, NULL, NULL);
   if(-1 == ret){
      if(errno == EINTR){
         goto LABEL_AGAIN;
      }
      throw ErrorInfo("select error");
   }
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
               command = m_cmdBuff.trimmed();
               m_cmdBuff.clear();
               m_insertPos = 0;
               std::cout << "\n";
               return;
            }
         }
         goto LABEL_AGAIN;
      }
   }
}

void TaskLoop::filterBuffer(char* buffer, QString &ret)
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

void TaskLoop::readUnitCycle(QString &unit, SpecialKeyName keyType)
{
   
   switch(keyType){
   case SpecialKeyName::ARROW_LEFT:
   case SpecialKeyName::ARROW_RIGHT:
   case SpecialKeyName::ARROW_UP:
   case SpecialKeyName::ARROW_DOWN:
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

void TaskLoop::arrowCommand(QString&, SpecialKeyName keyType)
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

void TaskLoop::asciiCommand(QString &unit, SpecialKeyName)
{
   if(unit == "\177"){
      removeCharAtCurrentCursorAction();
   }else if(unit != "\n"){
      m_cmdBuff.insert(m_insertPos, unit);
      m_insertPos += unit.size();
      refreshLine(m_insertPos - unit.size());
   }
}

void TaskLoop::delKeyCommand(QString&, SpecialKeyName)
{
   removeCharAtCurrentCursorAction();
}

void TaskLoop::moveToBeginCommand(QString&, SpecialKeyName)
{
   Terminal::setCursorPos(m_cycleBeginX, m_cycleBeginY);
   m_insertPos = 0;
}

void TaskLoop::moveToEndCommand(QString&, SpecialKeyName)
{
   m_insertPos = m_cmdBuff.size();
   QPair<int, int> targetPos = calculateCursorPosByInsertPointer();
   Terminal::setCursorPos(targetPos.first, targetPos.second);
}


void TaskLoop::refreshLine(int startPointer)
{
      int bufSize = m_cmdBuff.size();
      Q_ASSERT_X(startPointer >= 0 && startPointer <= bufSize, "TaskLoop::refreshLine", "startPointer out of range");
      QPair<int, int> startCleanPointer = calculateCursorPosByInsertPointer(startPointer);
      Terminal::setCursorPos(startCleanPointer.first, startCleanPointer.second);
      std::cout << "\x1b[J";
      std::cout << m_cmdBuff.mid(startPointer, bufSize - startPointer).toStdString() << std::flush;
      QPair<int, int> targetCursorPos = calculateCursorPosByInsertPointer();
      Terminal::setCursorPos(targetCursorPos.first, targetCursorPos.second);
}


QPair<int, int> TaskLoop::calculateCursorPosByInsertPointer()
{
   return calculateCursorPosByInsertPointer(m_insertPos);
}

QPair<int, int> TaskLoop::calculateCursorPosByInsertPointer(int insertPointer)
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

TaskLoop::SpecialKeyName TaskLoop::getKeyTypeName(QString& unit)
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

void TaskLoop::removeCharAtCurrentCursorAction()
{
   if(m_insertPos > 0){
      m_insertPos--;
      m_cmdBuff.remove(m_insertPos, 1);
      refreshLine(m_insertPos);
   }
}

int TaskLoop::calculateCursorStep(const QString &unit)
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

int TaskLoop::calculateCursorStep(const QString &unit, int end)
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

bool TaskLoop::isMultiByteChar(const QChar& unicode)
{
   if(unicode.unicode() >= UNICODE_RANGE_START && unicode.unicode() <= UNICODE_RANGE_END){
      return true;
   }
   return false;
}

int TaskLoop::getSpaceCountForChar(const QChar& unicode)
{
   if(isMultiByteChar(unicode)){
      return 2;
   }
   return 1;
}

QPair<int, int> TaskLoop::getCursorPos()
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

void TaskLoop::saveCycleBeginCursorPos()
{
   QPair<int, int> pair = getCursorPos();
   m_cycleBeginX = pair.first;
   m_cycleBeginY = pair.second;
}

QPair<int, int> TaskLoop::getCycleEndCursorPos()
{
   int psSize = calculateCursorStep(m_ps);
   int bufSize = calculateCursorStep(m_cmdBuff);
   int deltaY = (psSize + bufSize) / m_windowWidth;
   int deltaX = ((bufSize + psSize) % m_windowWidth) + 1;
   return QPair<int, int>(deltaX, m_cycleBeginY + deltaY);
}

TaskLoop::~TaskLoop()
{
   tcsetattr(STDIN_FILENO, TCSANOW, &m_savedTerminalAttr);
}

QMap<QString, TaskLoop::SpecialKeyName> TaskLoop::m_specialKeyMap{
   {"\x1B[H", TaskLoop::SpecialKeyName::HOME},
   {"\x1B[5~", TaskLoop::SpecialKeyName::PAGE_UP},
   {"\x1B[6~", TaskLoop::SpecialKeyName::PAGE_DOWN},
   {"\x1B[3~", TaskLoop::SpecialKeyName::DEL},
   {"\x1B[F", TaskLoop::SpecialKeyName::END},
   {"\x1B[A", TaskLoop::SpecialKeyName::ARROW_UP},
   {"\x1B[B", TaskLoop::SpecialKeyName::ARROW_DOWN},
   {"\x1B[C", TaskLoop::SpecialKeyName::ARROW_RIGHT},
   {"\x1B[D", TaskLoop::SpecialKeyName::ARROW_LEFT},
};

}//shell
}//cloudcontroller