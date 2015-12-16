#include "task_loop.h"
#include "io/terminal.h"
#include <iostream>
#include <cstdio>

namespace cloudcontroller{
namespace shell{

using sn::corelib::Terminal;

TaskLoop::TaskLoop()
{
   setupTerminalAttr();
}

void TaskLoop::setupTerminalAttr()
{
   tcgetattr(STDIN_FILENO, &m_savedTerminalAttr);
   struct termios terminalAttr;
   tcgetattr(STDIN_FILENO,&terminalAttr);
   terminalAttr.c_lflag &= ~ICANON;
//   terminalAttr.c_lflag |= ISIG;
//   terminalAttr.c_lflag |= (ECHOE|IEXTEN);
   terminalAttr.c_lflag &= ~(ECHO);
   terminalAttr.c_cc[VMIN] = 1;
//   terminalAttr.c_cc[VTIME] = 0;
   int status = tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminalAttr);
   Q_ASSERT_X(status == 0, "TaskLoop::setupTerminalAttr()", "tcsetattr error");
}

void TaskLoop::run()
{
   //   Terminal::clearScreen();
   //   Terminal::hideCursor();
   //   Terminal::showCursor();
   char name[256], title[256];
   char c;
   std::cout << "asdfgahsdfjhsgadf >> ";
   int curLine = 1;
   while(c = std::getchar())
   {
//      if(curLine < 5){
//         std::cout << c;
//      }
      std::cout << curLine << std::endl;
      curLine++;
   }

   
     std::cout << "Please, enter your favourite movie: ";
     std::cin.getline (title,256);
   
     std::cout <<  "'s favourite movie is " << title;
   
}

TaskLoop::~TaskLoop()
{
   tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_savedTerminalAttr);
}

}//shell
}//cloudcontroller