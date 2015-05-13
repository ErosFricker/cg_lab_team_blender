//
//  Util.h
//  Framework
//
//  Created by David Steiner on 3/4/13.
//
//

#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace util
{
    enum LogMode
    {
        LM_INFO,
        LM_WARNING,
        LM_ERROR
    };
    
    void log(const std::string &arg, LogMode mode = LM_INFO);
    
    std::string getFilePath(const std::string &fileName);
    
    bool fileExists(const std::string &fileName);
}

#endif
