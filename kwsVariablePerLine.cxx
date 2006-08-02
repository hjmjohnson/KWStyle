/*=========================================================================

  Program:   KWStyle - Kitware Style Checker
  Module:    kwsVariablePerLine.cxx

  Copyright (c) Kitware, Inc.  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "kwsParser.h"

namespace kws {


/** Check the number of statements per line */
bool Parser::CheckVariablePerLine(unsigned long max)
{
  m_TestsDone[VARIABLEPERLINE] = true;
  char* val = new char[255];
  sprintf(val,"Variables per line = %d max",max);
  m_TestsDescription[VARIABLEPERLINE] = val;
  delete [] val;

  // For the moment only standard types are defined.
  // We might be able to do more with finding typedefs
  #define ntypes 3
  const char* types[ntypes] = {
   {"int"},
   {"unsigned int"},
   {"char"}};

  bool hasError = false;
  for(unsigned int i = 0;i<ntypes;i++)
    {
    std::string typeToFind = types[i];
    typeToFind += " ";
    long int posType = m_BufferNoComment.find(typeToFind,0);
    while(posType != -1)
      {
      // Check that this is the first word
      bool firstWord = false;
      long int pos=posType;
      pos--;
      while((pos>0) && (m_BufferNoComment[pos]==' '))
        {
        pos--;
        }
      if((pos<=0) || (m_BufferNoComment[pos]=='\n'))
        {
        firstWord = true;
        }

      if(firstWord)
        {
        bool ignore = false;
        std::string line = this->GetLine(this->GetLineNumber(posType,false));
        // If we have any '(' or ')' in the line we stop
        if(line.find('(') == -1)
          {
          // This is a very simple check we count the number of coma
          unsigned int vars = 1;
          pos = line.find(',',0);
          while(pos!=-1)
            {
            vars++;
            pos = line.find(',',pos+1);
            }
          
          if(vars > max)
            {
            Error error;
            error.line = this->GetLineNumber(posType,true);
            error.line2 = error.line;
            error.number = VARIABLEPERLINE;
            error.description = "Number of variable per line exceed: ";
            char* val = new char[10];
            sprintf(val,"%d",vars);
            error.description += val;
            error.description += " (max=";
            delete [] val;
            val = new char[10];
            sprintf(val,"%d",max);
            error.description += val;
            error.description += ")";
            delete [] val;
            m_ErrorList.push_back(error);
            hasError = true;
            }
          }
        }// end firstWord
      posType = m_BufferNoComment.find(typeToFind,posType+1);
      }
    }

  return !hasError;
}

} // end namespace kws
